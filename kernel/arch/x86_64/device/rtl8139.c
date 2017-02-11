#include "rtl8139.h"
#include "pci.h"
#include "callout.h"
#include "printk.h"
#include "stdlib.h"
#include "cpu/atomic.h"
#include "mm.h"
#include "cpu/atomic.h"
#include "irq.h"
#include "threadsync.h"
#include "string.h"
#include "bswap.h"

// test
#include "net/dhcp.h"

typedef struct rtl8139_dev_t rtl8139_dev_t;
struct rtl8139_dev_t {
    void *vtbl;

    uintptr_t mmio_physaddr;
    void volatile *mmio;
    void *rx_buffer_physaddr;
    void *tx_buffer_physaddr;
    void *rx_buffer;
    void *tx_buffer;
    int use_msi;
    int irq;

    // Actually 6 bytes
    uint8_t mac_addr[8];

    mutex_t lock;
    condition_var_t tx_avail;
};

static rtl8139_dev_t **rtl8139_devices;
static size_t rtl8139_device_count;

// MAC address (must be 32-bit I/O)
#define RTL8139_IO_IDR_HI       0x00
#define RTL8139_IO_IDR_LO       0x04

// Multicast Address Register (must be 32-bit I/O)
#define RTL8139_IO_MAR_LO       0x08
#define RTL8139_IO_MAR_HI       0x0C

// Tx status descriptor 0-3 (32-bit)
#define RTL8139_IO_TSD_n(n)     (0x10 + ((n) << 2))

// Tx start address descriptor 0-3 (32-bit)
#define RTL8139_IO_TSAD_n(n)    (0x20 + ((n) << 2))

// Rx buffer address descriptor (32-bit)
#define RTL8139_IO_RBSTART      0x30

// Early rx byte count (16-bit)
#define RTL8139_IO_ERBCR        0x34

// Early rx status register (8-bit)
#define RTL8139_IO_ERSR         0x36

// Command register (8-bit)
#define RTL8139_IO_CR           0x37

// Current address of packet read (16-bit)
#define RTL8139_IO_CAPR         0x38

// Current rx buffer address (16-bit)
#define RTL8139_IO_CBR          0x3A

// Interrupt Mask Register (16-bit)
#define RTL8139_IO_IMR          0x3C

// Interrupt Status Register (16-bit)
#define RTL8139_IO_ISR          0x3E

// Tx Configuration Register (32-bit)
#define RTL8139_IO_TCR          0x40

// Rx Configuration Register (32-bit)
#define RTL8139_IO_RCR          0x44

// Timer Count Register (32-bit)
#define RTL8139_IO_TCTR         0x48

// Missed Packet Counter (32-bit)
#define RTL8139_IO_MPC          0x4C

// 93C46 Command Register (8-bit)
#define RTL8139_IO_9346CR       0x50

// Configuration Register 0 (8-bit)
#define RTL8139_IO_CONFIG0      0x51

// Configuration Register 1 (8-bit)
#define RTL8139_IO_CONFIG1      0x52

// Timer Interrupt Register (32-bit)
#define RTL8139_IO_TIMERINT     0x54

// Media Status Register (8-bit)
#define RTL8139_IO_MSR          0x58

// Configuration Register 3 (8-bit)
#define RTL8139_IO_CONFIG3      0x59

// Configuration Register 4 (8-bit)
#define RTL8139_IO_CONFIG4      0x5A

// Multiple Interrupt Select (16-bit)
#define RTL8139_IO_MULINT       0x5C

// PCI Revision ID (8-bit)
#define RTL8139_IO_RERID        0x5E

// Tx Status of All Descriptors (16-bit)
#define RTL8139_IO_TSAD         0x60

// Basic Mode Control Register (16-bit)
#define RTL8139_IO_BMCR         0x62

// Basic Mode Status Register (16-bit)
#define RTL8139_IO_BMSR         0x64

// Auto-Negotiation Address Register (16-bit)
#define RTL8139_IO_ANAR         0x66

// Auto-Negotiation Link Partner Register (16-bit)
#define RTL8139_IO_ANLPAR       0x68

// Auto-Negotiation Expansion Register (16-bit)
#define RTL8139_IO_ANER         0x6A

// Disconnect counter (16-bit)
#define RTL8139_IO_DIS          0x6C

