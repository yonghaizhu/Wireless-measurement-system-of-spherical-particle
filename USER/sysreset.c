#include "sysreset.h"

void Soft_Reset_System(void)
{
	__set_FAULTMASK(1);      // �ر������ж�
	NVIC_SystemReset();// ��λ
}

