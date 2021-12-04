#include "Russian.h"

#include <ctime>

#include "HAL/HAL.h"
#include   <stdio.h>
#include  <stdlib.h>
using namespace Page;


#define CANVAS_WIDTH  100
#define CANVAS_HEIGHT  220
#define BLOCK_HEIGHT 22
#define BLOCK_WIDTH 10

#define KEY_LEFT 0
#define KEY_RIGHT 1
#define KEY_TURN 2
#define KEY_DOWN 3

static void* cbuf;
int i, j, N, T, F, J, X, Y, dx, dy,
KEY_V, Cache1, Cache2, NU, NI, RU, RI,
P_X, P_Y, POS_Y_MAX, LEVEL = 1, SCORE = 0,
P[4], POINT_V[12][22], MARK[21],
FLAG[5] = { 0,0,0,1,0 };
int TGM[7][4] = {
	{0x159D,0x89AB,0x159D,0x89AB},
	{0x126A,0x4856,0x159A,0x4526},
	{0x926A,0x456A,0x1592,0x0456},
	{0x4859,0x4859,0x4859,0x4859},
	{0x5926,0x0156,0x5926,0x0156},
	{0x4159,0x4596,0x1596,0x4156},
	{0x156A,0x4152,0x156A,0x4152} };
int SRS[7][4] = {
	{0x159D,0x89AB,0x26AE,0x4567},
	{0x0159,0x4856,0x159A,0x4526},
	{0x8159,0x456A,0x1592,0x0456},
	{0x4859,0x4859,0x4859,0x4859},
	{0x4815,0x459A,0x5926,0x0156},
	{0x4159,0x4596,0x1596,0x4156},
	{0x0459,0x8596,0x156A,0x4152} };




/***********显示方块*************/
void Russian::DrawBlock(int x, int y, int block_type)
{
	// printf("%d %d %d\n", block_type, x, y);
	x--;
	y++;
	if (block_type)
		lv_canvas_draw_rect(canvas, 10 * x + 1, 10 * y + 1, 9, 9, &rect_dsc);
	else
		lv_canvas_draw_rect(canvas, 10 * x, 10 * y, 10, 10, &empty_rect_dsc);
}

/***********块体转置*************/
int Russian::Rotate(int S, int I)
{
	return (F == 0) ? TGM[S][(I + 4) % 4] : SRS[S][(I + 4) % 4];
}

/***********擦除显示*************/
int Russian::Display(int x, int y, int CAC, int Mode)
{
	for (j = 0; j <= 3; j++) {
		P[j] = CAC & 0xF, CAC >>= 4;
		if (Mode == 1)
		{
			DrawBlock((P[j] >> 2) + x, (P[j] & 0x3) + y, 1);
		}
		else if (Mode == 0)
		{
			DrawBlock((P[j] >> 2) + x, (P[j] & 0x3) + y, 0);
		}
	}
	return 0;
}

/***********固化块体*************/
int Russian::DoBlocks()
{
	//~~~游戏结束
	if (Y < 2) {
		printf("GAME OVER!\n");
		GameInit();
		Canvas_Clear();

		Manager->BackHome();
	}
	//~~~固化块体
	POS_Y_MAX = 0, FLAG[3] = 1;
	for (j = 0; j <= 3; j++) {
		P_X = (P[j] >> 2) + X, P_Y = (P[j] & 0x3) + Y;
		if (POS_Y_MAX < P_Y) { POS_Y_MAX = P_Y; }
		POINT_V[P_X][P_Y] = 1;
	}
	//~~~关卡得分
	for (j = Y; j <= POS_Y_MAX; j++) {
		FLAG[2] = 1;
		for (i = 1; i <= 10; i++) {
			if (POINT_V[i][j] == 0) { FLAG[2] = 0; }
		}
		if (FLAG[2]) {
			SCORE += 10, MARK[j] = 1;
			if (SCORE == 400) {
				SCORE = 0;
				LEVEL += 1;
				T -= 100;
				FLAG[4] = 1;
			}
		}
	}
	//~~~极品消行
	for (j = 20; j >= 5; j--) {
		if (FLAG[4]) {
			for (i = 1; i <= 10; i++) {
				POINT_V[i][j] = 0;
				DrawBlock(i, j, 0);

			}
		}
		else if (MARK[j])
		{
			MARK[j] = 0, J = j - 1;
			for (N = 1; N <= 3; N++) {
				if (MARK[J]) { J--; }
			}
			MARK[J] = 1;
			for (i = 1; i <= 10; i++) {
				if (POINT_V[i][j] = POINT_V[i][J]) {
					DrawBlock(i, j, 1);

				}
				else {
					DrawBlock(i, j, 0);

				}
			}
		}
	}
	FLAG[4] = 0;
	return 0;
}

/***********碰撞检测*************/
int Russian::CheckCollision()
{
	for (j = 0; j <= 3; j++) {
		P_X = (P[j] >> 2) + X + dx, P_Y = (P[j] & 0x3) + Y + dy;
		if (POINT_V[P_X][P_Y]) {
			if (dx != 0)
			{
				return 1;
			}
			if (dy) {
				DoBlocks();
				printf("LEVEL：%-3d;", LEVEL);
				printf("SCORE：%-3d\n", SCORE);
				lv_label_set_text_fmt(level_label, "level:%03d", LEVEL);
				lv_label_set_text_fmt(score_label, "score:%03d", SCORE);
				return 2;
			}
			if (KEY_V == KEY_TURN)
			{
				FLAG[0] = 1;
			}
		}
	}
	return 0;
}

/***********init*************/
void Russian::GameInit()
{
	i = 0, j = 0, N = 0, T = 0, F = 0, J = 0, X = 0, Y = 0, dx = 0, dy = 0,
	KEY_V = 0, Cache1 = 0, Cache2 = 0, NU = 0, NI = 0, RU = 0, RI = 0, P_X = 0, P_Y = 0,
	POS_Y_MAX = 0, LEVEL = 1, SCORE = 0;

	for (int l = 0; l < 5; l++)
	{
		FLAG[l] = 0;
	}
	FLAG[3] = 1;

	srand((unsigned)time(NULL));
	RU = rand() % 7, RI = (rand() % 4);

	int i, j, BOUNDARY;
	for (j = 1; j <= 21; j++) {
		for (i = 0; i <= 11; i++) {
			BOUNDARY = i * (i - 11) * (j - 21);
			if (BOUNDARY == 0) {
				POINT_V[i][j] = 1;
			}
			else {
				POINT_V[i][j] = 0;
			}
		}
	}
}

void Russian::Canvas_Init()
{
	lv_draw_rect_dsc_init(&rect_dsc);
	rect_dsc.radius = 0;
	rect_dsc.bg_opa = LV_OPA_COVER;
	rect_dsc.bg_grad_dir = LV_GRAD_DIR_NONE;// LV_GRAD_DIR_HOR;
	rect_dsc.bg_color = lv_palette_main(LV_PALETTE_RED);
	rect_dsc.bg_grad_color = lv_palette_main(LV_PALETTE_BLUE);
	rect_dsc.border_width = 0;
	rect_dsc.border_opa = LV_OPA_0;
	rect_dsc.border_color = lv_color_white();
	rect_dsc.shadow_width = 0;
	rect_dsc.shadow_ofs_x = 0;
	rect_dsc.shadow_ofs_y = 0;

	lv_draw_rect_dsc_init(&empty_rect_dsc);
	empty_rect_dsc.radius = 0;
	empty_rect_dsc.bg_opa = LV_OPA_COVER;
	empty_rect_dsc.bg_grad_dir = LV_GRAD_DIR_NONE;// LV_GRAD_DIR_HOR;
	empty_rect_dsc.bg_color = lv_palette_lighten(LV_PALETTE_GREY, 3);
	empty_rect_dsc.bg_grad_color = lv_palette_main(LV_PALETTE_BLUE);
	empty_rect_dsc.border_width = 0;
	empty_rect_dsc.border_opa = LV_OPA_0;
	empty_rect_dsc.border_color = lv_color_white();
	empty_rect_dsc.shadow_width = 0;
	empty_rect_dsc.shadow_ofs_x = 0;
	empty_rect_dsc.shadow_ofs_y = 0;
	// lv_draw_label_dsc_t label_dsc;
	// lv_draw_label_dsc_init(&label_dsc);
	// label_dsc.color = lv_palette_main(LV_PALETTE_YELLOW);

	cbuf = lv_mem_alloc(LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT));
	// static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT)];
	canvas = lv_canvas_create(root);
	lv_canvas_set_buffer(canvas, cbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
	lv_obj_align(canvas, LV_ALIGN_CENTER, 0, 0);
	Canvas_Clear();
	// lv_canvas_draw_rect(canvas, 1, 1, 8, 8, &rect_dsc);
	// lv_canvas_draw_rect(canvas, 10 + 1, 10 + 1, 8, 8, &rect_dsc);
	// lv_canvas_draw_text(canvas, 40, 20, 100, &label_dsc, "Some text on text canvas");

}

void Russian::Canvas_Clear()
{
	lv_canvas_fill_bg(canvas, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_OPA_COVER);
}

void Russian::Labels_Init()
{

	level_label = lv_label_create(root);
	lv_label_set_text(level_label, "level:");
	lv_obj_set_style_text_color(level_label, lv_color_white(), 0);
	lv_obj_set_style_text_font(level_label, &lv_font_montserrat_12, 0);
	lv_obj_align(level_label, LV_ALIGN_TOP_RIGHT, -5, 25);

	score_label = lv_label_create(root); 
	lv_obj_set_style_text_color(score_label, lv_color_white(), 0);
	lv_obj_set_style_text_font(score_label, &lv_font_montserrat_12, 0);
	lv_obj_align(score_label, LV_ALIGN_TOP_RIGHT, -5, 25+20);

	// next_label = lv_label_create(root); lv_label_set_text(next_label, "next:");
	// lv_obj_set_style_text_color(next_label, lv_color_white(), 0);
	// lv_obj_set_style_text_font(next_label, &lv_font_montserrat_12, 0);
	// lv_obj_align(next_label, LV_ALIGN_TOP_RIGHT, -5, 25+20+20);

	lv_label_set_text_fmt(level_label, "level:%03d", 0);
	lv_label_set_text_fmt(score_label, "score:%03d", 0);
}

/***********Page*************/
Russian::Russian()
{
}

Russian::~Russian()
{

}

void Russian::onCustomAttrConfig()
{
	SetCustomCacheEnable(true);
	SetCustomLoadAnimType(PageManager::LOAD_ANIM_OVER_BOTTOM, 500, lv_anim_path_bounce);
}

void Russian::onViewLoad()
{
	lv_obj_remove_style_all(root);
	lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES);
	StatusBar::SetStyle(StatusBar::STYLE_BLACK);
	Labels_Init();
	GameInit();
	Canvas_Init();

	ui.group = lv_group_create();
	lv_group_add_obj(ui.group, canvas);

	AttachEvent(canvas);

	/*Test the rotation. It requires an other buffer where the orignal image is stored.
	 *So copy the current image to buffer and rotate it to the canvas*/
	 // static lv_color_t cbuf_tmp[CANVAS_WIDTH * CANVAS_HEIGHT];
	 // memcpy(cbuf_tmp, cbuf, sizeof(cbuf_tmp));
	 // lv_img_dsc_t img;
	 // img.data = (uint8_t*)cbuf_tmp;
	 // img.header.cf = LV_IMG_CF_TRUE_COLOR;
	 // img.header.w = CANVAS_WIDTH;
	 // img.header.h = CANVAS_HEIGHT;
	 //
	 // lv_canvas_fill_bg(canvas, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_OPA_COVER);
	 // lv_canvas_transform(canvas, &img, 30, LV_IMG_ZOOM_NONE, 0, 0, CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, true);
}

