#define _CRT_SECURE_NO_WARNINGS
#include"head_file.h"
using std::cout;

AccountManageSystem ams;

int main(int argc, const char * argv[])
{
	char name[] = "cxk";
	string pass = "123567";
	ams.new_account(name,pass);
}