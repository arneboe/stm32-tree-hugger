#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "arm_math.h"
#include "adc.h"
#include "usart.h"
#include "tim.h"

extern "C" {
void SystemClock_Config();
}

const int FFT_SIZE = 1024;

float samples[FFT_SIZE*2];
float magnitudes[FFT_SIZE];


volatile uint32_t sampleCounter = 0;


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

    uint32_t startTime = HAL_GetTick();

    HAL_ADC_Start(&hadc1);
    printf("started\n");

    while(true)
    {
        sampleCounter = 0;
        HAL_TIM_Base_Start_IT(&htim2);
        //wait until samples are collected
        while(sampleCounter < FFT_SIZE * 2)
        {
        }
        printf("a\n");
        arm_cfft_radix4_instance_f32 fft_inst;
        arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);
        printf("aa\n");
        arm_cfft_radix4_f32(&fft_inst, samples);
        printf("aaa\n");
        arm_cmplx_mag_f32(samples, magnitudes, FFT_SIZE);
        printf("aaaa\n");
        printf("b\n");

        //9,765625 hz per bin


/*
        for(int i = 0; i < 30; ++i)
            printf("\n");

        for(int i = 10; i < 20; ++i)
        {
            printf("bin: %.1f: %.2f\n", (i - 1) * (19.53125), magnitudes[i]);
        }
*/

        int maxI = 1;

        for(int i = 1; i < FFT_SIZE; ++i)
        {
            if(magnitudes[i] > magnitudes[maxI])
                maxI = i;
        }

        for(int i = 0; i < 25; ++i)
        {
            printf("\n");
        }

        const float maxMag = magnitudes[maxI];
        for(int i = maxI - 3; i < maxI + 3; ++i)
        {
            if(i >= 0 && i < FFT_SIZE)
            {
                //FIXME magic constants 10k = sampling rate,
                const float freq = i * 10000.0 / FFT_SIZE;
                printf("bin: %3d (%.1fHz): %.2f\n", i, freq, magnitudes[i]/maxMag);
            }
        }
        memset(magnitudes, 0, sizeof(magnitudes));
        HAL_Delay(1000);
    }

 //104


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
    const uint32_t adcValue = HAL_ADC_GetValue(&hadc1);
    //Trigger next conversion
    HAL_ADC_Start(&hadc1);
    samples[sampleCounter] = (float32_t)adcValue;
    samples[sampleCounter+1] = 0.0f; //only have real data
    sampleCounter += 2;
    if (sampleCounter >= FFT_SIZE*2)
    {
        HAL_TIM_Base_Stop_IT(htim);
    }
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
   //
    // Complex FFT functions require a coefficient for the imaginary part of the input.
    // Since we only have real data, set this coefficient to zero.
    //
    // Update sample buffer position and stop after the buffer is filled
    //
    //if (sampleCounter >= FFT_SIZE*2) {
    //    samplingTimer.end();
    //}
}



