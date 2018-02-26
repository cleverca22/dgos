#include "usb_hid.h"
#include "dev_usb_ctl.h"
#include "keyboard.h"
#include "mouse.h"

#define USBHID_DEBUG 1
#if USBHID_DEBUG
#define USBHID_TRACE(...) printdbg("usbhid: " __VA_ARGS__)
#else
#define USBHID_TRACE(...) ((void)0)
#endif

static int const usb_hid_keybd_lookup[] = {
    // 00-03 <special>
    0,  // no event
    0,  // error rollover
    0,  // post failed
    0,  // undefined error

    // 04-1d A-Z
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',

    // 1e-26  1-9
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',

    // 27 0
    '0',

    // 28 Enter
    '\n',

    // 29 Esc
    '\x1b',

    // 2a Backspace
    '\b',

    // 2b Tab
    '\t',

    // 2c Spacebar
    ' ',

    // 2d -
    '-',

    // 2e =
    '=',

    // 2f [
    '[',

    // 30 ]
    ']',

    // 31 \ ..
    '\\',

    // 32 non-US # and ~
    '#',

    // 33 ;
    ';',

    // 34 '
    '\'',

    // 35 `
    '`',

    // 36 ,
    ',',

    // 37 .
    '.',

    // 38 /
    '/',

    // 39 Capslock
    KEYB_VK_CAPSLOCK,

    // 3a-45 F1-F12
    KEYB_VK_F1,
    KEYB_VK_F2,
    KEYB_VK_F3,
    KEYB_VK_F4,
    KEYB_VK_F5,
    KEYB_VK_F6,
    KEYB_VK_F7,
    KEYB_VK_F8,
    KEYB_VK_F9,
    KEYB_VK_F10,
    KEYB_VK_F11,
    KEYB_VK_F12,

    // 46 Printscreen
    KEYB_VK_PRNSCR,

    // 47 Scrolllock
    KEYB_VK_SCRLOCK,

    // 48 Pause
    KEYB_VK_PAUSE,

    // 49 Ins
    KEYB_VK_INS,

    // 4a Home
    KEYB_VK_HOME,

    // 4b Pgup
    KEYB_VK_PGUP,

    // 4c Del
    KEYB_VK_DEL,

    // 4d End
    KEYB_VK_END,

    // 4e Pgdn
    KEYB_VK_PGDN,

    // 4f Right
    KEYB_VK_RIGHT,

    // 50 Left
    KEYB_VK_LEFT,

    // 51 Down
    KEYB_VK_DOWN,

    // 52 Up
    KEYB_VK_UP,

    // 53 Numlock
    KEYB_VK_NUMLOCK,

    // 54 np/
    KEYB_VK_NUMPAD_SLASH,

    // 55 np*
    KEYB_VK_NUMPAD_STAR,

    // 56 np-
    KEYB_VK_NUMPAD_MINUS,

    // 57 np+
    KEYB_VK_NUMPAD_PLUS,

    // 58 npenter
    KEYB_VK_NUMPAD_ENTER,

    // 59-61 np1-np9
    KEYB_VK_NUMPAD_1,
    KEYB_VK_NUMPAD_2,
    KEYB_VK_NUMPAD_3,
    KEYB_VK_NUMPAD_4,
    KEYB_VK_NUMPAD_5,
    KEYB_VK_NUMPAD_6,
    KEYB_VK_NUMPAD_7,
    KEYB_VK_NUMPAD_8,
    KEYB_VK_NUMPAD_9,

    // 62 np0
    KEYB_VK_NUMPAD_0,

    // 63 np.
    KEYB_VK_NUMPAD_DOT,

    // 64 non-us \ and |
    '\\',

    // 65 Application
    KEYB_VK_RMENU,

    // 66 Power
    KEYB_VK_POWER,

    // 67 np=
    KEYB_VK_NUMPAD_EQUALS,

    // 68-73 F13-F24
    KEYB_VK_F13,
    KEYB_VK_F14,
    KEYB_VK_F15,
    KEYB_VK_F16,
    KEYB_VK_F17,
    KEYB_VK_F18,
    KEYB_VK_F19,
    KEYB_VK_F20,
    KEYB_VK_F21,
    KEYB_VK_F22,
    KEYB_VK_F23,
    KEYB_VK_F24,

    // 74 Execute
    KEYB_VK_EXECUTE,

    // 75 Help
    KEYB_VK_HELP,

    // 76 Menu
    KEYB_VK_MENU,

    // 77 Select
    KEYB_VK_SELECT,

    // 78 Stop
    KEYB_VK_STOP,

    // 79 Again
    KEYB_VK_AGAIN,

    // 7A Undo
    KEYB_VK_UNDO,

    // 7B Cut
    KEYB_VK_CUT,

    // 7C Copy
    KEYB_VK_COPY,

    // 7D Paste
    KEYB_VK_PASTE,

    // 7E Find
    KEYB_VK_FIND,

    // 7F Mute
    KEYB_VK_VOL_MUTE,

    // 80 Volume+
    KEYB_VK_VOL_UP,

    // 81 Volume-
    KEYB_VK_VOL_DOWN,

    // 82 Locking capslock
    KEYB_VK_LOCKING_CAPS,

    // 83 Locking numlock
    KEYB_VK_LOCKING_NUM,

    // 84 Locking scrolllock
    KEYB_VK_LOCKING_SCROLL,

    // 85 np,
    KEYB_VK_NUMPAD_COMMA,

    // 86 np=
    KEYB_VK_NUMPAD_EQUALS,

    // 87-8F International1-International9
    KEYB_VK_INTL_1,
    KEYB_VK_INTL_2,
    KEYB_VK_INTL_3,
    KEYB_VK_INTL_4,
    KEYB_VK_INTL_5,
    KEYB_VK_INTL_6,
    KEYB_VK_INTL_7,
    KEYB_VK_INTL_8,
    KEYB_VK_INTL_9,

    // 90-98 Lang1-Lang9
    KEYB_VK_LANG_1,
    KEYB_VK_LANG_2,
    KEYB_VK_LANG_3,
    KEYB_VK_LANG_4,
    KEYB_VK_LANG_5,
    KEYB_VK_LANG_6,
    KEYB_VK_LANG_7,
    KEYB_VK_LANG_8,
    KEYB_VK_LANG_9,

    // 99 Alternate erase
    KEYB_VK_NUMPAD_ALT_ERASE,

    // 9a SysRq/Attention
    KEYB_VK_SYSRQ,

    // 9b Cancel
    KEYB_VK_CANCEL,

    // 9c Clear
    KEYB_VK_CLEAR,

    // 9d Prior
    KEYB_VK_PRIOR,

    // 9e Return
    '\r',

    // 9f Separator
    KEYB_VK_SEPARATOR,

    // a0 Out
    KEYB_VK_OUT,

    // a1 Oper
    KEYB_VK_OPER,

    // a2 Clear/Again
    KEYB_VK_CLEAR,

    // a3 CrSel/Props
    KEYB_VK_CRSEL,

    // a4 ExSel
    KEYB_VK_EXSEL,

    // a5-af <reserved>
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,

    // b0 np00
    KEYB_VK_00,

    // b1 np000
    KEYB_VK_000,

    // b2 Thousands separator
    KEYB_VK_THOUSANDS_SEP,

    // b3 Decimal separator
    KEYB_VK_DECIMAL_SEP,

    // b4 Currency unit
    KEYB_VK_CURRENCY_UNIT,

    // b5 Currency sub-unit
    KEYB_VK_CURRENCY_SUBUNIT,

    // b6 np(
    KEYB_VK_NUMPAD_PAREN_OPEN,

    // b7 np)
    KEYB_VK_NUMPAD_PAREN_CLOSE,

    // b8 np{
    KEYB_VK_NUMPAD_BRACE_OPEN,

    // b9 np}
    KEYB_VK_NUMPAD_BRACE_CLOSE,

    // ba nptab
    KEYB_VK_NUMPAD_TAB,

    // bb npbackspace
    KEYB_VK_NUMPAD_BACKSPACE,

    // bc-c1 npA-npF
    KEYB_VK_NUMPAD_A,
    KEYB_VK_NUMPAD_B,
    KEYB_VK_NUMPAD_C,
    KEYB_VK_NUMPAD_D,
    KEYB_VK_NUMPAD_E,
    KEYB_VK_NUMPAD_F,

    // c2 npXor
    KEYB_VK_NUMPAD_XOR,

    // c3 np^
    KEYB_VK_NUMPAD_CARET,

    // c4 np%
    KEYB_VK_NUMPAD_PERCENT,

    // c5 np<
    KEYB_VK_NUMPAD_LESSTHAN,

    // c6 np>
    KEYB_VK_NUMPAD_GREATERTHAN,

    // c7 np&
    KEYB_VK_NUMPAD_AMPERSAND,

    // c8 np&&
    KEYB_VK_NUMPAD_AMPERSAND2,

    // c9 np|
    KEYB_VK_NUMPAD_PIPE,

    // ca np||
    KEYB_VK_NUMPAD_PIPE2,

    // cb np:
    KEYB_VK_NUMPAD_COLON,

    // cc np#
    KEYB_VK_NUMPAD_HASH,

    // cd npSpace
    KEYB_VK_NUMPAD_SPACE,

    // ce np@
    KEYB_VK_NUMPAD_AT,

    // cf np!
    KEYB_VK_NUMPAD_EXCLAMATION,

    // d0 npMemStore
    KEYB_VK_NUMPAD_MEM_STORE,

    // d1 npMemRecall
    KEYB_VK_NUMPAD_MEM_RECALL,

    // d2 npClear
    KEYB_VK_NUMPAD_CLEAR,

    // d3 npMemAdd
    KEYB_VK_NUMPAD_MEM_ADD,

    // d4 npMemSubtract
    KEYB_VK_NUMPAD_MEM_SUB,

    // d5 npMemMultiply
    KEYB_VK_NUMPAD_MEM_MUL,

    // d6 npMemDivide
    KEYB_VK_NUMPAD_MEM_DIV,

    // d7 npChgSign
    KEYB_VK_NUMPAD_SIGN,

    // d8 npClear
    KEYB_VK_NUMPAD_CLEAR,

    // d9 npClearEntry
    KEYB_VK_NUMPAD_CLEAR_ENTRY,

    // da npBinary
    KEYB_VK_NUMPAD_BINARY,

    // db npOctal
    KEYB_VK_NUMPAD_OCTAL,

    // dc npDecimal
    KEYB_VK_NUMPAD_DECIMAL,

    // dd npHexadecimal
    KEYB_VK_NUMPAD_HEX,

    // de-df <reserved>
    0,
    0,

    // e0 LeftCtrl
    KEYB_VK_LCTRL,

    // e1 LeftShift
    KEYB_VK_LSHIFT,

    // e2 LeftAlt
    KEYB_VK_LALT,

    // e3 LeftGui
    KEYB_VK_LGUI,

    // e4 RightCtrl
    KEYB_VK_RCTRL,

    // e5 RightShift
    KEYB_VK_RSHIFT,

    // e6 RightAlt
    KEYB_VK_RALT,

    // e7 RightGui
    KEYB_VK_RGUI

    // e8-ffff <reserved>
};

