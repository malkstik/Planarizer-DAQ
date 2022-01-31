/** @file shares.h
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

/// @brief Yaw data
extern Queue<float> yaw;

/// @brief Pitch data
extern Queue<float> pitch;

/// @brief Time data
extern Queue<float> time_data;     
#endif // _SHARES_H_
