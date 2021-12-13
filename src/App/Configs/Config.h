#ifndef __CONFIG_H
#define __CONFIG_H

/*=========================
   Application configuration
 *=========================*/

#define CONFIG_SYSTEM_SAVE_FILE_PATH          "S:/SystemSave.json"
#define CONFIG_SYSTEM_LANGUAGE_DEFAULT        {'e','n','-','G','B'}
#define CONFIG_SYSTEM_GMT_OFFSET_DEFAULT      8 // GMT+ 8
#define CONFIG_WEIGHT_DEFAULT                 65 // kg
#define CONFIG_GPS_REFR_PERIOD                1000 // ms
#define CONFIG_GPS_LNG_DEFAULT                116.391332
#define CONFIG_GPS_LAT_DEFAULT                39.907415
#define CONFIG_TRACK_FILTER_OFFSET_THRESHOLD  2 // pixel
#define CONFIG_TRACK_RECORD_FILE_DIR_NAME     "Track"
#define CONFIG_MAP_USE_WGS84_DEFAULT          false
#define CONFIG_MAP_DIR_PATH                   {'/','M','A','P'}//"/MAP"
#define CONFIG_ARROW_THEME_DEFAULT            "default"
#define CONFIG_LIVE_MAP_LEVEL_DEFAULT         16
#define CONFIG_LIVE_MAP_VIEW_WIDTH            LV_HOR_RES
#define CONFIG_LIVE_MAP_VIEW_HEIGHT           LV_VER_RES
#define CONFIG_TRACK_VIRTUAL_GPX_FILE_PATH    "S:/TRK_20210801_203324.gpx"

/*=========================
   Hardware Configuration
 *=========================*/

#define CONFIG_BUZZ_PIN             25
#define CONFIG_BUZZ_CHANNEL         2
#define CONFIG_SOUND_ENABLE_DEFAULT false
#define CONFIG_POWER_EN_PIN         21
#define CONFIG_SD_DET_PIN           22

#ifdef BOARD_WROOM32

#define CONFIG_SCREEN_BLK_PIN       12
#define CONFIG_BAT_DET_PIN          37
#define CONFIG_BAT_CHG_DET_PIN      38
#define CONFIG_MCU_SDA_PIN          32
#define CONFIG_MCU_SCL_PIN          33
#define CONFIG_ENCODER_B_PIN        34
#define CONFIG_ENCODER_A_PIN        32
#define CONFIG_ENCODER_PUSH_PIN     35

#elif defined(BOARD_WROVER32) 

#define CONFIG_SCREEN_BLK_PIN       27
#define CONFIG_BAT_DET_PIN          32
#define CONFIG_BAT_CHG_DET_PIN      33
#define CONFIG_MCU_SDA_PIN          21
#define CONFIG_MCU_SCL_PIN          22
#define CONFIG_ENCODER_B_PIN        36
#define CONFIG_ENCODER_A_PIN        34
#define CONFIG_ENCODER_PUSH_PIN     35

#else

#define CONFIG_SCREEN_BLK_PIN       12
#define CONFIG_BAT_DET_PIN          37
#define CONFIG_BAT_CHG_DET_PIN      38
#define CONFIG_MCU_SDA_PIN          32
#define CONFIG_MCU_SCL_PIN          33
#define CONFIG_ENCODER_B_PIN        34
#define CONFIG_ENCODER_A_PIN        35
#define CONFIG_ENCODER_PUSH_PIN     27

#endif

#endif
