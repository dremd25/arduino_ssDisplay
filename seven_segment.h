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

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief Initializes statemachine and pins for a Seven Second Delay.
 * 
 * @param pinsIn    Array containing the pin maping, first the seven segment pins then the commong pins.
 * @param numDigits Number of digits controlled by driver, if 0 is entered, assumed that there is only one digit.
 * 
 */
void sseg_init(sseg_pin_t *pinsIn, sseg_len_t numDigits);

/**
 * @brief Waits for SSEG_STROBE and then switches digits.
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

#ifdef __cplusplus
}
#endif
#endif     /* SEVEN_SEG_H */
