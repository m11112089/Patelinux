#ifndef _AF_AXI_DMA_CTRL_H_
#define _AF_AXI_DMA_CTRL_H_

#include <stdint.h>
#include <string.h>

#ifndef __linux__
/* Configurable */
#include "../lisco_common.h"

#ifndef bus_wr32
	#error "null: bus_wr32"
#endif

#ifndef bus_rd32
	#error "null: bus_rd32"
#endif
/* Configurable */
#endif

#define IP_AFDMA_CTRL__REG_ADDR__CHNL_OFFSET	0x20

#define IP_AFDMA_CTRL__MODE_BLOCKING                               0x1
#define IP_AFDMA_CTRL__MODE_NON_BLOCKING                           0x0

#define IP_AFDMA_CTRL__WR_CH                                       0x1
#define IP_AFDMA_CTRL__RD_CH                                       0x0

#define IP_AFDMA_CTRL__MC_MODE_ENABLE                              0x1
#define IP_AFDMA_CTRL__MC_MODE_DISABLE                             0x0

#define AFDMA_PASS	0
#define AFDMA_FAIL	-1

#ifndef BOOL_TRUE
#define BOOL_TRUE (1)
#endif

#ifndef BOOL_FALSE
#define BOOL_FALSE (0)
#endif

/* 
    address / register
 */
#define IP_AFDMA_CTRL__REG_ADDR__START_ADDR         0x00
#define IP_AFDMA_CTRL__REG_ADDR__LEN                0x04
#define IP_AFDMA_CTRL__REG_ADDR__TAG                0x08
#define IP_AFDMA_CTRL__REG_ADDR__CR                 0x0C
#define IP_AFDMA_CTRL__REG_ADDR__SR                 0x10
#define IP_AFDMA_CTRL__REG_ADDR__STS_TAG            0x14
#define IP_AFDMA_CTRL__REG_ADDR__STS_LEN            0x18

/* DMA write chnl register */
#define IP_AFDMA_CTRL__REG_ADDR__WR_START_ADDR     (IP_AFDMA_CTRL__REG_ADDR__CHNL_OFFSET+IP_AFDMA_CTRL__REG_ADDR__START_ADDR)
#define IP_AFDMA_CTRL__REG_ADDR__WR_LEN            (IP_AFDMA_CTRL__REG_ADDR__CHNL_OFFSET+IP_AFDMA_CTRL__REG_ADDR__LEN)
#define IP_AFDMA_CTRL__REG_ADDR__WR_TAG            (IP_AFDMA_CTRL__REG_ADDR__CHNL_OFFSET+IP_AFDMA_CTRL__REG_ADDR__TAG)
#define IP_AFDMA_CTRL__REG_ADDR__WR_CR             (IP_AFDMA_CTRL__REG_ADDR__CHNL_OFFSET+IP_AFDMA_CTRL__REG_ADDR__CR)
#define IP_AFDMA_CTRL__REG_ADDR__WR_SR             (IP_AFDMA_CTRL__REG_ADDR__CHNL_OFFSET+IP_AFDMA_CTRL__REG_ADDR__SR)
#define IP_AFDMA_CTRL__REG_ADDR__WR_STS_TAG        (IP_AFDMA_CTRL__REG_ADDR__CHNL_OFFSET+IP_AFDMA_CTRL__REG_ADDR__STS_TAG)
#define IP_AFDMA_CTRL__REG_ADDR__WR_STS_LEN        (IP_AFDMA_CTRL__REG_ADDR__CHNL_OFFSET+IP_AFDMA_CTRL__REG_ADDR__STS_LEN)

/* DMA read chnl register */
#define IP_AFDMA_CTRL__REG_ADDR__RD_START_ADDR     (IP_AFDMA_CTRL__REG_ADDR__START_ADDR)
#define IP_AFDMA_CTRL__REG_ADDR__RD_LEN            (IP_AFDMA_CTRL__REG_ADDR__LEN)
#define IP_AFDMA_CTRL__REG_ADDR__RD_TAG            (IP_AFDMA_CTRL__REG_ADDR__TAG)
#define IP_AFDMA_CTRL__REG_ADDR__RD_CR             (IP_AFDMA_CTRL__REG_ADDR__CR)
#define IP_AFDMA_CTRL__REG_ADDR__RD_SR             (IP_AFDMA_CTRL__REG_ADDR__SR)
#define IP_AFDMA_CTRL__REG_ADDR__RD_STS_TAG        (IP_AFDMA_CTRL__REG_ADDR__STS_TAG)
#define IP_AFDMA_CTRL__REG_ADDR__RD_STS_LEN        (IP_AFDMA_CTRL__REG_ADDR__STS_LEN)

