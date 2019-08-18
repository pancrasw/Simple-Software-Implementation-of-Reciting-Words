#ifndef ACCOUNT_CPP
#define ACCOUNT_CPP
#include"account.h"
#include<iomanip>
//@author:霄
Md5Encode Account::encode_obj;

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;

void str_copy(char*destination,char*source) 
{
	for (int i = 0; i < NAME; i++) 
	{
		destination[i] = source[i];
	}
}

Account::Account(char id, string name, string md) :
	vid(id),
	md5(md),
	na(name)
{
}

Account::~Account()
{

}

Account_running::Account_running(Account ac):
	Account(ac)
{
	int a = vid;
	wv_undone = new vector<Wordnode>*[4];
	for (int i = 0; i < 4; i++)
	{
		wv_undone[i] = new vector<Wordnode>;
	}
	string file_name = string("0" + a) + string(".dat");
	ifstream ifs(file_name.c_str(), ios_base::binary);
	char ch = ifs.get();
	if (!ifs.is_open() || ifs.bad() || (ifs.eof()))
		return;
	ifs.putback(ch);
	short* id=new short;
	char* st=new char;
	char* ti=new char;
	short* da = new short;
	for (; ifs;) 
	{
		ifs.read((char*)id, 2);
		ifs.read((char*)st, 1);
		ifs.read((char*)ti, 1);
		ifs.read((char*)da, 2);
		Date d(*da);
		Wordnode w(*id, *st, *ti, d);
		switch (*ti)
		{
		case Wordnode::none:wl_new.addword(w);
		case Wordnode::first:wv_undone[0]->push_back(w);
		case Wordnode::second:wv_undone[1]->push_back(w);
		case Wordnode::third:wv_undone[2]->push_back(w);
		case Wordnode::fourth:wv_undone[3]->push_back(w);
		case Wordnode::fifth:wl_done.addword(*id, *st, *ti, *da);
		}
	}
	ifs.close();
	delete id, st, ti, da;
}

Account_running::~Account_running() 
{
	int b = vid;
	string file_name = string("0" + b) + string(".dat");
	ofstream ofs(file_name.c_str(), ios_base::binary);
	Wordnode w;
	for (; wl_done.count_total() != 0;)
	{
		w = wl_done.pop();
		wl_done.delete_node(0);
		ofs.write((char*)&w.id, 2);
		ofs.write((char*)&w.st, 1);
		ofs.write((char*)&w.ti, 1);
		ofs.write((char*)&w.da, 2);
	}
	for (; wl_new.count_total() != 0;)
	{
		w = wl_new.pop();
		wl_new.delete_node(0);
		ofs.write((char*)&w.id, 2);
		ofs.write((char*)&w.st, 1);
		ofs.write((char*)&w.ti, 1);
		ofs.write((char*)&w.da, 2);
	}
	for (int i = 0; i < 4; i++) 
	{
		for (int j = (*(wv_undone[i])).size() ; j > 0; j--)
		{
			w = (*(wv_undone[i])).back();
			(*(wv_undone[i])).pop_back();
			ofs.write((char*)&w.id, 2);
			ofs.write((char*)&w.st, 1);
			ofs.write((char*)&w.ti, 1);
			ofs.write((char*)&w.da, 2);
		}
	}
	ofs.close();
	for (int i = 0; i < 4; i++)
	{
		delete wv_undone[i];
	}
	delete[]wv_undone;
	string fn = string("0" + b) + string("1.dat");//存放每日任务
	ofs.open(fn.c_str(), ios_base::binary);
	Date da;
	da.set_date();
	short a = da.get_short();
	ofs.write((char*)&a, 2);
	for (; wl_daily.count_total() != 0;)
	{
		w = wl_daily.pop();
		wl_daily.delete_node(0);
		ofs.write((char*)&w.id, 2);
		ofs.write((char*)&w.st, 1);
		ofs.write((char*)&w.ti, 1);
		ofs.write((char*)&w.da, 2);
	}
	ofs.close();
}

