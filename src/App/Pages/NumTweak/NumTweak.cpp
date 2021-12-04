#include "NumTweak.h"

#include <ctime>

#include "HAL/HAL.h"
#include   <stdio.h>
#include  <stdlib.h>
using namespace Page;


NumTweak::NumTweak()
{
}

NumTweak::~NumTweak()
{

}

void NumTweak::onCustomAttrConfig()
{
	SetCustomCacheEnable(true);
	SetCustomLoadAnimType(PageManager::LOAD_ANIM_OVER_BOTTOM, 500, lv_anim_path_bounce);
}
static void set_angle(void * obj, int32_t v)
{
    lv_arc_set_value((lv_obj_t*)obj, v);
}
void NumTweak::onViewLoad()
{
	// View.Create(root);
	lv_obj_remove_style_all(root);
	lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES);
	StatusBar::SetStyle(StatusBar::STYLE_BLACK);
	ui.group = lv_group_create();

	ui.arc = lv_arc_create(lv_scr_act());
	lv_obj_set_size(ui.arc, 150, 150);
	// lv_arc_set_rotation(ui.arc, 135);
	// lv_arc_set_bg_angles(ui.arc, 0, 270);
	lv_arc_set_rotation(ui.arc, 270);
  	lv_arc_set_bg_angles(ui.arc, 0, 360);
	lv_arc_set_value(ui.arc, 40);
	lv_obj_remove_style(ui.arc, NULL, LV_PART_KNOB);
	lv_obj_set_style_shadow_opa(ui.arc, LV_OPA_COVER,0);
	lv_obj_set_style_shadow_color(ui.arc, lv_palette_main(LV_PALETTE_RED),0);
	lv_obj_set_style_shadow_width(ui.arc, 10,0);
	lv_obj_set_style_bg_color(ui.arc, lv_palette_main(LV_PALETTE_RED), LV_PART_KNOB);
	lv_obj_set_style_bg_color(ui.arc, lv_palette_main(LV_PALETTE_GREY), LV_PART_INDICATOR);
	lv_obj_set_style_bg_color(ui.arc, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
	lv_obj_center(ui.arc);
	lv_group_add_obj(ui.group, ui.arc);

	lv_anim_t a;
	lv_anim_init(&a);
	lv_anim_set_var(&a, ui.arc);
	lv_anim_set_exec_cb(&a, set_angle);
	lv_anim_set_time(&a, 1000);
	lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);    /*Just for the demo*/
	lv_anim_set_repeat_delay(&a, 500);
	lv_anim_set_values(&a, 0, 100);
	lv_anim_start(&a);


	ui.indicateText = lv_label_create(root);
	lv_label_set_recolor(ui.indicateText, true);
	lv_obj_set_style_text_font(ui.indicateText, &lv_font_montserrat_26, 0);
	lv_obj_set_style_text_color(ui.indicateText, lv_color_white(), 0);
	lv_obj_align(ui.indicateText, LV_ALIGN_CENTER,0,0);
	lv_label_set_text(ui.indicateText, "40");
	
	
}

void NumTweak::onViewDidLoad()
{

}

void NumTweak::onViewWillAppear()
{
	lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), ui.group);
	StatusBar::SetStyle(StatusBar::STYLE_TRANSP);

	lv_obj_set_style_bg_color(root, lv_color_white(), LV_PART_MAIN);

	timer = lv_timer_create(onTimerUpdate, 500, this);
	lv_timer_ready(timer);

	lv_obj_fade_in(root, 300, 0);
}

void NumTweak::onViewDidAppear()
{
	// lv_group_focus_obj(View.ui.canvas);
}

void NumTweak::onViewWillDisappear()
{
	lv_obj_fade_out(root, 300, 0);
}

void NumTweak::onViewDidDisappear()
{
	lv_timer_del(timer);
}

void NumTweak::onViewDidUnload()
{
	// View.Delete();
	// Model.DeInit();
}

void NumTweak::AttachEvent(lv_obj_t* obj)
{
	lv_obj_set_user_data(obj, this);
	lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void NumTweak::Update()
{

}

void NumTweak::onTimerUpdate(lv_timer_t* timer)
{
	NumTweak* instance = (NumTweak*)timer->user_data;
	instance->Update();
}

void NumTweak::onEvent(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (NumTweak*)lv_obj_get_user_data(obj);

	if (code == LV_EVENT_RELEASED)
	{
		HAL::TerminalPrintln("NumTweak LV_EVENT_PRESSED");
		instance->Manager->Pop();
	}
	else if (
		code == LV_EVENT_READY ||
		code == LV_EVENT_LONG_PRESSED ||
		code == LV_EVENT_CLICKED ||
		code == LV_EVENT_FOCUSED
		) {

		instance->Manager->Pop();
	}
}