/* JA mode register */
#define IP_AFDMA_CTRL__REG_ADDR__JA_MODE__CYCLE_D0    (0x4*16)
#define IP_AFDMA_CTRL__REG_ADDR__JA_MODE__CYCLE_D1    (0x4*17)
#define IP_AFDMA_CTRL__REG_ADDR__JA_MODE__CYCLE_D2    (0x4*18)
#define IP_AFDMA_CTRL__REG_ADDR__JA_MODE__CYCLE_D3    (0x4*19)
#define IP_AFDMA_CTRL__REG_ADDR__JA_MODE__CYCLE_D4    (0x4*20)
#define IP_AFDMA_CTRL__REG_ADDR__JA_MODE__CYCLE_D5    (0x4*21)
#define IP_AFDMA_CTRL__REG_ADDR__JA_MODE__CYCLE_D6    (0x4*22)
#define IP_AFDMA_CTRL__REG_ADDR__JA_MODE__CYCLE_D7    (0x4*23)
#define IP_AFDMA_CTRL__REG_ADDR__JA_MODE__STEP_D0     (0x4*24)
#define IP_AFDMA_CTRL__REG_ADDR__JA_MODE__STEP_D1     (0x4*25)
#define IP_AFDMA_CTRL__REG_ADDR__JA_MODE__STEP_D2     (0x4*26)
#define IP_AFDMA_CTRL__REG_ADDR__JA_MODE__STEP_D3     (0x4*27)
#define IP_AFDMA_CTRL__REG_ADDR__JA_MODE__STEP_D4     (0x4*28)
#define IP_AFDMA_CTRL__REG_ADDR__JA_MODE__STEP_D5     (0x4*29)
#define IP_AFDMA_CTRL__REG_ADDR__JA_MODE__STEP_D6     (0x4*30)
#define IP_AFDMA_CTRL__REG_ADDR__JA_MODE__STEP_D7     (0x4*31)
#define IP_AFDMA_CTRL__REG_ADDR__JA_MODE__CR          (0x4*32)

/* DMA ctrl register bit offset */
#define IP_AFDMA_CTRL__BIT_OFF__CR__TRIG                           0x0
#define IP_AFDMA_CTRL__BIT_OFF__CR__ENA                            0x1
#define IP_AFDMA_CTRL__BIT_OFF__CR__MULTICYCLE_MODE                0x2
#define IP_AFDMA_CTRL__BIT_OFF__SR__RUNNING                        0x0
#define IP_AFDMA_CTRL__BIT_OFF__SR__FINISH                         0x1
#define IP_AFDMA_CTRL__BIT_OFF__JA_MODE_CR__ENA_DIMENSION_LEVEL    0x0
#define IP_AFDMA_CTRL__BIT_OFF__JA_MODE_CR__STEP_DESCREASE         0x8
#define IP_AFDMA_CTRL__BIT_OFF__JA_MODE_CR__RD_LAST                16
#define IP_AFDMA_CTRL__BIT_OFF__JA_MODE_CR__SIKP_RD_LAST           24
#define IP_AFDMA_CTRL__BIT_OFF__SG_MODE_CR__LAST_DESCRIPTOR        0x0
#define IP_AFDMA_CTRL__BIT_OFF__SG_MODE_CR__ASSERT_RD_LAST         0x1

/* SG mode register */
#define IP_AFDMA_CTRL__REG_ADDR__SG_NEXT_DESC                      (0x4*0)
#define IP_AFDMA_CTRL__REG_ADDR__SG_CR                             (0x4*1)
#define IP_AFDMA_CTRL__REG_ADDR__SG_DMA_ADDR                       (0x4*2)
#define IP_AFDMA_CTRL__REG_ADDR__SG_DMA_LEN                        (0x4*3)
#define IP_AFDMA_CTRL__REG_ADDR__SG_DMA_TAG                        (0x4*4)

#define __linux__

