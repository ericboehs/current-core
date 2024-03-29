// #define SELPIN 10 //Selection Pin
#define DATAOUT A2 //MOSI
#define DATAIN  A1 //MISO
#define SPICLOCK  A0 //Clock
int readvalue;

void setup(){
 //set pin modes
 // pinMode(SELPIN, OUTPUT);
 pinMode(DATAOUT, OUTPUT);
 pinMode(DATAIN, INPUT);
 pinMode(SPICLOCK, OUTPUT);
 //disable device to start with
/*  digitalWrite(SELPIN,HIGH); */
 digitalWrite(DATAOUT,LOW);
 digitalWrite(SPICLOCK,LOW);

 Serial.begin(9600);
}

int read_adc(int channel){
  int adcvalue = 0;
  byte commandbits = B11000000; //command bits - start, mode, chn (3), dont care (3)

  //allow channel selection
  commandbits|=((channel-1)<<3);

  // digitalWrite(SELPIN,LOW); //Select adc
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
  // digitalWrite(SELPIN, HIGH); //turn off device
  return adcvalue;
}


void loop() {
 readvalue = read_adc(1);
 Serial.println(readvalue,DEC);
 readvalue = read_adc(2);
 Serial.println(readvalue,DEC);
 Serial.println(" ");
 delay(250);
}