// False Carrier Sense Counter (16-bit)
#define RTL8139_IO_FCSC         0x6E

// N-Way Test Register (16-bit)
#define RTL8139_IO_NWAYTR       0x70

// RX_ER Counter (16-bit)
#define RTL8139_IO_REC          0x72

// CS Configuration Register (16-bit)
#define RTL8139_IO_CSCR         0x74

// PHY Parameter 1 (32-bit)
#define RTL8139_IO_PHY1_PARM    0x78

// Twister Parameter (32-bit)
#define RTL8139_IO_TW_PARM      0x7C

// PHY Parameter 2 (8-bit)
#define RTL8139_IO_PHY2_PARM    0x80

// Power Management CRC register for wakeup frame n (8-bit)
#define RTL8139_IO_CRC_n(n)     (0x84 + (n))

// Power Management wakeup frame0 (64-bit)
#define RTL8139_IO_WAKELO_n(n)  (0x8C + ((n) << 2))
#define RTL8139_IO_WAKEHI_n(n)  (0x90 + ((n) << 2))

// LSB of mask byte of wakeup frame n within offset 12 to 75 (8-bit)
#define RTL8139_IO_LSBCRC0_n(n) (0xCC + (n))

//
// Register bit defines

//
// RTL8139_IO_CR: Command register

#define RTL8139_CR_RST_BIT      4
#define RTL8139_CR_RXEN_BIT     3
#define RTL8139_CR_TXEN_BIT     2
#define RTL8139_CR_RXEMPTY_BIT  0

// Reset
#define RTL8139_CR_RST          (1<<RTL8139_CR_RST_BIT)

// Rx enable
#define RTL8139_CR_RXEN         (1<<RTL8139_CR_RXEN_BIT)

// Tx enable
#define RTL8139_CR_TXEN         (1<<RTL8139_CR_TXEN_BIT)

// Rx buffer empty (read only)
#define RTL8139_CR_RXEMPTY      (1<<RTL8139_CR_RXEMPTY_BIT)

//
// RTL8139_IO_TSD_n: Tx status descriptor 0-3

#define RTL8139_TSD_CRS_BIT     31
#define RTL8139_TSD_TABT_BIT    30
#define RTL8139_TSD_OWC_BIT     29
#define RTL8139_TSD_CDH_BIT     28
#define RTL8139_TSD_NCC_BIT     24
#define RTL8139_TSD_ERTXTH_BIT  16
#define RTL8139_TSD_TOK_BIT     15
#define RTL8139_TSD_TUN_BIT     14
#define RTL8139_TSD_OWN_BIT     13
#define RTL8139_TSD_SIZE_BIT    0

#define RTL8139_TSD_NCC_BITS    4
#define RTL8139_TSD_ERTXTH_BITS 6
#define RTL8139_TSD_SIZE_BITS   13

#define RTL8139_TSD_NCC_MASK    ((1<<RTL8139_TSD_NCC_BITS)-1)
#define RTL8139_TSD_ERTXTH_MASK ((1<<RTL8139_TSD_ERTXTH_BITS)-1)
#define RTL8139_TSD_SIZE_MASK   ((1<<RTL8139_TSD_SIZE_BITS)-1)

// Carrier sense lost (read only)
#define RTL8139_TSD_CRS         (1<<RTL8139_TSD_CRS_BIT)

// Transmit aborted (read only)
#define RTL8139_TSD_TABT        (1<<RTL8139_TSD_TABT_BIT)

// Out of window collision (read only)
#define RTL8139_TSD_OWC         (1<<RTL8139_TSD_OWC_BIT)

// CD Heart Beat (10Mbps only) (read only)
#define RTL8139_TSD_CDH         (1<<RTL8139_TSD_CDH_BIT)

// Transmit OK (read only)
#define RTL8139_TSD_TOK         (1<<RTL8139_TSD_TOK_BIT)

// Transmit FIFO underrun (read only)
#define RTL8139_TSD_TUN         (1<<RTL8139_TSD_TUN_BIT)

// Set to 0 if NIC is using descriptor,
// Set to 1 if ready for driver to provide another packet
// Must write 0 to this bit to start tx
#define RTL8139_TSD_OWN         (1<<RTL8139_TSD_OWN_BIT)

