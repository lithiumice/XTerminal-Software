#include "HAL/HAL.h"
#include "HAL_Config.h"
#include "App/Configs/Version.h"
#include <Arduino.h>
#include "HAL/HAL.h"
#include "Port/Display.h"
#include "App/App.h"

#define DISP_HOR_RES         CONFIG_SCREEN_HOR_RES
#define DISP_VER_RES         CONFIG_SCREEN_VER_RES
#define DISP_BUF_SIZE        (240*240/6)//18.6KB

TaskHandle_t handleTaskUrl;
void notifyUrlThread()
{
#ifdef ARDUINO
    HAL::TerminalPrintln("notifyUrlThread");
    xTaskNotifyGive(handleTaskUrl);
#endif
}

void TaskUrlUpdate(void* parameter)
{
    for (;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        HAL::Weather_Update();
        delay(5);
        HAL::Clock_Update();
        delay(5);
    }
}

void HAL::Init() {
    Serial.begin(115200);

    lv_disp_buf_p = static_cast<lv_color_t *>(
            malloc(DISP_BUF_SIZE * sizeof(lv_color_t))
    );
    if (lv_disp_buf_p == nullptr)
        LV_LOG_WARN("lv_port_disp_init malloc failed!\n");


    config_load();
    config_weather_load(&weaInfo);
    config_clock_load(&time_stamp_info.preNetTimestamp);
    time_stamp_info.preLocalTimestamp = millis();

    HAL::Power_Init();
    HAL::Backlight_Init();
    HAL::Encoder_Init();
    HAL::Buzz_init();
    HAL::Audio_Init();
    HAL::SD_Init();
    HAL::I2C_Init(true);
    HAL::IMU_Init();
    HAL::wifi_init();
    HAL::wifi_connect();
    HAL::Audio_PlayMusic("Startup");

    Port_Init();
    App_Init();

    TaskHandle_t handleTaskWeather;
    xTaskCreate(
            TaskUrlUpdate,
            "GetWeather",
            1024*30, //KB
            NULL,
            configMAX_PRIORITIES - 1,
            &handleTaskUrl);
}

void notify() {
    static char progress_chars[] = "|\\-/";//0-3
    static int p_index = 0;
    p_index++;//0-4
    if (p_index >= 4)
        p_index = 0;
    Serial.print("\r");
    Serial.print(progress_chars[p_index]);
    Serial.print("OK");
}

void normal()
{
    HAL::Power_Update();
    HAL::Encoder_Update();
    HAL::Audio_Update();
    HAL::IMU_Update();
}

void HAL::Update() {
    lv_task_handler();
    __IntervalExecute(normal(), 20);
    __IntervalExecute(HAL::SD_Update(), 500);
    __IntervalExecute(notify(), 600);

    static uint8_t wifi_smartconfig_state = 0;
    if(wifi_smartconfig_state==0)
    {
        if(HAL::wifi_isconnected())
        {
            wifi_smartconfig_state = 2;
        }
        if(millis()>1000*20)
        {
            wifi_smartconfig_state = 1;
        }
    }
    //已经连到wifi
    else if(wifi_smartconfig_state==2)
    {
        wifi_smartconfig_state = 2;
    }
    //开始智能配网
    else if(wifi_smartconfig_state==1)
    {
        wifi_smartconfig_state = 3;
        HAL::wifi_smartConfig();
    }
    //等待配网
    else if(wifi_smartconfig_state==3)
    {
        if (WiFi.smartConfigDone())
        {
             wifi_smartconfig_state = 4;
            Serial.println("配网成功");
            Serial.printf("SSID:%s", WiFi.SSID().c_str());
            Serial.printf("PSW:%s", WiFi.psk().c_str());
        }
    }
    //配网成功
    else if(wifi_smartconfig_state==4)
    {
        wifi_smartconfig_state = 4;
    }

    // static uint8_t connected_to_wifi_from_boot = 0;
    // if(HAL::wifi_isconnected())
    // {
    //     connected_to_wifi_from_boot = 1;
    // }

    // static uint8_t started_smartconfig = 0;
    // if(connected_to_wifi_from_boot==0&&
    //     started_smartconfig==0&&
    //     millis()>1000*20
    // )
    // {
    //     started_smartconfig = 1;
    //     HAL::wifi_smartConfig();
    // }

    // if (WiFi.smartConfigDone())
    // {
    //   Serial.println("配网成功");
    //   Serial.printf("SSID:%s", WiFi.SSID().c_str());
    //   Serial.printf("PSW:%s", WiFi.psk().c_str());
    // }

    //    xTaskNotifyGive(handleTaskLvgl);
    //    __IntervalExecute(HAL::Weather_Update(), 1000 * 30*1);
    //    __IntervalExecute(HAL::parseTimeStamp(HAL::getTimestampUrl()), 1000 * 60*1.5);

    // static unsigned long lasttime_clock = 0;
    // static unsigned long lasttime_weather = 0;

//    if (millis() > 1000) {
//        do {
//            if (fisrt_get_weather_flag == 0 ||
//                millis() - lasttime_weather >=
//                        (1000 * 60 * 3))
////                (1000 * 60 * config.update_weather_interval_minute))
//            {
//                if (fisrt_get_weather_flag &&
//                    abs(lasttime_weather - lasttime_weather) < 1000 * 25)
//                    break;
//
//                HAL::Weather_Update();
//                lasttime_weather = millis();
//            }
//        } while (0);
//
//
//        do {
//            if (fisrt_get_clock_flag == 0 ||
//                millis() - lasttime_clock >=
//                        (1000 * 60 * 2.5))
////                (1000 * 60 * config.update_clock_interval_minute))
//            {
//                if (fisrt_get_clock_flag &&
//                    abs(lasttime_weather - lasttime_weather) < 1000 * 20)
//                    break;
//
//                HAL::Clock_Update();
//                lasttime_clock = millis();
//            }
//        } while (0);
//    }
    // delay(15);
}
