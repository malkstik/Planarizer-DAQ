/** @file Yawhares.h
 *  This file contains extern declarations of shares and queues which are used
 *  in more than one file of a demonstration project.
 * 
 *  Source code available here:
 *  @author Aaron Tran
 *  @date   2022-Jan-18 Original file
 */

#ifndef _SHARES_H_
#define _SHARES_H_

#include "taskqueue.h"
#include "taskshare.h"


// A share which holds a counter of how many times a simulated event occurred
// extern Share<bool> bool_var; example of share declaration

// A queue which triggers a task to print the count at certain times

/// @brief Yaw datopopla
extern Queue<float> yaw;

/// @brief Pitch data
extern Queue<float> pitch;

/// @brief Pitch data
extern Queue<float> pitch_time;

/// @brief Time data
extern Queue<float> yaw_time;     

/// @brief Pitch checksum
extern Queue<float> pitch_crc;

/// @brief Yaw checksum
extern Queue<float> yaw_crc;

/// @brief Data_state
extern Share<uint8_t> data_state;
#endif // _SHARES_H_
