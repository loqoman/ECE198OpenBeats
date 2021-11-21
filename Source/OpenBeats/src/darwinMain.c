#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions

#include "ece198.c"

int main(void) {
    HAL_Init(); // initialize the Hardware Abstraction Layer
    // Peripherals (including GPIOs) are disabled by default to save power, so we
    // use the Reset and Clock Control registers to enable the GPIO peripherals that we're using.
    
    // LEDS pluigged into pins D6-D3
    // TODO: Ping definitions up here with #Define operators

    // TODO
    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable port A (for the on-board LED, for example)
    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable port B (for the rotary encoder inputs, for example)
    __HAL_RCC_GPIOC_CLK_ENABLE(); // enable port C (for the on-board blue pushbutton, for example) 
    //__HAL_RCC_ADC1_CLK_ENABLE();

    //ADC_HandleTypeDef adcInstance;
    //InitializeADC(&adcInstance, ADC1);

    // initialize the pins to be input, output, alternate function, etc...
    // TODO: What's the last option (`uint8_t alternate`)? 
    // --- LED Pins ---
        // OUTPUT_PP --> Output Push Pull
    InitializePin(GPIOB, GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED
    InitializePin(GPIOB, GPIO_PIN_4, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED
    InitializePin(GPIOB, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED
    InitializePin(GPIOB, GPIO_PIN_3, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED
    
   
    // --- CD74HC Pins --- 
    // The Sig pin (A0) is used as the input and the 4 digital pin is used to select which pin you 
    // InitializePin(GPIOA, GPIO_PIN_0,  GPIO_MODE_ANALOG, GPIO_NOPULL, 0);    // SIG -> A0
                                                                            // Must be an analog pin
        // N.B This line assoicated the ADC to pin0, then you read ADC0, not pin0 
    InitializePin(GPIOA, GPIO_PIN_0,  GPIO_MODE_INPUT, GPIO_NOPULL, 0);    // SIG -> A0 


    InitializePin(GPIOB, GPIO_PIN_15, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // S3 -> PB15
    InitializePin(GPIOB, GPIO_PIN_14, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // S2 -> PB14
    InitializePin(GPIOB, GPIO_PIN_1,  GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // S1 -> PB1
    InitializePin(GPIOB, GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // S0 -> PB13

    //InitializePin(GPIOA, GPIO_PIN_3, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, 0);  // EN -> PA3 
    
    
    // note: the on-board pushbutton is fine with the default values (no internal pull-up resistor
    // is required, since there's one on the board)
    
    // --- Setting everything to 0 --- 
    // Set all the CH74HC pins to zero 
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);   // S3
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);   // S2
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,  0);   // S1
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);   // S0 
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3,  0);   // EN
    // Turn off all LEDs
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);      
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,  0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,  0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,  0);

    // Set up serial
    SerialSetup(9600);
    HAL_Delay(1000);

    SerialPuts("--- Running darwinMain.c --- ");  


    char outputBitmask = 0x1; 
    int bpm = 120;                  // Change if you want a different bpm 
    int timeDelay = 60000 / bpm;   // Time between each beat

    // --- While True --- 
    while(true) {
        // --- Beatmap blinking --- 
            // N.B Green LED is beat 1. Assumes 4/4. Read Left to right  
        // TODO: Can put a for loop here
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, outputBitmask & 0x01);     // B5  -> First LED (Green)
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, outputBitmask & 0x02);     // B3  -> Second LED (Red)
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, outputBitmask & 0x04);     // B4  -> Third LED (Red)
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, outputBitmask & 0x08);    // B10 -> Fourth LED (Red)
         
        HAL_Delay(timeDelay); 
        // TODO: Right now we poll the buttons after each beat

        if(outputBitmask > 8) {  // TODO this gets changed to 8 bits with 8 LEDs
            outputBitmask = 0x1; 
        } else {
            outputBitmask *= 2;
        }
        // --- Multiplexer Polling ---
        for(char channel = 0; channel < 5; channel++) {        // For each of the 16 channels 
                                                                // In reality only going to check 8 channels
            // TODO: Can put a for loop here  
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);//channel & 0x01);    // S0 
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);//channel & 0x02);      // S1  
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);//channel & 0x04);    // S2
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);//channel & 0x08);    // S3  
            
            //uint16_t result = ReadADC(&adcInstance, ADC_CHANNEL_0);     // Read our ADC
            uint16_t result = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

            char buff[100];
            sprintf(buff, "Channel%d: %d\r\n",channel, result);  // %hu == "unsigned short" (16 bit)
            SerialPuts(buff);
        }
    } 

    return 0;
}

void SysTick_Handler(void) {
    HAL_IncTick();
}

bool delayWhilePolling(double delay, GPIO_TypeDef* buttonPort, uint8_t buttonPin) {
    float endTime = HAL_GetTick() + delay;

    while (HAL_GetTick() < endTime) {
        return !(HAL_GPIO_ReadPin(buttonPort, buttonPort));
    }

    return false;
}
