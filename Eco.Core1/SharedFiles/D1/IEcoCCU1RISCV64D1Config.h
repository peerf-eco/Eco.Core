/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IEcoCCU1RISCV64D1Config
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию интерфейса IEcoCCU1RISCV64D1Config
 *
 * 
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2023 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __I_ECO_CCU_1_RISC_V_64_D_1_CONFIG_H__
#define __I_ECO_CCU_1_RISC_V_64_D_1_CONFIG_H__

#include "IEcoBase1.h"

typedef union ECO_D1_H_CCU_UART_BGR_REGISTER {
    struct {
        uint32_t UART0_GATING   : 1;    /* Gating Clock for UART0,  Bit 0      */
        uint32_t UART1_GATING   : 1;    /* Gating Clock for UART1,  Bit 1      */
        uint32_t UART2_GATING   : 1;    /* Gating Clock for UART2,  Bit 2      */
        uint32_t UART3_GATING   : 1;    /* Gating Clock for UART3,  Bit 3      */
        uint32_t UART4_GATING   : 1;    /* Gating Clock for UART4,  Bit 4      */
        uint32_t UART5_GATING   : 1;    /* Gating Clock for UART5,  Bit 5      */
        uint32_t RES0           : 10;   /* Reserved,                Bit 15:6   */
        uint32_t UART0_RST      : 1;    /* UART0 Reset,             Bit 16     */
        uint32_t UART1_RST      : 1;    /* UART1 Reset,             Bit 17     */
        uint32_t UART2_RST      : 1;    /* UART2 Reset,             Bit 18     */
        uint32_t UART3_RST      : 1;    /* UART3 Reset,             Bit 19     */
        uint32_t UART4_RST      : 1;    /* UART4 Reset,             Bit 20     */
        uint32_t UART5_RST      : 1;    /* UART5 Reset,             Bit 21     */
        uint32_t RES1           : 10;   /* Reserved,                Bit 31:22  */
    } bits;
    uint32_t dword;
} ECO_D1_H_CCU_UART_BGR_REGISTER;

typedef union ECO_D1_H_CCU_PB_CFG1_REGISTER {
    struct {
        uint32_t PB8_SELECT     : 4;    /* PB0 Select,   Bit 3:0    */
        uint32_t PB9_SELECT     : 4;    /* PB1 Select,   Bit 7:4    */
        uint32_t PB10_SELECT    : 4;    /* PB2 Select,   Bit 11:8   */
        uint32_t PB11_SELECT    : 4;    /* PB3 Select,   Bit 15:12  */
        uint32_t PB12_SELECT    : 4;    /* PB4 Select,   Bit 19:16  */
        uint32_t RES            : 12;   /* Reserved,     Bit 31:20  */
    } bits;
    uint32_t dword;
} ECO_D1_H_CCU_PB_CFG1_REGISTER;


typedef union ECO_D1_H_CCU_SMHC0_CLK_REGISTER {
    struct {
        uint32_t FACTOR_M           : 4;    /* Factor M,     Bit 3:0    */
        uint32_t RES0               : 4;    /* Reserved,     Bit 7:4    */
        uint32_t FACTOR_N           : 2;    /* Factor N,     Bit 9:8    */
        uint32_t RES1               : 14;   /* Reserved,     Bit 23:10  */
        uint32_t CLK_SRC_SEL        : 3;    /* Clock Source Select,   Bit 26:24  */
        uint32_t RES2               : 4;    /* Reserved,     Bit 30:27  */
        uint32_t SMHC0_CLK_GATING   : 1;    /* PB4 Select,   Bit 31     */
    } bits;
    uint32_t dword;
} ECO_D1_H_CCU_SMHC0_CLK_REGISTER;

