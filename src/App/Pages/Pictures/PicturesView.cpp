#include "PicturesView.h"

using namespace Page;

void PicturesView::GuiInit(lv_obj_t* root)
{
}

void PicturesView::ListAddItem(
    lv_obj_t* list,
    const char* icon,
    const char* name
)
{
    lv_obj_t* btn;
    btn = lv_list_add_btn(list, icon, name);
    lv_obj_set_style_text_font(btn, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(btn, lv_color_white(), 0);
    lv_group_add_obj(ui.group, btn);
}

void PicturesView::ListAddText(
    lv_obj_t* list,
    const char* name
)
{
    lv_obj_t* txt;
    txt = lv_list_add_text(list, name);
    lv_obj_set_style_text_font(txt, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(txt, lv_color_white(), 0);
}



void PicturesView::Create(lv_obj_t* root)
{
	// ui.canvas = lv_img_create(root);
	// lv_img_set_src(ui.canvas, Resource.GetImage("arm"));
	// lv_obj_center(ui.canvas);

    lv_obj_remove_style_all(root);
    lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES);

    ui.group = lv_group_create();
    ui.list1 = lv_list_create(root);
    lv_obj_set_size(ui.list1, LV_HOR_RES, LV_VER_RES-15);
    lv_obj_align(ui.list1, LV_ALIGN_CENTER,0,15);
    
    ListAddText(ui.list1, "Files");
    ListAddItem(ui.list1, LV_SYMBOL_FILE, "New");
    ListAddItem(ui.list1, LV_SYMBOL_SAVE, "Save");
    ListAddItem(ui.list1, LV_SYMBOL_CLOSE, "Delete");
    ListAddItem(ui.list1, LV_SYMBOL_EDIT, "Edit");

    ListAddText(ui.list1, "Connectivity");
    ListAddItem(ui.list1, LV_SYMBOL_BLUETOOTH, "Bluetooth");
    ListAddItem(ui.list1, LV_SYMBOL_GPS, "Navigation");
    ListAddItem(ui.list1, LV_SYMBOL_USB, "USB");
    ListAddItem(ui.list1, LV_SYMBOL_BATTERY_FULL, "Edit");
    ListAddItem(ui.list1, LV_SYMBOL_OK, "Apply");
    ListAddItem(ui.list1, LV_SYMBOL_CLOSE, "Close");
    
	
    // lv_group_add_obj(ui.group, root);
    // lv_group_add_obj(ui.group, ui.list1);
}

void PicturesView::Delete()
{
	
}
