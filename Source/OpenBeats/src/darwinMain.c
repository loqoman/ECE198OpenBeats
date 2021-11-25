#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions

#include "ece198.c"

// --- Pin Assignments ---
// LEDs (temp)
#define LED1G GPIOB, GPIO_PIN_10     // The 1st LED is beat 1 of measure 1
#define LED2 GPIOB, GPIO_PIN_9
#define LED3 GPIOB, GPIO_PIN_4  
#define LED4 GPIOA, GPIO_PIN_6
#define LED5G GPIOA, GPIO_PIN_7                     // The 5th LED is beat 1 of measure 2
#define LED6 GPIOB, GPIO_PIN_6 
#define LED7 GPIOC, GPIO_PIN_7
#define LED8 GPIOA, GPIO_PIN_9

// Buttons
#define BUTTON1 GPIOC, GPIO_PIN_10   // Button 1 must correpond to led 1
#define BUTTON2 GPIOC, GPIO_PIN_12
#define BUTTON3 GPIOA, GPIO_PIN_13
#define BUTTON4 GPIOA, GPIO_PIN_14 
#define BUTTON5 GPIOB, GPIO_PIN_15   // Button 1 must correpond to led 1
#define BUTTON6 GPIOC, GPIO_PIN_13
#define BUTTON7 GPIOC, GPIO_PIN_14 
#define BUTTON8 GPIOC, GPIO_PIN_15

// Button Multiplexer Pins
#define MUXSIG GPIOB, GPIO_PIN_8

#define MUXS3 GPIOB, GPIO_PIN_15
#define MUXS2 GPIOB, GPIO_PIN_14
#define MUXS1 GPIOB, GPIO_PIN_1
#define MUXS0 GPIOB, GPIO_PIN_13


