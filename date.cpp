#ifndef DATE_CPP
#define DATE_CPP
#include"date.h"
#include <stdio.h>
#include <time.h>

void Date::set_date()
{
	time_t t;
	struct tm *p;
	t = time(0) + 28800;
	t /= DAY;
	date = t;
}

//char* Date::get_date_now()
//{
//	time_t t;
//	struct tm *p;
//	t = time(0)+28800;
//	p = gmtime(&t);
//	char *s=new char[11];//11���ַ������а������ַ�
//	strftime(s, sizeof(s), "%Y-%m-%d", p);
//}
//
//char* Date::get_time_now()
//{
//	time_t t;
//	struct tm *p;
//	t = time(0) + 28800;
//	p = gmtime(&t);
//	char *s = new char[20];//20���ַ������а������ַ�
//	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", p);
//
//}

char* Date::get_date()
{
	time_t t;
	struct tm *p=nullptr;
	t = date * DAY;
	p=gmtime(&t);
	static char s[11];//11���ַ������а������ַ�
	strftime(s, 11, "%Y-%m-%d", p);
	return s;
}

bool Date::istoday()
{
	short date_today;
	date_today = (time(0) + 28800) / DAY;
	return date_today == date;
}

short Date::get_short() { return date; }

#endif // !DATE_CPP