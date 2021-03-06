// THIS FILE IS AUTOMATICALLY GENERATED
// from kernel/device/es137x/es137x.bits

//
// ES1370_CONTROL: R/W: Interrupt/Chip select control register
#define ES1370_CONTROL                    0x00

#define ES1370_CONTROL_ADC_STOP_BIT       31
#define ES1370_CONTROL_XCTL1_BIT          30
#define ES1370_CONTROL_PCLKDIV_BIT        16
#define ES1370_CONTROL_MSFMTSEL_BIT       15
#define ES1370_CONTROL_SBB_BIT            14
#define ES1370_CONTROL_WTSRSEL_BIT        12
#define ES1370_CONTROL_DAC_SYNC_BIT       11
#define ES1370_CONTROL_CCB_INTE_BIT       10
#define ES1370_CONTROL_M_CB_BIT           9
#define ES1370_CONTROL_XCTL0_BIT          8
#define ES1370_CONTROL_BREQ_BIT           7
#define ES1370_CONTROL_DAC1EN_BIT         6
#define ES1370_CONTROL_DAC2EN_BIT         5
#define ES1370_CONTROL_ADCEN_BIT          4
#define ES1370_CONTROL_UARTEN_BIT         3
#define ES1370_CONTROL_JOYEN_BIT          2
#define ES1370_CONTROL_CDC_EN_BIT         1
#define ES1370_CONTROL_SERR_DIS_BIT       0

#define ES1370_CONTROL_ADC_STOP_BITS      1
#define ES1370_CONTROL_XCTL1_BITS         1
#define ES1370_CONTROL_PCLKDIV_BITS       13
#define ES1370_CONTROL_MSFMTSEL_BITS      1
#define ES1370_CONTROL_SBB_BITS           1
#define ES1370_CONTROL_WTSRSEL_BITS       2
#define ES1370_CONTROL_DAC_SYNC_BITS      1
#define ES1370_CONTROL_CCB_INTE_BITS      1
#define ES1370_CONTROL_M_CB_BITS          1
#define ES1370_CONTROL_XCTL0_BITS         1
#define ES1370_CONTROL_BREQ_BITS          1
#define ES1370_CONTROL_DAC1EN_BITS        1
#define ES1370_CONTROL_DAC2EN_BITS        1
#define ES1370_CONTROL_ADCEN_BITS         1
#define ES1370_CONTROL_UARTEN_BITS        1
#define ES1370_CONTROL_JOYEN_BITS         1
#define ES1370_CONTROL_CDC_EN_BITS        1
#define ES1370_CONTROL_SERR_DIS_BITS      1
#define ES1370_CONTROL_ADC_STOP_MASK \
    ((1U << ES1370_CONTROL_ADC_STOP_BITS)-1)
#define ES1370_CONTROL_XCTL1_MASK         ((1U << ES1370_CONTROL_XCTL1_BITS)-1)
#define ES1370_CONTROL_PCLKDIV_MASK \
    ((1U << ES1370_CONTROL_PCLKDIV_BITS)-1)
#define ES1370_CONTROL_MSFMTSEL_MASK \
    ((1U << ES1370_CONTROL_MSFMTSEL_BITS)-1)
#define ES1370_CONTROL_SBB_MASK           ((1U << ES1370_CONTROL_SBB_BITS)-1)
#define ES1370_CONTROL_WTSRSEL_MASK \
    ((1U << ES1370_CONTROL_WTSRSEL_BITS)-1)
#define ES1370_CONTROL_DAC_SYNC_MASK \
    ((1U << ES1370_CONTROL_DAC_SYNC_BITS)-1)
#define ES1370_CONTROL_CCB_INTE_MASK \
    ((1U << ES1370_CONTROL_CCB_INTE_BITS)-1)
#define ES1370_CONTROL_M_CB_MASK          ((1U << ES1370_CONTROL_M_CB_BITS)-1)
#define ES1370_CONTROL_XCTL0_MASK         ((1U << ES1370_CONTROL_XCTL0_BITS)-1)
#define ES1370_CONTROL_BREQ_MASK          ((1U << ES1370_CONTROL_BREQ_BITS)-1)
#define ES1370_CONTROL_DAC1EN_MASK \
    ((1U << ES1370_CONTROL_DAC1EN_BITS)-1)
#define ES1370_CONTROL_DAC2EN_MASK \
    ((1U << ES1370_CONTROL_DAC2EN_BITS)-1)
#define ES1370_CONTROL_ADCEN_MASK         ((1U << ES1370_CONTROL_ADCEN_BITS)-1)
#define ES1370_CONTROL_UARTEN_MASK \
    ((1U << ES1370_CONTROL_UARTEN_BITS)-1)
#define ES1370_CONTROL_JOYEN_MASK         ((1U << ES1370_CONTROL_JOYEN_BITS)-1)
#define ES1370_CONTROL_CDC_EN_MASK \
    ((1U << ES1370_CONTROL_CDC_EN_BITS)-1)
#define ES1370_CONTROL_SERR_DIS_MASK \
    ((1U << ES1370_CONTROL_SERR_DIS_BITS)-1)

// disable capture buffer transfers
#define ES1370_CONTROL_ADC_STOP \
    (ES1370_CONTROL_ADC_STOP_MASK << ES1370_CONTROL_ADC_STOP_BIT)

// general purpose output bit
#define ES1370_CONTROL_XCTL1 \
    (ES1370_CONTROL_XCTL1_MASK << ES1370_CONTROL_XCTL1_BIT)

// clock divide ratio for DAC2
#define ES1370_CONTROL_PCLKDIV \
    (ES1370_CONTROL_PCLKDIV_MASK << ES1370_CONTROL_PCLKDIV_BIT)

// MPEG serial data format; 0 = SONY, 1 = I2S
#define ES1370_CONTROL_MSFMTSEL \
    (ES1370_CONTROL_MSFMTSEL_MASK << ES1370_CONTROL_MSFMTSEL_BIT)

// clock source for DAC - 0 = clock generator; 1 = MPEG clocks
#define ES1370_CONTROL_SBB \
    (ES1370_CONTROL_SBB_MASK << ES1370_CONTROL_SBB_BIT)

// fixed frequency clock for DAC1
#define ES1370_CONTROL_WTSRSEL \
    (ES1370_CONTROL_WTSRSEL_MASK << ES1370_CONTROL_WTSRSEL_BIT)

// DAC's are synchronous
#define ES1370_CONTROL_DAC_SYNC \
    (ES1370_CONTROL_DAC_SYNC_MASK << ES1370_CONTROL_DAC_SYNC_BIT)

// CCB voice interrupts enable
#define ES1370_CONTROL_CCB_INTE \
    (ES1370_CONTROL_CCB_INTE_MASK << ES1370_CONTROL_CCB_INTE_BIT)

// capture clock source; 0 = ADC; 1 = MPEG
#define ES1370_CONTROL_M_CB \
    (ES1370_CONTROL_M_CB_MASK << ES1370_CONTROL_M_CB_BIT)

// generap purpose output bit
#define ES1370_CONTROL_XCTL0 \
    (ES1370_CONTROL_XCTL0_MASK << ES1370_CONTROL_XCTL0_BIT)

// memory bus request enable
#define ES1370_CONTROL_BREQ \
    (ES1370_CONTROL_BREQ_MASK << ES1370_CONTROL_BREQ_BIT)

// DAC1 playback channel enable
#define ES1370_CONTROL_DAC1EN \
    (ES1370_CONTROL_DAC1EN_MASK << ES1370_CONTROL_DAC1EN_BIT)

// DAC2 playback channel enable
#define ES1370_CONTROL_DAC2EN \
    (ES1370_CONTROL_DAC2EN_MASK << ES1370_CONTROL_DAC2EN_BIT)

// ADC capture channel enable
#define ES1370_CONTROL_ADCEN \
    (ES1370_CONTROL_ADCEN_MASK << ES1370_CONTROL_ADCEN_BIT)

