#include "AgendaService.hpp"
#include <list>
using namespace std;

AgendaService::AgendaService() {
	startAgenda();
}

AgendaService::~AgendaService() {
	quitAgenda();
}

bool AgendaService::userLogIn(const std::string &userName,
	const std::string &password) {
	auto list_test = [userName, password](const User & user) {
		return user.getName() == userName && user.getPassword() == password;
	};
	if (m_storage->queryUser(list_test).size() > 0)	return true;
	return false;
}

bool AgendaService::userRegister(const std::string &userName,
	const std::string &password,
	const std::string &email, const std::string &phone) {

	auto list_test = [userName, email, phone](const User & user) {
		return (user.getName() == userName || user.getPhone() == phone || user.getEmail() == email);
	};
	if (m_storage->queryUser(list_test).empty()) {
		User add(userName, password, email, phone);
		m_storage->createUser(add);
		return true;
	}
	else return false;
}

bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
	auto list_test = [userName, password](const User & user) {
		return user.getName() == userName && user.getPassword() == password;
	};
	int count = m_storage->deleteUser(list_test);
	if (count > 0) {     //delete user's meetings???
		deleteAllMeetings(userName);

		std::list<Meeting> t = listAllParticipateMeetings(userName);
		auto itor_t = t.begin();
		for (; itor_t != t.end(); ++itor_t) {
			std::string sponser_test = itor_t->getSponsor();

			std::string title_t = itor_t->getTitle();
			removeMeetingParticipator(sponser_test, title_t, userName);

		}

		auto del = [&](const Meeting &m) {
			return m.getParticipator().empty();
		};
		m_storage->deleteMeeting(del);

		return true;
	}
	return false;
}

std::list<User> AgendaService::listAllUsers() const {
	auto list_test = [](const User &user) {
		return true;
	};
	return m_storage->queryUser(list_test);
}

bool AgendaService::createMeeting(const std::string &userName,
	const std::string &title,
	const std::string &startDate, const std::string &endDate,
	const std::vector<std::string> &participator) {
	Date empty(0, 0, 0, 0, 0);
	Date start(startDate);
	Date end(endDate);
	int exist = 1;
	if (start >= end) {
		exist = 0;
		return false;
	} // 1 检测时间是否重叠或结束时间比开始时间前
	if (start == empty || end == empty) { // 2检测时间格式是否正确
		return false;
	}
	if (!start.isValid(start) || !end.isValid(end)) {
		return false; //3 检测时间是否符合逻辑
	}
	// 4 检测参加者，举办者是否是已注册用户
	auto sponser_test = [userName](const User &user) {
		return user.getName() == userName;
	};
	if (m_storage->queryUser(sponser_test).empty()) {
		return false;
	}
	int size = participator.size();
	for (int i = 0; i < size; i++) {
		string current_name = participator[i];
		auto participator_test = [current_name](const User &user) {
			return user.getName() == current_name;
		};
		if (m_storage->queryUser(participator_test).empty()) {
			return false;
		}
	}

	//5 检测参加者是否重复 是否包括举办者
	//int size = participator.size();
	for (int i = 0; i < size; i++) {
		//if (participator[i] == userName)
			//return false;
		for (int j = i + 1; j < size; ++j) {
			if (participator[i] == participator[j])
				return false;
		}
	}

	for (int i = 0; i < size; i++) {
		if (participator[i] == userName)
			return false;
	}

	// 6检测title是否唯一
	auto filter2 = [&](const Meeting &meeting) {
		return meeting.getTitle() == title;
	};
	if (!m_storage->queryMeeting(filter2).empty()) {
		return false;
	}



	//7 检测时间冲突
	auto filter1 = [&](const Meeting & meeting) {
		Date s = meeting.getStartDate();
		Date e = meeting.getEndDate();
		if (s >= end || e <= start) {
			return false;
		}
		else {
			int size1 = participator.size();
			for (int i = 0; i < size1; i++) {
				if (meeting.isParticipator(participator[i]) || meeting.getSponsor() == participator[i])
					return true;
			}
			if (userName == meeting.getSponsor() || meeting.isParticipator(userName))
				return true;
		}
		return false;

	};
	if (m_storage->queryMeeting(filter1).size() > 0) {
		return false;
	}
	//顺利通过验证，没有中途return，即可创建meeting，返回true
	Meeting t_meeting(userName, participator, start, end, title);
	m_storage->createMeeting(t_meeting);
	return true;
}


bool AgendaService::addMeetingParticipator(const std::string &userName,
	const std::string &title,
	const std::string &participator) {



	//1 检测用户是否存在
	auto participator_exist = [participator](const User &user) {
		return user.getName() == participator;
	};
	if (m_storage->queryUser(participator_exist).empty()) {
		return false;
	}

	if (userName == participator)	return false;//用户不能是举办者
	//需要先遍历会议，对找到目标会议

	list<Meeting> participator_in = listAllMeetings(participator);
	auto filter = [&](const Meeting & meeting) {
		//找到
		if (meeting.getSponsor() == userName && meeting.getTitle() == title && !(meeting.isParticipator(participator))) {
			Date start = meeting.getStartDate();
			Date end = meeting.getEndDate();
			//2 检测用户是否已在会议中
			//if(meeting.isParticipator(participator))
				//return false;
			//3 检测用户是否有时间冲突
			for (auto itor = participator_in.begin(); itor != participator_in.end(); itor++) {
				if (!(end <= itor->getStartDate() || start >= itor->getEndDate()))
					return false;
			}
			return true;
		}
		return false;
	};
	auto switcher = [participator](Meeting & meeting) {
		meeting.addParticipator(participator);
	}; // judge is exist ??
	int count = m_storage->updateMeeting(filter, switcher);
	if (count > 0)	return true;
	return false;
}

