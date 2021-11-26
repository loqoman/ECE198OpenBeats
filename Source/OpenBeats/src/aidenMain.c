#include "ece198.c"
#include <stdbool.h>
#include <stdio.h>   

// --- Pin Assignments ---
// LEDs 
#define LED1G   GPIOB, GPIO_PIN_10    // The 1st LED is beat 1 of measure 1
#define LED2    GPIOB, GPIO_PIN_9
#define LED3    GPIOB, GPIO_PIN_4  
#define LED4    GPIOA, GPIO_PIN_6
#define LED5G   GPIOB, GPIO_PIN_5     // The 5th LED is beat 1 of measure 2
#define LED6    GPIOC, GPIO_PIN_6
#define LED7    GPIOA, GPIO_PIN_10
#define LED8    GPIOC, GPIO_PIN_4

// Buttons
#define BUTTON1 GPIOC, GPIO_PIN_10   
#define BUTTON2 GPIOC, GPIO_PIN_12
#define BUTTON3 GPIOC, GPIO_PIN_2
#define BUTTON4 GPIOC, GPIO_PIN_3 
#define BUTTON5 GPIOB, GPIO_PIN_15   
#define BUTTON6 GPIOC, GPIO_PIN_13
#define BUTTON7 GPIOC, GPIO_PIN_14 
#define BUTTON8 GPIOC, GPIO_PIN_15

// Instruments
#define SNARE   GPIOA, GPIO_PIN_9
#define TOM     GPIOC, GPIO_PIN_7
#define CYMBOL  GPIOB, GPIO_PIN_6
#define BASS    GPIOA, GPIO_PIN_7

