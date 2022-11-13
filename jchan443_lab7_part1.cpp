//test later
#include <LiquidCrystal.h>
#include <DHT.h>
#define DHTPIN A0     
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
//output
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int blue = 8;
int yellow = 9;
int white = 10;
int RED = 11;

//input
int JS_X = A1;
int JS_Y = A2;
int JS_Button = 13;
int DHC = A0;

//global variables for logic
int humidity = 0;
int temp = 0;
int setTemp = 75;
int hum = 0;
int setHum = 30;
int off = 0;
int i = 0;
typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);

} task;

int delay_gcd;
const unsigned short tasksNum = 8;
task tasks[tasksNum];
enum Off_States { OFF_INIT, S0};

//turns machine on off;
int Off_Tick(int state1) {
  switch (state1) { // State transitions
    case OFF_INIT:
        state1 = S0;
        break;
    case S0:
        if(digitalRead(JS_Button) == 1){
            if(off == 0){    //glorified !off
                off = 1;
            }
            else{
                off = 0;
            }
        }
        break;   
  }
  switch (state1) { // State Action
    case S0:
        break;
  }
  return state1;
}

//this part is in charge of deciding whether the joysick is pressed up or down. 
enum UP_States { UP_INIT, O0, S1};
int UP_Tick(int state2) {
  switch (state2) { // State transitions
    case UP_INIT:
        state2 = O0;
        break;
    case O0:
        if(off == 1){
            state2 = S1;
        }
        else{
            state2 = O0;
        }
        break;
    case S1:
        int yval = analogRead(JS_Y);
        if(off==0){
            state2 = O0;
        }
        else if(yval < 500){
            humidity = 1;
        }
        else if(yval > 600){
            humidity = 0;
        }
        state2 = S1;
        break;
  }
  switch (state2) { // State Action
    case O0:
        break;
    case S1:
        break;
  }
  return state2;
}


//does the setTemperature joystick logic
enum ST_States { ST_INIT, O1, N1, Base1, Wait11, Wait12};
int ST_Tick(int state3) {
  switch (state3) { // State transitions
    case ST_INIT:
        state3 = O1;
        break;
    case O1:
        if(off == 1){
            state3 = N1;
        }
        else{
            state3 = O1;
        }
        break;
    case N1:
        if(off==0){
            state3 = O1;
        }
        else if(humidity == 0){
            state3 = Base1;
        }
        else{
            state3 = N1;
        }
        break;
    case Base1:
        int xval = analogRead(JS_X);
        if(off == 0){
            state3 = O1;
        }
        else if(humidity == 1){
            state3 = N1;
        }
        else if(xval >600){
            setTemp +=1;
            state3 = Wait11;
        }
        else if(xval < 500){
            setTemp = setTemp -1;
            state3 = Wait12;
        }
        break;
    case Wait11:
        int xval = analogRead(JS_X);
        if(xval <600){
            state3 = Base1;
        }
        else{
            state3 = Wait11;
        }
        break;
    case Wait12:
        int xval = analogRead(JS_X);
        if(xval > 500){
            state3 = Base1;
        }
        else{
            state3 = Wait12;
        }
        break;

  }
  switch (state3) { // State Action
    case O1:
        break;
    case N1:
        break;
    case Base1:
        break;
    case Wait11:
        break;
    case Wait12:
        break;
  }
  return state3;
}


//does the joycon setHumidity portion
enum SH_States { SH_INIT, O2, N2, Base2, Wait21, Wait22};
int SH_Tick(int state4) {
  switch (state4) { // State transitions
    case SH_INIT:
        state4 = O2;
        break;
    case O2:
        if(off == 1){
            state4 = N2;
        }
        else{
            state4 = O2;
        }
        break;
    case N2:
        if(off == 0){
            state4 = O2;
        }
        else if(humidity == 1){
            state4 = Base2;
        }
        else{
            state4 = N2;
        }
        break;
    case Base2:
        int xvals = analogRead(JS_X);
        if(off == 0){
            state4 = O2;
        }
        else if(humidity == 0){
            state4 = N2;
        }
        else if(xvals >600){
            setTemp +=1;
            state4 = Wait21;
        }
        else if(xvals < 500){
            setTemp = setTemp -1;
            state4 = Wait22;
        }
        break;
    case Wait21:
        int xvals = analogRead(JS_X);
        if(xvals <600){
            state4 = Base2;
        }
        else{
            state4 = Wait21;
        }
        break;
    case Wait22:
        int xvals = analogRead(JS_X);
        if(xvals > 500){
            state4 = Base2;
        }
        else{
            state4 = Wait22;
        }
        break;

  }
  switch (state4) { // State Action
    case O2:
        break;
    case N2:
        break;
    case Base2:
        break;
    case Wait21:
        break;
    case Wait22:
        break;
  }
  return state4;
}

