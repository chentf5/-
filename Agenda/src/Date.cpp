
#include "Date.hpp"

Date::Date() {
	m_year = 0;
	m_month = 0;
	m_day = 0;
	m_hour = 0;
	m_minute = 0;
}
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
	m_year = t_year;
	m_month = t_month;
	m_day = t_day;
	m_hour = t_hour;
	m_minute = t_minute;
}
//2016-08-01/00:00
//0123456789012345
bool test(const std::string & dateString) {
	if (dateString[4] != '-')    return 0;
	if (dateString[7] != '-')    return 0;
	if (dateString[10] != '/')   return 0;
	if (dateString[13] != ':')   return 0;
	for (int i = 0; i < 4; ++i) {
		char year1 = dateString[i];
		int a = year1 - '0';
		if (a > 9 || a < 0)
			return 0;
	}
	for (int i = 5; i < 7; ++i) {
		char month1 = dateString[i];
		int a = month1 - '0';
		if (a > 9 || a < 0)
			return 0;
	}
	for (int i = 8; i < 10; ++i) {
		char data1 = dateString[i];
		int a = data1 - '0';
		if (a > 9 || a < 0)
			return 0;
	}
	for (int i = 11; i < 13; i++) {
		char hour1 = dateString[i];
		int a = hour1 - '0';
		if (a > 9 || a < 0)
			return 0;
	}
	for (int i = 14; i < 16; ++i) {
		char minute1 = dateString[i];
		int a = minute1 - '0';
		if (a > 9 || a < 0)
			return 0;
	}

	return 1;
}
Date::Date(const std::string & dateString) {
	if (test(dateString)) {
		int year = 0;
		for (int i = 0; i < 4; ++i) {
			char year1 = dateString[i];
			int a = year1 - '0';
			year = year * 10 + a;
		}
		m_year = year;
		int month = 0;
		for (int i = 5; i < 7; ++i) {
			char month1 = dateString[i];
			int a = month1 - '0';
			month = month * 10 + a;
		}
		m_month = month;

		int data = 0;
		for (int i = 8; i < 10; ++i) {
			char data1 = dateString[i];
			int a = data1 - '0';
			data = data * 10 + a;
		}
		m_day = data;

		int hour = 0;
		for (int i = 11; i < 13; i++) {
			char hour1 = dateString[i];
			int a = hour1 - '0';
			hour = hour * 10 + a;
		}
		m_hour = hour;

		int minute = 0;
		for (int i = 14; i < 16; ++i) {
			char minute1 = dateString[i];
			int a = minute1 - '0';
			minute = minute * 10 + a;
		}
		m_minute = minute;
	}
	else {
		m_year = 0;
		m_month = 0;
		m_day = 0;
		m_hour = 0;
		m_minute = 0;
	}

}

int Date::getYear(void) const {
	return m_year;
}

void Date::setYear(const int t_year) {

	m_year = t_year;
}

int Date::getMonth(void) const {
	return m_month;
}

void Date::setMonth(const int t_month) {
	//if(t_month <= 12 && t_month >=1)	{
	m_month = t_month;
	//}
}

int Date::getDay(void) const {
	return m_day;
}
void Date::setDay(const int t_day) {
	/*if(m_month == 2)  {
		if(m_year%4 == 0)  {
			if(t_day <= 29 &&t_day >= 1)    {
				m_day = t_day;
			}
		}else {
			if(t_day <=28 && t_day >=1) {
				m_day = t_day;
			}
		}
	}else if(m_month == 1 || m_month == 3 || m_month == 5 || m_month == 7
			  || m_month == 8 || m_month == 10 || m_month == 12)    {
				if(t_day <= 31 && t_day >=1)  {
					m_day = t_day;
				}
			  }
	else {
		if(t_day <=30 && t_day >=1) {
			m_day == t_day;
		}
	}*/

	m_day = t_day;
}

int Date::getHour(void) const {
	return m_hour;
}
void Date::setHour(const int t_hour) {
	//if(t_hour < 24 && t_hour >= 0)	{
	m_hour = t_hour;
	//}
}

int Date::getMinute(void) const {
	return m_minute;
}
void Date::setMinute(const int t_minute) {
	//if(t_minute < 60 && t_minute >= 0)	{
	m_minute = t_minute;
	//}
}
bool Date::isValid(const Date & t_date) {
	if (t_date.getYear() < 1000 || t_date.getYear() > 9999)    return 0;
	if (t_date.getMonth() <= 0 || t_date.getMonth() > 12) return 0;
	if (t_date.getMonth() == 2) {
		if ((t_date.getYear() % 100 != 0 && t_date.getYear() % 4 == 0) || (t_date.getYear() % 100 == 0 && t_date.getYear() % 400 == 0)) { //run nian
			if (t_date.getDay() > 29 || t_date.getDay() <= 0) {
				return 0;
			}
		}
		else {
			if (t_date.getDay() > 28 || t_date.getDay() <= 0) {
				return 0;
			}
		}
	}
	else if (t_date.getMonth() == 1 || t_date.getMonth() == 3 || t_date.getMonth() == 5 || t_date.getMonth() == 7 || t_date.getMonth() == 8 || t_date.getMonth() == 10
		|| t_date.getMonth() == 12) {
		if (t_date.getDay() > 31 || t_date.getDay() <= 0) {
			return 0;
		}
	}
	else {
		if (t_date.getDay() > 30 || t_date.getDay() <= 0) {
			return 0;
		}
	}
	if (t_date.getHour() >= 24 || t_date.getHour() < 0) {
		return 0;
	}
	if (t_date.getMinute() >= 60 || t_date.getMinute() < 0) {
		return 0;
	}
	return 1;

}

