#include "HAL/HAL.h"
#include "App/Configs/Version.h"
#include <Arduino.h>
#include "HAL/HAL.h"
#include "Port/Display.h"
#include "App/App.h"
#include "App/ano_dt.hpp"
#include "App/Configs/Config.h"
#include "HAL_Audio.h"
#include "App/rgb_led.h"

Pixel rgb;
TaskHandle_t handleTaskUrl;
data_send dataMonitor;
extern long irValue;
extern long redValue;
extern long greenValue;
extern float beatsPerMinute;
extern uint8_t update_max_flag;

void notify()
{
    static char progress_chars[] = "|\\-/"; // 0-3
    static int p_index = 0;
    p_index++; // 0-4
    if (p_index >= 4)
        p_index = 0;
    Serial.print("\r");
    Serial.print(progress_chars[p_index]);
    Serial.print("OK");
}

void upload()
{
    dataMonitor.send_vofa_4(
        // irValue,
        // beatsPerMinute,
        // redValue,
        // greenValue
        imuInfo.pitch,
        imuInfo.roll,
        imuInfo.yaw,
        imuInfo.az);
}

void normal()
{
    HAL::Power_Update();
    HAL::Encoder_Update();
    HAL::Audio_Update();
    HAL::IMU_Update();
}

void get_sys_info()
{
    uint64_t chipid = ESP.getEfuseMac();                             // The chip ID is essentially its MAC address(length: 6 bytes).
    Serial.printf("ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32)); // print High 2 bytes
    Serial.printf("%08X\n", (uint32_t)chipid);                       // print Low 4bytes.

    Serial.printf("total heap size = %u\n", ESP.getHeapSize());
    Serial.printf("available heap = %u\n", ESP.getFreeHeap());
    Serial.printf("lowest level of free heap since boot = %u\n", ESP.getMinFreeHeap());
    Serial.printf("largest block of heap that can be allocated at once = %u\n", ESP.getMaxAllocHeap());

    Serial.printf("total Psram size = %u\n", ESP.getPsramSize());
    Serial.printf("available Psram = %u\n", ESP.getFreePsram());
    Serial.printf("lowest level of free Psram since boot = %u\n", ESP.getMinFreePsram());
    Serial.printf("largest block of Psram that can be allocated at once = %u\n", ESP.getMinFreePsram());

    Serial.printf("get Chip Revision = %u\n", ESP.getChipRevision());
    Serial.printf("getCpuFreqMHz = %u\n", ESP.getCpuFreqMHz());
    Serial.printf("get Cycle Count = %u\n", ESP.getCycleCount());
    Serial.printf("get SdkVersion = %s\n", ESP.getSdkVersion());
}

void notifyUrlThread()
{
#ifdef ARDUINO
    HAL::TerminalPrintln("notifyUrlThread");
    xTaskNotifyGive(handleTaskUrl);
#endif
}

void TaskUrlUpdate(void *parameter)
{
    // audio_init();
    // audio_start();
    for (;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        HAL::Weather_Update();
        delay(5);
        HAL::Clock_Update();
        // audio_loop();
        delay(1);
    }
}

void NormalUpdate(void *parameter)
{
    for (;;)
    {
        __IntervalExecute(normal(), 30);
        // __IntervalExecute(upload(), 200);
        // __IntervalExecute(get_sys_info(), 1500);
        // __IntervalExecute(HAL::sensors_max30102_data(), 1000);
        delay(1);
if (update_max_flag)
        __IntervalExecute(HAL::sensors_max30102_data(), 300);
            // HAL::sensors_max30102_data();
    }
}

void HAL::Init()
{
    Serial.begin(115200);

    lv_disp_buf_p = static_cast<lv_color_t *>(
        malloc(DISP_BUF_SIZE * sizeof(lv_color_t)));
    if (lv_disp_buf_p == nullptr)
        LV_LOG_WARN("lv_port_disp_init malloc failed!\n");

    config_load();
    config_weather_load(&HAL::weaInfo);
    config_clock_load(&HAL::time_stamp_info);

    // config_clock_load(&HAL::time_stamp_info.preNetTimestamp);
    // time_stamp_info.preLocalTimestamp = millis();

    HAL::Power_Init();
    HAL::Backlight_Init();
    HAL::Buzz_init();
    HAL::Audio_Init();
    HAL::wifi_init();
    HAL::wifi_connect();
    HAL::I2C_Init(true); // 0x68 0x76
    HAL::sensors_init(); 
    HAL::IMU_Init();
    // HAL::SD_Init();
    // audio_init();
    HAL::Encoder_Init();
    HAL::Audio_PlayMusic("Startup");

    Port_Init();
    App_Init();
    rgb.init();
    rgb.setBrightness(0.1).setRGB(0, 0, 122, 204).setRGB(1, 0, 122, 204);

    // audio_start();


    // 116-76=40KB
    // xTaskCreate(
    //         TaskUrlUpdate,
    //         "TaskUrlUpdate",
    //         1024*35, //KB
    //         NULL,
    //         1,
    //         &handleTaskUrl
    //         );

    xTaskCreatePinnedToCore(
        TaskUrlUpdate,
        "TaskUrlUpdate",
        1024 * 45, // KB
        NULL,
        configMAX_PRIORITIES - 1,
        &handleTaskUrl,
        0);

    xTaskCreatePinnedToCore(
        NormalUpdate,
        "NormalUpdate",
        1024 * 1.5, // KB
        NULL,
        configMAX_PRIORITIES - 2,
        NULL,
        0);
}

void HAL::Update()
{
    // core 1
    lv_task_handler();
    // if (millis() > 3000)
    //     audio_loop();
}
