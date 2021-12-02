#include "Games.h"
#include <ctime>
#include "HAL/HAL.h"
#include   <stdio.h>
#include  <stdlib.h>
using namespace Page;
#include "Game/GamePrivate.h"
#include "Game/GameDisp.h"
#include "lvgl.h"

#define GAME_DISP_WIDTH  128
#define GAME_DISP_HEIGHT 64
#define BTN_SIZE 48

extern void chribocchi_loop();
extern void chribocchi_setup();
extern void GameArduboy3D_setup();
extern void GameArduboy3D_loop();

uint8_t game_select_index = 1;

static lv_obj_t* contGameDisp;
static lv_obj_t* canvasGame;
static lv_color_t canvasBuf[LV_CANVAS_BUF_SIZE_INDEXED_1BIT(GAME_DISP_WIDTH, GAME_DISP_HEIGHT)];
static lv_obj_t* contBtnGrp;
static lv_obj_t* btnGrp[GAME_BUTTON_MAX];

static BtnCfg_TypeDef btnCfg[GAME_BUTTON_MAX] =
{
    {"A",  LV_ALIGN_BOTTOM_RIGHT, -20, -90},
    {"B",  LV_ALIGN_BOTTOM_RIGHT, -20, -30},
    {"U",  LV_ALIGN_TOP_MID,        0,  5},
    {"D",  LV_ALIGN_BOTTOM_MID,     0,  -5},
    {"L",  LV_ALIGN_LEFT_MID,       5,  0},
    {"R",  LV_ALIGN_RIGHT_MID,     -5,  0},
};
static LED_TypeDef ledGrp[GAME_LED_MAX] = 
{
    {lv_palette_main(LV_PALETTE_RED),   -20},
    {lv_palette_main(LV_PALETTE_GREEN), -0},
    {lv_palette_main(LV_PALETTE_BLUE),   20},
};

static void ButtonGrp_EventHandler(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (lv_obj_t*)lv_obj_get_user_data(obj);

    for(int i = 0; i < GAME_BUTTON_MAX; i++)
    {
        if(obj == btnGrp[i])
        {
            if(code == LV_EVENT_PRESSED)
            {
                Game_SetButtonState(i, true);
            }
            else if(code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST)
            {
                Game_SetButtonState(i, false);
            }
        }
    }
}

static void ButtonGrp_Create(lv_obj_t* par)
{
    lv_obj_t* contBtn = lv_obj_create(par);
    lv_obj_set_size(contBtn, BTN_SIZE * 3 + 10, BTN_SIZE * 3 + 10);
    lv_obj_set_style_radius(contBtn,  5,0);
    lv_obj_set_style_border_width(contBtn, 0, 0);
    lv_obj_align(contBtn, LV_ALIGN_BOTTOM_LEFT, 10, -5);
    lv_obj_set_style_bg_color(contBtn, lv_color_black(), 0);
    contBtnGrp = contBtn;

    for (int i = 0; i < GAME_BUTTON_MAX; i++)
    {
        lv_obj_t* btn = lv_btn_create((i >= GAME_BUTTON_UP) ? contBtn : par);
        lv_obj_set_size(btn, BTN_SIZE, BTN_SIZE);
        lv_obj_align(btn, btnCfg[i].align, btnCfg[i].x_mod, btnCfg[i].y_mod);
        lv_obj_set_style_bg_color(btn, lv_color_black(),0);
        lv_obj_set_style_radius(btn, 0,0);
        lv_obj_set_style_border_color(btn,  lv_color_black(),0);
        lv_obj_set_style_border_width(btn,  0,0);
        lv_obj_add_event_cb(btn, ButtonGrp_EventHandler,LV_EVENT_ALL,NULL);

        lv_obj_t* label = lv_label_create(btn);
        lv_label_set_text(label, btnCfg[i].sym);
        lv_obj_set_style_text_color(label,lv_color_white(),0);
        lv_obj_set_opa_scale(btn, LV_OPA_TRANSP);
        
        btnGrp[i] = btn;
    }
}

static void CanvasGame_Create(lv_obj_t* par)
{
    lv_obj_t* canvas = lv_canvas_create(par);
    lv_canvas_set_buffer(canvas, canvasBuf, GAME_DISP_WIDTH, GAME_DISP_HEIGHT, LV_IMG_CF_INDEXED_1BIT);
    lv_canvas_set_palette(canvas, 0, lv_color_black());
    lv_canvas_set_palette(canvas, 1, lv_color_white());
    lv_obj_align(canvas, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(canvas, LV_OBJ_FLAG_SCROLLABLE);
    canvasGame = canvas;
}

void CanvasGame_DrawBuf(uint8_t* buf, int16_t w, int16_t h)
{
#   define IsWhite ((buf[(row*w) + x] & (1 << bit_position)) >> bit_position)
    lv_color_t c;
    lv_obj_t* canvas = canvasGame;
	lv_img_dsc_t *dsc= lv_canvas_get_img(canvas);

	for (int16_t y = 0; y < h; y++)
    {
        for (int16_t x = 0; x < w; x++)
        {
            uint8_t row = y / 8;
            uint8_t bit_position = y % 8;
            c.full = IsWhite;
            lv_img_buf_set_px_color(dsc, x, y, c);
        }
    }

    lv_obj_invalidate(canvas);
}

static void ContGameDisp_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_obj_create(par);
    lv_obj_set_size(cont, GAME_DISP_WIDTH + 4, GAME_DISP_HEIGHT + 4);
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 10);
	lv_obj_set_style_border_color(cont, LV_COLOR_MAKE(0x33, 0x99, 0x99), 0);
	contGameDisp = cont;
}