// Number of tx collisions count
#define RTL8139_TSD_NCC \
    (RTL8139_TSD_NCC_MASK<<RTL8139_TSD_NCC_BIT)

// Early tx threshold (in units of 8 bytes, 0=8 bytes, max 2KB)
#define RTL8139_TSD_ERTXTH \
    (RTL8139_TSD_ERTXTH_MASK<<RTL8139_TSD_ERTXTH_BIT)

// Tx packet size
#define RTL8139_TSD_SIZE \
    (RTL8139_TSD_SIZE_MASK<<RTL8139_TSD_SIZE_BIT)

//
// RTL8139_IO_IMR and RTL8139_IO_ISR: Interrupt Mask/Status Registers

#define RTL8139_IxR_SERR_BIT        15
#define RTL8139_IxR_TIMEOUT_BIT     14
#define RTL8139_IxR_LENCHG_BIT      13
#define RTL8139_IxR_FOVW_BIT        6
#define RTL8139_IxR_PUNLC_BIT       5
#define RTL8139_IxR_RXOVW_BIT       4
#define RTL8139_IxR_TER_BIT         3
#define RTL8139_IxR_TOK_BIT         2
#define RTL8139_IxR_RER_BIT         1
#define RTL8139_IxR_ROK_BIT         0

// System error
#define RTL8139_IxR_SERR            (1<<RTL8139_IxR_SERR_BIT)

// Timeout
#define RTL8139_IxR_TIMEOUT         (1<<RTL8139_IxR_TIMEOUT_BIT)

// Cable length change
#define RTL8139_IxR_LENCHG          (1<<RTL8139_IxR_LENCHG_BIT)

// Rx FIFO Overflow
#define RTL8139_IxR_FOVW            (1<<RTL8139_IxR_FOVW_BIT)

// Packet underrun / link change
#define RTL8139_IxR_PUNLC           (1<<RTL8139_IxR_PUNLC_BIT)

// Rx overflow
#define RTL8139_IxR_RXOVW           (1<<RTL8139_IxR_RXOVW_BIT)

// Transmit error
#define RTL8139_IxR_TER             (1<<RTL8139_IxR_TER_BIT)

// Transmit OK
#define RTL8139_IxR_TOK             (1<<RTL8139_IxR_TOK_BIT)

// Rx error
#define RTL8139_IxR_RER             (1<<RTL8139_IxR_RER_BIT)

// Rx OK
#define RTL8139_IxR_ROK             (1<<RTL8139_IxR_ROK_BIT)

//
// RTL8139_IO_RCR: Rx Configuration Register (32-bit)

#define RTL8139_RCR_ERTH_BIT        24
#define RTL8139_RCR_MULERINT_BIT    17
#define RTL8139_RCR_RER8_BIT        16
#define RTL8139_RCR_RXFTH_BIT       13
#define RTL8139_RCR_RBLEN_BIT       11
#define RTL8139_RCR_MXDMA_BIT       8
#define RTL8139_RCR_WRAP_BIT        7
#define RTL8139_RCR_AER_BIT         5
#define RTL8139_RCR_AR_BIT          4
#define RTL8139_RCR_AB_BIT          3
#define RTL8139_RCR_AM_BIT          2
#define RTL8139_RCR_APM_BIT         1
#define RTL8139_RCR_AAP_BIT         0

#define RTL8139_RCR_ERTH_BITS       4
#define RTL8139_RCR_RXFTH_BITS      3
#define RTL8139_RCR_RBLEN_BITS      2
#define RTL8139_RCR_MXDMA_BITS      3

#define RTL8139_RCR_ERTH_MASK       ((1<<RTL8139_RCR_ERTH_BITS)-1)
#define RTL8139_RCR_RXFTH_MASK      ((1<<RTL8139_RCR_RXFTH_BITS)-1)
#define RTL8139_RCR_RBLEN_MASK      ((1<<RTL8139_RCR_RBLEN_BITS)-1)
#define RTL8139_RCR_MXDMA_MASK      ((1<<RTL8139_RCR_MXDMA_BITS)-1)

#define RTL8139_RCR_ERTH \
    (RTL8139_RCR_ERTH_MASK<<RTL8139_RCR_ERTH_BIT)