// UART enable
#define ES1370_CONTROL_UARTEN \
    (ES1370_CONTROL_UARTEN_MASK << ES1370_CONTROL_UARTEN_BIT)

// Joystick module enable
#define ES1370_CONTROL_JOYEN \
    (ES1370_CONTROL_JOYEN_MASK << ES1370_CONTROL_JOYEN_BIT)

// Codec interface enable
#define ES1370_CONTROL_CDC_EN \
    (ES1370_CONTROL_CDC_EN_MASK << ES1370_CONTROL_CDC_EN_BIT)

// PCI serr signal disable
#define ES1370_CONTROL_SERR_DIS \
    (ES1370_CONTROL_SERR_DIS_MASK << ES1370_CONTROL_SERR_DIS_BIT)

#define ES1370_CONTROL_ADC_STOP_n(n)      ((n) << ES1370_CONTROL_ADC_STOP_BIT)
#define ES1370_CONTROL_XCTL1_n(n)         ((n) << ES1370_CONTROL_XCTL1_BIT)
#define ES1370_CONTROL_PCLKDIV_n(n)       ((n) << ES1370_CONTROL_PCLKDIV_BIT)
#define ES1370_CONTROL_MSFMTSEL_n(n)      ((n) << ES1370_CONTROL_MSFMTSEL_BIT)
#define ES1370_CONTROL_SBB_n(n)           ((n) << ES1370_CONTROL_SBB_BIT)
#define ES1370_CONTROL_WTSRSEL_n(n)       ((n) << ES1370_CONTROL_WTSRSEL_BIT)
#define ES1370_CONTROL_DAC_SYNC_n(n)      ((n) << ES1370_CONTROL_DAC_SYNC_BIT)
#define ES1370_CONTROL_CCB_INTE_n(n)      ((n) << ES1370_CONTROL_CCB_INTE_BIT)
#define ES1370_CONTROL_M_CB_n(n)          ((n) << ES1370_CONTROL_M_CB_BIT)
#define ES1370_CONTROL_XCTL0_n(n)         ((n) << ES1370_CONTROL_XCTL0_BIT)
#define ES1370_CONTROL_BREQ_n(n)          ((n) << ES1370_CONTROL_BREQ_BIT)
#define ES1370_CONTROL_DAC1EN_n(n)        ((n) << ES1370_CONTROL_DAC1EN_BIT)
#define ES1370_CONTROL_DAC2EN_n(n)        ((n) << ES1370_CONTROL_DAC2EN_BIT)
#define ES1370_CONTROL_ADCEN_n(n)         ((n) << ES1370_CONTROL_ADCEN_BIT)
#define ES1370_CONTROL_UARTEN_n(n)        ((n) << ES1370_CONTROL_UARTEN_BIT)
#define ES1370_CONTROL_JOYEN_n(n)         ((n) << ES1370_CONTROL_JOYEN_BIT)
#define ES1370_CONTROL_CDC_EN_n(n)        ((n) << ES1370_CONTROL_CDC_EN_BIT)
#define ES1370_CONTROL_SERR_DIS_n(n)      ((n) << ES1370_CONTROL_SERR_DIS_BIT)

#define ES1370_CONTROL_ADC_STOP_GET(n) \
    (((n) >> ES1370_CONTROL_ADC_STOP_BIT) & ES1370_CONTROL_ADC_STOP_MASK)
#define ES1370_CONTROL_XCTL1_GET(n) \
    (((n) >> ES1370_CONTROL_XCTL1_BIT) & ES1370_CONTROL_XCTL1_MASK)
#define ES1370_CONTROL_PCLKDIV_GET(n) \
    (((n) >> ES1370_CONTROL_PCLKDIV_BIT) & ES1370_CONTROL_PCLKDIV_MASK)
#define ES1370_CONTROL_MSFMTSEL_GET(n) \
    (((n) >> ES1370_CONTROL_MSFMTSEL_BIT) & ES1370_CONTROL_MSFMTSEL_MASK)
#define ES1370_CONTROL_SBB_GET(n) \
    (((n) >> ES1370_CONTROL_SBB_BIT) & ES1370_CONTROL_SBB_MASK)
#define ES1370_CONTROL_WTSRSEL_GET(n) \
    (((n) >> ES1370_CONTROL_WTSRSEL_BIT) & ES1370_CONTROL_WTSRSEL_MASK)
#define ES1370_CONTROL_DAC_SYNC_GET(n) \
    (((n) >> ES1370_CONTROL_DAC_SYNC_BIT) & ES1370_CONTROL_DAC_SYNC_MASK)
#define ES1370_CONTROL_CCB_INTE_GET(n) \
    (((n) >> ES1370_CONTROL_CCB_INTE_BIT) & ES1370_CONTROL_CCB_INTE_MASK)
#define ES1370_CONTROL_M_CB_GET(n) \
    (((n) >> ES1370_CONTROL_M_CB_BIT) & ES1370_CONTROL_M_CB_MASK)
#define ES1370_CONTROL_XCTL0_GET(n) \
    (((n) >> ES1370_CONTROL_XCTL0_BIT) & ES1370_CONTROL_XCTL0_MASK)
#define ES1370_CONTROL_BREQ_GET(n) \
    (((n) >> ES1370_CONTROL_BREQ_BIT) & ES1370_CONTROL_BREQ_MASK)
#define ES1370_CONTROL_DAC1EN_GET(n) \
    (((n) >> ES1370_CONTROL_DAC1EN_BIT) & ES1370_CONTROL_DAC1EN_MASK)
#define ES1370_CONTROL_DAC2EN_GET(n) \
    (((n) >> ES1370_CONTROL_DAC2EN_BIT) & ES1370_CONTROL_DAC2EN_MASK)
#define ES1370_CONTROL_ADCEN_GET(n) \
    (((n) >> ES1370_CONTROL_ADCEN_BIT) & ES1370_CONTROL_ADCEN_MASK)
#define ES1370_CONTROL_UARTEN_GET(n) \
    (((n) >> ES1370_CONTROL_UARTEN_BIT) & ES1370_CONTROL_UARTEN_MASK)
#define ES1370_CONTROL_JOYEN_GET(n) \
    (((n) >> ES1370_CONTROL_JOYEN_BIT) & ES1370_CONTROL_JOYEN_MASK)
#define ES1370_CONTROL_CDC_EN_GET(n) \
    (((n) >> ES1370_CONTROL_CDC_EN_BIT) & ES1370_CONTROL_CDC_EN_MASK)
#define ES1370_CONTROL_SERR_DIS_GET(n) \
    (((n) >> ES1370_CONTROL_SERR_DIS_BIT) & ES1370_CONTROL_SERR_DIS_MASK)