C_ASSERT(countof(usb_hid_keybd_lookup) == 0xe8);

class usb_hid_t : public usb_class_drv_t {
protected:
    static usb_hid_t usb_hid;

    enum struct hid_request_t : uint8_t {
        GET_REPORT = 0x1,
        GET_IDLE = 0x2,
        GET_PROTOCOL = 0x3,

        SET_REPORT = 0x9,
        SET_IDLE = 0xA,
        SET_PROTOCOL = 0xB
    };

    // usb_class_drv_t interface
    virtual bool probe(usb_config_helper *cfg, usb_bus_t *bus) override final;

private:
    bool set_protocol(usb_pipe_t &pipe, uint16_t proto);
    bool set_idle(usb_pipe_t &pipe, uint8_t report_id, uint8_t idle);

    static int keybd_poll_thread(void *p);
    int keybd_poll_thread();

    static int keybd_in_thread(void *p);
    int keybd_in_thread();

    static int mouse_poll_thread(void *p);
    int mouse_poll_thread();

    static int mouse_in_thread(void *p);
    int mouse_in_thread();

    void detect_keybd_changes();

    keybd_fsa_t fsa;

    uint8_t last_keybd_state[8];
    uint8_t this_keybd_state[8];

    uint8_t last_mouse_state[8];
    uint8_t this_mouse_state[8];

