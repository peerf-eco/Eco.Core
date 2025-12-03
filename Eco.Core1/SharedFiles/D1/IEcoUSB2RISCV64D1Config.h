/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IEcoUSB2RISCV64D1Config
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию интерфейса IEcoUSB2RISCV64D1Config
 *
 * 
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2023 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __I_ECO_USB_2_RISCV64_D1_CONFIG_H__
#define __I_ECO_USB_2_RISCV64_D1_CONFIG_H__

#include "IEcoBase1.h"


/* Определение базовых адресов регистров USB для D1 H (9.6.4 Register List) */
#define ECO_D1_H_USB0       0x04100000UL
#define ECO_D1_H_USB1       0x04200000UL

typedef struct ECO_D1_H_USB_REGISTER_MAP {
    /*       Register Name                 Description                                                      Offset      */
    uint16_t E_CAPLENGTH;               /* EHCI Capability Register Length Register,        Address offset: 0x00        */
    uint16_t E_HCIVERSION;              /* EHCI Host Interface Version Number Register,     Address offset: 0x02        */
    uint32_t E_HCSPARAMS;               /* EHCI Host Control Structuaral Parameter Register,Address offset: 0x04        */
    uint32_t E_HCCPARAMS;               /* EHCI Host Control Capability Parameter Register, Address offset: 0x08        */
    uint32_t E_HCSPPORTROUTE;           /* EHCI Companion Port Route Description,           Address offset: 0x0C        */
    uint32_t E_USBCMD;                  /* EHCI USB Command Register,                       Address offset: 0x10        */
    uint32_t E_USBSTS;                  /* EHCI USB Status Register,                        Address offset: 0x14        */
    uint32_t E_USBINTR;                 /* EHCI USB Interrupt Enable Register,              Address offset: 0x18        */
    uint32_t E_FRINDEX;                 /* EHCI USB Frame Index Register,                   Address offset: 0x1C        */
    uint32_t E_CTRLDSSEGMENT;           /* EHCI 4G Segment Selector Register,               Address offset: 0x20        */
    uint32_t E_PERIODICLISTBASE;        /* EHCI Frame List Base Address Register,           Address offset: 0x24        */
    uint32_t E_ASYNCLISTADDR;           /* EHCI Next Asynchronous List Address Register,    Address offset: 0x28        */
    uint32_t RESERVED0;                 /* Reserved,                                        Address offset: 0x2C        */
    uint32_t E_TIMERINTERRUPT;          /* EHCI Timer Configured And Standby Interrupt Status Register, Address offset: 0x30        */
    uint32_t RESERVED1[10];             /* Reserved,                                        Address offset: 0x2C-0x4C   */
    uint32_t E_CONFIGFLAG;              /* EHCI Configured Flag Register,                   Address offset: 0x50        */
    uint32_t E_PORTSC;                  /* EHCI Port Status/Control Register,               Address offset: 0x54        */
    uint32_t RESERVED2[235];            /* Reserved,                                        Address offset: 0x58-0x400  */
    uint32_t O_HcControl;               /* OHCI Control Register,                           Address offset: 0x404       */
    uint32_t O_HcCommandStatus;         /* OHCI Command Status Register,                    Address offset: 0x408       */
    uint32_t O_HcInterruptStatus;       /* OHCI Interrupt Status Register,                  Address offset: 0x40C       */
    uint32_t O_HcInterruptEnable;       /* OHCI Interrupt Enable Register,                  Address offset: 0x410       */
    uint32_t O_HcInterruptDisable;      /* OHCI Interrupt Disable Register,                 Address offset: 0x414       */
    uint32_t O_HcHCCA;                  /* OHCI HCCA Base,                                  Address offset: 0x418       */
    uint32_t O_HcPeriodCurrentED;       /* OHCI Period Current ED Base,                     Address offset: 0x41C       */
    uint32_t O_HcControlHeadED;         /* OHCI Control Head ED Base,                       Address offset: 0x420       */
    uint32_t O_HcControlCurrentED;      /* OHCI Control Current ED Base,                    Address offset: 0x424       */
    uint32_t O_HcBulkHeadED;            /* OHCI Bulk Head ED Base,                          Address offset: 0x428       */
    uint32_t O_HcBulkCurrentED;         /* OHCI Bulk Current ED Base,                       Address offset: 0x42C       */
    uint32_t O_HcDoneHead;              /* OHCI Done Head Base,                             Address offset: 0x430       */
    uint32_t O_HcFmInterval;            /* OHCI Frame Interval Register,                    Address offset: 0x434       */
    uint32_t O_HcFmRemaining;           /* OHCI Frame Remaining Register,                   Address offset: 0x438       */
    uint32_t O_HcFmNumber;              /* OHCI Frame Number Register,                      Address offset: 0x43C       */
    uint32_t O_HcPeriodicStart;         /* OHCI Periodic Start Register,                    Address offset: 0x440       */
    uint32_t O_HcLSThreshold;           /* OHCI LS Threshold Register,                      Address offset: 0x444       */
    uint32_t O_HcRhDescriptorA;         /* OHCI Root Hub Descriptor Register A,             Address offset: 0x448       */
    uint32_t O_HcRhDescriptorB;         /* OHCI Root Hub Descriptor Register B,             Address offset: 0x44C       */
    uint32_t O_HcRhStatus;              /* OHCI Root Hub Status Register,                   Address offset: 0x450       */
    uint32_t O_HcRhPortStatus;          /* OHCI Root Hub Port Status Register,              Address offset: 0x454       */
    uint32_t RESERVED3[234];            /* Reserved,                                        Address offset: 0x458-0x7FC */
    uint32_t HCI_Interface;             /* HCI Interface Register,                          Address offset: 0x800       */
    uint32_t RESERVED4;                 /* Reserved,                                        Address offset: 0x804       */
    uint32_t HCI_CTRL3;                 /* HCI Control Register,                            Address offset: 0x808       */
    uint32_t PHY_Control;               /* PHY Control Register,                            Address offset: 0x810       */
    uint32_t RESERVED5[4];              /* Reserved,                                        Address offset: 0x814-0x820 */
    uint32_t PHY_STATUS;                /* PHY Status Register,                             Address offset: 0x824       */
    uint32_t HCI_SPDCR;                 /* HCI SIE Port Disable Control Register,           Address offset: 0x828       */
} ECO_D1_H_USB_REGISTER_MAP;

