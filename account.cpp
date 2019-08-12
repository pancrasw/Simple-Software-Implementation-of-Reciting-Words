#ifndef ACCOUNT_CPP
#define ACCOUNT_CPP
#include"account.h"
#define WORD 4
//@author:Ïö
Md5Encode Account::encode_obj;

using std::vector;
using std::ifstream;
using std::ofstream;

Account::Account(char id, char* name, string md) :
	vid(id),
	md5(md)
{
	strcpy(na, name);
}

Account::~Account()
{
}

Account_running::Account_running(Account ac):
	Account(ac)
{
	char* file_name = new char[24];
	strcpy(file_name, na);
	char*ptr;
	for (ptr = file_name; (*ptr) != 0; ptr++);
	char suffix[] = ".dat";
	strcpy(ptr, suffix);
	ifstream ifs(file_name, ios_base::binary);
	delete file_name;
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
	strcpy(file_name, na);
	char*ptr;
	for (ptr = file_name; (*ptr) != 0; ptr++);
	char suffix[] = ".dat";
	strcpy(ptr, suffix);
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
	if (account_list[int(vid)].md5 == Account::encode_obj.Encode(password)) 
	{
		ar = new Account_running(account_list[int(vid)]);
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
	char vid;
	char na[20];
	string md5;
	while (!file_is.eof())
	{
		file_is.read((char*)&vid,1);
		file_is.read((char*)&na, 20);
		file_is.read((char*)&md5, 32);
		Account a(vid, na, md5);
		account_list.push_back(a);
	}
	if (account_list.size() != 0)
	{
		namelist = new char*[account_list.size()];
		for (int i = 0; i < account_list.size(); i++)
		{
			namelist[i] = account_list[i].na;
		}
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
			os.write((char*)&((account_list[i]).na), 20);
			os.write((char*)&((account_list[i]).md5), 32);
		}
		delete namelist;
	}
}

void AccountManageSystem::new_account(char*name,string password) 
{
	string md5 = Account::encode_obj.Encode(password);
	Account a(int(account_list.size()), name, md5);
	account_list.push_back(a);
	sign_in(int(account_list.size()) - 1, password);
}

Account_running* AccountManageSystem::ar = nullptr;

vector<Account> AccountManageSystem::account_list;

#endif // !ACCOUNT_CPP