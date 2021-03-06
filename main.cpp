#include"head_file.h"
using std::cout;


int main()
{
	AccountManageSystem::init();
	cout << AccountManageSystem::account_list.size();
	AccountManageSystem::new_account("wpx", "123456");
	//cout << AccountManageSystem::account_list.size();
	////AccountManageSystem::new_account("wqh", "123456");
	////cout << AccountManageSystem::account_list.size();


	if (AccountManageSystem::sign_in("wpx", "123456"))
	{
		std::cout << "User " << AccountManageSystem::get_current_user_name() << " login" << std::endl;

		//AccountManageSystem::ar->add_new_word(2);
		AccountManageSystem::ar->add_new_word(1);

		//cout << AccountManageSystem::ar->wl_new.count_total();
		AccountManageSystem::ar->create_daily_wordlist();
		//cout << AccountManageSystem::ar->wl_new.count_total();
		//cout << AccountManageSystem::ar->wl_daily.count_total();
		//cout << AccountManageSystem::ar->wl_daily.count_new() << endl;

		//Wordnode a = AccountManageSystem::ar->wl_daily.pop();
		//cout << a.id << char(a.st + '0') << char(a.ti + '0') << endl;
		//AccountManageSystem::ar->wl_daily.check(false);
		//a = AccountManageSystem::ar->wl_daily.pop();
		//cout << a.id << char(a.st + '0') << char(a.ti + '0') << endl;
		AccountManageSystem::ar->wl_daily.check(true);
		//a = AccountManageSystem::ar->wl_daily.pop();
		//cout << a.id << char(a.st + '0') << char(a.ti + '0') << endl;

	}
	//else
	//{
	//	std::cout << "User login failed" << std::endl;
	//}
	////if(AccountManageSystem::delete_current_account())cout<<"delete complete"<<endl;
	//cout << AccountManageSystem::account_list.size();


	AccountManageSystem::ar->read_dl();
	AccountManageSystem::ar->complete();
	cout << (AccountManageSystem::ar->iscomplete());
	AccountManageSystem::ar->save_dl();


	if (AccountManageSystem::end())cout << "successfully end";
}