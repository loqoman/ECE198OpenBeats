#include "ece198.c"
#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions

// #define INS1 GPIOA, GPIO_PIN_9
// #define INS2 GPIOC, GPIO_PIN_7
// #define INS3 GPIOB, GPIO_PIN_6
// #define INS4 GPIOA, GPIO_PIN_7

// bool delayWhilePolling( double delay, GPIO_TypeDef* buttonPort, uint16_t
// buttonPin ) {
//     double endTime = HAL_GetTick() + delay;

//     while (HAL_GetTick() < endTime) {
//         if (HAL_GPIO_ReadPin(buttonPort, buttonPin) > 0) {
//             return true;
//         }
//     }

//     return false;
// }

typedef struct {
  GPIO_TypeDef* port;
  uint16_t pin;
} pin_st;

void playInstrument( GPIO_TypeDef* buttonPort, uint16_t buttonPin, unsigned int note_len, unsigned int delay ) {
  HAL_GPIO_WritePin(buttonPort, buttonPin, 1);
  HAL_Delay(note_len);
  HAL_GPIO_WritePin(buttonPort, buttonPin, 0);
  HAL_Delay(delay);
}

void playBar(
  bool* snare_arr, 
  bool* tom_arr, 
  bool* cymbol_arr, 
  bool* bass_arr,
  pin_st* snare,
  pin_st* tom,
  pin_st* cymbol,
  pin_st* bass, 
  unsigned int BPM) {

    // assume to be in 4/4 timing, which implies that we have equal notes
    // four notes of equal sequencing    
    float beat_len = 60.0 / BPM * 1000; // convert to ms
    float time_bar = 4 * beat_len;

    // but still must be much lower
    float note_len = time_bar / 8;

    // TODO - determine how long to pause in order to make the note

    // keep in mind that we're only playing one bar
    // and there are four beats in the bar

    // all off the arrays are going to be predefined as being eight elements
    // loop from 0 to 7

    for ( unsigned int i = 0; i < 8; i++ ) {
      if (// we detect)
      // play
    }
}

int main(void) {
  HAL_Init();

  __HAL_RCC_GPIOA_CLK_ENABLE(); // enable port A (for the on-board LED, for
  __HAL_RCC_GPIOB_CLK_ENABLE(); // enable port B (for the rotary encoder inputs,
  __HAL_RCC_GPIOC_CLK_ENABLE(); // enable port C (for the on-board blue

  // InitializePin(GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);
  // InitializePin(GPIOB, GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);
  // InitializePin(GPIOB, GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);
  // InitializePin(GPIOB, GPIO_PIN_4, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);
  // InitializePin(GPIOB, GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0);

  // SNARE PIN
  // InitializePin(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0);

  // BASS PIN
  // InitializePin(GPIOA, GPIO_PIN_6, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0);

  // InitializePin(INS1, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0);
  // InitializePin(INS2, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0);
  // InitializePin(INS3, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0);
  // InitializePin(INS4, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0);
  // note: the on-board pushbutton is fine with the default values (no internal
  // pull-up resistor is required, since there's one on the board)




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
    unsigned int pause = 80;

    playInstrument(INS1, pause, 0);
    playInstrument(INS2, pause, pause);
    playInstrument(INS2, pause, pause);
    playInstrument(INS3, pause, pause);
    playInstrument(INS4, pause, pause);
    
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
void SysTick_Handler(void) {
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
