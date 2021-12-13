#include "HeartBeat.h"
#include <ctime>
#include "Arduino.h"
#include "HAL/HAL.h"
#include <stdio.h>
#include <stdlib.h>


using namespace Page;

HeartBeat::HeartBeat()
{
}

HeartBeat::~HeartBeat()
{
}

void HeartBeat::onCustomAttrConfig()
{
	SetCustomCacheEnable(true);
	SetCustomLoadAnimType(PageManager::LOAD_ANIM_OVER_BOTTOM, 500, lv_anim_path_bounce);
}
static void anim_zoom_cb(void * var, int32_t v)
{
    lv_img_set_zoom((lv_obj_t *)var, v);
}

static void draw_event_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);

    /*Add the faded area before the lines are drawn*/
    lv_obj_draw_part_dsc_t * dsc = lv_event_get_draw_part_dsc(e);
    if(dsc->part == LV_PART_ITEMS) {
        if(!dsc->p1 || !dsc->p2) return;

        /*Add a line mask that keeps the area below the line*/
        lv_draw_mask_line_param_t line_mask_param;
        lv_draw_mask_line_points_init(&line_mask_param, dsc->p1->x, dsc->p1->y, dsc->p2->x, dsc->p2->y, LV_DRAW_MASK_LINE_SIDE_BOTTOM);
        int16_t line_mask_id = lv_draw_mask_add(&line_mask_param, NULL);

        /*Add a fade effect: transparent bottom covering top*/
        lv_coord_t h = lv_obj_get_height(obj);
        lv_draw_mask_fade_param_t fade_mask_param;
        lv_draw_mask_fade_init(&fade_mask_param, &obj->coords, LV_OPA_COVER, obj->coords.y1 + h / 8, LV_OPA_TRANSP,obj->coords.y2);
        int16_t fade_mask_id = lv_draw_mask_add(&fade_mask_param, NULL);

        /*Draw a rectangle that will be affected by the mask*/
        lv_draw_rect_dsc_t draw_rect_dsc;
        lv_draw_rect_dsc_init(&draw_rect_dsc);
        draw_rect_dsc.bg_opa = LV_OPA_20;
        draw_rect_dsc.bg_color = dsc->line_dsc->color;

        lv_area_t a;
        a.x1 = dsc->p1->x;
        a.x2 = dsc->p2->x - 1;
        a.y1 = LV_MIN(dsc->p1->y, dsc->p2->y);
        a.y2 = obj->coords.y2;
        lv_draw_rect(&a, dsc->clip_area, &draw_rect_dsc);

        /*Remove the masks*/
        lv_draw_mask_free_param(&line_mask_param);
        lv_draw_mask_free_param(&fade_mask_param);
        lv_draw_mask_remove_id(line_mask_id);
        lv_draw_mask_remove_id(fade_mask_id);
    }
    /*Hook the division lines too*/
    else if(dsc->part == LV_PART_MAIN) {
        if(dsc->line_dsc == NULL || dsc->p1 == NULL || dsc->p2 == NULL) return;

        /*Vertical line*/
        if(dsc->p1->x == dsc->p2->x) {
            dsc->line_dsc->color  = lv_palette_lighten(LV_PALETTE_GREY, 1);
            if(dsc->id == 3) {
                dsc->line_dsc->width  = 2;
                dsc->line_dsc->dash_gap  = 0;
                dsc->line_dsc->dash_width  = 0;
            }
            else {
                dsc->line_dsc->width = 1;
                dsc->line_dsc->dash_gap  = 6;
                dsc->line_dsc->dash_width  = 6;
            }
        }
        /*Horizontal line*/
        else {
            if(dsc->id == 2) {
                dsc->line_dsc->width  = 2;
                dsc->line_dsc->dash_gap  = 0;
                dsc->line_dsc->dash_width  = 0;
            }
            else {
                dsc->line_dsc->width = 2;
                dsc->line_dsc->dash_gap  = 6;
                dsc->line_dsc->dash_width  = 6;
            }

            if(dsc->id == 1  || dsc->id == 3) {
                dsc->line_dsc->color  = lv_palette_main(LV_PALETTE_GREEN);
            } else {
                dsc->line_dsc->color  = lv_palette_lighten(LV_PALETTE_GREY, 1);
            }
        }
    }
}
void HeartBeat::GUICreate()
{
	ui.rate_text = lv_label_create(root);
	lv_label_set_recolor(ui.rate_text, true);
	lv_label_set_text_fmt(ui.rate_text, "#ffa500 %02d# rpm", 52);
	lv_obj_set_style_text_font(ui.rate_text, &lv_font_montserrat_28, 0);
	// lv_obj_set_style_text_font(ui.rate_text, Resource.GetFont("EuroStar_60"), 0);
	lv_obj_set_style_text_color(ui.rate_text, lv_color_white(), 0);
	lv_obj_align(ui.rate_text, LV_ALIGN_TOP_RIGHT, -40, 30);

	lv_obj_t* img = lv_img_create(root);
    lv_img_set_src(img,Resource.GetImage("red_heart"));
    lv_obj_align(img,LV_ALIGN_TOP_LEFT,30,30);
	lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, img);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_img_set_zoom);
    lv_anim_set_values(&a, LV_IMG_ZOOM_NONE, 160);
    lv_anim_set_time(&a, 200);
    lv_anim_set_playback_time(&a, 200);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&a, 500);
    lv_anim_start(&a);
	lv_anim_set_exec_cb(&a, anim_zoom_cb);
	lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
	lv_anim_start(&a);

	static lv_style_t sytle_chart;
	lv_style_init(&sytle_chart);
	lv_style_set_border_width(&sytle_chart, 0);
	lv_style_set_bg_color(&sytle_chart, lv_color_black());

	ui.rate_chart = lv_chart_create(root);
	// lv_chart_set_type(ui.rate_chart, LV_CHART_TYPE_BAR);
	lv_obj_set_size(ui.rate_chart, 240, 120);
	// lv_obj_set_size(ui.rate_chart, LV_HOR_RES - 80, 150);
	lv_obj_add_style(ui.rate_chart, &sytle_chart, 0);
	lv_chart_set_update_mode(ui.rate_chart, LV_CHART_UPDATE_MODE_SHIFT);
	lv_chart_set_point_count(ui.rate_chart, 100);
	// int tmp = lv_obj_get_width(ui.rate_chart);
	// lv_chart_set_point_count(ui.rate_chart, 10);
	lv_obj_align(ui.rate_chart, LV_ALIGN_BOTTOM_MID, 15, -30);
	lv_obj_add_event_cb(ui.rate_chart, draw_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
	lv_obj_set_style_bg_color(ui.rate_chart, lv_palette_main(LV_PALETTE_RED), LV_PART_ITEMS);
	// lv_obj_set_style_width(ui.rate_chart,2,LV_PART_ITEMS);
	// lv_obj_set_style_bg_color(ui.rate_chart, lv_palette_lighten(LV_PALETTE_RED,50), LV_PART_INDICATOR  );
	lv_chart_set_div_line_count(ui.rate_chart, 4, 3);
	lv_obj_set_style_size(ui.rate_chart, 0, LV_PART_INDICATOR);
	lv_chart_set_range(ui.rate_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
	lv_chart_set_range(ui.rate_chart, LV_CHART_AXIS_SECONDARY_Y, 0, 400);
	lv_chart_set_axis_tick(ui.rate_chart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 12, 3, false, 40);
	lv_chart_set_axis_tick(ui.rate_chart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 6, 2, true, 50);
	// lv_chart_set_axis_tick(ui.rate_chart, LV_CHART_AXIS_SECONDARY_Y, 10, 5, 3, 4, true, 50);

	ui.rate_ser = lv_chart_add_series(ui.rate_chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
	// lv_chart_init_points(chartAlt, serAlt, (int16_t)BMP180.altitude);
	lv_chart_set_all_value(ui.rate_chart, ui.rate_ser, 0);

	// lv_chart_series_t *ser2 = lv_chart_add_series(ui.rate_chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_SECONDARY_Y);
	// /*Directly set points on 'ser2'*/
	// ui.rate_ser->y_points[0] = 90;
	// ui.rate_ser->y_points[1] = 70;
	// ui.rate_ser->y_points[2] = 65;
	// ui.rate_ser->y_points[3] = 65;
	// ui.rate_ser->y_points[4] = 65;
	// ui.rate_ser->y_points[5] = 65;
	// ui.rate_ser->y_points[6] = 65;
	// ui.rate_ser->y_points[7] = 65;
	// ui.rate_ser->y_points[8] = 65;
	// ui.rate_ser->y_points[9] = 65;
	// lv_chart_refresh(ui.rate_chart); /*Required after direct set*/
}

void HeartBeat::onViewLoad()
{

	lv_obj_remove_style_all(root);
	lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES);
	StatusBar::SetStyle(StatusBar::STYLE_BLACK);
	ui.group = lv_group_create();

	GUICreate();
	lv_group_add_obj(ui.group, root);
	AttachEvent(root);
}

void HeartBeat::onViewDidLoad()
{
}

void HeartBeat::onViewWillAppear()
{
	lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), ui.group);
	StatusBar::SetStyle(StatusBar::STYLE_TRANSP);

	lv_obj_set_style_bg_color(root, lv_color_white(), LV_PART_MAIN);

	timer = lv_timer_create(onTimerUpdate, 100, this);
	lv_timer_ready(timer);

	lv_obj_fade_in(root, 300, 0);
}

void HeartBeat::onViewDidAppear()
{
}

void HeartBeat::onViewWillDisappear()
{
	lv_obj_fade_out(root, 300, 0);
}

void HeartBeat::onViewDidDisappear()
{
	lv_timer_del(timer);
}

void HeartBeat::onViewDidUnload()
{
	// View.Delete();
	// Model.DeInit();
}

void HeartBeat::AttachEvent(lv_obj_t *obj)
{
	lv_obj_set_user_data(obj, this);
	lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void HeartBeat::Update()
{
	static int index = 0;
	index+=5;
#define PI 3.14159265
	lv_chart_set_next_value(ui.rate_chart, ui.rate_ser, 40*sin(index*PI / 180)+40);
}

void HeartBeat::onTimerUpdate(lv_timer_t *timer)
{
	HeartBeat *instance = (HeartBeat *)timer->user_data;
	instance->Update();
}

void HeartBeat::onEvent(lv_event_t *event)
{
	lv_obj_t *obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto *instance = (HeartBeat *)lv_obj_get_user_data(obj);

	if (code == LV_EVENT_RELEASED)
	{
		instance->Manager->Pop();
	}
	else if (code == LV_EVENT_LONG_PRESSED)
	{

		instance->Manager->Pop();
	}
}
