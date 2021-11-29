#ifndef __Pictures_VIEW_H
#define __Pictures_VIEW_H

#include <string>

#include "../Page.h"

namespace Page
{

	class PicturesView
	{
	public:
		void GuiInit();
		void GuiInit(lv_obj_t* root);
		void ListAddItem(lv_obj_t* list, const char* icon, const char* name);
		void ListAddText(lv_obj_t* list, const char* name);
		void Create(lv_obj_t* root);
		void Delete();

	public:
		struct
		{
			lv_obj_t* list1;
			lv_group_t* group;
		} ui;

	};

}

#endif // !__VIEW_H
