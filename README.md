# Planarizer-DAQ

Our Senior Project Group, consisting of Aaron Tran, Eric Ramos, Matthew Wimberley, and Tim Jain, was tasked with designing and manufacturing a test stand for the Cal Poly Legged Robot Team. The team needed a way to constrain a single robot leg to 2DOF in the sagittal plane. Hence, our test stand "planarizes" the robot leg. With the additional need of requiring that the robot leg be able to reach a steady state gait, we determined that a boom system would work best, though it only approximates the sagittal plane. This DAQ is responsible for measuring the corresponding rotational value of the Planarizer, allowing for us to track the position of the robot leg during testing.

The DAQ consists of two STM32L476RG Nucleos, synchronized using two HiLetgo HC-05 bluetooth transceivers. The DAQ makes use of two US Digital E6 Optical Kit Encoders to measure the yaw and pitch of the encoders. These values are then processed, plotted, and saved in MATLAB. 

How to use:

Mount the Pitch MCU on the DAQ mount, which attaches to the yoke. Place the Yaw MCU somewhere neat the yaw encoder. It does not have a specified mounting position. Refer to the wiring diagram in the images folder for
making proper electrical connections. All wires provided have been labeled according to what they need to be plugged into. Then, connect the Yaw MCU to a nearby PC using the 10ft USB A to Mini B cable provided. Download the files in the MATLAB Frontend folder, and make sure that the MATLAB Support for MinGW-w64C/C++ Compiler add on has been installed. From there, follow the instructions available in 
PlanarizerDAQFrontEnd.mlx to operate the DAQ.


Refer to the Senior Project report and project documentation for more information.

See project documentation here:
https://malkstik.github.io/Planarizer-DAQ/

See Senior Project report here:

