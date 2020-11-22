#include <SPI.h>
#include <Enrf24.h>
#include <nRF24L01.h>
#include <string.h>
#define xval 2
#define yval 6
float x=0;
float y=0;
float value1=0;
float value2=0;
float xref=2.5;
float yref=2.5;
float xdiff=0;
float ydiff=0;

Enrf24 radio(P2_0, P2_1, P2_2);  // P2.0=CE, P2.1=CSN, P2.2=IRQ
const uint8_t txaddr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01 };

const char *hold = "H";
const char *left = "L";
const char *right ="R";
const char *front ="F";
const char *back ="B";

void dump_radio_status_to_serialport(uint8_t);

void setup()
{
  Serial.begin(9600);
  pinMode(xval,INPUT);
  pinMode(yval,INPUT);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);

  radio.begin();  // Defaults 1Mbps, channel 0, max TX power
  //dump_radio_status_to_serialport(radio.radioState());

  radio.setTXaddress((void*)txaddr);
}

void loop() 
{
  value1=analogRead(xval);
  value2=analogRead(yval);
  x=(value1*5)/1023;
  y=(value2*5)/1023;
  xdifference();
  ydifference();
  Serial.print("X value : ");
  Serial.println(x);
  Serial.print("Y value : ");
  Serial.println(y);
  compare();
}

void xdifference()
{
  if(x>xref)
  {
    xdiff=x-xref;
    Serial.print("X difference : ");
    Serial.println(xdiff);
  }
  else
  {
    xdiff=xref-x;
    Serial.print("X difference : ");
    Serial.println(xdiff);
  }
}
void ydifference()
{
  if(y>yref)
  {
    ydiff=y-yref;
    Serial.print("Y difference : ");
    Serial.println(ydiff);
  }
  else
  {
    ydiff=yref-y;
    Serial.print("Y difference : ");
    Serial.println(ydiff);
  }
}
void compare()
{
  if(xdiff>0.20)
  {
    if(x>xref)
    {
      Serial.println("Moving Forward");
      Serial.print("Moving front..");
      Serial.println(front);
      radio.print(front);
      radio.flush();
    //dump_radio_status_to_serialport(radio.radioState());
    }
    else if(x<xref)
    {
      Serial.println("Moving Backward");
      Serial.println(back);
      radio.print(back);
      radio.flush();
    //dump_radio_status_to_serialport(radio.radioState());
    }
  }
  else if(ydiff>0.20)
  {
    if(y>yref)
    {
      Serial.println("Moving Right");
      Serial.println(right);
      radio.print(right);
      radio.flush();
    //dump_radio_status_to_serialport(radio.radioState());
    }
    else if(y<yref)
    {
      Serial.println("Moving Left");
      Serial.println(left);
      radio.print(left);
      radio.flush();
    //dump_radio_status_to_serialport(radio.radioState());
    }
  }
  else
  {
    Serial.println("Hold..");
    Serial.println(hold);
    radio.print(hold);
    radio.flush();
    //dump_radio_status_to_serialport(radio.radioState());
  }
}

void dump_radio_status_to_serialport(uint8_t status)
{
  Serial.print("Enrf24 radio transceiver status: ");
  switch (status) {
    case ENRF24_STATE_NOTPRESENT:
      Serial.println("NO TRANSCEIVER PRESENT");
      break;

    case ENRF24_STATE_DEEPSLEEP:
      Serial.println("DEEP SLEEP <1uA power consumption");
      break;

    case ENRF24_STATE_IDLE:
      Serial.println("IDLE module powered up w/ oscillators running");
      break;

    case ENRF24_STATE_PTX:
      Serial.println("Actively Transmitting");
      break;

    case ENRF24_STATE_PRX:
      Serial.println("Receive Mode");
      break;

    default:
      Serial.println("UNKNOWN STATUS CODE");
  }
}
