// /**
//  ******************************************************************************
//  * @file    lv_100ask_stm32_game_snake.c
//  * @author  ���ʿƼ�
//  * @version V1.2
//  * @date    2020-12-12
//  * @brief	̰������Ϸ
//  ******************************************************************************
//  * Change Logs:
//  * Date           Author          Notes
//  * 2020-12-12     zhouyuebiao     First version
//  * 2021-01-25     zhouyuebiao     V1.2 
//  ******************************************************************************
//  * @attention
//  *
//  * Copyright (C) 2008-2021 ���ڰ������Ƽ����޹�˾<https://www.100ask.net/>
//  * All rights reserved
//  *
//  ******************************************************************************
//  */
//
// /*********************
//  *      INCLUDES
//  *********************/
// #include <stdio.h>
// #include <stdlib.h>
// #include "driver_key.h"
// #include "lv_100ask_stm32_game_snake.h"
//
//
// /**********************
//  *  STATIC VARIABLES
//  **********************/
// static PT_lv_100ask_snake g_pt_lv_100ask_snake;  	// ���ݽṹ��
// static link_snake *spriteSnake;              		// ����ͷ(��ͷ)
//
// /**********************
//  *  STATIC PROTOTYPES
//  **********************/
// static void lv_100ask_stm32_game_snake_init(void);							// ��ʼ������
// static link_snake *lv_100ask_stm32_game_snake_initLink(void);				// ��ʼ������
// static void lv_100ask_stm32_game_snake_clear_list(link_snake* ppHeadNode);	// ��������ͷŽ���ڴ棬����������Ϊ�ձ�
// static void lv_100ask_stm32_game_snake_linkAddNode(void);					// ������β�������½ڵ�(����С������)
// static void lv_task_100ask_game_snake(lv_task_t * task);					// ��Ϸ����
// static void lv_100ask_stm32_game_snake_update_snake_data(void); 			// ����С�����ݽṹ
// static void event_handler_snake_gesture_cb(lv_obj_t * obj, lv_event_t e);	// ��������⴦���¼�(������������Ϸ)
// static void event_handler_back_to_home(lv_obj_t * obj, lv_event_t event);	// ���������¼�������
//
//
// /*
//  *  ��������   void lv_100ask_stm32_game_snake(void)
//  *  ��������� ��
//  *  ����ֵ��   ��
//  *  �������ã� Ӧ�ó�ʼ�����
// */
// void lv_100ask_stm32_game_snake(void)
// {
// 	g_pt_lv_100ask_snake = (T_lv_100ask_snake *)malloc(sizeof(T_lv_100ask_snake));   // �����ڴ�
// 	g_pt_lv_100ask_snake->gesture = 0; 
//
// 	g_pt_lv_100ask_snake->bg = lv_obj_create(lv_scr_act(), NULL);
// 	lv_obj_set_size(g_pt_lv_100ask_snake->bg, LV_HOR_RES, LV_VER_RES);
// 	lv_obj_set_y(g_pt_lv_100ask_snake->bg, 0);
//
//     lv_100ask_stm32_game_snake_init();
//
//     /* ���������ⰴ����������Ϸ���� */
// 	g_pt_lv_100ask_snake->task_handle = lv_task_create(lv_task_100ask_game_snake, 150, LV_TASK_PRIO_MID, NULL);
// 	
//     lv_obj_set_click(lv_layer_top(), true);
//     lv_obj_set_event_cb(lv_layer_top(), event_handler_snake_gesture_cb);   	// �����¼�����
// 	
//     add_title(g_pt_lv_100ask_snake->bg, "SNAKE");							// ����
// 	add_back(lv_layer_top(), event_handler_back_to_home);    				// �������水ť
// }
//
//
// /*
//  *  ��������   static void lv_100ask_stm32_game_snake_init(void)
//  *  ��������� ��
//  *  ����ֵ��   ��
//  *  �������ã� Ӧ�ý����ʼ��
// */
// static void lv_100ask_stm32_game_snake_init(void)
// {
//     spriteSnake = lv_100ask_stm32_game_snake_initLink();
//
//     link_snake* head = NULL;
//     head = spriteSnake;         //��tempָ������ָ��ͷ���
//     int i = 0;
//     lv_coord_t init_x = (rand()%(LV_HOR_RES));
//     lv_coord_t init_y = (rand()%(LV_VER_RES));
//     while (head->next)
//     {
//         head = head->next;
//         head->obj = lv_obj_create(g_pt_lv_100ask_snake->bg, NULL);
//         if (i == 0)
//         {
//             lv_obj_set_style_local_bg_color(head->obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_100ASK_GAME_SNAKE_HEAD_COLOR); //������ɫ
//         }
//         else
//             lv_obj_set_style_local_bg_color(head->obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_100ASK_GAME_SNAKE_BODY_COLOR); //������ɫ
//         lv_obj_set_style_local_radius(head->obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_100ASK_GAME_SNAKE_RADIUS);  		 // ����Բ��
//         lv_obj_set_size(head->obj, LV_100ASK_GAME_SNAKE_SIZE, LV_100ASK_GAME_SNAKE_SIZE);
//         lv_obj_set_pos(head->obj, init_x+(i * LV_100ASK_GAME_SNAKE_SIZE), init_y);
//         head->x = lv_obj_get_x(head->obj);
//         head->y = lv_obj_get_y(head->obj);
//         i++;
//     }
//
//     // ��ʼ��ʳ��
//     g_pt_lv_100ask_snake->obj_food = lv_obj_create(g_pt_lv_100ask_snake->bg, NULL);
//     lv_obj_set_size(g_pt_lv_100ask_snake->obj_food, LV_100ASK_GAME_FOOD_SIZE, LV_100ASK_GAME_FOOD_SIZE);
//     lv_obj_set_style_local_bg_color(g_pt_lv_100ask_snake->obj_food, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_100ASK_GAME_FOOD_COLOR); //������ɫ
//     lv_obj_set_style_local_radius(g_pt_lv_100ask_snake->obj_food, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_100ASK_GAME_FOOD_RADIUS);	// ����Բ��
//     lv_obj_set_pos(g_pt_lv_100ask_snake->obj_food, (rand()%(LV_100ASK_GAME_FOOD_MAX_HOR)), (rand()%(LV_100ASK_GAME_FOOD_MAX_VER)));
// 	
// 	g_pt_lv_100ask_snake->sroce = 3;
//     g_pt_lv_100ask_snake->len = 3;
//     g_pt_lv_100ask_snake->label_info = lv_label_create(g_pt_lv_100ask_snake->bg, NULL);
// 	lv_obj_set_style_local_text_font(g_pt_lv_100ask_snake->label_info, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);  		// ���������ʽ
// 	lv_label_set_text_fmt(g_pt_lv_100ask_snake->label_info, "LEN: %d\nSROCE: %d", g_pt_lv_100ask_snake->len, g_pt_lv_100ask_snake->sroce);  // չʾ��Ϸ��Ϣ
// 	lv_obj_align(g_pt_lv_100ask_snake->label_info, NULL, LV_ALIGN_IN_TOP_LEFT, 0, LV_100ASK_GAME_SNAKE_TITLE_SPACE);
//
// }
//
// // 
// /*
//  *  ��������   static link_snake* lv_100ask_stm32_game_snake_initLink(void)
//  *  ��������� ��
//  *  ����ֵ��   ��ʼ���������
//  *  �������ã� ��ʼ������(��)
// */
// static link_snake* lv_100ask_stm32_game_snake_initLink(void)
// {
//     link_snake* head = (link_snake*)malloc(sizeof(link_snake));		// ���������һ����㣨��Ԫ��㣩
//     head->prior = NULL;
//     head->next = NULL;
//     head->obj = NULL;
//     head->x = 0;
//     head->y = 0;
//     link_snake * list = head;   //����һ��ָ��ָ��ͷ��㣬�������������������´����Ľڵ�
//
//     // ��������(��ʼ������)
//     for (int i = 0; i < LV_100ASK_GAME_SNAKE_INIT_LINE; i++) {
//         link_snake *body = (link_snake*)malloc(sizeof(link_snake));
//         body->prior = NULL;
//         body->next = NULL;
//         body->obj = NULL;
//         body->x = i;
//         body->y = i;
//
//         //�½ڵ����������һ���ڵ㽨����ϵ
//         list->next = body;
//         body->prior = list;
//         //list��Զָ�����������һ���ڵ�
//         list = list->next;
//     }
//     return head;
// }
//
//
//
// /*
//  *  ��������   static void lv_100ask_stm32_game_snake_linkAddNode(void)
//  *  ��������� ��
//  *  ����ֵ��   ��
//  *  �������ã� ������β�������½ڵ�(��������)
// */
// static void lv_100ask_stm32_game_snake_linkAddNode(void)
// {
//     link_snake *temp_list;
//
//     // ��ʼ���½ڵ�
//     link_snake *list_new = (link_snake*)malloc(sizeof(link_snake));
//     list_new->prior = NULL;
//     list_new->next = NULL;
//     list_new->x = 0;
//     list_new->y = 0;
//     list_new->obj = lv_obj_create(g_pt_lv_100ask_snake->bg, NULL);
//     lv_obj_set_style_local_bg_color(list_new->obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_100ASK_GAME_SNAKE_BODY_COLOR); //������ɫ
//     lv_obj_set_style_local_radius(list_new->obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_100ASK_GAME_SNAKE_RADIUS);  // ����Բ��
//     lv_obj_set_size(list_new->obj, LV_100ASK_GAME_SNAKE_SIZE, LV_100ASK_GAME_SNAKE_SIZE);
//
//     // ���ͷ�ڵ�
//     if(spriteSnake == NULL)
//     {
//         spriteSnake = list_new;
//     }
//     else
//     {
//         temp_list = spriteSnake;
//         while(temp_list->next)  // ��λ������β��
//         {
//             temp_list = temp_list->next;
//         }
//         temp_list->next = list_new;
//         list_new->prior = temp_list;
//     }
// }
//
//
// /*
//  *  ��������   static void lv_100ask_stm32_game_snake_clear_list(link_snake* ppHeadNode)
//  *  ��������� ��
//  *  ����ֵ��   ��
//  *  �������ã� ��������ͷŽ���ڴ棬����������Ϊ�ձ�
// */
// static void lv_100ask_stm32_game_snake_clear_list(link_snake* ppHeadNode)
// {
//     link_snake* pListNodeTmp = NULL;
//     if((ppHeadNode) == NULL)
//     {
//         printf("The list is empty, no need to clear.\n");
//         return;
//     }
//
//     //bool is_head = true;
//     // ѭ���ͷ������еĽ����ռ�ڴ棬
//     while((ppHeadNode)->next != NULL)
//     {
//         pListNodeTmp = (ppHeadNode)->next;
// 		if (ppHeadNode->obj != NULL)	lv_obj_clean(ppHeadNode->obj);  // ɾ������ڵ�
//         free((ppHeadNode));
//         (ppHeadNode) = pListNodeTmp;
//     }
//
//     // ������һ�����
//     if((ppHeadNode) != NULL)
//     {
// 		if (ppHeadNode->obj != NULL)	lv_obj_clean(ppHeadNode->obj);  // ɾ������ڵ�
//         free((ppHeadNode));
//         (ppHeadNode) = NULL;
//     }
//
//     printf("The list is cleared.\n");
// }
//
//
// /*
//  *  ��������   static void lv_task_100ask_game_snake(lv_task_t * task)
//  *  ��������� ����������
//  *  ����ֵ��   ��
//  *  �������ã� ��������������Ϸ�����ϵ��ƶ�С��
// */
// static void lv_task_100ask_game_snake(lv_task_t * task)
// {
// #if LV_100ASK_GAME_SNAKE_USE_KEY
// 	uint8_t *key = (uint8_t*)&key_value.up_value;
// 	if(key[0] == 1)
// 	{
// 		printf("UP\n\r");
// 		g_pt_lv_100ask_snake->gesture |= 1;
// 		key[0] = 0;
// 	}
// 	else if(key[1] == 1)
// 	{
// 		printf("DOWN\n\r");
// 		g_pt_lv_100ask_snake->gesture |= 2;
// 		key[1] = 0;
// 	}
// 	else if(key[2] == 1)
// 	{
// 		printf("LEFT\n\r");
// 		g_pt_lv_100ask_snake->gesture |= 8;
// 		key[2] = 0;
// 	}
// 	else if(key[3] == 1)
// 	{
// 		printf("RIGHT\n\r");
// 		g_pt_lv_100ask_snake->gesture |= 4;
// 		key[3] = 0;
// 	}
// #endif // LV_100ASK_GAME_SNAKE_USE_KEY
// 	
//     lv_coord_t x = spriteSnake->next->x - lv_obj_get_x(g_pt_lv_100ask_snake->obj_food);   // ��ȡ x ��Ĳ���
//     lv_coord_t y = spriteSnake->next->y - lv_obj_get_y(g_pt_lv_100ask_snake->obj_food);   // ��ȡ y ��Ĳ���
//     if(((x >= -LV_100ASK_GAME_FOOD_CHECK_SPACE)&&(x <= LV_100ASK_GAME_FOOD_CHECK_SPACE)) && ((y >= -LV_100ASK_GAME_FOOD_CHECK_SPACE)&&(y <= LV_100ASK_GAME_FOOD_CHECK_SPACE)))
//     {
//         lv_100ask_stm32_game_snake_linkAddNode();
//         g_pt_lv_100ask_snake->len += 1;
//         g_pt_lv_100ask_snake->sroce += 1;
// 		
//         lv_obj_set_pos(g_pt_lv_100ask_snake->obj_food, rand()%(LV_100ASK_GAME_FOOD_MAX_HOR), rand()%(LV_100ASK_GAME_FOOD_MAX_VER));
// 		lv_label_set_text_fmt(g_pt_lv_100ask_snake->label_info, "LEN: %u\nSROCE: %u", g_pt_lv_100ask_snake->len, g_pt_lv_100ask_snake->sroce);  // չʾ��Ϸ��Ϣ
//     }
//     lv_100ask_stm32_game_snake_update_snake_data();
// }
//
//
//
// /*
//  *  ��������   static void event_handler_snake_gesture_cb(lv_obj_t * obj, lv_event_t e)
//  *  ��������� �����¼��Ķ���
//  *  ��������� �������¼�����
//  *  ����ֵ��   ��
//  *  �������ã� ����������¼���������С���ƶ�
// */
// static void event_handler_snake_gesture_cb(lv_obj_t * obj, lv_event_t e)
// {
// 	if (e == LV_EVENT_GESTURE)
// 	{
// 		switch(lv_indev_get_gesture_dir(lv_indev_get_act()))
// 		{
// 			case LV_GESTURE_DIR_TOP:
// 			    g_pt_lv_100ask_snake->gesture |= 1;
//                 break;
// 			case LV_GESTURE_DIR_BOTTOM:
// 				g_pt_lv_100ask_snake->gesture |= 2;
// 				break;
// 			case LV_GESTURE_DIR_RIGHT:
// 				g_pt_lv_100ask_snake->gesture |= 4;
// 				break;
// 			case LV_GESTURE_DIR_LEFT:
// 				g_pt_lv_100ask_snake->gesture |= 8;
// 				break;
// 			default: break;
// 		}
// 		lv_100ask_stm32_game_snake_update_snake_data();
// 	}
// 	else if (e == LV_EVENT_LONG_PRESSED)   // ������ʱ����ǰ��
//     {
//         lv_100ask_stm32_game_snake_update_snake_data();
//     }
//
// }
//
// /*
//  *  ��������   static void lv_100ask_stm32_game_snake_update_snake_data(void)
//  *  ��������� ��
//  *  ����ֵ��   ��
//  *  �������ã� ����С�����ݽṹ
// */
// static void lv_100ask_stm32_game_snake_update_snake_data(void)
// {
//     static lv_coord_t x = LV_100ASK_GAME_SNAKE_SPEED, y = 0;  // Ĭ�����ұ���
//     volatile lv_coord_t obj_size_x = LV_100ASK_GAME_SNAKE_SPACE, obj_size_y = 0;  // ��ಹ��
//     if (((g_pt_lv_100ask_snake->gesture) & 1) == 1)         // ��
//     {
//         y = -LV_100ASK_GAME_SNAKE_SPEED;
//         x = 0;
//         obj_size_y = -LV_100ASK_GAME_SNAKE_SPACE;
//         obj_size_x = 0;
//     }
//     else if (((g_pt_lv_100ask_snake->gesture) & 2) == 2)   // ��
//     {
//         y = LV_100ASK_GAME_SNAKE_SPEED;
//         x = 0;
//         obj_size_y = LV_100ASK_GAME_SNAKE_SPACE;
//         obj_size_x = 0;
//     }
//     else if (((g_pt_lv_100ask_snake->gesture) & 4) == 4)   // ��
//     {
//         y = 0;
//         x = LV_100ASK_GAME_SNAKE_SPEED;
//         obj_size_y = 0;
//         obj_size_x = LV_100ASK_GAME_SNAKE_SPACE;
//     }
//     else if (((g_pt_lv_100ask_snake->gesture) & 8) == 8)   // ��
//     {
//         y = 0;
//         x = -LV_100ASK_GAME_SNAKE_SPEED;
//         obj_size_y = 0;
//         obj_size_x = -LV_100ASK_GAME_SNAKE_SPACE;
//     }
//     g_pt_lv_100ask_snake->gesture = 0;  // ��ձ�־λ
//
//     link_snake* temp_list = NULL;
//     temp_list = spriteSnake;    // �� temp_list ָ������ָ��ͷ���
//     while (temp_list->next)     // ��λ������β��
//     {
//         temp_list = temp_list->next;
//     }
//     while (temp_list->prior->prior)  // �׽ڵ㲻����
//     {
//         // �Ӻ���ǰ����������������
//         temp_list->x = temp_list->prior->x;
//         temp_list->y = temp_list->prior->y;
//         lv_obj_set_pos(temp_list->obj, temp_list->x, temp_list->y);
//         temp_list = temp_list->prior;
//     }
//     // �����׽ڵ�
//     temp_list->x = lv_obj_get_x(temp_list->obj) + x;
//     temp_list->y = lv_obj_get_y(temp_list->obj) + y;
//     lv_obj_set_pos(temp_list->obj, temp_list->x, temp_list->y);
// }
//
//
// /*
//  *  ��������   static void event_handler_back_to_home(lv_obj_t * obj, lv_event_t event)
//  *  ��������� �����¼��Ķ���
//  *  ��������� �������¼�����
//  *  ����ֵ��   ��
//  *  �������ã� ���������¼�������
// */
// static void event_handler_back_to_home(lv_obj_t * obj, lv_event_t event)
// {
//     if(event == LV_EVENT_CLICKED)
//     {
//         if (g_pt_lv_100ask_snake->task_handle != NULL)	lv_task_del(g_pt_lv_100ask_snake->task_handle);  // ɾ������.
//         if (g_pt_lv_100ask_snake->bg != NULL)			lv_obj_del(g_pt_lv_100ask_snake->bg);  			// ɾ������
// 		if (g_pt_lv_100ask_snake->obj_food != NULL)		lv_obj_del(g_pt_lv_100ask_snake->obj_food);  				// ɾ��ʳ��
// 		if (g_pt_lv_100ask_snake->label_info != NULL)	lv_obj_del(g_pt_lv_100ask_snake->label_info);  		// ɾ����Ϸ��Ϣǽ
// 		
// 		
// 		/* ɾ������ */
//         lv_100ask_stm32_game_snake_clear_list((spriteSnake));  		// �������
//         g_pt_lv_100ask_snake->sroce = 0; 	// �����Ϸ����
//         g_pt_lv_100ask_snake->len = 0; 		// ��ճ���
// 		
// 		free(g_pt_lv_100ask_snake);
// 		
// 		lv_obj_set_event_cb(lv_layer_top(), NULL);   /* �����¼����� */
//         lv_obj_set_click(lv_layer_top(), false);
// 		lv_obj_clean(lv_layer_top());
// 		
//         lv_100ask_stm32_anim_out_all(lv_scr_act(), 0);
//         lv_100ask_stm32_demo_home(10);
//     }
// }
//
