#include <SPI.h>
#include "LoRa.h"



unsigned int freq = 865200000;//HZ
int i = 0;
String serialResponse = ""; // Pour la séparation


//tableau des IDs

  char* liste_nom[] = {"DACHI", "OUAJIH", "IAN"};   //BDD
  int donnee_0 = 6442230; //ID Dachi 
  int donnee_1 = 8667153 ; //ID Ouajih
  int donnee_2 = 1547080; //ID Ian
  int autorise = 0 ;
  
void setup() {
  Serial.begin(38400);
  while (!Serial);

  Serial.println("LoRa Receiver");

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


void loop() {
        char trame[500];
        char var[500];
        int vrai = 0;
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    //Serial.print("Received packet '");
    // read packet
    while (LoRa.available()) {
        var[0] = (char)LoRa.read();
        if( var[0]=='\\'  ){
          var[1] = (char)LoRa.read();
          
          var[2] = (char)LoRa.read();
                    
          var[3] = (char)LoRa.read();
          
          var[4] = (char)LoRa.read();
          
          if (var[1] =='!'  and var[2] =='2'  and var[3] =='0'  and var[4] =='9')
          {
            trame[0]='\\';
            trame[1] =  var[1];
            trame[2] =  var[2];
            trame[3] =  var[3];    
            trame[4] =  var[4];  
            vrai = 1; 
            Serial.println("hada dkhelt");
            for(int c = 5 ; c <= 37 ; c++)
            {
              trame[c] = (char)LoRa.read();

            }

            for(int c = 5 ; c <= 37 ; c++)
            {
              if (trame[c] == '2' and trame[c-1] == '3' and trame[c-2] == '4' and trame[c-3] == '#')
              {
                Serial.println("hada dkhelt l température");
                 char temperature[2];
                 temperature[0]= trame[c + 2];
                 temperature[1]= trame[c + 3];
                Serial.print(temperature[0]); 
                Serial.print(temperature[1]); 
                Serial.println(""); 

              }
              if (trame[c] == '3' and trame[c-1] == '3' and trame[c-2] == '4' and trame[c-3] == '#')
              {
                Serial.println("hada dkhelt l id");
                 char id[7];
                 if(trame[c + 3]=='.')
                 {
                  id[0]= '0'; 
                  id[1]= '\0';
                 }
                 else{
                 id[0]= trame[c + 2];
                 id[1]= trame[c + 3];
                 id[2]= trame[c + 4];
                 id[3]= trame[c + 5];
                 id[4]= trame[c + 6];
                 id[5]= trame[c + 7];
                 id[6]= trame[c + 8];
                 id[7]= '\0';
                 }
                
                int id_i = atoi(id);

                if (id_i == donnee_0)
                {
                  autorise = 1; 
                                  Serial.print(id); 
                                  Serial.println(""); 
                }else
                {
                  autorise = 0;
                }
              }
            }

              
              //Serial.println(trame);// trame dyalna ljamila




              
          }
        }

  
  }


}
  


}