//gets reading from dhc sensor(temperature)
enum DHCT_States { DHCT_INIT, O3, DT};
int DHCT_Tick(int state5) {
  switch (state5) { // State transitions
    case DHCT_INIT:
        state5 = O3;
        break;
    case O3:
        if(off == 1){
            state5 = DT;
        }
        else{
            state5 = O3;
        }
        break;
    case DT:
        if(off == 0){
            state5 = O3;
        }
        else{
            state5 = DT;
        }
        temp = dht.readTemperature(true);
        break;
  }
  switch (state5) { // State Action
    case O3:
        break;
    case DT:
        break;
  }
  return state5;
}

//gets reading from dhc sensor(humidity)
enum DHCH_States { DHCH_INIT, O4, DH};
int DHCH_Tick(int state6) {
  switch (state6) { // State transitions
    case DHCH_INIT:
        state6 = O4;
        break;
    case O4:
        if(off == 1){
            state6 = DH;
        }
        else{
            state6 = O4;
        }
        break;
    case DH:
        if(off == 0)[
            state6 = O4;
        ]
        else{
            state6 = DH;
        }
        hum = dht.readHumidity();
        break;
  }
  switch (state6) { // State Action
    case O4:
        break;
    case DH:
        break;
  }
  return state6;
}


//handles temperature output;
enum TOut_States { TO_INIT, O5, N3, hON, hOFF, cON, cOFF};
int TOut_Tick(int state7) {
  switch (state7) { // State transitions
    case TO_INIT:
        state7 = O5;
        break;
    case O5:
        if(off == 1){
            state7 = N3;
        }
        else{
            state7 = O5;
        }
        break;
    case N3:
        if(off == 0){
            state7 = O5;
        }
        else if(setTemp > temp){
            state7 = hON;
            i = 0;
        }
        else if(setTemp < temp){
            state7 = cON;
            i = 0;
        }
        else{
            state7 = N3;
        }
        break;
    case hON:
        if(off == 0){
            state7 = O5;
            i =0;
        }
        else if(setTemp <= temp){
            state7 = N3;
            i = 0;
        }
        else if(i < 19){
            state7 = hON;
        }
        else if(i > 19){
            state7 = hOFF;
            i = 0;
        }
        break;
    case hOFF:
        if(off == 0){
            state7 = O5;
            i = 0;
        }
        else if(setTemp <= temp){
            state7 = N3;
            i = 0;
        }
        else if(i < 1){
            state7 = hOFF;
        }
        else if(i > 1){
            state7 = hON;
            i = 0;
        }
        break;
    case cON:
        if(off == 0){
            state7 = O5;
            i = 0;
        }
        else if(setTemp >= temp){
            state7 = N3;
            i = 0;
        }
        else if(i < 36){
            state7 = cON;
        }
        else if(i > 36){
            state7 = cOFF;
            i = 0;
        }
        break;
    case cOFF:
        if(off == 0){
            state7 = O5;
            i = 0;
        }
        else if(setTemp >= temp){
            state7 = N3;
            i = 0;
        }
        else if(i < 24){
            state7 = cOFF;
        }
        else if(i > 24){
            state7 = cON;
            i = 0;
        }
        break;
  }
  switch (state7) { // State Action
    case O5:
        break;
    case N3:
        digitalWrite(white, LOW);
        digitalWrite(RED, LOW);
        break;
    case hON:
        i++;
        digitalWrite(RED, HIGH);
        break;
    case hOFF:
        i++;
        digitalWrite(RED, LOW);
        break;
    case cON:
        i++;
        digitalWrite(white, HIGH);
        break;
    case cOFF:
        i++;
        digialWrite(white, LOW);
        break;
  }
  return state7;
}

