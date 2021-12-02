#include <HAL/HAL.h>
#include "Weather.h"
#include <map>
#include "stdio.h"

using namespace Page;
extern std::map<String, int> weather_str2int;
extern String *ari_level_int2str;
extern HAL::Weather_Info_t weaInfo;
extern HAL::TimeStamp_t time_stamp_info;
extern HAL::Time_str_t time_info;

Weather::Weather() {
}

Weather::~Weather() {

}

void Weather::onCustomAttrConfig() {
    SetCustomCacheEnable(true);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_OVER_BOTTOM, 500, lv_anim_path_bounce);
}

void Weather::onViewLoad() {
    Model.Init();
    View.Create(root);
    AttachEvent(root);
}

void Weather::onViewDidLoad() {

}

void Weather::onViewWillAppear() {
    lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), View.ui.group);

    StatusBar::SetStyle(StatusBar::STYLE_TRANSP);

    lv_obj_set_style_bg_color(root, lv_color_white(), LV_PART_MAIN);

    timer = lv_timer_create(onTimerUpdate, 10, this);
    lv_timer_ready(timer);

    lv_obj_fade_in(root, 600, 0);

    //info init
    notifyUrlThread();
    updateWeather();
#ifdef ARDUINO
    HAL::parseTimeStamp(HAL::getTimestampLocal());
#endif
    updateClockInfo();
}

void Weather::onViewDidAppear() {
    // lv_group_focus_obj(View.ui.cityLabel);
}

void Weather::onViewWillDisappear() {
    lv_obj_fade_out(root, 800, 0);
}

void Weather::onViewDidDisappear() {
    lv_timer_del(timer);
}

void Weather::onViewDidUnload() {
    View.Delete();
    Model.DeInit();
}

void Weather::AttachEvent(lv_obj_t *obj) {
    lv_obj_set_user_data(obj, this);
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Weather::updateSeconds() {
    time_info.second++;
    if (time_info.second >= 60)//60s
    {
        time_info.second = 0;

#ifdef ARDUINO
        HAL::parseTimeStamp(HAL::getTimestampLocal());
#endif
        updateClockInfo();
    }
    View.SetClockSec(time_info.second);

}

void Weather::updateSpaceImg() {
    // static int _spaceIndex = 0;
    // lv_img_set_src(View.ui.spaceImg, View.manImage_map[_spaceIndex]);
    // _spaceIndex = (_spaceIndex + 1) % 10;

    static int _spaceIndex = 0;
    lv_img_set_src(View.ui.spaceImg, View.carImage_map[_spaceIndex]);
    _spaceIndex = (_spaceIndex + 1) % 20;

}

void Weather::updateClockInfo() {
    View.SetClockDay(time_info.month, time_info.day);
    View.SetClockHour(time_info.hour, time_info.minute);
}

void Weather::updateWeather() {
    View.SetWeather(weaInfo.weather);
    View.SetAirLevel(weaInfo.airQulity);
    View.SetCityName(weaInfo.cityname);
    View.SetHuminature(weaInfo.humidity);
    View.SetTemperatue(weaInfo.temperature);
    View.SetTextInfo(weaInfo.minTemp, weaInfo.maxTmep, weaInfo.windDir, weaInfo.windLevel);
}



void Weather::Update() {
    __IntervalExecute(updateSpaceImg(), 30);
    // __IntervalExecute(updateSpaceImg(), 60);
    __IntervalExecute(updateSeconds(), 1000);
    __IntervalExecute(updateWeather(), 1000 * 5);
    __IntervalExecute(notifyUrlThread(), (1000 * 60 * HAL::config.update_weather_interval_minute));

    if (HAL::config.weather_url_get_sucess_flag) {
        HAL::config.weather_url_get_sucess_flag = 0;
        updateWeather();
    }

    if (HAL::config.clock_url_get_sucess_flag) {
        HAL::config.clock_url_get_sucess_flag = 0;
        updateClockInfo();
        updateSeconds();
    }
}

void Weather::onTimerUpdate(lv_timer_t *timer) {
    Weather *instance = (Weather *) timer->user_data;
    instance->Update();
}

void Weather::onEvent(lv_event_t *event) {
    lv_obj_t *obj = lv_event_get_target(event);
    lv_event_code_t code = lv_event_get_code(event);
    auto *instance = (Weather *) lv_obj_get_user_data(obj);

    if (code == LV_EVENT_RELEASED) {
        instance->Manager->Pop();
    } else if (code == LV_EVENT_LONG_PRESSED) {
        instance->Manager->Pop();
    }
}
