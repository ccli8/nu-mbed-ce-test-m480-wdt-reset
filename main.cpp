#include "mbed.h"
#include <inttypes.h>

void RTC_AccessEnable()
{
    RTC->RWEN = RTC_WRITE_KEY;
    while(!(RTC->RWEN & RTC_RWEN_RWENF_Msk));
}

int main()
{
    printf("Hello world\n");
    printf("SYS->RSTSTS=0x%08x\n", SYS->RSTSTS);
    printf("CLK->PMUSTS=0x%08x\n", CLK->PMUSTS);
    printf("CLK->PMUCTL=0x%08x\n", CLK->PMUCTL);
    printf("SYS_IS_WDT_RST()=%d\n", SYS_IS_WDT_RST());

    uint32_t Spare_Data = 0;
    uint32_t Spare_Counter = 0;
    uint32_t i;
    uint64_t t;

#if 1
    t = rtc_read();
    printf("Now is %" PRId64 " (s)\n", t);
#endif

    /* Check if RTC spare registers are cleared unexpectedly */
#if 1
    rtc_init();

    if (!(SYS->CSERVER & 0x1)) {
        Spare_Counter = 20;
    } else {
        Spare_Counter = 5;
    }

    for(i = 1; i < Spare_Counter; i++)
    {
        RTC_AccessEnable();
        Spare_Data = RTC_READ_SPARE_REGISTER(i);
	printf(" SPARE_REGISTER[%d] = 0x%x \n", i, Spare_Data);
	Spare_Data ++;
	RTC_AccessEnable();
        RTC_WRITE_SPARE_REGISTER(i, Spare_Data);
    }
    
    for(i = 1; i < Spare_Counter; i++)
    {
        RTC_AccessEnable();
        Spare_Data = RTC_READ_SPARE_REGISTER(i);
        printf(" SPARE_REGISTER[%d] = 0x%x \n", i, Spare_Data);
    }
#endif

    /* Flush UART Tx buffer */
#if 0
    fflush(stdout);
#else
    while(!UART_IS_TX_EMPTY((UART_T *) NU_MODBASE(MBED_CONF_TARGET_USB_UART)));
#endif

    /* Arm WDT reset */
    Watchdog &watchdog = Watchdog::get_instance();
    watchdog.start(3000);

#if 1
    /* To reproduce WDT reset from PD */
    while (1) {
        SYS_UnlockReg();
        CLK_PowerDown();
    }
#else
    /* To reproduce WDT wake-up from PD and then WDT reset */
    SYS_UnlockReg();
    CLK_PowerDown();
    while (1);
#endif

    return 0;
}
