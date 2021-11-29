#include "AppListView.h"

using namespace Page;

#define ITEM_HEIGHT_MIN   100
#define ITEM_PAD          ((LV_VER_RES - ITEM_HEIGHT_MIN) / 2)


void AppListView::Create(lv_obj_t* root)
{
	lv_obj_remove_style_all(root);
	lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES);
	lv_obj_set_style_bg_color(root, lv_color_black(), 0);
	lv_obj_set_style_bg_opa(root, LV_OPA_COVER, 0);
	lv_obj_set_style_pad_ver(root, ITEM_PAD, 0);

	lv_obj_set_flex_flow(root, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(
		root,
		LV_FLEX_ALIGN_START,
		LV_FLEX_ALIGN_START,
		LV_FLEX_ALIGN_CENTER
	);

	Style_Init();

	Item_Create(
		&ui.cubedemo,
		root,
		"CubeDemo",
		"cube",

		"Men at some time are masters of their fates. The fault, dear Brutus, is not in our"
	);

	Item_Create(
		&ui.terminal,
		root,
		"Terminal",
		"terminal",

		"When I got enough confidence, the stage was gone. When I was sure of losing, I won. When I needed people the most, they left me."
	);

	Item_Create(
		&ui.pictures,
		root,
		"Pictures",
		"picture",

		"Never play with the feelings of others, because you may win the game but the risk is that you will surely lose the person for life time"
	);

	Item_Create(
		&ui.settings,
		root,
		"Settings",
		"setting",

		"When I got enough confidence, the stage was gone. When I was sure of losing, I won. When I needed people the most, they left me."
	);

	Item_Create(
		&ui.filemanager,
		root,
		"filemanager",
		"filemanager",
	
		"When I got enough confidence, the stage was gone. When I was sure of losing, I won. When I needed people the most, they left me. When I learnt to dry my tears, I found a shoulder to cry on. And when I mastered the art of hating, somebody started loving me. --William Shakespeare"
	);
	
	Item_Create(
		&ui.videoplayer,
		root,
		"videoplayer",
		"video",
	
		"Men at some time are masters of their fates. The fault, dear Brutus, is not in our stars, but in ourselves, that we are underlings. --William Shakespeare, Julius Caesar"
	);
	
	Item_Create(
		&ui.fandisplay,
		root,
		"fandisplay",
		"zhihu",
	
		"Never play with the feelings of others, because you may win the game but the risk is that you will surely lose the person for life time"
	);
	
	Item_Create(
		&ui.weather,
		root,
		"weather",
		"weather",
	
		"Never play with the feelings of others, because you may win the game but the risk is that you will surely lose the person for life time"
	);
	
	Item_Create(
		&ui.clock,
		root,
		"clock",
		"clock",
	
		"Never play with the feelings of others, because you may win the game but the risk is that you will surely lose the person for life time"
	);

	Group_Init();
}

void AppListView::Group_Init()
{
	ui.group = lv_group_create();
	lv_group_set_focus_cb(ui.group, onFocus);
	lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), ui.group);

#define APPLIST_DEF(className)\
	lv_group_add_obj(ui.group, ui.className.icon);
#include "_APPLIST_DEF.inc"
#undef APPLIST_DEF
	
	lv_group_focus_obj(ui.terminal.icon);
}

void AppListView::Delete()
{
	lv_group_del(ui.group);
	Style_Reset();
}

void AppListView::SetScrollToY(lv_obj_t* obj, lv_coord_t y, lv_anim_enable_t en)
{
	lv_coord_t scroll_y = lv_obj_get_scroll_y(obj);
	lv_coord_t diff = -y + scroll_y;

	lv_obj_scroll_by(obj, 0, diff, en);
}

void AppListView::onFocus(lv_group_t* g)
{
	lv_obj_t* icon = lv_group_get_focused(g);
	lv_obj_t* cont = lv_obj_get_parent(icon);
	lv_coord_t y = lv_obj_get_y(cont);
	lv_obj_scroll_to_y(lv_obj_get_parent(cont), y, LV_ANIM_ON);
}

