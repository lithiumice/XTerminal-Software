#ifndef __TEMPLATE_PRESENTER_H
#define __TEMPLATE_PRESENTER_H

#include "TemplateView.h"
#include "TemplateModel.h"

namespace Page
{

class Template : public PageBase
{
public:
    typedef struct
    {
        uint16_t time;
        lv_color_t color;
    } Param_t;

public:
    Template();
    virtual ~Template();

    virtual void onCustomAttrConfig();
    virtual void onViewLoad();
    virtual void onViewDidLoad();
    virtual void onViewWillAppear();
    virtual void onViewDidAppear();
    virtual void onViewWillDisappear();
    virtual void onViewDidDisappear();
    virtual void onViewDidUnload();

private:
    void updateWeather();
    void Update();
    void AttachEvent(lv_obj_t* obj);
    void updateSpaceImg();
    static void onTimerUpdate(lv_timer_t* timer);
    static int onWeatherEvent(Account* account, Account::EventParam_t* param);
    int onClockEvent(Account* account, Account::EventParam_t* param);
    static void onEvent(lv_event_t* event);

private:
    TemplateView View;
    TemplateModel Model;
    lv_timer_t* timer;
};

}

#endif
