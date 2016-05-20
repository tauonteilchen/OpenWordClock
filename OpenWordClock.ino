#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <Adafruit_TLC5947.h>
#include <TimeLib.h> 
#include <Timezone.h>

const char *ssid     = "SSID";
const char *password = "Pasword";

// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
//NTPClient timeClient("europe.pool.ntp.org", 0,3600000); //60000*60

NTPClient timeClient("0.de.pool.ntp.org", 0,3600000);

#define NUM_TLC5974 1

#define data D1
#define clock D6
#define latch D2

#define ES_IST 0 
#define FUNF_MINUTES 1
#define ZHEN_MINUTES 2
#define NACH 3
#define VIERTEL_MINUTES 4
#define VOR 5
#define HALB_MINUTES 6

#define EINS_HOURS 8
#define ZWEI_HOURS 13
#define DREI_HOURS 9
#define VIER_HOURS 14
#define FUNF_HOURS 12
#define SECHS_HOURS 10
#define SIEBEN_HOURS 11
#define ACHT_HOURS 15
#define NEUN_HOURS 7
#define ZEHN_HOURS 19
#define ELF_HOURS 18
#define ZWOLF_HOURS 16

#define brightness 0xFFF
#define OFF 0x000

const char hours[12] = {EINS_HOURS, ZWEI_HOURS, DREI_HOURS, VIER_HOURS, FUNF_HOURS, SECHS_HOURS, SIEBEN_HOURS, ACHT_HOURS, NEUN_HOURS, ZEHN_HOURS, ELF_HOURS, ZWOLF_HOURS};

#define UHR 17
#define WIFI 23

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, clock, data, latch);

//Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Standard Time
Timezone CE(CEST, CET);

int write_minutes_to_tlc(Adafruit_TLC5947 *tlc, int minutes,int seconds)
{
  //clear minute section
  tlc->setPWM(FUNF_MINUTES,OFF);
  tlc->setPWM(ZHEN_MINUTES,OFF);
  tlc->setPWM(NACH,OFF);
  tlc->setPWM(VIERTEL_MINUTES,OFF);
  tlc->setPWM(VOR,OFF);
  tlc->setPWM(HALB_MINUTES,OFF);
  tlc->setPWM(UHR,OFF);
  
  int hour_offset = 0;
  int pos = minutes / 5;
  int modulo = minutes % 5;
  int gap_sec = (modulo * 60)+seconds;
  if(gap_sec>150)
  {
    pos++;
    pos = pos % 12;
    if(pos == 0)
    {
      hour_offset = 1;
    }
  }
  switch(pos)
  {
    case 0:
      tlc->setPWM(UHR,brightness);
      break;
    case 1:
      tlc->setPWM(FUNF_MINUTES,brightness);
      tlc->setPWM(NACH,brightness);
      break;
    case 2:
      tlc->setPWM(ZHEN_MINUTES,brightness);
      tlc->setPWM(NACH,brightness);
      break;
    case 3:
      tlc->setPWM(VIERTEL_MINUTES,brightness);
      tlc->setPWM(NACH,brightness);
      break;
    case 4:
      tlc->setPWM(ZHEN_MINUTES,brightness);
      tlc->setPWM(VOR,brightness);
      tlc->setPWM(HALB_MINUTES,brightness);
      hour_offset = 1;
      break;
    case 5:
      tlc->setPWM(FUNF_MINUTES,brightness);
      tlc->setPWM(VOR,brightness);
      tlc->setPWM(HALB_MINUTES,brightness);
      hour_offset = 1;
      break;
    case 6:
      tlc->setPWM(HALB_MINUTES,brightness);
      hour_offset = 1;
      break;
    case 7:
      tlc->setPWM(FUNF_MINUTES,brightness);
      tlc->setPWM(NACH,brightness);
      tlc->setPWM(HALB_MINUTES,brightness);
      hour_offset =1;
      break;
    case 8:
      tlc->setPWM(ZHEN_MINUTES,brightness);
      tlc->setPWM(NACH,brightness);
      tlc->setPWM(HALB_MINUTES,brightness);
      hour_offset =1;
      break;
    case 9:
      tlc->setPWM(VIERTEL_MINUTES,brightness);
      tlc->setPWM(VOR,brightness);
      hour_offset =1;
      break;
    case 10:
      tlc->setPWM(ZHEN_MINUTES,brightness);
      tlc->setPWM(VOR,brightness);
      hour_offset =1;
      break;
    case 11:
      tlc->setPWM(FUNF_MINUTES,brightness);
      tlc->setPWM(VOR,brightness);
      hour_offset =1;
      break;
  }
  return hour_offset;
}

void write_hours_to_tlc(Adafruit_TLC5947 *tlc, int h)
{
  for(int i = 0; i < 12; i++)
  {
    tlc->setPWM(hours[i],OFF);
  }
  h = (h-1) % 12;
  tlc->setPWM(hours[h],brightness);
}

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  tlc.begin();
  for(int i = 0; i < 24; i++)
  {
    tlc.setPWM(i,OFF);  
  }
  tlc.setPWM(ES_IST,brightness);
  tlc.write();
}

void loop() {
  
  if (WiFi.status() == WL_CONNECTED)
  {
    timeClient.update();
    tlc.setPWM(23,brightness);  
  }
  else
  {
    Serial.println("no wifi");
    tlc.setPWM(23,0x00);
  }
  
  time_t utc = (time_t) timeClient.getRawTime();
  time_t local_time = CE.toLocal(utc);
  
  int offset = write_minutes_to_tlc(&tlc, minute(local_time),second(local_time));
  write_hours_to_tlc(&tlc,hour(local_time) + offset);
  
  tlc.write();
  delay(1000);
}
