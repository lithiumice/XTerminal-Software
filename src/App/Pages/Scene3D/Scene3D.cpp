#include "Scene3D.h"

#include "HAL/HAL.h"

#ifdef ARDUINO
#include "Arduino.h"
#endif

using namespace Page;

Scene3D::Scene3D()
{
}

Scene3D::~Scene3D()
{

}

void Scene3D::onCustomAttrConfig()
{
    SetCustomCacheEnable(true);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_OVER_BOTTOM, 500, lv_anim_path_bounce);
}

void Scene3D::onViewLoad()
{
    Model.Init();
    View.Create(root);

    AttachEvent(root);
    AttachEvent(View.ui.canvas);
    AttachEvent(View.ui.labelTitle);
}

void Scene3D::onViewDidLoad()
{
    lv_obj_set_user_data(View.ui.canvas, this);
    lv_obj_add_event_cb(View.ui.canvas, onEvent, LV_EVENT_VALUE_CHANGED, this);
}

void Scene3D::onViewWillAppear()
{
    lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), View.ui.group);

    timer = lv_timer_create(onTimerUpdate, 20, this);
    lv_timer_ready(timer);
    lv_group_focus_obj(View.ui.canvas);

}

void Scene3D::onViewDidAppear()
{
    lv_indev_set_cursor(lv_get_indev(LV_INDEV_TYPE_ENCODER), View.ui.canvas);
}

void Scene3D::onViewWillDisappear()
{
    lv_timer_del(timer);
}

void Scene3D::onViewDidDisappear()
{
}

void Scene3D::onViewDidUnload()
{
    
    View.Delete();
}

void Scene3D::AttachEvent(lv_obj_t* obj)
{
    lv_obj_set_user_data(obj, this);
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Scene3D::Update()
{
     Model.Update(View.ui.canvas);
}

void Scene3D::onTimerUpdate(lv_timer_t* timer)
{
    Scene3D* instance = (Scene3D*) timer->user_data;
    instance->Update();
}

void Scene3D::onEvent(lv_event_t* event)
{
    lv_obj_t* obj = lv_event_get_target(event);
    lv_event_code_t code = lv_event_get_code(event);
    auto* instance = (Scene3D*) lv_obj_get_user_data(obj);
    
    if (code == LV_EVENT_PRESSED)
    {
        HAL::TerminalPrintln("Scene3D LV_EVENT_PRESSED");
        instance->Manager->BackHome();
        // instance->Manager->Pop();
    }
    else if (code == LV_EVENT_LONG_PRESSED)
    {
        HAL::TerminalPrintln("Scene3D LV_EVENT_LONG_PRESSED");
        instance->Manager->Pop();
    }
}