typedef union ECO_D1_H_CCU_SMHC1_CLK_REGISTER {
    struct {
        uint32_t FACTOR_M           : 4;    /* Factor M,     Bit 3:0    */
        uint32_t RES0               : 4;    /* Reserved,     Bit 7:4    */
        uint32_t FACTOR_N           : 2;    /* Factor N,     Bit 9:8    */
        uint32_t RES1               : 14;   /* Reserved,     Bit 23:10  */
        uint32_t CLK_SRC_SEL        : 3;    /* Clock Source Select,   Bit 26:24  */
        uint32_t RES2               : 4;    /* Reserved,     Bit 30:27  */
        uint32_t SMHC1_CLK_GATING   : 1;    /* PB4 Select,   Bit 31     */
    } bits;
    uint32_t dword;
} ECO_D1_H_CCU_SMHC1_CLK_REGISTER;

typedef union ECO_D1_H_CCU_SMHC2_CLK_REGISTER {
    struct {
        uint32_t FACTOR_M           : 4;    /* Factor M,     Bit 3:0    */
        uint32_t RES0               : 4;    /* Reserved,     Bit 7:4    */
        uint32_t FACTOR_N           : 2;    /* Factor N,     Bit 9:8    */
        uint32_t RES1               : 14;   /* Reserved,     Bit 23:10  */
        uint32_t CLK_SRC_SEL        : 3;    /* Clock Source Select,   Bit 26:24  */
        uint32_t RES2               : 4;    /* Reserved,     Bit 30:27  */
        uint32_t SMHC2_CLK_GATING   : 1;    /* PB4 Select,   Bit 31     */
    } bits;
    uint32_t dword;
} ECO_D1_H_CCU_SMHC2_CLK_REGISTER;

/* Определение базовых адресов регистров CCU для D1 H (3.2.5 Register List) */
#define ECO_D1_H_CCU     0x02001000UL

