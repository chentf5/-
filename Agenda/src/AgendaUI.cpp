#include "AgendaUI.hpp"
#include <iomanip>

using namespace std;

AgendaUI::AgendaUI() {

}
void AgendaUI::OperationLoop() {
	std::string userinput;
	do
	{
		std::cout << "--------------------------------- Agenda ------------------------"
			<< "------------------------------" << std::endl;
		std::cout << "Action :" << std::endl;
		std::cout << "l		- log in Agenda by user name and password" << std::endl;
		std::cout << "r		- register an Agenda account" << std::endl;
		std::cout << "q		- quit Agenda" << std::endl;
		std::cout << "----------------------------------------------------------------"
			<< "------------------------------" << std::endl;
		std::cout << "Agenda :~$ ";
		std::cin >> userinput;
		if (userinput == "l")	userLogIn();
		else if (userinput == "r") userRegister();
		else if(userinput == "q"){
			quitAgenda();
		}
		else
		{
			cout << "Wrong Instruction" << endl;
		}
	} while (userinput[0] != 'q');

}

void AgendaUI::startAgenda() {
	bool is_ex = true;
	while (is_ex)
	{
		std::cout << "--------------------------------- Agenda ------------------------"
			<< "------------------------------" << std::endl;
		std::cout << "Action :" << std::endl;
		std::cout << "o		- log out Agenda" << std::endl;
		std::cout << "s		- reset account's detail" << endl;
		std::cout << "dc	- delete Agenda account" << std::endl;
		std::cout << "lu	- list all Agenda user" << std::endl;
		std::cout << "cm	- creat a meeting" << std::endl;
		std::cout << "amp	- add meeting participator" << std::endl;
		std::cout << "rmp	- remove meeting participator" << std::endl;
		std::cout << "rqm	- request to quit meeting" << std::endl;
		std::cout << "la	- list all meeting" << std::endl;
		std::cout << "las	- list all sponsor meetings" << std::endl;
		std::cout << "lap	- list all participator meetings" << std::endl;
		std::cout << "qm	- query meeting by title" << std::endl;
		std::cout << "qt	- query meeting by time interval" << std::endl;
		std::cout << "dm	- delete meeting by title" << std::endl;
		std::cout << "da	- delete all meetings" << std::endl;
		//std::cout << "" << std::endl;
		std::cout << "----------------------------------------------------------------"
			<< "------------------------------" << std::endl;
		std::cout << "Agenda :~$ ";
		is_ex = executeOperation(getOperation());
	}
}

std::string AgendaUI::getOperation() {
	std::string userinput;
	cout << "Agenda@" << m_userName << " : # ";
	std::cin >> userinput;
	return userinput;
}

bool AgendaUI::executeOperation(std::string t_operation) {
	if (t_operation == "o") {
		userLogOut();
		return false;
	}
	else if (t_operation == "s") {
		userCountReset();
	}
	else if (t_operation == "dc") {
		deleteUser();
		return false;
	}
	else if (t_operation == "lu") {
		listAllUsers();
	}
	else if (t_operation == "cm") {
		createMeeting();
	}
	else if (t_operation == "amp") {
		//add meeting participator
		//listAllSponsorMeetings();
		cout << "input the title of title and Participator's username:" << endl;
		cout << "[title] [username]" << endl;
		string n_title,add_username;
		cin >> n_title >> add_username;
		bool is_add = m_agendaService.addMeetingParticipator(m_userName, n_title, add_username);
		if (is_add)	cout << "add a participator suceed!" << endl;
		else cout << "failed" << endl;

	}
	else if (t_operation == "rmp") {
		//remove meeting participator
		listAllSponsorMeetings();
		cout << "input the title of title and Participator's username:" << endl;
		cout << "[title] [username]" << endl;
		string n_title, add_username;
		cin >> n_title >> add_username;
		bool is_add = m_agendaService.removeMeetingParticipator(m_userName, n_title, add_username);
		if (is_add)	cout << "remve the participator suceed!" << endl;
		else cout << "failed" << endl;


	}
	else if (t_operation == "rqm") {
		//request to quit meeting
		listAllParticipateMeetings();
		cout << "input the title of meeting" << endl;
		cout << "[title]" << endl;
		string n_title;
		cin >> n_title;
		bool is_res = m_agendaService.quitMeeting(m_userName, n_title);
		if (is_res)	cout << "request is succeed" << endl;
		else cout << "failed" << endl;
	}
	else if (t_operation == "la") {
		
		listAllMeetings();

	}
	else if (t_operation == "las") {
		listAllSponsorMeetings();
	}
	else if (t_operation == "lap") {
		listAllParticipateMeetings();
	}
	else if (t_operation == "qm") {
		queryMeetingByTitle();
	}
	else if (t_operation == "qt") {
		queryMeetingByTimeInterval();
	}
	else if (t_operation == "dm") {
		deleteMeetingByTitle();
	}
	else if (t_operation == "da") {
		deleteAllMeetings();
	}
	else {
		cout << "Wrong Instruction!" << endl;
	}
	return true;
}

