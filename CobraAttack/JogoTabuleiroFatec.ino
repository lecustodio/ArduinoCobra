/* TabuleiroFatec))) Sensor
  
  Modificade and Developer By Leandro Custódio
  E-mail: lgckks@gmail.com

   The circuit:
	* +V connection of the PING))) attached to +5V
	* GND connection of the PING))) attached to ground
	* SIG connection of the PING))) attached to digital pin 7
  * TRIG connection of the PING))) attached to return sonic sensor pin 8
  * Servo of object moviment))) attached to pin 9

 */

#include <Servo.h> 
Servo myservo;                  // create servo object to control a servo
int val;                        // range-mapped value for servo control

const int pingPin = 7;          // Parallax ping sensor's sugnal pin
const int trigPin = 8;          // Parallax ping sensor triger pin
const int numReadings = 5;      // set higher to smooth more, also causes more latency
int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int lastValue = 0;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  
  //clear the smoothing value buffer
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0; 
}

void loop()
{
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;
  int fadeValue = 0;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(trigPin, INPUT);
  duration = pulseIn(trigPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  
  //smoothing code starts here
  // subtract the last reading:
  total= total - readings[index];         
  // read from the sensor:  
  readings[index] = cm;//analogRead(inputPin); 
  // add the reading to the total:
  total= total + readings[index];       
  // advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings)              
    // ...wrap around to the beginning: 
    index = 0;                           

  // calculate the average:
  average = total / numReadings;
  //smoothing code ends here  
  
  //remap value range and move the servo
  val = average;
  val = map(val, 10, 40, 0, 179);     // scale value to use it with the Tower Pro half turn analog servo (value between 0 and 180) 
  if(average < 25)
      {myservo.write(val);}     // sets the servo position according to the scaled value if within a certain distance
  delay(10);  //let the servo cool down, or something
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}