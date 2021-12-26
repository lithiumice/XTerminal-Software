// #ifndef __MuiscPlayer_PRESENTER_H
// #define __MuiscPlayer_PRESENTER_H
#pragma once

#include "MuiscPlayerView.h"

namespace Page
{

class MuiscPlayer : public PageBase
{
public:

public:
    MuiscPlayer();
    virtual ~MuiscPlayer();

    virtual void onCustomAttrConfig();
    virtual void onViewLoad();
    virtual void onViewDidLoad();
    virtual void onViewWillAppear();
    virtual void onViewDidAppear();
    virtual void onViewWillDisappear();
    virtual void onViewDidDisappear();
    virtual void onViewDidUnload();

private:
    void Update();
    void AttachEvent(lv_obj_t* obj);
    static void onTimerUpdate(lv_timer_t* timer);
    static void onEvent(lv_event_t* event);

private:
    MuiscPlayerView View;
    lv_timer_t* timer;
};

}

// #endif
