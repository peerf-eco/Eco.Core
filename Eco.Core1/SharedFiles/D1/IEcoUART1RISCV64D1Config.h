/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IEcoUART1RISCV64D1Config
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию интерфейса IEcoUART1RISCV64D1Config
 *
 * 
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2023 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __I_ECO_UART_1_RISCV64_D1_CONFIG_H__
#define __I_ECO_UART_1_RISCV64_D1_CONFIG_H__

#include "IEcoBase1.h"

typedef union ECO_D1_H_UART_RBR_REGISTER {
    struct {
        uint32_t RBR    : 8;    /* Receiver Buffer Register */
        uint32_t RES    : 24;   /* Reserved. */
    } bits;
    uint32_t dword;
} ECO_D1_H_UART_RBR_REGISTER;

typedef union ECO_D1_H_UART_THR_REGISTER {
    struct {
        uint32_t THR    : 8;    /* Transmit Holding Register */
        uint32_t RES    : 24;   /* Reserved. */
    } bits;
    uint32_t dword;
} ECO_D1_H_UART_THR_REGISTER;

typedef union ECO_D1_H_UART_DLL_REGISTER {
    struct {
        uint32_t DLL    : 8;    /* Divisor Latch Low */
        uint32_t RES    : 24;   /* Reserved. */
    } bits;
    uint32_t dword;
} ECO_D1_H_UART_DLL_REGISTER;

typedef union ECO_D1_H_UART_DLH_REGISTER {
    struct {
        uint32_t DLH    : 8;    /* Divisor Latch High */
        uint32_t RES    : 24;   /* Reserved. */
    } bits;
    uint32_t dword;
} ECO_D1_H_UART_DLH_REGISTER;

typedef union ECO_D1_H_UART_IER_REGISTER {
    struct {
        uint32_t ERBFI          : 1;    /* Enable Received Data Available Interrupt */
        uint32_t ETBEI          : 1;    /* Enable Transmit Holding Register Empty Interrupt */
        uint32_t ELSI           : 1;    /* Enable Receiver Line Status Interrupt */
        uint32_t EDSSI          : 1;    /* Enable Modem Status Interrupt */
        uint32_t RS485_INT_EN   : 1;    /* RS485 Interrupt Enable */
        uint32_t RES0           : 2;    /* Reserved. */
        uint32_t PTIME          : 1;    /* Programmable THRE Interrupt Mode Enable */
        uint32_t RES1           : 24;   /* Reserved. */
    } bits;
    uint32_t dword;
} ECO_D1_H_UART_IER_REGISTER;

typedef union ECO_D1_H_UART_IIR_REGISTER {
    struct {
        uint32_t IID            : 4;    /* Interrupt ID */
        uint32_t RES0           : 2;    /* Reserved. */
        uint32_t FEFLAG         : 2;    /* FIFOs Enable Flag */
        uint32_t RES1           : 24;   /* Reserved. */
    } bits;
    uint32_t dword;
} ECO_D1_H_UART_IIR_REGISTER;

typedef union ECO_D1_H_UART_FCR_REGISTER {
    struct {
        uint32_t FIFOE          : 1;    /* Enable FIFOs */
        uint32_t RFIFOR         : 1;    /* RCVR FIFO Reset */
        uint32_t XFIFOR         : 1;    /* XMIT FIFO Reset */
        uint32_t DMAM           : 1;    /* DMA Mode */
        uint32_t TFT            : 2;    /* TX Empty Trigger */
        uint32_t RT             : 2;    /* RCVR Trigger */
        uint32_t RES            : 24;   /* Reserved. */
    } bits;
    uint32_t dword;
} ECO_D1_H_UART_FCR_REGISTER;

typedef union ECO_D1_H_UART_LCR_REGISTER {
    struct {
        uint32_t DLS            : 2;    /* Data Length Select */
        uint32_t STOP           : 1;    /* Number of stop bits */
        uint32_t PEN            : 1;    /* Parity Enable */
        uint32_t EPS            : 2;    /* Even Parity Select */
        uint32_t BC             : 1;    /* Break Control Bit */
        uint32_t DLAB           : 1;    /* Divisor Latch Acces Bit */
        uint32_t RES            : 24;   /* Reserved. */
    } bits;
    uint32_t dword;
} ECO_D1_H_UART_LCR_REGISTER;

typedef union ECO_D1_H_UART_MCR_REGISTER {
    struct {
        uint32_t DTR            : 1;    /* Data Terminal Ready */
        uint32_t RTS            : 1;    /* Request to Send */
        uint32_t RES0           : 2;    /* Reserved. */
        uint32_t LOOP           : 1;    /* Loop Back Mode */
        uint32_t AFCE           : 1;    /* Auto Flow Control Enable */
        uint32_t UART_FUNCTION  : 2;    /* Select IrDA or RS485 */
        uint32_t RES1           : 24;   /* Reserved. */
    } bits;
    uint32_t dword;
} ECO_D1_H_UART_MCR_REGISTER;

