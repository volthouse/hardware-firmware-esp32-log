#include <WiFi.h>
#include "file.h"
#include "camera.h"
#include "time.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "settings.h"
#include "upload.h"

const char *ssid = SSID;
const char *password = PWD;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

void startCameraServer();

//#define ARDUHAL_LOG_LEVEL 5


void printLocalTime()
{
	struct tm timeinfo;
	if(!getLocalTime(&timeinfo)){
		Serial.println("Failed to obtain time");
		return;
	}

	Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup()
{
	Serial.begin(115200);
	Serial.setDebugOutput(true);
	Serial.println();
	initFileSystem();
	initCamera();

	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");
	startCameraServer();
	Serial.print("Camera Ready! Use 'http://");
	Serial.print(WiFi.localIP());
	Serial.println("' to connect");

	//init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();	

}

void loop()
{
	static unsigned long previousMillis = 0;
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= 10000) {
		upload();
		previousMillis = currentMillis;
	}
}