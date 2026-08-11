// This is my code for a solar panel that tracks the sun
// It uses 2 axis to control both the pitch (up and down) and the azimuth (horizontal angular distance) of the solar panel

#include <Servo.h> // Importing the required servo library needed to control the servos

Servo azimuthServo; // Creating the azimuth servo
Servo tiltServo; // Creating the tilt servo

int topLDRPin = 0; // Analogue pin for the LDR that goes on the top
int rightLDRPin = 1; // Analogue pin for the LDR that goes on the right
int bottomLDRPin = 2; // Analogue pin for the LDR that goes on the bottom
int leftLDRPin = 3; // Analogue pin for the LDR that goes on the left
int staticSolarPin = 4; // Analogue pin for the static solar panel
int movingSolarPin = 5; // Analogue pin for the moving solar panel

int azimuthServoPin = 13; // Digital pin for the servo that controls the azimuth
int tiltServoPin = 12; // Digital pin for the servo that controls the tilt

int azimuthServoPos = 90; // Initialising the starting position of the azimuth servo
int tiltServoPos = 90; // Initialising the starting position of the tilt servo

int totalTopLDR = 0; // Initialising variables needed for calculations
int totalBottomLDR = 0; // Initialising variables needed for calculations
int totalLeftLDR = 0; // Initialising variables needed for calculations
int totalRightLDR = 0; // Initialising variables needed for calculations
int totalStaticSolar = 0; // Initialising variables needed for calculations
int totalMovingSolar = 0; // Initialising variables needed for calculations
int averageTopLDR = 0; // Initialising variables needed for calculations
int averageBottomLDR = 0; // Initialising variables needed for calculations
int averageLeftLDR = 0; // Initialising variables needed for calculations
int averageRightLDR = 0; // Initialising variables needed for calculations
float averageStaticSolar = 0; // Initialising variables needed for calculations
float averageMovingSolar = 0; // Initialising variables needed for calculations

int numberOfReadings = 10; // The total number of reading you want to calculate a mean out of. More = more time. Less = less accurate

unsigned long interval = 300000; // The total delay between consecutive solar panel readings in milliseconds
unsigned long lastLogTime = 0; // Initialising the time variable

int allowance = 1; // The degree of light intensity allowed between LDRs before the solar panel moves. Smaller amount if using a bright light
int angle = 1; // The degrees the solar panel moves by if it decides to move

float panelVoltage = 6.0;



void setup()
{
  Serial.begin(9600); // Start the serial connection
  azimuthServo.attach(azimuthServoPin); // Attach the azimuth servo to the azimuth digital pin
  tiltServo.attach(tiltServoPin); // Attach the tilt servo to the tilt digital pin
}

void loop()
{
  unsigned long currentTime = millis(); // Record the time from initiation
  azimuthCorrection(); // Run the azimuth correction algorithm
  tiltCorrection(); // Run the tilt correction algorithm
  //solarPanels(); // Run the solar panel reading algorithm
  //delay(10); // Delay to smooth things out
  Serial.print("Average Top:");
  Serial.print(averageTopLDR);
  Serial.print(",");

  Serial.print("Average Bottom:");
  Serial.print(averageBottomLDR);
  Serial.print(",");

  Serial.print("Average Left:");
  Serial.print(averageLeftLDR);
  Serial.print(",");

  Serial.print("Average Right:");
  Serial.print(averageRightLDR);
  Serial.print(",");

  Serial.print("Average Static:");
  Serial.print(averageStaticSolar);
  Serial.print(",");

  Serial.print("Average Moving:");
  Serial.print(averageMovingSolar);
  Serial.print(",");

  Serial.print("Azimuth Pos:");
  Serial.print(azimuthServoPos);
  Serial.print(",");

  Serial.print("Tilt Pos:");
  Serial.print(tiltServoPos);
  Serial.print(",");

  Serial.print("Time (ms):");
  Serial.println(currentTime);
}

