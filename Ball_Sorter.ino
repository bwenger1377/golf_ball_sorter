#include <Wire.h>
#include <Servo.h> //Include the servo library
#include "Adafruit_TCS34725.h" //Include the color sensor library
#define commonAnode true //Set to false if using a common cathode LED

// Initialize all three servos
Servo servo1;
Servo servo2;
Servo servo3;

int ballcount = 0; // Initialize a variable to count how many balls have been sorted.

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  // Declare what pin the servo is connected to (attach) and put it at an initial angle of 0.
  servo1.attach(3);
  servo2.attach(5);
  servo3.attach(6);
  servo1.write(25);
  servo2.write(15);
  servo3.write(10);
  
  Serial.begin(9600); // Connect to the serial window for display purposes.
  
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // Halt if sensor not found.
  }
}

void loop() {
  uint16_t red, green, blue, clear;
  
  // SENSE: Read the sensor
  tcs.setInterrupt(false);  // turn on LED
  delay(60);  // 50ms to read + 10ms buffer
  tcs.getRawData(&red, &green, &blue, &clear);  // Use getRawData instead of getRGB.
  tcs.setInterrupt(true);  // turn off LED
  
  // DISPLAY: Print the RGB values to the Serial Monitor
  Serial.print("R:\t"); Serial.print(red); //Prints the red value. 
  Serial.print("\tG:\t"); Serial.print(green); //Prints the green value.
  Serial.print("\tB:\t"); Serial.print(blue); //Prints the blue value.
  Serial.println("\n"); //Newline

  // SORT: Use color to decide which servo to move
  if ((red * 0.75) > green) {
    Serial.println("Pink");
    servo2.write(50); // Open servo2
    // OPEN: Release one ball
    servo1.write(60);  // Open servo1
    delay(230);  // Delay before closing
    servo1.write(25);  // Close servo1
    ballcount = ballcount + 1;
    Serial.print("Balls Sorted: ");
    Serial.println(ballcount);  // Print the ball count
    delay(1000); //Give the ball time to fall in
    servo2.write(15); // Close servo2
  } else if (blue > (red * 0.5)) {
    Serial.println("White");
    servo3.write(50); //Open servo3
    // OPEN: Release one ball
    servo1.write(60);  // Open servo1
    delay(230);  // Delay before closing
    servo1.write(25);  // Close servo1
    ballcount = ballcount + 1;
    Serial.print("Balls Sorted: ");
    Serial.println(ballcount);  // Print the ball count
    delay(1000); //Give the ball time to fall in
    servo3.write(10); // Close servo3 
  } else if (red < 200) {
    Serial.println("No ball");
  } else {
    Serial.println("Yellow");
    // OPEN: Release one ball
    servo1.write(60);  // Open servo1
    delay(230);  // Delay before closing
    servo1.write(25);  // Close servo1
    ballcount = ballcount + 1;
    Serial.print("Balls Sorted: ");
    Serial.println(ballcount);  // Print the ball count
    servo2.write(10); // Close servo2
    servo3.write(10); // Close servo3
  }

  // DELAY: Delay before next reading
  delay(2000);
}
