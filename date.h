#ifndef DATE_H
#define DATE_H
//author:霄
#include<time.h>
#include"slist.h"
#define DAY 86400

struct Date_concrete 
{
	short year;
	short month;
	short day;
};

class Date 
{
public:
	Date(short da = 0) :date(da) {};
	void set_date();//取现在时间作为日期
	string get_date();//格式："YYYY-MM-DD"
	Date_concrete get_date_concrete();
	bool istoday();
	short get_short();
private:
	short date = 0;//仅包含日期，不包含具体时间的时间戳
	//char* get_date_now();
	//char* get_time_now();
};

#endif // !DATE_H