typedef union ECO_D1_H_UART_LSR_REGISTER {
    struct {
        uint32_t DR             : 1;    /* Data Ready */
        uint32_t OE             : 1;    /* Overrun Error */
        uint32_t PE             : 1;    /* Parity Error */
        uint32_t FE             : 1;    /* Framing Error */
        uint32_t BI             : 1;    /* Break Interrupt */
        uint32_t THRE           : 1;    /* TX Holding Register Empty */
        uint32_t TEMT           : 1;    /* Transmitter Empty */
        uint32_t FIFOERR        : 1;    /* RX Data Error in FIFO */
        uint32_t RES            : 24;   /* Reserved. */
    } bits;
    uint32_t dword;
} ECO_D1_H_UART_LSR_REGISTER;

typedef union ECO_D1_H_UART_MSR_REGISTER {
    struct {
        uint32_t DCTS           : 1;    /* Delta Clear to Send */
        uint32_t DDSR           : 1;    /* Delta Data Set Ready */
        uint32_t TERI           : 1;    /* Trailing Edge Ring Indicator */
        uint32_t DDCD           : 1;    /* Delta Data Carrier Detect */
        uint32_t CTS            : 1;    /* Line State of Clear To Send */
        uint32_t DSR            : 1;    /* Line State of Data Set Ready */
        uint32_t RI             : 1;    /* Line State of Ring Indicator */
        uint32_t DCD            : 1;    /* Line State of Data Carrier Detect */
        uint32_t RES            : 24;   /* Reserved. */
    } bits;
    uint32_t dword;
} ECO_D1_H_UART_MSR_REGISTER;

typedef union ECO_D1_H_UART_SCH_REGISTER {
    struct {
        uint32_t SCRATCH_REG    : 8;    /* Scratch Register */
        uint32_t RES            : 24;   /* Reserved. */
    } bits;
    uint32_t dword;
} ECO_D1_H_UART_SCH_REGISTER;

typedef union ECO_D1_H_UART_USR_REGISTER {
    struct {
        uint32_t BUSY           : 1;    /* UART Busy Bit */
        uint32_t TFNF           : 1;    /* TX FIFO Not Full */
        uint32_t TFE            : 1;    /* TX FIFO Empty */
        uint32_t RFNE           : 1;    /* RX FIFO Not Empty */
        uint32_t RFF            : 1;    /* RX FIFO Full */
        uint32_t RES            : 27;   /* Reserved. */
    } bits;
    uint32_t dword;
} ECO_D1_H_UART_USR_REGISTER;

typedef union ECO_D1_H_UART_TFL_REGISTER {
    struct {
        uint32_t TFL            : 9;    /* TX FIFO Level */
        uint32_t RES            : 23;   /* Reserved. */
    } bits;
    uint32_t dword;
} ECO_D1_H_UART_TFL_REGISTER;

typedef union ECO_D1_H_UART_RFL_REGISTER {
    struct {
        uint32_t RFL            : 9;    /* RX FIFO Level */
        uint32_t RES            : 23;   /* Reserved. */
    } bits;
    uint32_t dword;
} ECO_D1_H_UART_RFL_REGISTER;


/* Определение базовых адресов регистров UART для D1 H (9.2.5 Register List) */
#define ECO_D1_H_UART0     0x02500000UL
#define ECO_D1_H_UART1     0x02500400UL
#define ECO_D1_H_UART2     0x02500800UL
#define ECO_D1_H_UART3     0x02500C00UL
#define ECO_D1_H_UART4     0x02501000UL
#define ECO_D1_H_UART5     0x02501400UL