#define ES1370_CONTROL_ADC_STOP_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_ADC_STOP) | ES1370_CONTROL_ADC_STOP_n((n)))
#define ES1370_CONTROL_XCTL1_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_XCTL1) | ES1370_CONTROL_XCTL1_n((n)))
#define ES1370_CONTROL_PCLKDIV_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_PCLKDIV) | ES1370_CONTROL_PCLKDIV_n((n)))
#define ES1370_CONTROL_MSFMTSEL_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_MSFMTSEL) | ES1370_CONTROL_MSFMTSEL_n((n)))
#define ES1370_CONTROL_SBB_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_SBB) | ES1370_CONTROL_SBB_n((n)))
#define ES1370_CONTROL_WTSRSEL_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_WTSRSEL) | ES1370_CONTROL_WTSRSEL_n((n)))
#define ES1370_CONTROL_DAC_SYNC_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_DAC_SYNC) | ES1370_CONTROL_DAC_SYNC_n((n)))
#define ES1370_CONTROL_CCB_INTE_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_CCB_INTE) | ES1370_CONTROL_CCB_INTE_n((n)))
#define ES1370_CONTROL_M_CB_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_M_CB) | ES1370_CONTROL_M_CB_n((n)))
#define ES1370_CONTROL_XCTL0_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_XCTL0) | ES1370_CONTROL_XCTL0_n((n)))
#define ES1370_CONTROL_BREQ_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_BREQ) | ES1370_CONTROL_BREQ_n((n)))
#define ES1370_CONTROL_DAC1EN_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_DAC1EN) | ES1370_CONTROL_DAC1EN_n((n)))
#define ES1370_CONTROL_DAC2EN_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_DAC2EN) | ES1370_CONTROL_DAC2EN_n((n)))
#define ES1370_CONTROL_ADCEN_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_ADCEN) | ES1370_CONTROL_ADCEN_n((n)))
#define ES1370_CONTROL_UARTEN_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_UARTEN) | ES1370_CONTROL_UARTEN_n((n)))
#define ES1370_CONTROL_JOYEN_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_JOYEN) | ES1370_CONTROL_JOYEN_n((n)))
#define ES1370_CONTROL_CDC_EN_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_CDC_EN) | ES1370_CONTROL_CDC_EN_n((n)))
#define ES1370_CONTROL_SERR_DIS_SET(r,n) \
    ((r) = ((r) & ~ES1370_CONTROL_SERR_DIS) | ES1370_CONTROL_SERR_DIS_n((n)))

// ES1370_CONTROL_WTSRSEL_5K
#define ES1370_CONTROL_WTSRSEL_5K 0

// ES1370_CONTROL_WTSRSEL_11K
#define ES1370_CONTROL_WTSRSEL_11K 1

// ES1370_CONTROL_WTSRSEL_22K
#define ES1370_CONTROL_WTSRSEL_22K 2

// ES1370_CONTROL_WTSRSEL_44K
#define ES1370_CONTROL_WTSRSEL_44K 3

// ES1370_STATUS
#define ES1370_STATUS                    0x04

#define ES1370_STATUS_INTR_BIT           31
#define ES1370_STATUS_CSTAT_BIT          10
#define ES1370_STATUS_CBUSY_BIT          9
#define ES1370_STATUS_CWRIP_BIT          8
#define ES1370_STATUS_VC_BIT             5
#define ES1370_STATUS_CCB_INT_BIT        4
#define ES1370_STATUS_UART_INT_BIT       3
#define ES1370_STATUS_DAC1_INT_BIT       2
#define ES1370_STATUS_DAC2_INT_BIT       1
#define ES1370_STATUS_ADC_INT_BIT        0

#define ES1370_STATUS_INTR_BITS          1
#define ES1370_STATUS_CSTAT_BITS         1
#define ES1370_STATUS_CBUSY_BITS         1
#define ES1370_STATUS_CWRIP_BITS         1
#define ES1370_STATUS_VC_BITS            2
#define ES1370_STATUS_CCB_INT_BITS       1
#define ES1370_STATUS_UART_INT_BITS      1
#define ES1370_STATUS_DAC1_INT_BITS      1
#define ES1370_STATUS_DAC2_INT_BITS      1
#define ES1370_STATUS_ADC_INT_BITS       1
#define ES1370_STATUS_INTR_MASK          ((1U << ES1370_STATUS_INTR_BITS)-1)
#define ES1370_STATUS_CSTAT_MASK         ((1U << ES1370_STATUS_CSTAT_BITS)-1)
#define ES1370_STATUS_CBUSY_MASK         ((1U << ES1370_STATUS_CBUSY_BITS)-1)
#define ES1370_STATUS_CWRIP_MASK         ((1U << ES1370_STATUS_CWRIP_BITS)-1)
#define ES1370_STATUS_VC_MASK            ((1U << ES1370_STATUS_VC_BITS)-1)
#define ES1370_STATUS_CCB_INT_MASK       ((1U << ES1370_STATUS_CCB_INT_BITS)-1)
#define ES1370_STATUS_UART_INT_MASK \
    ((1U << ES1370_STATUS_UART_INT_BITS)-1)
#define ES1370_STATUS_DAC1_INT_MASK \
    ((1U << ES1370_STATUS_DAC1_INT_BITS)-1)
#define ES1370_STATUS_DAC2_INT_MASK \
    ((1U << ES1370_STATUS_DAC2_INT_BITS)-1)
#define ES1370_STATUS_ADC_INT_MASK       ((1U << ES1370_STATUS_ADC_INT_BITS)-1)

// Interrupt is pending
#define ES1370_STATUS_INTR \
    (ES1370_STATUS_INTR_MASK << ES1370_STATUS_INTR_BIT)

// CODEC is busy or register write in progress
#define ES1370_STATUS_CSTAT \
    (ES1370_STATUS_CSTAT_MASK << ES1370_STATUS_CSTAT_BIT)

// CODEC is busy
#define ES1370_STATUS_CBUSY \
    (ES1370_STATUS_CBUSY_MASK << ES1370_STATUS_CBUSY_BIT)

// CODEC register write in progress
#define ES1370_STATUS_CWRIP \
    (ES1370_STATUS_CWRIP_MASK << ES1370_STATUS_CWRIP_BIT)

// voice code from CCB module
#define ES1370_STATUS_VC \
    (ES1370_STATUS_VC_MASK << ES1370_STATUS_VC_BIT)

// CCB interrupt pending
#define ES1370_STATUS_CCB_INT \
    (ES1370_STATUS_CCB_INT_MASK << ES1370_STATUS_CCB_INT_BIT)

// UART interrupt pending
#define ES1370_STATUS_UART_INT \
    (ES1370_STATUS_UART_INT_MASK << ES1370_STATUS_UART_INT_BIT)

// DAC1 channel interrupt pending
#define ES1370_STATUS_DAC1_INT \
    (ES1370_STATUS_DAC1_INT_MASK << ES1370_STATUS_DAC1_INT_BIT)

// DAC2 channel interrupt pending
#define ES1370_STATUS_DAC2_INT \
    (ES1370_STATUS_DAC2_INT_MASK << ES1370_STATUS_DAC2_INT_BIT)

// ADC channel interrupt pending
#define ES1370_STATUS_ADC_INT \
    (ES1370_STATUS_ADC_INT_MASK << ES1370_STATUS_ADC_INT_BIT)

#define ES1370_STATUS_INTR_n(n)          ((n) << ES1370_STATUS_INTR_BIT)
#define ES1370_STATUS_CSTAT_n(n)         ((n) << ES1370_STATUS_CSTAT_BIT)
#define ES1370_STATUS_CBUSY_n(n)         ((n) << ES1370_STATUS_CBUSY_BIT)
#define ES1370_STATUS_CWRIP_n(n)         ((n) << ES1370_STATUS_CWRIP_BIT)
#define ES1370_STATUS_VC_n(n)            ((n) << ES1370_STATUS_VC_BIT)
#define ES1370_STATUS_CCB_INT_n(n)       ((n) << ES1370_STATUS_CCB_INT_BIT)
#define ES1370_STATUS_UART_INT_n(n)      ((n) << ES1370_STATUS_UART_INT_BIT)
#define ES1370_STATUS_DAC1_INT_n(n)      ((n) << ES1370_STATUS_DAC1_INT_BIT)
#define ES1370_STATUS_DAC2_INT_n(n)      ((n) << ES1370_STATUS_DAC2_INT_BIT)
#define ES1370_STATUS_ADC_INT_n(n)       ((n) << ES1370_STATUS_ADC_INT_BIT)

#define ES1370_STATUS_INTR_GET(n) \
    (((n) >> ES1370_STATUS_INTR_BIT) & ES1370_STATUS_INTR_MASK)
#define ES1370_STATUS_CSTAT_GET(n) \
    (((n) >> ES1370_STATUS_CSTAT_BIT) & ES1370_STATUS_CSTAT_MASK)
#define ES1370_STATUS_CBUSY_GET(n) \
    (((n) >> ES1370_STATUS_CBUSY_BIT) & ES1370_STATUS_CBUSY_MASK)
