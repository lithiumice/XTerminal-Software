//
// Created by lithiumice on 2021/11/29.
//
#include <Preferences.h>
#include "HAL.h"

namespace HAL {
    Config_t config={
            .wifi_name="LithiumMe",
            .wifi_pwd="1234qwer",
            .host_name="LithiumPeak",

            .auto_enter_weather=true,
            .auto_enter_weather_delay_sec=4,

            .backlight_256=256,
            .update_clock_interval_minute=4,
            .update_weather_interval_minute=5,

            .wifi_name_passswd = WIFI_SET_NAME,
            .clock_url_get_sucess_flag=0,
            .weather_url_get_sucess_flag=0,
            .enc_btn_first_push_flag=0,
    };
}
Preferences prefs;

#define AUTO_ENTER_WEATHER "K1"
#define AUTO_ENTER_WEATHER_DELAY_SEC "K2"
#define BACKLIGHT_256 "K3"
#define UPDATE_CLOCK_INTERVAL "K4"
#define UPDATE_WEATHER_INTERVAL "K5"

void HAL::config_load()
{
    prefs.begin("Config");

//    if(prefs.isKey("host_name"))
    config.host_name=prefs.getString("host_name", "LithiumPeak");
    config.wifi_name=prefs.getString("wifi_name", "LithiumMe");
    config.wifi_pwd=prefs.getString("wifi_pwd", "1234qwer");

    config.auto_enter_weather=prefs.getBool(AUTO_ENTER_WEATHER,0);
    config.auto_enter_weather_delay_sec=prefs.getInt(AUTO_ENTER_WEATHER_DELAY_SEC,5);
    config.backlight_256=prefs.getInt(BACKLIGHT_256,256);
    config.update_clock_interval_minute=prefs.getInt(UPDATE_CLOCK_INTERVAL, 2);
    config.update_weather_interval_minute=prefs.getInt(UPDATE_WEATHER_INTERVAL, 5);

    prefs.end();
}

void HAL::config_save()
{
    prefs.begin("Config");

    prefs.putString("host_name", config.host_name);
    prefs.putString("wifi_name", config.wifi_name);
    prefs.putString("wifi_pwd", config.wifi_pwd);

    prefs.putBool(AUTO_ENTER_WEATHER,config.auto_enter_weather);
    prefs.putInt(AUTO_ENTER_WEATHER_DELAY_SEC,config.auto_enter_weather_delay_sec);
    prefs.putInt(BACKLIGHT_256,config.backlight_256);
    prefs.putInt(UPDATE_CLOCK_INTERVAL, config.update_clock_interval_minute);
    prefs.putInt(UPDATE_WEATHER_INTERVAL,config.update_weather_interval_minute);

    prefs.end();
}

void HAL::config_set(HAL::Weather_Info_t* info)
{
    prefs.begin("Config");

    prefs.putString("cityname", info->cityname);
    prefs.putString("weather", info->weather);
    prefs.putInt("humidity", info->humidity);
    prefs.putInt("temperature", info->temperature);
    prefs.putInt("maxTmep", info->maxTmep);
    prefs.putInt("minTemp", info->minTemp);
    prefs.putInt("windLevel", info->windLevel);
    prefs.putInt("airQulity", info->airQulity);
    prefs.putString("windDir", info->windDir);

//    HAL::TerminalPrintln("pref:" + String(prefs.freeEntries()));
    prefs.end();
}

void HAL::config_set_clock(int64_t Timestamp)
{
    //prefs.begin("mynamespace");
//
//String naisu = prefs.getString("naisu", "555");
//prefs.putString("naisu", "233");
//prefs.remove("naisu");
//prefs.clear();
//prefs.end();
//Serial.println(prefs.freeEntries());
    prefs.begin("Config");

    prefs.putLong64("Timestamp",Timestamp);

    prefs.end();
}

void HAL::config_get_clock(int64_t* Timestamp)
{
    prefs.begin("Config");

    *Timestamp=prefs.getLong64("Timestamp",0);

    prefs.end();
}

void HAL::config_get(HAL::Weather_Info_t* info)
{
    prefs.begin("Config");

    String tmp=prefs.getString("cityname", "Chongqing");
    strcpy(info->cityname,tmp.c_str());
//    HAL::TerminalPrintln(tmp);

    tmp=prefs.getString("weather", "yun").c_str();
    strcpy(info->weather,tmp.c_str());
//    HAL::TerminalPrintln(tmp);

    info->humidity=prefs.getInt("humidity", 0);
    info->temperature=prefs.getInt("temperature", 0);
    info->maxTmep=prefs.getInt("maxTmep", 0);
    info->minTemp=prefs.getInt("minTemp", 0);
    info->windLevel=prefs.getInt("windLevel", 0);
    info->airQulity=prefs.getInt("airQulity", 0);

    tmp=prefs.getString("windDir", "east").c_str();
    strcpy(info->windDir,tmp.c_str());
//    HAL::TerminalPrintln(tmp);

    prefs.end();
}