# Projectile Motion Simulator

## 👥 Authors
**Krzysztof Gąsior**, **Karol Świderski**

## 🚀 Short description
The program allows you to simulate the projectile motion of an object and lets you modify the following physical parameters:
- initial velocity of the object
- initial height of the object
- initial launch angle
- acceleration of gravity

![pms-exec](https://github.com/user-attachments/assets/f61886e5-079f-4361-92e5-76d02e0f6ad3)



## 🧭 How It Works

![testparams](https://github.com/user-attachments/assets/781c1d8c-202c-4a6a-ac21-e57553ce8a4d)

At the top of the program window, there are eight text fields. To enter a value into one of them, click on it with the mouse. 
The border will highlight in yellow, indicating that the field is active, and you can then type in a numerical value.
The following variables affect the parameters of the projectile motion:
- launch angle [deg] (range: <-90; 90>)
- initial height [m] (range: <0; 1 000 000>)
- acceleration [m/s^2] (range: <0.01; 1 000>)
- initial velocity [m/s] (range: <0.01; 299 792 457.99>, the max value's the speed of light minus 0.01 m/s)

Decimal values are accepted with an accuracy of up to the second decimal place. Entering a value with higher precision results in rounding to the hundredth place.
To the left of the "Acceleration" text field, there is a drop-down list where you can select the gravitational value found on selected celestial bodies in the Solar System.

![table](https://github.com/user-attachments/assets/1b8002d8-3a2f-41e7-aaac-24f167010b1d)



The fields "Time multiplier" and "Sim. at given time [s]" are values that modify how the simulation behaves. You can speed it up, slow it down, 
or pause it at a specific point in time. The multiplier range goes from 1:10 000 000 000 to 1 000 000 000.
The value range for the simulation time point goes from 0 s to 100 000 000 000 s.

The remaining two text fields are related to exporting data to a file with the .csv extension, where:
- t – time
- V – resultant velocity of the object
- Vx – horizontal velocity component
- Vy – vertical velocity component
- h – height of the object
- d – traveled distance
- alpha – angle between the resultant velocity vector and the ground plane

Intervals define the number of measurements of physical values at evenly spaced time steps. The minimum number of such samples is 2, and the maximum is 10 000.
Precision determines the numeric accuracy of the recorded values. The available range is ⟨–1; 99⟩, where –1 means unlimited precision (limited only by the memory allocated 
for the variables).
Next to the text fields are two buttons: "Export to csv [E]" and "Screenshot [I]".
The first button creates a file named "measurements.csv", saving the data using the specified numbers of time samples and precision.

![measurements](https://github.com/user-attachments/assets/3c7d2fbf-fbcf-4331-b010-b25db65ff90a)

The second button captures a screenshot of the program window and saves it to the "screenshots" directory, naming the file with the current date, time, and a unique numeric 
string (microseconds).

![screenshot](https://github.com/user-attachments/assets/c8b4a41b-0a30-464a-b09b-902ac37eb3a0)

Sometimes, exporting data may take over a minute. For this reason, the export functionality is temporarily disabled during the process, which is indicated by 
the button color changing from white to red.

![redbutton](https://github.com/user-attachments/assets/3b30759a-0d80-4313-8134-e576a542ca20)

Both features can also be triggered by pressing the E and I keys on the keyboard, respectively.



![checkboxes-interface](https://github.com/user-attachments/assets/1be0c9d5-e6c2-4a30-9aa7-21d201e67c29)

On the right side of the top segment, there are checkboxes:
- Resultant vector – toggles the visibility of the resultant velocity vector
- X-vector – toggles the visibility of the horizontal velocity component
- Y-vector – toggles the visibility of the vertical velocity component
- Curve – shows or hides the trajectory path
- Axis guides – toggles visibility of the axis reference lines
- Unequal axes' scaling – switches between symmetric and asymmetric axis scaling. This is useful when the difference between maximum height and range is very large.



The middle part of the window shows a 2D plane with two axes. Depending on the physical parameters, both the axes and the vector arrows rescale dynamically 
so that the simulation remains clearly visible at all times.
![input-field-change](https://github.com/user-attachments/assets/5f0e62ef-122e-43a0-9b29-c286b34e1829)



At the bottom-left segment of the window, there are buttons that control the simulation: "Start [S]", "Reset [R]", and "Pause [P]". These do not need to be clicked manually, 
as the corresponding keyboard shortcuts can be used instead.

![no-color-rectangle](https://github.com/user-attachments/assets/c904f51b-a29e-4ee6-a4f0-1ba5e6edab8f)

![color-rectangle](https://github.com/user-attachments/assets/ab5075a6-4a0c-4e70-a97c-3aa84c118663)

Start begins the simulation. Reset restores the object to its initial state. Pause pauses the simulation, and pressing it again resumes it.



Next to these buttons are displayed the object's physical parameters.
Note that Vx is constant and does not change during flight. Vy0 is the initial vertical velocity and is also constant.
In addition, the "Distance" section displays both the current traveled distance and the total range. The "Time" section shows the elapsed time in relation to the total flight time 
(which is the sum of the ascent and descent durations).
![time-change](https://github.com/user-attachments/assets/994fccb8-f735-4f8d-82cc-4b68f11aa665)
