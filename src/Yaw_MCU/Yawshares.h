/** @file Yawshares.h
 *  This file contains extern declarations of shares and queues which are used
 *  in more than one file of a demonstration project.
 * 
 *  Source code available here: https://github.com/malkstik/Planarizer-DAQ/blob/main/src/Yaw_MCU/Yawshares.h
 *  @author Aaron Tran
 *  @date   2022-Jan-18 Original file
 */

#ifndef _YAWSHARES_H_
#define _YAWSHARES_H_

#include "taskqueue.h"
#include "taskshare.h"
#include "textqueue.h"

/// @brief Yaw datopopla
extern Queue<float> yaw;

/// @brief Time data
extern Queue<float> yaw_time;     

/// @brief Yaw checksum
extern Queue<float> yaw_crc;

/// @brief Pitch data via bluetooth
extern TextQueue serial_queue;

/// @brief Data_state
extern Share<uint8_t> data_state;

/// @brief 
extern Share<unsigned long> zero_time;


#endif // _YAWSHARES_H_