void AgendaUI::userCountReset() {
	std::cout << "---------------------User Count Reset-------------------" << endl;
	std::cout << "---------------------User message-----------------------" << endl;
	std::cout << "userName : " << m_userName << endl;
	std::cout << "email : ";
	User target = m_agendaService.getUser(m_userName);
	cout << target.getEmail() << endl;
	cout << "phone : " << target.getPhone() << endl;
	std::string userinput;
	do
	{
		std::cout << endl;
		std::cout << "n		- change your UserName" << std::endl;
		std::cout << "p		- change your password" << std::endl;
		std::cout << "t		- change your telphone" << endl;
		std::cout << "e		- change your emial" << std::endl;
		std::cout << "q		- back to Agenda";
		std::cout << "----------------------------------------------------------------"
			<< "------------------------------" << std::endl;
		std::cout << "Agenda :~$ ";
		cin >> userinput;
		if (userinput == "n") {
			changeName();
		}
		else if (userinput == "p") {
			changePassword();
		}
		else if (userinput == "t") {
			changePhone();
		}
		else if (userinput == "e") {
			changeEmail();
		}
		else if (userinput == "q") {
			break;
		}
		else {
			cout << "Wrong Instruction" << endl;
		}
	} while (userinput[0] != 'q');
}

void AgendaUI::changeName() {
	std::string newname;
	cout << "[new name]" << endl;
	cin >> newname;
	bool is = m_agendaService.updateUserName(m_userName, m_userPassword, newname);
	if (is) {
		m_userName = newname;
		cout << "Your userName has changed!" << endl;
		cout << "Your New name : " << newname << endl;
	}
	else {
		cout << "change name fail" << endl;
	}
}

void AgendaUI::changePassword() {
	std::string comfirm;
	std::string newpassoword;
	std::string secondpassword;
	cout << endl;
	cout << "please input your current password!" << endl;
	cout << "[password]" << endl;
	cin >> comfirm;
	if (comfirm == m_userPassword) {
		cout << "right!" << endl;
		cout << "please input your new password!" << endl;
		cout << "[newpassword]" << endl;
		cin >> newpassoword;
		while(true) {
			cout << "please input again to cheak" << endl;
			cout << "[newpassword]";
			cin >> secondpassword;
			if (secondpassword == newpassoword) {
				cout << "right" << endl;
				break;
			}
			else {
				cout << "not same,try again ";
			}
		}
		char tag;
		cout << endl;
		cout << "Are you sure to change your password?(Y or N) :";
		cin >> tag;
		if (tag == 'Y') {
			bool is = m_agendaService.updateUserpassword(m_userName, comfirm, newpassoword);
			if (is) {
				cout << "Your password has changed!" << endl;
			}
			else {
				cout << "change password fail!" << endl;
			} 
		}
		else if (tag == 'N') {
			cout << "you quit to change password." << endl;
		}
		else
		{
			cout << "Wrong Instruction,quit to change password" << endl;
		}


	}
	else {
		cout << "Wrong Password!" << endl;
	}
}

void AgendaUI::changePhone() {
	std::string newphone;
	cout << "[new phone]" << endl;
	cin >> newphone;
	bool is = m_agendaService.updateUserphone(m_userName, m_userPassword, newphone);
	if (is) {
		cout << "Your phone has changed!" << endl;
		cout << "Your New phone : " << newphone << endl;
	}
	else {
		cout << "change phone fail" << endl;
	}
}

void AgendaUI::changeEmail() {
	std::string newemail;
	cout << "[new email]" << endl;
	cin >> newemail;
	bool is = m_agendaService.updateUserphone(m_userName, m_userPassword, newemail);
	if (is) {
		cout << "Your email has changed!" << endl;
		cout << "Your New email : " << newemail << endl;
	}
	else {
		cout << "change email fail" << endl;
	}
}

void AgendaUI::userLogIn(void) {
	std::cout << std::endl << "[log in] [username] [password]" << std::endl;
	std::cout << "[log in] ";
	std::string t_username;
	std::string t_password;
	std::cin >> t_username >> t_password;
	m_agendaService.userLogIn(t_username, t_password);
	bool test = m_agendaService.userLogIn(t_username, t_password);
	if (test) {
		m_userName = t_username;
		m_userPassword = t_password;
		std::cout << "[log in] succeed!" << std::endl;
		std::cout << std::endl;
		startAgenda();
	}
	else
	{
		std::cout << "[log in] Password error or user doesn't exist" << std::endl;
	}
}

