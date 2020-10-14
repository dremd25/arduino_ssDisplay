/**
 * @file seven_segment_defs.h
 * @author Darren Remund (darren.remund@gmail.com)
 * @brief Define constants for the seven segment display.
 * @version 0.1
 * @date 2020-10-11
 * 
 * 
 */

#include "Arduino.h"

#ifndef SEVEN_SEGMENT_DEFS_H
#define SEVEN_SEGMENT_DEFS_H

#ifdef __cplusplus
extern "C"{
#endif

#define SEVEN_SEGMENT_DBG_PRINT

#ifndef ARDUINO_MS_PER_TICK
 #define ARDUINO_MS_PER_TICK 1
#endif

#define SEVEN_SEGMENT_STROBE_PERIOD_MS 10

#define SEVEN_SEGMENT_NUM_PINS 8
#define SEVEN_SEGMENT_MAX_VAL 16
#define SEVEN_SEGMENT_MAX_NUM_DIGITS 8
#define SEVEN_SEGMENT_BLANK 16
#define SEVEN_SEGMENT_INIT_VAL SEVEN_SEGMENT_BLANK

typedef uint8_t sseg_length_t;
typedef uint16_t sseg_periodCounter_t;
typedef uint8_t sseg_pin_t;
typedef uint8_t sseg_val_t;

typedef struct sseg_pins {
    // sseg_pin_t a, b, c, d, e, f, g, dp;
    sseg_pin_t pins[8];         //! Pin list for seven segment display
    sseg_pin_t *digits;         //! Pin list for digit cathods
    sseg_length_t numDigits;    //! Number of digit on the display (Max: 8)
} sseg_pins_t;

/// Default pins for Arduino Uno
#define SEVEN_SEGMENT_DEFAULT_PINS {        \
        .pins= { 0, 0, 0, 0, 0, 0, 0, 0 }   \
        .numDigits=1                        \
    }

#ifdef __cplusplus
}
#endif

#endif  /* SEVEN_SEGMENT_DEFS_H */
