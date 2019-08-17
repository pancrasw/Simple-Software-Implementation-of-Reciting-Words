#ifndef WORDLIST_H
#define WORDLIST_H
#include<string>
#include"slist.h"
#include"date.h"
//@author:霄

struct Wordnode 
{
	short id;//单词序列号
	enum {wrong,undone,right,done };
	enum {none,first,second,third,fourth,fifth};
	char st=undone;//背的状态
	char ti = none;//背的次数
	Date da;//日期
	Wordnode() {};
	Wordnode(short wid, char state, char time, Date date) :
		id(wid),
		st(state),
		ti(time),
		da(date)
	{}
};

class WordList :public Slist<Wordnode>
{
public:
	WordList();
	~WordList();
	void addword(short,char,char,short);//添加单词
	void addword(Wordnode);
	Wordnode pop();//获取单词表中第一个单词
	int count_total();
	int count_undone();
	int count_done();
	int count_wrong();
	int count_right();
protected:
	int undone_t=0;//当天还没开始背的
	int done_t = 0;//已经背过了的
	int wrong_t = 0;//背错了的
	int right_t = 0;//背错了只背对了一次的
private://禁用的函数
	void plusmerge() {};
	void show() {};
	void operator+() {};
};

class WordList_daily:public WordList
{
public:
	void check(bool);
	Date da;
private:
	void put_tail();//将单词放到末尾
};

#endif