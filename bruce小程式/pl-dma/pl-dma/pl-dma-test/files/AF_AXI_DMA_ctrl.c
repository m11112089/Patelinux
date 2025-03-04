#ifndef _AF_AXI_DMA_CTRL_C_
#define _AF_AXI_DMA_CTRL_C_
#include "AF_AXI_DMA_ctrl.h"

#endif

#include "os_wrap.h"

void AF_DMA_Initial(
    AF_AXI_DMA_ctrl_t *pDMA_ctrl
    ,uint32_t IP_address
    ,uint8_t blocking_mode
    ,uint8_t multiCycle_mode
)
{
    memset(pDMA_ctrl,0,sizeof(*pDMA_ctrl)) ;
#ifdef __linux__
	if ((pDMA_ctrl->IP_address = prepare_memory(IP_address)) == NULL)
		printf("prepare_memory failed\n");
#else
    pDMA_ctrl->IP_address = IP_address ;
#endif
    pDMA_ctrl->blocking_mode[IP_AFDMA_CTRL__WR_CH] = blocking_mode ;
    pDMA_ctrl->blocking_mode[IP_AFDMA_CTRL__RD_CH] = blocking_mode ;
    pDMA_ctrl->mc_mode[IP_AFDMA_CTRL__WR_CH] = multiCycle_mode ;
    pDMA_ctrl->mc_mode[IP_AFDMA_CTRL__RD_CH] = multiCycle_mode ;
}


uint8_t AF_DMA_trigger(
    AF_AXI_DMA_ctrl_t *pDMA_ctrl
    ,uint8_t channel
    ,uint32_t memory_address
    ,uint32_t data_length
    ,uint32_t tag
)
{
    if (pDMA_ctrl->busy[channel] == BOOL_TRUE) {
        return (AFDMA_FAIL) ;
    }

    uint8_t trig_res = AF_DMA_transfer_trig_func(
        pDMA_ctrl->IP_address
        ,channel
        ,data_length
        ,memory_address
        ,tag
        ,pDMA_ctrl->blocking_mode[channel] == IP_AFDMA_CTRL__MODE_NON_BLOCKING
        ,pDMA_ctrl->mc_mode[channel]
    )  ;

    if (trig_res == AFDMA_PASS) {
        pDMA_ctrl->busy[channel] = BOOL_TRUE ;
        pDMA_ctrl->finish[channel] = BOOL_FALSE ;
    }

    return (trig_res) ;
}


uint8_t AF_DMA_getFlag_busy(AF_AXI_DMA_ctrl_t *pDMA_ctrl,uint8_t channel)
{
    return (pDMA_ctrl->busy[channel]) ;
}


uint8_t AF_DMA_getFlag_finish(AF_AXI_DMA_ctrl_t *pDMA_ctrl,uint8_t channel)
{
    return (pDMA_ctrl->finish[channel]) ;
}

void AF_DMA_clearFlag_finish(AF_AXI_DMA_ctrl_t *pDMA_ctrl,uint8_t channel)
{
    pDMA_ctrl->finish[channel] = BOOL_FALSE ;
}

void AF_DMA_handle(
    AF_AXI_DMA_ctrl_t *pDMA_ctrl
    ,uint8_t channel
)
{
    if (pDMA_ctrl->busy[channel] == BOOL_TRUE) {
        uint8_t escape = pDMA_ctrl->blocking_mode[channel] == IP_AFDMA_CTRL__MODE_NON_BLOCKING ;
        uint8_t clear_res = AF_DMA_transfer_clear(
            pDMA_ctrl->IP_address
            ,channel
            ,escape
        ) ;

        if (clear_res == AFDMA_PASS) {
            pDMA_ctrl->busy[channel] = BOOL_FALSE ;
            pDMA_ctrl->finish[channel] = BOOL_TRUE ;
        }
    }
}

