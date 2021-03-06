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
	for (int i = 0; i < ACCOUNT_NAME; i++)
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
	string file_name = string(to_string(a)) + string(".dat");
	ifstream ifs(file_name.c_str(), ios_base::binary);
	char ch = ifs.get();
	if (!ifs.is_open() || ifs.bad() || (ifs.eof()))
		return;
	ifs.putback(ch);
	short* id=new short;
	char* st=new char;
	char* ti=new char;
	short* da = new short;
	bool ifs_ok = true;
	for (; ifs&&ifs_ok;) 
	{
		char ch = ifs.get();
		if (!ifs.is_open() || ifs.bad() || (ifs.eof()))
		{
			ifs_ok = false;
			break;
		}
		ifs.putback(ch);
		ifs.read((char*)id, 2);
		ifs.read((char*)st, 1);
		ifs.read((char*)ti, 1);
		ifs.read((char*)da, 2);
		Date d(*da);
		Wordnode w(*id, *st, *ti, d);
		switch (*ti)
		{
		case Wordnode::none:wl_new.addword(w); break;
		case Wordnode::first:wv_undone[0]->push_back(w); break;
		case Wordnode::second:wv_undone[1]->push_back(w); break;
		case Wordnode::third:wv_undone[2]->push_back(w); break;
		case Wordnode::fourth:wv_undone[3]->push_back(w); break;
		case Wordnode::fifth:wl_done.addword(*id, *st, *ti, *da); break;
		}
	}
	ifs.close();
	delete id, st, ti, da;
}