void Account_running::setpassword(string password) 
{
	md5 = encode_obj.Encode(password);
};

void Account_running::setgoal(int goal)
{
	go = goal;
}

int Account_running::known_t()
{
	return wl_done.count_total();
}

void Account_running::create_daily_wordlist() 
{
	int a = vid;
	string fn = string("0" + a) + string("1.dat");//每日任务单词
	short* id = new short;
	char* st = new char;
	char* ti = new char;
	short* da = new short;
	ifstream ifs(fn.c_str(), ios_base::binary);
	char ch = ifs.get();
	bool ifs_ok = true;
	if (!ifs.is_open() || ifs.bad() || (ifs.eof()))
		ifs_ok = false;
	ifs.putback(ch);
	ifs.read((char*)da, 2);
	Date d(*da);
	if (d.istoday() && ifs_ok)
	{
		for (; ifs;)
		{
			ifs.read((char*)id, 2);
			ifs.read((char*)st, 1);
			ifs.read((char*)ti, 1);
			ifs.read((char*)da, 2);
			Date d(*da);
			Wordnode w(*id, *st, *ti, d);
			wl_daily.addword(w);
		}
	}
	else 
	{
		for (; ifs&&ifs_ok;)
		{
			ifs.read((char*)id, 2);
			ifs.read((char*)st, 1);
			ifs.read((char*)ti, 1);
			ifs.read((char*)da, 2);
			Date d(*da);
			Wordnode w(*id, *st, *ti, d);
			switch (*ti)
			{
			case Wordnode::none:wl_new.addword(w);
			case Wordnode::first:wv_undone[0]->push_back(w);
			case Wordnode::second:wv_undone[1]->push_back(w);
			case Wordnode::third:wv_undone[2]->push_back(w);
			case Wordnode::fourth:wv_undone[3]->push_back(w);
			case Wordnode::fifth:wl_done.addword(*id, *st, *ti, *da);
			}
		}
		for (int i = 3; i >= 0; i--) 
		{
			int count = wv_undone[i]->size();
			for (int j = count - 1; j >= 0; j--) 
			{
				Wordnode w = (*(wv_undone[i]))[j];
				if (w.isEbbinghaus()) 
				{
					wl_daily.addword(w);
					(*(wv_undone[i])).erase((*(wv_undone[i])).begin() + j);
				}
				if (wl_daily.count_total() == go) break;
			}
			if (wl_daily.count_total() == go) break;
		}
		if (wl_daily.count_total() != go) 
		{
			for (; wl_daily.count_total() != go && wl_new.count_total() != 0;)
			{
				Wordnode w = (wl_new.access(0))->data;
				wl_daily.addword(w);
				wl_new.delete_node(0);
			}
		}
	}
	wl_daily.shuffle();
	ifs.close();
	delete id, st, ti, da;
};

int Account_running::learning_t() 
{
	int count = 0;
	for (int i = 0; i < 4; i++) 
	{
		count += wv_undone[i]->size();
	}
	return count;
}

int Account_running::word_goal()
{
	return go;
}

int Account_running::word_rest() 
{
	if (wl_daily.count_total() == 0) return 0;
	return wl_daily.count_total() - wl_daily.count_done();
}

int Account_running::word_new() 
{
	return wl_daily.count_new();
}

void Account_running::complete() 
{
	Date da;
	da.get_date();
	date_list.operator+(da);
}

bool Account_running::iscomplete() 
{
	Date da = date_list.pop().data;
	return da.istoday();
}

int Account_running::complete_t()
{
	return date_list.count();
}

void Account_running::add_new_word(short vid)
{
	Date da;
	da.set_date();
	Wordnode w(vid, Wordnode::undone, Wordnode::none, da);
	wl_new.addword(w);
}

