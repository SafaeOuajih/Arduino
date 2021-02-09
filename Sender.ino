/* By Ouajih Safae */
#include <stdio.h>
#include <SPI.h>
#include "LoRa.h"
#include <RFID.h>
#include <avr/dtostrf.h>
#define WITH_APPKEY

#define SS_PIN 6
#define RST_PIN 7
int counter = 0;
int k = 1;

RFID rfid(SS_PIN, RST_PIN); 

// Setup variables:
    int serNum0;
    int serNum1;
    int serNum2;
    int serNum3;
    int serNum4;

unsigned int freq = 865200000;//HZ
unsigned int idlePeriodInMin = 1; // in minute
unsigned int nCycle = idlePeriodInMin*60;
unsigned short id_frame = 0;



/*sensor*/
const unsigned int number_of_sensors = 2;
uint16_t Temperature_id     = 432;
uint16_t luminosite_id      = 433;
uint16_t id_node            = 209;
uint16_t id_sensor[number_of_sensors]={Temperature_id,luminosite_id};
unsigned long value[number_of_sensors];// stock donnée 


#ifdef WITH_APPKEY
uint8_t my_appKey[4]={5, 6, 7, 8};
#endif



char message[100];


char *ftoa(char *a, double f, int precision)
{
 long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
 
 char *ret = a;
 long heiltal = (long)f;
 itoa(heiltal, a, 10);
 while (*a != '\0') a++;
 *a++ = '.';
 long desimal = abs((long)((f - heiltal) * p[precision]));
 itoa(desimal, a, 10);
 return ret;
}

void send_data(){
   
  uint8_t app_key_offset = 0;
  int e;
  #ifdef WITH_APPKEY
      app_key_offset = sizeof(my_appKey);
      memcpy(message,my_appKey,app_key_offset);
  #endif
      uint8_t r_size;
      char final_str[100] = "\\";
      char aux[20] = "";
      char id[1] = "";
      sprintf(final_str, "%s!%i!%hd", final_str,id_node, id_frame++);
      for (int i=0; i<number_of_sensors; i++) {
              ftoa(aux,value[i], 2);
              sprintf(final_str, "%s#%d/%s", final_str, id_sensor[i], aux);
              strcpy(aux,"");
              
      }

      r_size=sprintf(message+app_key_offset,final_str);

      Serial.println(message);
   
      LoRa.beginPacket();
      LoRa.print(message);
      LoRa.endPacket(); 

      for(uint8_t j=0;j<number_of_sensors;j++){
        value[j]=0;
      }
}
void setup(){

	
	Serial.begin(38400);
  delay(5000);
  Serial.println("Set LoRa modulation\r\n");
  SPI.begin(); 
  rfid.init();
  if (!LoRa.begin(freq)) {
    Serial.println("Starting LoRa failed!");
    while (1); 
  }

  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.setPreambleLength(8);
  LoRa.enableCrc();
  LoRa.setTxPower(20, PA_OUTPUT_PA_BOOST_PIN);



}


void loop()
{
  
    if (rfid.isCard()) {
        if (rfid.readCardSerial()) {
         /* if (rfid.serNum[0] != serNum0
                || rfid.serNum[1] != serNum1
                || rfid.serNum[2] != serNum2
                || rfid.serNum[3] != serNum3
                || rfid.serNum[4] != serNum4
            ) {*/
                /* With a new cardnumber, show it. */
                Serial.println(" ");
                Serial.println("Card found");
                serNum0 = rfid.serNum[0];
                serNum1 = rfid.serNum[1];
                serNum2 = rfid.serNum[2];
                serNum3 = rfid.serNum[3];
                serNum4 = rfid.serNum[4];
               
                //Serial.println(" ");
                Serial.println("Cardnumber:");
                Serial.print("Dec: ");
    Serial.print(rfid.serNum[0],DEC);
                Serial.print(", ");
    Serial.print(rfid.serNum[1],DEC);
                Serial.print(", ");
    Serial.print(rfid.serNum[2],DEC);
                Serial.print(", ");
    Serial.print(rfid.serNum[3],DEC);
                Serial.print(", ");
    Serial.print(rfid.serNum[4],DEC);
                Serial.println(" ");
                        
                Serial.print("Hex: ");
    Serial.print(rfid.serNum[0],HEX);
                Serial.print(", ");
    Serial.print(rfid.serNum[1],HEX);
                Serial.print(", ");
    Serial.print(rfid.serNum[2],HEX);
                Serial.print(", ");
    Serial.print(rfid.serNum[3],HEX);
                Serial.print(", ");
    Serial.print(rfid.serNum[4],HEX);
                Serial.println(" ");


                      long int s = (rfid.serNum[2]*100000)+(rfid.serNum[3]*1000)+(rfid.serNum[4]);
                      Serial.print(s,DEC);



                        
    }
    }
    rfid.halt(); 
    
      value[0]=0;
      value[1]= (rfid.serNum[2]*100000)+(rfid.serNum[3]*1000)+(rfid.serNum[4]);
  
       send_data();
        for (int j=0;j<nCycle;j++){    // a eliminer apres
          delay(1000);
        } 

        
}
