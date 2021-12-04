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


	ui.indicateText = lv_label_create(root);
	lv_label_set_recolor(ui.indicateText, true);
	lv_obj_set_style_text_font(ui.indicateText, &lv_font_montserrat_42, 0);
	// lv_obj_set_style_text_font(ui.indicateText, Resource.GetFont("EuroStar_60"), 0);
	lv_obj_set_style_text_color(ui.indicateText, lv_color_white(), 0);
	// lv_obj_set_style_text_color(ui.indicateText, lv_palette_main(LV_PALETTE_BLUE), 0);
	lv_obj_align(ui.indicateText, LV_ALIGN_CENTER, 0, 0);
	// lv_obj_align(ui.indicateText, LV_ALIGN_CENTER,0,-10);
	lv_label_set_text(ui.indicateText, "40");
	// ui.unitText = lv_label_create(root);
	// lv_label_set_recolor(ui.unitText, true);
	// lv_obj_set_style_text_font(ui.unitText, Resource.GetFont("Morganite_36"), 0);
	// lv_obj_set_style_text_color(ui.unitText, lv_color_white(), 0);
	// lv_obj_align(ui.unitText, LV_ALIGN_CENTER,0,10);
	// lv_label_set_text(ui.unitText, "min");

}

void Clock::onViewLoad()
{
	

	lv_obj_remove_style_all(root);
	lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES);
	StatusBar::SetStyle(StatusBar::STYLE_BLACK);
	ui.group = lv_group_create();

	GUICreate();
	lv_group_add_obj(ui.group, ui.arc);
	ArcValueCtrl(0);
	AttachEvent(ui.arc);

	
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
}

void Clock::onViewDidAppear()
{
	lv_group_focus_obj(ui.arc);
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
		HAL::TerminalPrintln("Clock LV_EVENT_PRESSED");
		instance->Manager->Pop();
	}
	else if (code == LV_EVENT_LONG_PRESSED)
	{

		HAL::TerminalPrintln("Clock LV_EVENT_LONG_PRESSED");
		instance->Manager->Pop();
	}
}