int main(void) {
    HAL_Init(); // initialize the Hardware Abstraction Layer
    // Peripherals (including GPIOs) are disabled by default to save power, so we
    // use the Reset and Clock Control registers to enable the GPIO peripherals that we're using.
    
    // LEDS pluigged into pins D6-D3
    // TODO: Ping definitions up here with #Define operators 

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
    InitializePin(LED1G, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED
    InitializePin(LED2, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED
    InitializePin(LED3, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED
    InitializePin(LED4, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED
    InitializePin(LED5G, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED
    InitializePin(LED6, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED
    InitializePin(LED7, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED
    InitializePin(LED8, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED

    // --- (temp) Button Pins --- // Will get removed
    InitializePin(BUTTON1, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);  // PC13
    InitializePin(BUTTON2, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);  // PC14
    InitializePin(BUTTON3,  GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);  // PC2
    InitializePin(BUTTON4,  GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);  // PC3 
   
    // --- CD74HC Pins --- 
    // The Sig pin (A0) is used as the input and the 4 digital pin is used to select which pin you 
    // InitializePin(GPIOA, GPIO_PIN_0,  GPIO_MODE_ANALOG, GPIO_NOPULL, 0);    // SIG -> A0
                                                                            // Must be an analog pin
    // N.B This line assoicated the ADC to pin0, then you read ADC0, not pin0 
    InitializePin(MUXSIG,  GPIO_MODE_INPUT, GPIO_NOPULL, 0);    // SIG -> A0 

    InitializePin(MUXS3, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // S3 -> PB15
    InitializePin(MUXS2, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // S2 -> PB14 
    InitializePin(MUXS1, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // S1 -> PB1
    InitializePin(MUXS0, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // S0 -> PB13
    
    // note: the on-board pushbutton is fine with the default values (no internal pull-up resistor
    // is required, since there's one on the board)
    
    // --- Setting everything to 0 --- 
    // Set all the CH74HC pins to zero 
    HAL_GPIO_WritePin(MUXS3, 0);   // S3 
    HAL_GPIO_WritePin(MUXS2, 0);   // S2
    HAL_GPIO_WritePin(MUXS1, 0);   // S1
    HAL_GPIO_WritePin(MUXS0, 0);   // S0 
    // Turn off all LEDs
    HAL_GPIO_WritePin(LED1G, 0);      
    HAL_GPIO_WritePin(LED2,  0);
    HAL_GPIO_WritePin(LED3,  0);
    HAL_GPIO_WritePin(LED4,  0);
    HAL_GPIO_WritePin(LED5G, 0);      
    HAL_GPIO_WritePin(LED6,  0);
    HAL_GPIO_WritePin(LED7,  0);
    HAL_GPIO_WritePin(LED8,  0);

    // Set up serial
    SerialSetup(9600);
    HAL_Delay(1000);

    SerialPuts("--- Running darwinMain.c --- ");  


    int outputBitmask = 0x1; 
    int bpm = 220;                  // Change if you want a different bpm 
    int timeDelay = 60000 / bpm;   // Time between each beat

    //bool led1State = 1, led2State = 1, led3State = 1, led4State = 1;  // TODO: Will be 8 here 
    bool ledStates[8] = {1,1,1,1,1,1,1,1};
    // --- While True --- 
    while(true) {
        // --- Beatmap blinking --- 
        // TODO: This is where debouncing buffer goes

        // N.B Green LED is beat 1. Assumes 4/4. Read Left to right  
        // TODO: Can put a for loop here   
        // TODO: Can maybe go under the hood and do a bit shift with LED state object (not sure what stm likes)
        if(outputBitmask > 128) {  // TODO this gets changed to 8 bits with 8 LEDs
            outputBitmask = 0x1; 
        } else {
            outputBitmask *= 2;  // Shift the output bitmask
        }

        if (ledStates[0]) {
            HAL_GPIO_WritePin(LED1G, outputBitmask & 0x01);     // B5  -> First LED (Green)
        } if (ledStates[1]) {
            HAL_GPIO_WritePin(LED2, outputBitmask & 0x02);      // B3  -> Second LED (Red)
        } if (ledStates[2]) {
            HAL_GPIO_WritePin(LED3, outputBitmask & 0x04);      // B4  -> Third LED (Red) 
        } if (ledStates[3]) {
            HAL_GPIO_WritePin(LED4, outputBitmask & 0x08);      // B10 -> Fourth LED (Red) 
        }
        if (ledStates[4]) {
            HAL_GPIO_WritePin(LED5G, outputBitmask & 16);     // B5  -> First LED (Green)
        } if (ledStates[5]) {
            HAL_GPIO_WritePin(LED6, outputBitmask & 32);      // B3  -> Second LED (Red)
        } if (ledStates[6]) {
            HAL_GPIO_WritePin(LED7, outputBitmask & 64);      // B4  -> Third LED (Red) 
        } if (ledStates[7]) {
            HAL_GPIO_WritePin(LED8, outputBitmask & 128);      // B4  -> Third LED (Red) 
        }
        HAL_Delay(timeDelay);  // NOTE: This is the line that correponds to the BPM between beats
        // TODO: Right now we poll the buttons after each beat
        // 0b10000000 = 128 
        // --- Multiplexer Polling --- 
        /*
        for(char channel = 0; channel < 5; channel++) {        // For each of the 16 channels 
                                                                // In reality only going to check 8 channels
            // TODO: Can put a for loop here  
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);//channel & 0x01);    // S0 
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);//channel & 0x02);      // S1  
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);//channel & 0x04);    // S2
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);//channel & 0x08);    // S3  
            
            //uint16_t result = ReadADC(&adcInstance, ADC_CHANNEL_0);     // Read our ADC
            uint16_t result = HAL_GPIO_ReadPin(MUXSIG);

            char buff[100];
            sprintf(buff, "Channel%d: %d\r\n",channel, result);  // %hu == "unsigned short" (16 bit)
            SerialPuts(buff);
        }
        */
        
        // --- (temp) Button Polling ---
        // TODO: Re-Match Button defines with proper LED's in hard (rn buttons don't match with right led)
        bool button1 = HAL_GPIO_ReadPin(BUTTON1);
        bool button2 = HAL_GPIO_ReadPin(BUTTON2);
        bool button3 = HAL_GPIO_ReadPin(BUTTON3);
        bool button4 = HAL_GPIO_ReadPin(BUTTON4);   
        bool button5 = HAL_GPIO_ReadPin(BUTTON5);
        bool button6 = HAL_GPIO_ReadPin(BUTTON6);
        bool button7 = HAL_GPIO_ReadPin(BUTTON7);
        bool button8 = HAL_GPIO_ReadPin(BUTTON8);   
        
        if(button1) {ledStates[0] ^= button1;}
        if(button2) {ledStates[1] ^= button2;}
        if(button3) {ledStates[2] ^= button3;}
        if(button4) {ledStates[3] ^= button4;} 
        if(button5) {ledStates[4] ^= button5;}
        if(button6) {ledStates[5] ^= button6;}
        if(button7) {ledStates[6] ^= button7;}
        if(button8) {ledStates[7] ^= button8;} 
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
