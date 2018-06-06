#pragma once
#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
#include<fstream>
using namespace std;
#define MINUTE 1
#define li "LOGIN"
#define lo "LOGOUT"
#define wt "WATER"
#define cp "CHANGEPASSWORD"
#define rg "REGISTER"
#define nd "DoNotDisturb"
#define nm "NORMAL"
#define up "UPDATE"
#define un "Username:"
#define pw "Password:"
#define opw "OldPassword:"
#define npw "NewPassword:"
#define nb "Number:"
#define bg "BEGIN"
#define ed "END"       //设置信息结束标志
// char Buf[500] = "adfsuhfsiafduBEGIN\nCHANGEPASSWORD\nUsername:602B\nOldPassword:123456\nNewPassword:123466\nEND\nadsjfioawowebvuirewvbeirufiu";
// char Buf[500] = "adfsuhfsiafduBEGIN\nLOGIN\nUsername:603B\nPassword:123456\nEND\nadsjfioawowebvuirewvbeirufiu";
// char Buf[500] = "adfsuhfsiafduBEGIN\nLOGOUT\nUsername:603B\nPassword:123456\nEND\nadsjfioawowebvuirewvbeirufiu";
// char Buf[500] = "adfsuhfsiafduBEGIN\nREGISTER\nUsername:603B\nPassword:123456\nEND\nadsjfioawowebvuirewvbeirufiu";
//char Buf[500] = "adfsuhfsiafduBEGIN\nWATER\nUsername:603B\nNUMBER:3\nEND\nadsjfioawowebvuirewvbeirufiu";
// char Buf[500] = "adfsuhfsiafduBEGIN\nDoNotDisturb\nUsername:603B\nPassword:123456\nEND\nadsjfioawowebvuirewvbeirufiu";
// char Buf[500] = "adfsuhfsiafduBEGIN\nNORMAL\nUsername:602B\nPassword:123456\nEND\nadsjfioawowebvuirewvbeirufiu";
struct User
{
	string name;      //用户名限制为 X0X A/B
	string key;       //密码长度不超过15
	int state;        //0为离线  1为在线  2为免打扰
	time_t time;
};
struct Water {
	string name;
	int number;
};

istream &operator>>(istream &in, User &a);
ostream &operator<<(ostream &os, User &a);
time_t time_record();
void changestate(string name, int state);
bool name_check(string name);
string login(string info);
string enroll(string info);
string logout(string info);
string changepassword(string info);
string water(string info);
string nodisturb(string info);
string normal(string info);
string update(string info);
bool check_time(time_t time1, time_t time2);
string auto_logout(void);
string handle(char * Buf);
string operate(string meg);