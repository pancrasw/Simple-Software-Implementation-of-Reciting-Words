#ifndef ACCOUNT_H
#define ACCOUNT_H
#include"md5_encode.h"
#include<cstring>
#include"wordlist.h"
#include<iostream>
#include<fstream>
#include<vector>
#define ACCOUNT
//@author:霄
using std::vector;
class Account
{
public:
	Account() {};
	~Account();
protected:
	char vid;//序列号
	char na[20];//名字，限定字符数
	string md5;//密码md5
	static Md5Encode encode_obj;//md5编码器
	Account(char, char*, string);
protected:
	short go;//每日目标
	friend class AccountManageSystem;
};

class Account_running :public Account//运行时的用户
{
public:
	WordList wl_done;//已背的词汇表
	WordList_daily wl_daily;//今日任务词汇表
	Account_running(Account);
	~Account_running();
	void setpassword(string);//修改密码
	void setgoal(int);//设置每日目标
	int known_t();//已会单词共计
	int learning_t();//正在学共计
	int word_goal();//目标单词
	int word_rest();//剩余单词
	int word_new();//新词个数
	bool complete();//今日打卡与否
	int complete_t();//打卡天数统计
};

class AccountManageSystem 
{
public:
	char**namelist;//所有用户的名字表
	static Account_running* ar;//正在运行的用户
	static vector<Account> account_list;//用户表
	AccountManageSystem();
	~AccountManageSystem();
	bool sign_in(char,string);//验证密码
	void new_account(char*,string);
};

#endif // !ACCOUNT_H
