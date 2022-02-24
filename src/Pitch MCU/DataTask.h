/** @file DataTask.h
 *  This class is responsible for reading the encoder and sending the data in a queue to @c SerialTask.cpp
 *  
 *  Source code available here:
 *  @c https://github.com/malkstik/Planarizer-DAQ/blob/main/src/DataTask.h
 *  @author  Aaron Tran
 *  @date    2022-Jan-17 Original file
 */

#ifndef _DATATASK_H
#define _DATATASK_H

void task_data(void* p_params);

#endif // _DATATASK_H