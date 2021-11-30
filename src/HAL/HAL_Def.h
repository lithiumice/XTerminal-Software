#ifndef __HAL_DEF_H
#define __HAL_DEF_H

#include <stdint.h>
#include <string>
#ifdef ARDUINO
#include "Arduino.h"
#include "FreeRTOS.h"
#include <ESP32Time.h>
#include "WString.h"

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#else
#include "WString/WString.h"
class ESP32Time{};
#define millis() lv_tick_get()
#endif

#define WIFI_SET_NAME 0
#define WIFI_SET_PWD 1

namespace HAL
{

    typedef struct {
        String wifi_name;
        String wifi_pwd;
        String host_name;

        bool auto_enter_weather;
        int auto_enter_weather_delay_sec;

        int backlight_256;
        int update_clock_interval_minute;//min
        int update_weather_interval_minute;//min

        //flag
        uint8_t wifi_name_passswd;
        uint8_t clock_url_get_sucess_flag;
        uint8_t weather_url_get_sucess_flag;
        uint8_t enc_btn_first_push_flag;


    } Config_t;

	/* Time */
	typedef struct
	{
		int month;
		int day;
		int hour;
		int minute;
		int second;
		int weekday;
	}Time_str_t;

	/* TimeStamp*/
	typedef struct
	{
		uint64_t preWeatherMillis;
		uint64_t preTimeMillis;
		uint64_t weatherUpdataInterval;
		uint64_t timeUpdataInterval;
		int64_t preNetTimestamp;
		int64_t errorNetTimestamp;
		int64_t preLocalTimestamp;
		uint32_t coactusUpdateFlag;
		int clock_page;
	}TimeStamp_t;

	/* Weather */
	typedef struct
	{
        char cityname[20];//"city":"重庆"--->"Chongqing"
        char weather[20];//"wea_img":"yun"
        int humidity;

        int temperature;//"tem":"13"
        int maxTmep;//"tem_day":"16"
        int minTemp;//"tem_night":"7"

        char windDir[20];//"win":"北风"
		int windLevel;//"win_speed":"1级"--->1

		int airQulity;//"air":"63"--->2
		//short daily_max[7];
		//short daily_min[7];
	}Weather_Info_t;

	/* Clock */
	typedef struct
	{
		uint16_t year;
		uint8_t month;
		uint8_t day;
		uint8_t week;
		uint8_t hour;
		uint8_t minute;
		uint8_t second;
		uint16_t millisecond;
	} Clock_Info_t;

	/* IMU */
	typedef struct
	{
		float ax;
		float ay;
		float az;
		float gx;
		float gy;
		float gz;
		float mx;
		float my;
		float mz;
		float roll;
		float yaw;
		float pitch;
	} IMU_Info_t;

	/* Power */
	typedef struct
	{
		uint16_t voltage;
		uint8_t usage;
		bool isCharging;
	} Power_Info_t;

}

#endif