#define ES1370_STATUS_CWRIP_GET(n) \
    (((n) >> ES1370_STATUS_CWRIP_BIT) & ES1370_STATUS_CWRIP_MASK)
#define ES1370_STATUS_VC_GET(n) \
    (((n) >> ES1370_STATUS_VC_BIT) & ES1370_STATUS_VC_MASK)
#define ES1370_STATUS_CCB_INT_GET(n) \
    (((n) >> ES1370_STATUS_CCB_INT_BIT) & ES1370_STATUS_CCB_INT_MASK)
#define ES1370_STATUS_UART_INT_GET(n) \
    (((n) >> ES1370_STATUS_UART_INT_BIT) & ES1370_STATUS_UART_INT_MASK)
#define ES1370_STATUS_DAC1_INT_GET(n) \
    (((n) >> ES1370_STATUS_DAC1_INT_BIT) & ES1370_STATUS_DAC1_INT_MASK)
#define ES1370_STATUS_DAC2_INT_GET(n) \
    (((n) >> ES1370_STATUS_DAC2_INT_BIT) & ES1370_STATUS_DAC2_INT_MASK)
#define ES1370_STATUS_ADC_INT_GET(n) \
    (((n) >> ES1370_STATUS_ADC_INT_BIT) & ES1370_STATUS_ADC_INT_MASK)

#define ES1370_STATUS_INTR_SET(r,n) \
    ((r) = ((r) & ~ES1370_STATUS_INTR) | ES1370_STATUS_INTR_n((n)))
#define ES1370_STATUS_CSTAT_SET(r,n) \
    ((r) = ((r) & ~ES1370_STATUS_CSTAT) | ES1370_STATUS_CSTAT_n((n)))
#define ES1370_STATUS_CBUSY_SET(r,n) \
    ((r) = ((r) & ~ES1370_STATUS_CBUSY) | ES1370_STATUS_CBUSY_n((n)))
#define ES1370_STATUS_CWRIP_SET(r,n) \
    ((r) = ((r) & ~ES1370_STATUS_CWRIP) | ES1370_STATUS_CWRIP_n((n)))
#define ES1370_STATUS_VC_SET(r,n) \
    ((r) = ((r) & ~ES1370_STATUS_VC) | ES1370_STATUS_VC_n((n)))
#define ES1370_STATUS_CCB_INT_SET(r,n) \
    ((r) = ((r) & ~ES1370_STATUS_CCB_INT) | ES1370_STATUS_CCB_INT_n((n)))
#define ES1370_STATUS_UART_INT_SET(r,n) \
    ((r) = ((r) & ~ES1370_STATUS_UART_INT) | ES1370_STATUS_UART_INT_n((n)))
#define ES1370_STATUS_DAC1_INT_SET(r,n) \
    ((r) = ((r) & ~ES1370_STATUS_DAC1_INT) | ES1370_STATUS_DAC1_INT_n((n)))
#define ES1370_STATUS_DAC2_INT_SET(r,n) \
    ((r) = ((r) & ~ES1370_STATUS_DAC2_INT) | ES1370_STATUS_DAC2_INT_n((n)))
#define ES1370_STATUS_ADC_INT_SET(r,n) \
    ((r) = ((r) & ~ES1370_STATUS_ADC_INT) | ES1370_STATUS_ADC_INT_n((n)))

// ES1370_UARTDATA
#define ES1370_UARTDATA 0x08

//
// ES1370_UARTSTATUS: R/O: UART status register
#define ES1370_UARTSTATUS                   0x09

#define ES1370_UARTSTATUS_RX_INT_BIT        7
#define ES1370_UARTSTATUS_TX_INT_BIT        2
#define ES1370_UARTSTATUS_TX_RDY_BIT        1
#define ES1370_UARTSTATUS_RX_RDY_BIT        0
#define ES1370_UARTSTATUS_RX_INTE_BIT       7
#define ES1370_UARTSTATUS_TX_INTE_BIT       5
#define ES1370_UARTSTATUS_CTRL_BIT          0

#define ES1370_UARTSTATUS_RX_INT_BITS       1
#define ES1370_UARTSTATUS_TX_INT_BITS       1
#define ES1370_UARTSTATUS_TX_RDY_BITS       1
#define ES1370_UARTSTATUS_RX_RDY_BITS       1
#define ES1370_UARTSTATUS_RX_INTE_BITS      1
#define ES1370_UARTSTATUS_TX_INTE_BITS      2
#define ES1370_UARTSTATUS_CTRL_BITS         2
#define ES1370_UARTSTATUS_RX_INT_MASK \
    ((1U << ES1370_UARTSTATUS_RX_INT_BITS)-1)
#define ES1370_UARTSTATUS_TX_INT_MASK \
    ((1U << ES1370_UARTSTATUS_TX_INT_BITS)-1)
#define ES1370_UARTSTATUS_TX_RDY_MASK \
    ((1U << ES1370_UARTSTATUS_TX_RDY_BITS)-1)
#define ES1370_UARTSTATUS_RX_RDY_MASK \
    ((1U << ES1370_UARTSTATUS_RX_RDY_BITS)-1)
#define ES1370_UARTSTATUS_RX_INTE_MASK \
    ((1U << ES1370_UARTSTATUS_RX_INTE_BITS)-1)
#define ES1370_UARTSTATUS_TX_INTE_MASK \
    ((1U << ES1370_UARTSTATUS_TX_INTE_BITS)-1)
#define ES1370_UARTSTATUS_CTRL_MASK \
    ((1U << ES1370_UARTSTATUS_CTRL_BITS)-1)

// Rx interrupt occurred
#define ES1370_UARTSTATUS_RX_INT \
    (ES1370_UARTSTATUS_RX_INT_MASK << ES1370_UARTSTATUS_RX_INT_BIT)

// Tx interrupt occurred
#define ES1370_UARTSTATUS_TX_INT \
    (ES1370_UARTSTATUS_TX_INT_MASK << ES1370_UARTSTATUS_TX_INT_BIT)

// Tx ready
#define ES1370_UARTSTATUS_TX_RDY \
    (ES1370_UARTSTATUS_TX_RDY_MASK << ES1370_UARTSTATUS_TX_RDY_BIT)

// Rx ready
#define ES1370_UARTSTATUS_RX_RDY \
    (ES1370_UARTSTATUS_RX_RDY_MASK << ES1370_UARTSTATUS_RX_RDY_BIT)

// Rx interrupt enable
#define ES1370_UARTSTATUS_RX_INTE \
    (ES1370_UARTSTATUS_RX_INTE_MASK << ES1370_UARTSTATUS_RX_INTE_BIT)

// Tx interrupt enable
#define ES1370_UARTSTATUS_TX_INTE \
    (ES1370_UARTSTATUS_TX_INTE_MASK << ES1370_UARTSTATUS_TX_INTE_BIT)

// Control
#define ES1370_UARTSTATUS_CTRL \
    (ES1370_UARTSTATUS_CTRL_MASK << ES1370_UARTSTATUS_CTRL_BIT)

#define ES1370_UARTSTATUS_RX_INT_n(n) \
    ((n) << ES1370_UARTSTATUS_RX_INT_BIT)
#define ES1370_UARTSTATUS_TX_INT_n(n) \
    ((n) << ES1370_UARTSTATUS_TX_INT_BIT)
#define ES1370_UARTSTATUS_TX_RDY_n(n) \
    ((n) << ES1370_UARTSTATUS_TX_RDY_BIT)
#define ES1370_UARTSTATUS_RX_RDY_n(n) \
    ((n) << ES1370_UARTSTATUS_RX_RDY_BIT)
#define ES1370_UARTSTATUS_RX_INTE_n(n) \
    ((n) << ES1370_UARTSTATUS_RX_INTE_BIT)
