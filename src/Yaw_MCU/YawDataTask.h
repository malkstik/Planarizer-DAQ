/** @file YawDataTask.h
 *  This task is responsible for reading the encoder and sending the data in a queue to @c YawSerialTask.cpp
 *  
 *  Source code available here:
 *  @c https://github.com/malkstik/Planarizer-DAQ/blob/main/src/Yaw_MCU/YawDataTask.h
 *  @author  Aaron Tran
 *  @date    2022-Jan-17 Original file
 */

#ifndef _YAWDATATASK_H
#define _YAWDATATASK_H

void task_data(void* p_params);

#endif // _YAWDATATASK_H