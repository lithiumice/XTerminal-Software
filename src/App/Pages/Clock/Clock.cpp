#include "Clock.h"

#include <ctime>
#include "Arduino.h"
#include "HAL/HAL.h"
#include <stdio.h>
#include <stdlib.h>
using namespace Page;

Clock::Clock()
{
}

Clock::~Clock()
{
}

void Clock::onCustomAttrConfig()
{
	SetCustomCacheEnable(true);
	SetCustomLoadAnimType(PageManager::LOAD_ANIM_OVER_BOTTOM, 500, lv_anim_path_bounce);
}

void Clock::GUICreate()
{

	ui.clockLabel_1 = lv_label_create(root);
	lv_label_set_recolor(ui.clockLabel_1, true);
	lv_label_set_text_fmt(ui.clockLabel_1, "%02d#ffa500 %02d#%02d", 10, 52,11);
	// lv_label_set_text_fmt(ui.clockLabel_1, "%02d : %02d", 10, 52);
	// lv_label_set_text_fmt(ui.clockLabel_1, "%02d : #ffa500 %02d#", 10, 52);
	lv_obj_set_style_text_font(ui.clockLabel_1, Resource.GetFont("Morganite_100"), 0);
	// lv_obj_set_style_text_font(ui.clockLabel_1, &lv_font_montserrat_42, 0);
	lv_obj_set_style_text_color(ui.clockLabel_1, lv_color_white(), 0);
	lv_obj_align(ui.clockLabel_1, LV_ALIGN_CENTER, 0, 0);

	// ui.clockLabel_2 = lv_label_create(root);
	// lv_label_set_recolor(ui.clockLabel_2, true);
	// lv_label_set_text_fmt(ui.clockLabel_2, "%02d", 00);
	// lv_obj_set_style_text_font(ui.clockLabel_2, &lv_font_montserrat_12, 0);
	// lv_obj_set_style_text_color(ui.clockLabel_2, lv_color_white(), 0);
	// // lv_obj_set_pos(ui.clockLabel_2, 170, 105);
	// lv_obj_align(ui.clockLabel_2, LV_ALIGN_CENTER, 0, 60);
}

// void Clock::updateClock()
// {
// 	lv_label_set_text_fmt(
// 		ui.clockLabel_1,
// 		"%02d#ffa500 %02d#",
// 		HAL::time_info.hour,
// 		HAL::time_info.minute);
// }

void Clock::updateSeconds()
{
	HAL::time_info.second++;
	if (HAL::time_info.second >= 60) // 60s
	{
		HAL::time_info.second = 0;

#ifdef ARDUINO
		HAL::parseTimeStamp(HAL::getTimestampLocal());
#endif
		// updateClock();
	}
	// lv_label_set_text_fmt(
	// 	ui.clockLabel_2,
	// 	"%02dSECONDS",
	// 	HAL::time_info.second);
		lv_label_set_text_fmt(
		ui.clockLabel_1,
		"%02d#ffa500 %02d#%02d",
		HAL::time_info.hour,
		HAL::time_info.minute,
		HAL::time_info.second);
}

void Clock::onViewLoad()
{

	lv_obj_remove_style_all(root);
	lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES);
	StatusBar::SetStyle(StatusBar::STYLE_BLACK);
	ui.group = lv_group_create();

	GUICreate();
	lv_group_add_obj(ui.group, root);
	AttachEvent(root);
}

void Clock::onViewDidLoad()
{
}

void Clock::onViewWillAppear()
{
	lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), ui.group);
	StatusBar::SetStyle(StatusBar::STYLE_TRANSP);

	lv_obj_set_style_bg_color(root, lv_color_white(), LV_PART_MAIN);

	timer = lv_timer_create(onTimerUpdate, 500, this);
	lv_timer_ready(timer);

	lv_obj_fade_in(root, 300, 0);

#ifdef ARDUINO
	notifyUrlThread();
	HAL::parseTimeStamp(HAL::getTimestampLocal());
#endif
	updateSeconds();
}

void Clock::onViewDidAppear()
{
}

void Clock::onViewWillDisappear()
{
	lv_obj_fade_out(root, 300, 0);
}

void Clock::onViewDidDisappear()
{
	lv_timer_del(timer);
}

void Clock::onViewDidUnload()
{
	// View.Delete();
	// Model.DeInit();
}

void Clock::AttachEvent(lv_obj_t *obj)
{
	lv_obj_set_user_data(obj, this);
	lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Clock::Update()
{
	__IntervalExecute(notifyUrlThread(), (1000 * 60 * HAL::config.update_weather_interval_minute));
	__IntervalExecute(updateSeconds(), 1000);

	    if (HAL::clock_url_get_sucess_flag)
    {
        HAL::clock_url_get_sucess_flag = 0;
        // updateClock();
        updateSeconds();
    }
}

void Clock::onTimerUpdate(lv_timer_t *timer)
{
	Clock *instance = (Clock *)timer->user_data;
	instance->Update();
}

void Clock::onEvent(lv_event_t *event)
{
	lv_obj_t *obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto *instance = (Clock *)lv_obj_get_user_data(obj);

	if (code == LV_EVENT_RELEASED)
	{
		instance->Manager->Pop();
	}
	else if (code == LV_EVENT_LONG_PRESSED)
	{

		instance->Manager->Pop();
	}
}
