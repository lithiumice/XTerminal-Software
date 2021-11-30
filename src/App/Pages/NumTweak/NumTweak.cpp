#include "NumTweak.h"

#include <ctime>

#include "HAL/HAL.h"
#include   <stdio.h>
#include  <stdlib.h>
using namespace Page;


NumTweak::NumTweak()
{
}

NumTweak::~NumTweak()
{

}

void NumTweak::onCustomAttrConfig()
{
	SetCustomCacheEnable(true);
	SetCustomLoadAnimType(PageManager::LOAD_ANIM_OVER_BOTTOM, 500, lv_anim_path_bounce);
}

void NumTweak::onViewLoad()
{
	// View.Create(root);
	lv_obj_remove_style_all(root);
	lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES);
	StatusBar::SetStyle(StatusBar::STYLE_BLACK);

	lv_obj_t* level_label = lv_label_create(root); lv_label_set_text(level_label, "level:");
	lv_obj_set_style_text_color(level_label, lv_color_white(), 0);
	lv_obj_set_style_text_font(level_label, &lv_font_montserrat_12, 0);
	lv_obj_align(level_label, LV_ALIGN_TOP_RIGHT, -40, 10);

}

void NumTweak::onViewDidLoad()
{

}

void NumTweak::onViewWillAppear()
{
	lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), ui.group);
	StatusBar::SetStyle(StatusBar::STYLE_TRANSP);

	lv_obj_set_style_bg_color(root, lv_color_white(), LV_PART_MAIN);

	timer = lv_timer_create(onTimerUpdate, 500, this);
	lv_timer_ready(timer);

	lv_obj_fade_in(root, 300, 0);
}

void NumTweak::onViewDidAppear()
{
	// lv_group_focus_obj(View.ui.canvas);
}

void NumTweak::onViewWillDisappear()
{
	lv_obj_fade_out(root, 300, 0);
}

void NumTweak::onViewDidDisappear()
{
	lv_timer_del(timer);
}

void NumTweak::onViewDidUnload()
{
	// View.Delete();
	// Model.DeInit();
}

void NumTweak::AttachEvent(lv_obj_t* obj)
{
	lv_obj_set_user_data(obj, this);
	lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void NumTweak::Update()
{

}

void NumTweak::onTimerUpdate(lv_timer_t* timer)
{
	NumTweak* instance = (NumTweak*)timer->user_data;
	instance->Update();
}

void NumTweak::onEvent(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (NumTweak*)lv_obj_get_user_data(obj);

	if (code == LV_EVENT_RELEASED)
	{
		HAL::TerminalPrintln("NumTweak LV_EVENT_PRESSED");
		instance->Manager->Pop();
	}
	else if (
		code == LV_EVENT_READY ||
		code == LV_EVENT_LONG_PRESSED ||
		code == LV_EVENT_CLICKED ||
		code == LV_EVENT_FOCUSED
		) {

		instance->Manager->Pop();
	}
}