    ticketlock change_lock;
    usb_pipe_t keybd_control;
    usb_pipe_t keybd_in;
    usb_pipe_t mouse_control;
    usb_pipe_t mouse_in;
    uint8_t keybd_iface_idx;
    uint8_t mouse_iface_idx;
};

bool usb_hid_t::probe(usb_config_helper *cfg_hlp, usb_bus_t *bus)
{
    match_result match = match_config(
                cfg_hlp, 0, int(usb_class_t::hid), 1, -1, -1);

    if (!match.dev)
        return false;

    // Keyboard
    if (match.iface->iface_proto == 1) {
        keybd_iface_idx = match.iface_idx;

        bus->get_pipe(cfg_hlp->slot(), 0, keybd_control);

        set_protocol(keybd_control, 0);
        set_idle(keybd_control, 0, 0);

        // Try to find interrupt pipe
        usb_desc_ep const *ep_desc = cfg_hlp->find_ep(match.iface, 0);

        int tid = -1;

        if (ep_desc) {
            // Allocate interrupt IN endpoint and use it
            if (bus->alloc_pipe(cfg_hlp->slot(), ep_desc->ep_addr, keybd_in,
                                ep_desc->max_packet_sz, ep_desc->interval,
                                ep_desc->ep_attr)) {
                USBHID_TRACE("using interrupt pipe for keyboard input\n");
                tid = thread_create(keybd_in_thread, this, 0, false);
            }
        }

        // Fallback to polling if we didn't create the interrupt IN endpoint
        if (tid == -1) {
            USBHID_TRACE("using polling for keyboard input\n");
            tid = thread_create(keybd_poll_thread, this, 0, false);
        }

        thread_set_priority(tid, 16);

        return true;
    } else if (match.iface->iface_proto == 2) {
        mouse_iface_idx = match.iface_idx;

        bus->get_pipe(cfg_hlp->slot(), 0, mouse_control);

        set_protocol(mouse_control, 0);
        set_idle(mouse_control, 0, 0);

        usb_desc_ep const *ep_desc = cfg_hlp->find_ep(match.iface, 0);

        int tid = -1;

        if (ep_desc) {
            if (bus->alloc_pipe(cfg_hlp->slot(), ep_desc->ep_addr, mouse_in,
                                ep_desc->max_packet_sz, ep_desc->interval,
                                ep_desc->ep_attr)) {
                USBHID_TRACE("using interrupt pipe for mouse input\n");
                tid = thread_create(mouse_in_thread, this, 0, false);
            }
        }

        if (tid == -1) {
            USBHID_TRACE("using polling for mouse input\n");
            tid = thread_create(mouse_in_thread, this, 0, false);
        }

        thread_set_priority(tid, 16);

        return true;
    }

    return false;
}

