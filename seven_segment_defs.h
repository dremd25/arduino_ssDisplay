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

// #define SSEG_DBG_PRINT

#ifndef ARDUINO_MS_PER_TICK
 #define ARDUINO_MS_PER_TICK 1
#endif

#define SSEG_STROBE_MS 10

#define SSEG_NUM_PINS 8
#define SSEG_MAX_VAL 16
#define SSEG_MAX_NUM_DIGITS 8
#define SSEG_BLANK 16
#define SSEG_INIT_VAL SSEG_BLANK

typedef uint8_t sseg_len_t;
typedef uint16_t sseg_counter_t;
typedef uint8_t sseg_pin_t;
typedef uint8_t sseg_val_t;

/// Default pins for Arduino Uno
#define SSEG_DEFAULT_UNO_PINS { 0, 0, 0, 0, 0, 0, 0, 0 }

#ifdef __cplusplus
}
#endif

#endif  /* SEVEN_SEGMENT_DEFS_H */
