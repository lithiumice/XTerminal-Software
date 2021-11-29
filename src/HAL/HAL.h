#ifndef __HAL_H
#define __HAL_H

#include "string"
#include <stdint.h>
#include "HAL_Def.h"
#include "App/Configs/Config.h"
#include "CommonMacro.h"

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
#endif



namespace HAL
{

    void getWeatherNowUrl(void);
    void getWeatherWeekUrl(short maxT[], short minT[]);
    long long getTimestampLocal();
    long long getTimestampUrl();

    void Init();
    void Update();

/* WIFI */
    void wifi_init();
    void wifi_search();
    bool wifi_connect();
    bool wifi_close();
    bool wifi_open_ap();
    bool wifi_isconnected();

/* Terminal */
    extern std::string globalTermText;
    extern uint8_t term_text_update_flag;

    void TerminalPrint( char* text);
    void TerminalPrintln( char* text);
    // void TerminalPrint( string text);
    // void TerminalPrintln( string text);
    void TerminalPrintln( String text);

/* Backlight */
    void Backlight_Init();
    uint32_t Backlight_GetValue();
    void Backlight_SetValue(int32_t val);
    void Backlight_SetGradual(uint32_t target, uint16_t time = 500);
    void Backlight_ForceLit(bool en);

/* I2C */
    void I2C_Init(bool startScan);

/* IMU */
    void IMU_Init();
    void IMU_Update();

    void Weather_Update();

/* SD */
    bool SD_Init();
    void SD_Update();
    bool SD_GetReady();
    float SD_GetCardSizeMB();
    typedef void(* SD_CallbackFunction_t)(bool insert);
    void SD_SetEventCallback(SD_CallbackFunction_t callback);


/* Power */
    void Power_Init();
    void Power_HandleTimeUpdate();
    void Power_SetAutoLowPowerTimeout(uint16_t sec);
    uint16_t Power_GetAutoLowPowerTimeout();
    void Power_SetAutoLowPowerEnable(bool en);
    void Power_Shutdown();
    void Power_Update();
    void Power_EventMonitor();
    void Power_GetInfo(Power_Info_t* info);
    typedef void(* Power_CallbackFunction_t)(void);
    void Power_SetEventCallback(Power_CallbackFunction_t callback);

/* Buzzer */
    void Buzz_init();
    void Buzz_SetEnable(bool en);
    void Buzz_Tone(uint32_t freq, int32_t duration = 0);

/* Encoder */
    void Encoder_Init();
    void Encoder_Update();
    int16_t Encoder_GetDiff();
    bool Encoder_GetIsPush();
    void Encoder_SetEnable(bool en);

/* Audio */
    void Audio_Init();
    void Audio_Update();
    bool Audio_PlayMusic(const char* name);


/* Bluetooth */
    bool BT_Init();
    void BT_Update();
    bool BluetoothConnected();
}

#endif
