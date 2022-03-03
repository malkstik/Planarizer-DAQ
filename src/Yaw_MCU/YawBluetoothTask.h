/** @file YawBluetoothTask.cpp
 *  Sends data over serial communication to frontend, @c PlanarizerDAQFrontEnd.mlx
 *  
 *  Source code available here:
 *  @c https://github.com/malkstik/Planarizer-DAQ/blob/main/src/Yaw_MCU/YawBluetoothTask.h
 *  @author  Aaron Tran
 *  @date    2022-Feb-28 Original file
 */


#ifndef _YAWBLUETOOTHTASK_H
#define _YAWBLUETOOTHTASK_H

void task_bluetooth(void* p_params);

#endif // _YAWBLUETOOTHTASK_H