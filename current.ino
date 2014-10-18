#include <math.h>
#include <ctime>
#define SELPIN  A3 //CS
#define DATAOUT A2 //MOSI
#define DATAIN  A1 //MISO
#define SPICLOCK  A0 //Clock

double squared_voltage;
double sum_squared_voltage;
double inst_voltage;

double mean_square_voltage;
double root_mean_square_voltage;

int read_value;
int current_iterations;
unsigned long start_time;
unsigned long finish_time;
//int current_vals[150];

void setup(){
 //set pin modes
 pinMode(SELPIN, OUTPUT);
 pinMode(DATAOUT, OUTPUT);
 pinMode(DATAIN, INPUT);
 pinMode(SPICLOCK, OUTPUT);

 //disable device to start with
 digitalWrite(SELPIN,HIGH);
 digitalWrite(DATAOUT,LOW);
 digitalWrite(SPICLOCK,LOW);

 Serial.begin(9600);
}

int read_adc(int channel){
  int adcvalue = 0;
  // byte commandbits = 0b11000000; //command bits - start, mode, chn (3), dont care (3)
  byte commandbits = 0xc0;

  //allow channel selection
  commandbits|=((channel-1)<<3);

  digitalWrite(SELPIN,LOW); //Select adc

  // setup bits to be written
  for (int i=7; i>=3; i--){
    digitalWrite(DATAOUT,commandbits&1<<i);
    //cycle clock
    digitalWrite(SPICLOCK,HIGH);
    digitalWrite(SPICLOCK,LOW);
  }

  digitalWrite(SPICLOCK,HIGH);    //ignores 2 null bits
  digitalWrite(SPICLOCK,LOW);
  digitalWrite(SPICLOCK,HIGH);
  digitalWrite(SPICLOCK,LOW);

    //read bits from adc
  for (int i=11; i>=0; i--){
    adcvalue+=digitalRead(DATAIN)<<i;
    //cycle clock
    digitalWrite(SPICLOCK,HIGH);
    digitalWrite(SPICLOCK,LOW);
  }
  digitalWrite(SELPIN, HIGH); //turn off device
  return adcvalue;
}

void loop() {
  squared_voltage = 0;
  sum_squared_voltage = 0;
  inst_voltage = 0;

  start_time = millis();

  current_iterations = 5000;

  for (int i=current_iterations; i>=0; i--){
    // inst_voltage calculation from raw ADC input goes here.
    read_value = read_adc(1);
    inst_voltage = (double)(read_value - 2051) / 27.3;

    squared_voltage = inst_voltage * inst_voltage;
    sum_squared_voltage += squared_voltage;
//    current_vals[i] = read_value;
  }

//  for (int i=149; i>=0; i--){
//    Serial.println(current_vals[i]);
//  }

  Serial.println("Total time:");
  Serial.println(millis() - start_time);

  mean_square_voltage = sum_squared_voltage / current_iterations;
  root_mean_square_voltage = sqrt(mean_square_voltage);

  Serial.println("Last Read Value:");
  Serial.println(read_value);

  Serial.println("RMS:");
  Serial.println(root_mean_square_voltage);
  Serial.println(" ");

  delay(250);
}
