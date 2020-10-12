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

 static sevenSegment_period_ms_t ss_period;

/**
 * @brief 
 * 
 */
 void seven_segment_init();

/**
 * @brief 
 * 
 */
 void seven_segment_tick();

/**
 * @brief 
 * 
 * @param val 
 */
 void seven_segment_setDigit(uint8_t val);

 #endif     /* SEVEN_SEG_H */