typedef struct ECO_D1_H_CCU_REGISTER_MAP {
    /*       Register Name                 Description                                                  Offset      */
    uint32_t PLL_CPU_CTRL_REG;          /* PLL_CPU Control Register,                    Address offset: 0x00        */
    uint32_t RESERVED0[3];              /* Reserved,                                    Address offset: 0x04-0x0C   */
    uint32_t PLL_DDR_CTRL_REG;          /* PLL_DDR Control Register,                    Address offset: 0x10        */
    uint32_t RESERVED1[3];              /* Reserved,                                    Address offset: 0x14-0x1C   */
    uint32_t PLL_PERI_CTRL_REG;         /* PLL_PERI Control Register,                   Address offset: 0x20        */
    uint32_t RESERVED2[7];              /* Reserved,                                    Address offset: 0x24-0x3C   */
    uint32_t PLL_VIDEO0_CTRL_REG;       /* PLL_VIDEO0 Control Register,                 Address offset: 0x40        */
    uint32_t RESERVED3;                 /* Reserved,                                    Address offset: 0x44        */
    uint32_t PLL_VIDEO1_CTRL_REG;       /* PLL_VIDEO1 Control Register,                 Address offset: 0x48        */
    uint32_t RESERVED4[3];              /* Reserved,                                    Address offset: 0x4C-0x54   */
    uint32_t PLL_VE_CTRL_REG;           /* PLL_VE Control Register,                     Address offset: 0x58        */
    uint32_t RESERVED5[7];              /* Reserved,                                    Address offset: 0x5С-0x74   */
    uint32_t PLL_AUDIO0_CTRL_REG;       /* PLL_AUDIO0 Control Register,                 Address offset: 0x78        */
    uint32_t RESERVED6;                 /* Reserved,                                    Address offset: 0x7С        */
    uint32_t PLL_AUDIO1_CTRL_REG;       /* PLL_AUDIO1 Control Register,                 Address offset: 0x80        */
    uint32_t RESERVED7[35];             /* Reserved,                                    Address offset: 0x84-0x10C  */
    uint32_t PLL_DDR_PAT0_CTRL_REG;     /* PLL_DDR Pattern0 Control Register,           Address offset: 0x110       */
    uint32_t PLL_DDR_PAT1_CTRL_REG;     /* PLL_DDR Pattern1 Control Register,           Address offset: 0x114       */
    uint32_t RESERVED8[2];              /* Reserved,                                    Address offset: 0x118-0x11C */
    uint32_t PLL_PERI_PAT0_CTRL_REG;    /* PLL_PERI Pattern0 Control Register,          Address offset: 0x120       */
    uint32_t PLL_PERI_PAT1_CTRL_REG;    /* PLL_PERI Pattern1 Control Register,          Address offset: 0x124       */
    uint32_t RESERVED9[6];              /* Reserved,                                    Address offset: 0x128-0x13C */
    uint32_t PLL_VIDEO0_PAT0_CTRL_REG;  /* PLL_VIDEO0 Pattern0 Control Register,        Address offset: 0x140       */
    uint32_t PLL_VIDEO0_PAT1_CTRL_REG;  /* PLL_VIDEO0 Pattern1 Control Register,        Address offset: 0x144       */
    uint32_t PLL_VIDEO1_PAT0_CTRL_REG;  /* PLL_VIDEO1 Pattern0 Control Register,        Address offset: 0x148       */
    uint32_t PLL_VIDEO1_PAT1_CTRL_REG;  /* PLL_VIDEO1 Pattern1 Control Register,        Address offset: 0x14C       */
    uint32_t RESERVED10[2];             /* Reserved,                                    Address offset: 0x150-0x154 */
    uint32_t PLL_VE_PAT0_REG;           /* PLL_VE Pattern0 Control Register,            Address offset: 0x158       */
    uint32_t PLL_VE_PAT1_REG;           /* PLL_VE Pattern1 Control Register,            Address offset: 0x15C       */
    uint32_t RESERVED11[6];             /* Reserved,                                    Address offset: 0x160-0x174 */
    uint32_t PLL_AUDIO0_PAT0_CTRL_REG;  /* PLL_AUDIO0 Pattern0 Control Register,        Address offset: 0x178       */
    uint32_t PLL_AUDIO0_PAT1_CTRL_REG;  /* PLL_AUDIO0 Pattern1 Control Register,        Address offset: 0x17C       */
    uint32_t PLL_AUDIO1_PAT0_CTRL_REG;  /* PLL_AUDIO1 Pattern0 Control Register,        Address offset: 0x180       */
    uint32_t PLL_AUDIO1_PAT1_CTRL_REG;  /* PLL_AUDIO1 Pattern1 Control Register,        Address offset: 0x184       */
    uint32_t RESERVED12[94];            /* Reserved,                                    Address offset: 0x188-0x2FC */
    uint32_t PLL_CPU_BIAS_REG;          /* PLL_CPU Bias Register,                       Address offset: 0x300       */
    uint32_t RESERVED13[3];             /* Reserved,                                    Address offset: 0x304-0x30C */
    uint32_t PLL_DDR_BIAS_REG;          /* PLL_DDR Bias Register,                       Address offset: 0x310       */
    uint32_t RESERVED14[3];             /* Reserved,                                    Address offset: 0x314-0x31C */
    uint32_t PLL_PERI_BIAS_REG;         /* PLL_PERI Bias Register,                      Address offset: 0x320       */
    uint32_t RESERVED15[7];             /* Reserved,                                    Address offset: 0x324-0x33C */
    uint32_t PLL_VIDEO0_BIAS_REG;       /* PLL_VIDEO0 Bias Register,                    Address offset: 0x340       */
    uint32_t RESERVED16;                /* Reserved,                                    Address offset: 0x344       */
    uint32_t PLL_VIDEO1_BIAS_REG;       /* PLL_VIDEO1 Bias Register,                    Address offset: 0x348       */
    uint32_t RESERVED17[3];             /* Reserved,                                    Address offset: 0x34C-0x354 */
    uint32_t PLL_VE_BIAS_REG;           /* PLL_VE Bias Register,                        Address offset: 0x358       */
    uint32_t RESERVED18[7];             /* Reserved,                                    Address offset: 0x35C-0x374 */
    uint32_t PLL_AUDIO0_BIAS_REG;       /* PLL_AUDIO0 Bias Register,                    Address offset: 0x378       */
    uint32_t RESERVED19;                /* Reserved,                                    Address offset: 0x37C       */
    uint32_t PLL_AUDIO1_BIAS_REG;       /* PLL_AUDIO1 Bias Register,                    Address offset: 0x380       */
    uint32_t RESERVED20[31];            /* Reserved,                                    Address offset: 0x384-0x3FC */
    uint32_t PLL_CPU_TUN_REG;           /* PLL_CPU Tuning Register,                     Address offset: 0x400       */
    uint32_t RESERVED21[67];            /* Reserved,                                    Address offset: 0x404-0x50C */
    uint32_t PSI_CLK_REG;               /* PSI Clock Register,                          Address offset: 0x510       */
    uint32_t RESERVED22[3];             /* Reserved,                                    Address offset: 0x514-0x51C */
    uint32_t APB0_CLK_REG;              /* APB0 Clock Register,                         Address offset: 0x520       */
    uint32_t APB1_CLK_REG;              /* APB1 Clock Register,                         Address offset: 0x524       */
    uint32_t RESERVED23[6];             /* Reserved,                                    Address offset: 0x528-0x53C */
    uint32_t MBUS_CLK_REG;              /* MBUS Clock Register,                         Address offset: 0x540       */
    uint32_t RESERVED24[47];            /* Reserved,                                    Address offset: 0x544-0x5FC */
    uint32_t DE_CLK_REG;                /* DE Clock Register,                           Address offset: 0x600       */
    uint32_t RESERVED25[2];             /* Reserved,                                    Address offset: 0x604-0x608 */
    uint32_t DE_BGR_REG;                /* DE Bus Gating Reset Register,                Address offset: 0x60C       */
    uint32_t RESERVED26[4];             /* Reserved,                                    Address offset: 0x610-0x61C */
    uint32_t DI_CLK_REG;                /* DI Clock Register,                           Address offset: 0x620       */
    uint32_t RESERVED27[2];             /* Reserved,                                    Address offset: 0x624-0x628 */
    uint32_t DI_BGR_REG;                /* DI Bus Gating Reset  Register,               Address offset: 0x62C       */
    uint32_t G2D_CLK_REG;               /* G2D Clock Register,                          Address offset: 0x630       */
    uint32_t RESERVED29[2];             /* Reserved,                                    Address offset: 0x634-0x638 */
    uint32_t G2D_BGR_REG;               /* G2D Bus Gating Reset Register,               Address offset: 0x63C       */
    uint32_t RESERVED30[16];            /* Reserved,                                    Address offset: 0x640-0x67C */
    uint32_t CE_CLK_REG;                /* CE Clock Register,                           Address offset: 0x680       */
    uint32_t RESERVED31[2];             /* Reserved,                                    Address offset: 0x684-0x688 */
    uint32_t CE_BGR_REG;                /* CE Bus Gating Reset Register,                Address offset: 0x68C       */
    uint32_t VE_CLK_REG;                /* VE Clock Register,                           Address offset: 0x690       */
    uint32_t RESERVED33[2];             /* Reserved,                                    Address offset: 0x694-0x698 */
    uint32_t VE_BGR_REG;                /* VE Bus Gating Reset Register,                Address offset: 0x69C       */
    uint32_t RESERVED34[27];            /* Reserved,                                    Address offset: 0x6A0-0x708 */
    uint32_t DMA_BGR_REG;               /* DMA Bus Gating Reset Register,               Address offset: 0x70C       */
    uint32_t RESERVED35[3];             /* Reserved,                                    Address offset: 0x000-0x000 */
    uint32_t MSGBOX_BGR_REG;            /* MSGBOX Bus Gating Reset Register,            Address offset: 0x71C       */
    uint32_t RESERVED36[3];             /* Reserved,                                    Address offset: 0x720-0x728 */
    uint32_t SPINLOCK_BGR_REG;          /* SPINLOCK Bus Gating Reset Register,          Address offset: 0x72C       */
    uint32_t RESERVED37[3];             /* Reserved,                                    Address offset: 0x730-0x738 */
    uint32_t HSTIMER_BGR_REG;           /* HSTIMER Bus Gating Reset Register,           Address offset: 0x73C       */
    uint32_t AVS_CLK_REG;               /* AVS Clock Register,                          Address offset: 0x740       */
    uint32_t RESERVED39[18];            /* Reserved,                                    Address offset: 0x744-0x788 */
    uint32_t DBGSYS_BGR_REG;            /* DBGSYS Bus Gating Reset Register,            Address offset: 0x78C       */
    uint32_t RESERVED40[7];             /* Reserved,                                    Address offset: 0x790-0x7A8 */
    uint32_t PWM_BGR_REG;               /* PWM Bus Gating Reset Register,               Address offset: 0x7AC       */
    uint32_t RESERVED41[3];             /* Reserved,                                    Address offset: 0x7B0-0x7B8 */
    uint32_t IOMMU_BGR_REG;             /* IOMMU Bus Gating Reset Register,             Address offset: 0x7BC       */
    uint32_t RESERVED42[16];            /* Reserved,                                    Address offset: 0x7C0-0x7FC */
    uint32_t DRAM_CLK_REG;              /* DRAM Clock Register,                         Address offset: 0x800       */
    uint32_t MBUS_MAT_CLK_GATING_REG;   /* MBUS Master Clock Gating Register,           Address offset: 0x804       */
    uint32_t RESERVED43;                /* Reserved,                                    Address offset: 0x808       */
    uint32_t DRAM_BGR_REG;              /* DRAM Bus Gating Reset Register,              Address offset: 0x80C       */
    uint32_t RESERVED44[8];             /* Reserved,                                    Address offset: 0x810-0x82C */
    uint32_t SMHC0_CLK_REG;             /* SMHC0 Clock Register,                        Address offset: 0x830       */
    uint32_t SMHC1_CLK_REG;             /* SMHC1 Clock Register,                        Address offset: 0x834       */
    uint32_t SMHC2_CLK_REG;             /* SMHC2 Clock Register,                        Address offset: 0x838       */
    uint32_t RESERVED45[4];             /* Reserved,                                    Address offset: 0x83C-0x848 */
    uint32_t SMHC_BGR_REG;              /* SMHC Bus Gating Reset Register,              Address offset: 0x84C       */
    uint32_t RESERVED46[47];            /* Reserved,                                    Address offset: 0x850-0x908 */
    uint32_t UART_BGR_REG;              /* UART Bus Gating Reset Register,              Address offset: 0x90C       */
    uint32_t RESERVED47[3];             /* Reserved,                                    Address offset: 0x910-0x918 */
    uint32_t TWI_BGR_REG;               /* TWI Bus Gating Reset Register,               Address offset: 0x91C       */
    uint32_t RESERVED48[8];             /* Reserved,                                    Address offset: 0x920-0x93C */
    uint32_t SPI0_CLK_REG;              /* SPI0 Clock Register,                         Address offset: 0x940       */
    uint32_t SPI1_CLK_REG;              /* SPI1 Clock Register,                         Address offset: 0x944       */
    uint32_t RESERVED49[9];             /* Reserved,                                    Address offset: 0x948-0x968 */
    uint32_t SPI_BGR_REG;               /* SPI Bus Gating Reset Register,               Address offset: 0x96C       */
    uint32_t EMAC_25M_CLK_REG;          /* EMAC_25M Clock Register,                     Address offset: 0x970       */
    uint32_t RESERVED51[2];             /* Reserved,                                    Address offset: 0x974-0x978 */
    uint32_t EMAC_BGR_REG;              /* EMAC Bus Gating Reset Register,              Address offset: 0x97C       */
    uint32_t RESERVED52[16];            /* Reserved,                                    Address offset: 0x980-0x9BC */
    uint32_t IRTX_CLK_REG;              /* IRTX Clock Register,                         Address offset: 0x9C0       */
    uint32_t RESERVED53[2];             /* Reserved,                                    Address offset: 0x9C4-0x9C8 */
    uint32_t IRTX_BGR_REG;              /* IRTX Bus Gating Reset Register,              Address offset: 0x9CC       */
    uint32_t RESERVED54[7];             /* Reserved,                                    Address offset: 0x9D0-0x9E8 */
    uint32_t GPADC_BGR_REG;             /* GPADC Bus Gating Reset Register,             Address offset: 0x9EC       */
    uint32_t RESERVED55[3];             /* Reserved,                                    Address offset: 0x9F0-0x9F8 */
    uint32_t THS_BGR_REG;               /* THS Bus Gating Reset Register,               Address offset: 0x9FC       */
    uint32_t RESERVED56[4];             /* Reserved,                                    Address offset: 0xA00-0xA0C */
    uint32_t I2S0_CLK_REG;              /* I2S0 Clock Register,                         Address offset: 0xA10       */
    uint32_t I2S1_CLK_REG;              /* I2S1 Clock Register,                         Address offset: 0xA14       */
    uint32_t I2S2_CLK_REG;              /* I2S2 Clock Register,                         Address offset: 0xA18       */
    uint32_t I2S2_ASRC_CLK_REG;         /* I2S2_ASRC Clock Register,                    Address offset: 0xA1C       */
    uint32_t I2S_BGR_REG;               /* I2S Bus Gating Reset Register,               Address offset: 0xA20       */
    uint32_t OWA_TX_CLK_REG;            /* OWA_TX Clock Register,                       Address offset: 0xA24       */
    uint32_t RESERVED57;                /* Reserved,                                    Address offset: 0xA28       */
    uint32_t OWA_BGR_REG;               /* OWA Bus Gating Reset Register,               Address offset: 0xA2C       */
    uint32_t RESERVED58[4];             /* Reserved,                                    Address offset: 0xA30-0xA3C */
    uint32_t DMIC_CLK_REG;              /* DMIC Clock Register,                         Address offset: 0xA40       */
    uint32_t RESERVED59[2];             /* Reserved,                                    Address offset: 0xA44-0xA48 */
    uint32_t DMIC_BGR_REG;              /* DMIC Bus Gating Reset Register,              Address offset: 0xA4C       */
    uint32_t AUDIO_CODEC_DAC_CLK_REG;   /* AUDIO_CODEC_DAC Clock Register,              Address offset: 0xA50       */
    uint32_t AUDIO_CODEC_ADC_CLK_REG;   /* AUDIO_CODEC_ADC Clock Register,              Address offset: 0xA54       */
    uint32_t RESERVED61;                /* Reserved,                                    Address offset: 0xA58       */
    uint32_t AUDIO_CODEC_BGR_REG;       /* AUDIO_CODEC Bus Gating Reset Register,       Address offset: 0xA5C       */
    uint32_t RESERVED62[4];             /* Reserved,                                    Address offset: 0xA60-0xA6C */
    uint32_t USB0_CLK_REG;              /* USB0 Clock Register,                         Address offset: 0xA70       */
    uint32_t USB1_CLK_REG;              /* USB1 Clock Register,                         Address offset: 0xA74       */
    uint32_t RESERVED63[5];             /* Reserved,                                    Address offset: 0xA78-0xA88 */
    uint32_t USB_BGR_REG;               /* USB Bus Gating Reset Register,               Address offset: 0xA8C       */
    uint32_t RESERVED64[11];            /* Reserved,                                    Address offset: 0xA90-0xAB8 */
    uint32_t DPSS_TOP_BGR_REG;          /* DPSS Bus Gating Reset Register,              Address offset: 0xABC       */
    uint32_t RESERVED65[17];            /* Reserved,                                    Address offset: 0xAC0-0xB00 */
    uint32_t HDMI_24M_CLK_REG;          /* HDMI_24M Clock Register,                     Address offset: 0xB04       */
    uint32_t RESERVED66[2];             /* Reserved,                                    Address offset: 0xB08-0xB0C */
    uint32_t HDMI_CEC_CLK_REG;          /* HDMI_CEC Clock Register,                     Address offset: 0xB10       */
    uint32_t RESERVED67[2];             /* Reserved,                                    Address offset: 0xB14-0xB18 */
    uint32_t HDMI_BGR_REG;              /* HDMI Bus Gating Reset Register,              Address offset: 0xB1C       */
    uint32_t RESERVED68;                /* Reserved,                                    Address offset: 0xB20       */
    uint32_t DSI_CLK_REG;               /* DSI Clock Register,                          Address offset: 0xB24       */
    uint32_t RESERVED69[9];             /* Reserved,                                    Address offset: 0xB28-0xB48 */
    uint32_t DSI_BGR_REG;               /* DSI Bus Gating Reset Register,               Address offset: 0xB4C       */
    uint32_t RESERVED70[4];             /* Reserved,                                    Address offset: 0xB50-0xB5C */
    uint32_t TCONLCD_CLK_REG;           /* TCONLCD Clock Register,                      Address offset: 0xB60       */
    uint32_t RESERVED71[6];             /* Reserved,                                    Address offset: 0xB64-0xB78 */
    uint32_t TCONLCD_BGR_REG;           /* TCONLCD Bus Gating Reset Register,           Address offset: 0xB7C       */
    uint32_t TCONTV_CLK_REG;            /* TCONTV Clock Register,                       Address offset: 0xB80       */
    uint32_t RESERVED73[6];             /* Reserved,                                    Address offset: 0xB84-0xB98 */
    uint32_t TCONTV_BGR_REG;            /* TCONTV Bus Gating Reset Register,            Address offset: 0xB9C       */
    uint32_t RESERVED74[3];             /* Reserved,                                    Address offset: 0xBA0-0xBA8 */
    uint32_t LVDS_BGR_REG;              /* LVDS Bus Gating Reset Register,              Address offset: 0xBAC       */
    uint32_t TVE_CLK_REG;               /* TVE Clock Register,                          Address offset: 0xBB0       */
    uint32_t RESERVED76[2];             /* Reserved,                                    Address offset: 0xBB4-0xBB8 */
    uint32_t TVE_BGR_REG;               /* TVE Bus Gating Reset Register,               Address offset: 0xBBC       */
    uint32_t TVD_CLK_REG;               /* TVD Clock Register,                          Address offset: 0xBC0       */
    uint32_t RESERVED78[6];             /* Reserved,                                    Address offset: 0xBC4-0xBD8 */
    uint32_t TVD_BGR_REG;               /* TVD Bus Gating Reset Register,               Address offset: 0xBDC       */
    uint32_t RESERVED79[4];             /* Reserved,                                    Address offset: 0xBE0-0xBEC */
    uint32_t LEDC_CLK_REG;              /* LEDC Clock Register,                         Address offset: 0xBF0       */
    uint32_t RESERVED80[2];             /* Reserved,                                    Address offset: 0xBF4-0xBF8 */
    uint32_t LEDC_BGR_REG;              /* LEDC Bus Gating Reset Register,              Address offset: 0xBFC       */
    uint32_t RESERVED81;                /* Reserved,                                    Address offset: 0xC00       */
    uint32_t CSI_CLK_REG;               /* CSI Clock Register,                          Address offset: 0xC04       */
    uint32_t CSI_MASTER_CLK_REG;        /* CSI_MASTER Clock Register,                   Address offset: 0xC08       */
    uint32_t RESERVED82[4];             /* Reserved,                                    Address offset: 0xC0C-0xC18 */
    uint32_t CSI_BGR_REG;               /* CSI Bus Gating Reset Register,               Address offset: 0xC1C       */
    uint32_t RESERVED83[12];            /* Reserved,                                    Address offset: 0xC20-0xC4C */
    uint32_t TPADC_CLK_REG;             /* TPADC Clock Register,                        Address offset: 0xC50       */
    uint32_t RESERVED84[2];             /* Reserved,                                    Address offset: 0xC54-0xC58 */
    uint32_t TPADC_BGR_REG;             /* TPADC Bus Gating Reset Register,             Address offset: 0xC5C       */
    uint32_t RESERVED85[4];             /* Reserved,                                    Address offset: 0xC60-0xC6C */
    uint32_t DSP_CLK_REG;               /* DSP Clock Register,                          Address offset: 0xC70       */
    uint32_t RESERVED86[2];             /* Reserved,                                    Address offset: 0xC74-0xC78 */
    uint32_t DSP_BGR_REG;               /* DSP Bus Gating Reset Register,               Address offset: 0xC7C       */
    uint32_t RESERVED87[32];            /* Reserved,                                    Address offset: 0xC80-0xCFC */
    uint32_t RISC_V_CLK_REG;            /* RISC_V Clock Register,                       Address offset: 0xD00       */
    uint32_t RISC_V_GATING_REG;         /* RISC_V GATING Configuration Register,        Address offset: 0xD04       */
    uint32_t RESERVED88;                /* Reserved,                                    Address offset: 0xD08       */
    uint32_t RISC_V_CFG_BGR_REG;        /* RISC_V_CFG Bus Gating Reset Register,        Address offset: 0xD0C       */
    uint32_t RESERVED89[125];           /* Reserved,                                    Address offset: 0xD10-0xF00 */
    uint32_t PLL_LOCK_DBG_CTRL_REG;     /* PLL Lock Debug Control Register,             Address offset: 0xF04       */
    uint32_t FRE_DET_CTRL_REG;          /* Frequency Detect Control Register,           Address offset: 0xF08       */
    uint32_t FRE_UP_LIM_REG;            /* Frequency Up Limit Register,                 Address offset: 0xF0C       */
    uint32_t FRE_DOWN_LIM_REG;          /* Frequency Down Limit Register,               Address offset: 0xF10       */
    uint32_t RESERVED90[7];             /* Reserved,                                    Address offset: 0xF14-0xF2C */
    uint32_t CCU_FAN_GATE_REG;          /* CCU FANOUT CLOCK GATE Register,              Address offset: 0xF30       */
    uint32_t CLK27M_FAN_REG;            /* CLK27M FANOUT Register,                      Address offset: 0xF34       */
    uint32_t PCLK_FAN_REG;              /* PCLK FANOUT Register,                        Address offset: 0xF38       */
    uint32_t CCU_FAN_REG;               /* CCU FANOUT Register,                         Address offset: 0xF3C       */
} ECO_D1_H_CCU_REGISTER_MAP;

