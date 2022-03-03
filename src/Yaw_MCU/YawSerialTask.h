/** @file YawSerialTask.cpp
 *  Sends data over serial communication to frontend, @c PlanarizerDAQFrontEnd.mlx
 *  
 *  Source code available here:
 *  @c https://github.com/malkstik/Planarizer-DAQ/blob/main/src/Yaw_MCU/YawSerialTask.h
 *  @author  Aaron Tran
 *  @date    2022-Jan-18 Original file
 */


#ifndef _YAWSERIALTASK_H
#define _YAWSERIALTASK_H

void task_serial(void* p_params);

#endif // _YAWSERIALTASK_H