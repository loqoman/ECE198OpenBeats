#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions

#include "ece198.h"

int main(void) {
    HAL_Init(); // initialize the Hardware Abstraction Layer
    // Peripherals (including GPIOs) are disabled by default to save power, so we
    // use the Reset and Clock Control registers to enable the GPIO peripherals that we're using.

    // TODO
    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable port A (for the on-board LED, for example)
    // __HAL_RCC_GPIOB_CLK_ENABLE(); // enable port B (for the rotary encoder inputs, for example)
    __HAL_RCC_GPIOC_CLK_ENABLE(); // enable port C (for the on-board blue pushbutton, for example)

    // initialize the pins to be input, output, alternate function, etc...
    // TODO
    InitializePin(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED
    // InitializePin(GPIOA, GPIO_PIN_8, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0);
    InitializePin(GPIOC, GPIO_PIN_13, GPIO_MODE_INPUT, GPIO_PULLUP, 0);
    
    // note: the on-board pushbutton is fine with the default values (no internal pull-up resistor
    // is required, since there's one on the board)
    
    // Set up serial
    SerialSetup(9600);
    HAL_Delay(1000);

    SerialPuts("This works up to here.");


    while (1) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
        HAL_Delay(50);
        // enough that it takes some time  to load

        if (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)) {
            while (1) {
                HAL_Delay(50);
                HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
                HAL_Delay(50);

                if (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)) {
                    break;
                }
            }
            continue;
        }
        
    }


    return 0;
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

bool delayWhilePolling( double delay, GPIO_TypeDef* buttonPort, uint8_t buttonPin ) {
    float endTime = HAL_GetTick() + delay;

    while (HAL_GetTick() < endTime) {
        return !(HAL_GPIO_ReadPin(buttonPort, buttonPort));
    }

    return false;
}
