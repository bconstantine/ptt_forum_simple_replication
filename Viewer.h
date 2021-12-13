#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <conio.h>
#include <windows.h>

#include "nlohmann/json.hpp"
#include "User.h"
#include "Board.h"
#include "Chat.h"
#include "DungeonGame/main.h"

using namespace std;
using json = nlohmann::json;

enum Color
{
	L_BLACK,
	L_BLUE,
	L_GREEN,
	L_CYAN,
	L_RED,
	L_PINK,
	L_YELLOW,
	L_WHITE,
	S_BLACK,
	S_BLUE,
	S_GREEN,
	S_CYAN,
	S_RED,
	S_PINK,
	S_YELLOW,
	S_WHITE,
};

enum PageSelection { BOARDS, CHATS, GAMES, NOTIFICATIONS, EXIT };

class Viewer
{
public:
	static vector<Admin> fakeAdmins;
	static vector<Guest> fakeGuests;
	static vector<Member> fakeMembers;
	static vector<string> fakeUsername;

	//username will be returned in the username section
	//return false if the id is not found
	static bool userNameSearch(string userId, string& userName)
	{
		for (auto iterator = fakeAdmins.begin(); iterator != fakeAdmins.end(); iterator++)
		{
			if (iterator->userId == userId)
			{
				userName = iterator->userName;
				return true;
			}
		}

		for (auto iterator = fakeMembers.begin(); iterator != fakeMembers.end(); iterator++)
		{
			if (iterator->userId == userId)
			{
				userName = iterator->userName;
				return true;
			}
		}
		return false;
	}


	static void fakeLoadUserJSON()
	{
		json userJson;
		fakeAdmins.clear();
		fakeGuests.clear();
		fakeMembers.clear();
		fakeUsername.clear();
		fstream jsonFile("save_data/User.json", ios::in);
		jsonFile >> userJson;
		jsonFile.close();
		for (auto iterator = userJson.begin(); iterator != userJson.end(); iterator++)
		{
			int adminLevelTemp = 2;
			string userIdTemp = iterator.key();
			string userNameTemp;
			string passwordTemp;
			vector<string> userPostIDTemp;
			vector<string> notificationTemp;
			for (auto innerIterator = iterator->begin(); innerIterator != iterator->end(); innerIterator++)
			{
				if (innerIterator.key() == "adminLevel")
				{
					adminLevelTemp = innerIterator.value();
				}
				else if (innerIterator.key() == "userName")
				{
					userNameTemp = innerIterator.value();
				}
				else if (innerIterator.key() == "password")
				{
					passwordTemp = innerIterator.value();
				}
				else if (innerIterator.key() == "userPostID")
				{
					for (auto vectorIterator = innerIterator->begin(); vectorIterator != innerIterator->end(); vectorIterator++)
					{
						userPostIDTemp.push_back(*vectorIterator);
					}
				}
				else if (innerIterator.key() == "notification")
				{
					for (auto vectorIterator = innerIterator->begin(); vectorIterator != innerIterator->end(); vectorIterator++)
					{
						notificationTemp.push_back(*vectorIterator);
					}
				}
			}
			fakeUsername.push_back(userNameTemp);
			if (adminLevelTemp == 0)
			{
				Admin tempAdmin(userIdTemp, userNameTemp, passwordTemp, userPostIDTemp, notificationTemp);
				fakeAdmins.push_back(tempAdmin);
			}
			else if (adminLevelTemp == 1)
			{
				Member tempMember(userIdTemp, userNameTemp, passwordTemp, userPostIDTemp, notificationTemp);
				fakeMembers.push_back(tempMember);
			}
		}
	}

	static bool editComment(int index, string commentEdit, Board* currentBoard, User* currentUser)
	{
		Comment* currentComment = &(currentBoard->currentPost->userComment[index]);
		if (currentComment->userId != currentUser->userId)
		{
			return false;
		}
		else
		{
			currentComment->comment = commentEdit;
			currentBoard->savePostJSON();
			return true;
		}
	}

	static bool checkPostSameUser(User* currentUser, Board* currentBoard)
	{
		if (currentBoard->currentPost->posterId == currentUser->userId)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	static void resetColor()
	  {
		  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	  }

	static void setColor(int bgColor, int textColor)
	  {
		  int colorNumber = (16 * bgColor) + textColor;

		  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNumber);
	  }

	static void colorText(int bgColor, int textColor, string text)
	  {
		  int colorNumber = (16 * bgColor) + textColor;

		  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNumber);

		  cout << text;

		  resetColor();
	  }

	static void printGreetings(string username);
	static void printHeader(string title);
	static void printCenter(string text);

	static void displayMenu(string& accessType, string& username, bool& usernameTyped, string& password, bool& passwordTyped);
	static void displaySelectBoard(User* user, PageSelection& chosen);
	static void displayBoard(User* user, bool& isBack, string& cmd, int& chosenBoard);
	static void displayPostList(User* user, Board* currentBoard, bool& isBack, int& chosenPost, string& cmd);
	static void displayPost(User* user, Board* currentBoard, bool& isBack, string& finalComment, bool& deleting, int& deleteIndex, bool& deletingPost, bool& editingPost);
	static void displayNotification(User* user, bool& isBack);
	static void displayChatPage(bool& isBack, string& userTarget);
	static void displayChat(bool& isBack, User* user1, User* user2);
	static void showLoading();

	static void displayDungeon(bool& isBack)
	{
		system("cls");
		launchDungeon(isBack);
		system("cls");
	}
};
