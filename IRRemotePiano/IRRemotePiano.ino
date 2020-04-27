
// Cole Delong
// Infared Remote Control
// 4-25-20


// IR REMOTE CODES
//    ------------------
//  /                    \
// |  1
// | FFA25D FF629D FFE21D |
// |                      |
// |  3     2
// | FF22DD FF02FD FFC23D |
// |                      |
// |   5       4        |
// | FFE01F FFA857 FF906F |
// |                      |
// |   6
// | FF6897 FF9867 FFB04F |
// |                      |
// |   8       7        |
// | FF30CF FF18E7 FF7A85 |
// |                      |
// |   10       9       |
// | FF10EF FF38C7 FF5AA5 |
// |                      |
// |   12       11       |
// | FF42BD FF4AB5 FF52AD |
// |                      |
// |         Car          |
// |         mp3          |
//  \                    /
//    ------------------
// (FFFFFFFF for repeat when a button is held)

// DISPLAY KEY
//      A
//     ---
//  F |   | B
//    | G |
//     ---
//  E |   | C
//    |   |
//     ---
//      D

// includes
#include <IRremote.h>

// Global Variables
const int recievePin = 12;
const int buzzerPin = 13;
const int switchPin = 0;
const int joyX = A5;
const int pinA = 1;
const int pinB = 2;
const int pinC = 3;
const int pinD = 4;
const int pinE = 5;
const int pinF = 6;
const int pinG = 7;
const int D1 = 8;
const int D2 = 9;
const int D3 = 10;
const int D4 = 11;
IRrecv rec(recievePin);
decode_results results;
double lastPing = millis() / 1000.0;
int octave = 4;
int note;
bool play = false;
float freq;

void setup() {
  // initialize the pins
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinE, OUTPUT);
  pinMode(pinF, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(recievePin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(switchPin, INPUT);
  rec.enableIRIn();
}

void loop() {

  // if a button is currently being pressed
  if (rec.decode(&results)) {

    // reset the last ping
    lastPing = millis() / 1000.0;

    // change the note or octave depending on the button pressed
    switch (results.value) {
      // CHANGE NOTE
      case 0xFFA25D:
        note = 1;
        play = true;
        break;
      case 0xFF02FD:
        note = 2;
        play = true;
        break;
      case 0xFF22DD:
        note = 3;
        play = true;
        break;
      case 0xFFA857:
        note = 4;
        play = true;
        break;
      case 0xFFE01F:
        note = 5;
        play = true;
        break;
      case 0xFF6897:
        note = 6;
        break;
      case 0xFF18E7:
        note = 7;
        play = true;
        break;
      case 0xFF30CF:
        note = 8;
        play = true;
        break;
      case 0xFF38C7:
        note = 9;
        play = true;
        break;
      case 0xFF10EF:
        note = 10;
        play = true;
        break;
      case 0xFF4AB5:
        note = 11;
        play = true;
        break;
      case 0xFF42BD:
        note = 12;
        play = true;
        break;
      // CHANGE OCTAVE
      case 0xFFE21D:
        octave = 1;
        play = false;
        break;
      case 0xFFC23D:
        octave = 2;
        play = false;
        break;
      case 0xFF906F:
        octave = 3;
        play = false;
        break;
      case 0xFFB04F:
        octave = 4;
        play = false;
        break;
      case 0xFF7A85:
        octave = 5;
        play = false;
        break;
      case 0xFF5AA5:
        octave = 6;
        play = false;
        break;
      case 0xFF52AD:
        octave = 7;
        play = false;
        break;
      // NO USE
      case 0xFF629D:
        play = false;
        break;
      case 0xFF9867:
        play = false;
        break;
    }

    // play the decided note if a note was pressed
    if (play) playNote(note, octave);
    rec.resume();

    // if a button hasn't been pressed in 109 millis, don't play a sound and don't display
  } else {
    if (millis() / 1000.0 - lastPing > 0.110) {
      noTone(buzzerPin);

      digitalWrite(D1, HIGH);
      digitalWrite(D2, HIGH);
      digitalWrite(D3, HIGH);
      digitalWrite(D4, HIGH);
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, HIGH);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      delay(10);

    } else {
      // display the frequency on the display
      if (play) displayNumber(freq);
    }
  }
}

