#include <MD_MAX72xx.h> 
#include <NewPing.h> 

#define LUMINOSITY A5 
#define DEVICES_NUMBER 2 
#define PIN_DATA 11  
#define PIN_CS 10  
#define PIN_CLK 13  
#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

MD_MAX72XX matrix = MD_MAX72XX(HARDWARE_TYPE, PIN_CS, DEVICES_NUMBER);

#define maximumDistance 3000 // the max distance
#define  trigger1 2  // Ultrasonic Sensor 1
#define  echo1 3  // Ultrasonic Sensor 1
#define  trigger2 4  // Ultrasonic Sensor 2
#define  echo2 5  // Ultrasonic Sensor 2

NewPing leftEye(trigger1, echo1, maximumDistance); 
NewPing rightEye(trigger2, echo2, maximumDistance); 

float light = 0;
long time1, time2;
int distance1;
int distance2;

//Drawing of eyes
uint8_t straight[COL_SIZE] = {
  0b00111100,
  0b01000010,
  0b10011001,
  0b10101101,
  0b10111101,
  0b10011001,
  0b01000010,
  0b00111100 };

uint8_t right[COL_SIZE] = {
  0b00111100,
  0b01000010,
  0b10110001,
  0b11011001,
  0b11111001,
  0b10110001,
  0b01000010,
  0b00111100 };

uint8_t left[COL_SIZE] = {
  0b00111100,
  0b01000010,
  0b10001101,
  0b10010111,
  0b10011111,
  0b10001101,
  0b01000010,
  0b00111100 };

uint8_t fullBlink[COL_SIZE] = {
  0b00000000,
  0b00111100,
  0b01111110,
  0b11111111,
  0b11111111,
  0b10111101,
  0b01000010,
  0b00111100 };

uint8_t squint[COL_SIZE] = {
  0b00000000,
  0b00111100,
  0b01011010,
  0b10101101,
  0b10111101,
  0b01111110,
  0b00000000,
  0b00000000 };

uint8_t heart[COL_SIZE] = {
  0b00000000,
  0b00110110,
  0b01001001,
  0b01000001,
  0b00100010,
  0b00010100,
  0b00001000,
  0b00000000 };


void setup() {
  Serial.begin(115200);
  matrix.begin();
  pinMode( trigger1, OUTPUT );
  pinMode( echo1, INPUT );
  pinMode( trigger2, OUTPUT );
  pinMode( echo2, INPUT );
  digitalWrite( trigger1, LOW );
  digitalWrite( trigger2, LOW );
  pinMode(LUMINOSITY, INPUT );
  //start looking straight
  displayStraight();
}

void loop() {
  light = analogRead(LUMINOSITY);
  light = (light/255) * MAX_INTENSITY;
  matrix.control(MD_MAX72XX::INTENSITY, light);

  distance1 = leftEye.ping_median(5);
  delay(500);
  distance2 = rightEye.ping_median(5);

  if ( light < 2 ){
      displayFullBlink();
    delay(250);
    return;
  }

  if ( light > 20  ){
      displaySquint();
    delay(250);
    return;
  }

  //calculates the distance between sensors
  float difference = ( distance2 - distance1 );
  if ((difference  < 8 &&  difference  > -8) || (distance1 == 0 && distance2 == 0 )) {
    displayStraight();
  }
  // look right  
  else if ( difference  > 8) {
    displayRight();
  }
  // look left  
  else if ( difference  < -8 ) {
    displayLeft();
  }

  if((distance1 < 1000 || distance2 < 1000) && (distance1 > -1 || distance2 > -1) ){
    displayHeart();
  }
  
  delay(250);

  if (true) {
    Serial.print("Light Sensor: ");
    Serial.print(light);
    Serial.println("");
    Serial.print("Distance1 / Distance2: ");
    Serial.print(distance1);
    Serial.print("/");
    Serial.print(distance2);
    Serial.println("");
    Serial.print("Distance2 - Distance1: ");
    Serial.print(distance2-distance1);
    Serial.println("");
    Serial.println("");
    }
}



//MATRICES LED 
void clearMatrix(){
  matrix.clear();
  matrix.control(MD_MAX72XX::INTENSITY, light ); 
}

void displaySquint(){
  clearMatrix();
  for (uint8_t iRow = 0; iRow < ROW_SIZE; iRow++){
    matrix.setColumn(iRow, squint[iRow]); // Affiche Matrice LED 1
    matrix.setColumn(iRow + 8, squint[iRow]); // AfficheMatrice LED 2
  }
}

void displayFullBlink() {
  clearMatrix();
  for (uint8_t iRow = 0; iRow < ROW_SIZE; iRow++) {
    matrix.setColumn(iRow, fullBlink[iRow]); // Affiche Matrice LED 1
    matrix.setColumn(iRow + 8, fullBlink[iRow]); // AfficheMatrice LED 2
  }
}


void displayRight(){
  clearMatrix();
  for (uint8_t iRow=0; iRow < ROW_SIZE; iRow++) {
    matrix.setColumn(iRow, right[iRow]); // Affiche Matrice LED 1
    matrix.setColumn(iRow + 8, right[iRow]); // AfficheMatrice LED 2
  }
}

void displayLeft(){
  clearMatrix();
  for (uint8_t iRow = 0; iRow < ROW_SIZE; iRow++) {
    matrix.setColumn(iRow, left[iRow]); // Affiche Matrice LED 1
    matrix.setColumn(iRow + 8, left[iRow]); // AfficheMatrice LED 2
  }
}


void displayStraight(){
  clearMatrix();
  for (uint8_t iRow=0; iRow<ROW_SIZE; iRow++) {
    matrix.setColumn(iRow, straight[iRow]); // Affiche Matrice LED 1
    matrix.setColumn(iRow+8, straight[iRow]); // AfficheMatrice LED 2
  }
}

void displayHeart() {
  clearMatrix();
  for (uint8_t iRow=0; iRow<ROW_SIZE; iRow++) {
    matrix.setColumn(iRow, heart[iRow]); // Affiche Matrice LED 1
    matrix.setColumn(iRow+8, heart[iRow]); // AfficheMatrice LED 2
  }
}
