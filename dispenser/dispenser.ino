#include <Servo.h>
#include <RTClib.h>
#include <Wire.h>
#include <LowPower.h>   // LowPowerLab

// Pin definitions
const byte MILLPIN = 9;
const byte LEDPIN = 4;
const int FOTRESPIN = A0;
#define CLOCK_INTERRUPT_PIN 2   // the pin that is connected to SQW

// Servo defines
Servo mill;
#define MILLMOVE 2000
#define MILLSTOP 1500
#define LIGHTUPTHR 550      // amount of light of direct led light for encoder
#define LIGHTDOWNTHR 300      // amount of light of no light from encoder LED

// RTC setup
RTC_DS3231 rtc;
volatile bool alarm = false;  // Interrupt signaling byte
#define ALARMHOUR 13          // hour of the day to set alarm (24 format)
#define ALARMMIN 0            // minute of the alarm set
#define ALARMSEC 0            // second of the alarm set

// Foto-encoder variables
int fotoresVal = 0;

void alarmCallback() {
  alarm = true;
}

// Set up the alarm cleanly
void alarmSetup() {
  Serial.println("Start to schedule new alarm");
  // 1- get the time of the alarm for today or tomorrow
  DateTime now = rtc.now();
  DateTime alarm = new DateTime(now.year(), now.month(), now.day(), ALARMHOUR, ALARMMIN, ALARMSEC);

  // in case is later than alarm hour for today we schedule for tomorrow
  if(now < alarm){    
    alarm = DateTime(now.year(), now.month(), now.day()+1, ALARMHOUR, ALARMMIN, ALARMSEC);
    Serial.print("Alarm scheduled for tomorrow"); Serial.println();
  }
  // // Codigo debug
  // DateTime now = rtc.now();
  // DateTime alarm = rtc.now() + TimeSpan(0,0,1,0);

  // // in case is later than alarm hour for today we schedule for tomorrow
  // if(now < alarm){    
  //   alarm = alarm  + TimeSpan(0, 24, 0, 0);
  //   Serial.print("Alarm scheduled for tomorrow");
  // }

  // 2- We set the alarm on the system
  if(!rtc.setAlarm1(alarm, DS3231_A1_Date)){    // A1_Date to fire alarm only when the full date is the same
    Serial.println("Error, alarm wasn't set");
  } else {
    Serial.print("Now is: "); Serial.println(now.timestamp());
    Serial.print("Alarm scheduled for: "); 
    Serial.println(alarm.timestamp());
  }
}

// Manage the mill to make one step
void millStep() {
  // 1- Prepare the encoder
  digitalWrite(LEDPIN, HIGH);
  delay(100);

  // 1- Get away from actual led
  while((LIGHTDOWNTHR < analogRead(FOTRESPIN))){
    Serial.print("FASE1 - Light threshold: "); Serial.println(analogRead(FOTRESPIN));
    mill.writeMicroseconds(MILLMOVE);
    delay(20);
    mill.writeMicroseconds(MILLSTOP);
    delay(30);
  }

  Serial.print("FASE1 -> FASE2: "); Serial.println(analogRead(FOTRESPIN));

  // 2- while we can't see the led again we move
  while((LIGHTUPTHR > analogRead(FOTRESPIN))){
    Serial.print("FASE2 - Moving: "); Serial.println(analogRead(FOTRESPIN));
    mill.writeMicroseconds(MILLMOVE);
    delay(20);
    mill.writeMicroseconds(MILLSTOP);
    delay(30);
  }

  // 3- once reached we end the turn
  mill.writeMicroseconds(MILLSTOP);
  Serial.print("FASE3 - Ready to next step: "); Serial.println(analogRead(FOTRESPIN));
  digitalWrite(LEDPIN, LOW);
  Serial.println("Position reached");
}

void setup() {
  // set up basics
  delay(1000);
  Serial.begin(9600);  // Serial communication
  Wire.begin();        // I2C communication

  Serial.flush();
  Serial.println("//////////////////////////////////////////////////////////////////////////////////////\n");

  // Start RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1) delay(10);
  }
  // if RTC lost power on compilation, adjust hour
  if (rtc.lostPower()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  //we don't need the 32K Pin, so disable it
  rtc.disable32K();

  // Alarm setup
  // Setup pin for interruption
  pinMode(CLOCK_INTERRUPT_PIN, INPUT_PULLUP);                  // Pull-up resistance
  attachInterrupt(digitalPinToInterrupt(CLOCK_INTERRUPT_PIN), alarmCallback, FALLING);    // RTC3231 falls on alarm

  // Clean RTC for new program
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
  rtc.writeSqwPinMode(DS3231_OFF);    // top oscillating signals at SQW Pin otherwise setAlarm1 will fail
  rtc.disableAlarm(2);                // we dont use alarm2
  alarmSetup();
  
  

  // Define servo as mill
  mill.attach(MILLPIN);
  mill.writeMicroseconds(MILLSTOP);  // stop mill

  // Run a step to check the funcionality
  millStep();

  // Define LED
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
  Serial.println("Setup terminado");
  delay(500);
}

void loop() {
  // wihtout an alarm we sleep
  if(!alarm) {
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    return;
  } else {
    alarm = false;
    Serial.println("Alarm fired!");
    // 1- execute step
    millStep();

    // 2- reset alarm
    rtc.clearAlarm(1);

    // 3- set new alarm
    alarmSetup();
    digitalWrite(LEDPIN, LOW);
    delay(300);
  }
}