#define RTL8139_RCR_RXFTH \
    (RTL8139_RCR_RXFTH_MASK<<RTL8139_RCR_RXFTH_BIT)

#define RTL8139_RCR_RBLEN \
    (RTL8139_RCR_RBLEN_MASK<<RTL8139_RCR_RBLEN_BIT)

#define RTL8139_RCR_MXDMA \
    (RTL8139_RCR_MXDMA_MASK<<RTL8139_RCR_MXDMA_BIT)

// Multiple early interrupt select
// 0 = Invoke early interrupt for unknown
//      protocols using RCR<ERTH[3:0]>
// 1 = Invoke early interrupt for known
//      protocols using MULINT<MISR[11:0]>
#define RTL8139_RCR_MULERINT        (1<<RTL8139_RCR_MULERINT_BIT)

// 0 = Receive error packets larger than 8 bytes
// 1 = Receive error packets larger than 64 bytes (default)
#define RTL8139_RCR_RER8            (1<<RTL8139_RCR_RER8_BIT)

// 0 = Wrap at end of buffer,
// 1 = overflow up to 1500 bytes past end
#define RTL8139_RCR_WRAP            (1<<RTL8139_RCR_WRAP_BIT)

// Accept error packets
#define RTL8139_RCR_AER             (1<<RTL8139_RCR_AER_BIT)

// Accept runt packets
#define RTL8139_RCR_AR              (1<<RTL8139_RCR_AR_BIT)

// Accept broadcast packets
#define RTL8139_RCR_AB              (1<<RTL8139_RCR_AB_BIT)

// Accept multicast packets
#define RTL8139_RCR_AM              (1<<RTL8139_RCR_AM_BIT)

// Accept physical match packets
#define RTL8139_RCR_APM             (1<<RTL8139_RCR_APM_BIT)

// Accept all packets
#define RTL8139_RCR_AAP             (1<<RTL8139_RCR_AAP_BIT)

// Early Rx threshold (n/16th, or, 0=none)
#define RTL8139_RCR_ERTH_n(n)   ((n)<<RTL8139_RCR_ERTH_BIT)

// Rx FIFO threshold (1<<(n+4), or 7=none)
#define RTL8139_RCR_RXFTH_n(n)  ((n)<<RTL8139_RCR_RXFTH_BIT)

// Rx Buffer Length (0=8K+16, 1=16K+16, 2=32K+16, 3=64K+16)
#define RTL8139_RCR_RBLEN_n(n)  ((n)<<RTL8139_RCR_RBLEN_BIT)

// Max DMA burst per Rx DMA burst (1<<(n+4), or 7=unlimited)
#define RTL8139_RCR_MXDMA_n(n)  ((n)<<RTL8139_RCR_MXDMA_BIT)

//
// RTL8139_IO_TCR: Tx Configuration Register (32-bit)

#define RTL8139_TCR_HWVERID_A_BIT   26
#define RTL8139_TCR_IFG_BIT         24
#define RTL8139_TCR_HWVERID_B_BIT   22
#define RTL8139_TCR_LBK_BIT         17
#define RTL8139_TCR_CRC_BIT         16
#define RTL8139_TCR_MXDMA_BIT       8
#define RTL8139_TCR_TXRR_BIT        4
#define RTL8139_TCR_CLRABT_BIT      0

#define RTL8139_TCR_HWVERID_A_BITS  5
#define RTL8139_TCR_IFG_BITS        2
#define RTL8139_TCR_HWVERID_B_BITS  2
#define RTL8139_TCR_LBK_BITS        2
#define RTL8139_TCR_MXDMA_BITS      3
#define RTL8139_TCR_TXRR_BITS       4

#define RTL8139_TCR_HWVERID_A_MASK  ((1<<RTL8139_TCR_HWVERID_A_BITS)-1)
#define RTL8139_TCR_IFG_MASK        ((1<<RTL8139_TCR_IFG_BITS)-1)
#define RTL8139_TCR_HWVERID_B_MASK  ((1<<RTL8139_TCR_HWVERID_B_BITS)-1)
#define RTL8139_TCR_LBK_MASK        ((1<<RTL8139_TCR_LBK_BITS)-1)
#define RTL8139_TCR_MXDMA_MASK      ((1<<RTL8139_TCR_MXDMA_BITS)-1)
#define RTL8139_TCR_TXRR_MASK       ((1<<RTL8139_TCR_TXRR_BITS)-1)