//in charge of the how the humidity leds work. 
enum HOut_States { HO_INIT, O6, N4, huON, huOFF, dON, dOFF};
int HOut_Tick(int state8) {
    switch (state8) { // State transitions
    case TO_INIT:
        state8 = O6;
        break;
    case O6:
        if(off == 1){
            state8 = N4;
        }
        else{
            state8 = O6;
        }
        break;
    case N4:
        if(off == 0){
            state8 = O6;
        }
        else if(setHum > hum){
            state8 = huON;
            i = 0;
        }
        else if(setHum < hum){
            state8 = dON;
            i = 0;
        }
        else{
            state8 = N4;
        }
        break;
    case huON:
        if(off == 0){
            state8 = O6;
            i = 0;
        }
        else if(setHum <= hum){
            state8 = N4;
            i = 0;
        }
        else if(i < 4){
            state8 = huON;
        }
        else if(i > 4){
            state8 = huOFF;
            i = 0;
        }
        break;
    case huOFF:
        if(off == 0){
            state8 = O6;
            i = 0;
        }
        else if(setHum <= hum){
            state8 = N4;
            i = 0;
        }
        else if(i < 16){
            state8 = huOFF;
        }
        else if(i > 16){
            state8 = huON;
            i = 0;
        }
        break;
    case dON:
        if(off == 0){
            state8 = O6;
            i = 0;
        }
        else if(setHum >= hum){
            state8 = N4;
            i = 0;
        }
        else if(i < 10){
            state8 = dON;
        }
        else if(i > 10){
            state8 = dOFF;
            i = 0;
        }
        break;
    case dOFF:
        if(off == 0){
            state8 = O6;
            i = 0;
        }
        else if(setHum >= hum){
            state8 = N4;
            i = 0;
        }
        else if(i < 30){
            state8 = dOFF;
        }
        else if(i > 30){
            state8 = dON;
            i = 0;
        }
        break;
  }
  switch (state8) { // State Action
    case O6:
        break;
    case N4:
        digitalWrite(blue, LOW);
        digitalWrite(yellow, LOW);
        break;
    case huON:
        i++;
        digitalWrite(blue, HIGH);
        break;
    case huOFF:
        i++;
        digitalWrite(blue, LOW);
        break;
    case dON:
        i++;
        digitalWrite(yellow, HIGH);
        break;
    case dOFF:
        i++;
        digialWrite(yellow, LOW);
        break;
  }
  return state8; 
}


//does the lcd crap
enum LS_States {LS_INIT, O7, S2};
int LS_Tick(int state9){
    switch (state9){
        case LS_INIT:
            state9 = O7;
            break;
        case O7:
            if(off == 1){
                state9 = S2;
            }
            else{
                state9 = O7;
            }
            break;
        case S2:
            if(off == 0){
                state9 = O7;
            }
            state9 = S2;
            break;
    }
    switch (state9){
        case O7:
            lcd.noDisplay();
        case S2:
            lcd.clear();
            lcd.setCursor(0,0);
            string l1 = "SetTemp:" + setTemp + "*F SetHumidity:" + setHum + "%";
            lcd.print(l1);
            string l2 = "CurTemp:" + temp + "*F CurHumidity:" + hum + "%";
            lcd.setCursor(0,1);
            lcd.print(l2);
            break;
    }
    return state9
}

void setup() {
  // put your setup code here, to run once:
  
  //setup outputs;
  lcd.begin(28,2);
  pinMode(blue, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(white, OUTPUT);
  pinMode(RED, OUTPUT);

  //setup inputs;
  pinMode(JS_Button, INPUT);
  //set up state machine
  unsigned char i = 0;
  
  tasks[i].state = OFF_INIT;
  tasks[i].period = 10;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &Off_Tick;
  i++;
  tasks[i].state = UP_INIT;
  tasks[i].period = 300;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &UP_Tick;
  i++;
  tasks[i].state = ST_INIT;
  tasks[i].period = 300;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &ST_Tick;
  i++;
  tasks[i].state = SH_INIT;
  tasks[i].period = 300;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SH_Tick;
  i++;
  tasks[i].state = DHCT_INIT;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &DHCT_Tick;
  i++;
  tasks[i].state = DHCH_INIT;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &DHCH_Tick;
  i++;
  tasks[i].state = TO_INIT;
  tasks[i].period = 5;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TOut_Tick;
  i++;
  tasks[i].state = HO_INIT;
  tasks[i].period = 10;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &HOut_Tick;
  i++;
  tasks[i].state = LS_INIT;
  tasks[i].period = 150;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &LS_Tick;

  delay_gcd = 5; // GCD
  Serial.begin(9600);
}

void loop() {
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = millis(); // Last time this task was ran
    }
  }
}