// Selector Switch
#define POS1    GPIOA, GPIO_PIN_0
#define POS2    GPIOA, GPIO_PIN_1
#define POS3    GPIOA, GPIO_PIN_4
#define POS4    GPIOB, GPIO_PIN_0

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

    size_t currentIndex = currentBeat - 2;

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
    
    // LEDS pluigged into pins D6-D3
    // TODO: Ping definitions up here with #Define operators 

    __HAL_RCC_GPIOA_CLK_ENABLE(); 
    __HAL_RCC_GPIOB_CLK_ENABLE(); 
    __HAL_RCC_GPIOC_CLK_ENABLE(); 
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
    InitializePin(BUTTON5, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);  // PC13
    InitializePin(BUTTON6, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);  // PC14
    InitializePin(BUTTON7,  GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);  // PC2
    InitializePin(BUTTON8,  GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);  // PC3 
   
    // --- Setting everything to 0 --- 
    // Turn off all LEDs
    HAL_GPIO_WritePin(LED1G, 0);      
    HAL_GPIO_WritePin(LED2,  0);
    HAL_GPIO_WritePin(LED3,  0);
    HAL_GPIO_WritePin(LED4,  0);
    HAL_GPIO_WritePin(LED5G, 0);      
    HAL_GPIO_WritePin(LED6,  0);
    HAL_GPIO_WritePin(LED7,  0);
    HAL_GPIO_WritePin(LED8,  0);
    // --- Init The Insturments ---
    InitializePin(SNARE,    GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0);  // PC13
    InitializePin(TOM,      GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0);  // PC14
    InitializePin(CYMBOL,   GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0);  // PC2
    InitializePin(BASS,     GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0);  // PC3 
    // --- INit the selector switch
    InitializePin(POS1,     GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);  // PC13
    InitializePin(POS2,     GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);  // PC14
    InitializePin(POS3,     GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);  // PC2
    InitializePin(POS4,     GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);  // PC3 

    // Set up serial
    SerialSetup(9600);
    HAL_Delay(1000);

    SerialPuts("--- Running combinedMain.c --- ");  

    int outputBitmask = 1; 
    int bpm = 350;                  // Change if you want a different bpm 
    int timeDelay = 60000 / bpm;   // Time between each beat

    //bool led1State = 1, led2State = 1, led3State = 1, led4State = 1;  // TODO: Will be 8 here 
    bool snareState[8]  = {1,0,1,0,1,0,1,1};
    bool tomState[8]    = {0,1,0,1,0,1,0,1};
    bool cymbolState[8] = {0,0,1,0,1,1,0,1};
    bool bassState[8]   = {1,0,0,0,1,0,0,0};
    // --- While True --- 
    int beat = 1;
    while(true) { 
        bool currentInsturment[8] = {1,1,1,1,1,1,1,1};
        // Go get the insturment states we're operating on
        // NOTE: Green LED is beat 1. Assumes 4/4. Read Left to right  
        // Increment the current beat

        //char buf[70];
        //sprintf(buf, "The current value of outputBitmask %d of beat: %d, AND function is %d\n", outputBitmask, beat, (outputBitmask & 32));
        //SerialPuts(buf);
        if (currentInsturment[0]) {
            HAL_GPIO_WritePin(LED1G, outputBitmask & 1);     // B5  -> First LED (Green)
        } if (currentInsturment[1]) {
            HAL_GPIO_WritePin(LED2, outputBitmask & 2);      // B3  -> Second LED (Red)
        } if (currentInsturment[2]) {
            HAL_GPIO_WritePin(LED3, outputBitmask & 4);      // B4  -> Third LED (Red) 
        } if (currentInsturment[3]) {
            HAL_GPIO_WritePin(LED4, outputBitmask & 8);      // B10 -> Fourth LED (Red) 
        } if (currentInsturment[4]) {
            HAL_GPIO_WritePin(LED5G, outputBitmask & 16);     // B5  -> First LED (Green)
        } if (currentInsturment[5]) {
            HAL_GPIO_WritePin(LED6, outputBitmask & 32);      // B3  -> Second LED (Red)

            //sprintf(buf, "The current value of outputBitmask %d of beat: %d, AND function is %d\n", outputBitmask, beat, (outputBitmask & 32));
            //SerialPuts(buf);
            //if ((outputBitmask & 32) != 0) {
                //HAL_GPIO_WritePin(LED6, 1);
            //}
        } if (currentInsturment[6]) {
            HAL_GPIO_WritePin(LED7, outputBitmask & 64);      // B4  -> Third LED (Red) 
        } if (currentInsturment[7]) {
            HAL_GPIO_WritePin(LED8, outputBitmask & 128);      // B4  -> Third LED (Red) 
        }
        if(outputBitmask >= 128) {   // TODO this gets changed to 8 bits with 8 LEDs
            outputBitmask = 0x1;
            beat = 1; 
        } else {
            outputBitmask *= 2;     // Shift the output bitmask
            beat++; 
        }
        
        playBar(snareState, tomState, cymbolState, bassState, timeDelay, beat); 

                                // NOTE: This is where playBar() will go 
        // --- Button Polling ---  
        bool button1 = HAL_GPIO_ReadPin(BUTTON1);
        bool button2 = HAL_GPIO_ReadPin(BUTTON2);
        bool button3 = HAL_GPIO_ReadPin(BUTTON3);
        bool button4 = HAL_GPIO_ReadPin(BUTTON4);   
        bool button5 = HAL_GPIO_ReadPin(BUTTON5);
        bool button6 = HAL_GPIO_ReadPin(BUTTON6);
        bool button7 = HAL_GPIO_ReadPin(BUTTON7);
        bool button8 = HAL_GPIO_ReadPin(BUTTON8);   
    
        if(button1) {currentInsturment[0] ^= button1;}
        if(button2) {currentInsturment[1] ^= button2;}
        if(button3) {currentInsturment[2] ^= button3;}
        if(button4) {currentInsturment[3] ^= button4;} 
        if(button5) {currentInsturment[4] ^= button5;}
        if(button6) {currentInsturment[5] ^= button6;}
        if(button7) {currentInsturment[6] ^= button7;}
        if(button8) {currentInsturment[7] ^= button8;} 
    } 
    return 0;
}

void SysTick_Handler(void) {
  HAL_IncTick(); 
}

//magic selector switch code
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