typedef struct ECO_D1_H_UART_REGISTER_MAP {
    /*       Register Name         Description                                                  Offset      */
    union {
        uint32_t RBR;           /* Receive Buffer Register,                     Address offset: 0x00        */
        uint32_t THR;           /* Transmit Holding Register,                   Address offset: 0x00        */
        uint32_t DLL;           /* Divisor Latch Low Register,                  Address offset: 0x00        */
    } Offset0x0000;
    union {
        uint32_t DLH;           /* Divisor Latch High Register,                 Address offset: 0x04        */
        uint32_t IER;           /* Interrupt Enable Register,                   Address offset: 0x04        */
    } Offset0x0004;
    union {
        uint32_t IIR;           /* Interrupt Identity Register,                 Address offset: 0x08        */
        uint32_t FCR;           /* FIFO Control Register,                       Address offset: 0x08        */
    } Offset0x0008;
    uint32_t LCR;               /* Line Control Register,                       Address offset: 0x0C        */
    uint32_t MCR;               /* Modem Control Register,                      Address offset: 0x10        */
    uint32_t LSR;               /* Line Status Register,                        Address offset: 0x14        */
    uint32_t MSR;               /* Modem Status Register,                       Address offset: 0x18        */
    uint32_t SCH;               /* Scratch Register,                            Address offset: 0x1C        */
    uint32_t RESERVED0[23];     /* Reserved,                                    Address offset: 0x20-0x78   */
    uint32_t USR;               /* Status Register,                             Address offset: 0x7C        */
    uint32_t TFL;               /* Transmit FIFO Level Register,                Address offset: 0x80        */
    uint32_t RFL;               /* Receive FIFO Level Register,                 Address offset: 0x84        */
    uint32_t HSK;               /* DMA Handshake Configuration Register,        Address offset: 0x88        */
    uint32_t DMA_REQ_EN;        /* DMA Request Enable Register,                 Address offset: 0x8C        */
    uint32_t RESERVED1[5];      /* Reserved,                                    Address offset: 0x90-0xA0   */
    uint32_t HALT;              /* Halt TX Register,                            Address offset: 0xA4        */
    uint32_t RESERVED2[2];      /* Reserved,                                    Address offset: 0xA8-0xAC   */
    uint32_t DBG_DLL;           /* Debug DLL Register,                          Address offset: 0xB0        */
    uint32_t DBG_DLH;           /* Debug DLH Register,                          Address offset: 0xB4        */
    uint32_t RESERVED3[2];      /* Reserved,                                    Address offset: 0xA8-0xAC   */
    uint32_t A_FCC;             /* FIFO Clock Control Register,                 Address offset: 0xF0        */
    uint32_t RESERVED4[3];      /* Reserved,                                    Address offset: 0xF4-0xFC   */
    uint32_t A_RXDMA_CTRL;      /* RXDMA Control Register,                      Address offset: 0x100       */
    uint32_t A_RXDMA_STR;       /* RXDMA Start Register,                        Address offset: 0x104       */
    uint32_t A_RXDMA_STA;       /* RXDMA Status Register,                       Address offset: 0x108       */
    uint32_t A_RXDMA_LMT;       /* RXDMA Limit Register,                        Address offset: 0x10C       */
    uint32_t A_RXDMA_SADDRL;    /* RXDMA Buffer Start Address Дщц Register,     Address offset: 0x110       */
    uint32_t A_RXDMA_SADDRH;    /* RXDMA Buffer Start Address High Register,    Address offset: 0x114       */
    uint32_t A_RXDMA_BL;        /* RXDMA Buffer Length Register,                Address offset: 0x118       */
    uint32_t RESERVED5;         /* Reserved,                                    Address offset: 0x11C       */
    uint32_t A_RXDMA_IE;        /* RXDMA Interrupt Enable Register,             Address offset: 0x120       */
    uint32_t A_RXDMA_IS;        /* RXDMA Interrupt Status Register,             Address offset: 0x124       */
    uint32_t A_RXDMA_WADDRL;    /* RXDMA Write Address Low Register,            Address offset: 0x128       */
    uint32_t A_RXDMA_WADDRH;    /* RXDMA Write Address High Register,           Address offset: 0x12C       */
    uint32_t A_RXDMA_RADDRL;    /* RXDMA Read Address Low Register,             Address offset: 0x130       */
    uint32_t A_RXDMA_RADDRH;    /* RXDMA Read Address High Register,            Address offset: 0x134       */
    uint32_t A_RXDMA_DCNT;      /* RXDMA Data Count Register,                   Address offset: 0x138       */
} ECO_D1_H_UART_REGISTER_MAP;

#define ECO_UART_REGISTER_MAP ECO_D1_H_UART_REGISTER_MAP

typedef struct ECO_UART_CONFIG_DESCRIPTOR {
    union {
        uint32_t BaseAddress;
        struct ECO_UART_REGISTER_MAP* Map;
    } Register;
    uint32_t Fclk;
} ECO_UART_CONFIG_DESCRIPTOR;

/* IEcoUART1RISCV64D1Config IID = {BCF99D0D-0259-4AE9-9368-D8805C95DB09} */
#ifndef __IID_IEcoUART1RISCV64D1Config
static const UGUID IID_IEcoUART1RISCV64D1Config = { 0x01, 0x10, {0xBC, 0xF9, 0x9D, 0x0D, 0x02, 0x59, 0x4A, 0xE9, 0x93, 0x68, 0xD8, 0x80, 0x5C, 0x95, 0xDB, 0x09}};
#endif /* __IID_IEcoUART1RISCV64D1Config */

typedef struct IEcoUART1RISCV64D1Config* IEcoUART1RISCV64D1ConfigPtr_t;

typedef struct IEcoUART1RISCV64D1ConfigVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoUART1RISCV64D1ConfigPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoUART1RISCV64D1ConfigPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoUART1RISCV64D1ConfigPtr_t me);

    /* IEcoUART1RISCV64D1Config */
    int16_t (ECOCALLMETHOD *set_ConfigDescriptor)(/* in */ IEcoUART1RISCV64D1ConfigPtr_t me, /* in */ ECO_UART_CONFIG_DESCRIPTOR* config);
    ECO_UART_CONFIG_DESCRIPTOR* (ECOCALLMETHOD *get_ConfigDescriptor)(/* in */ IEcoUART1RISCV64D1ConfigPtr_t me);

} IEcoUART1RISCV64D1ConfigVTbl, *IEcoUART1RISCV64D1ConfigVTblPtr_t;

interface IEcoUART1RISCV64D1Config {
    struct IEcoUART1RISCV64D1ConfigVTbl *pVTbl;
} IEcoUART1RISCV64D1Config;

#endif /* __I_ECO_UART_1_RISCV64_D1_CONFIG_H__ */
