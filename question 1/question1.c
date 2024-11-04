/******************************************************************************

Author: Paulo Ricardo Pabst
Date:	03/11/2024
Question 1

*******************************************************************************/


/*	Due to transportation delay (also known as time lag or dead time) of some 
	plants, which can cause phase shifts that limit the bandwidth of the control 
	system and can lead to closed-loop instabilities.

	This happens due to delays in sensors and actuators. For example, temperature 
	control system may have time lag due the time necessary to the object being 
	heated (or cooled) have a large thermal capacity. Even the sensor have a mass 
	which requires some time to achieve the same temperature of its surrounding.
	
	In these scenarios is good to have trapezoidal set-point (SP) changes, which 
    allows the control loop to better track the changes in the SP. Sometimes is 
	even better to have an S-curve profile. S-curve profile is very common in 
	speed control plants, which suffer from high inertia.

	The trapezoidal change in SP can be designed in different ways. One option is 
	to compute the new PWM after the previous cycle ends. This can be time-
	consuming for very low performance MCU, but isn't a challenge to the moderns
	MCU. Another option is to pre-compute the whole ramp (or the partial ramp) 
	and use the DMA to transfer new duty cycles to the timer.
	
	Inductive loads require way more care than resistive loads. Usually because 
	inductive loads, especially BLDC motors, have so little inductance when 
	compared to induction motors, they can reach very high currents rapidly.
	
	So, when driving inductive loads using PWM is very important to measure the 
	actual current. Most modern microcontroller, like STM32, have the ability 
	to trigger an ADC conversion on the ending of the PWM switching. In this way 
	is possible to measure the peak current.
	
	Additionally, PEC (protective electronics circuits) can also be used to 
	disable switching when current is too high. But these circuits require an 
	additional immunity tests.
*/