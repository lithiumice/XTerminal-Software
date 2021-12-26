#include "Settings.h"

#include <ctime>

#include "HAL/HAL.h"
#include <stdio.h>
#include <stdlib.h>
#include "../NumTweak/NumTweak.h"
#include "HAL/HAL_Audio.h"
#include "HAL/HAL_SdCard.h"

using namespace Page;
extern NumTweakConfig NumTweak_config;
extern uint8_t num_tweak_type;

Settings::Settings()
{
}

Settings::~Settings()
{
}

void Settings::onCustomAttrConfig()
{
	SetCustomCacheEnable(true);
	SetCustomLoadAnimType(PageManager::LOAD_ANIM_OVER_BOTTOM, 500, lv_anim_path_bounce);
}

#define ListAddItem(icon, name)                                         \
	{                                                                   \
		ui.name = lv_list_add_btn(ui.list1, icon, #name);               \
		lv_obj_set_style_text_font(ui.name, &lv_font_montserrat_14, 0); \
		lv_obj_set_style_text_color(ui.name, lv_color_white(), 0);      \
		lv_group_add_obj(ui.group, ui.name);                            \
		lv_obj_set_style_bg_color(ui.name, lv_color_black(), 0);        \
	}
#define ListAddText(name)                                            \
	{                                                                \
		lv_obj_t *name = lv_list_add_text(ui.list1, #name);          \
		lv_obj_set_style_text_font(name, &lv_font_montserrat_12, 0); \
		lv_obj_set_style_text_color(name, lv_color_white(), 0);      \
		lv_obj_set_style_bg_color(name, lv_color_black(), 0);        \
	}
#define ListAddOption(icon, name)                                                                    \
	{                                                                                                \
		ListAddItem(icon, name);                                                                     \
		ui.name##_sw = lv_switch_create(ui.name);                                                    \
		lv_group_add_obj(ui.group, ui.name##_sw);                                                    \
		lv_obj_set_height(ui.name##_sw, lv_pct(90));                                                 \
		lv_obj_align(ui.name##_sw, LV_ALIGN_RIGHT_MID, 0, 0);                                        \
		lv_obj_set_style_bg_color(ui.name##_sw, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR); \
		lv_obj_set_style_bg_color(ui.name##_sw, lv_palette_main(LV_PALETTE_GREY), 0);                \
		lv_obj_add_state(ui.name##_sw, LV_STATE_CHECKED);                                            \
	}

void Settings::onViewLoad()
{
	lv_obj_remove_style_all(root);
	lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES);
	StatusBar::SetStyle(StatusBar::STYLE_BLACK);
	lv_obj_set_style_bg_color(root, lv_color_black(), 0);

	ui.group = lv_group_create();
	ui.list1 = lv_list_create(root);
	lv_obj_set_size(ui.list1, LV_HOR_RES, LV_VER_RES - 22);
	lv_obj_align(ui.list1, LV_ALIGN_CENTER, 0, 22);
	lv_obj_set_style_radius(ui.list1, 0, 0);
	lv_obj_set_style_bg_color(ui.list1, lv_color_black(), 0);
	lv_obj_set_style_border_width(ui.list1, 0, 0);

#define SET_DEF(icon, className)  \
	ListAddItem(icon, className); \
	AttachEvent(ui.className);

#define SET_DEF2(className) \
	ListAddText(className);

#define SET_DEF3(icon, className) \
	ListAddOption(icon, className);

#include "_Setting_DEF.inc"

#undef SET_DEF3
#undef SET_DEF2
#undef SET_DEF

	// lv_obj_t *sw = lv_switch_create(ui.EnDataUpload);
	// lv_group_add_obj(ui.group, sw);
	// // lv_obj_set_width(sw, 75);
	// lv_obj_set_height(sw, lv_pct(90));
	// lv_obj_align(sw, LV_ALIGN_RIGHT_MID, 0, 0);
	// lv_obj_set_style_bg_color(sw, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
	// lv_obj_set_style_bg_color(sw, lv_palette_main(LV_PALETTE_GREY), 0);
	// lv_obj_add_state(sw, LV_STATE_CHECKED);
	// // lv_obj_add_event_cb(sw, event_handler, LV_EVENT_ALL, NULL);
}

void Settings::onViewDidLoad()
{
}

void Settings::onViewWillAppear()
{
	lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), ui.group);
	StatusBar::SetStyle(StatusBar::STYLE_TRANSP);

	lv_obj_set_style_bg_color(root, lv_color_white(), LV_PART_MAIN);

	timer = lv_timer_create(onTimerUpdate, 500, this);
	lv_timer_ready(timer);

	lv_obj_fade_in(root, 300, 0);
}

void Settings::onViewDidAppear()
{
	// lv_group_focus_obj(View.ui.canvas);
}

void Settings::onViewWillDisappear()
{
	lv_obj_fade_out(root, 300, 0);
}

void Settings::onViewDidDisappear()
{
	lv_timer_del(timer);
}

void Settings::onViewDidUnload()
{
	// View.Delete();
	// Model.DeInit();
	lv_group_del(ui.group);
}

void Settings::AttachEvent(lv_obj_t *obj)
{
	lv_obj_set_user_data(obj, this);
	lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Settings::Update()
{
}

void Settings::onTimerUpdate(lv_timer_t *timer)
{
	Settings *instance = (Settings *)timer->user_data;
	instance->Update();
}

void Settings::onEvent(lv_event_t *event)
{
	lv_obj_t *obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto *instance = (Settings *)lv_obj_get_user_data(obj);

#ifdef ARDUINO

	if (code == LV_EVENT_RELEASED)
	{
		if (obj == instance->ui.ExitThisApp)
		{
			instance->Manager->Pop();
		}
		else if (obj == instance->ui.ReconnectWifi)
		{
			HAL::I2C_Scan(&Wire1);
			// HAL::IMU_Calibrate();
			// 			HAL::wifi_connect();
			// HAL::IMU_Init();
			delay(50);
			HAL::IMU_Update();
		}
		else if (obj == instance->ui.WifiSmarConfig)
		{
			HAL::wifi_smartConfig();
		}
		else if (obj == instance->ui.RestartDevice)
		{
			ESP.restart();
		}
		else if (obj == instance->ui.RestoreConfig)
		{
			HAL::config_clear();
			ESP.restart();
		}
		else if (obj == instance->ui.ModifyBlackLight)
		{
			// uint8_t num_tweak_type = BLACKLIGHT;
			// instance->Manager->Push("Pages/NumTweak");
			listDir(SD, "/", 0);
		}

		else if (obj == instance->ui.UrlUpdateInterval)
		{
			audio_start();
			// uint8_t num_tweak_type = BLACKLIGHT;
			// instance->Manager->Push("Pages/NumTweak");
		}
		else if (obj == instance->ui.ModifyWifiName)
		{
			HAL::wifi_name_passswd = WIFI_SET_NAME;
			instance->Manager->Push("Pages/WifiText");
		}
		else if (obj == instance->ui.ModifyWifiPasswd)
		{
			HAL::wifi_name_passswd = WIFI_SET_PWD;
			instance->Manager->Push("Pages/WifiText");
		}

		HAL::TerminalPrintln("Pictures LV_EVENT_PRESSED");
	}
	else if (code == LV_EVENT_LONG_PRESSED)
	{
		HAL::TerminalPrintln("Pictures LV_EVENT_LONG_PRESSED");
		instance->Manager->Pop();
	}
#endif

}