#define RTL8139_TCR_HWVERID_A \
    (RTL8139_TCR_HWVERID_A_MASK<<RTL8139_TCR_HWVERID_A_BIT)

#define RTL8139_TCR_IFG \
    (RTL8139_TCR_IFG_MASK<<RTL8139_TCR_IFG_BIT)

#define RTL8139_TCR_HWVERID_B \
    (RTL8139_TCR_HWVERID_B_MASK<<RTL8139_TCR_HWVERID_B_BIT)

#define RTL8139_TCR_LBK \
    (RTL8139_TCR_LBK_MASK<<RTL8139_TCR_LBK_BIT)

#define RTL8139_TCR_MXDMA \
    (RTL8139_TCR_MXDMA_MASK<<RTL8139_TCR_MXDMA_BIT)
#define RTL8139_TCR_TXRR \
    (RTL8139_TCR_TXRR_MASK<<RTL8139_TCR_TXRR_BIT)

// Append CRC
#define RTL8139_TCR_CRC             (1<<RTL8139_TCR_CRC_BIT)

// Clear abort (only write 1 if in tx abort state)
#define RTL8139_TCR_CLRABT          (1<<RTL8139_TCR_CLRABT_BIT)

// Hardware version ID A
#define RTL8139_TCR_HWVERID_A_n(n)  ((n)<<RTL8139_TCR_HWVERID_A_BIT)

// Interframe gap
#define RTL8139_TCR_IFG_n(n)        ((n)<<RTL8139_TCR_IFG_BIT)

// Hardware version ID B
#define RTL8139_TCR_HWVERID_B_n(n)  ((n)<<RTL8139_TCR_HWVERID_B_BIT)

// Loopback test
#define RTL8139_TCR_LBK_n(n)        ((n)<<RTL8139_TCR_LBK_BIT)

// Max tx DMA burst (1<<(n+4))
#define RTL8139_TCR_MXDMA_n(n)      ((n)<<RTL8139_TCR_MXDMA_BIT)

// Tx retry count (16+n*16)
#define RTL8139_TCR_TXRR_n(n)       ((n)<<RTL8139_TCR_TXRR_BIT)

//
// MMIO

#define RTL8139_MMIO_PTR(ptr, type, reg) \
    (*(type volatile *)((char*)(ptr)->mmio + reg))

#define RTL8139_MMIO(type, reg)     RTL8139_MMIO_PTR(self, type, reg)

#define RTL8139_MM_WR_8(reg, val) \
    rtl8139_mm_out_8(self, reg, val)

#define RTL8139_MM_WR_16(reg, val) \
    rtl8139_mm_out_16(self, reg, val)

#define RTL8139_MM_WR_32(reg, val) \
    rtl8139_mm_out_32(self, reg, val)

#define RTL8139_MM_RD_8(reg) \
    rtl8139_mm_in_8(self, reg)

#define RTL8139_MM_RD_16(reg) \
    rtl8139_mm_in_16(self, reg)

#define RTL8139_MM_RD_32(reg) \
    rtl8139_mm_in_32(self, reg)

__attribute__((used))
static inline void rtl8139_mm_out_8(rtl8139_dev_t *self,
                             uint32_t reg, uint8_t val)
{
    RTL8139_MMIO(uint8_t, reg) = val;
    atomic_barrier();
}

__attribute__((used))
static inline void rtl8139_mm_out_16(rtl8139_dev_t *self,
                              uint32_t reg, uint16_t val)
{
    RTL8139_MMIO(uint16_t, reg) = val;
    atomic_barrier();
}

__attribute__((used))
static inline void rtl8139_mm_out_32(rtl8139_dev_t *self,
                              uint32_t reg, uint32_t val)
{
    RTL8139_MMIO(uint32_t, reg) = val;
    atomic_barrier();
}

__attribute__((used))
static inline uint8_t rtl8139_mm_in_8(rtl8139_dev_t *self,
                                      uint32_t reg)
{
    atomic_barrier();
    return RTL8139_MMIO(uint8_t, reg);
}

__attribute__((used))
static inline uint16_t rtl8139_mm_in_16(rtl8139_dev_t *self,
                                        uint32_t reg)
{
    atomic_barrier();
    return RTL8139_MMIO(uint16_t, reg);
}

