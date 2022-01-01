// #ifndef LV_100ASK_STM32_GAME_SANKE_H
// #define LV_100ASK_STM32_GAME_SANKE_H
//
// #ifdef __cplusplus
// extern "C" {
// #endif
//
//
// /*********************
//  *      INCLUDES
//  *********************/
// #include "lvgl/lvgl.h"
//
//
//
// /*********************
//  *      DEFINES
//  *********************/
// #define LV_100ASK_GAME_SNAKE_TITLE_SPACE   (50)
// #define LV_100ASK_GAME_SNAKE_INIT_LINE     (3)
// #define LV_100ASK_GAME_SNAKE_MAX_LINE      (256)
// #define LV_100ASK_GAME_SNAKE_SIZE          (15)
// //#define LV_100ASK_GAME_SNAKE_EYE_SIZE    (5)
// #define LV_100ASK_GAME_SNAKE_SPEED         (15)
// #define LV_100ASK_GAME_SNAKE_SPACE         (LV_100ASK_GAME_SNAKE_SIZE - LV_100ASK_GAME_SNAKE_SPEED)
// #define LV_100ASK_GAME_SNAKE_HEAD_COLOR    (LV_COLOR_YELLOW)  		// ��ͷ��ɫ
// #define LV_100ASK_GAME_SNAKE_BODY_COLOR    (LV_COLOR_BLACK)  		// ������ɫ
// #define LV_100ASK_GAME_SNAKE_RADIUS        (15)   					// ������Բ������radius
// #define LV_100ASK_GAME_FOOD_RADIUS         (50)   					// ʳ��Բ������
// #define LV_100ASK_GAME_FOOD_COLOR          (LV_COLOR_PURPLE) 	    // ʳ����ɫ
// #define LV_100ASK_GAME_FOOD_SIZE           (LV_100ASK_GAME_SNAKE_SIZE)
// #define LV_100ASK_GAME_FOOD_CHECK_SPACE    (10)
// #define LV_100ASK_GAME_FOOD_MAX_HOR        (LV_HOR_RES - LV_100ASK_GAME_SNAKE_SIZE)
// #define LV_100ASK_GAME_FOOD_MAX_VER        (LV_VER_RES - LV_100ASK_GAME_SNAKE_TITLE_SPACE)
// #define LV_100ASK_GAME_SNAKE_USE_KEY       (1)  					// ʹ�ð�������
//
//
// /**********************
//  *      TYPEDEFS
//  **********************/
// // ��Ϸ����
// typedef struct _lv_100ask_snake {
// 	lv_obj_t  * bg;				// ����
// 	lv_obj_t  * obj_food;		// ʳ����
// 	lv_obj_t  * label_info;		// ��Ϸ��Ϣ
// 	lv_task_t * task_handle; 	// ������
// 	uint8_t	sroce;				// �÷�
// 	uint8_t len;				// ����
// 	uint8_t gesture;			// ��ͷ�����־
// } T_lv_100ask_snake, *PT_lv_100ask_snake;
//
// // ��������
// typedef struct Link{
// 	lv_obj_t *obj;            	// ����
// 	lv_coord_t x;             	// x����
// 	lv_coord_t y;             	// y����
// 	struct Link *prior;       	// ָ��ֱ��ǰ��
// 	struct Link *next;        	// ָ��ֱ�Ӻ����
// }link_snake, p_link_snake;
//
// /**********************
//  * GLOBAL PROTOTYPES
//  **********************/
// void lv_100ask_stm32_game_snake(void);
//
//
//
//
//
// /**********************
//  *      MACROS
//  **********************/
//
// #ifdef __cplusplus
// } /* extern "C" */
// #endif
//
// #endif /* LV_100ASK_STM32_GAME_SANKE_H */
//
//
//
//
//
