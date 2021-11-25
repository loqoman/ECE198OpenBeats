#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions
#include "ece198.c"

#define INS1 GPIOA, GPIO_PIN_9
#define INS2 GPIOC, GPIO_PIN_7
#define INS3 GPIOB, GPIO_PIN_6
#define INS4 GPIOA, GPIO_PIN_7

// bool delayWhilePolling( double delay, GPIO_TypeDef* buttonPort, uint16_t buttonPin ) {
//     double endTime = HAL_GetTick() + delay;

//     while (HAL_GetTick() < endTime) {
//         if (HAL_GPIO_ReadPin(buttonPort, buttonPin) > 0) {
//             return true;
//         }
//     }

//     return false;
// }

int main(void)
{
    HAL_Init(); 

    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable port A (for the on-board LED, for example)
    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable port B (for the rotary encoder inputs, for example)
    __HAL_RCC_GPIOC_CLK_ENABLE(); // enable port C (for the on-board blue pushbutton, for example)

    // InitializePin(GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);
    // InitializePin(GPIOB, GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);
    // InitializePin(GPIOB, GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);
    // InitializePin(GPIOB, GPIO_PIN_4, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);
    // InitializePin(GPIOB, GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);

    // SNARE PIN
    // InitializePin(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0);

    // BASS PIN
    // InitializePin(GPIOA, GPIO_PIN_6, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0);

    InitializePin(INS1, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0);
    InitializePin(INS2, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0);
    InitializePin(INS3, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0);
    InitializePin(INS4, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0);
    // note: the on-board pushbutton is fine with the default values (no internal pull-up resistor
    // is required, since there's one on the board)

    // iterate through and play depending on whether high or low
    // bool kickArray[8];    
    // bool bassArray[8];    
    // bool cymbolArray[8];    
    // bool snareArray[8];    

    SerialSetup(9600);
    HAL_Delay(1000);

    SerialPuts("This works up to here.\n");

    // main control flow loop up to here
    while (1) {
        unsigned int pause = 50;
        SerialPuts("In");
        HAL_GPIO_WritePin(INS1, 1);
        HAL_Delay(pause);
        HAL_GPIO_WritePin(INS1, 0);
        HAL_Delay(pause);

        HAL_GPIO_WritePin(INS2, 1);
        HAL_Delay(pause);
        HAL_GPIO_WritePin(INS2, 0);
        HAL_Delay(pause);

        HAL_GPIO_WritePin(INS3, 1);
        HAL_Delay(pause);
        HAL_GPIO_WritePin(INS3, 0);
        HAL_Delay(pause);

        HAL_GPIO_WritePin(INS4, 1);
        HAL_Delay(pause);
        HAL_GPIO_WritePin(INS4, 0);
        HAL_Delay(pause);
        // if (delayWhilePolling(100.0, GPIOC, GPIO_PIN_13)) {
        //     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
        // }
        // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
        // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
        // if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)) {

        //     while (1) {
        //         if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)) {
        //             SerialPuts("Snare selected.    \r");
        //             HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
        //             HAL_Delay(50);
        //             HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
        //             break;
        //         }
        //         if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)) {
        //             SerialPuts("Bass selected.     \r");
        //             HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
        //             HAL_Delay(50);
        //             HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
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
    }

    return 0;
}

// This function is called by the HAL once every millisecond
void SysTick_Handler(void)
{
    HAL_IncTick(); // tell HAL that a new tick has happened
    // we can do other things in here too if we need to, but be careful
}

// play this from the sorted time
// int playLoop(int BPM, int Speaker, &bool Pattern) {
//     // return 0 if it executes well, otherwise zero
    

//     // read it from memory

//     // play the speaker 8 times

//     return 0;
// }

