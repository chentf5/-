#include "Storage.hpp"
#include <iostream>
#include <fstream>
#include <functional>
#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <memory>
#include <vector>
#include <functional>
#include <sstream>
#include <ios>
#include "Path.hpp"


using namespace std;

std::shared_ptr<Storage> Storage::m_instance = nullptr;
Storage::Storage() {
	//m_dirty = false;
	readFromFile();
	m_dirty = false;
}


bool Storage::readFromFile(void)
{
	ifstream fout(Path::meetingPath);
	ifstream foutu(Path::userPath);
	if (fout.is_open() == false)
	{

		return false;
	}
	if (foutu.is_open() == false)
	{

		return false;
	}
	string name;
	string pass;
	string mail;
	string phone;

	while (1)
	{

		string str;
		getline(foutu, str);

		vector<int> sign;
		int len = str.size();
		for (int index = 0; index < len; index++)
		{
			if (str[index] == '"')
				sign.push_back(index);
		}
		if (sign.size() != 8)
			break;
		string name;
		string pass;
		string email;
		string phone;

		for (int index = sign[0] + 1; index < sign[1]; index++)
			name.push_back(str[index]);
		for (int index = sign[2] + 1; index < sign[3]; index++)
			pass.push_back(str[index]);
		for (int index = sign[4] + 1; index < sign[5]; index++)
			email.push_back(str[index]);
		for (int index = sign[6] + 1; index < sign[7]; index++)
			phone.push_back(str[index]);
		User u(name, pass, email, phone);
		m_userList.push_back(u);


	}

	foutu.close();

	string spon;
	string temp_v;
	vector<string> v;
	string start;
	string end;
	string title;

	while (1)
	{

		vector<int> sign;
		vector<int> sep;

		string str;
		getline(fout, str);

		vector<string> par;
		string spon;
		string startDate;
		string endDate;
		string title;
		int len = str.size();
		if (len <= 2) break;


		for (int index = 0; index < len; index++)
		{
			if (str[index] == '"')
				sign.push_back(index);
			if (str[index] == '&')
				sep.push_back(index);
		}
		if (sign.size() < 2)
			break;
		for (int index = sign[0] + 1; index < sign[1]; index++)
			spon.push_back(str[index]);
		for (int index = sign[4] + 1; index < sign[5]; index++)
			startDate.push_back(str[index]);
		for (int index = sign[6] + 1; index < sign[7]; index++)
			endDate.push_back(str[index]);
		for (int index = sign[8] + 1; index < sign[9]; index++)
			title.push_back(str[index]);
		len = sep.size() + 1;
		if (len != 1) {
			for (int index = 0; index < len; index++)
			{
				if (index == 0)
				{
					string ptr;
					for (int jndex = sign[2] + 1; jndex < sep[index]; jndex++)
						ptr.push_back(str[jndex]);
					par.push_back(ptr);
					continue;
				}
				if (index == len - 1)
				{
					string ptr;
					for (int jndex = sep[index - 1] + 1; jndex < sign[3]; jndex++)
						ptr.push_back(str[jndex]);
					par.push_back(ptr);
					continue;
				}
				string ptr;
				for (int jndex = sep[index - 1] + 1; jndex < sep[index]; jndex++)
					ptr.push_back(str[jndex]);
				par.push_back(ptr);

			}
		}
		else
		{
			string htr;
			for (int jndex = sign[2] + 1; jndex < sign[3]; jndex++)
				htr.push_back(str[jndex]);
			par.push_back(htr);
		}
		Date b(startDate);
		Date e(endDate);
		Meeting m(spon, par, b, e, title);
		m_meetingList.push_back(m);
	}
	fout.close();
	return true;
}

