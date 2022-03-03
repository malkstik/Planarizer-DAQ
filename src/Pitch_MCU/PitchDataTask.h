/** @file PitchDataTask.h
 *  This class is responsible for reading the encoder and sending the data in a queue to @c PitchBluetoothTask.cpp
 *  
 *  Source code available here:
 *  @c https://github.com/malkstik/Planarizer-DAQ/blob/main/src/Pitch_MCU/PitchDataTask.h
 *  @author  Aaron Tran
 *  @date    2022-Jan-17 Original file
 */

#ifndef _PITCHDATATASK_H
#define _PITCHDATATASK_H

void task_data(void* p_params);

#endif // _PITCHDATATASK_H