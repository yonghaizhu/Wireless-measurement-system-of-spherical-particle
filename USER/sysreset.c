#include "sysreset.h"

void Soft_Reset_System(void)
{
	__set_FAULTMASK(1);      // 关闭所有中端
	NVIC_SystemReset();// 复位
}

