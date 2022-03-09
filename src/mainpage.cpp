/*! \mainpage Planarizer DAQ Documentation
 *
 * \section intro_sec Introduction
 *
 * Our Senior Project Group, consisting of Aaron Tran, Eric Ramos, Matthew Wimberley, and Tim Jain, was tasked with designing and manufacturing a test stand for the Cal Poly Legged Robot Team. 
 * The team needed a way to constrain a single robot leg to 2DOF in the sagittal plane. Hence, our test stand "planarizes" the robot leg. With the additional need of requiring that the robot 
 * leg be able to reach a steady state gait, we determined that a boom system would work best, though it only approximates the sagittal plane. This DAQ is responsible for measuring the corresponding 
 * rotational value of the Planarizer, allowing for us to track the position of the robot leg during testing.
 * 
 * The DAQ consists of two STM32L476RG Nucleos, synchronized using two HiLetgo HC-05 bluetooth transceivers. The DAQ makes use of two US Digital E6 Optical Kit Encoders to measure the yaw 
 * and pitch of the encoders. These values are then processed, plotted, and saved in MATLAB. 
 * \n
 * For more information, refer to the following links \n
 *     - GitHub:                   https://github.com/malkstik/Planarizer-DAQ
 *     - Senior Project Report:    
 * 
 * \section yaw_task_dia Task Diagram 
 * 
 * 
 * 
 * \section st_dia State Transition Diagram
 * 
 * While each task can be considered to be running as its own finite state machine, each is synchronized to be in the same overall state. Accordingly, one 
 * state transition diagram is adequate to describe each task.
 */