void AccountManageSystem::init() 
{
	ifstream file_is("accountlist.dat", ios_base::binary);
	char ch = file_is.get();
	if (!file_is.is_open()|| file_is.bad()||(file_is.eof()))
		return;
	file_is.putback(ch);
	char vid;
	char na[21];
	string md5;
	char tMd5[33];
	tMd5[32] = 0;
	short go;
	while (!file_is.eof())
	{
		file_is.read((char*)&vid,1);
		file_is.read((char*)&na, NAME);
		file_is.read((char*)tMd5, 32);
		file_is.read((char*)&go, 2);
		md5 = string(tMd5);
		Account a(vid, string(na), md5);
		a.go = go;
		account_list.push_back(a);
	}
}

void AccountManageSystem::end() 
{
	ofstream os("accountlist.dat",ios_base::binary|ios_base::trunc);
	if (account_list.size() != 0) 
	{
		for (char i = 0; i < account_list.size(); i++)
		{
			os.write(&(account_list[i]).vid, 1);
			os.write((account_list[i]).na.c_str(), NAME);
			os.write((account_list[i]).md5.c_str(), 32);
			os.write((char*)&(account_list[i]).go, 2);
		}
	}
}

bool AccountManageSystem::sign_in(char vid, string password)
{
	if (vid > account_list.back().vid)return false;
	if (account_list[vid].md5 == Account::encode_obj.Encode(password))
	{
		ar = new Account_running(account_list[vid]);// 发生错误：写入堆内存出错
		return true;
	}
	return false;
}

bool AccountManageSystem::new_account(string name,string password) 
{
	for (int i = 0; i < account_list.size(); i++) 
	{
		if (account_list[i].na == name)return false;
	}
	string md5 = Account::encode_obj.Encode(password);

	int vid = 0;
	if (account_list.size() > 0)
	{
		vid = account_list.back().vid + 1;
	}
	else
	{
		vid = 0;
	}

	Account a(vid, name, md5);
	account_list.push_back(a);
	sign_in(vid, password);
	return true;
}

bool AccountManageSystem::delete_account(char vid)
{
	if (ar == nullptr || ar->vid != vid)return false;
	string path = account_list[getindex(vid)].na + string(".dat");
	remove(path.c_str());
	path = account_list[getindex(vid)].na + string("1.dat");
	remove(path.c_str());
	int i = 0;
	for (; (account_list[i].vid != vid) || (i > account_list.size()); i++);
	if (i > account_list.size()) { return false; }
	account_list.erase(account_list.begin() + i);
	return true;
}

string AccountManageSystem::showname(char vid) 
{
	return account_list[vid].na;
}

bool AccountManageSystem::sign_out()
{
	if (ar != nullptr)
	{
		delete ar;
		ar = nullptr;

		//
	}
	return true;
}

bool AccountManageSystem::sign_in(string account, std::string password)
{
	int account_index = -1;
	for (int i = 0; i < account_list.size(); i++)
	{
		if (account_list[i].na == account)
		{
			account_index = i;
			break;
		}
	}
	if (account_index < 0)
		return false;
	else
		return sign_in((unsigned char)account_index, password);
}


bool AccountManageSystem::delete_current_account()
{
	if (ar != nullptr)
	{
		for (int i = 0; i < AccountManageSystem::account_list.size(); i++)
		{
			if (ar->vid == account_list[i].vid)
			{
				AccountManageSystem::delete_account(i);
				return true;
			}
		}
	}
	return false;
}

string AccountManageSystem::get_current_user_name()
{
	if (AccountManageSystem::ar != nullptr)
		return ar->na;
	else
		return std::string();
}


vector<std::string> AccountManageSystem::get_all_users_name()
{
	std::vector<std::string> result;
	for (auto n : account_list)
		result.push_back(n.na);
	return result;
}

char AccountManageSystem::getindex(char vid)
{
	for (int i = 0; i < account_list.size(); i++) 
	{
		if (account_list[i].vid == vid)return i;
	}
}

Account_running* AccountManageSystem::ar = nullptr;

vector<Account> AccountManageSystem::account_list;

#endif // !ACCOUNT_CPP