#ifndef DATE_H
#define DATE_H
//author:霄
#include<time.h>
#include"slist.h"
#define DAY 86400

class Date 
{
public:
	Date(short da = 0) :date(da) {};
	void set_date();//取现在时间作为日期
	char* get_date();//格式："YYYY-MM-DD"
	bool istoday();
	short get_short();
private:
	short date = 0;//仅包含日期，不包含具体时间的时间戳
	//char* get_date_now();
	//char* get_time_now();
};

class DateList:public Slist<Date>
{
	
};

#endif // !DATE_H