void AgendaUI::userRegister() {
	cout << endl;
	cout << "[register] [username] [passowrd] [email] [phone]" << endl;
	cout << "[register] ";
	string n_username, n_password, n_email, n_phone;
	cin >> n_username >> n_password >> n_email >> n_phone;
	bool is_register = m_agendaService.userRegister(n_username, n_password, n_email, n_phone);
	if (is_register) cout << "[register] succeed!" << endl;
	else cout << "[error] register failed!" << endl;
}

void AgendaUI::userLogOut() {
	m_userName = "";
	m_userPassword = "";
	cout << endl;
}

void AgendaUI::quitAgenda() {
	cout << endl << "Thank you for using Agenda. Good bye!" << std::endl;
}

void AgendaUI::deleteUser() {
	bool is_del = m_agendaService.deleteUser(m_userName, m_userPassword);
	if (is_del)	cout << "[delete Agenda account] succeed!" << endl;
	else cout << "[delete Agenda account] failed!" << endl;
}

void AgendaUI::listAllUsers() {
	cout << "[list all users]" << endl;
	cout << "      name        email          phone   " << endl;
	list<User> u = m_agendaService.listAllUsers();
	auto itor = u.begin();
	for (; itor != u.end(); ++itor) {
		cout << setw(10) << (*itor).getName();
		cout << setw(15) << itor->getEmail();
		cout << setw(15) << itor->getPhone() << endl;
	}

}

void AgendaUI::createMeeting() {
	cout << "[create meeting] [the number of participators]" << endl;
	cout << "[create meeting] ";
	int num;
	cin >> num;
	vector<string> part;
	for (int i = 0; i < num; i++) {
		cout << "[create meeting]  [please enter the participator "<<i+1<<"]" << endl;
		cout << "[create meeting] ";
		string temp;
		cin >> temp;
		part.push_back(temp);
	}
	cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
	string n_title,n_start, n_end;
	cout << "[create meeting] ";
	cin >> n_title >> n_start >> n_end;
	int is_create = m_agendaService.createMeeting(m_userName, n_title, n_start, n_end, part);
	if (is_create) {
		cout << "[create meeting] succeed!" << endl;
	}
	else
	{
		cout << "[create meeting] error!" << endl;
	}
}

void AgendaUI::listAllMeetings() {
	list<Meeting> a = m_agendaService.listAllMeetings(m_userName);
	printMeetings(a);
}

void AgendaUI::listAllSponsorMeetings() {
	list<Meeting> a = m_agendaService.listAllSponsorMeetings(m_userName);
	printMeetings(a);
}

void AgendaUI::listAllParticipateMeetings() {
	list<Meeting> a = m_agendaService.listAllParticipateMeetings(m_userName);
	printMeetings(a);
}

void AgendaUI::queryMeetingByTitle() {
	cout << endl;
	cout << "[query meetings] [title]" << endl;
	cout << "[query meetings] ";
	string n_title;
	cin >> n_title;
	cout << endl;
	list<Meeting> a = m_agendaService.meetingQuery(m_userName, n_title);
	printMeetings(a);
}

void AgendaUI::queryMeetingByTimeInterval() {
	cout << endl;
	cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
	cout << "[query meetings] ";
	string start, end;
	cin >> start >> end;

	cout << "[query meetings]" << endl << endl;
	list<Meeting> a = m_agendaService.meetingQuery(m_userName, start, end);
	printMeetings(a);
	cout << endl;
}

void AgendaUI::printMeetings(list<Meeting> a) {
	cout << "[list all participator meetings]" << endl << endl;
	//												  2018-02-22/22:00
	cout << "     title       sponsor       start time       end time          participator" << endl;
	auto itor = a.begin();
	for (; itor != a.end(); ++itor) {
		cout << setw(10) << itor->getTitle();
		cout << setw(14) << itor->getSponsor();
		cout << ' ' <<  itor->getStartDate().dateToString(itor->getStartDate()) << " ";
		cout << ' ' <<itor->getEndDate().dateToString(itor->getEndDate()) << "          ";
		vector<string> name = itor->getParticipator();
		int size = name.size();
		for (int i = 0; i < size; ++i) {
			cout << name[i];
			if (i != name.size() - 1)
				cout << ",";
		}
		cout << endl;
	}
}

void AgendaUI::deleteMeetingByTitle() {
	cout << "[delete meeting] [title]" << endl;
	cout << "[delete meeting] ";
	string n_title;
	cin >> n_title;
	cout << endl;
	bool is_del = m_agendaService.deleteMeeting(m_userName, n_title);
	if (is_del) cout << "[delete meeting by title] succeed!" << endl;
	else cout << "[error] delete meeting fail!" << endl;
}

void AgendaUI::deleteAllMeetings() {
	bool is_del = m_agendaService.deleteAllMeetings(m_userName);
	if (is_del) cout << "[delete all meetings] succeed!" << endl;
	else cout << "[error] delete all meetings fail!" << endl;
}