__attribute__((used))
static inline uint32_t rtl8139_mm_in_32(rtl8139_dev_t *self,
                                        uint32_t reg)
{
    atomic_barrier();
    return RTL8139_MMIO(uint32_t, reg);
}

//
// IRQ handler

static void *rtl8139_irq_handler(int irq, void *ctx)
{
    for (size_t i = 0; i < rtl8139_device_count; ++i) {
        rtl8139_dev_t *self = rtl8139_devices[i];

        if (irq != self->irq)
            continue;

        uint16_t isr = RTL8139_MM_RD_16(RTL8139_IO_ISR);

        printdbg("RTL8139 IRQ status = %x\n", isr);

        if (isr & RTL8139_IxR_SERR)
            printdbg("RTL8139 IRQ: System Error\n");
        if (isr & RTL8139_IxR_TOK)
            printdbg("RTL8139 IRQ: Tx OK\n");
        if (isr & RTL8139_IxR_ROK)
            printdbg("RTL8139 IRQ: Rx OK\n");
        if (isr & RTL8139_IxR_FOVW)
            printdbg("RTL8139 IRQ: Rx FIFO overflow\n");
        if (isr & RTL8139_IxR_TER)
            printdbg("RTL8139 IRQ: Tx Error\n");
        if (isr & RTL8139_IxR_RER)
            printdbg("RTL8139 IRQ: Rx Error\n");
        if (isr & RTL8139_IxR_RXOVW)
            printdbg("RTL8139 IRQ: Rx Overflow Error\n");
        if (isr & RTL8139_IxR_SERR)
            printdbg("RTL8139 IRQ: System Error\n");

        // Acknowledge IRQ
        RTL8139_MM_WR_16(RTL8139_IO_ISR, isr);
    }

    return ctx;
}

//
// Initialization

