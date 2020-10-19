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

#define SSEG_STROBE (SSEG_STROBE_MS / ARDUINO_MS_PER_TICK)


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

/**
 * @brief State machine states for sseg driver.
 * 
 */
typedef enum sseg_states {
    sseg_init_e,                                //! State machine entry point, initializes driver.
    sseg_idle_e,                                //! Counts ticks for each digit to be lit.
    sseg_print_e,                               //! Sets the pin values for current digit and value.
    sseg_nextDigit_e                            //! Moves to the next digit.
} sseg_states_t;

static sseg_states_t currentState;              //! State machine veriable.
static sseg_val_t *sseg_digitVals;              //! Values in each digit.
static sseg_pin_t *sseg_pins;                   //! Pin (a-dp) mapping.
static sseg_pin_t *sseg_digitPins;              //! Common pin mapping.
static sseg_len_t sseg_numDigits;               //! Number of digits in the display (0 = no auto control of digits).
static sseg_len_t sseg_digit;                   //! Current digit displayed.
static sseg_counter_t sseg_counter;             //! Tick counter.
static bool sseg_pending;                       //! Digit value change flag.


#ifdef SSEG_DBG_PRINT
void sseg_tickDBPrint();
#endif


//
void sseg_init(sseg_pin_t *pinsIn, sseg_len_t numDigits) {
    // Initialize state machine
    currentState = sseg_init_e;

    sseg_digit = 0;
    sseg_counter = 0;
    sseg_pending = true;
    sseg_numDigits = numDigits;

    sseg_pins = malloc(SSEG_NUM_PINS * sizeof(sseg_pin_t));

    // Check if there is digit control enabled
    if (!sseg_numDigits) {
        // If there is no digit control enabled, assume there is one digit.
        sseg_digitVals = malloc(sizeof(sseg_val_t));
        sseg_digitVals[0] = SSEG_INIT_VAL;
    }
    else {
        // Initialize digit pins and digit values.
        sseg_digitVals = malloc(sseg_numDigits * sizeof(sseg_val_t));
        sseg_digitPins = malloc(sseg_numDigits * sizeof(sseg_pin_t));
        for (int i = 0; i < sseg_numDigits; i++) {
            sseg_digitPins[i] = pinsIn[i + SSEG_NUM_PINS];
            sseg_digitVals[i] = SSEG_INIT_VAL;
            pinMode(sseg_digitPins[i], OUTPUT);
        }
    }

    // Initialize seven segment pins
    for (int i = 0; i < SSEG_NUM_PINS; i++)
        pinMode(sseg_pins[i], OUTPUT);
}

void sseg_setDigit(uint8_t digit, sseg_val_t val) {
    // Check if value or digit it out of range
    if (val > SSEG_MAX_VAL || digit >= sseg_numDigits)
        return;
    // Save new value and toggle flag
    sseg_pending = true;
    sseg_digitVals[digit] = val;
}

void sseg_tick() {
    // Moore
    switch(currentState) {
    case sseg_init_e:
        currentState = sseg_idle_e;
        break;
    case sseg_idle_e:
        // Check if there is new data
        if (sseg_pending)
            currentState = sseg_print_e;
        // Check if it's time to switch digits
        else if (sseg_counter >= SSEG_STROBE) 
            currentState = sseg_nextDigit_e;
        break;
    case sseg_print_e:
        // Return to idle, clear pending change flag
        sseg_pending = false;
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
        // Set active segments HIGH
        for (int i = 0; i < SSEG_NUM_PINS; i++)
            digitalWrite(sseg_pins[i], sseg_code[sseg_digitVals[sseg_digit]][i]);
        // Set active digit LOW
        for (int i = 0; i < sseg_numDigits; i++)
            digitalWrite(sseg_digitPins[i], i != sseg_digit);
        // Todo: add support for dp and Common Anode
        break;
    case sseg_nextDigit_e:
        // Increase the digit to max and then loop back to 0
        sseg_digit++;
        if (sseg_digit >= sseg_numDigits)
            sseg_digit = 0;
        break;
    default:
        break;
    }
#ifdef SSEG_DBG_PRINT
    // If debug in enabled, print state machine transisions.
    sseg_tickDBPrint();
#endif
}


#ifdef SSEG_DBG_PRINT
static sseg_states_t prevState;

/**
 * @brief 
 * 
 */
void sseg_tickDBPrint() {
    if (currentState != prevState) {
        switch(currentState) {
        case sseg_init_e:
            Serial.println("STATE: sseg_init_e");
            break;
        case sseg_idle_e:
            Serial.println("STATE: sseg_idle_e");
            break;
        case sseg_print_e:
            Serial.println("STATE: sseg_print_e");
            break;
        case sseg_nextDigit_e:
            Serial.println("STATE: sseg_nextDigit_e");
            break;
        default:
            Serial.println("SSEG ERROR");
            break;
        }
    }

    prevState = currentState;
}

#endif  /* SEVEN_SEGMENT_DBG_PRINT */