bool usb_hid_t::set_protocol(usb_pipe_t &pipe, uint16_t proto)
{
    return pipe.send_default_control(
                uint8_t(usb_dir_t::OUT) |
                (uint8_t(usb_req_type::CLASS) << 5) |
                uint8_t(usb_req_recip_t::INTERFACE),
                uint8_t(hid_request_t::SET_PROTOCOL),
                proto, keybd_iface_idx, 0, nullptr) >= 0;
}

bool usb_hid_t::set_idle(usb_pipe_t &pipe, uint8_t report_id, uint8_t idle)
{
    return pipe.send_default_control(
                uint8_t(usb_dir_t::OUT) |
                (uint8_t(usb_req_type::CLASS) << 5) |
                uint8_t(usb_req_recip_t::INTERFACE),
                uint8_t(hid_request_t::SET_IDLE),
                report_id | (idle << 8), mouse_iface_idx, 0, nullptr) >= 0;
}

int usb_hid_t::keybd_poll_thread(void *p)
{
    return ((usb_hid_t*)p)->keybd_poll_thread();
}

int usb_hid_t::keybd_poll_thread()
{
    memset(last_keybd_state, 0, sizeof(last_keybd_state));

    while (true) {
        keybd_control.send_default_control(
                    uint8_t(usb_dir_t::IN) |
                    (uint8_t(usb_req_type::CLASS) << 5) |
                    uint8_t(usb_req_recip_t::INTERFACE),
                    1,
                    0, 0, sizeof(this_keybd_state), this_keybd_state);

        unique_lock<ticketlock> hold_change_lock(change_lock);
        detect_keybd_changes();

        thread_sleep_for(20);
    }

    return true;
}

