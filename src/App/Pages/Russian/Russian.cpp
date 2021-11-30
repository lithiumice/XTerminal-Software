#include "Russian.h"

#include <ctime>

#include "HAL/HAL.h"
#include   <stdio.h>
#include  <stdlib.h>
using namespace Page;


/**********初始化参数************/
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

void draw_at_pos(int x,int y,const char* c)
{
	printf("%s %d %d\n", c, x, y);
}
/***********块体转置*************/
int Rote(int S, int I)
{
	return (F == 0) ? TGM[S][(I + 4) % 4] : SRS[S][(I + 4) % 4];
}

/***********擦除显示*************/
int Display(int x, int y, int CAC, int Mode)
{
	for (j = 0; j <= 3; j++) {
		P[j] = CAC & 0xF, CAC >>= 4;
		if (Mode == 1)
		{
			draw_at_pos((P[j] >> 2) + x, (P[j] & 0x3) + y, "X");
		}
		else if (Mode == 0)
		{
			draw_at_pos((P[j] >> 2) + x, (P[j] & 0x3) + y, " ");
		}
	}
	return 0;
}

/***********固化块体*************/
int DoBlocks()
{
	//~~~游戏结束
	if (Y < 2) {
		// Pos(1, 22);
		printf("GAME OVER！");
		exit(0);
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
				SCORE = 0, LEVEL += 1, T -= 100;
				FLAG[4] = 1;
			}
		}
	}
	//~~~极品消行
	for (j = 20; j >= 5; j--) {
		if (FLAG[4]) {
			for (i = 1; i <= 10; i++) {
				POINT_V[i][j] = 0;
				draw_at_pos(i, j, "　");

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
					draw_at_pos(i, j, "X");

				}
				else {
					draw_at_pos(i, j, " ");

				}
			}
		}
	}
	FLAG[4] = 0;
	return 0;
}

/***********碰撞检测*************/
int CheckCollision()
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
				// Pos(12, 3);
				printf("LEVEL：%-3d", LEVEL);
				// Pos(12, 5);
				printf("SCORE：%-3d", SCORE);
				return 2;
			}
			if (KEY_V == 119) { FLAG[0] = 1; }
		}
	}
	return 0;
}


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
	// View.Create(root);
	lv_obj_remove_style_all(root);
	lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES);
	StatusBar::SetStyle(StatusBar::STYLE_BLACK);

	lv_obj_t* level_label = lv_label_create(root); lv_label_set_text(level_label, "level:");
	lv_obj_set_style_text_color(level_label, lv_color_white(), 0);
	lv_obj_set_style_text_font(level_label, &lv_font_montserrat_12, 0);
	lv_obj_align(level_label, LV_ALIGN_TOP_RIGHT, -40, 10);

	lv_obj_t* score_label = lv_label_create(root); lv_label_set_text(score_label, "score:");
	lv_obj_set_style_text_color(score_label, lv_color_white(), 0);
	lv_obj_set_style_text_font(score_label, &lv_font_montserrat_12, 0);
	lv_obj_align(score_label, LV_ALIGN_TOP_RIGHT, -40, 30);

	lv_obj_t* next_label = lv_label_create(root); lv_label_set_text(next_label, "next:");
	lv_obj_set_style_text_color(next_label, lv_color_white(), 0);
	lv_obj_set_style_text_font(next_label, &lv_font_montserrat_12, 0);
	lv_obj_align(next_label, LV_ALIGN_TOP_RIGHT, -40, 50);

	ui.group = lv_group_create();
	AttachEvent(root);

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

void Russian::onViewDidLoad()
{

}

void Russian::onViewWillAppear()
{
	lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), ui.group);
	StatusBar::SetStyle(StatusBar::STYLE_TRANSP);

	lv_obj_set_style_bg_color(root, lv_color_white(), LV_PART_MAIN);

	srand((unsigned)time(NULL));
	RU = rand() % 7, RI = (rand() % 4);
	timer = lv_timer_create(onTimerUpdate, 500, this);
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
}

void Russian::AttachEvent(lv_obj_t* obj)
{
	lv_obj_set_user_data(obj, this);
	lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Russian::Update()
{

	if (FLAG[3]) {
		Display(12, 8, Rote(RU, RI), 0);
		X = 4, Y = 1, NU = RU, NI = RI, RU = rand() % 7, RI = (rand() % 4), FLAG[3] = 0, KEY_V = 0;
		Display(12, 8, Rote(RU, RI), 1);
		Display(X, Y, Rote(NU, NI), 1);
	}
	dx = 0, dy = 0;

	// Getkey(10, 800);
	// KEY_V = Getkey(N, T);
	//
	dy=1;
	//
	// if (KEY_V == 119) {
	// 	NI++;
	// 	Display(X, Y, Rote(NU, NI), 2);
	// }//旋W
	// else if (KEY_V == 115)
	// {
	// 	dy = 1;
	// }//下S
	// else if (KEY_V == 97)
	// {
	// 	dx = -1;
	// }//左A
	// else if (KEY_V == 100)
	// {
	// 	dx = 1;
	// }//右D
	// else if (KEY_V == 112)
	// {
	// 	getch();
	// }//暂停P
	// else if (KEY_V == 113)
	// {
	// 	return 0;
	// }//退出Q

	if (dx != 0 || dy != 0 || KEY_V == 119) {
		if (!CheckCollision()) {
			if (FLAG[0]) {
				NI--, FLAG[0] = 0;
				Display(X, Y, Rote(NU, NI), 0);
			}
			else if (KEY_V == 119) {
				Display(X, Y, Rote(NU, NI - 1), 0);
			}
			else {
				Display(X, Y, Rote(NU, NI), 0);
			}
			Display(X + dx, Y + dy, Rote(NU, NI), 1);
			X += dx, Y += dy;
		}

	}
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

	if (code == LV_EVENT_RELEASED)
	{
		HAL::TerminalPrintln("Russian LV_EVENT_PRESSED");
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
