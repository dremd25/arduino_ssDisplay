/**
 * @file seven_segment.h
 * @author Darren Remund (darren.remund@gmail.com)
 * @brief Arduino driver for a LED Seven Segment Display.
 * @version 0.1
 * @date 2020-10-11
 * 
 * 
 */

#include "seven_segment_defs.h"

#ifndef SEVEN_SEG_H
#define SEVEN_SEG_H

/**
 * @brief Initializes statemachine and pins for a Seven Second Delay.
 * 
 * @param pinsIn    Struct containing pin numbers and the number of digits.
 * @param digits    Array of common pins, if already added in pinsIn then use NULL.
 * 
 */
void sseg_init(sseg_pins_t pinsIn, sseg_pins_t *digits);

/**
 * @brief Waits for SEVEN_SEGMENT_STROBE_PERIOD_MS and then switches digits.
 * 
 * @warning If intended to use with other code, add as interrupt call back on a timer. Must be initialized by user.
 */
void sseg_tick();

/**
 * @brief Set the value of any given digit.
 * 
 * @param digit Digit value 0-7.
 * @param val Value to be written 0x0-0xf, 0x10 for blank.
 */
void sseg_setDigit(uint8_t digit, sseg_val_t val);

#endif     /* SEVEN_SEG_H */
