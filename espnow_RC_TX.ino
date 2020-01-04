#include <WiFi.h>

extern "C" { 
  #include <esp_now.h> 
}

//ADC_MODE(ADC_VCC);

#include "RC.h"

// this is the MAC Address of the remote ESP 
  uint8_t remoteMac1[] = {0x24, 0x6F, 0x28, 0xAA, 0x3B, 0x18}; // Server MAC
  uint8_t remoteMac2[] = {0xEC, 0xFA, 0xBC, 0x07, 0xE2, 0xAB};

#define WIFI_CHANNEL 4

volatile boolean recv;
#define TdataSizeMax 32
uint8_t TdataLen;
uint8_t Tdata[TdataSizeMax];

void recv_cb(const uint8_t  *macaddr, const uint8_t  *data, int len)
{
  recv = true;
  Serial.print("recv_cb ");
  Serial.println(len); 
  if (len <= TdataSizeMax)
  {
    TdataLen = len;
    for (int i=0;i<len;i++) Tdata[i] = data[i];
  }
};

void send_cb(const uint8_t* mac, esp_now_send_status_t  sendStatus) 
{
  Serial.print("send_cb ");
};

void setup() 
{
  Serial.begin(115200); Serial.println();

  WiFi.mode(WIFI_STA); // Station mode for esp-now 
  WiFi.disconnect();

  Serial.printf("This mac: %s, ", WiFi.macAddress().c_str()); 
  Serial.printf(", channel: %i\n", WIFI_CHANNEL); 

  if (esp_now_init() != 0) Serial.println("*** ESP_Now init failed");

  esp_now_peer_info peer = { 
                   { 0x24, 0x6F, 0x28, 0xAA, 0x3B, 0x18 }, 
                   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},  
                   WIFI_CHANNEL, ESP_IF_WIFI_STA, false, NULL };
  //esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_add_peer(&peer);
  //esp_now_add_peer(remoteMac2, ESP_NOW_ROLE_COMBO, WIFI_CHANNEL, NULL, 0);

  esp_now_register_recv_cb(recv_cb);
  esp_now_register_send_cb(send_cb);

  init_RC();
}

uint32_t lastRC; 

void loop() 
{
  uint32_t now; 
  delay(2);
  now = millis(); // actual time
  
  if (gotRC)
  {
    gotRC = false;
    rc_to_buf();
    esp_now_send(remoteMac1, RCdata.data, RCdataSize); 
    //esp_now_send(NULL, RCdata.data, RCdataSize); // Sent to all peers
    seqno++;
    
    //Serial.println(now -lastRC); 
    lastRC = now;

    //Serial.print(rcValue[0]); Serial.print("  ");
    //Serial.print(rcValue[1]); Serial.print("  ");
    //Serial.print(rcValue[2]); Serial.print("  ");
    //Serial.print(rcValue[3]); Serial.println();
  }
  else if (now >= lastRC + 100)
  {
    lastRC = now;
    Serial.println("No PPM input");  
  }

  if (recv)
  {
    recv = false;
    Serial.write(Tdata,TdataLen);
  }

  //Serial.println(analogRead(A0));
}