#define ES1370_UARTSTATUS_TX_INTE_n(n) \
    ((n) << ES1370_UARTSTATUS_TX_INTE_BIT)
#define ES1370_UARTSTATUS_CTRL_n(n)         ((n) << ES1370_UARTSTATUS_CTRL_BIT)

#define ES1370_UARTSTATUS_RX_INT_GET(n) \
    (((n) >> ES1370_UARTSTATUS_RX_INT_BIT) & ES1370_UARTSTATUS_RX_INT_MASK)
#define ES1370_UARTSTATUS_TX_INT_GET(n) \
    (((n) >> ES1370_UARTSTATUS_TX_INT_BIT) & ES1370_UARTSTATUS_TX_INT_MASK)
#define ES1370_UARTSTATUS_TX_RDY_GET(n) \
    (((n) >> ES1370_UARTSTATUS_TX_RDY_BIT) & ES1370_UARTSTATUS_TX_RDY_MASK)
#define ES1370_UARTSTATUS_RX_RDY_GET(n) \
    (((n) >> ES1370_UARTSTATUS_RX_RDY_BIT) & ES1370_UARTSTATUS_RX_RDY_MASK)
#define ES1370_UARTSTATUS_RX_INTE_GET(n) \
    (((n) >> ES1370_UARTSTATUS_RX_INTE_BIT) & ES1370_UARTSTATUS_RX_INTE_MASK)
#define ES1370_UARTSTATUS_TX_INTE_GET(n) \
    (((n) >> ES1370_UARTSTATUS_TX_INTE_BIT) & ES1370_UARTSTATUS_TX_INTE_MASK)
#define ES1370_UARTSTATUS_CTRL_GET(n) \
    (((n) >> ES1370_UARTSTATUS_CTRL_BIT) & ES1370_UARTSTATUS_CTRL_MASK)

#define ES1370_UARTSTATUS_RX_INT_SET(r,n) \
    ((r) = ((r) & ~ES1370_UARTSTATUS_RX_INT) | ES1370_UARTSTATUS_RX_INT_n((n)))
#define ES1370_UARTSTATUS_TX_INT_SET(r,n) \
    ((r) = ((r) & ~ES1370_UARTSTATUS_TX_INT) | ES1370_UARTSTATUS_TX_INT_n((n)))
#define ES1370_UARTSTATUS_TX_RDY_SET(r,n) \
    ((r) = ((r) & ~ES1370_UARTSTATUS_TX_RDY) | ES1370_UARTSTATUS_TX_RDY_n((n)))
#define ES1370_UARTSTATUS_RX_RDY_SET(r,n) \
    ((r) = ((r) & ~ES1370_UARTSTATUS_RX_RDY) | ES1370_UARTSTATUS_RX_RDY_n((n)))
#define ES1370_UARTSTATUS_RX_INTE_SET(r,n)  ((r) \
    = ((r) & ~ES1370_UARTSTATUS_RX_INTE) | ES1370_UARTSTATUS_RX_INTE_n((n)))
#define ES1370_UARTSTATUS_TX_INTE_SET(r,n)  ((r) \
    = ((r) & ~ES1370_UARTSTATUS_TX_INTE) | ES1370_UARTSTATUS_TX_INTE_n((n)))
#define ES1370_UARTSTATUS_CTRL_SET(r,n) \
    ((r) = ((r) & ~ES1370_UARTSTATUS_CTRL) | ES1370_UARTSTATUS_CTRL_n((n)))

// ES1370_UARTTEST
#define ES1370_UARTTEST                0x0A

#define ES1370_UARTTEST_TEST_BIT       0

#define ES1370_UARTTEST_TEST_BITS      1
#define ES1370_UARTTEST_TEST_MASK      ((1U << ES1370_UARTTEST_TEST_BITS)-1)

// Test mode enabled
#define ES1370_UARTTEST_TEST \
    (ES1370_UARTTEST_TEST_MASK << ES1370_UARTTEST_TEST_BIT)

#define ES1370_UARTTEST_TEST_n(n)      ((n) << ES1370_UARTTEST_TEST_BIT)

#define ES1370_UARTTEST_TEST_GET(n) \
    (((n) >> ES1370_UARTTEST_TEST_BIT) & ES1370_UARTTEST_TEST_MASK)

#define ES1370_UARTTEST_TEST_SET(r,n) \
    ((r) = ((r) & ~ES1370_UARTTEST_TEST) | ES1370_UARTTEST_TEST_n((n)))

// ES1370_MEMPAGE
#define ES1370_MEMPAGE                0x0C

#define ES1370_MEMPAGE_PAGE_BIT       0

#define ES1370_MEMPAGE_PAGE_BITS      4
#define ES1370_MEMPAGE_PAGE_MASK      ((1U << ES1370_MEMPAGE_PAGE_BITS)-1)

// page select
#define ES1370_MEMPAGE_PAGE \
    (ES1370_MEMPAGE_PAGE_MASK << ES1370_MEMPAGE_PAGE_BIT)

#define ES1370_MEMPAGE_PAGE_n(n)      ((n) << ES1370_MEMPAGE_PAGE_BIT)

#define ES1370_MEMPAGE_PAGE_GET(n) \
    (((n) >> ES1370_MEMPAGE_PAGE_BIT) & ES1370_MEMPAGE_PAGE_MASK)

#define ES1370_MEMPAGE_PAGE_SET(r,n) \
    ((r) = ((r) & ~ES1370_MEMPAGE_PAGE) | ES1370_MEMPAGE_PAGE_n((n)))

// ES1370_PAGE_DAC
#define ES1370_PAGE_DAC 0x0c

// ES1370_PAGE_ADC
#define ES1370_PAGE_ADC 0x0d

// ES1370_PAGE_UART
#define ES1370_PAGE_UART 0x0e

// ES1370_PAGE_UART1
#define ES1370_PAGE_UART1 0x0f

//
// ES1370_CODEC: W/O: Codec write register address
#define ES1370_CODEC                0x10

#define ES1370_CODEC_ADDR_BIT       8
#define ES1370_CODEC_DATA_BIT       0

#define ES1370_CODEC_ADDR_BITS      8
#define ES1370_CODEC_DATA_BITS      8
#define ES1370_CODEC_ADDR_MASK      ((1U << ES1370_CODEC_ADDR_BITS)-1)
#define ES1370_CODEC_DATA_MASK      ((1U << ES1370_CODEC_DATA_BITS)-1)
#define ES1370_CODEC_ADDR \
    (ES1370_CODEC_ADDR_MASK << ES1370_CODEC_ADDR_BIT)
#define ES1370_CODEC_DATA \
    (ES1370_CODEC_DATA_MASK << ES1370_CODEC_DATA_BIT)

#define ES1370_CODEC_ADDR_n(n)      ((n) << ES1370_CODEC_ADDR_BIT)
#define ES1370_CODEC_DATA_n(n)      ((n) << ES1370_CODEC_DATA_BIT)

#define ES1370_CODEC_ADDR_GET(n) \
    (((n) >> ES1370_CODEC_ADDR_BIT) & ES1370_CODEC_ADDR_MASK)
#define ES1370_CODEC_DATA_GET(n) \
    (((n) >> ES1370_CODEC_DATA_BIT) & ES1370_CODEC_DATA_MASK)

#define ES1370_CODEC_ADDR_SET(r,n) \
    ((r) = ((r) & ~ES1370_CODEC_ADDR) | ES1370_CODEC_ADDR_n((n)))
#define ES1370_CODEC_DATA_SET(r,n) \
    ((r) = ((r) & ~ES1370_CODEC_DATA) | ES1370_CODEC_DATA_n((n)))

//
// ES1370_SERIAL: R/W: Serial interface control register
#define ES1370_SERIAL                        0x20

