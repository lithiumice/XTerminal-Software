#include "MuiscPlayer.h"
#include "App/App.h"
#include "App/Configs/Version.h"
#include "HAL/HAL.h"
using namespace Page;

MuiscPlayer::MuiscPlayer()
{
}

MuiscPlayer::~MuiscPlayer()
{

}

void MuiscPlayer::onCustomAttrConfig()
{

}

void MuiscPlayer::onViewLoad()
{
	View.Create(root);
	AttachEvent(root);
}

void MuiscPlayer::onViewDidLoad()
{
}

void MuiscPlayer::onViewWillAppear()
{
	lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), View.ui.group);
	StatusBar::SetStyle(StatusBar::STYLE_TRANSP);

	timer = lv_timer_create(onTimerUpdate, 100, this);
	lv_timer_ready(timer);

	View.SetScrollToY(root, -LV_VER_RES, LV_ANIM_OFF);
	lv_obj_fade_in(root, 300, 0);
}

void MuiscPlayer::onViewDidAppear()
{
	View.onFocus(View.ui.group);
}

void MuiscPlayer::onViewWillDisappear()
{
	lv_obj_fade_out(root, 300, 0);
}

void MuiscPlayer::onViewDidDisappear()
{
	lv_timer_del(timer);
}

void MuiscPlayer::onViewDidUnload()
{
	View.Delete();
}

void MuiscPlayer::AttachEvent(lv_obj_t* obj)
{
	lv_obj_set_user_data(obj, this);
	lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void MuiscPlayer::Update()
{
}

void MuiscPlayer::onTimerUpdate(lv_timer_t* timer)
{
	MuiscPlayer* instance = (MuiscPlayer*)timer->user_data;

	instance->Update();
}

void MuiscPlayer::onEvent(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (MuiscPlayer*)lv_obj_get_user_data(obj);
	if (code == LV_EVENT_RELEASED)
	{
		
	}
}