void AppListView::Style_Init()
{
	lv_style_init(&style.icon);
	lv_style_set_width(&style.icon, 220);
	lv_style_set_bg_color(&style.icon, lv_color_black());
	lv_style_set_bg_opa(&style.icon, LV_OPA_COVER);
	lv_style_set_text_font(&style.icon, Resource.GetFont("bahnschrift_17"));
	lv_style_set_text_color(&style.icon, lv_color_white());

	lv_style_init(&style.focus);
	lv_style_set_width(&style.focus, 95);
	lv_style_set_border_side(&style.focus, LV_BORDER_SIDE_RIGHT);
	lv_style_set_border_width(&style.focus, 2);
	lv_style_set_border_color(&style.focus, lv_color_hex(0xff0000));

	static const lv_style_prop_t style_prop[] =
	{
		LV_STYLE_WIDTH,
		LV_STYLE_PROP_INV
	};

	static lv_style_transition_dsc_t trans;
	lv_style_transition_dsc_init(
		&trans,
		style_prop,
		lv_anim_path_overshoot,
		200,
		0,
		nullptr
	);
	lv_style_set_transition(&style.focus, &trans);
	lv_style_set_transition(&style.icon, &trans);

	lv_style_init(&style.info);
	lv_style_set_text_font(&style.info, Resource.GetFont("bahnschrift_13"));
	lv_style_set_text_color(&style.info, lv_color_hex(0x999999));

	lv_style_init(&style.data);
	lv_style_set_text_font(&style.data, Resource.GetFont("bahnschrift_13"));
	lv_style_set_text_color(&style.data, lv_color_white());
}

void AppListView::Style_Reset()
{
	lv_style_reset(&style.icon);
	lv_style_reset(&style.info);
	lv_style_reset(&style.data);
	lv_style_reset(&style.focus);
}

void AppListView::Item_Create(
	item_t* item,
	lv_obj_t* par,
	const char* name,
	const char* img_src,
	const char* infos
)
{
	lv_obj_t* cont = lv_obj_create(par);
	lv_obj_remove_style_all(cont);
	lv_obj_set_width(cont, 220);

	lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
	item->cont = cont;

	/* icon */
	lv_obj_t* icon = lv_obj_create(cont);
	lv_obj_remove_style_all(icon);
	lv_obj_clear_flag(icon, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_add_style(icon, &style.icon, 0);
	lv_obj_add_style(icon, &style.focus, LV_STATE_FOCUSED);
	lv_obj_set_style_align(icon, LV_ALIGN_LEFT_MID, 0);

	lv_obj_set_flex_flow(icon, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(
		icon,
		LV_FLEX_ALIGN_SPACE_AROUND,
		LV_FLEX_ALIGN_CENTER,
		LV_FLEX_ALIGN_CENTER
	);

	lv_obj_t* img = lv_img_create(icon);
	lv_img_set_src(img, Resource.GetImage(img_src));

	lv_obj_t* label = lv_label_create(icon);
	lv_label_set_text(label, name);
	item->icon = icon;

	/* infos */
	label = lv_label_create(cont);
	lv_label_set_text(label, infos);
	lv_obj_add_style(label, &style.info, 0);
	lv_obj_align(label, LV_ALIGN_LEFT_MID, 110, 0);
	lv_obj_set_size(label, 100, 90);
	lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
	item->labelInfo = label;

	lv_obj_move_foreground(icon);

	/* get real max height */
	lv_obj_update_layout(item->labelInfo);
	lv_coord_t height = lv_obj_get_height(item->labelInfo);
	height = LV_MAX(height, ITEM_HEIGHT_MIN);
	lv_obj_set_height(cont, height);
	lv_obj_set_height(icon, height);
}