#define ES1370_SERIAL_END_INC_BIT            19
#define ES1370_SERIAL_ST_INC_BIT             16
#define ES1370_SERIAL_ADC_LOOP_SEL_BIT       15
#define ES1370_SERIAL_PB1_LOOP_SEL_BIT       14
#define ES1370_SERIAL_PB2_LOOP_SEL_BIT       13
#define ES1370_SERIAL_PB2_PAUSE_BIT          12
#define ES1370_SERIAL_PB1_PAUSE_BIT          11
#define ES1370_SERIAL_ADC_INTE_BIT           10
#define ES1370_SERIAL_PB2_INTE_BIT           9
#define ES1370_SERIAL_PB1_INTE_BIT           8
#define ES1370_SERIAL_SCT_RLD_BIT            7
#define ES1370_SERIAL_DAC_SEN_BIT            6
#define ES1370_SERIAL_ADC_BITS_BIT           5
#define ES1370_SERIAL_ADC_CHAN_BIT           4
#define ES1370_SERIAL_PB2_BITS_BIT           3
#define ES1370_SERIAL_PB2_CHAN_BIT           2
#define ES1370_SERIAL_PB1_BITS_BIT           1
#define ES1370_SERIAL_PB1_CHAN_BIT           0

#define ES1370_SERIAL_END_INC_BITS           3
#define ES1370_SERIAL_ST_INC_BITS            3
#define ES1370_SERIAL_ADC_LOOP_SEL_BITS      1
#define ES1370_SERIAL_PB1_LOOP_SEL_BITS      1
#define ES1370_SERIAL_PB2_LOOP_SEL_BITS      1
#define ES1370_SERIAL_PB2_PAUSE_BITS         1
#define ES1370_SERIAL_PB1_PAUSE_BITS         1
#define ES1370_SERIAL_ADC_INTE_BITS          1
#define ES1370_SERIAL_PB2_INTE_BITS          1
#define ES1370_SERIAL_PB1_INTE_BITS          1
#define ES1370_SERIAL_SCT_RLD_BITS           1
#define ES1370_SERIAL_DAC_SEN_BITS           1
#define ES1370_SERIAL_ADC_BITS_BITS          1
#define ES1370_SERIAL_ADC_CHAN_BITS          1
#define ES1370_SERIAL_PB2_BITS_BITS          1
#define ES1370_SERIAL_PB2_CHAN_BITS          1
#define ES1370_SERIAL_PB1_BITS_BITS          1
#define ES1370_SERIAL_PB1_CHAN_BITS          1
#define ES1370_SERIAL_END_INC_MASK \
    ((1U << ES1370_SERIAL_END_INC_BITS)-1)
#define ES1370_SERIAL_ST_INC_MASK \
    ((1U << ES1370_SERIAL_ST_INC_BITS)-1)
#define ES1370_SERIAL_ADC_LOOP_SEL_MASK \
    ((1U << ES1370_SERIAL_ADC_LOOP_SEL_BITS)-1)
#define ES1370_SERIAL_PB1_LOOP_SEL_MASK \
    ((1U << ES1370_SERIAL_PB1_LOOP_SEL_BITS)-1)
#define ES1370_SERIAL_PB2_LOOP_SEL_MASK \
    ((1U << ES1370_SERIAL_PB2_LOOP_SEL_BITS)-1)
#define ES1370_SERIAL_PB2_PAUSE_MASK \
    ((1U << ES1370_SERIAL_PB2_PAUSE_BITS)-1)
#define ES1370_SERIAL_PB1_PAUSE_MASK \
    ((1U << ES1370_SERIAL_PB1_PAUSE_BITS)-1)
#define ES1370_SERIAL_ADC_INTE_MASK \
    ((1U << ES1370_SERIAL_ADC_INTE_BITS)-1)
#define ES1370_SERIAL_PB2_INTE_MASK \
    ((1U << ES1370_SERIAL_PB2_INTE_BITS)-1)
#define ES1370_SERIAL_PB1_INTE_MASK \
    ((1U << ES1370_SERIAL_PB1_INTE_BITS)-1)
#define ES1370_SERIAL_SCT_RLD_MASK \
    ((1U << ES1370_SERIAL_SCT_RLD_BITS)-1)
#define ES1370_SERIAL_DAC_SEN_MASK \
    ((1U << ES1370_SERIAL_DAC_SEN_BITS)-1)
#define ES1370_SERIAL_ADC_BITS_MASK \
    ((1U << ES1370_SERIAL_ADC_BITS_BITS)-1)
#define ES1370_SERIAL_ADC_CHAN_MASK \
    ((1U << ES1370_SERIAL_ADC_CHAN_BITS)-1)
#define ES1370_SERIAL_PB2_BITS_MASK \
    ((1U << ES1370_SERIAL_PB2_BITS_BITS)-1)
#define ES1370_SERIAL_PB2_CHAN_MASK \
    ((1U << ES1370_SERIAL_PB2_CHAN_BITS)-1)
#define ES1370_SERIAL_PB1_BITS_MASK \
    ((1U << ES1370_SERIAL_PB1_BITS_BITS)-1)
#define ES1370_SERIAL_PB1_CHAN_MASK \
    ((1U << ES1370_SERIAL_PB1_CHAN_BITS)-1)

// binary offset value to increment / loop end */
#define ES1370_SERIAL_END_INC \
    (ES1370_SERIAL_END_INC_MASK << ES1370_SERIAL_END_INC_BIT)

// binary offset value to increment / start
#define ES1370_SERIAL_ST_INC \
    (ES1370_SERIAL_ST_INC_MASK << ES1370_SERIAL_ST_INC_BIT)

// 0=loop, 1=stop
#define ES1370_SERIAL_ADC_LOOP_SEL \
    (ES1370_SERIAL_ADC_LOOP_SEL_MASK << ES1370_SERIAL_ADC_LOOP_SEL_BIT)

// 0=loop, 1=stop
#define ES1370_SERIAL_PB1_LOOP_SEL \
    (ES1370_SERIAL_PB1_LOOP_SEL_MASK << ES1370_SERIAL_PB1_LOOP_SEL_BIT)

// 0=loop, 1=stop
#define ES1370_SERIAL_PB2_LOOP_SEL \
    (ES1370_SERIAL_PB2_LOOP_SEL_MASK << ES1370_SERIAL_PB2_LOOP_SEL_BIT)

// 0=play 1=pause
#define ES1370_SERIAL_PB2_PAUSE \
    (ES1370_SERIAL_PB2_PAUSE_MASK << ES1370_SERIAL_PB2_PAUSE_BIT)

// 0=play 1=pause
#define ES1370_SERIAL_PB1_PAUSE \
    (ES1370_SERIAL_PB1_PAUSE_MASK << ES1370_SERIAL_PB1_PAUSE_BIT)

// ADC interrupt enable
#define ES1370_SERIAL_ADC_INTE \
    (ES1370_SERIAL_ADC_INTE_MASK << ES1370_SERIAL_ADC_INTE_BIT)

// DAC2 interrupt enable
#define ES1370_SERIAL_PB2_INTE \
    (ES1370_SERIAL_PB2_INTE_MASK << ES1370_SERIAL_PB2_INTE_BIT)

// DAC1 interrupt enable
#define ES1370_SERIAL_PB1_INTE \
    (ES1370_SERIAL_PB1_INTE_MASK << ES1370_SERIAL_PB1_INTE_BIT)

// force saple count reload for DAC1
#define ES1370_SERIAL_SCT_RLD \
    (ES1370_SERIAL_SCT_RLD_MASK << ES1370_SERIAL_SCT_RLD_BIT)

// stop when: 0=DAC2 play back zeros, 1=play back last sample
#define ES1370_SERIAL_DAC_SEN \
    (ES1370_SERIAL_DAC_SEN_MASK << ES1370_SERIAL_DAC_SEN_BIT)

// 0=8-bit, 1=16-bit
#define ES1370_SERIAL_ADC_BITS \
    (ES1370_SERIAL_ADC_BITS_MASK << ES1370_SERIAL_ADC_BITS_BIT)

