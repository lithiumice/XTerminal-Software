#include "HAL/HAL.h"
#include "HAL_Config.h"
#include "App/Configs/Version.h"

#define DISP_HOR_RES         CONFIG_SCREEN_HOR_RES
#define DISP_VER_RES         CONFIG_SCREEN_VER_RES
//#define DISP_BUF_SIZE        CONFIG_SCREEN_BUFFER_SIZE//112KB
#define DISP_BUF_SIZE        (240*240/6)//18.6KB
extern lv_color_t *lv_disp_buf_p;
extern HAL::Weather_Info_t weaInfo;
extern HAL::TimeStamp_t time_stamp_info;
extern uint8_t fisrt_get_weather_flag;
extern uint8_t fisrt_get_clock_flag;

void HAL::Init() {
    Serial.begin(115200);

    lv_disp_buf_p = static_cast<lv_color_t *>(
            malloc(DISP_BUF_SIZE * sizeof(lv_color_t))
    );
    if (lv_disp_buf_p == nullptr)
        LV_LOG_WARN("lv_port_disp_init malloc failed!\n");


    config_load();
    config_get(&weaInfo);
    config_get_clock(&time_stamp_info.preNetTimestamp);
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


void HAL::Update() {
    HAL::Power_Update();
    HAL::Encoder_Update();
    HAL::Audio_Update();
    HAL::IMU_Update();

    __IntervalExecute(HAL::SD_Update(), 500);
    __IntervalExecute(notify(), 600);
//    __IntervalExecute(HAL::Weather_Update(), 1000 * 30*1);
//    __IntervalExecute(HAL::parseTimeStamp(HAL::getTimestampUrl()), 1000 * 60*1.5);

    static unsigned long lasttime_clock = 0;
    static unsigned long lasttime_weather = 0;

    if (millis() > 1000) {
        do {
            if (fisrt_get_weather_flag == 0 ||
                millis() - lasttime_weather >=
                (1000 * 60 * config.update_weather_interval_minute))
            {
                if (fisrt_get_weather_flag &&
                    abs(lasttime_weather - lasttime_weather) < 1000 * 25)
                    break;

                HAL::Weather_Update();
                lasttime_weather = millis();
            }
        } while (0);


        do {
            if (fisrt_get_clock_flag == 0 ||
                millis() - lasttime_clock >= (1000 * 60 * config.update_clock_interval_minute))
            {
                if (fisrt_get_clock_flag &&
                    abs(lasttime_weather - lasttime_weather) < 1000 * 20)
                    break;

                HAL::Clock_Update();
                lasttime_clock = millis();
            }
        } while (0);
    }


}