static void rtl8139_detect(void)
{
    pci_dev_iterator_t pci_iter;

    pci_enumerate_begin(&pci_iter,
                        PCI_DEV_CLASS_NETWORK,
                        PCI_SUBCLASS_NETWORK_ETHERNET);

    do {
        if (pci_iter.config.vendor != 0x10EC ||
                pci_iter.config.device != 0x8139)
            continue;

        printdbg("Detected RTL8139 %d/%d/%d"
                 " %x %x %x %x %x %x irq=%d\n",
                 pci_iter.bus,
                 pci_iter.slot,
                 pci_iter.func,
                 pci_iter.config.base_addr[0],
                 pci_iter.config.base_addr[1],
                 pci_iter.config.base_addr[2],
                 pci_iter.config.base_addr[3],
                 pci_iter.config.base_addr[4],
                 pci_iter.config.base_addr[5],
                 pci_iter.config.irq_line);

        rtl8139_dev_t *self = calloc(1, sizeof(rtl8139_dev_t));

        rtl8139_devices = realloc(rtl8139_devices,
                                  sizeof(rtl8139_devices) *
                                  (rtl8139_device_count + 1));
        rtl8139_devices[rtl8139_device_count++] = self;

        mutex_init(&self->lock);
        condvar_init(&self->tx_avail);

        self->mmio_physaddr = pci_iter.config.base_addr[1] & -16;

        self->mmio = mmap((void*)self->mmio_physaddr, 256,
                          PROT_READ | PROT_WRITE,
                          MAP_PHYSICAL, -1, 0);

        // Enable MMIO and bus master, disable port I/O
        pci_adj_control_bits(pci_iter.bus, pci_iter.slot,
                             pci_iter.func,
                             PCI_CMD_BUSMASTER |
                             PCI_CMD_MEMEN,
                             PCI_CMD_IOEN);

        // Power on
        RTL8139_MM_WR_8(RTL8139_IO_CONFIG1, 0);

        // Reset
        RTL8139_MM_WR_8(RTL8139_IO_CR, RTL8139_CR_RST);

        // Allocate contiguous rx buffer
        self->rx_buffer_physaddr = mm_alloc_contiguous(16384 + 16);

        // Allocate contiguous rx buffer
        self->tx_buffer_physaddr = mm_alloc_contiguous(1536*4);

        // Map rx buffer physical memory
        self->rx_buffer = mmap(self->rx_buffer_physaddr, 16384 + 16,
                               PROT_READ | PROT_WRITE, MAP_PHYSICAL,
                               -1, 0);

        // Map tx buffer physical memory
        self->tx_buffer = mmap(self->tx_buffer_physaddr, 1536*4,
                               PROT_READ | PROT_WRITE, MAP_PHYSICAL,
                               -1, 0);

        // Wait for reset to finish
        while ((RTL8139_MM_RD_8(RTL8139_IO_CR) &
                RTL8139_CR_RST) != 0)
            pause();

        // Read MAC address
        uint32_t mac_hi;
        uint32_t mac_lo;
        mac_hi = rtl8139_mm_in_32(self, RTL8139_IO_IDR_HI);
        mac_lo = rtl8139_mm_in_32(self, RTL8139_IO_IDR_LO);

        //mac_hi = htonl(mac_hi);
        //mac_lo = htons(mac_lo);

        memcpy(self->mac_addr + 4, &mac_lo, sizeof(uint16_t));
        memcpy(self->mac_addr, &mac_hi, sizeof(uint32_t));

        // Restore BIOS assigned IRQ
        pci_irq_range_t irq_range = {
            pci_iter.config.irq_line,
            1
        };

        // Use MSI IRQ if possible
        self->use_msi = pci_set_msi_irq(
                    pci_iter.bus, pci_iter.slot, pci_iter.func,
                    &irq_range, 1, 0, 0, rtl8139_irq_handler);

        self->irq = irq_range.base;

        if (!self->use_msi) {
            // Plain IRQ pin
            pci_set_irq_pin(pci_iter.bus, pci_iter.slot,
                            pci_iter.func,
                            pci_iter.config.irq_pin);
            pci_set_irq_line(pci_iter.bus, pci_iter.slot,
                             pci_iter.func,
                             pci_iter.config.irq_line);

            irq_hook(pci_iter.config.irq_line, rtl8139_irq_handler);
            irq_setmask(pci_iter.config.irq_line, 1);
        }

        // Set rx buffer physical address
        RTL8139_MM_WR_32(RTL8139_IO_RBSTART,
                         (uint32_t)self->rx_buffer_physaddr);

        // Set tx buffer physical addresses
        for (unsigned i = 0; i < 4; ++i) {
            RTL8139_MM_WR_32(RTL8139_IO_TSAD_n(i),
                             (uint32_t)self->tx_buffer_physaddr +
                             i * 1536);
        }

        // Enable rx and tx
        RTL8139_MM_WR_8(RTL8139_IO_CR,
                        RTL8139_CR_RXEN |
                        RTL8139_CR_TXEN);

        // Rx configuration
        RTL8139_MM_WR_32(RTL8139_IO_RCR,
                         RTL8139_RCR_ERTH_n(0) |
                         RTL8139_RCR_RXFTH_n(7) |
                         RTL8139_RCR_RBLEN_n(1) |
                         RTL8139_RCR_MXDMA_n(7) |
                         RTL8139_RCR_WRAP |
                         RTL8139_RCR_AB |
                         RTL8139_RCR_AM |
                         RTL8139_RCR_APM);

        // Tx configuration
        RTL8139_MM_WR_32(RTL8139_IO_TCR,
                         RTL8139_TCR_CRC |
                         RTL8139_TCR_IFG_n(3) |
                         RTL8139_TCR_TXRR_n(0) |
                         RTL8139_TCR_MXDMA_n(7) |
                         RTL8139_TCR_LBK_n(0));

        uint16_t unmask =
                RTL8139_IxR_SERR |
                RTL8139_IxR_TIMEOUT |
                RTL8139_IxR_FOVW |
                RTL8139_IxR_PUNLC |
                RTL8139_IxR_RXOVW |
                RTL8139_IxR_TER |
                RTL8139_IxR_TOK |
                RTL8139_IxR_RER |
                RTL8139_IxR_ROK;

        // Acknowledge IRQs
        RTL8139_MM_WR_16(RTL8139_IO_ISR, unmask);

        // Unmask IRQs
        RTL8139_MM_WR_16(RTL8139_IO_IMR, unmask);
    } while (pci_enumerate_next(&pci_iter));
}