// 0=mono, 1=stereo
#define ES1370_SERIAL_ADC_CHAN \
    (ES1370_SERIAL_ADC_CHAN_MASK << ES1370_SERIAL_ADC_CHAN_BIT)

// 0=8-bit, 1=16-bit
#define ES1370_SERIAL_PB2_BITS \
    (ES1370_SERIAL_PB2_BITS_MASK << ES1370_SERIAL_PB2_BITS_BIT)

// 0=mono, 1=stereo
#define ES1370_SERIAL_PB2_CHAN \
    (ES1370_SERIAL_PB2_CHAN_MASK << ES1370_SERIAL_PB2_CHAN_BIT)

// 0=8-bit, 1=16-bit
#define ES1370_SERIAL_PB1_BITS \
    (ES1370_SERIAL_PB1_BITS_MASK << ES1370_SERIAL_PB1_BITS_BIT)

// 0=mono, 1=stereo
#define ES1370_SERIAL_PB1_CHAN \
    (ES1370_SERIAL_PB1_CHAN_MASK << ES1370_SERIAL_PB1_CHAN_BIT)

#define ES1370_SERIAL_END_INC_n(n)           ((n) << ES1370_SERIAL_END_INC_BIT)
#define ES1370_SERIAL_ST_INC_n(n)            ((n) << ES1370_SERIAL_ST_INC_BIT)
#define ES1370_SERIAL_ADC_LOOP_SEL_n(n) \
    ((n) << ES1370_SERIAL_ADC_LOOP_SEL_BIT)
#define ES1370_SERIAL_PB1_LOOP_SEL_n(n) \
    ((n) << ES1370_SERIAL_PB1_LOOP_SEL_BIT)
#define ES1370_SERIAL_PB2_LOOP_SEL_n(n) \
    ((n) << ES1370_SERIAL_PB2_LOOP_SEL_BIT)
#define ES1370_SERIAL_PB2_PAUSE_n(n) \
    ((n) << ES1370_SERIAL_PB2_PAUSE_BIT)
#define ES1370_SERIAL_PB1_PAUSE_n(n) \
    ((n) << ES1370_SERIAL_PB1_PAUSE_BIT)
#define ES1370_SERIAL_ADC_INTE_n(n) \
    ((n) << ES1370_SERIAL_ADC_INTE_BIT)
#define ES1370_SERIAL_PB2_INTE_n(n) \
    ((n) << ES1370_SERIAL_PB2_INTE_BIT)
#define ES1370_SERIAL_PB1_INTE_n(n) \
    ((n) << ES1370_SERIAL_PB1_INTE_BIT)
#define ES1370_SERIAL_SCT_RLD_n(n)           ((n) << ES1370_SERIAL_SCT_RLD_BIT)
#define ES1370_SERIAL_DAC_SEN_n(n)           ((n) << ES1370_SERIAL_DAC_SEN_BIT)
#define ES1370_SERIAL_ADC_BITS_n(n) \
    ((n) << ES1370_SERIAL_ADC_BITS_BIT)
#define ES1370_SERIAL_ADC_CHAN_n(n) \
    ((n) << ES1370_SERIAL_ADC_CHAN_BIT)
#define ES1370_SERIAL_PB2_BITS_n(n) \
    ((n) << ES1370_SERIAL_PB2_BITS_BIT)
#define ES1370_SERIAL_PB2_CHAN_n(n) \
    ((n) << ES1370_SERIAL_PB2_CHAN_BIT)
#define ES1370_SERIAL_PB1_BITS_n(n) \
    ((n) << ES1370_SERIAL_PB1_BITS_BIT)
#define ES1370_SERIAL_PB1_CHAN_n(n) \
    ((n) << ES1370_SERIAL_PB1_CHAN_BIT)

#define ES1370_SERIAL_END_INC_GET(n) \
    (((n) >> ES1370_SERIAL_END_INC_BIT) & ES1370_SERIAL_END_INC_MASK)
#define ES1370_SERIAL_ST_INC_GET(n) \
    (((n) >> ES1370_SERIAL_ST_INC_BIT) & ES1370_SERIAL_ST_INC_MASK)
#define ES1370_SERIAL_ADC_LOOP_SEL_GET(n) \
    (((n) >> ES1370_SERIAL_ADC_LOOP_SEL_BIT) & ES1370_SERIAL_ADC_LOOP_SEL_MASK)
#define ES1370_SERIAL_PB1_LOOP_SEL_GET(n) \
    (((n) >> ES1370_SERIAL_PB1_LOOP_SEL_BIT) & ES1370_SERIAL_PB1_LOOP_SEL_MASK)
#define ES1370_SERIAL_PB2_LOOP_SEL_GET(n) \
    (((n) >> ES1370_SERIAL_PB2_LOOP_SEL_BIT) & ES1370_SERIAL_PB2_LOOP_SEL_MASK)
#define ES1370_SERIAL_PB2_PAUSE_GET(n) \
    (((n) >> ES1370_SERIAL_PB2_PAUSE_BIT) & ES1370_SERIAL_PB2_PAUSE_MASK)
#define ES1370_SERIAL_PB1_PAUSE_GET(n) \
    (((n) >> ES1370_SERIAL_PB1_PAUSE_BIT) & ES1370_SERIAL_PB1_PAUSE_MASK)
#define ES1370_SERIAL_ADC_INTE_GET(n) \
    (((n) >> ES1370_SERIAL_ADC_INTE_BIT) & ES1370_SERIAL_ADC_INTE_MASK)
#define ES1370_SERIAL_PB2_INTE_GET(n) \
    (((n) >> ES1370_SERIAL_PB2_INTE_BIT) & ES1370_SERIAL_PB2_INTE_MASK)
#define ES1370_SERIAL_PB1_INTE_GET(n) \
    (((n) >> ES1370_SERIAL_PB1_INTE_BIT) & ES1370_SERIAL_PB1_INTE_MASK)
#define ES1370_SERIAL_SCT_RLD_GET(n) \
    (((n) >> ES1370_SERIAL_SCT_RLD_BIT) & ES1370_SERIAL_SCT_RLD_MASK)
#define ES1370_SERIAL_DAC_SEN_GET(n) \
    (((n) >> ES1370_SERIAL_DAC_SEN_BIT) & ES1370_SERIAL_DAC_SEN_MASK)
#define ES1370_SERIAL_ADC_BITS_GET(n) \
    (((n) >> ES1370_SERIAL_ADC_BITS_BIT) & ES1370_SERIAL_ADC_BITS_MASK)
#define ES1370_SERIAL_ADC_CHAN_GET(n) \
    (((n) >> ES1370_SERIAL_ADC_CHAN_BIT) & ES1370_SERIAL_ADC_CHAN_MASK)
#define ES1370_SERIAL_PB2_BITS_GET(n) \
    (((n) >> ES1370_SERIAL_PB2_BITS_BIT) & ES1370_SERIAL_PB2_BITS_MASK)
#define ES1370_SERIAL_PB2_CHAN_GET(n) \
    (((n) >> ES1370_SERIAL_PB2_CHAN_BIT) & ES1370_SERIAL_PB2_CHAN_MASK)
#define ES1370_SERIAL_PB1_BITS_GET(n) \
    (((n) >> ES1370_SERIAL_PB1_BITS_BIT) & ES1370_SERIAL_PB1_BITS_MASK)
#define ES1370_SERIAL_PB1_CHAN_GET(n) \
    (((n) >> ES1370_SERIAL_PB1_CHAN_BIT) & ES1370_SERIAL_PB1_CHAN_MASK)

#define ES1370_SERIAL_END_INC_SET(r,n) \
    ((r) = ((r) & ~ES1370_SERIAL_END_INC) | ES1370_SERIAL_END_INC_n((n)))
#define ES1370_SERIAL_ST_INC_SET(r,n) \
    ((r) = ((r) & ~ES1370_SERIAL_ST_INC) | ES1370_SERIAL_ST_INC_n((n)))