#define ECO_USB_REGISTER_MAP ECO_D1_H_USB_REGISTER_MAP

typedef struct ECO_USB_CONFIG_DESCRIPTOR {
    union {
        uint32_t BaseAddress;
        struct ECO_USB_REGISTER_MAP* Map;
    } Register;
} ECO_USB_CONFIG_DESCRIPTOR;

/* IEcoUSB2RISCV64D1Config IID = {4DA27ECA-4419-4848-95C9-40A11B73680F} */
#ifndef __IID_IEcoUSB2RISCV64D1Config
static const UGUID IID_IEcoUSB2RISCV64D1Config = { 0x01, 0x10, {0x4D, 0xA2, 0x7E, 0xCA, 0x44, 0x19, 0x48, 0x48, 0x95, 0xC9, 0x40, 0xA1, 0x1B, 0x73, 0x68, 0x0F}};
#endif /* __IID_IEcoUSB2RISCV64D1Config */

typedef struct IEcoUSB2RISCV64D1Config* IEcoUSB2RISCV64D1ConfigPtr_t;

typedef struct IEcoUSB2RISCV64D1ConfigVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoUSB2RISCV64D1ConfigPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoUSB2RISCV64D1ConfigPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoUSB2RISCV64D1ConfigPtr_t me);

    /* IEcoUSB2RISCV64D1Config */
    int16_t (ECOCALLMETHOD *set_ConfigDescriptor)(/* in */ IEcoUSB2RISCV64D1ConfigPtr_t me, /* in */ ECO_USB_CONFIG_DESCRIPTOR* config);
    ECO_USB_CONFIG_DESCRIPTOR* (ECOCALLMETHOD *get_ConfigDescriptor)(/* in */ IEcoUSB2RISCV64D1ConfigPtr_t me);

} IEcoUSB2RISCV64D1ConfigVTbl, *IEcoUSB2RISCV64D1ConfigVTblPtr_t;

interface IEcoUSB2RISCV64D1Config {
    struct IEcoUSB2RISCV64D1ConfigVTbl *pVTbl;
} IEcoUSB2RISCV64D1Config;

#endif /* __I_ECO_USB_2_RISCV64_D1_CONFIG_H__ */
