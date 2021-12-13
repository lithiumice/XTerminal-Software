#ifndef __Clock_PRESENTER_H
#define __Clock_PRESENTER_H

#include "App/Utils/PageManager/PageBase.h"
#include "HAL/HAL.h"
#include "../Page.h"
#include "lvgl.h"

namespace Page
{

class Clock : public PageBase
{
public:
    struct
    {
        lv_obj_t* clockLabel_1;
        lv_obj_t* clockLabel_2;
        lv_group_t* group;
    } ui;

    Clock();
    virtual ~Clock();

    virtual void onCustomAttrConfig();
    virtual void onViewLoad();
    virtual void onViewDidLoad();
    virtual void onViewWillAppear();
    virtual void onViewDidAppear();
    virtual void onViewWillDisappear();
    virtual void onViewDidDisappear();
    virtual void onViewDidUnload();

    void ArcValueCtrl(int step);
    void GUICreate();
    void updateSeconds();

private:
    void Update();
    void AttachEvent(lv_obj_t* obj);
    static void onTimerUpdate(lv_timer_t* timer);
    static void onEvent(lv_event_t* event);
    

private:
    lv_timer_t* timer;
};

}

#endif