void tiltCorrection()
{
  for (int i = 0; i < numberOfReadings; i ++) // For loop that calculates the average
  {
    int topLDRReading = analogRead(topLDRPin); // Read the analog reading of the top LDR
  	int bottomLDRReading = analogRead(bottomLDRPin); // Read the analog reading of the bottom LDR
  	totalTopLDR = totalTopLDR + topLDRReading; // Calculate a total of readings for the top LDR
    totalBottomLDR = totalBottomLDR + bottomLDRReading; // Calculate a total of readings for the botom LDR
    delay(10); // Delay to smooth things out / Space the readings out
  }
  
  averageTopLDR = (totalTopLDR / numberOfReadings); // Calculates the average of the top LDR
  averageBottomLDR = (totalBottomLDR / numberOfReadings); // Calculates the average of the bottom LDR
  
  totalTopLDR = 0; // Resets the top LDR total to 0 for correct calculation of average next time
  totalBottomLDR = 0; // Resets the bottom LDR total to 0 for correct calculation of average next time
  
  if ((averageTopLDR < (averageBottomLDR - allowance)) or (averageTopLDR > (averageBottomLDR + allowance))) // Sees if the average of the top LDR is within the allowance range of the bottom LDR
  {
    if (averageTopLDR > averageBottomLDR)
    {
      if ((tiltServoPos - angle) < 0) // If the angle goes into the negatives
      {
        tiltServoPos = 0;
      }
      else
      {
        tiltServoPos = tiltServoPos - angle; // Decrease the postition of the tilt servo
        tiltServo.write(tiltServoPos); // Write the servo to the new angle
      }
    }
    if (averageTopLDR < averageBottomLDR)
    {
      if ((tiltServoPos + angle) > 180) // If the angle goes above 180
      {
        tiltServoPos = 180;
      }
      else
      {
        tiltServoPos = tiltServoPos + angle; // Increase the position of the tilt servo
        tiltServo.write(tiltServoPos); // Write the servo to the new angle
      }
    }
  }
}

void azimuthCorrection()
{
  for (int i = 0; i < numberOfReadings; i ++) // For loop that calculates the average
  {
    int leftLDRReading = analogRead(leftLDRPin); // Read the analog reading of the left LDR
  	int rightLDRReading = analogRead(rightLDRPin); // Read the analog reading of the right LDR
  	totalLeftLDR = totalLeftLDR + leftLDRReading; // Calculate a total of readings for the left LDR
    totalRightLDR = totalRightLDR + rightLDRReading; // Calculate a total of readings for the right LDR
    delay(10); // Delay to smooth things out / Space the readings out
  }
  
  averageLeftLDR = (totalLeftLDR / numberOfReadings); // Calculates the average of the left LDR
  averageRightLDR = (totalRightLDR / numberOfReadings); // Calculates the average of the right LDR
  
  totalLeftLDR = 0; // Resets the left LDR total to 0 for correct calculation of average next time
  totalRightLDR = 0; // Resets the right LDR total to 0 for correct calculation of average next time
  
  if ((averageLeftLDR < (averageRightLDR - allowance)) or (averageLeftLDR > (averageRightLDR + allowance))) // Sees if the average of the left LDR is within the allowance range of the right LDR
  {
    if (averageLeftLDR < averageRightLDR)
    {
      if ((azimuthServoPos - angle) < 0) // If the angle goes into the negatives
      {
        azimuthServoPos = 0;
      }
      else
      {
        azimuthServoPos = azimuthServoPos - angle; // Decrease the postition of the azimuth servo
        azimuthServo.write(azimuthServoPos); // Write the servo to the new angle
      }
    }
    if (averageLeftLDR > averageRightLDR)
    {
      if ((azimuthServoPos + angle) > 180) // If the angle goes above 180
      {
        azimuthServoPos = 180;
      }
      else
      {
        azimuthServoPos = azimuthServoPos + angle; // Increase the postition of the azimuth servo
        azimuthServo.write(azimuthServoPos); // Write the servo to the new angle
      }
    }
  }
}

void solarPanels()
{
  unsigned long currentTime = millis(); // Record the time from initiation
  if (currentTime - lastLogTime >= interval) // If the time from last recording is greater than the interval
  {
    lastLogTime = currentTime; // Save the time
    for (int i = 0; i < numberOfReadings; i ++) // For loop that calculates the average
  {
    totalStaticSolar = totalStaticSolar + analogRead(staticSolarPin); // Calculate a total of readings for the static solar panel
    totalMovingSolar = totalMovingSolar + analogRead(movingSolarPin); // Calculate a total of readings for the moving solar panel
    delay(10); // Delay to smooth things out / Space the readings out
  }
  averageStaticSolar = 10 * ((totalStaticSolar / (float)numberOfReadings) * panelVoltage * 2) / 1023.0; // Calculates the average of the static solar panel in Volts // The readings came out 10x less than actual, so I have compensated
  averageMovingSolar = 10 * ((totalMovingSolar / (float)numberOfReadings) * panelVoltage * 2) / 1023.0; // Calculates the average of the static solar panel in Volts // The readings came out 10x less than actual, so I have compensated

  totalStaticSolar = 0; // Resets the static solar panel total to 0 for correct calculation of average next time
  totalMovingSolar = 0; // Resets the moving solar panel total to 0 for correct calculation of average next time
  }
}