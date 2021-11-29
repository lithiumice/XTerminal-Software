#include "HAL/HAL.h"
#include "App/Configs/Version.h"

#define DISP_HOR_RES         CONFIG_SCREEN_HOR_RES
#define DISP_VER_RES         CONFIG_SCREEN_VER_RES
//#define DISP_BUF_SIZE        CONFIG_SCREEN_BUFFER_SIZE//112KB
#define DISP_BUF_SIZE        (240*240/6)//18.6KB
extern lv_color_t* lv_disp_buf_p;

void HAL::Init()
{
    Serial.begin(115200);

    lv_disp_buf_p = static_cast<lv_color_t*>(
            malloc(DISP_BUF_SIZE * sizeof(lv_color_t))
            );
    if (lv_disp_buf_p == nullptr)
        LV_LOG_WARN("lv_port_disp_init malloc failed!\n");

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
#include <Ticker.h>
#include <esp_heap_caps.h>
void notify()
{
   static char progress_chars[] = "|\\-/";//0-3
   static int p_index = 0;

   p_index++;//0-4
   if(p_index>=4)
       p_index = 0;
   Serial.print("\r");
   Serial.print(progress_chars[p_index]);
   Serial.print("OK");
}


void HAL::Update()
{
    HAL::Power_Update();
    HAL::Encoder_Update();
    HAL::Audio_Update();
    HAL::IMU_Update();
    if(millis()>2000)
    {
   __IntervalExecute(HAL::Weather_Update(), 1000 * 30);

    }
    __IntervalExecute(HAL::SD_Update(), 500);
    __IntervalExecute(notify(), 600);

}