bool Storage::writeToFile(void)
{

	m_dirty = false;
	fstream foutu(Path::userPath, ios::out);
	fstream fout(Path::meetingPath, ios::out);
	if (!fout.is_open())
	{

		return false;
	}
	if (!foutu.is_open())
	{

		return false;
	}
	int len1 = m_userList.size();
	int len2 = m_meetingList.size();

	if (len1 == 0 && len2 == 0)
		return false;


	auto it = m_userList.begin();

	for (it; it != m_userList.end(); it++)
	{

		foutu << "\"" << (*it).getName() << "\"" << ',' << "\"" << (*it).getPassword() << "\"";
		foutu << ',' << "\"" << (*it).getEmail() << "\"" << ',' << "\"" << (*it).getPhone() << "\"";

		auto itor = it;
		itor++;
		if (itor != m_userList.end())
			foutu << "\n";
	}
	foutu.close();
	auto ft = m_meetingList.begin();
	for (ft; ft != m_meetingList.end(); ft++)
	{
		fout << '"' << (*ft).getSponsor() << '"' << ',' << '"';
		vector<string> lenn = (*ft).getParticipator();
		int len = lenn.size();
		for (int index = 0; index < len; index++)
		{
			fout << lenn[index];
			if (index != len - 1)
				fout << '&';
		}
		fout << '"' << ',';
		fout << '"' << (*ft).getStartDate().dateToString((*ft).getStartDate()) << '"';
		fout << ',' << '"' << (*ft).getEndDate().dateToString((*ft).getEndDate()) << '"' << ',' << '"' << (*ft).getTitle() << '"';
		auto ftor = ft;
		ftor++;
		if (ftor != m_meetingList.end())
			fout << "\n";
	}
	fout.close();
	return true;
}
std::shared_ptr<Storage> Storage::getInstance(void) {
	if (m_instance == nullptr)
		m_instance = std::shared_ptr<Storage>(new Storage());
	return m_instance;
}

Storage::~Storage() {
	sync();
}

void Storage::createUser(const User &t_user) {
	m_userList.push_back(t_user);
	m_dirty = true;
}
std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
	std::list<User> k;
	auto i = m_userList.begin();
	for (i; i != m_userList.end(); i++) {
		if (filter(*i))
			k.push_back(*i);
	}
	return k;
}

int Storage::updateUser(std::function<bool(const User &)> filter,
	std::function<void(User &)> switcher) {
	int count = 0;
	auto i = m_userList.begin();
	for (i; i != m_userList.end(); i++) {
		if (filter(*i)) {
			switcher(*i);
			count++;
		}
	}
	if (count != 0)
		m_dirty = true;
	return count;
}

int Storage::deleteUser(std::function<bool(const User &)> filter) {
	int count = 0;
	auto i = m_userList.begin();
	for (i; i != m_userList.end();) {
		if (filter(*i)) {
			auto it = i;
			i = m_userList.erase(it);
			count++;
			continue;
		}
		i++;
	}
	if (count != 0)
		m_dirty = true;
	return count;

}


void Storage::createMeeting(const Meeting &t_meeting) {

	m_meetingList.push_back(t_meeting);
	m_dirty = true;
}

std::list<Meeting> Storage::queryMeeting(
	std::function<bool(const Meeting &)> filter) const {
	std::list<Meeting > k;
	auto i = m_meetingList.begin();
	for (i; i != m_meetingList.end(); i++) {
		if (filter(*i))
			k.push_back(*i);
	}
	return k;
}

int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
	std::function<void(Meeting &)> switcher) {
	int count = 0;
	auto i = m_meetingList.begin();
	for (i; i != m_meetingList.end(); i++) {
		if (filter(*i)) {
			switcher(*i);
			count++;
		}
	}
	if (count != 0)
		m_dirty = true;
	return count;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
	int count = 0;
	auto i = m_meetingList.begin();
	for (i; i != m_meetingList.end();) {
		if (filter(*i)) {
			count++;
			auto it = i;
			i = m_meetingList.erase(it);

			continue;
		}
		i++;
	}
	if (count != 0)
		m_dirty = true;
	return count;

}
bool Storage::sync(void) {
	if (m_dirty == false)    return false;
	if (m_dirty == true) {
		bool f1 = writeToFile();
		if (f1 == false)
			return false;
	}
	return true;
}
