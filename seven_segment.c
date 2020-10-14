/**
 * @file seven_segment.c
 * @author Darren Remund (darren.remund@gmail.com)
 * @brief Implimentation of seven segment driver.
 * @version 0.1
 * @date 2020-10-11
 * 
 * 
 */

#include "seven_segment.h"

#define SEVEN_SEGMENT_STROBE_TICKS (SEVEN_SEGMENT_SEVEN_SEGMENT_STROBE_PERIOD_MS / ARDUINO_MS_PER_TICK)

static uint8_t *sseg_digit_vals;                //! Values in each digit.
static sseg_pins_t sseg_pins;                   //! Pin mapping.
static sseg_length_t sseg_digit_disp;           //! Current digit displayed.
static sseg_periodCounter_t sseg_counter;       //! Tick counter.
static bool sseg_changePending;                 //! Digit value change flag.

const uint8_t sseg_code[18][8] = {              //! Active pins for each corresponding number
    // 7 segment code table
    //  a  b  c  d  e  f  g  . 
    { 1, 1, 1, 1, 1, 1, 0, 0},  // 0
    { 0, 1, 1, 0, 0, 0, 0, 0},  // 1
    { 1, 1, 0, 1, 1, 0, 1, 0},  // 2
    { 1, 1, 1, 1, 0, 1, 0, 0},  // 3
    { 0, 1, 1, 0, 0, 1, 1, 0},  // 4
    { 1, 0, 1, 1, 0, 1, 1, 0},  // 5
    { 1, 0, 1, 1, 1, 1, 1, 0},  // 6
    { 1, 1, 1, 0, 0, 0, 0, 0},  // 7
    { 1, 1, 1, 1, 1, 1, 1, 0},  // 8
    { 1, 1, 1, 1, 0, 1, 1, 0},  // 9
    { 1, 1, 1, 0, 1, 1, 1, 0},  // a
    { 0, 0, 1, 1, 1, 1, 1, 0},  // b
    { 1, 0, 0, 1, 1, 1, 0, 0},  // c
    { 0, 1, 1, 1, 1, 0, 1, 0},  // d
    { 1, 0, 0, 1, 1, 1, 1, 0},  // e
    { 1, 0, 0, 0, 1, 1, 1, 0},  // f
    { 0, 0, 0, 0, 0, 0, 0, 0},  // blank
    { 0, 0, 1, 1, 1, 0, 0, 0}   // error
};

static typedef enum sseg_states {
    sseg_init_e,
    sseg_idle_e,
    sseg_print_e,
    sseg_nextDigit_e
} sseg_states_t currentState;


#ifdef SEVEN_SEGMENT_DBG_PRINT
void sseg_tickDBPrint();
#endif


//
void sseg_init(sseg_pins_t pinsIn, sseg_pins_t *digits) {
    // Initialize state machine
    currentState = sseg_init_e;

    sseg_pins = pinsIn;
    sseg_digit_disp = 0;
    sseg_counter = 0;
    sseg_changePending = true;

    // Initialize each digit to default value
    for (i = 0; i < sseg_pins.numDigits; i++) {
        sseg_digit_vals[i] = SEVEN_SEGMENT_INIT_VAL;
    }

    // Save and initialize cathode pins
    // Todo: Add Common Anode Mode
    if (sseg_pins.digits != NULL) {
        for (i = 0; i < sseg_pins.numDigits; i++) {
            pinMode(digits[i], OUTPUT);
        }
    }
    else if (digits != NULL) {
        sseg_pins.digits = malloc(sseg_pins.numDigits * sizeof(sseg_pint_t));
        for (i = 0; i < sseg_pins.numDigits; i++) {
            sseg_pins.digits[i] = digits[i];
            pinMode(digits[i], OUTPUT);
        }
    }

    // Initialize seven segment pins
    for (int i = 0; i < SEVEN_SEGMENT_NUM_PINS; i++)
        pinMode(sseg_pins.pins[i], OUTPUT);
}

void sseg_setDigit(uint8_t digit, uint8_t val) {
    // Check if value or digit it out of range
    if (val > SEVEN_SEGMENT_MAX_VAL || digit >= sseg_pins.numDigits)
        return;
    // Save new value and toggle flag
    sseg_changePending = true;
    sseg_digit_vals[digit] = val;
}

void sseg_tick() {
    // Moore
    switch(currentState) {
    case sseg_init_e:
        currentState = sseg_idle_e;
        break;
    case sseg_idle_e:
        // Check if there is new data
        if (sseg_changePending)
            currentState = sseg_print_e;
        // Check if it's time to switch digits
        else if (sseg_counter >= SEVEN_SEGMENT_STROBE_TICKS) 
            currentState = sseg_nextDigit_e;
        break;
    case sseg_print_e:
        // Return to idle, clear pending change flag
        sseg_changePending = false;
        currentState = sseg_idle_e;
        break;
    case sseg_nextDigit_e:
        // Reset counter, start printing next digit
        sseg_counter = 0;
        currentState = sseg_print_e;
        break;
    default:
        currentState = sseg_idle_e;
        break;
    }
    
    // Mealy
    switch(currentState) {
    case sseg_init_e:
        break;
    case sseg_idle_e:
        // Increment the tick timer
        sseg_counter++;
        break;
    case sseg_print_e:
        // Print digit by setting sseg pins HIGH and digit pin LOW
        for (int i = 0; i < SEVEN_SEGMENT_NUM_PINS; i++)
            digitalWrite(sseg_pins.pins[i], sseg_code[sseg_digit_vals[sseg_digit_disp]][i]);
        for (int i = 0; i < sseg_pins.numDigits; i++)
            digitalWrite(sseg_pins.digits[i], i != sseg_digit_disp);
        // Todo: add support for dp
        break;
    case sseg_nextDigit_e:
        // Increase the digit to max and then loop back to 0
        sseg_digit_disp++;
        if (sseg_digit_disp >= sseg_pins.numDigits)
            sseg_digit_disp = 0;
        break;
    default:
        break;
    }
#ifdef SEVEN_SEGMENT_DBG_PRINT
    // If debug in enabled, print state machine transisions.
    sseg_tickDBPrint();
#endif
}


#ifdef SEVEN_SEGMENT_DBG_PRINT
static sseg_states_t prevState;

/**
 * @brief 
 * 
 */
void sseg_tickDBPrint() {
    
}

#endif  /* SEVEN_SEGMENT_DBG_PRINT */
