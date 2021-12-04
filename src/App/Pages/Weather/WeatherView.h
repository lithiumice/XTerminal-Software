#ifndef __Weather_VIEW_H
#define __Weather_VIEW_H

#include "../Page.h"

extern "C"
{
	extern lv_img_dsc_t love_1;
extern lv_img_dsc_t love_10;
extern lv_img_dsc_t love_11;
extern lv_img_dsc_t love_12;
extern lv_img_dsc_t love_13;
extern lv_img_dsc_t love_14;
extern lv_img_dsc_t love_15;
extern lv_img_dsc_t love_16;
extern lv_img_dsc_t love_17;
extern lv_img_dsc_t love_18;
extern lv_img_dsc_t love_19;
extern lv_img_dsc_t love_2;
extern lv_img_dsc_t love_20;
extern lv_img_dsc_t love_21;
extern lv_img_dsc_t love_22;
extern lv_img_dsc_t love_23;
extern lv_img_dsc_t love_24;
extern lv_img_dsc_t love_25;
extern lv_img_dsc_t love_26;
extern lv_img_dsc_t love_27;
extern lv_img_dsc_t love_28;
extern lv_img_dsc_t love_29;
extern lv_img_dsc_t love_3;
extern lv_img_dsc_t love_30;
extern lv_img_dsc_t love_4;
extern lv_img_dsc_t love_5;
extern lv_img_dsc_t love_6;
extern lv_img_dsc_t love_7;
extern lv_img_dsc_t love_8;
extern lv_img_dsc_t love_9;

	extern lv_img_dsc_t weather_0;
	extern lv_img_dsc_t weather_9;
	extern lv_img_dsc_t weather_14;
	extern lv_img_dsc_t weather_5;
	extern lv_img_dsc_t weather_25;
	extern lv_img_dsc_t weather_30;
	extern lv_img_dsc_t weather_26;
	extern lv_img_dsc_t weather_11;
	extern lv_img_dsc_t weather_23;
	extern lv_img_dsc_t man_0;
	extern lv_img_dsc_t man_1;
	extern lv_img_dsc_t man_2;
	extern lv_img_dsc_t man_3;
	extern lv_img_dsc_t man_4;
	extern lv_img_dsc_t man_5;
	extern lv_img_dsc_t man_6;
	extern lv_img_dsc_t man_7;
	extern lv_img_dsc_t man_8;
	extern lv_img_dsc_t man_9;
	extern lv_img_dsc_t temp;
	extern lv_img_dsc_t humi;
}

namespace Page
{

	class WeatherView
	{
	public:
		void Create(lv_obj_t *root);
		void Delete();

	public:
		struct
		{
			lv_group_t *group;

			lv_style_t style_bg;
			lv_style_t style_indic;
			lv_style_t default_style;
			lv_style_t chFont_style;
			lv_style_t numberSmall_style;
			lv_style_t numberBig_style;
			lv_style_t btn_style;
			lv_style_t bar_style;

			lv_obj_t *weatherImg;
			lv_obj_t *cityLabel;
			lv_obj_t *btn;
			lv_obj_t *airLabel;
			lv_obj_t *weatherLabel;
			lv_obj_t *txtLabel;

			lv_obj_t *clockLabel_1;
			lv_obj_t *clockLabel_2;
			lv_obj_t *dateLabel;

			lv_obj_t *tempImg;
			lv_obj_t *tempBar;
			lv_obj_t *tempLabel;

			lv_obj_t *humiImg;
			lv_obj_t *humiBar;
			lv_obj_t *humiLabel;

			lv_obj_t *spaceImg;
			// lv_chart_series_t* ser1, * ser2;
			// lv_obj_t* chart, * titleLabel;

		} ui;

		void SetDailyWeather(
			const char *cityName,
			const int weatherLevel,
			const int airLevel,
			const int temperatue,
			const int huminature,
			const int minTemp,
			const int maxTemp,
			const int windDir,
			const int windLevel);

		void SetCityName();
		void SetAirLevel();
		void SetClockTime();
		void SetHumiTemp();

		void weather_obj_del(void);
		void weather_gui_del(void);
		void weather_group_add(lv_obj_t *root);
		void weather_style_init(void);
		void weather_gui_init(lv_obj_t *root);

		void SetCityName(const char *cityName);

		void SetWeather(const char *weather_pinyin);

		void SetWeather(const int weatherCode);
		void SetAirLevel(const int airLevel);

		void SetTemperatue(const int temperatue);

		void SetHuminature(const int huminature);

		void SetTextInfo(const int minTemp, const int maxTemp, const char *windDir, const int windLevel);

		void SetClockDay(const int month, const int day);

		void SetClockHour(const int hour, const int min);

		void SetClockSec(const int sec);

		void *weaImage_map[9] = {
			&weather_0, &weather_9, &weather_14, &weather_5, &weather_25,
			&weather_30, &weather_26, &weather_11, &weather_23};
		void *manImage_map[10] = {
			&man_0, &man_1, &man_2, &man_3, &man_4,
			&man_5, &man_6, &man_7, &man_8, &man_9};
		lv_img_dsc_t* loveImage_map[30] = {
&love_1,
&love_10,
&love_11,
&love_12,
&love_13,
&love_14,
&love_15,
&love_16,
&love_17,
&love_18,
&love_19,
&love_2,
&love_20,
&love_21,
&love_22,
&love_23,
&love_24,
&love_25,
&love_26,
&love_27,
&love_28,
&love_29,
&love_3,
&love_30,
&love_4,
&love_5,
&love_6,
&love_7,
&love_8,
&love_9,
};
	};

}

#endif // !__VIEW_H
