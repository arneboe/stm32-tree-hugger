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

    while (true) {
        sampleCounter = 0;
        HAL_TIM_Base_Start_IT(&htim2);
        //wait until samples are collected
        while (sampleCounter < FFT_SIZE * 2) {
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

        int maxI = 1;

        for (int i = 1; i < FFT_SIZE; ++i) {
            if (magnitudes[i] > magnitudes[maxI])
                maxI = i;
        }

        for (int i = 0; i < 25; ++i) {
            printf("\n");
        }

        const float maxMag = magnitudes[maxI];
        for (int i = maxI - 3; i < maxI + 3; ++i) {
            if (i >= 0 && i < FFT_SIZE) {
                //FIXME magic constants 10k = sampling rate,
                const float freq = i * 10000.0 / FFT_SIZE;
                printf("bin: %3d (%.1fHz): %.2f\n", i, freq, magnitudes[i] / maxMag);
            }
        }
        memset(magnitudes, 0, sizeof(magnitudes)); //FIXME do I need this?
        HAL_Delay(1000);
    }
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
}


