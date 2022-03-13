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
 * and pitch of the encoders. These values are then processed, plotted, and saved in MATLAB. This code is currently able to collect data at 60Hz.
 * \n
 * For more information, refer to the following links \n
 *     - GitHub:                   https://github.com/malkstik/Planarizer-DAQ
 *     - Senior Project Report:    
 * 
 * \section yaw_task_dia Task Diagram 
 * 
 * \image html PlanarizerTD.png
 * 
 * \section st_dia State Transition Diagram
 * 
 * While each task can be considered to be running as its own finite state machine, each is synchronized to be in the same overall state. Accordingly, one 
 * state transition diagram is adequate to describe each task.
 * 
 * \image html PlanarizerSTD.png
 * 
 * \section improvementSW Areas For Improvement in Software
 * 
 * The initial target for the data polling rate was 200Hz. However, due to time constraints we left it at 60Hz. Some notable issues limiting the polling rate include the speed of 
 * vanilla FreeRTOS and the level of corruption in the data sent over bluetooth. The amount of corruption is a function of how quickly we are reading and writing data, and we are operating
 * near the limits of vanilla FreeRTOS. While the hardware and EncoderDriver are certainly able to handle 200Hz, we can not send the data quickly enough. 
 * 
 * Some solutions we believe may work include working in the STM32CubeMX environment, moving away from FreeRTOS, and reconfiguring FreeRTOS. 
 * 
 * Using STM32CubeMX gives the user greater access to manipulating the Nucleo's hardware. The code can then be made more specific to using the Nucleo, and make better use of the
 * on board hardware. This should result in significant speed increases, though the environment can be a little tricky to work with.
 * 
 * This code was built with FreeRTOS to allow for greater modularity. If this is unnecessary, and we need more speed, FreeRTOS may not be the best route.
 * 
 * This code uses vanilla FreeRTOS. However, FreeRTOS can be forked from https://github.com/stm32duino/STM32FreeRTOS.git to be modified and sped up. 
 * 
 * \section improvementHW Areas For Improvement in Hardware
 * 
 * In terms of hardware, the Arduino Shield is functional but does not have the best form factor. Additionally, the solution to maintaining a high enough load on the power bank to keep
 * it from powering off when the Nucleo goes to sleep is power inefficient.
 * 
 * The Arduino Shield is a solder breadboard, and is accordingly much larger than it needs to be. Creating a custom PCB may be desirable to reduce the overall form factor.It may also reduce
 * concerns of breaking the bluetooth module, as the breakout board used sticks out a lot.
 * 
 * Powerbanks automatically stop supplying power when they do not detect high enough loads, and Nucleos go to sleep periodically, dropping their load to zero. To work around this
 * five 100 ohm resistors, rated for 1/4 W, have been attached in parallel to draw out 250mA. This is far more than necessary and results in significant heat dissipation. This brings up concern over
 * injury, though minor, from touching the resistors. Luckily, the power bank we use has more than enough capacity to run for a couple hours during testing, so if the hot resistors are acceptable
 * then no change is necessary. However, if desired, a different circuit may be used that periodically draws power to keep the powerbank "awake." A quick internet search will yield 
 * examples of such circuits. 
 * 
 * 
 */