static void rtl8139_tx_packet(rtl8139_dev_t *self,
                              int slot,
                              void *pkt, size_t size)
{
    assert(slot < 3);
    memcpy((char*)self->tx_buffer + 1536*slot, pkt, size);
    atomic_barrier();

    RTL8139_MM_WR_32(RTL8139_IO_TSD_n(slot), size);
}

static void rtl8139_startup_hack(void *p)
{
    (void)p;
    rtl8139_detect();

    if (rtl8139_device_count == 0)
        return;

    rtl8139_dev_t *self = rtl8139_devices[0];

    // Send a DHCP discover
    dhcp_pkt_t discover;

    memset(&discover, 0, sizeof(discover));

    //
    // Ethernet

    // Destination MAC FF:FF:FF:FF:FF:FF
    memset(discover.udp_hdr.ipv4_hdr.eth_hdr.d_mac, 0xFF,
           sizeof(discover.udp_hdr.ipv4_hdr.eth_hdr.d_mac));

    // Source MAC from NIC
    memcpy(discover.udp_hdr.ipv4_hdr.eth_hdr.s_mac,
           self->mac_addr,
           sizeof(discover.udp_hdr.ipv4_hdr.eth_hdr.s_mac));

    // Ethernet length field
    discover.udp_hdr.ipv4_hdr.eth_hdr.len_ethertype = htons(0x800);
            //htons(sizeof(discover)-sizeof(discover.udp_hdr.ipv4_hdr.eth_hdr));

    //
    // IPv4

    // IPv4 destination IP 255.255.255.255
    memset(discover.udp_hdr.ipv4_hdr.d_ip, 0xFF,
           sizeof(discover.udp_hdr.ipv4_hdr.d_ip));

    discover.udp_hdr.ipv4_hdr.ver_ihl = 0x45;
    discover.udp_hdr.ipv4_hdr.dscp_ecn = 0;
    discover.udp_hdr.ipv4_hdr.len = htons(sizeof(discover) -
            sizeof(discover.udp_hdr.ipv4_hdr.eth_hdr));
    discover.udp_hdr.ipv4_hdr.id = 0;
    discover.udp_hdr.ipv4_hdr.flags_fragofs = 0;
    discover.udp_hdr.ipv4_hdr.ttl = 64;
    discover.udp_hdr.ipv4_hdr.protocol = IPV4_PROTO_UDP;
    discover.udp_hdr.ipv4_hdr.hdr_checksum =
            ipv4_checksum(&discover.udp_hdr.ipv4_hdr, 0);

    // 0.0.0.0
    memset(discover.udp_hdr.ipv4_hdr.s_ip, 0,
           sizeof(discover.udp_hdr.ipv4_hdr.s_ip));

    // 255.255.255.255
    memset(discover.udp_hdr.ipv4_hdr.d_ip, 255,
           sizeof(discover.udp_hdr.ipv4_hdr.d_ip));

    //
    // UDP

    discover.udp_hdr.d_port = htons(67);
    discover.udp_hdr.s_port = htons(68);
    discover.udp_hdr.len = htons(sizeof(discover) -
            sizeof(discover.udp_hdr.ipv4_hdr));
    discover.udp_hdr.checksum =
            udp_checksum(&discover.udp_hdr.ipv4_hdr);

    //
    // DHCP

    discover.op = 1;
    discover.htype = 1;
    discover.hlen = 6;
    discover.hops = 0;
    discover.xid = htonl(0x3903f326);
    discover.secs = 0;
    discover.flags = htons(0x8000);
    memcpy(discover.ch_addr, self->mac_addr, discover.hlen);
    discover.magic_cookie = htonl(0x63825363);

    int opt = 0;
    discover.options[opt++] = 53;
    discover.options[opt++] = 1;
    discover.options[opt++] = 1;
    discover.options[opt++] = 0;
    discover.options[opt++] = 55;
    discover.options[opt++] = 4;
    discover.options[opt++] = 1;
    discover.options[opt++] = 3;
    discover.options[opt++] = 15;
    discover.options[opt++] = 6;
    discover.options[opt++] = 255;

    rtl8139_tx_packet(self, 0, &discover, sizeof(discover));

    //rtl8139_tx_packet(self, 1, &discover, sizeof(discover));
}

REGISTER_CALLOUT(rtl8139_startup_hack, 0, 'L', "000");