#define ES1370_SERIAL_ADC_LOOP_SEL_SET(r,n)  ((r) \
    = ((r) & ~ES1370_SERIAL_ADC_LOOP_SEL) | ES1370_SERIAL_ADC_LOOP_SEL_n((n)))
#define ES1370_SERIAL_PB1_LOOP_SEL_SET(r,n)  ((r) \
    = ((r) & ~ES1370_SERIAL_PB1_LOOP_SEL) | ES1370_SERIAL_PB1_LOOP_SEL_n((n)))
#define ES1370_SERIAL_PB2_LOOP_SEL_SET(r,n)  ((r) \
    = ((r) & ~ES1370_SERIAL_PB2_LOOP_SEL) | ES1370_SERIAL_PB2_LOOP_SEL_n((n)))
#define ES1370_SERIAL_PB2_PAUSE_SET(r,n) \
    ((r) = ((r) & ~ES1370_SERIAL_PB2_PAUSE) | ES1370_SERIAL_PB2_PAUSE_n((n)))
#define ES1370_SERIAL_PB1_PAUSE_SET(r,n) \
    ((r) = ((r) & ~ES1370_SERIAL_PB1_PAUSE) | ES1370_SERIAL_PB1_PAUSE_n((n)))
#define ES1370_SERIAL_ADC_INTE_SET(r,n) \
    ((r) = ((r) & ~ES1370_SERIAL_ADC_INTE) | ES1370_SERIAL_ADC_INTE_n((n)))
#define ES1370_SERIAL_PB2_INTE_SET(r,n) \
    ((r) = ((r) & ~ES1370_SERIAL_PB2_INTE) | ES1370_SERIAL_PB2_INTE_n((n)))
#define ES1370_SERIAL_PB1_INTE_SET(r,n) \
    ((r) = ((r) & ~ES1370_SERIAL_PB1_INTE) | ES1370_SERIAL_PB1_INTE_n((n)))
#define ES1370_SERIAL_SCT_RLD_SET(r,n) \
    ((r) = ((r) & ~ES1370_SERIAL_SCT_RLD) | ES1370_SERIAL_SCT_RLD_n((n)))
#define ES1370_SERIAL_DAC_SEN_SET(r,n) \
    ((r) = ((r) & ~ES1370_SERIAL_DAC_SEN) | ES1370_SERIAL_DAC_SEN_n((n)))
#define ES1370_SERIAL_ADC_BITS_SET(r,n) \
    ((r) = ((r) & ~ES1370_SERIAL_ADC_BITS) | ES1370_SERIAL_ADC_BITS_n((n)))
#define ES1370_SERIAL_ADC_CHAN_SET(r,n) \
    ((r) = ((r) & ~ES1370_SERIAL_ADC_CHAN) | ES1370_SERIAL_ADC_CHAN_n((n)))
#define ES1370_SERIAL_PB2_BITS_SET(r,n) \
    ((r) = ((r) & ~ES1370_SERIAL_PB2_BITS) | ES1370_SERIAL_PB2_BITS_n((n)))
#define ES1370_SERIAL_PB2_CHAN_SET(r,n) \
    ((r) = ((r) & ~ES1370_SERIAL_PB2_CHAN) | ES1370_SERIAL_PB2_CHAN_n((n)))
#define ES1370_SERIAL_PB1_BITS_SET(r,n) \
    ((r) = ((r) & ~ES1370_SERIAL_PB1_BITS) | ES1370_SERIAL_PB1_BITS_n((n)))
#define ES1370_SERIAL_PB1_CHAN_SET(r,n) \
    ((r) = ((r) & ~ES1370_SERIAL_PB1_CHAN) | ES1370_SERIAL_PB1_CHAN_n((n)))

// ES1370_PB1FC
#define ES1370_PB1FC 0x24

// ES1370_PB2FC
#define ES1370_PB2FC 0x28

// ES1370_RECFC
#define ES1370_RECFC                      0x2C

#define ES1370_RECFC_CURR_COUNT_BIT       16
#define ES1370_RECFC_COUNT_BIT            0

#define ES1370_RECFC_CURR_COUNT_BITS      16
#define ES1370_RECFC_COUNT_BITS           16
#define ES1370_RECFC_CURR_COUNT_MASK \
    ((1U << ES1370_RECFC_CURR_COUNT_BITS)-1)
#define ES1370_RECFC_COUNT_MASK           ((1U << ES1370_RECFC_COUNT_BITS)-1)
#define ES1370_RECFC_CURR_COUNT \
    (ES1370_RECFC_CURR_COUNT_MASK << ES1370_RECFC_CURR_COUNT_BIT)
#define ES1370_RECFC_COUNT \
    (ES1370_RECFC_COUNT_MASK << ES1370_RECFC_COUNT_BIT)

#define ES1370_RECFC_CURR_COUNT_n(n)      ((n) << ES1370_RECFC_CURR_COUNT_BIT)
#define ES1370_RECFC_COUNT_n(n)           ((n) << ES1370_RECFC_COUNT_BIT)

#define ES1370_RECFC_CURR_COUNT_GET(n) \
    (((n) >> ES1370_RECFC_CURR_COUNT_BIT) & ES1370_RECFC_CURR_COUNT_MASK)
#define ES1370_RECFC_COUNT_GET(n) \
    (((n) >> ES1370_RECFC_COUNT_BIT) & ES1370_RECFC_COUNT_MASK)

#define ES1370_RECFC_CURR_COUNT_SET(r,n) \
    ((r) = ((r) & ~ES1370_RECFC_CURR_COUNT) | ES1370_RECFC_CURR_COUNT_n((n)))
#define ES1370_RECFC_COUNT_SET(r,n) \
    ((r) = ((r) & ~ES1370_RECFC_COUNT) | ES1370_RECFC_COUNT_n((n)))

// ES1370_PGC_PB1ADDR
#define ES1370_PGC_PB1ADDR 0x30

// ES1370_PGC_PB1DEF
#define ES1370_PGC_PB1DEF 0x34

// ES1370_PGC_PB2ADDR
#define ES1370_PGC_PB2ADDR 0x38

// ES1370_PGC_PB2DEF
#define ES1370_PGC_PB2DEF 0x3C

// ES1370_PGD_RECADDR
#define ES1370_PGD_RECADDR 0x30

// ES1370_PGD_RECDEF
#define ES1370_PGD_RECDEF 0x34

// ES1370_PGE_UARTDATA
#define ES1370_PGE_UARTDATA 0x30

// ES1370_PGE_PHANTOM_ADDR
#define ES1370_PGE_PHANTOM_ADDR 0x38

// ES1370_PGE_PHANTOM_SIZE
#define ES1370_PGE_PHANTOM_SIZE 0x3c

// ES1370_SMPREG_DAC1
#define ES1370_SMPREG_DAC1 0x70

// ES1370_SMPREG_DAC2
#define ES1370_SMPREG_DAC2 0x74

// ES1370_SMPREG_ADC
#define ES1370_SMPREG_ADC 0x78

// ES1370_SMPREG_VOL_ADC
#define ES1370_SMPREG_VOL_ADC 0x6c

// ES1370_SMPREG_VOL_DAC1
#define ES1370_SMPREG_VOL_DAC1 0x7c

// ES1370_SMPREG_VOL_DAC2
#define ES1370_SMPREG_VOL_DAC2 0x7e

// ES1370_SMPREG_TRUNC_N
#define ES1370_SMPREG_TRUNC_N 0x00

// ES1370_SMPREG_INT_REGS
#define ES1370_SMPREG_INT_REGS 0x01

// ES1370_SMPREG_ACCUM_FRAC
#define ES1370_SMPREG_ACCUM_FRAC 0x02

// ES1370_SMPREG_VFREQ_FRAC
#define ES1370_SMPREG_VFREQ_FRAC 0x03

