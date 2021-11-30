#include "TerminalView.h"

using namespace Page;

void TerminalView::SetTermText(std::string termText)
{
	lv_label_set_text(
		ui.termTextLabel,
		termText.c_str()
	);
}

#include "lv_demos/lv_demo.h"


void TerminalView::Create(lv_obj_t* root)
{
	lv_obj_remove_style_all(root);
	lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES);

	ui.termTextLabel = lv_label_create(root);
	 lv_obj_set_size(ui.termTextLabel, LV_HOR_RES, LV_VER_RES);
	 lv_label_set_long_mode(ui.termTextLabel, LV_LABEL_LONG_WRAP);
	 lv_label_set_recolor(ui.termTextLabel, true);
	 lv_label_set_text(ui.termTextLabel, "hello world");
	 lv_obj_set_style_text_font(ui.termTextLabel, &lv_font_montserrat_10, 0);
	 // lv_obj_set_style_text_font(ui.termTextLabel, &lv_font_simsun_16_cjk, 0);
	 lv_obj_set_style_text_color(ui.termTextLabel, lv_color_white(), 0);
	 // lv_obj_align(ui.termTextLabel, LV_ALIGN_TOP_MID, 0, 120);
	 // lv_obj_set_pos(ui.termTextLabel, 5, 15);
	 
	ui.group = lv_group_create();
	// lv_group_add_obj(ui.group, root);
	lv_group_add_obj(ui.group, ui.termTextLabel);
}

void TerminalView::Delete()
{
	lv_group_del(ui.group);
}
