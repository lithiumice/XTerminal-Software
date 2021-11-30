#include <HAL/HAL.h>
#include "Template.h"
#include <map>
#include "stdio.h"
using namespace Page;
extern std::map<String, int> weather_str2int;
extern String* ari_level_int2str;
extern HAL::Weather_Info_t weaInfo;
extern HAL::TimeStamp_t time_stamp_info;
extern HAL::Time_str_t time_info;

Template::Template()
{
}

Template::~Template()
{

}

void Template::onCustomAttrConfig()
{
	SetCustomCacheEnable(true);
	SetCustomLoadAnimType(PageManager::LOAD_ANIM_OVER_BOTTOM, 500, lv_anim_path_bounce);
}

void Template::onViewLoad()
{
	Model.Init();
	View.Create(root);
	AttachEvent(root);
}

void Template::onViewDidLoad()
{

}

void Template::onViewWillAppear()
{
	lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), View.ui.group);

	StatusBar::SetStyle(StatusBar::STYLE_TRANSP);

	lv_obj_set_style_bg_color(root, lv_color_white(), LV_PART_MAIN);

	timer = lv_timer_create(onTimerUpdate, 10, this);
	lv_timer_ready(timer);

	lv_obj_fade_in(root, 600, 0);

	//info init
    updateWeather();
#ifdef ARDUINO
	HAL::parseTimeStamp(HAL::getTimestampLocal());
#endif
    updateClockInfo();
}

void Template::onViewDidAppear()
{
	// lv_group_focus_obj(View.ui.cityLabel);
}

void Template::onViewWillDisappear()
{
	lv_obj_fade_out(root, 800, 0);
}

void Template::onViewDidDisappear()
{
	lv_timer_del(timer);
}

void Template::onViewDidUnload()
{
	View.Delete();
	Model.DeInit();
}

void Template::AttachEvent(lv_obj_t* obj)
{
	lv_obj_set_user_data(obj, this);
	lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Template::updateSeconds()
{
	time_info.second++;
	if (time_info.second >= 60)//60s
	{
        time_info.second=0;

#ifdef ARDUINO
		HAL::parseTimeStamp(HAL::getTimestampLocal());
#endif
        updateClockInfo();
	}
	View.SetClockSec(time_info.second);

}

void Template::updateSpaceImg()
{
	static int _spaceIndex = 0;
	lv_img_set_src(View.ui.spaceImg, View.manImage_map[_spaceIndex]);
	_spaceIndex = (_spaceIndex + 1) % 10;

}

void Template::updateClockInfo()
{
	View.SetClockDay(time_info.month, time_info.day);
	View.SetClockHour(time_info.hour, time_info.minute);
}

void Template::updateWeather()
{
	View.SetWeather(weaInfo.weather);
	View.SetAirLevel(weaInfo.airQulity);
	View.SetCityName(weaInfo.cityname);
	View.SetHuminature(weaInfo.humidity);
	View.SetTemperatue(weaInfo.temperature);
	View.SetTextInfo(
		weaInfo.minTemp,
		weaInfo.maxTmep,
		weaInfo.windDir,
		weaInfo.windLevel
	);
}

void Template::Update()
{
	__IntervalExecute(updateSpaceImg(), 60);
	__IntervalExecute(updateSeconds(), 1000);
	__IntervalExecute(updateWeather(), 1000*5);

	if(HAL::config.weather_url_get_sucess_flag)
    {
        HAL::config.weather_url_get_sucess_flag=0;
        updateWeather();
    }

    if(HAL::config.clock_url_get_sucess_flag)
    {
        HAL::config.clock_url_get_sucess_flag=0;
        updateClockInfo();
        updateSeconds();
    }
	// static int count = 0;
	// count++;//10ms
	//
	// if (count % 3 == 0)//60ms
	// {
	// 	updateSpaceImg();
	// }
	// if (count % (100 * 5) == 0)//5s
	// {
	// 	updateWeather();
	// 	updateClockInfo();
	// }



}

void Template::onTimerUpdate(lv_timer_t* timer)
{
	Template* instance = (Template*)timer->user_data;
	instance->Update();
}

// int Template::onWeatherEvent(Account* account, Account::EventParam_t* param)
// {
// 	Template* instance = (Template*)param->data_p;
//
// 	HAL::Weather_Info_t info;
// 	instance->Model.GetWeatherInfo(&info);
// 	instance->View.SetAirLevel(info.airQulity);
// 	instance->View.SetCityName(info.cityname);
// 	instance->View.SetHuminature(info.humidity);
// 	instance->View.SetTemperatue(info.temperature);
// 	instance->View.SetTextInfo(info.minTemp, info.maxTmep, info.windDir, info.windLevel);
//
// 	return 0;
// }
//
// int Template::onClockEvent(Account* account, Account::EventParam_t* param)
// {
// 	Template* instance = (Template*)param->data_p;
//
// 	View.SetClockDay(11, 27);
// 	View.SetClockHour(12, 10);
// 	View.SetClockSec(33);
// 	return 0;
// }

void Template::onEvent(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (Template*)lv_obj_get_user_data(obj);

	if (code == LV_EVENT_RELEASED)
	{
		instance->Manager->Pop();
	}
	else if (code == LV_EVENT_LONG_PRESSED)
	{
		instance->Manager->Pop();
	}
}
