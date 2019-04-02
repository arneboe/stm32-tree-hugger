#include "stm32f1xx_hal.h"


extern "C" {
void SystemClock_Config();
}


int main()
{
    SystemClock_Config();

    HAL_Init();
//     MX_GPIO_Init();
//     MX_ADC1_Init(); 
//     MX_USART2_UART_Init();
//     MX_UART4_Init();
//     MX_TIM3_Init();
//     MX_TIM4_Init();
//     MX_SPI1_Init();
//     MX_USB_DEVICE_Init();
   
    while(1)
    {       
  
    }
}



