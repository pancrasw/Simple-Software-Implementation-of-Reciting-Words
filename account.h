#ifndef ACCOUNT_H
#define ACCOUNT_H
#include"md5_encode.h"
#include<cstring>
#include"wordlist.h"
#include<iostream>
#include<fstream>
#include<vector>
#include<cstdio>
#define ACCOUNT_WORD 4
#define ACCOUNT_NAME 21
//@author:霄
using std::vector;
using std::string;

class Account
{
public:
	Account() {};
	~Account();
protected:
	char vid;//序列号
	string na;//名字，限定字符数
	string md5;//密码md5
	static Md5Encode encode_obj;//md5编码器
	Account(char, string, string);
protected:
	short go = 50;//每日目标
	friend class AccountManageSystem;
};

class Account_running :public Account//运行时的用户
{
public:
	WordList wl_done;//已背的词汇表
	vector<Wordnode>** wv_undone;//未背的词汇表
	WordList wl_new;//新单词
	WordList_daily wl_daily;//今日任务词汇表
	Slist<Date> date_list;//打卡日期
	Account_running(Account);
	~Account_running();
	int	 known_t();//已会单词共计
	void create_daily_wordlist();
	int  learning_t();//正在学共计
	int  word_goal();//目标单词
	int  word_rest();//剩余单词
	int  word_new();//新词个数
	bool complete();//打卡
	bool iscomplete();//今日打卡与否
	int  complete_t();//打卡天数统计
	void add_new_word(short);//添加单词至用户目标单词
	void save();	
	void read_dl();
	void save_dl();
private:
	
	friend class AccountManageSystem;
};

class AccountManageSystem 
{
public:
	static Account_running* ar;//正在运行的用户
	static vector<Account> account_list;//用户表
	static string path;//所在文件夹路径
	static void           init();
	static void           save();
	static bool           sign_in(char,string);//验证密码
	static bool           new_account(string,string);
	static string         showname(char);
	static bool           sign_out();                                        //登出
	static bool           sign_in(std::string account, std::string password);//登录
	static bool           delete_current_account();                          //删除当前用户
	static string         get_current_user_name();                           //获取当前用户名称
	static vector<string> get_all_users_name();//获取所有用户名称
	static bool           setpassword(string);
	static bool           setgoal(short); 
	static bool           end();
private:
	static bool           delete_account(char);
	static char           getindex(char);
};

#endif // !ACCOUNT_H