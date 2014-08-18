/* Divergence: A wearable EMF detector with haptic and sonic feedback
 * By Afroditi Psarra http://afroditipsarra.com
 *
 * July 2014
 *
 * Circuit:
 * Adafruit Flora
 * 2 handmade copper coils connected on analog pins A7 and A9 and grounded with 3.6 M Ohms resistors 
 * 1 vibration motor on digital pin 10 (PWM pin)
 * 1 mini jack - soundOut connected to digital pin 12 via a low-pass filter with a 47uF 
 * and a zipper slider < 100 K Ohms (the zipper uses the uC internal pullup resistor
 */

#define soundOut 12 // sound output connected to digital pin 12
#define vibe 10 // vibration motor located on the chest connected to digital pin 3
#define coilRight A7 // coil antenna located on the right wrist connected to analog pin A7
#define coilLeft A9 // coil antenna located on the left wrist connected to analog pin A9

#define NUMREADINGSRIGHT 15 // raise this number to increase data smoothing
#define NUMREADINGSLEFT 15 // raise this number to increase data smoothing

int senseLimitRight = 125; // raise this number to decrease sensitivity on the right probe(up to 1023 max)
int senseLimitLeft = 125; // raise this number to decrease sensitivity on the left probe(up to 1023 max)

int probeRight; // probe readings from the right coil
int probeLeft; // probe readings from the left coil

// variables for smoothing
int readingsRight[NUMREADINGSRIGHT]; // the readings from the analog input A7
int indexRight = 0; // the index of the current reading from the right coil
int totalRight = 0; // the running total pf the right coil
int averageRight = 0; // final average of the probe reading from the right coil

int readingsLeft[NUMREADINGSLEFT]; // the readings from the analog input A9
int indexLeft = 0; // the index of the current reading from the left coil
int totalLeft = 0; // the running total of the left coil
int averageLeft = 0; // final average of the probe reading from the left coil

void setup() {
  //Serial.begin(9600); // initialize the serial communication (for debugging-calibrating only)
  pinMode(soundOut, OUTPUT); // initialize the mini jack/speaker as an output
  pinMode(A11, INPUT_PULLUP); // use the internal pullup resistor on the same pin as above
  pinMode(vibe, OUTPUT); // initialize the mini jack/speaker as an output
}

void loop() {
  coilProbeRight();
  coilProbeLeft();

//  Serial.print(averageRight);
//  Serial.print(" ");
//  Serial.println(averageLeft);
  delay(1);
}

void coilProbeRight() {
  probeRight = analogRead(coilRight);

  if(probeRight >= 1){ // if the reading isn't zero, proceed

    probeRight = constrain(probeRight, 1, senseLimitRight); // turn any reading higher than the senseLimit value into the senseLimit value
    probeRight = map(probeRight, 1, senseLimitRight, 1, 1023); // remap the constrained value within a 1 to 1023 range

    totalRight -= readingsRight[indexRight]; // subtract the last reading
    readingsRight[indexRight] = probeRight; // read from the sensor
    totalRight += readingsRight[indexRight]; // add the reading to the total
    indexRight = (indexRight + 1); // advance to the next index

    if (indexRight >= NUMREADINGSRIGHT) // if we're at the end of the array...
      indexRight = 0; // ...wrap around to the beginning

    averageRight = totalRight / NUMREADINGSRIGHT; // calculate the average
  }
  
  int vibeIntensity = map(averageRight, 32, 768, 0, 255); // map the average to pwm values
  analogWrite(vibe, vibeIntensity); //  make the motor vibrate depending on the average

  int freq = map(averageRight, 32, 768, 880, 60);
  int dur = map(freq, 880, 60, 10, 1000);
  tone(soundOut, freq, dur);
}

void coilProbeLeft() {
  probeLeft = analogRead(coilLeft); 

  if(probeLeft >= 1){ // if the reading isn't zero, proceed

    probeLeft = constrain(probeLeft, 1, senseLimitLeft); // turn any reading higher than the senseLimit value into the senseLimit value
    probeLeft = map(probeLeft, 1, senseLimitLeft, 1, 1023); // remap the constrained value within a 1 to 1023 range

    totalLeft -= readingsLeft[indexLeft]; // subtract the last reading
    readingsLeft[indexLeft] = probeLeft; // read from the sensor
    totalLeft += readingsLeft[indexLeft]; // add the reading to the total
    indexLeft = (indexLeft + 1); // advance to the next index

    if (indexLeft >= NUMREADINGSLEFT) // if we're at the end of the array...
      indexLeft = 0; // ...wrap around to the beginning

    averageLeft = totalLeft / NUMREADINGSLEFT; // calculate the average
  }

  int vibeIntensity = map(averageLeft, 0, 1023, 0, 255); // map the average to pwm values
  analogWrite(vibe, vibeIntensity); // make the motor vibrate depending on the average

  int freq = map(averageLeft, 0, 1023, 880, 60);
  int dur = map(freq, 880, 60, 10, 1000);
  tone(soundOut, freq, dur);
}