bool AgendaService::removeMeetingParticipator(const std::string &userName,
	const std::string &title,
	const std::string &participator) {
	auto filter = [userName, title, participator](const Meeting & meeting) {
		return meeting.getSponsor() == userName && meeting.getTitle() == title && meeting.isParticipator(participator);
	};
	auto switcher = [participator](Meeting & meeting) {
		meeting.removeParticipator(participator);
	};


	//m_storage->updateMeeting(filter,switcher);
	if (m_storage->updateMeeting(filter, switcher) != 0) {
		auto del = [&](const Meeting &m) {
			return m.getParticipator().empty();
		};
		m_storage->deleteMeeting(del);
		return true;
	}
	else return false;
}

bool AgendaService::quitMeeting(const std::string &userName,
	const std::string &title) {
	auto filter = [userName, title](const Meeting & meeting) {
		return (meeting.isParticipator(userName) && meeting.getTitle() == title && meeting.getSponsor() != userName);
	};
	auto switcher = [userName](Meeting & meeting) {
		meeting.removeParticipator(userName);

	};

	int count = m_storage->updateMeeting(filter, switcher);
	auto del = [&](const Meeting &m) {
		return m.getParticipator().empty();
	};
	m_storage->deleteMeeting(del);
	// todo
	if (count != 0)	return true;
	else return false;

}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
	const std::string &title) const {
	auto filter = [userName, title](const Meeting & meeting) {
		if (meeting.getSponsor() == userName) {
			if (meeting.getTitle() == title) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (meeting.isParticipator(userName) && meeting.getTitle() == title)
				return true;
			else return false;
		}
	};
	return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
	const std::string &startDate,
	const std::string &endDate) const {
	std::list<Meeting> empty_meeting;
	Date s_test(startDate);
	Date e_test(endDate);
	Date empty(0, 0, 0, 0, 0);

	if (s_test > e_test) {
		return empty_meeting;
	} // 1 检测时间是否重叠或结束时间比开始时间前
	if (s_test == empty || e_test == empty) { // 2检测时间格式是否正确
		return empty_meeting;
	}
	if (s_test.isValid(s_test) == 0 || e_test.isValid(e_test) == 0) {
		return empty_meeting; //3 检测时间是否符合逻辑
	}

	auto filter = [userName, startDate, endDate](const Meeting &meeting) {
		Date s(startDate);
		Date e(endDate);
		if (meeting.getSponsor() == userName) {
			if (meeting.getStartDate() >= s && meeting.getEndDate() <= e)
				return true;
			else if (meeting.getStartDate() >= s && meeting.getStartDate() <= e) {
				return true;
			}
			else if (meeting.getEndDate() >= s && meeting.getEndDate() <= e)
			{
				return true;
			}
			else if (meeting.getEndDate() >= e && meeting.getStartDate() <= s) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (meeting.isParticipator(userName)) {
				if (meeting.getStartDate() >= s && meeting.getEndDate() <= e)
					return true;
				else if (meeting.getStartDate() >= s && meeting.getStartDate() <= e) {
					return true;
				}
				else if (meeting.getEndDate() >= s && meeting.getEndDate() <= e)
				{
					return true;
				}
				else if (meeting.getEndDate() >= e && meeting.getStartDate() <= s) {
					return true;
				}
				else {
					return false;
				}
			}
			return false;
		}
	};
	return m_storage->queryMeeting(filter);
}


std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
	auto filter = [userName](const Meeting &meeting) {
		return meeting.getSponsor() == userName || meeting.isParticipator(userName);
	};
	return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
	auto filter = [userName](const Meeting &meeting) {
		return meeting.getSponsor() == userName;
	};
	return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(
	const std::string &userName) const {
	auto filter = [userName](const Meeting &meeting) {
		return meeting.isParticipator(userName);
	};
	return m_storage->queryMeeting(filter);
}

bool AgendaService::deleteMeeting(const std::string &userName,
	const std::string &title) {
	auto filter = [userName, title](const Meeting &meeting) {
		return meeting.getSponsor() == userName && meeting.getTitle() == title;
	};
	int count = m_storage->deleteMeeting(filter);
	if (count != 0) {
		return true;
	}
	else return false;
}

bool AgendaService::deleteAllMeetings(const std::string &userName) {
	auto filter = [userName](const Meeting &meeting) {
		return meeting.getSponsor() == userName;
	};
	m_storage->deleteMeeting(filter);
	return true;

}

void AgendaService::startAgenda() {
	m_storage = Storage::getInstance();
	//m_storage->readFromFile();
}

void AgendaService::quitAgenda() {
	m_storage->sync();
}