Account_running::~Account_running() 
{
	int b = vid;
	string file_name = string(to_string(b)) + string(".dat");
	ofstream ofs(file_name.c_str(), ios_base::binary);
	Wordnode w;
	for (; wl_done.count_total() != 0;)
	{
		w = wl_done.pop();

		//w.da = Date(w.da.get_short() - 1);//测试日期用

		wl_done.delete_node(0);
		ofs.write((char*)&w.id, 2);
		ofs.write((char*)&w.st, 1);
		ofs.write((char*)&w.ti, 1);
		ofs.write((char*)&w.da, 2);
	}
	for (; wl_new.count_total() != 0;)
	{
		w = wl_new.pop();

	    //w.da = Date(w.da.get_short() - 1);//测试日期用

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
			w = (*(wv_undone[i]))[0];

			//w.da = Date(w.da.get_short() - 1);//测试日期用

			(*(wv_undone[i])).erase((*(wv_undone[i])).begin());
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
	if (wl_daily.count_total() != 0)
	{
		string fn = string(to_string(b)) + string("1.dat");//存放每日任务
		ofs.open(fn.c_str(), ios_base::binary);
		Date da;
		da.set_date();
		short a = da.get_short();

		//a--;//测试日期用

		ofs.write((char*)&a, 2);
		for (; wl_daily.count_total() != 0;)
		{
			w = wl_daily.pop();

			//w.da = Date(w.da.get_short() - 1);//测试日期用

			wl_daily.delete_node(0);
			ofs.write((char*)&w.id, 2);
			ofs.write((char*)&w.st, 1);
			ofs.write((char*)&w.ti, 1);
			ofs.write((char*)&w.da, 2);
		}
		ofs.close();
	}
}

int Account_running::known_t()
{
	return wl_done.count_total();
}

void Account_running::create_daily_wordlist() 
{
	int a = vid;
	string fn = string(to_string(a)) + string("1.dat");//每日任务单词
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
		for (; ifs&&ifs_ok;)
		{
			char ch = ifs.get();
			if (!ifs.is_open() || ifs.bad() || (ifs.eof()))
			{
				ifs_ok = false;
				break;
			}
			ifs.putback(ch);
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
			char ch = ifs.get();
			if (!ifs.is_open() || ifs.bad() || (ifs.eof()))
			{
				ifs_ok = false;
				break;
			}
			ifs.putback(ch);
			ifs.read((char*)id, 2);
			ifs.read((char*)st, 1);
			ifs.read((char*)ti, 1);
			ifs.read((char*)da, 2);

			//*da -= 1;//测试用

			Date d(*da);
			Wordnode w(*id, Wordnode::undone, *ti, d);//过一天后读取默认重新背，重置st
			switch (*ti)
			{
			case Wordnode::none:wl_new.addword(w); break;
			case Wordnode::first:wv_undone[0]->push_back(w); break;
			case Wordnode::second:wv_undone[1]->push_back(w); break;
			case Wordnode::third:wv_undone[2]->push_back(w); break;
			case Wordnode::fourth:wv_undone[3]->push_back(w); break;
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

bool Account_running::complete() 
{
	if ((wl_daily.count_total() != 0) && (wl_daily.count_total() == wl_daily.count_done()))
	{
		Date da;
		da.set_date();
		date_list.operator+(da);
		return true;
	}
	return false;
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

//添加单词至用户目标单词

void Account_running::save()
{
	int b = vid;
	string file_name = string(to_string(b)) + string(".dat");
	ofstream ofs(file_name.c_str(), ios_base::binary);
	Wordnode w;
	for (int i = 0; i < wl_done.count_total(); i++)
	{
		w = wl_done.access(i)->data;
		ofs.write((char*)&w.id, 2);
		ofs.write((char*)&w.st, 1);
		ofs.write((char*)&w.ti, 1);
		ofs.write((char*)&w.da, 2);
	}
	for (int i = 0; i < wl_new.count_total(); i++)
	{
		w = wl_new.access(i)->data;
		ofs.write((char*)&w.id, 2);
		ofs.write((char*)&w.st, 1);
		ofs.write((char*)&w.ti, 1);
		ofs.write((char*)&w.da, 2);
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < (*(wv_undone[i])).size(); j++)
		{
			w = (*(wv_undone[i]))[j];
			ofs.write((char*)&w.id, 2);
			ofs.write((char*)&w.st, 1);
			ofs.write((char*)&w.ti, 1);
			ofs.write((char*)&w.da, 2);
		}
	}
	ofs.close();
	string fn = string(to_string(b)) + string("1.dat");//每日任务单词
	ofs.open(fn.c_str(), ios_base::binary);
	Date da;
	da.set_date();
	short a = da.get_short();
	ofs.write((char*)&a, 2);
	for (int i = 0; i < wl_daily.count_total(); i++)
	{
		w = wl_daily.access(i)->data;
		ofs.write((char*)&w.id, 2);
		ofs.write((char*)&w.st, 1);
		ofs.write((char*)&w.ti, 1);
		ofs.write((char*)&w.da, 2);
	}
	ofs.close();
}

void Account_running::read_dl()
{
	string fn = to_string(vid) + string("2.dat");
	ifstream ifs(fn.c_str(), ios_base::binary);
	char ch = ifs.get();
	bool ifs_ok = true;
	if (!ifs.is_open() || ifs.bad() || (ifs.eof()))
		ifs_ok = false;
	ifs.putback(ch);
	short da = 0;
	for (; ifs&&ifs_ok;)
	{
		ifs.read((char*)&da, 2);
		Date d(da);
		date_list.operator+(d);
		ch = ifs.get();
		if (!ifs.is_open() || ifs.bad() || (ifs.eof()))
			ifs_ok = false;
		ifs.putback(ch);
	}
}

void Account_running::save_dl()
{
	string fn = to_string(vid) + string("2.dat");
	ofstream ofs(fn.c_str(), ios_base::binary);
	Snode<Date>*ptr = date_list.access(0);
	short d;
	for (; ; ptr = ptr->next)
	{
		d = ptr->data.get_short();
		ofs.write((char*)&d, 2);
		if (ptr->next == nullptr)break;
	}
}

void AccountManageSystem::init() 
{
	ifstream ifs("accountlist.dat", ios_base::binary);
	char ch = ifs.get();
	if (!ifs.is_open()|| ifs.bad()||(ifs.eof()))
		return;
	ifs.putback(ch);
	char vid;
	char na[21];
	string md5;
	char tMd5[33];
	tMd5[32] = 0;
	short go;
	while (!ifs.eof())
	{
		char ch = ifs.get();
		if (!ifs.is_open() || ifs.bad() || (ifs.eof()))
			return;
		ifs.putback(ch);
		ifs.read((char*)&vid,1);
		ifs.read((char*)&na, ACCOUNT_NAME);
		ifs.read((char*)tMd5, 32);
		ifs.read((char*)&go, 2);
		md5 = string(tMd5);
		Account a(vid, string(na), md5);
		a.go = go;
		account_list.push_back(a);
	}
}

void AccountManageSystem::save()
{

	ofstream os("accountlist.dat", ios_base::binary | ios_base::trunc);
	if (account_list.size() != 0)
	{
		int count = account_list.size();
		for (char i = 0; i < count; i++)
		{
			os.write(&(account_list[i]).vid, 1);
			os.write((account_list[i]).na.c_str(), ACCOUNT_NAME);
			os.write((account_list[i]).md5.c_str(), 32);
			os.write((char*)&(account_list[i]).go, 2);
		}
	}
}

bool AccountManageSystem::sign_in(char vid, string password)
{
	if (vid > account_list.back().vid)return false;
	if (account_list[getindex(vid)].md5 == Account::encode_obj.Encode(password))
	{
		ar = new Account_running(account_list[getindex(vid)]);// 发生错误：写入堆内存出错
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
	save();
	return true;
}

bool AccountManageSystem::delete_account(char vid)
{
	int i = getindex(vid);
	if (i < 0 || (!sign_out())) { return false; }
	string path = string(to_string(vid)) + string(".dat");
	remove(path.c_str());
	path = string(to_string(vid)) + string("1.dat");
	remove(path.c_str());
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
		return true;
	}
	return false;
}

bool AccountManageSystem::sign_in(string account, std::string password)
{
	int account_vid = -1;
	for (int i = 0; i < account_list.size(); i++)
	{
		if (account_list[i].na == account)
		{
			account_vid = account_list[i].vid;
			break;
		}
	}
	if (account_vid < 0)
		return false;
	else
		return sign_in((unsigned char)account_vid, password);
}


bool AccountManageSystem::delete_current_account()
{
	if (ar != nullptr)
	{
		return delete_account(ar->vid);		
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

bool AccountManageSystem::setpassword(string password)
{
	if (ar == nullptr)return false;
	Account* ac = &account_list[getindex(ar->vid)];
	ar->md5 = ac->md5 = Account::encode_obj.Encode(password);
	return true;
}

bool AccountManageSystem::setgoal(short go)
{
	if (ar == nullptr)return false;
	Account* ac = &account_list[getindex(ar->vid)];
	ar->go = ac->go = go;
	return true;
}

bool AccountManageSystem::end()
{
	save();
	account_list.clear();
	return sign_out();
}

char AccountManageSystem::getindex(char vid)
{
	for (int i = 0; i < account_list.size(); i++) 
	{
		if (account_list[i].vid == vid)return i;
	}
	return -1;
}

Account_running* AccountManageSystem::ar = nullptr;

vector<Account> AccountManageSystem::account_list;

#endif // !ACCOUNT_CPP