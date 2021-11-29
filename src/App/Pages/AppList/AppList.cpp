#include "AppList.h"
#include "App/Configs/Version.h"

using namespace Page;

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
	Model.Init();
	View.Create(root);
	AttachEvent(root);

#define APPLIST_DEF(className)\
	AttachEvent(View.ui.className.icon);
	// AttachEvent(View.ui.##className##.icon);
#include "_APPLIST_DEF.inc"
#undef APPLIST_DEF
}

void AppList::onViewDidLoad()
{

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
		}else if (obj == instance->View.ui.pictures.icon)
		{
			instance->Manager->Push("Pages/Pictures");
		}
		else if (obj == instance->View.ui.settings.icon)
		{
			instance->Manager->Push("Pages/SystemInfos");
		}
		// none
		else if (obj == instance->View.ui.filemanager.icon)
		{
			instance->Manager->Push("Pages/Pictures");
		}
		else if (obj == instance->View.ui.fandisplay.icon)
		{
			instance->Manager->Push("Pages/SystemInfos");
		}
		else if (obj == instance->View.ui.videoplayer.icon)
		{
			instance->Manager->Push("Pages/Pictures");
		}
		//weather clock
		else if (obj == instance->View.ui.weather.icon)
		{
			instance->Manager->Push("Pages/Template");
		}
		else if (obj == instance->View.ui.clock.icon)
		{
			instance->Manager->Push("Pages/Template");
		}
	}
	else if (code == LV_EVENT_LONG_PRESSED)
	{
		instance->Manager->Pop();
	}
}