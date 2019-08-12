#ifndef ACCOUNT_CPP
#define ACCOUNT_CPP
#include"account.h"
#include<iomanip>
//@author:霄
Md5Encode Account::encode_obj;

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

	// 文件名最长为24B
	char* file_name = new char[25];
	strcpy_s(file_name,20, na.c_str());//na最长为19B
	int tIndex = 0;
	//
	for (int i = 0; i < 24; i++)
	{
		if (tIndex == '\0')
		{
			tIndex = i;
			break;
		}
	}
	char suffix[] = ".dat";
	strcpy_s(file_name + tIndex, 5, suffix);

	ifstream ifs(file_name, ios_base::binary);
	delete[] file_name;
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
		if ((!d.istoday())&&(*ti!=Wordnode::fifth)) 
		{
			wl_daily.addword(*id, *st, *ti, *da);
			continue;
		}
		wl_done.addword(*id, *st, *ti, *da);
	}
	delete id, st, ti, da;
}

Account_running::~Account_running() 
{
	char* file_name = new char[24];
	strcpy_s(file_name,24, na.c_str());
	char*ptr;
	for (ptr = file_name; (*ptr) != 0; ptr++);
	char suffix[] = ".dat";
	strcpy_s(ptr,24, suffix);
	ofstream ofs(file_name, ios_base::binary);
	delete file_name;
	Wordnode w;
	for (int i=0;i<wl_done.count_total();i++) 
	{
		w = wl_done.pop();
		wl_done.delete_node(0);
		ofs.write((char*)&w.id, 2);
		ofs.write((char*)&w.st, 1);
		ofs.write((char*)&w.ti, 1);
		ofs.write((char*)&w.da, 2);
	}
	for (int i = 0; i < wl_daily.count_total(); i++)
	{
		w = wl_daily.pop();
		wl_daily.delete_node(0);
		ofs.write((char*)&w.id, 2);
		ofs.write((char*)&w.st, 1);
		ofs.write((char*)&w.ti, 1);
		ofs.write((char*)&w.da, 2);
	}
}

void Account_running::setpassword(string password) 
{
	md5 = encode_obj.Encode(password);
};

bool AccountManageSystem::sign_in(char vid,string password)
{	
	if (account_list[unsigned(vid)].md5 == Account::encode_obj.Encode(password))
	{
		ar = new Account_running(account_list[unsigned(vid)]);// 发生错误：写入堆内存出错
		return true;
	}
	return false;
}

void Account_running::setgoal(int goal)
{
	go = goal;
}



AccountManageSystem::AccountManageSystem() 
{
	ifstream file_is("accountlist.dat", ios_base::binary);

	if (!file_is.is_open()|| file_is.bad())
		return;

	char vid;
	char na[21];
	string md5;
	char tMd5[33];
	tMd5[32] = 0;
	int i = 0;
	while (!file_is.eof())
	{
		file_is.read((char*)&vid,1);
		if (vid != i)return;
		file_is.read((char*)&na, NAME);
		file_is.read((char*)tMd5, 32);//
		md5 = std::string(tMd5);
		Account a(vid, string(na), md5);
		account_list.push_back(a);
		i++;
	}
}

AccountManageSystem::~AccountManageSystem() 
{
	ofstream os("accountlist.dat",ios_base::binary|ios_base::trunc);
	if (account_list.size() != 0) 
	{
		for (int i = 0; i < account_list.size(); i++)
		{
			os.write((char*)&((account_list[i]).vid), 1);
			os.write((account_list[i]).na.c_str(), NAME);
			os.write((account_list[i]).md5.c_str(), 32);
		}
	}
}

void AccountManageSystem::new_account(string name,string password) 
{
	string md5 = Account::encode_obj.Encode(password);
	Account a(int(account_list.size()), name, md5);
	account_list.push_back(a);
	sign_in(int(account_list.size()) - 1, password);
}

string AccountManageSystem::showname(char vid) 
{
	return account_list[vid].na;
}

Account_running* AccountManageSystem::ar = nullptr;

vector<Account> AccountManageSystem::account_list;

#endif // !ACCOUNT_CPP