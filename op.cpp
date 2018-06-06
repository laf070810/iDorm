#include "op.h"

istream &operator>>(istream &in, User &a)
{
	in >> a.name >> a.key >> a.state >> a.time;
	return in;
}
ostream &operator<<(ostream &os, User &a)
{
	os.fill(' ');
	os << setiosflags(ios::left);
	os << setw(10) << a.name << setw(20) << a.key << setw(5) << a.state << a.time;
	return os;
}
istream &operator>>(istream &in, Water &a)
{
	in >> a.name >> a.number;
	return in;
}
ostream &operator<<(ostream &os, Water &a)
{
	os << setiosflags(ios::left);
	os << setw(10) << a.name << a.number;
	return os;
}
time_t time_record()
{
	time_t rawtime = time(0);
	char pblgtime[20];
	strftime(pblgtime, 20, "%Y-%m-%d %X", localtime(&rawtime));
	string time;
	time.assign(pblgtime);
	return rawtime;
}
void changestate(string name, int state)
{//改变用户状态
	ifstream in("user.txt");
	User a;
	vector<User> USER;
	vector<User>::iterator i;
	while (!in.eof() && in.peek() != EOF)
	{
		in >> a;
		USER.push_back(a);
		in.get();
	}
	// cout << USER.size();
	ofstream out;
	out.open("user.txt", ios::in);
	for (i = USER.begin(); i != USER.end(); i++) {
		a = *i;
		if (a.name == name)
		{
			a.state = state;
			a.time = time_record();
		}
		out << a << endl;
	}
}
bool name_check(string name)
{//601B  检测用户名格式
	if (name == "701")
		return 1;
	if (name.length() != 4)
		return 0;
	for (int i = 0; i < 3; i++) {
		if (name[i]>'9' || name[i]<'0')
			return 0;
	}
	if (name[1] != '0')
		return 0;
	if (name[3] != 'A'&&name[3] != 'B')
		return 0;
	return 1;
}
string login(string info)
{//登陆
	string line[2];
	int pos[2];
	User user;
	bool flag = 0;
	for (int i = 0; info.length() > 0; i++)
	{
		pos[i] = info.find('\n');
		line[i] = info.substr(0, pos[i]);
		info = info.substr(pos[i] + 1);
		// cout << info;
		// cout << line[i] << endl;
	}
	if (line[0].substr(0, 9) != un || line[1].substr(0, 9) != pw)  //判断信息格式
		return "login failed, wrong format\n";
	user.name = line[0].substr(9);
	// cout << username;
	user.key = line[1].substr(9);
	ifstream in("user.txt");
	if (!in.is_open()) {
		return "no user now, please register";
	}
	User a;
	while (!in.eof() && in.peek() != EOF) {
		in >> a;
		if (a.name == user.name) {
			flag = 1;
			if (a.key == user.key) {
				if (a.state == 0) {
					changestate(a.name, 1);
					return "login succeeded";
				}
				else
					return "login failed, user already online";
			}
			else
				return "login failed, wrong password";
		}
	}
	if (flag == 0)
		return "login failed, user non-exist";
	return "login succeeded";
}
string enroll(string info)
{//注册
	string line[2];
	int pos[2];
	User user;
	for (int i = 0; info.length() > 0; i++)
	{
		pos[i] = info.find('\n');
		line[i] = info.substr(0, pos[i]);
		info = info.substr(pos[i] + 1);
		// cout << info;
		// cout << line[i] << endl;
	}
	if (line[0].substr(0, 9) != un || line[1].substr(0, 9) != pw)  //判断信息格式
		return "register failed, wrong format\n";
	user.name = line[0].substr(9);
	if (!name_check(user.name))
		return "register failed, wrong name format";
	// cout << username;
	user.key = line[1].substr(9);
	if (user.key.length() < 6 || user.key.length() >15)
		return "register failed, password too short or too long";
	// cout << key;
	ifstream in("user.txt");
	if (!in.is_open()) {
		ofstream out("user.txt");
		out.close();
		in.clear();
		in.open("user.txt");
	}
	User a;
	while (!in.eof() && in.peek() != EOF) {
		in >> a;
		if (a.name == user.name)
			return "register failed, repeated username";
	}
	user.state = 0;
	user.time = time_record();
	ofstream os;
	os.open("user.txt", ios::app);
	os << user << endl;
	return "successfully register";
}
string logout(string info)
{//登出
	string line, name;
	int pos;
	bool flag = 0;
	pos = info.find('\n');
	line = info.substr(0, pos);
	if (line.substr(0, 9) != un) //判断信息格式
		return "change failed, wrong format\n";
	name = line.substr(9);
	ifstream in("user.txt");
	if (!in.is_open())
		return "request failed, please login";
	User a;
	while (!in.eof() && in.peek() != EOF) {
		in >> a;
		if (a.name == name) {
			flag = 1;
			break;
		}
	}
	if (flag == 0 || a.state == 0)
		return "request failed, please login";
	changestate(name, 0);
	return "successfully logout";
}
string changepassword(string info)
{//修改密码
	string line[3], name, oldkey, newkey;
	int pos[3];
	bool flag = 0;
	for (int i = 0; info.length() > 0; i++)
	{
		pos[i] = info.find('\n');
		line[i] = info.substr(0, pos[i]);
		// cout << line[i] << endl;
		info = info.substr(pos[i] + 1);
	}
	if (line[0].substr(0, 9) != un || line[1].substr(0, 12) != opw || line[2].substr(0, 12) != npw) //判断信息格式
		return "change failed, wrong format\n";
	name = line[0].substr(9);
	oldkey = line[1].substr(12);
	newkey = line[2].substr(12);
	if (newkey.length()<6 || newkey.length()>15)
		return "change password failed, password too long or too short";
	// user.key = line[1].substr(9);
	ifstream in("user.txt");
	if (!in.is_open())
		return "change password failed, please login";
	User a;
	vector<User> USER;
	while (!in.eof() && in.peek() != EOF)
	{
		in >> a;
		if (a.name == name) {
			flag = 1;
			if (a.state == 0)
				return "change password failed, please login";
			if (a.key != oldkey)
				return "change password failed, wrong initial number";
		}
		a.time = time_record();
		USER.push_back(a);
		in.get();
	}
	if (flag == 0)
		return "change password failed, please login";
	vector<User>::iterator i;
	ofstream out;
	out.open("user.txt");
	for (i = USER.begin(); i != USER.end(); i++)
	{
		a = *i;
		if (a.name == name) {
			a.key = newkey;
			// cout << a.key;
		}
		out << a << endl;
	}
	return "successfully change your password";
}
string water(string info)
{//自动叫水
	string line[2], name, num;
	int number;
	string::size_type sz;
	int pos[2];
	bool flag = 0;
	for (int i = 0; info.length() > 0; i++)
	{
		pos[i] = info.find('\n');
		line[i] = info.substr(0, pos[i]);
		info = info.substr(pos[i] + 1);
	}
	if (line[0].substr(0, 9) != un || line[1].substr(0, 7) != nb)  //判断信息格式
		return "call failed, wrong format\n";
	name = line[0].substr(9);
	num = line[1].substr(7);
	for (int i = 0; i < num.length(); i++)
		if (num[i]>'9' || num[i]<'0')
			return "call failed, wrong number";
	number = stoi(num, &sz);
	if (number > 10 && number <= 0)
		return "call failed, wrong number";
	ifstream in("user.txt");
	if (!in.is_open())
		return "call failed, please login";
	User a;
	while (!in.eof() && in.peek() != EOF)
	{ //判断是否登陆
		in >> a;
		if (a.name == name) {
			flag = 1;
			if (a.state == 0)
				return "call failed, please login";
			break;
		}
	}
	if (flag == 0)
		return "call failed, please login";
	changestate(a.name, a.state);
	ifstream read;
	//read.open("water.txt");
	read.open("D:\\VS-CODE\\Ti\\web\\water.txt");
	if (!read.is_open())
	{
		//ofstream out("water.txt");
		ofstream out("D:\\VS-CODE\\Ti\\web\\water.txt");
		out.close();
		read.clear();
		//read.open("water.txt");
		read.open("D:\\VS-CODE\\Ti\\web\\water.txt");
	}
	Water w;
	bool wflag = 0;
	vector<Water> WATER;
	vector<Water>::iterator i;
	int now;
	while (!read.eof() && read.peek() != EOF)
	{
		read >> w;
		if (w.name == name)
		{
			wflag = 1;
			w.number += number;
			now = w.number;
			if (w.number > 10)
				return "water " + to_string(number) + " call failed, too much water now " + to_string(w.number - number);
		}
		WATER.push_back(w);
		read.get();
	}
	// cout << WATER.size();
	if (wflag == 0)
	{
		w.name = name;
		w.number = number;
		WATER.push_back(w);
	}
	ofstream out;
	//out.open("water.txt");
	out.open("D:\\VS-CODE\\Ti\\web\\water.txt");
	for (i = WATER.begin(); i != WATER.end(); i++)
	{
		w = *i;
		out << w << endl;
	}
	return "call succeed" + to_string(number) + ", now " + to_string(now);
}
string nodisturb(string info)
{//免打扰模式
	string line, name;
	int pos;
	bool flag = 0;
	pos = info.find('\n');
	line = info.substr(0, pos);
	if (line.substr(0, 9) != un) //判断信息格式
		return "change failed, wrong format\n";
	name = line.substr(9);
	ifstream in("user.txt");
	if (!in.is_open())
		return "request failed, please login";
	User a;
	while (!in.eof() && in.peek() != EOF) {
		in >> a;
		if (a.name == name) {
			flag = 1;
			break;
		}
	}
	if (flag == 0 || a.state == 0)
		return "request failed, please login";
	if (a.state == 2)
		return "request failed, you have already in DND mode";
	changestate(name, 2);
	return "successfully turn on DND mode";
}
string normal(string info)
{//正常模式
	string line, name;
	int pos;
	bool flag = 0;
	pos = info.find('\n');
	line = info.substr(0, pos);
	if (line.substr(0, 9) != un) //判断信息格式
		return "change failed, wrong format\n";
	name = line.substr(9);
	ifstream in("user.txt");
	if (!in.is_open())
		return "request failed, please login";
	User a;
	while (!in.eof() && in.peek() != EOF) {
		in >> a;
		if (a.name == name) {
			flag = 1;
			break;
		}
	}
	if (flag == 0 || a.state == 0)
		return "request failed, please login";
	if (a.state == 1)
		return "request failed, you have already been in normal mode";
	changestate(name, 1);
	return "successfully turn on normal mode";
}
string update(string info)
{//日常更新
	string line, name, key;
	int pos;
	bool flag = 0;
	pos = info.find('\n');
	line = info.substr(0, pos);
	if (line.substr(0, 9) != un) //判断信息格式
		return "change failed, wrong format\n";
	name = line.substr(9);
	ifstream in("user.txt");
	if (!in.is_open())
		return "request failed, please login";
	User a;
	while (!in.eof() && in.peek() != EOF) {
		in >> a;
		if (a.name == name) {
			flag = 1;
			break;
		}
	}
	if (flag == 0 || a.state == 0)
		return "request failed, please login";
	changestate(a.name, a.state);
	return "successfully update";
}
bool check_time(time_t time1, time_t time2)
{//时间检测
	if ((time2 - time1) > 60 * MINUTE)
		return 0;
	else
		return 1;
}
string auto_logout()
{//自动登出
	time_t Time;
	Time = time_record();
	vector<User> USER;
	vector<User>::iterator i;
	User a;
	ifstream in("user.txt");
	while (!in.eof() && in.peek() != EOF)
	{
		in >> a;
		if (!check_time(a.time, Time) && a.state != 0)
		{
			changestate(a.name, 0);
			string information = a.name;
			information.append(" : auto-logout\n");
			return information;
		}
		in.get();
	}
	return "";
}
string handle(char * Buf)
{//取有效信息
	string meg, BUF;
	BUF.assign(Buf);
	meg = BUF.substr(BUF.find(bg));
	meg = meg.substr(6, meg.find(ed, 0) - 6);
	// cout << meg;
	return meg;
}
string operate(string meg)
{//不同操作
	string op, info, name;
	int pos = meg.find('\n');
	op = meg.substr(0, pos);   //取出用户操作
	info = meg.substr(pos + 1);  //操作下的信息
	name = info.substr(9, 4) + " : ";
	if (op == li)
		return name + login(info);
	else if (op == rg)
		return name + enroll(info);
	else if (op == lo)
		return name + logout(info);
	else if (op == cp)
		return name + changepassword(info);
	else if (op == wt)
		return name + water(info);
	else if (op == nd)
		return name + nodisturb(info);
	else if (op == nm)
		return name + normal(info);
	else if (op == up)
		return name + update(info);
	else
		return name + "illegal request";
}
