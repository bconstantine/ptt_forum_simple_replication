#pragma once
#include "nlohmann/json.hpp"
#include "Post.h"
#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;
using json = nlohmann::json;

enum NotificationType
{
	MAIL,
	POST_STRIKE,
	COMMENT_STRIKE
};

class User
{
public:
	//guest cannot comment and post
	int adminLevel; //0 admin, 1 member, 2 guest
	string userId;
	string userName;
	string password;
	vector<string> userPostID;
	vector<string> notification; 


	User()
	{

	}
	User(int adminLevel, string userName, string password)
	{
		//username double is handled inside the boardManager
		this->adminLevel = adminLevel;
		this->userName = userName;
		this->userId = constructId();
		this->password = password;
	}

	string static constructId()
	{
		//for constructing the userId
		time_t currentTime;
		struct tm tPointer;
		time(&currentTime);
		localtime_s(&tPointer, &currentTime);
		string idString = to_string(tPointer.tm_year) + to_string(tPointer.tm_mon) + to_string(tPointer.tm_mday) + to_string(tPointer.tm_hour) + to_string(tPointer.tm_min) + to_string(tPointer.tm_sec);
		return idString;
	}

	static bool validPassword(string password)
	{
		//length should have been more than eight
		//alphanumeric
		//can contain _
		if (password.length() < 8)
		{
			return false;
		}
		bool containNumber = false;
		bool containAlpha = false;
		bool containWeird = false;

		for (auto iterator = password.begin(); iterator != password.end(); iterator++)
		{
			if (*iterator >= '0' && *iterator <= '9')
			{
				containNumber = true;
			}
			else if ((*iterator >= 'a' && *iterator <= 'z') || (*iterator >= 'A' && *iterator <= 'Z'))
			{
				containAlpha = true;
			}
			else
			{
				if (*iterator == '_')
				{
					continue;
				}
				containWeird = true;
				break;
			}
		}

		if (containWeird)
		{
			return false;
		}
		else if (containAlpha && containNumber)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	static bool validUserName(string userName)
	{
		//length should have been 8 - 12
		//alphanumeric only
		//length should have been more than eight
		//alphanumeric
		//can contain _
		//should not contain __GUEST__
		if (userName.length() < 8 && userName.length() > 12)
		{
			return false;
		}

		if (userName.find("__GUEST__") != string::npos)
		{
			return false;
		}
		bool containNumber = false;
		bool containAlpha = false;
		bool containWeird = false;

		for (auto iterator = userName.begin(); iterator != userName.end(); iterator++)
		{
			if (*iterator >= '0' && *iterator <= '9')
			{
				containNumber = true;
			}
			else if ((*iterator >= 'a' && *iterator <= 'z') || (*iterator >= 'A' && *iterator <= 'Z'))
			{
				containAlpha = true;
			}
			else
			{
				if (*iterator == '_')
				{
					continue;
				}
				containWeird = true;
				break;
			}
		}

		if (containWeird)
		{
			return false;
		}
		else if (containAlpha && containNumber)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void changeUserName(string userName)
	{
		//changing the username, making sure to also change the json (in boardManager)
		this->userName = userName;
	}
	void changePassword(string password)
	{
		//changing the password, making sure to also change the json in boardManager
		this->password = password;
	}
	void constructNewNotification(NotificationType notifCode, string usernameOrPostCode, string postTitle, DeleteReason deleteCode = NODELETE)
	{
		string message;
		if (notifCode == MAIL)
		{
			message = usernameOrPostCode + " has sent you a message, kindly check your mail.";
		}
		else if (notifCode == POST_STRIKE)
		{
			message = "Your post with title: [" + postTitle + "]\nwith code: [" + usernameOrPostCode + "]\n has been deleted with the following reason: ";
			if (deleteCode == INAPPROPRIATE)
			{
				message += "INAPPROPRIATE CONTENT.";
			}
			else if (deleteCode == SPAM)
			{
				message += "SPAM CONTENT.";
			}
		}
		else if (notifCode == COMMENT_STRIKE)
		{
			message = "Your comment in a post with title: [" + postTitle + "]\nwith code: [" + usernameOrPostCode + "]\nhas been deleted with the following reason: ";
			if (deleteCode == INAPPROPRIATE)
			{
				message += "INAPPROPRIATE CONTENT.";
			}
			else if (deleteCode == SPAM)
			{
				message += "SPAM CONTENT.";
			}
		}
		if (!message.empty())
		{
			notification.push_back(message);
		}
	}

	int notificationAmount()
	{
		return notification.size();
	}

	void clearNotification()
	{
		notification.clear();
	}

	void addNewPost(string postAddress)
	{
		userPostID.push_back(postAddress);
	}
	bool deletePost(string postAdress)
	{
		int index = 0;
		while (index != userPostID.size())
		{
			if (userPostID[index] == postAdress)
			{
				break;
			}
			index++;
		}
		if (index != userPostID.size())
		{
			userPostID.erase(userPostID.begin() + index);
			return true;
		}
		else
		{
			return false;
		}
	};

	void deleteAllPostContainingTitle(string postTitle)
	{
		for (int index = 0; index < userPostID.size(); index++)
		{
			if (userPostID[index].find(postTitle) != string::npos)
			{
				userPostID.erase(userPostID.begin() + index);
				index--;
			}
		}
	}

};

class Admin : public User
{
public:
	Admin()
	{

	}
	Admin(string userName, string password)
	{
		//username double is handled inside the boardManager
		this->adminLevel = 0;
		this->userName = userName;
		this->userId = constructId();
		this->password = password;
	}
	Admin(string userId, string userName, string password, vector<string> userPostID, vector<string> notification)
	{
		this->adminLevel = 0;
		this->userId = userId;
		this->userName = userName;
		this->password = password;
		this->userPostID = userPostID;
		this->notification = notification;
	}
};

class Member : public User
{
public:
	Member()
	{
	}
	Member(string userName, string password)
	{
		//username double is handled inside the boardManager
		this->adminLevel = 1;
		this->userName = userName;
		this->userId = constructId();
		this->password = password;
	}
	Member(string userId, string userName, string password, vector<string> userPostID, vector<string> notification)
	{
		this->adminLevel = 1;
		this->userId = userId;
		this->userName = userName;
		this->password = password;
		this->userPostID = userPostID;
		this->notification = notification;
	}
};

class Guest : public User
{
public:
	Guest()
	{
		//username double is handled inside the boardManagerl
		this->adminLevel = 2;
		this->userId = "-" + constructId();
		this->userName = "__GUEST__";
		this->password = "";
	}
};