static void LED_Create(lv_obj_t* par)
{
    for (int i = 0; i < GAME_LED_MAX; i++)
    {
        lv_obj_t* led = lv_led_create(par);
        lv_obj_set_style_bg_color(led, ledGrp[i].color,0);
        lv_obj_set_style_border_width(led, 0, 0);
        lv_obj_set_size(led, 10, 10);
        lv_obj_set_opa_scale(led, LV_OPA_TRANSP);
        lv_obj_align(led, LV_ALIGN_LEFT_MID,ledGrp[i].x_ofs, -10);
        lv_led_off(led);
        ledGrp[i].led = led;
    }
}

void Game_SetLEDState(uint8_t led_id, bool val)
{
    if(led_id < GAME_LED_MAX && ledGrp[led_id].led != NULL)
    {
        lv_obj_t* led = ledGrp[led_id].led;
        val ? lv_led_on(led) : lv_led_off(led);
    }
}

void Game_SetLEDBright(uint8_t led_id, uint8_t val)
{
    if(led_id < GAME_LED_MAX && ledGrp[led_id].led != NULL)
    {
        lv_obj_t* led = ledGrp[led_id].led; 
        lv_led_set_brightness(led, val);
    }
}

Games::Games()
{
}

Games::~Games()
{

}

void Games::onCustomAttrConfig()
{
	SetCustomCacheEnable(true);
	SetCustomLoadAnimType(PageManager::LOAD_ANIM_OVER_BOTTOM, 500, lv_anim_path_bounce);
}


void Games::onViewLoad()
{
	// View.Create(root);
	lv_obj_remove_style_all(root);
	lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES);
	StatusBar::SetStyle(StatusBar::STYLE_BLACK);

    if(game_select_index==0)
	chribocchi_setup();
    else if (game_select_index == 1)
        GameArduboy3D_setup();

	ContGameDisp_Create(root);
	LED_Create(root);
    CanvasGame_Create(contGameDisp);
    ButtonGrp_Create(root);
}

void Games::onViewDidLoad()
{

}

void Games::onViewWillAppear()
{
	lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), ui.group);
	StatusBar::SetStyle(StatusBar::STYLE_TRANSP);

	lv_obj_set_style_bg_color(root, lv_color_white(), LV_PART_MAIN);

	timer = lv_timer_create(onTimerUpdate, 10, this);
	lv_timer_ready(timer);

	lv_obj_fade_in(root, 300, 0);
}

void Games::onViewDidAppear()
{
	// lv_group_focus_obj(View.ui.canvas);
}

void Games::onViewWillDisappear()
{
	lv_obj_fade_out(root, 300, 0);
}

void Games::onViewDidDisappear()
{
	lv_timer_del(timer);
}

void Games::onViewDidUnload()
{
	// View.Delete();
	// Model.DeInit();
}

void Games::AttachEvent(lv_obj_t* obj)
{
	lv_obj_set_user_data(obj, this);
	lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Games::Update()
{
    if (game_select_index == 0)
        chribocchi_loop();
    else if (game_select_index == 1)
        GameArduboy3D_loop();

	Game_DispTaskUpdate();
}

void Games::onTimerUpdate(lv_timer_t* timer)
{
	Games* instance = (Games*)timer->user_data;
	instance->Update();
}

void Games::onEvent(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (Games*)lv_obj_get_user_data(obj);

	// if (code == LV_EVENT_RELEASED)
	// {
	// 	HAL::TerminalPrintln("Games LV_EVENT_PRESSED");
	// 	instance->Manager->Pop();
	// }
	// else if (
	// 	code == LV_EVENT_READY ||
	// 	code == LV_EVENT_LONG_PRESSED ||
	// 	code == LV_EVENT_CLICKED ||
	// 	code == LV_EVENT_FOCUSED
	// 	) {

	// 	instance->Manager->Pop();
	// }
	
	// if (code == LV_GESTURE_DIR_LEFT || code == LV_GESTURE_DIR_RIGHT)
	// {
	// 	instance->Manager->Pop();
	// }
}
