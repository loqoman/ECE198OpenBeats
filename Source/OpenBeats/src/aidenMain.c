#include "ece198.c"
#include <stdbool.h>
#include <stdio.h>   

#define SNARE   GPIOA, GPIO_PIN_9
#define TOM     GPIOC, GPIO_PIN_7
#define CYMBOL  GPIOB, GPIO_PIN_6
#define BASS    GPIOA, GPIO_PIN_7

bool delayWhilePolling( 
        double delay, 
        GPIO_TypeDef* buttonPort, 
        uint16_t buttonPin 
    ) {

    double endTime = HAL_GetTick() + delay;

    while (HAL_GetTick() < endTime) {
        if (HAL_GPIO_ReadPin(buttonPort, buttonPin) > 0)
            return true;
        
    }
    
    return false;
}

void playBar(
        bool *snare_arr, 
        bool *tom_arr, 
        bool *cymbol_arr, 
        bool *bass_arr, 
        unsigned int BPM, 
        size_t currentBeat
    ) {

    float beat_len = 60.0 / BPM * 1000; // convert to ms

    float time_on = beat_len * 0.75;
    float time_off = beat_len * 0.25; 

    size_t currentIndex = currentBeat - 1;

    if (snare_arr[currentIndex])
        HAL_GPIO_WritePin(SNARE, 1);
    if (tom_arr[currentIndex])
        HAL_GPIO_WritePin(TOM, 1);
    if (cymbol_arr[currentIndex])
        HAL_GPIO_WritePin(CYMBOL, 1);
    if (bass_arr[currentIndex])
        HAL_GPIO_WritePin(BASS, 1);

    HAL_Delay(time_on); 
    
    HAL_GPIO_WritePin(SNARE, 0);
    HAL_GPIO_WritePin(TOM, 0);
    HAL_GPIO_WritePin(CYMBOL, 0);
    HAL_GPIO_WritePin(BASS, 0);

    HAL_Delay(time_off);
}

int main(void) {
  HAL_Init();

  __HAL_RCC_GPIOA_CLK_ENABLE(); 
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  SerialSetup(9600);
  HAL_Delay(1000);

  SerialPuts("This works up to here.\n");


  while (1) {

  }

  return 0;
}

void SysTick_Handler(void) {
  HAL_IncTick(); 
}

    // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
    // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
    //
    // if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)) {
    //     while (1) {
    //         if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)) {
    //             SerialPuts("Snare selected.    \r");
    //             break;
    //         }
    //         if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)) {
    //             SerialPuts("Bass selected.     \r");
    //             break;
    //         }
    //         if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)) {
    //             SerialPuts("Tom selected.      \r");
    //             break;
    //         }
    //         if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)) {
    //             SerialPuts("Ride selected.     \r");
    //             break;
    //         }
    //     }
    // }
