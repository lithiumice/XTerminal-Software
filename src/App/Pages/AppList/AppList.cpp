#include "AppList.h"

#include "App/App.h"
#include "App/Configs/Version.h"
#include "HAL/HAL.h"
using namespace Page;
uint64_t enter_time=0;
		extern uint8_t anim_img_select;

AppList::AppList()
{
}

AppList::~AppList()
{

}

void AppList::onCustomAttrConfig()
{

}

void AppList::onViewLoad()
{
	enter_time = millis();

	Model.Init();
	View.Create(root);
	AttachEvent(root);

#define APPLIST_DEF(className)\
	AttachEvent(View.ui.className.icon);
#include "_APPLIST_DEF.inc"
#undef APPLIST_DEF
}

void AppList::onViewDidLoad()
{
	// App_Remove("Startup");
	// lv_group_set_default(g);
}

void AppList::onViewWillAppear()
{
	lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), View.ui.group);
	StatusBar::SetStyle(StatusBar::STYLE_TRANSP);

	timer = lv_timer_create(onTimerUpdate, 100, this);
	lv_timer_ready(timer);

	View.SetScrollToY(root, -LV_VER_RES, LV_ANIM_OFF);
	lv_obj_fade_in(root, 300, 0);
}

void AppList::onViewDidAppear()
{
	View.onFocus(View.ui.group);
}

void AppList::onViewWillDisappear()
{
	lv_obj_fade_out(root, 300, 0);
}

void AppList::onViewDidDisappear()
{
	lv_timer_del(timer);
}

void AppList::onViewDidUnload()
{
	View.Delete();
	Model.Deinit();
}

void AppList::AttachEvent(lv_obj_t* obj)
{
	lv_obj_set_user_data(obj, this);
	lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void AppList::Update()
{
    // static uint8_t auto_entered_weather_flag=0;
	// if( 
	// 	// HAL::config.auto_enter_weather&&
    //     HAL::enc_btn_first_push_flag==0&&
    //     auto_entered_weather_flag==0&&
    //     millis()-enter_time>= 1000*HAL::config.auto_enter_weather_delay_sec)
    // {
    //     auto_entered_weather_flag=1;
    //     Manager->Push("Pages/Weather");
    // }
}

void AppList::onTimerUpdate(lv_timer_t* timer)
{
	AppList* instance = (AppList*)timer->user_data;

	instance->Update();
}

void AppList::onEvent(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (AppList*)lv_obj_get_user_data(obj);
	// extern uint8_t game_select_index;
	if (code == LV_EVENT_RELEASED)
	{
		//tools
		if (obj == instance->View.ui.cubedemo.icon)
		{
			instance->Manager->Push("Pages/Scene3D");
		}
		else if (obj == instance->View.ui.terminal.icon)
		{
			instance->Manager->Push("Pages/Terminal");
		}else if (obj == instance->View.ui.sysinfo.icon)
		{
			instance->Manager->Push("Pages/SystemInfos");
		}
		else if (obj == instance->View.ui.settings.icon)
		{
			instance->Manager->Push("Pages/Settings");
		}
		// weather
		else if (obj == instance->View.ui.weather.icon)
		{
			anim_img_select = 1;
			instance->Manager->Push("Pages/Weather");
		}
		//clock
		else if (obj == instance->View.ui.clock.icon)
		{
			instance->Manager->Push("Pages/Clock");
		}
		//heart
		else if (obj == instance->View.ui.heart.icon)
		{
			instance->Manager->Push("Pages/HeartBeat");
		}
		// game
		else if (obj == instance->View.ui.gametetris.icon)
		{
			instance->Manager->Push("Pages/Russian");
		}
		else if (obj == instance->View.ui.gamecat.icon)
		{
			HAL::game_select_index = 0;
			instance->Manager->Push("Pages/Games");
		}
		else if (obj == instance->View.ui.game3d.icon)
		{
			HAL::game_select_index = 1;
			instance->Manager->Push("Pages/Games");
		}
	}
	// else if (code == LV_EVENT_LONG_PRESSED)
	// {
	// 	instance->Manager->Pop();
	// }
}
