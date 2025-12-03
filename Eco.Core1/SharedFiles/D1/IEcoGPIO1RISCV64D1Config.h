/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IEcoGPIO1RISCV64D1Config
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию интерфейса IEcoGPIO1RISCV64D1Config
 *
 * 
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2023 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __I_ECO_GPIO_1_RISC_V_64_D_1_CONFIG_H__
#define __I_ECO_GPIO_1_RISC_V_64_D_1_CONFIG_H__

#include "IEcoBase1.h"

typedef union ECO_D1_H_GPIO_PB_CFG0_REGISTER {
    struct {
        uint32_t PB0_SELECT     : 4;    /* PB0 Select,   Bit 3:0    */
        uint32_t PB1_SELECT     : 4;    /* PB1 Select,   Bit 7:4    */
        uint32_t PB2_SELECT     : 4;    /* PB2 Select,   Bit 11:8   */
        uint32_t PB3_SELECT     : 4;    /* PB3 Select,   Bit 15:12  */
        uint32_t PB4_SELECT     : 4;    /* PB4 Select,   Bit 19:16  */
        uint32_t PB5_SELECT     : 4;    /* PB5 Select,   Bit 23:20  */
        uint32_t PB6_SELECT     : 4;    /* PB6 Select,   Bit 27:24  */
        uint32_t PB7_SELECT     : 4;    /* PB7 Select,   Bit 31:28  */
    } bits;
    uint32_t dword;
} ECO_D1_H_GPIO_PB_CFG0_REGISTER;

typedef union ECO_D1_H_GPIO_PB_CFG1_REGISTER {
    struct {
        uint32_t PB8_SELECT     : 4;    /* PB0 Select,   Bit 3:0    */
        uint32_t PB9_SELECT     : 4;    /* PB1 Select,   Bit 7:4    */
        uint32_t PB10_SELECT    : 4;    /* PB2 Select,   Bit 11:8   */
        uint32_t PB11_SELECT    : 4;    /* PB3 Select,   Bit 15:12  */
        uint32_t PB12_SELECT    : 4;    /* PB4 Select,   Bit 19:16  */
        uint32_t RES            : 12;   /* Reserved,     Bit 31:20  */
    } bits;
    uint32_t dword;
} ECO_D1_H_GPIO_PB_CFG1_REGISTER;

/* Определение базовых адресов регистров GPIO для D1 H (9.7.4 Register List) */
#define ECO_D1_H_GPIO     0x02000000UL