typedef struct {
    uint32_t memAddr_nextDesc ;
    uint32_t dmaCfg_lastDesc : 1 ;
    uint32_t dmaCfg_assert_tlast : 1 ;
    uint32_t null : 30 ;
    uint32_t dma_address ;
    uint32_t dma_len ;
    uint32_t dma_tag ;
} af_dma_sg_desc_t ;

typedef struct {
    uint32_t cycle ;
    int32_t step ;
} af_dma_ja_dim_cfgInfo_t ;

typedef struct {
#ifdef __linux__
	void *IP_address;
#else
	uint32_t IP_address ;
#endif
	uint8_t sts[2] ;
	uint8_t busy[2] ;
	uint8_t transfer_streaming[2] ;
	uint8_t finish[2] ;
	uint8_t blocking_mode[2] ;
    uint8_t mc_mode[2] ;
    uint8_t IP_JA_DIM_MAX ;
} AF_AXI_DMA_ctrl_t ;

// ==============================================================<<<


// >>>==============================================================
// Function
// -----------------------------------------------------------------	

/* 
 @param[blocking_mode]
 - IP_AFDMA_CTRL__MODE_BLOCKING
 - IP_AFDMA_CTRL__MODE_NON_BLOCKING
 
 @param[multiCycle_mode]
 - IP_AFDMA_CTRL__MC_MODE_ENABLE
 - IP_AFDMA_CTRL__MC_MODE_DISABLE
 */
void AF_DMA_Initial(
    AF_AXI_DMA_ctrl_t *pDMA_ctrl
    ,uint32_t IP_address
    ,uint8_t blocking_mode
    ,uint8_t multiCycle_mode
) ;



/* 
 @ return
 - AFDMA_PASS
 - AFDMA_FAIL
 */
uint8_t AF_DMA_trigger(
    AF_AXI_DMA_ctrl_t *pDMA_ctrl
    ,uint8_t channel
    ,uint32_t memory_address
    ,uint32_t data_length
    ,uint32_t tag
) ;


/*  */
void AF_DMA_handle(
    AF_AXI_DMA_ctrl_t *pDMA_ctrl
    ,uint8_t channel
) ;


/* 
 @ return
 - BOOL_TRUE
 - BOOL_FALSE
 */
uint8_t AF_DMA_getFlag_busy(AF_AXI_DMA_ctrl_t *pDMA_ctrl,uint8_t channel) ;


/* 
 @ return
 - BOOL_TRUE
 - BOOL_FALSE
 */
uint8_t AF_DMA_getFlag_finish(AF_AXI_DMA_ctrl_t *pDMA_ctrl,uint8_t channel) ;


/*  */
void AF_DMA_clearFlag_finish(AF_AXI_DMA_ctrl_t *pDMA_ctrl,uint8_t channel) ;


/* 
 @ return
 - AFDMA_PASS
 - AFDMA_FAIL
 */
uint8_t AF_DMA_ja_config(
    AF_AXI_DMA_ctrl_t *pDMA_ctrl
    , af_dma_ja_dim_cfgInfo_t *pJaDimCfg
    , uint8_t active_dim
    , uint8_t skip_rd_tlast
    , uint8_t rd_tlast_dim
) ;


/* 
 ************************
 MACRO
 ************************
 */