Date Date::stringToDate(const std::string &t_dateString) {
	//Date n;
	if (test(t_dateString) == 0) {
		Date n;
		n.setMinute(0);
		n.setHour(0);
		n.setDay(0);
		n.setMonth(0);
		n.setYear(0);
		return n;
	}
	else {
		Date n(t_dateString);
		return n;
	}
}
//2016-08-01/00:00
//0123456789012345

std::string Date::dateToString(const Date &t_date) {
	//std::string t_dateString = "0000-00-00/00:00";
	if (!t_date.isValid(t_date)) {
		std::string t_dateString = "0000-00-00/00:00";
		return t_dateString;
	}
	else {
		std::string t_dateString = "";
		int k = 1000;
		int year = t_date.getYear();
		for (int i = 0; i < 4; i++) {
			char c = year / k + '0';
			t_dateString += c;
			year = year % k;
			k = k / 10;
		}

		t_dateString += '-';

		int month = t_date.getMonth();
		char t = month / 10 + '0';
		t_dateString += t;
		t = month % 10 + '0';
		t_dateString += t;

		t_dateString += '-';

		int day = t_date.getDay();
		char d = day / 10 + '0';
		t_dateString += d;
		d = day % 10 + '0';
		t_dateString += d;

		t_dateString += '/';

		int hour = t_date.getHour();
		char h = hour / 10 + '0';
		t_dateString += h;
		h = hour % 10 + '0';
		t_dateString += h;

		t_dateString += ':';

		int min = t_date.getMinute();
		char m = min / 10 + '0';
		t_dateString += m;
		m = min % 10 + '0';
		t_dateString += m;

		return t_dateString;
	}
}

Date  & Date::operator=(const Date & t_date) {
	m_year = t_date.getYear();
	m_month = t_date.getMonth();
	m_day = t_date.getDay();
	m_hour = t_date.getHour();
	m_minute = t_date.getMinute();
	return *this;
}

bool Date::operator==(const Date & t_date) const {
	if (m_year == t_date.getYear() && m_month == t_date.getMonth()
		&& m_day == t_date.getDay() && m_hour == t_date.getHour()
		&& m_minute == t_date.getMinute()) {
		return true;
	}
	else {
		return false;
	}
}
bool Date::operator>(const Date &t_date) const {
	if (m_year > t_date.getYear()) {
		return 1;
	}
	else if (m_year < t_date.getYear()) {
		return 0;
	}
	else {
		if (m_month > t_date.getMonth()) {
			return 1;
		}
		else if (m_month < t_date.getMonth()) {
			return 0;
		}
		else {
			if (m_day > t_date.getDay()) {
				return 1;
			}
			else if (m_day < t_date.getDay()) {
				return 0;
			}
			else {
				if (m_hour > t_date.getHour()) {
					return 1;
				}
				else if (m_hour < t_date.getHour()) {
					return 0;
				}
				else {
					if (m_minute > t_date.getMinute()) {
						return 1;
					}
					else if (m_minute < t_date.getMinute()) {
						return 0;
					}
					else {
						return 0;
					}
				}
			}
		}
	}
}
bool Date::operator<(const Date &t_date) const {
	if (m_year < t_date.getYear()) {
		return 1;
	}
	else if (m_year > t_date.getYear()) {
		return 0;
	}
	else {
		if (m_month < t_date.getMonth()) {
			return 1;
		}
		else if (m_month > t_date.getMonth()) {
			return 0;
		}
		else {
			if (m_day < t_date.getDay()) {
				return 1;
			}
			else if (m_day > t_date.getDay()) {
				return 0;
			}
			else {
				if (m_hour < t_date.getHour()) {
					return 1;
				}
				else if (m_hour > t_date.getHour()) {
					return 0;
				}
				else {
					if (m_minute < t_date.getMinute()) {
						return 1;
					}
					else if (m_minute > t_date.getMinute()) {
						return 0;
					}
					else {
						return 0;
					}
				}
			}
		}
	}
}

bool Date::operator>=(const Date &t_date) const {
	if (m_year > t_date.getYear()) {
		return 1;
	}
	else if (m_year < t_date.getYear()) {
		return 0;
	}
	else {
		if (m_month > t_date.getMonth()) {
			return 1;
		}
		else if (m_month < t_date.getMonth()) {
			return 0;
		}
		else {
			if (m_day > t_date.getDay()) {
				return 1;
			}
			else if (m_day < t_date.getDay()) {
				return 0;
			}
			else {
				if (m_hour > t_date.getHour()) {
					return 1;
				}
				else if (m_hour < t_date.getHour()) {
					return 0;
				}
				else {
					if (m_minute > t_date.getMinute()) {
						return 1;
					}
					else if (m_minute < t_date.getMinute()) {
						return 0;
					}
					else {
						return 1;
					}
				}
			}
		}
	}
}

bool Date::operator<=(const Date &t_date) const {
	if (m_year < t_date.getYear()) {
		return 1;
	}
	else if (m_year > t_date.getYear()) {
		return 0;
	}
	else {
		if (m_month < t_date.getMonth()) {
			return 1;
		}
		else if (m_month > t_date.getMonth()) {
			return 0;
		}
		else {
			if (m_day < t_date.getDay()) {
				return 1;
			}
			else if (m_day > t_date.getDay()) {
				return 0;
			}
			else {
				if (m_hour < t_date.getHour()) {
					return 1;
				}
				else if (m_hour > t_date.getHour()) {
					return 0;
				}
				else {
					if (m_minute < t_date.getMinute()) {
						return 1;
					}
					else if (m_minute > t_date.getMinute()) {
						return 0;
					}
					else {
						return 1;
					}
				}
			}
		}
	}

}

