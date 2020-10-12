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

 static uint8_t *ss_digit_vals;                     //! Values in each digit.
 static sevenSegment_length_t ss_digit_num;         //! Number of digits.
 static sevenSegment_length_t ss_digit_disp;        //! Current digit displayed.