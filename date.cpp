#ifndef DATE_CPP
#define DATE_CPP
#include"date.h"
#include <stdio.h>
#include <time.h>

void Date::set_date()
{
	time_t t;
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
//	char *s=new char[11];//11个字符，其中包括空字符
//	strftime(s, sizeof(s), "%Y-%m-%d", p);
//}
//
//char* Date::get_time_now()
//{
//	time_t t;
//	struct tm *p;
//	t = time(0) + 28800;
//	p = gmtime(&t);
//	char *s = new char[20];//20个字符，其中包括空字符
//	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", p);
//
//}

string Date::get_date()
{
	time_t t;
	struct tm p;
	t = date * DAY;
	gmtime_s(&p, &t);
	static char s[11];//11个字符，其中包括空字符
	strftime(s, 11, "%Y-%m-%d", &p);
	string d = s;
	return d;
}

Date_concrete Date::get_date_concrete()
{
	Date_concrete d;
	string s = get_date();
	d.year = (s[0] - '0') * 1000 + (s[1] - '0') * 100 + (s[2] - '0') * 10 + s[3] - '0';
	d.month = (s[5] - '0') * 10 + s[6] - '0';
	d.day = (s[8] - '0') * 10 + s[9] - '0';
	return d;
}

bool Date::istoday()
{
	short date_today;
	date_today = (time(0) + 28800) / DAY;
	return date_today == date;
}

short Date::get_short() { return date; }
#endif // !DATE_CPP