#define ECO_CCU_REGISTER_MAP ECO_D1_H_CCU_REGISTER_MAP

typedef struct ECO_CCU_CONFIG_DESCRIPTOR {
    union {
        uint32_t BaseAddress;
        struct ECO_CCU_REGISTER_MAP* Map;
    } Register;
} ECO_CCU_CONFIG_DESCRIPTOR;

/* IEcoCCU1RISCV64D1Config IID = {ED095029-1B10-49CC-A093-49F7CA6B61E0} */
#ifndef __IID_IEcoCCU1RISCV64D1Config
static const UGUID IID_IEcoCCU1RISCV64D1Config = { 0x01, 0x10, {0xED, 0x09, 0x50, 0x29, 0x1B, 0x10, 0x49, 0xCC, 0xA0, 0x93, 0x49, 0xF7, 0xCA, 0x6B, 0x61, 0xE0}};
#endif /* __IID_IEcoCCU1RISCV64D1Config */

typedef struct IEcoCCU1RISCV64D1Config* IEcoCCU1RISCV64D1ConfigPtr_t;

typedef struct IEcoCCU1RISCV64D1ConfigVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoCCU1RISCV64D1ConfigPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoCCU1RISCV64D1ConfigPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoCCU1RISCV64D1ConfigPtr_t me);

    /* IEcoCCU1RISCV64D1Config */
    int16_t (ECOCALLMETHOD *set_ConfigDescriptor)(/* in */ IEcoCCU1RISCV64D1ConfigPtr_t me, /* in */ ECO_CCU_CONFIG_DESCRIPTOR* config);
    ECO_CCU_CONFIG_DESCRIPTOR* (ECOCALLMETHOD *get_ConfigDescriptor)(/* in */ IEcoCCU1RISCV64D1ConfigPtr_t me);
    int16_t (ECOCALLMETHOD *set_LogicalPinNumber)(/* in */ IEcoCCU1RISCV64D1ConfigPtr_t me, /* in */ uint8_t LogicalPinNumber, /* in */ uint8_t BankNumber, /* in */ uint32_t PinNumber);

} IEcoCCU1RISCV64D1ConfigVTbl, *IEcoCCU1RISCV64D1ConfigVTblPtr_t;

interface IEcoCCU1RISCV64D1Config {
    struct IEcoCCU1RISCV64D1ConfigVTbl *pVTbl;
} IEcoCCU1RISCV64D1Config;

#endif /* __I_ECO_CCU_1_RISC_V_64_D_1_CONFIG_H__ */