typedef struct ECO_D1_H_GPIO_REGISTER_MAP {
    /*       Register Name         Description                                                  Offset      */
    uint32_t RESERVED0[12];     /* Reserved,                                    Address offset: 0x00-0x2C   */
    uint32_t PB_CFG0;           /* PB Configure Register 0,                     Address offset: 0x30        */
    uint32_t PB_CFG1;           /* PB Configure Register 1,                     Address offset: 0x34        */
    uint32_t RESERVED1[2];      /* Reserved,                                    Address offset: 0x38-0x3C   */
    uint32_t PB_DAT;            /* PB Data Register,                            Address offset: 0x40        */
    uint32_t PB_DRV0;           /* PB Multi_Driving Register 0,                 Address offset: 0x44        */
    uint32_t PB_DRV1;           /* PB Multi_Driving Register 1,                 Address offset: 0x48        */
    uint32_t RESERVED2[2];      /* Reserved,                                    Address offset: 0x4C-0x50   */
    uint32_t PB_PULL0;          /* PB Pull Register 0,                          Address offset: 0x54        */
    uint32_t RESERVED3[2];      /* Reserved,                                    Address offset: 0x58-0x5C   */
    uint32_t PC_CFG0;           /* PC Configure Register 0,                     Address offset: 0x60        */
    uint32_t RESERVED4[3];      /* Reserved,                                    Address offset: 0x64-0x6C   */
    uint32_t PC_DAT;            /* PC Data Register,                            Address offset: 0x70        */
    uint32_t PC_DRV0;           /* PC Multi_Driving Register 0,                 Address offset: 0x74        */
    uint32_t RESERVED5[3];      /* Reserved,                                    Address offset: 0x78-0x80   */
    uint32_t PC_PULL0;          /* PC Pull Register 0,                          Address offset: 0x84        */
    uint32_t RESERVED6[2];      /* Reserved,                                    Address offset: 0x88-0x8C   */
    uint32_t PD_CFG0;           /* PD Configure Register 0,                     Address offset: 0x90        */
    uint32_t PD_CFG1;           /* PD Configure Register 1,                     Address offset: 0x94        */
    uint32_t PD_CFG2;           /* PD Configure Register 2,                     Address offset: 0x98        */
    uint32_t RESERVED7;         /* Reserved,                                    Address offset: 0x9C        */
    uint32_t PD_DAT;            /* PD Data Register,                            Address offset: 0xA0        */
    uint32_t PD_DRV0;           /* PD Multi_Driving Register 0,                 Address offset: 0xA4        */
    uint32_t PD_DRV1;           /* PD Multi_Driving Register 1,                 Address offset: 0xA8        */
    uint32_t PD_DRV2;           /* PD Multi_Driving Register 2,                 Address offset: 0xAC        */
    uint32_t RESERVED8;         /* Reserved,                                    Address offset: 0xB0        */
    uint32_t PD_PULL0;          /* PD Pull Register 0,                          Address offset: 0xB4        */
    uint32_t PD_PULL1;          /* PD Pull Register 1,                          Address offset: 0xB8        */
    uint32_t RESERVED9;         /* Reserved,                                    Address offset: 0xBC        */
    uint32_t PE_CFG0;           /* PE Configure Register 0,                     Address offset: 0xC0        */
    uint32_t PE_CFG1;           /* PE Configure Register 1,                     Address offset: 0xC4        */
    uint32_t RESERVED10[2];     /* Reserved,                                    Address offset: 0xC8-0xCC   */
    uint32_t PE_DAT;            /* PE Data Register,                            Address offset: 0xD0        */
    uint32_t PE_DRV0;           /* PE Multi_Driving Register 0,                 Address offset: 0xD4        */
    uint32_t PE_DRV1;           /* PE Multi_Driving Register 1,                 Address offset: 0xD8        */
    uint32_t RESERVED11[2];     /* Reserved,                                    Address offset: 0xDC-0xE0   */
    uint32_t PE_PULL0;          /* PE Pull Register 0,                          Address offset: 0xE4        */
    uint32_t RESERVED12[2];     /* Reserved,                                    Address offset: 0xE8-0xEC   */
    uint32_t PF_CFG0;           /* PF Configure Register 0,                     Address offset: 0xF0        */
    uint32_t RESERVED13[3];     /* Reserved,                                    Address offset: 0xF4-0xFC   */
    uint32_t PF_DAT;            /* PF Data Register,                            Address offset: 0x100       */
    uint32_t PF_DRV0;           /* PF Multi_Driving Register 0,                 Address offset: 0x104       */
    uint32_t RESERVED14[3];     /* Reserved,                                    Address offset: 0x108-0x110 */
    uint32_t PF_PULL0;          /* PF Pull Register 0,                          Address offset: 0x114       */
    uint32_t RESERVED15[12];    /* Reserved,                                    Address offset: 0x00-0x2C   */
    uint32_t PG_CFG0;           /* PG Configure Register 0,                     Address offset: 0x120       */
    uint32_t PG_CFG1;           /* PG Configure Register 1,                     Address offset: 0x124       */
    uint32_t RESERVED16[2];     /* Reserved,                                    Address offset: 0x128-0x12C */
    uint32_t PG_DAT;            /* PG Data Register,                            Address offset: 0x130       */
    uint32_t PG_DRV0;           /* PG Multi_Driving Register 0,                 Address offset: 0x134       */
    uint32_t PG_DRV1;           /* PG Multi_Driving Register 1,                 Address offset: 0x138       */
    uint32_t RESERVED17;        /* Reserved,                                    Address offset: 0x13C       */
    uint32_t PG_DRV3;           /* PG Multi_Driving Register 3,                 Address offset: 0x140       */
    uint32_t PG_PULL0;          /* PG Pull Register 0,                          Address offset: 0x144       */
    uint32_t RESERVED18[55];    /* Reserved,                                    Address offset: 0x148-0x21C */
    uint32_t PB_EINT_CFG0;      /* PB External Interrupt Configure Register 0,  Address offset: 0x220       */
    uint32_t RESERVED19[3];     /* Reserved,                                    Address offset: 0x224-0x22C */
    uint32_t PB_EINT_CTL;       /* PB External Interrupt Control Register,      Address offset: 0x230       */
    uint32_t PB_EINT_STATUS;    /* PB External Interrupt Status Register,       Address offset: 0x234       */
    uint32_t PB_EINT_DEB;       /* PB External Interrupt Debounce Register,     Address offset: 0x238       */
    uint32_t RESERVED20;        /* Reserved,                                    Address offset: 0x23C       */
    uint32_t PC_EINT_CFG0;      /* PC External Interrupt Configure Register 0,  Address offset: 0x240       */
    uint32_t RESERVE21[3];      /* Reserved,                                    Address offset: 0x244-0x24C */
    uint32_t PC_EINT_CTL;       /* PC External Interrupt Control Register,      Address offset: 0x250       */
    uint32_t PC_EINT_STATUS;    /* PC External Interrupt Status Register,       Address offset: 0x254       */
    uint32_t PC_EINT_DEB;       /* PC External Interrupt Debounce Register,     Address offset: 0x258       */
    uint32_t RESERVED22;        /* Reserved,                                    Address offset: 0x25C       */
    uint32_t PD_EINT_CFG0;      /* PD External Interrupt Configure Register 0,  Address offset: 0x260       */
    uint32_t PD_EINT_CFG1;      /* PD External Interrupt Configure Register 1,  Address offset: 0x264       */
    uint32_t PD_EINT_CFG2;      /* PD External Interrupt Configure Register 2,  Address offset: 0x268       */
    uint32_t RESERVE23;         /* Reserved,                                    Address offset: 0x26C       */
    uint32_t PD_EINT_CTL;       /* PD External Interrupt Control Register,      Address offset: 0x270       */
    uint32_t PD_EINT_STATUS;    /* PD External Interrupt Status Register,       Address offset: 0x274       */
    uint32_t PD_EINT_DEB;       /* PD External Interrupt Debounce Register,     Address offset: 0x278       */
    uint32_t RESERVED24;        /* Reserved,                                    Address offset: 0x27C       */
    uint32_t PE_EINT_CFG0;      /* PE External Interrupt Configure Register 0,  Address offset: 0x280       */
    uint32_t PE_EINT_CFG1;      /* PE External Interrupt Configure Register 1,  Address offset: 0x284       */
    uint32_t RESERVE25[2];      /* Reserved,                                    Address offset: 0x288-0x28C */
    uint32_t PE_EINT_CTL;       /* PE External Interrupt Control Register,      Address offset: 0x290       */
    uint32_t PE_EINT_STATUS;    /* PE External Interrupt Status Register,       Address offset: 0x294       */
    uint32_t PE_EINT_DEB;       /* PE External Interrupt Debounce Register,     Address offset: 0x298       */
    uint32_t RESERVED26;        /* Reserved,                                    Address offset: 0x29C       */
    uint32_t PF_EINT_CFG0;      /* PF External Interrupt Configure Register 0,  Address offset: 0x2A0       */
    uint32_t RESERVE27[3];      /* Reserved,                                    Address offset: 0x2A4-0x2AC */
    uint32_t PF_EINT_CTL;       /* PF External Interrupt Control Register,      Address offset: 0x2B0       */
    uint32_t PF_EINT_STATUS;    /* PF External Interrupt Status Register,       Address offset: 0x2B4       */
    uint32_t PF_EINT_DEB;       /* PF External Interrupt Debounce Register,     Address offset: 0x2B8       */
    uint32_t RESERVED28;        /* Reserved,                                    Address offset: 0x2BC       */
    uint32_t PG_EINT_CFG0;      /* PG External Interrupt Configure Register 0,  Address offset: 0x2C0       */
    uint32_t PG_EINT_CFG1;      /* PG External Interrupt Configure Register 1,  Address offset: 0x2C4       */
    uint32_t RESERVE29[2];      /* Reserved,                                    Address offset: 0x2C8-0x2CC */
    uint32_t PG_EINT_CTL;       /* PG External Interrupt Control Register,      Address offset: 0x2D0       */
    uint32_t PG_EINT_STATUS;    /* PG External Interrupt Status Register,       Address offset: 0x2D4       */
    uint32_t PG_EINT_DEB;       /* PG External Interrupt Debounce Register,     Address offset: 0x2D8       */
    uint32_t RESERVE30[26];     /* Reserved,                                    Address offset: 0x2DC-0x33C */
    uint32_t PIO_POW_MOD_SEL;   /* PIO Group Withstand Voltage Mode Select Reg, Address offset: 0x340       */
    uint32_t PIO_POW_MS_CTL;    /* PIO Group Ws Voltage Mode Select Control Reg,Address offset: 0x344       */
    uint32_t PIO_POW_VAL;       /* PIO Group Voltage Value Register,            Address offset: 0x348       */
    uint32_t RESERVED31;        /* Reserved,                                    Address offset: 0x44C       */
    uint32_t PIO_POW_VOL_SEL_CTL;/*PIO Group Power Voltage Select Control Reg,  Address offset: 0x350       */
} ECO_D1_H_GPIO_REGISTER_MAP;

