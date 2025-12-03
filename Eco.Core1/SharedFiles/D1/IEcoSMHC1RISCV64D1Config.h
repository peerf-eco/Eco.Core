/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IEcoSMHC1RISCV64D1Config
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию интерфейса IEcoSMHC1RISCV64D1Config
 *
 * 
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2023 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __I_ECO_SMHC_1_RISC_V_64_D_1_CONFIG_H__
#define __I_ECO_SMHC_1_RISC_V_64_D_1_CONFIG_H__

#include "IEcoBase1.h"

typedef union ECO_D1_H_SMHC_CTRL_REGISTER {
    struct {
        uint32_t SOFT_RST       : 1;    /* Software Reset,          Bit 0       */
        uint32_t FIFO_RST       : 1;    /* FIFO Reset,              Bit 1       */
        uint32_t DMA_RST        : 1;    /* DMA Reset,               Bit 2       */
        uint32_t RES0           : 1;    /* Reserved,                Bit 3       */
        uint32_t INT_ENB        : 1;    /* Global Interrupt Enable, Bit 4       */
        uint32_t DMA_ENB        : 1;    /* DMA Global Enable,       Bit 5       */
        uint32_t RES1           : 2;    /* Reserved,                Bit 6:7     */
        uint32_t CD_DBC_ENB     : 1;    /* Card Detect De-bounce Enable, Bit 8  */
        uint32_t RES2           : 1;    /* Reserved,                Bit 9       */
        uint32_t DDR_MOD_SEL    : 1;    /* DDR Mode Select,         Bit 10      */
        uint32_t TIME_UNIT_DAT  : 1;    /* Time unit for data line, Bit 11      */
        uint32_t TIME_UNIT_CMD  : 1;    /* Time unit for command line, Bit 12   */
        uint32_t RES3           : 18;   /* Reserved,                Bit 30:13   */
        uint32_t FIFO_AC_MOD    : 1;    /* FIFO Access Mode,        Bit 31      */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_CTRL_REGISTER;

typedef union ECO_D1_H_SMHC_CLKDIV_REGISTER {
    struct {
        uint32_t CCLK_DIV       : 8;    /* Card Clock Divider,      Bit 7:0     */
        uint32_t RES0           : 8;    /* Reserved,                Bit 15:8    */
        uint32_t CCLK_ENB       : 1;    /* Card Clock Enable,       Bit 16      */
        uint32_t CCLK_CTRL      : 1;    /* Card Clock Output Control, Bit 17      */
        uint32_t RES1           : 13;   /* Reserved,                Bit 30:18   */
        uint32_t MASK_DATA0     : 1;    /* Mask data0,              Bit 31      */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_CLKDIV_REGISTER;

typedef union ECO_D1_H_SMHC_TMOUT_REGISTER {
    struct {
        uint32_t RTO_LMT        : 8;    /* Response Timeout Limit,  Bit 7:0     */
        uint32_t DTO_LMT        : 24;   /* Data TimeoutLimit,       Bit 31:8    */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_TMOUT_REGISTER;

typedef union ECO_D1_H_SMHC_CTYPE_REGISTER {
    struct {
        uint32_t CARD_WID       : 2;    /*  */
        uint32_t RES0           : 30;   /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_CTYPE_REGISTER;

typedef union ECO_D1_H_SMHC_BLKSIZ_REGISTER {
    struct {
        uint32_t BLK_SZ         : 16;   /*  */
        uint32_t RES0           : 16;   /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_BLKSIZ_REGISTER;

typedef union ECO_D1_H_SMHC_BYTCNT_REGISTER {
    struct {
        uint32_t BYTE_CNT       : 32;   /* Byte counter,            Bit 31:0     */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_BYTCNT_REGISTER;

#define ECO_D1_H_SMHC_CMD_RESP_RCV      0x00000001  /*  */
#define ECO_D1_H_SMHC_CMD_LONG_RESP     0x00000002  /*  */
#define ECO_D1_H_SMHC_CMD_CHK_RESP_CRC  0x00000004  /*  */
#define ECO_D1_H_SMHC_CMD_DATA_TRANS    0x00000008  /*  */
#define ECO_D1_H_SMHC_CMD_TRANS_DIR     0x0000000C  /*  */
#define ECO_D1_H_SMHC_CMD_TRANS_MODE    0x00000010  /*  */
#define ECO_D1_H_SMHC_CMD_STOP_CMD_FLAG 0x00000020  /*  */
#define ECO_D1_H_SMHC_CMD_WAIT_PRE_OVER 0x00000040  /*  */
#define ECO_D1_H_SMHC_CMD_STOP_ABT_CMD  0x00000080  /*  */
#define ECO_D1_H_SMHC_CMD_SEND_INIT_SEQ 0x000000C0  /*  */
#define ECO_D1_H_SMHC_CMD_PRG_CLK       0x00000100  /*  */
#define ECO_D1_H_SMHC_CMD_BOOT_MOD_0    0x00000200  /*  */
#define ECO_D1_H_SMHC_CMD_BOOT_MOD_1    0x00000400  /*  */
#define ECO_D1_H_SMHC_CMD_BOOT_MOD_2    0x00000800  /*  */
#define ECO_D1_H_SMHC_CMD_EXP_BOOT_ACK  0x00000C00  /*  */
#define ECO_D1_H_SMHC_CMD_BOOT_ABT      0x00001000  /*  */
#define ECO_D1_H_SMHC_CMD_VOL_SW        0x00002000  /*  */
#define ECO_D1_H_SMHC_CMD_CMD_LOAD      0x00004000  /*  */

typedef union ECO_D1_H_SMHC_CMD_REGISTER {
    struct {
        uint32_t CMD_IDX        : 6;    /* CMD Index,               Bit 5:0     */
        uint32_t RESP_RCV       : 1;    /* Response Receive,        Bit 6       */
        uint32_t LONG_RESP      : 1;    /* Response Type,           Bit 7       */
        uint32_t CHK_RESP_CRC   : 1;    /* Check Response CRC,      Bit 8       */
        uint32_t DATA_TRANS     : 1;    /* Data Transfer,           Bit 9       */
        uint32_t TRANS_DIR      : 1;    /* Transfer Direction,      Bit 10      */
        uint32_t TRANS_MODE     : 1;    /* Transfer Mode,           Bit 11      */
        uint32_t STOP_CMD_FLAG  : 1;    /* Send Stop CMD Automatically (CMD12), Bit 12      */
        uint32_t WAIT_PRE_OVER  : 1;    /* Wait for Data Transfer Over, Bit 13      */
        uint32_t STOP_ABT_CMD   : 1;    /* Stop Abort Command,      Bit 14      */
        uint32_t SEND_INIT_SEQ  : 1;    /* Send Initialization,     Bit 15      */
        uint32_t RES0           : 5;    /* Reserved,                Bit 20:16   */
        uint32_t PRG_CLK        : 1;    /* Change Clock,            Bit 21      */
        uint32_t RES1           : 2;    /* Reserved,                Bit 23:22   */
        uint32_t BOOT_MOD       : 2;    /* Boot Mode,               Bit 25:24   */
        uint32_t EXP_BOOT_ACK   : 1;    /* Expect Boot Acknowledge, Bit 26      */
        uint32_t BOOT_ABT       : 1;    /* Boot Abort    ,          Bit 27      */
        uint32_t VOL_SW         : 1;    /* Voltage Switch,          Bit 28      */
        uint32_t RES2           : 2;    /* Reserved,                Bit 30:29   */
        uint32_t CMD_LOAD       : 1;    /* Start Command,           Bit 31      */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_CMD_REGISTER;

typedef union ECO_D1_H_SMHC_CMDARG_REGISTER {
    struct {
        uint32_t CMD_ARG        : 32;    /* Command argument,       Bit 31:0    */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_CMDARG_REGISTER;

typedef union ECO_D1_H_SMHC_RESP0_REGISTER {
    struct {
        uint32_t CMD_RESP0      : 32;    /* Response 0,             Bit 31:0    */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_RESP0_REGISTER;

typedef union ECO_D1_H_SMHC_RESP1_REGISTER {
    struct {
        uint32_t CMD_RESP1      : 32;    /* Response 1,             Bit 31:0    */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_RESP1_REGISTER;

typedef union ECO_D1_H_SMHC_RESP2_REGISTER {
    struct {
        uint32_t CMD_RESP2      : 32;    /* Response 2,             Bit 31:0    */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_RESP2_REGISTER;

typedef union ECO_D1_H_SMHC_RESP3_REGISTER {
    struct {
        uint32_t CMD_RESP3      : 32;    /* Response 3,             Bit 31:0    */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_RESP3_REGISTER;

typedef union ECO_D1_H_SMHC_INTMASK_REGISTER {
    struct {
        uint32_t RES0                   : 1;    /*  */
        uint32_t RE_INT_EN              : 1;    /*  */
        uint32_t CC_INT_EN              : 1;    /*  */
        uint32_t DTC_INT_EN             : 1;    /*  */
        uint32_t DTR_INT_EN             : 1;    /*  */
        uint32_t DRR_INT_EN             : 1;    /*  */
        uint32_t RCE_INT_EN             : 1;    /*  */
        uint32_t DCE_INT_EN             : 1;    /*  */
        uint32_t DTO_BDS_INT_EN         : 1;    /*  */
        uint32_t RTO_BACK_INT_EN        : 1;    /*  */
        uint32_t DSTO_VSD_INT_EN        : 1;    /*  */
        uint32_t FU_FO_INT_EN           : 1;    /*  */
        uint32_t CB_IW_INT_EN           : 1;    /*  */
        uint32_t DSE_BC_INT_EN          : 1;    /*  */
        uint32_t ACD_INT_EN             : 1;    /*  */
        uint32_t DEE_INT_EN             : 1;    /*  */
        uint32_t SDIO_INT_EN            : 1;    /*  */
        uint32_t RES1                   : 13;   /*  */
        uint32_t CARD_INSERT_INT_EN     : 1;    /*  */
        uint32_t CARD_REMOVAL_INT_EN    : 1;    /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_INTMASK_REGISTER;

typedef union ECO_D1_H_SMHC_MINTSTS_REGISTER {
    struct {
        uint32_t RES0               : 1;    /*  */
        uint32_t M_RE_INT           : 1;    /*  */
        uint32_t M_CC_INT           : 1;    /*  */
        uint32_t M_DTC_INT          : 1;    /*  */
        uint32_t M_DTR_INT          : 1;    /*  */
        uint32_t M_DRR_INT          : 1;    /*  */
        uint32_t M_RCE_INT          : 1;    /*  */
        uint32_t M_DCE_INT          : 1;    /*  */
        uint32_t M_RTO_BACK_INT     : 1;    /*  */
        uint32_t M_DTO_BDS_INT      : 1;    /*  */
        uint32_t M_DSTO_VSD_INT     : 1;    /*  */
        uint32_t M_FU_FO_INT        : 1;    /*  */
        uint32_t M_CB_IW_INT        : 1;    /*  */
        uint32_t M_DSE_BC_INT       : 1;    /*  */
        uint32_t M_ACD_INT          : 1;    /*  */
        uint32_t M_DEE_INT          : 1;    /*  */
        uint32_t M_SDIO_INT         : 1;    /*  */
        uint32_t RES1               : 13;   /*  */
        uint32_t M_CARD_INSERT      : 1;    /*  */
        uint32_t M_CARD_REMOVAL_INT : 1;    /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_MINTSTS_REGISTER;

typedef union ECO_D1_H_SMHC_RINTSTS_REGISTER {
    struct {
        uint32_t RES0           : 1;    /* Reserved,                Bit 0       */
        uint32_t RE             : 1;    /* Response Error,          Bit 1       */
        uint32_t CC             : 1;    /* Command Complete,        Bit 2       */
        uint32_t DTC            : 1;    /* Data Transfer Complete,  Bit 3       */
        uint32_t DTR            : 1;    /* Data Transmit Request,   Bit 4       */
        uint32_t DRR            : 1;    /* Data Receive Request,    Bit 5       */
        uint32_t RCE            : 1;    /* Response CRC Error,      Bit 6       */
        uint32_t DCE            : 1;    /* Data CRC Error,          Bit 7       */
        uint32_t RTO_BACK       : 1;    /* Response Timeout/Boot ACK Received, Bit 8       */
        uint32_t DTO_BDS        : 1;    /* Data Timeout/Boot data Start,    Bit 9       */
        uint32_t DSTO_VSD       : 1;    /* Data Starvation Timeout/v1.8 Switch Done, Bit 10      */
        uint32_t FU_FO          : 1;    /* FIFO Underrun/Overflow,  Bit 11      */
        uint32_t CB_IW          : 1;    /* Command Busy And Illegal Write, Bit 12      */
        uint32_t DSE_BC         : 1;    /* Data Start Error/Busy Clear, Bit 13      */
        uint32_t ACD            : 1;    /* Auto Command Done,       Bit 14      */
        uint32_t DEE            : 1;    /* Data End-bit Error,      Bit 15      */
        uint32_t SDIOI_INT      : 1;    /* SDIO Interrupt,          Bit 16      */
        uint32_t RES1           : 13;   /* Reserved,                Bit 29:17   */
        uint32_t CARD_INSERT    : 1;    /* Card Inserted,           Bit 30      */
        uint32_t CARD_REMOVAL   : 1;    /* Card Removed,            Bit 31      */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_RINTSTS_REGISTER;

typedef union ECO_D1_H_SMHC_STATUS_REGISTER {
    struct {
        uint32_t FIFO_RX_LEVEL  : 1;    /* FIFO RX Water Level Flag, Bit 0       */
        uint32_t FIFO_TX_LEVEL  : 1;    /* FIFO TX Water Level Flag, Bit 1       */
        uint32_t FIFO_EMPTY     : 1;    /* FIFO Empty,              Bit 2       */
        uint32_t FIFO_FULL      : 1;    /* FIFO Full,               Bit 3       */
        uint32_t FSM_STA        : 4;    /* Command FSM States,      Bit 7:4     */
        uint32_t CARD_PRESENT   : 1;    /* Data[3] Status,          Bit 8       */
        uint32_t CARD_BUSY      : 1;    /* Card Data Busy,          Bit 9       */
        uint32_t FSM_BUSY       : 1;    /* Data FSM Busy,           Bit 10      */
        uint32_t RESP_IDX       : 6;    /* Response Index,          Bit 16:11   */
        uint32_t FIFO_LEVEL     : 9;    /* FIFO Level,              Bit 25:17   */
        uint32_t RES0           : 5;    /* Reserved,                Bit 30:26   */
        uint32_t DMA_REQ        : 1;    /* DMA Request,             Bit 31      */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_STATUS_REGISTER;

typedef union ECO_D1_H_SMHC_FIFOTH_REGISTER {
    struct {
        uint32_t TX_TL          : 8;    /*  */
        uint32_t RES0           : 8;    /*  */
        uint32_t RX_TL          : 8;    /*  */
        uint32_t RES1           : 4;    /*  */
        uint32_t BSIZE_OF_TRANS : 3;    /*  */
        uint32_t RES2           : 1;    /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_FIFOTH_REGISTER;

typedef union ECO_D1_H_SMHC_FUNS_REGISTER {
    struct {
        uint32_t HOST_SEND_MMC_IRQRESQ  : 1;    /*  */
        uint32_t READ_WAIT              : 1;    /*  */
        uint32_t ABT_RDATA              : 1;    /*  */
        uint32_t RES0                   : 29;   /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_FUNS_REGISTER;


typedef union ECO_D1_H_SMHC_TBC0_REGISTER {
    struct {
        uint32_t TBC0           : 32;   /* Transferred Count 0,     Bit 31:0    */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_TBC0_REGISTER;

typedef union ECO_D1_H_SMHC_TBC1_REGISTER {
    struct {
        uint32_t TBC1           : 32;   /* Transferred Count 1,     Bit 31:0    */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_TBC1_REGISTER;

typedef union ECO_D1_H_SMHC_CSDC_REGISTER {
    struct {
        uint32_t CRC_DET_PARA   : 4;    /*  */
        uint32_t RES0           : 28;   /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_CSDC_REGISTER;

typedef union ECO_D1_H_SMHC_A12A_REGISTER {
    struct {
        uint32_t SD_A12A        : 16;   /*  */
        uint32_t RES0           : 16;   /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_A12A_REGISTER;

typedef union ECO_D1_H_SMHC_NTSR_REGISTER {
    struct {
        uint32_t RES0                       : 4;   /*  */
        uint32_t CMD_SAMPLE_TIMING_PHASE    : 2;   /*  */
        uint32_t RES1                       : 2;   /*  */
        uint32_t DAT_SAMPLE_TIMING_PHASE    : 2;   /*  */
        uint32_t RES2                       : 6;   /*  */
        uint32_t CMD_SEND_RX_PHASE_CLR      : 1;   /*  */
        uint32_t RES3                       : 3;   /*  */
        uint32_t DAT_RECV_RX_PHASE_CLR      : 1;   /*  */
        uint32_t DAT_TRANS_RX_PHASE_CLR     : 1;   /*  */
        uint32_t DAT_CRC_STATUS_RX_PHASE_CLR : 1;   /*  */
        uint32_t RES4                       : 1;   /*  */
        uint32_t CMD_DAT_RX_PHASE_CLR       : 1;   /*  */
        uint32_t RES5                       : 2;   /*  */
        uint32_t DAT0_BYPASS                : 1;   /*  */
        uint32_t RES6                       : 3;   /*  */
        uint32_t MODE_SELEC                 : 1;   /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_NTSR_REGISTER;

typedef union ECO_D1_H_SMHC_HWRST_REGISTER {
    struct {
        uint32_t HW_RST         : 1;   /*  */
        uint32_t RES0           : 31;   /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_HWRST_REGISTER;

typedef union ECO_D1_H_SMHC_IDMAC_REGISTER {
    struct {
        uint32_t IDMAC_RST      : 1;   /*  */
        uint32_t FIX_BUST_CTRL  : 1;   /*  */
        uint32_t RES0           : 5;   /*  */
        uint32_t IDMAC_ENB      : 1;   /*  */
        uint32_t RES1           : 3;   /*  */
        uint32_t RES2           : 20;  /*  */
        uint32_t DES_LOAD_CTRL  : 1;   /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_IDMAC_REGISTER;

typedef union ECO_D1_H_SMHC_DLBA_REGISTER {
    struct {
        uint32_t DES_BASE_ADDR  : 32;  /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_DLBA_REGISTER;

typedef union ECO_D1_H_SMHC_IDST_REGISTER {
    struct {
        uint32_t TX_INT             : 1;   /*  */
        uint32_t RX_INT             : 1;   /*  */
        uint32_t FATAL_BERR_INT     : 1;   /*  */
        uint32_t RES0               : 1;   /*  */
        uint32_t DES_UNAVL_INT      : 1;   /*  */
        uint32_t ERR_FLAG_SUM       : 1;   /*  */
        uint32_t RES1               : 2;   /*  */
        uint32_t NOR_INT_SUM_NIS    : 1;   /*  */
        uint32_t ABN_INT_SUM_AIS    : 1;   /*  */
        uint32_t IDMAC_ERR_STA      : 3;   /*  */
        uint32_t RES2               : 4;   /*  */
        uint32_t RES3               : 15;   /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_IDST_REGISTER;

typedef union ECO_D1_H_SMHC_IDIE_REGISTER {
    struct {
        uint32_t TX_INT_ENB         : 1;   /*  */
        uint32_t RX_INT_ENB         : 1;   /*  */
        uint32_t FERR_INT_ENB       : 1;   /*  */
        uint32_t UNUSED0            : 1;   /*  */
        uint32_t DES_UNAVL_INT_ENB  : 1;   /*  */
        uint32_t ERR_SUM_INT_ENB    : 1;   /*  */
        uint32_t UNUSED1            : 2;   /*  */
        uint32_t RESERVED1          : 2;   /*  */
        uint32_t UNUSED2            : 22;  /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_IDIE_REGISTER;

typedef union ECO_D1_H_SMHC_THLD_REGISTER {
    struct {
        uint32_t CARD_RD_THLD_ENB   : 1;    /*  */
        uint32_t BCIG               : 1;    /*  */
        uint32_t CARD_WR_THLD_ENB   : 1;    /*  */
        uint32_t UNUSED0            : 13;   /*  */
        uint32_t CARD_RD_THLD       : 12;   /*  */
        uint32_t UNUSED1            : 4;    /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_THLD_REGISTER;

typedef union ECO_D1_H_SMHC_SFC_REGISTER {
    struct {
        uint32_t BYPASS_EN          : 1;    /*  */
        uint32_t STOP_CLK_CTRL      : 4;    /*  */
        uint32_t UNUSED0            : 27;   /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_SFC_REGISTER;

    typedef union ECO_D1_H_SMHC_A23A_REGISTER {
    struct {
        uint32_t A23A               : 32;   /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_A23A_REGISTER;

typedef union ECO_D1_H_SMHC_EMMC_DDR_SBIT_DET_REGISTER {
    struct {
        uint32_t HALF_START_BIT     : 1;    /*  */
        uint32_t UNUSED0            : 30;    /*  */
        uint32_t HS400_MD_EN        : 1;    /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_EMMC_DDR_SBIT_DET_REGISTER;

typedef union ECO_D1_H_SMHC_EXT_CMD_REGISTER {
    struct {
        uint32_t AUTO_CMD23_EN      : 1;    /*  */
        uint32_t UNUSED0            : 31;   /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_EXT_CMD_REGISTER;

typedef union ECO_D1_H_SMHC_EXT_RESP_REGISTER {
    struct {
        uint32_t SMHC_EXT_RESP      : 32;   /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_EXT_RESP_REGISTER;

typedef union ECO_D1_H_SMHC_DRV_DL_REGISTER {
    struct {
        uint32_t UNUSED0            : 16;    /*  */
        uint32_t CMD_DRV_PH_SEL     : 1;    /*  */
        uint32_t DAT_DRV_PH_SEL     : 1;    /*  */
        uint32_t UNUSED1            : 14;    /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_DRV_DL_REGISTER;

typedef union ECO_D1_H_SMHC_SAMP_DL_REGISTER {
    struct {
        uint32_t SAMP_DL_SW         : 6;    /*  */
        uint32_t UNUSED0            : 1;    /*  */
        uint32_t SAMP_DL_SW_EN      : 1;    /*  */
        uint32_t SAMP_DL            : 6;    /*  */
        uint32_t SAMP_DL_CAL_DONE   : 1;    /*  */
        uint32_t SAMP_DL_CAL_START  : 1;    /*  */
        uint32_t UNUSED1            : 16;    /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_SAMP_DL_REGISTER;

typedef union ECO_D1_H_SMHC_DS_DL_REGISTER {
    struct {
        uint32_t DS_DL_SW           : 6;    /*  */
        uint32_t UNUSED0            : 1;    /*  */
        uint32_t DS_DL_SW_EN        : 1;    /*  */
        uint32_t DS_DL              : 6;    /*  */
        uint32_t DS_DL_CAL_DONE     : 1;    /*  */
        uint32_t DS_DL_CAL_START    : 1;    /*  */
        uint32_t UNUSED1            : 16;   /*  */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_DS_DL_REGISTER;

typedef union ECO_D1_H_SMHC_HS400_DL_REGISTER {
    struct {
        uint32_t HS400_DLSW         : 6;    /*  */
        uint32_t UNUSED0            : 1;    /* / */
        uint32_t HS400_DLSW_EN      : 1;    /*  */
        uint32_t HS400_DL           : 6;    /*  */
        uint32_t HS400_DLCAL_DONE   : 1;    /*  */
        uint32_t HS400_DLCAL_START  : 1;    /*  */
        uint32_t UNUSED1            : 16;   /* / */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_HS400_DL_REGISTER;

typedef union ECO_D1_H_SMHC_FIFO_REGISTER {
    struct {
        uint32_t TX_RX_FIFO         : 32;   /* Data FIFO */
    } bits;
    uint32_t dword;
} ECO_D1_H_SMHC_FIFO_REGISTER;

/* Определение базовых адресов регистров SMHC для D1 H (3.2.5 Register List) */
#define ECO_D1_H_SMHC0     0x04020000UL
#define ECO_D1_H_SMHC1     0x04021000UL
#define ECO_D1_H_SMHC2     0x04022000UL

typedef struct ECO_D1_H_SMHC_REGISTER_MAP {
    /*       Register Name                 Description                                                  Offset      */
    uint32_t SMHC_CTRL;                 /* Control Register,                            Address offset: 0x00        */
    uint32_t SMHC_CLKDIV;               /* Clock Control Register,                      Address offset: 0x04        */
    uint32_t SMHC_TMOUT;                /* Time Out Register,                           Address offset: 0x08        */
    uint32_t SMHC_CTYPE;                /* Bus Width Register,                          Address offset: 0x0C        */
    uint32_t SMHC_BLKSIZ;               /* Block Size Register,                         Address offset: 0x10        */
    uint32_t SMHC_BYTCNT;               /* Byte Count Register,                         Address offset: 0x14        */
    uint32_t SMHC_CMD;                  /* Command Register,                            Address offset: 0x18        */
    uint32_t SMHC_CMDARG;               /* Command Argument Register,                   Address offset: 0x1C        */
    uint32_t SMHC_RESP0;                /* Response 0 Register,                         Address offset: 0x20        */
    uint32_t SMHC_RESP1;                /* Response 1 Register,                         Address offset: 0x24        */
    uint32_t SMHC_RESP2;                /* Response 2 Register,                         Address offset: 0x28        */
    uint32_t SMHC_RESP3;                /* Response 3 Register,                         Address offset: 0x2C        */
    uint32_t SMHC_INTMASK;              /* Interrupt Mask Register,                     Address offset: 0x30        */
    uint32_t SMHC_MINTSTS;              /* Masked Interrupt Status Register,            Address offset: 0x34        */
    uint32_t SMHC_RINTSTS;              /* Raw Interrupt Status Register,               Address offset: 0x38        */
    uint32_t SMHC_STATUS;               /* Status Register,                             Address offset: 0x3C        */
    uint32_t SMHC_FIFOTH;               /* FIFO Water Level Register,                   Address offset: 0x40        */
    uint32_t SMHC_FUNS;                 /* FIFO Function Select Register,               Address offset: 0x44        */
    uint32_t SMHC_TCBCNT;               /* Transfered Byte Count between Controller and Card, Address offset: 0x48  */
    uint32_t SMHC_TBBCNT;               /* Transfered Byte Count between Host Memory and Internal FIFO, Address offset: 0x4C */
    uint32_t SMHC_DBGC;                 /* Current Debug Control Register,              Address offset: 0x50        */
    uint32_t SMHC_CSDC;                 /* CRC Status Detect Control Register,          Address offset: 0x54        */
    uint32_t SMHC_A12A;                 /* Auto Command 12 Argument Register,           Address offset: 0x58        */
    uint32_t SMHC_NTSR;                 /* SD New Timing Set Register,                  Address offset: 0x5C        */
    uint32_t RESERVED0[6];              /* Reserved,                                    Address offset: 0x60-0x74   */
    uint32_t SMHC_HWRST;                /* Hardware Reset Register,                     Address offset: 0x78        */
    uint32_t RESERVED1;                 /* Reserved,                                    Address offset: 0x7C        */
    uint32_t SMHC_IDMAC;                /* IDMAC Control Register,                      Address offset: 0x80        */
    uint32_t SMHC_DLBA;                 /* Descriptor List Base Addres Register,        Address offset: 0x84        */
    uint32_t SMHC_IDST;                 /* IDMAC Status Register,                       Address offset: 0x88        */
    uint32_t SMHC_IDIE;                 /* IDMAC Interrupt Enable Register,             Address offset: 0x8C        */
    uint32_t RESERVED2[28];             /* Reserved,                                    Address offset: 0x90-0xFC   */
    uint32_t SMHC_THLD;                 /* Card Threshold Control Register,             Address offset: 0x100       */
    uint32_t SMHC_SFC;                  /* Sample FIFO Control Register,                Address offset: 0x104       */
    uint32_t SMHC_A23A;                 /* Auto Command 23 Argument Register,           Address offset: 0x108       */
    uint32_t SMHC_DDR_SBIT_DET;         /* eMMC4.5 DDR Start Bit Detection Control Register, Address offset: 0x10C  */
    uint32_t RESERVED3[10];             /* Reserved,                                    Address offset: 0x110-0x134 */
    uint32_t SMHC_EXT_CMD;              /* Extended Command Register,                   Address offset: 0x138       */
    uint32_t SMHC_EXT_RESP;             /* Extended Response Register,                  Address offset: 0x13C       */
    uint32_t SMHC_DRV_DL;               /* Drive Delay Control Register,                Address offset: 0x140       */
    uint32_t SMHC_SMAP_DL;              /* Sample Delay Control Register,               Address offset: 0x144       */
    uint32_t SMHC_DS_DL;                /* Data Strobe Delay Control Register,          Address offset: 0x148       */
    uint32_t SMHC_HS400_DL;             /* HS400 Delay Control Register,                Address offset: 0x14C       */
    uint32_t RESERVED4[44];             /* Reserved,                                   Address offset: 0x150-0x1FC */
    uint32_t SMHC_FIFO;                 /* Read/Write FIFO,                             Address offset: 0x200       */
} ECO_D1_H_SMHC_REGISTER_MAP;

#define ECO_SMHC_REGISTER_MAP ECO_D1_H_SMHC_REGISTER_MAP

typedef struct ECO_SMHC_CONFIG_DESCRIPTOR {
    union {
        uint32_t BaseAddress;
        struct ECO_SMHC_REGISTER_MAP* Map;
    } Register;
} ECO_SMHC_CONFIG_DESCRIPTOR;

/* IEcoSMHC1RISCV64D1Config IID = {CA89F197-AD00-4688-99FC-6ED9B17A9166} */
#ifndef __IID_IEcoSMHC1RISCV64D1Config
static const UGUID IID_IEcoSMHC1RISCV64D1Config = { 0x01, 0x10, {0xCA, 0x89, 0xF1, 0x97, 0xAD, 0x00, 0x46, 0x88, 0x99, 0xFC, 0x6E, 0xD9, 0xB1, 0x7A, 0x91, 0x66}};
#endif /* __IID_IEcoSMHC1RISCV64D1Config */

typedef struct IEcoSMHC1RISCV64D1Config* IEcoSMHC1RISCV64D1ConfigPtr_t;

typedef struct IEcoSMHC1RISCV64D1ConfigVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoSMHC1RISCV64D1ConfigPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoSMHC1RISCV64D1ConfigPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoSMHC1RISCV64D1ConfigPtr_t me);

    /* IEcoSMHC1RISCV64D1Config */
    int16_t (ECOCALLMETHOD *set_ConfigDescriptor)(/* in */ IEcoSMHC1RISCV64D1ConfigPtr_t me, /* in */ ECO_SMHC_CONFIG_DESCRIPTOR* config);
    ECO_SMHC_CONFIG_DESCRIPTOR* (ECOCALLMETHOD *get_ConfigDescriptor)(/* in */ IEcoSMHC1RISCV64D1ConfigPtr_t me);
    int16_t (ECOCALLMETHOD *set_LogicalPinNumber)(/* in */ IEcoSMHC1RISCV64D1ConfigPtr_t me, /* in */ uint8_t LogicalPinNumber, /* in */ uint8_t BankNumber, /* in */ uint32_t PinNumber);

} IEcoSMHC1RISCV64D1ConfigVTbl, *IEcoSMHC1RISCV64D1ConfigVTblPtr_t;

interface IEcoSMHC1RISCV64D1Config {
    struct IEcoSMHC1RISCV64D1ConfigVTbl *pVTbl;
} IEcoSMHC1RISCV64D1Config;

#endif /* __I_ECO_SMHC_1_RISC_V_64_D_1_CONFIG_H__ */
