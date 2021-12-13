#pragma once
#include <iostream>
#include "User.h"
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "nlohmann/json.hpp"
#include <algorithm>
using namespace std;
using json = nlohmann::json;

class Chat
{
public:
	//WARNING: USER 1 and USER 2 POINTER WILL NOT BE LOADED FROM THE LOAD SAVE FILE
	User* userPointer1;
	User* userPointer2;
	int starterMessage; //starter Message is the name of the user that will print the message
	string filename; 
	vector<string> messages;
	//DO NOT USE THIS CONSTRUCTOR
	Chat()
	{
		
	}
	Chat(User* user1, User* user2, User *currentUser)
	{
		userPointer1 = user1;
		userPointer2 = user2;
		bool swapped = false;
		if (userPointer1->userId > userPointer2->userId)
		{
			swap(userPointer1, userPointer2);
			swapped = true;
		}
		filename = "save_data/Chat" + userPointer1->userId + "_" + userPointer2->userId+".json";
		ifstream file;
		file.open(filename);
		if (!file)
		{
			if (swapped)
			{
				starterMessage = 2;
			}
			else
			{
				starterMessage = 1;
			}
			saveChat();
		}
		else
		{
			loadChat();
		}
		file.close();
	}
	void loadChat()
	{
		json jsonLoad;
		fstream file(filename, ios::in);
		file >> jsonLoad;
		starterMessage = jsonLoad["starterMessage"];
		messages.clear();
		for (auto iterator = jsonLoad["messages"].begin(); iterator != jsonLoad["messages"].end(); iterator++)
		{
			messages.push_back(*iterator);
		}
	}
	void saveChat()
	{
		json jsonSave;
		fstream file(filename, ios::out);
		jsonSave["messages"] = messages;
		jsonSave["starterMessage"] = starterMessage;
		file << jsonSave;
		file.close();
	}

	void writeMessage(string message, User* currentUser)
	{
		int userNumber;
		if (currentUser->userId == userPointer1->userId)
		{
			userNumber = 1;
		}
		else
		{
			userNumber = 2;
		}
		
		int lastUser;
		if (messages.size() % 2 == 0)
		{
			if (starterMessage == 1)
			{
				lastUser = 2;
			}
			else
			{
				lastUser = 1;
			}
		}
		else
		{
			lastUser = starterMessage;
		}

		if (messages.size() == 0)
		{
			messages.push_back(message);
		}
		else if (userNumber == lastUser)
		{
			messages[messages.size() - 1] += "\n" + message;
		}
		else
		{
			if (userNumber == 1)
			{
				userPointer2->constructNewNotification(NotificationType::MAIL, currentUser->userName, "");
			}
			else
			{
				userPointer1->constructNewNotification(NotificationType::MAIL, currentUser->userName, "");
			}
			messages.push_back(message);
		}
		saveChat();
	}
};