char AF_DMA_transfer_trig_func(
    uint32_t IP_addr
    ,uint8_t write_chnl
    ,uint32_t len
    ,uint32_t start_addr
    ,uint32_t tag
    ,uint32_t nonBlocking
    ,uint8_t mc_mode
)
{
    uint32_t base_addr = IP_addr + (write_chnl&1)*IP_AFDMA_CTRL__REG_ADDR__CHNL_OFFSET ;

    // Wait DMA idle
    uint32_t SR = 0;
    while (1) {
        SR = bus_rd32(base_addr + IP_AFDMA_CTRL__REG_ADDR__SR) ;
        if (BIT_OP_SPLIT(SR,IP_AFDMA_CTRL__BIT_OFF__SR__RUNNING,1)==0) {
            break ;
        }
        else if (nonBlocking) {
            return (AFDMA_FAIL) ;
        }
    }

    bus_wr32(base_addr + IP_AFDMA_CTRL__REG_ADDR__LEN,len) ;
    bus_wr32(base_addr + IP_AFDMA_CTRL__REG_ADDR__START_ADDR,start_addr) ;
    bus_wr32(base_addr + IP_AFDMA_CTRL__REG_ADDR__TAG,tag) ;
    bus_wr32(
        base_addr + IP_AFDMA_CTRL__REG_ADDR__CR
        ,(
            BIT_OP_MEGRE(1,IP_AFDMA_CTRL__BIT_OFF__CR__ENA,1)
            |BIT_OP_MEGRE(1,IP_AFDMA_CTRL__BIT_OFF__CR__TRIG,1)
            |BIT_OP_MEGRE(mc_mode,IP_AFDMA_CTRL__BIT_OFF__CR__MULTICYCLE_MODE,1)
        )
    ) ;

    while (1) {
        SR = bus_rd32(base_addr + IP_AFDMA_CTRL__REG_ADDR__SR) ;
        uint8_t running = BIT_OP_SPLIT(SR,IP_AFDMA_CTRL__BIT_OFF__SR__RUNNING,1) ;
        uint8_t finish = BIT_OP_SPLIT(SR,IP_AFDMA_CTRL__BIT_OFF__SR__FINISH,1) ;
        if ((running==1)|(finish==1)) {
            break ;
        }
        else if (nonBlocking) {
            return (AFDMA_FAIL) ;
        }
    }

    return (AFDMA_PASS); 
}

char AF_DMA_transfer_clear(uint32_t IP_addr,uint8_t write_chnl,uint8_t escape){
    uint32_t base_addr = IP_addr + (write_chnl&1)*IP_AFDMA_CTRL__REG_ADDR__CHNL_OFFSET ;
    uint32_t SR = 0 ;
    while(1) {
        SR = bus_rd32(base_addr + IP_AFDMA_CTRL__REG_ADDR__SR) ;
        if (BIT_OP_SPLIT(SR,IP_AFDMA_CTRL__BIT_OFF__SR__FINISH,1)==1){
            break ;
        }
        else {
            if (escape) return (AFDMA_FAIL) ;
        }
    } 

    bus_wr32(base_addr + IP_AFDMA_CTRL__REG_ADDR__CR,BIT_OP_MEGRE(1,IP_AFDMA_CTRL__BIT_OFF__CR__ENA,1)|BIT_OP_MEGRE(0,IP_AFDMA_CTRL__BIT_OFF__CR__TRIG,1)) ;

    do {
        SR = bus_rd32(base_addr + IP_AFDMA_CTRL__REG_ADDR__SR) ;
    }	while ((BIT_OP_SPLIT(SR,IP_AFDMA_CTRL__BIT_OFF__SR__RUNNING,1)!=0)&(BIT_OP_SPLIT(SR,IP_AFDMA_CTRL__BIT_OFF__SR__FINISH,1)!=0)) ;

    return (AFDMA_PASS) ;
}



uint8_t AF_DMA_ja_config(
    AF_AXI_DMA_ctrl_t *pDMA_ctrl
    , af_dma_ja_dim_cfgInfo_t *pJaDimCfg
    , uint8_t active_dim
    , uint8_t skip_rd_tlast
    , uint8_t rd_tlast_dim
) {
    if (pDMA_ctrl->IP_JA_DIM_MAX == 0) {
        return AFDMA_FAIL ;
    }

    uint32_t ip_addr = pDMA_ctrl->IP_address ;
    for (uint8_t dimIdx = 0 ; dimIdx < pDMA_ctrl->IP_JA_DIM_MAX ; dimIdx++) {
        uint32_t cycle = pJaDimCfg->cycle ;
        uint32_t step = pJaDimCfg->step ;
        if (dimIdx >= active_dim) {
            cycle = 1 ;
            step = 0 ;
        }
        bus_wr32(ip_addr+IP_AFDMA_CTRL__REG_ADDR__JA_MODE__CYCLE_D0+0x4*dimIdx,cycle) ;
        bus_wr32(ip_addr+IP_AFDMA_CTRL__REG_ADDR__JA_MODE__STEP_D0+0x4*dimIdx,step) ;
        pJaDimCfg++ ;
    }

    uint32_t ja_cr = 0;
    ja_cr |= active_dim << IP_AFDMA_CTRL__BIT_OFF__JA_MODE_CR__ENA_DIMENSION_LEVEL ;

    skip_rd_tlast&=1 ;
    if (skip_rd_tlast) {
        ja_cr |= (rd_tlast_dim) << IP_AFDMA_CTRL__BIT_OFF__JA_MODE_CR__RD_LAST ;
    }
    ja_cr |= skip_rd_tlast << IP_AFDMA_CTRL__BIT_OFF__JA_MODE_CR__SIKP_RD_LAST ;
    bus_wr32(ip_addr+IP_AFDMA_CTRL__REG_ADDR__JA_MODE__CR,ja_cr) ;

    return AFDMA_PASS ;
}
