#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "arm_math.h"
#include "adc.h"
#include "usart.h"
#include "tim.h"

extern "C" {
void SystemClock_Config();
}

/*const int FFT_SIZE = 256;*/
/*
float samples[FFT_SIZE*2];
float magnitudes[FFT_SIZE];
*/


int cnt = 0;


int main() {
    SystemClock_Config();

    HAL_Init();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_USART1_UART_Init();
    MX_TIM2_Init();


/*    if(HAL_ADC_Start_IT(&hadc1) != HAL_OK)
    {
        printf("ERROR STARTING HAL!\n");
        Error_Handler();
    }*/

    printf("aaa\n");
    HAL_TIM_Base_Start_IT(&htim2);
    printf("bbb\n");
    uint32_t startTime = HAL_GetTick();

    printf("start\n");

    while(true)
    {
        HAL_Delay(1000);
        const uint32_t timePassed = (HAL_GetTick() - startTime) / 1000;
        printf("time: %d\n", timePassed);
        //printf("time passed: %d\n", timePassed);
      //  printf("cnt: %d\n", cnt);
        const float ticksS = cnt / timePassed;
     //   printf("ticks:%f\n", ticksS);
        printf("%f\n", ticksS);


     /*   arm_cfft_radix4_instance_f32 fft_inst;
        arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);*/
    }



//     MX_USART2_UART_Init();
//     MX_UART4_Init();
//     MX_TIM3_Init();
//     MX_TIM4_Init();
//     MX_SPI1_Init();
//     MX_USB_DEVICE_Init();

// start timer with  SAMPLE_RATE_HZ = 9000;
//sample FFT_SIZE values
/*    if (samplingIsDone()) {
        // Run FFT on sample data.

        arm_cfft_radix4_instance_f32 fft_inst;
        arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);
        arm_cfft_radix4_f32(&fft_inst, samples);
        arm_cmplx_mag_f32(samples, magnitudes, FFT_SIZE);


        while (1) {
            HAL_Delay(200);
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

        }
        return 0;*/

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    //TODO if more timers run, check if this is the right timer

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    const uint32_t value = HAL_ADC_GetValue(&hadc1);
    printf("adc: %d\n", value);
    if(HAL_ADC_Start_IT(&hadc1) != HAL_OK)
    {
        printf("ERROR STARTING HAL!\n");
        Error_Handler();
    }

/*    for (int i = 0; i < NUM_FADERS; ++i)
{
    //Faders::faders[i].update(adcValues[i]);
    Faders::stats[i].add(adcValues[i]);
}
//start next reading
if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adcValues, NUM_FADERS) != HAL_OK)
    Error_Handler();*/
}

void samplingCallback() {
    // Read from the ADC and store the sample data
   // samples[sampleCounter] = (float32_t)analogRead(AUDIO_INPUT_PIN);
    // Complex FFT functions require a coefficient for the imaginary part of the input.
    // Since we only have real data, set this coefficient to zero.
    //samples[sampleCounter+1] = 0.0;
    // Update sample buffer position and stop after the buffer is filled
    //sampleCounter += 2;
    //if (sampleCounter >= FFT_SIZE*2) {
    //    samplingTimer.end();
    //}
}



