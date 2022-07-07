# AY2122_II_Project-1

## Pulseoximeter
The project involves the development of a portable pulseoximeter made using CY8CKIT-059 Development Kit by Cypress/Infineon and the MAX30101, a High-Sensitivity Pulse Oximeter and Heart-Rate Sensor for Wearable Health. Exploiting the circular buffer present inside the sensor, it is possible to extract the values coming from Infrared and Red LEDs and feed them inside a peak detection algorithm for PPG analysis, which outputs the SpO2 and Heart Rate values. Such function has been forked by the [MAX30101_Sensor_Library](https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library/blob/master/src/spo2_algorithm.cpp) of the manufacturer Maxim Integrated; the function has been modified to our specific case. 
The project also involved the PCB design and production through press-n-print technique, the 3D case design and printing and the overall assembly of the final case.
## GUI
![GUI_screenshot](https://user-images.githubusercontent.com/92868195/177736449-8a3d3b33-4399-40a8-bbf1-9a03c5391d3b.jpg)
On the GUI it is also possible to make instant changes to the registers, which directly influence the MAX30101, such as the ADC range, the LED pulse width of the two LEDs, the LEDs pulse amplitude and the Sample Rate. These values will change the Full Scale Range of the readings, as well as the SNR (signal-to-noise-ratio).
## Final assembled device
The final device resembles a commercial pulseoximeter with a rudimental hinge that secures the finger in place over the photodiode, so that a constant pressure is applied.
![photo_2022-07-07_11-20-44](https://user-images.githubusercontent.com/92868195/177739470-68d3b52b-e111-4c24-bd1d-c62aca32fa81.jpg)