int usb_hid_t::keybd_in_thread(void *p)
{
    return ((usb_hid_t*)p)->keybd_in_thread();
}

int usb_hid_t::keybd_in_thread()
{
    memset(last_keybd_state, 0, sizeof(last_keybd_state));

    while (true) {
        keybd_in.recv(this_keybd_state, sizeof(this_keybd_state));

        unique_lock<ticketlock> lock(change_lock);
        detect_keybd_changes();
    }

    return true;
}

int usb_hid_t::mouse_poll_thread(void *p)
{
    return ((usb_hid_t*)p)->mouse_poll_thread();
}

int usb_hid_t::mouse_poll_thread()
{
    memset(last_keybd_state, 0, sizeof(last_keybd_state));

    while (true) {
        mouse_control.send_default_control(
                    uint8_t(usb_dir_t::IN) |
                    (uint8_t(usb_req_type::CLASS) << 5) |
                    uint8_t(usb_req_recip_t::INTERFACE),
                    1,
                    0x100, mouse_iface_idx,
                    sizeof(this_mouse_state), this_mouse_state);

        unique_lock<ticketlock> hold_change_lock(change_lock);

        mouse_raw_event_t evt;
        evt.hdist = (int8_t)this_mouse_state[1];
        evt.vdist = -(int8_t)this_mouse_state[2];
        evt.wdist = (int8_t)this_mouse_state[3];
        evt.buttons = this_mouse_state[0];
        mouse_event(evt);
    }

    return true;
}

int usb_hid_t::mouse_in_thread(void *p)
{
    return ((usb_hid_t*)p)->mouse_in_thread();
}

int usb_hid_t::mouse_in_thread()
{
    memset(last_keybd_state, 0, sizeof(last_keybd_state));

    while (true) {
        mouse_in.recv(this_mouse_state, sizeof(this_mouse_state));

        unique_lock<ticketlock> hold_change_lock(change_lock);

        mouse_raw_event_t evt;
        evt.hdist = (int8_t)this_mouse_state[1];
        evt.vdist = -(int8_t)this_mouse_state[2];
        evt.wdist = (int8_t)this_mouse_state[3];
        evt.buttons = this_mouse_state[0];
        mouse_event(evt);
    }

    return true;
}

void usb_hid_t::detect_keybd_changes()
{
    static int modifier_vk[] = {
        KEYB_VK_LCTRL,
        KEYB_VK_LSHIFT,
        KEYB_VK_LALT,
        KEYB_VK_LGUI,
        KEYB_VK_RCTRL,
        KEYB_VK_RSHIFT,
        KEYB_VK_RALT,
        KEYB_VK_RGUI
    };

    // Detect modifier changes
    uint8_t modifier_changes = last_keybd_state[0] ^ this_keybd_state[0];

    // Generate modifier key up/down events
    for (int i = 0; i < 8; ++i) {
        int mask = 1 << i;
        int sign = (((this_keybd_state[0] & mask) != 0) * 2) - 1;

        if (modifier_changes & mask)
            fsa.deliver_vk(modifier_vk[i] * sign);
    }

    // Scan for keys released since last event
    for (int i = 2; i < 8; ++i) {
        uint8_t scancode = last_keybd_state[i];

        if (scancode < 4)
            continue;

        bool pressed = memchr(this_keybd_state + 2, scancode, 6);

        if (!pressed) {
            // Generate keyup event
            int vk = (scancode < countof(usb_hid_keybd_lookup))
                    ? usb_hid_keybd_lookup[scancode] : 0;

            fsa.deliver_vk(-vk);
        }
    }

    // Scan for keys pressed down in this event
    for (int i = 2; i < 8; ++i) {
        uint8_t scancode = this_keybd_state[i];

        if (scancode < 4)
            continue;

        // Detect edges
        if (memchr(last_keybd_state + 2, scancode, 6))
            continue;

        // Generate keydown event
        int vk = (scancode < countof(usb_hid_keybd_lookup))
                ? usb_hid_keybd_lookup[scancode] : 0;

        fsa.deliver_vk(vk);
    }

    memcpy(last_keybd_state, this_keybd_state, sizeof(last_keybd_state));
}

usb_hid_t usb_hid_t::usb_hid;
