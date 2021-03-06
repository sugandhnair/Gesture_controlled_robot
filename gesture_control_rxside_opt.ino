#include <SPI.h>
#include <Enrf24.h>
#include <nRF24L01.h>
#include <string.h>

Enrf24 radio(P2_0, P2_1, P2_2);
const uint8_t rxaddr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01 };

const char *hold = "H";
const char *left = "L";
const char *right ="R";
const char *front ="F";
const char *back ="B";

void dump_radio_status_to_serialport(uint8_t);

void setup()
{
  Serial.begin(9600);
  Serial.println("Start...");
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(5,OUTPUT);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  
  radio.begin();  // Defaults 1Mbps, channel 0, max TX power
  dump_radio_status_to_serialport(radio.radioState());

  radio.setRXaddress((void*)rxaddr);
  
  radio.enableRX();  // Start listening
}

void loop() 
{
  char inbuf[33];
  
  //dump_radio_status_to_serialport(radio.radioState());  // Should show Receive Mode

  while (!radio.available(true))
    ;
  if (radio.read(inbuf)) {
    Serial.print("Received packet: ");
    Serial.println(inbuf);

    if (!strcmp(inbuf, hold))
    {
      Serial.println("In hold..");
      digitalWrite(11,LOW);
      digitalWrite(12,LOW);
      digitalWrite(13,LOW);
      digitalWrite(5,LOW);
    }
    if (!strcmp(inbuf, front))
    {
      Serial.println("Moving forward");
      digitalWrite(11,LOW);
      digitalWrite(12,HIGH);
      digitalWrite(13,LOW);
      digitalWrite(5,HIGH);
    }
     if (!strcmp(inbuf, back))
    {
      Serial.println("Moving backward");
      digitalWrite(11,HIGH);
      digitalWrite(12,LOW);
      digitalWrite(13,HIGH);
      digitalWrite(5,LOW);
    }
     if (!strcmp(inbuf, left))
    {
      Serial.println("Moving left");
      digitalWrite(11,LOW);
      digitalWrite(12,LOW);
      digitalWrite(13,LOW);
      digitalWrite(5,HIGH);
    }
     if (!strcmp(inbuf, right))
    {
      Serial.println("Moving right");
      digitalWrite(11,LOW);
      digitalWrite(12,HIGH);
      digitalWrite(13,LOW);
      digitalWrite(5,LOW);
    }
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
