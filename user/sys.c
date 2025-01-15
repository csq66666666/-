#include "stm32f1xx_hal.h"
#include "sys.h"
void WFI_SET(void)
{
    __WFI();
}

void INTX_DISABLE(void)
{
    __disable_irq();
}

void INTX_ENABLE(void)
{
    __enable_irq();
}

void MSR_MSP(uint32_t addr)
{
    __set_MSP(addr);
}
