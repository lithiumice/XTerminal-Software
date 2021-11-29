#include "Pictures.h"
#include "App/App.h"
#include "stdio.h"
#include "HAL/HAL.h"
using namespace Page;

Pictures::Pictures()
{
}

Pictures::~Pictures()
{

}

void Pictures::onCustomAttrConfig()
{
	SetCustomCacheEnable(true);
	SetCustomLoadAnimType(PageManager::LOAD_ANIM_OVER_BOTTOM, 500, lv_anim_path_bounce);
}

void Pictures::onViewLoad()
{
	View.Create(root);

	AttachEvent(root);
}

void Pictures::onViewDidLoad()
{

}

void Pictures::onViewWillAppear()
{
	lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), View.ui.group);
	StatusBar::SetStyle(StatusBar::STYLE_TRANSP);
	
	lv_obj_set_style_bg_color(root, lv_color_white(), LV_PART_MAIN);

	timer = lv_timer_create(onTimerUpdate, 500, this);
	lv_timer_ready(timer);

	lv_obj_fade_in(root, 300, 0);
}

void Pictures::onViewDidAppear()
{
	// lv_group_focus_obj(View.ui.canvas);
}

void Pictures::onViewWillDisappear()
{
	lv_obj_fade_out(root, 300, 0);
}

void Pictures::onViewDidDisappear()
{
	lv_timer_del(timer);
}

void Pictures::onViewDidUnload()
{
	View.Delete();
	Model.DeInit();
}

void Pictures::AttachEvent(lv_obj_t* obj)
{
	lv_obj_set_user_data(obj, this);
	lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Pictures::Update()
{
}

void Pictures::onTimerUpdate(lv_timer_t* timer)
{
	Pictures* instance = (Pictures*)timer->user_data;

	instance->Update();
}

void Pictures::onEvent(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (Pictures*)lv_obj_get_user_data(obj);


	if (code == LV_EVENT_RELEASED)
	{
		HAL::TerminalPrintln("Pictures LV_EVENT_PRESSED");
		instance->Manager->Pop();
	}
	else if (code == LV_EVENT_LONG_PRESSED)
	{
		HAL::TerminalPrintln("Pictures LV_EVENT_LONG_PRESSED");
		instance->Manager->Pop();
	}
}
