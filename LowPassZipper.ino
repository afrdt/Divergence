// Low-pass filter that uses a zipper slider instead of a potentiometer

int speaker = 12; // speaker connected to digital pin 12 (on Flora, D12 and A11 are the same pin)

void setup() {
 pinMode(speaker, OUTPUT); // initialize speaker pin as an output
 pinMode(A11, INPUT_PULLUP); // use the internal pullup resistor
}

void loop() {
  tone(speaker, 880); // play a square wave of 880 Hz
}