#define AFDMA__WR_ENABLE(IP_addr)	bus_wr32(IP_addr + IP_AFDMA_CTRL__REG_ADDR__CR + IP_AFDMA_CTRL__REG_ADDR__CHNL_OFFSET,BIT_OP_MEGRE(1,IP_AFDMA_CTRL__BIT_OFF__CR__ENA,1))
#define AFDMA__WR_DISABLE(IP_addr)	bus_wr32(IP_addr + IP_AFDMA_CTRL__REG_ADDR__CR + IP_AFDMA_CTRL__REG_ADDR__CHNL_OFFSET,BIT_OP_MEGRE(0,IP_AFDMA_CTRL__BIT_OFF__CR__ENA,1))
#define AFDMA__RD_ENABLE(IP_addr)	bus_wr32(IP_addr + IP_AFDMA_CTRL__REG_ADDR__CR,BIT_OP_MEGRE(1,IP_AFDMA_CTRL__BIT_OFF__CR__ENA,1))
#define AFDMA__RD_DISABLE(IP_addr)	bus_wr32(IP_addr + IP_AFDMA_CTRL__REG_ADDR__CR,BIT_OP_MEGRE(0,IP_AFDMA_CTRL__BIT_OFF__CR__ENA,1))
#define Get_AFDMA_wrSts_len(IP_addr) bus_rd32(IP_addr + IP_AFDMA_CTRL__REG_ADDR__STS_LEN + IP_AFDMA_CTRL__REG_ADDR__CHNL_OFFSET)
#define Get_AFDMA_wrSts_tag(IP_addr) bus_rd32(IP_addr + IP_AFDMA_CTRL__REG_ADDR__STS_TAG + IP_AFDMA_CTRL__REG_ADDR__CHNL_OFFSET)
#define Get_AFDMA_rdSts_len(IP_addr) bus_rd32(IP_addr + IP_AFDMA_CTRL__REG_ADDR__STS_LEN )
#define Get_AFDMA_rdSts_tag(IP_addr) bus_rd32(IP_addr + IP_AFDMA_CTRL__REG_ADDR__STS_TAG )
#define Get_AFDMA_wrSR_running(IP_addr) BIT_OP_SPLIT(bus_rd32(IP_addr + IP_AFDMA_CTRL__REG_ADDR__SR+IP_AFDMA_CTRL__REG_ADDR__CHNL_OFFSET),IP_AFDMA_CTRL__BIT_OFF__SR__RUNNING,1)
#define Get_AFDMA_wrSR_finish(IP_addr) BIT_OP_SPLIT(bus_rd32(IP_addr + IP_AFDMA_CTRL__REG_ADDR__SR+IP_AFDMA_CTRL__REG_ADDR__CHNL_OFFSET),IP_AFDMA_CTRL__BIT_OFF__SR__FINISH,1)
#define Get_AFDMA_rdSR_running(IP_addr) BIT_OP_SPLIT(bus_rd32(IP_addr + IP_AFDMA_CTRL__REG_ADDR__SR),IP_AFDMA_CTRL__BIT_OFF__SR__RUNNING,1)
#define Get_AFDMA_rdSR_finish(IP_addr) BIT_OP_SPLIT(bus_rd32(IP_addr + IP_AFDMA_CTRL__REG_ADDR__SR),IP_AFDMA_CTRL__BIT_OFF__SR__FINISH,1)
#define AF_DMA_transfer_trig_write(IP_addr,len,start_addr,tag) AF_DMA_transfer_trig_func(IP_addr,1,len,start_addr,tag,IP_AFDMA_CTRL__MODE_BLOCKING)
#define AF_DMA_transfer_trig_read(IP_addr,len,start_addr,tag) AF_DMA_transfer_trig_func(IP_addr,0,len,start_addr,tag,IP_AFDMA_CTRL__MODE_BLOCKING)
#define AF_DMA_transfer_trig_write_nonblocking(IP_addr,len,start_addr,tag) AF_DMA_transfer_trig_func(IP_addr,1,len,start_addr,tag,IP_AFDMA_CTRL__MODE_NON_BLOCKING)
#define AF_DMA_transfer_trig_read_nonblocking(IP_addr,len,start_addr,tag) AF_DMA_transfer_trig_func(IP_addr,0,len,start_addr,tag,IP_AFDMA_CTRL__MODE_NON_BLOCKING)
#define AF_DMA_transfer_clear_write(IP_addr,escape) AF_DMA_transfer_clear(IP_addr,1,escape)
#define AF_DMA_transfer_clear_read(IP_addr,escape) AF_DMA_transfer_clear(IP_addr,0,escape)
char AF_DMA_transfer_trig_func(
    uint32_t IP_addr
    ,uint8_t write_chnl
    ,uint32_t len
    ,uint32_t start_addr
    ,uint32_t tag
    ,uint32_t nonBlocking
    ,uint8_t mc_mode
);
char AF_DMA_transfer_clear(uint32_t IP_addr,uint8_t write_chnl,uint8_t escape);
// ==============================================================<<<



#define BIT_OP_SPLIT(data,leftShfitBit,mask) (((data)>>leftShfitBit)&mask)
#define BIT_OP_MEGRE(data,rightShfitBit,mask) (((data&mask)<<rightShfitBit))



#endif