// play a note based off of the note and octave. Detune based off of joystick y val up and down a half step. Vibrado based off switch.
void playNote(int note, int octave) {
  //                   C      C#/Db    D      D#/Eb    E       F      F#/Gb    G      G#/Ab    A      A#/Bb     B
  float notes[12] = {130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.00, 196.00, 207.65, 220.00, 233.08, 246.94};
  freq = notes[note - 1] / pow(2, 4 - octave);
  int joy = 496 - analogRead(joyX);
  float offFreq;
  if (digitalRead(switchPin)) offFreq = map(495 - analogRead(joyX), -495, 528, freq / pow(1.0595, 2), freq * pow(1.0595, 2));
  else {
    if (joy <= 0) offFreq = map(joy, -495, 0, freq/pow(1.0595, 2), 0); 
    else offFreq = map(joy, 0, 528, 0, freq * pow(1.0595, 2));
  }
  freq += offFreq;
  if (!digitalRead(switchPin)) freq *= 2;
  tone(buzzerPin, freq);

}

// display a number on the screen
void displayNumber(int num) {

  // determine all digits needed
  int thousands = num / 1000;
  int hundreds = num / 100 - (10 * thousands);
  int tens = (num / 10) - (10 * hundreds) - (100 * thousands);
  int ones = num - (10 * tens) - (100 * hundreds) - (1000 * thousands);

  // define each number in terms of HIGH's and LOW's on the display
  bool nums[10][7]  = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1} // 9
  };

  // FIRST DIGIT
  digitalWrite(D1, HIGH);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  if (thousands != 0) {
    if (!nums[thousands][0]) digitalWrite(pinA, HIGH); else digitalWrite(pinA, LOW);
    if (!nums[thousands][1]) digitalWrite(pinB, HIGH); else digitalWrite(pinB, LOW);
    if (!nums[thousands][2]) digitalWrite(pinC, HIGH); else digitalWrite(pinC, LOW);
    if (!nums[thousands][3]) digitalWrite(pinD, HIGH); else digitalWrite(pinD, LOW);
    if (!nums[thousands][4]) digitalWrite(pinE, HIGH); else digitalWrite(pinE, LOW);
    if (!nums[thousands][5]) digitalWrite(pinF, HIGH); else digitalWrite(pinF, LOW);
    if (!nums[thousands][6]) digitalWrite(pinG, HIGH); else digitalWrite(pinG, LOW);
  } else {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
  }
  delay(2);

  // SECOND DIGIT
  digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  if (hundreds != 0 || thousands != 0) {
    if (!nums[hundreds][0]) digitalWrite(pinA, HIGH); else digitalWrite(pinA, LOW);
    if (!nums[hundreds][1]) digitalWrite(pinB, HIGH); else digitalWrite(pinB, LOW);
    if (!nums[hundreds][2]) digitalWrite(pinC, HIGH); else digitalWrite(pinC, LOW);
    if (!nums[hundreds][3]) digitalWrite(pinD, HIGH); else digitalWrite(pinD, LOW);
    if (!nums[hundreds][4]) digitalWrite(pinE, HIGH); else digitalWrite(pinE, LOW);
    if (!nums[hundreds][5]) digitalWrite(pinF, HIGH); else digitalWrite(pinF, LOW);
    if (!nums[hundreds][6]) digitalWrite(pinG, HIGH); else digitalWrite(pinG, LOW);
  } else {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
  }
  delay(2);

  // THIRD DIGIT
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, LOW);
  if (!nums[tens][0]) digitalWrite(pinA, HIGH); else digitalWrite(pinA, LOW);
  if (!nums[tens][1]) digitalWrite(pinB, HIGH); else digitalWrite(pinB, LOW);
  if (!nums[tens][2]) digitalWrite(pinC, HIGH); else digitalWrite(pinC, LOW);
  if (!nums[tens][3]) digitalWrite(pinD, HIGH); else digitalWrite(pinD, LOW);
  if (!nums[tens][4]) digitalWrite(pinE, HIGH); else digitalWrite(pinE, LOW);
  if (!nums[tens][5]) digitalWrite(pinF, HIGH); else digitalWrite(pinF, LOW);
  if (!nums[tens][6]) digitalWrite(pinG, HIGH); else digitalWrite(pinG, LOW);
  delay(2);

  // FOURTH DIGIT
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, HIGH);
  if (!nums[ones][0]) digitalWrite(pinA, HIGH); else digitalWrite(pinA, LOW);
  if (!nums[ones][1]) digitalWrite(pinB, HIGH); else digitalWrite(pinB, LOW);
  if (!nums[ones][2]) digitalWrite(pinC, HIGH); else digitalWrite(pinC, LOW);
  if (!nums[ones][3]) digitalWrite(pinD, HIGH); else digitalWrite(pinD, LOW);
  if (!nums[ones][4]) digitalWrite(pinE, HIGH); else digitalWrite(pinE, LOW);
  if (!nums[ones][5]) digitalWrite(pinF, HIGH); else digitalWrite(pinF, LOW);
  if (!nums[ones][6]) digitalWrite(pinG, HIGH); else digitalWrite(pinG, LOW);
  delay(2);

}