void Russian::onViewDidLoad()
{
	lv_indev_set_cursor(lv_get_indev(LV_INDEV_TYPE_ENCODER), canvas);
}

void Russian::onViewWillAppear()
{
	lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), ui.group);
	StatusBar::SetStyle(StatusBar::STYLE_TRANSP);

	lv_obj_set_style_bg_color(root, lv_color_white(), LV_PART_MAIN);

	timer = lv_timer_create(onTimerUpdate, 30, this);
	lv_timer_ready(timer);

	lv_obj_fade_in(root, 300, 0);
}

void Russian::onViewDidAppear()
{
	// lv_group_focus_obj(View.ui.canvas);
}

void Russian::onViewWillDisappear()
{
	lv_obj_fade_out(root, 300, 0);
}

void Russian::onViewDidDisappear()
{
	lv_timer_del(timer);
}

void Russian::onViewDidUnload()
{
	// View.Delete();
	// Model.DeInit();
	lv_group_del(ui.group);
	if (cbuf != nullptr)
	{
		lv_mem_free(cbuf);
	}
}

void Russian::AttachEvent(lv_obj_t* obj)
{
	lv_obj_set_user_data(obj, this);
	lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Russian::Update()
{
	dx = 0;
	dy = 0;

	static unsigned long lasttime = 0;
	if (millis() - lasttime > 300)
	{
		dy = 1;
		lasttime = millis();
	}

	if (FLAG[3]) {
		// Display(12, 8, Rotate(RU, RI), 0);
		X = 4, Y = 1, NU = RU, NI = RI, RU = rand() % 7, RI = (rand() % 4), FLAG[3] = 0, KEY_V = 0;
		// Display(12, 8, Rotate(RU, RI), 1);
		Display(X, Y, Rotate(NU, NI), 1);
	}


	if (KEY_V == KEY_TURN) {
		NI++;
		Display(X, Y, Rotate(NU, NI), 2);
	}//旋W
	else if (KEY_V == KEY_DOWN)
	{
		dy = 1;
	}//下S
	else if (KEY_V == KEY_LEFT)
	{
		dx = -1;
	}//左A
	else if (KEY_V == KEY_RIGHT)
	{
		dx = 1;
	}//右D


	if (dx != 0 || dy != 0 || KEY_V == KEY_TURN) {
		if (!CheckCollision()) {
			if (FLAG[0]) {
				NI--, FLAG[0] = 0;
				Display(X, Y, Rotate(NU, NI), 0);
			}
			else if (KEY_V == KEY_TURN) {
				Display(X, Y, Rotate(NU, NI - 1), 0);
			}
			else {
				Display(X, Y, Rotate(NU, NI), 0);
			}
			Display(X + dx, Y + dy, Rotate(NU, NI), 1);
			X += dx, Y += dy;
		}

	}

	KEY_V = -1;
}

void Russian::onTimerUpdate(lv_timer_t* timer)
{
	Russian* instance = (Russian*)timer->user_data;
	instance->Update();
}

void Russian::onEvent(lv_event_t* event)
{
	lv_obj_t* obj = lv_event_get_target(event);
	lv_event_code_t code = lv_event_get_code(event);
	auto* instance = (Russian*)lv_obj_get_user_data(obj);

	if (code == LV_EVENT_KEY)
	{
		HAL::TerminalPrintln("Russian LV_EVENT_KEY");
		uint32_t key = (*(uint32_t*)event->param);
		if (key == LV_KEY_LEFT)
		{
			HAL::TerminalPrintln("Russian KEY_LEFT");
			KEY_V = KEY_LEFT;
		}
		else if (key == LV_KEY_RIGHT)
		{
			HAL::TerminalPrintln("Russian KEY_RIGHT");
			KEY_V = KEY_RIGHT;
		}
		else if (key == LV_KEY_ENTER)
		{
			HAL::TerminalPrintln("Russian LV_KEY_DOWN");
			KEY_V = KEY_TURN;
		}
	}
}