#define ECO_GPIO_REGISTER_MAP ECO_D1_H_GPIO_REGISTER_MAP

typedef struct ECO_GPIO_CONFIG_DESCRIPTOR {
    union {
        uint32_t BaseAddress;
        struct ECO_GPIO_REGISTER_MAP* Map;
    } Register;
    uint8_t LogicalPinMask[64];
} ECO_GPIO_CONFIG_DESCRIPTOR;

/* IEcoGPIO1RISCV64D1Config IID = {2C25EC29-43B4-4E45-83B4-3375FC35C4D6} */
#ifndef __IID_IEcoGPIO1RISCV64D1Config
static const UGUID IID_IEcoGPIO1RISCV64D1Config = { 0x01, 0x10, {0x2C, 0x25, 0xEC, 0x29, 0x43, 0xB4, 0x4E, 0x45, 0x83, 0xB4, 0x33, 0x75, 0xFC, 0x35, 0xC4, 0xD6}};
#endif /* __IID_IEcoGPIO1RISCV64D1Config */

typedef struct IEcoGPIO1RISCV64D1Config* IEcoGPIO1RISCV64D1ConfigPtr_t;

typedef struct IEcoGPIO1RISCV64D1ConfigVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoGPIO1RISCV64D1ConfigPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoGPIO1RISCV64D1ConfigPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoGPIO1RISCV64D1ConfigPtr_t me);

    /* IEcoGPIO1RISCV64D1Config */
    int16_t (ECOCALLMETHOD *set_ConfigDescriptor)(/* in */ IEcoGPIO1RISCV64D1ConfigPtr_t me, /* in */ ECO_GPIO_CONFIG_DESCRIPTOR* config);
    ECO_GPIO_CONFIG_DESCRIPTOR* (ECOCALLMETHOD *get_ConfigDescriptor)(/* in */ IEcoGPIO1RISCV64D1ConfigPtr_t me);
    int16_t (ECOCALLMETHOD *set_LogicalPinNumber)(/* in */ IEcoGPIO1RISCV64D1ConfigPtr_t me, /* in */ uint8_t LogicalPinNumber, /* in */ uint8_t BankNumber, /* in */ uint32_t PinNumber);

} IEcoGPIO1RISCV64D1ConfigVTbl, *IEcoGPIO1RISCV64D1ConfigVTblPtr_t;

interface IEcoGPIO1RISCV64D1Config {
    struct IEcoGPIO1RISCV64D1ConfigVTbl *pVTbl;
} IEcoGPIO1RISCV64D1Config;

#endif /* __I_ECO_GPIO_1_RISC_V_64_D_1_CONFIG_H__ */
