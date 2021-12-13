#pragma once

#include <iostream>
#include <vector>
#include <conio.h>
#include <sstream>
#include "User.h"
#include "Post.h"
#include "Viewer.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include "Board.h"
using namespace std;
using json = nlohmann::json;

// =========================== BoardState ===========================

enum BoardState
{
	MENU,
	SELECT_BOARD,
	BOARD,
	POST_LIST,
	POST,
	CHAT_PAGE,
	NOTIFICATION,
	USER_POSTS,
	GAME_PAGE
};

// ===================================================================

class BoardManager
{
public:
	json userJson;
	User* currentUser = NULL; 
	vector<Admin> allAdmins;
	vector<Guest> allGuests;
	vector<Member> allMembers;
	vector<string> allUsername;
	Board* currentBoard = NULL;
	vector<Board> collectionOfAllBoard;

	string accessType = ""; //not save
	string username = ""; //not save
	bool usernameTyped = false; //not save
	string password = ""; //not save
	bool passwordTyped = false; //not save


	BoardState currentState = BoardState::MENU; //NOT SAVE 
	bool isExit = false; //NOT SAVE

	//username will be returned in the username section
	//return false if the id is not found
	bool userNameSearch(string userId, string& userName)
	{
		for (auto iterator = allAdmins.begin(); iterator != allAdmins.end(); iterator++)
		{
			if (iterator->userId == userId)
			{
				userName = iterator->userName;
				return true;
			}
		}

		for (auto iterator = allMembers.begin(); iterator != allMembers.end(); iterator++)
		{
			if (iterator->userId == userId)
			{
				userName = iterator->userName;
				return true;
			}
		}
		return false;
	}

	bool userNameSearchForUser(User* &user, string userName)
	{
		for (int index = 0; index < allAdmins.size(); index++)
		{
			if (allAdmins[index].userName == userName)
			{
				user = &allAdmins[index];
				return true;
			}
		}

		for (int index = 0; index < allMembers.size(); index++)
		{
			if (allMembers[index].userName == userName)
			{
				user = &allMembers[index];
				return true;
			}
		}
		return false;
	}

	void startLoop();


	void loadUserJSON()
	{
		userJson.clear();
		allAdmins.clear();
		allGuests.clear();
		allMembers.clear();
		allUsername.clear();
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
			allUsername.push_back(userNameTemp);
			if (adminLevelTemp == 0)
			{
				Admin tempAdmin(userIdTemp, userNameTemp, passwordTemp, userPostIDTemp, notificationTemp);
				allAdmins.push_back(tempAdmin);
			}
			else if (adminLevelTemp == 1)
			{
				Member tempMember(userIdTemp, userNameTemp, passwordTemp, userPostIDTemp, notificationTemp);
				allMembers.push_back(tempMember);
			}
		}
	}
	void saveUserJSON()
	{
		userJson.clear();
		for (auto iterator = allAdmins.begin(); iterator != allAdmins.end(); iterator++)
		{
			json tempJson;
			tempJson["adminLevel"] = iterator->adminLevel;
			tempJson["userName"] = iterator->userName;
			tempJson["password"] = iterator->password;
			tempJson["userPostID"] = iterator->userPostID;
			tempJson["notification"] = iterator->notification;
			userJson[iterator->userId] = tempJson;
		}
		for (auto iterator = allMembers.begin(); iterator != allMembers.end(); iterator++)
		{
			json tempJson;
			tempJson["adminLevel"] = iterator->adminLevel;
			tempJson["userName"] = iterator->userName;
			tempJson["password"] = iterator->password;
			tempJson["userPostID"] = iterator->userPostID;
			tempJson["notification"] = iterator->notification;
			userJson[iterator->userId] = tempJson;
		}
		fstream jsonFile("save_data/User.json", ios::out);
		jsonFile << userJson;
		jsonFile.close();
	}

	//check for duplicate, true for no duplicate, WARNING: DONT CHECK THE USERNAME VALIDITY
	bool checkForDuplicateUsername()
	{
		for (auto iterator = allAdmins.begin(); iterator != allAdmins.end(); iterator++)
		{
			if (iterator->userName == username)
			{
				return false;
			}
		}

		for (auto iterator = allMembers.begin(); iterator != allMembers.end(); iterator++)
		{
			if (iterator->userName == username)
			{
				return false;
			}
		}

		return true;
	}

	void createNewUser()
	{
		Member tempUser(username, password);
		allMembers.push_back(tempUser);
		currentUser = &(allMembers[allMembers.size() - 1]);
		allUsername.push_back(username);
	}

	void createNewAdmin()
	{
		Admin tempAdmin(username, password);
		allAdmins.push_back(tempAdmin);
		currentUser = &(allAdmins[allAdmins.size() - 1]);
		allUsername.push_back(username);
	}

	bool loginChecker()
	{
		bool loginFound = false;
		int index = 0;
		for (auto iterator = allAdmins.begin(); iterator != allAdmins.end(); iterator++)
		{
			if (iterator->userName == username && iterator->password == password)
			{
				loginFound = true;
				currentUser = &allAdmins[index];
				return true;
			}
			index++;
		}

		if (!loginFound)
		{
			int index = 0;
			for (auto iterator = allMembers.begin(); iterator != allMembers.end(); iterator++)
			{
				if (iterator->userName == username && iterator->password == password)
				{
					loginFound = true;
					currentUser = &allMembers[index];
					return true;
				}
				index++;
			}

			return false;
		}
	}

	void guessLogin()
	{
		Guest tempGuess;
		allGuests.push_back(tempGuess);
		currentUser = &allGuests[0];
	}

	bool addNewBoard(string title)
	{
		for (auto iterator = Board::allTitleXBoardMakerId.begin(); iterator != Board::allTitleXBoardMakerId.end(); iterator++)
		{
			if (iterator->first == title)
			{
				//duplicatae name of board
				return false;
			}
		}
		Board::allTitleXBoardMakerId.push_back(make_pair(title, currentUser->userId));
		Board::saveAllTitleJSON();
		Board newBoard;
		newBoard.boardTitle = title;
		newBoard.boardMakerId = currentUser->userId;
		collectionOfAllBoard.push_back(newBoard);
		return true;
	}

	bool deleteBoard(string title)
	{
		bool titleFound = false;
		auto fixIterator = Board::allTitleXBoardMakerId.begin();
		for (auto iterator = Board::allTitleXBoardMakerId.begin(); iterator != Board::allTitleXBoardMakerId.end(); iterator++)
		{
			if (iterator->first == title)
			{
				fixIterator = iterator;
				titleFound = true;
			}
		}
		if (!titleFound)
		{
			return false;
		}

		titleFound = false;
		auto fixBoardIterator = collectionOfAllBoard.begin();
		for (auto iterator = collectionOfAllBoard.begin(); iterator != collectionOfAllBoard.end(); iterator++)
		{
			if (iterator->boardTitle == title)
			{
				titleFound = true;
				fixBoardIterator = iterator;
			}
		}

		if (!titleFound)
		{
			return false;
		}
		else
		{
			collectionOfAllBoard.erase(fixBoardIterator);
			Board::allTitleXBoardMakerId.erase(fixIterator);

			for (auto iterator = allAdmins.begin(); iterator != allAdmins.end(); iterator++)
			{
				iterator->deleteAllPostContainingTitle(title);
			}
			for (auto iterator = allMembers.begin(); iterator != allMembers.end(); iterator++)
			{
				iterator->deleteAllPostContainingTitle(title);
			}
			saveAll();
		}
	}

	void loadBoardJSON()
	{
		collectionOfAllBoard.clear();
		Board::loadAllTitleJSON();
		for (auto iterator = Board::allTitleXBoardMakerId.begin(); iterator != Board::allTitleXBoardMakerId.end(); iterator++)
		{
			Board temporaryBoard;
			temporaryBoard.boardTitle = (iterator->first);
			temporaryBoard.boardMakerId = (iterator->second);
			temporaryBoard.loadPostJSON();
			collectionOfAllBoard.push_back(temporaryBoard);
		}
	}

	void saveBoardJSON()
	{
		Board::saveAllTitleJSON();
		for (auto iterator = collectionOfAllBoard.begin(); iterator != collectionOfAllBoard.end(); iterator++)
		{
			iterator->savePostJSON();
		}
	}

	void createDummyJSON()
	{
		json jsonFile; 
		fstream file("save_data/User.json", ios::out);
		file << jsonFile;
		file.close();
		fstream anotherFile("save_data/ALLTITLE.json", ios::out);
		anotherFile << jsonFile;
		anotherFile.close();
		for (auto iterator = Board::allTitleXBoardMakerId.begin(); iterator != Board::allTitleXBoardMakerId.end(); iterator++)
		{
			file.open("save_data/" + iterator->first + "Post.json", ios::out);
			file << jsonFile;
			file.close();
		}
	}

	void loadAll()
	{
		loadUserJSON();
		loadBoardJSON();
	}

	void saveAll()
	{
		saveUserJSON();
		saveBoardJSON();
	}

	//Using the fact that currentBoard must be filled
	//Using the fact that currentPost of the Board must be filled
	bool checkPostSameUser()
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

	//commentIndex from the top of the screen
	bool checkCommentSameUser(int commentIndex)
	{
		if (currentBoard->currentPost->userComment[commentIndex].userId == currentUser->userId)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//WARNING CANT CHECK IF THE USER IS GUEST OR NOT
	void addPost(string title, string postContent)
	{
		Post tempPost(title, postContent, currentUser->userId, currentBoard->boardTitle);
		currentBoard->collectionOfAllPost.insert(currentBoard->collectionOfAllPost.begin(), tempPost);
		currentBoard->savePostJSON();
		currentBoard->currentPost = &(currentBoard->collectionOfAllPost[0]);
		currentUser->addNewPost(currentBoard->boardTitle + tempPost.postId);
		saveUserJSON();
		currentBoard->generateTopThree();
	}
	//WARNING WITH A CONDITION THAT CURRENTPOST MUST BE FILLED
	//COMMENT TYPE: 0 TUIWEN, 1 NEUTRAL, 2 XUWEN
	//WARNING: CANT CHECK IF THE USER IS GUEST OR NOT, RETURN FALSE IF AT ONE TIME THE USER HAS ALREADY
	//CONDUCTED XUWEN OR TUIWEN COMMENT, CANT DO THE SAME AGAIN
	bool addComment(int commentType, string commentContent)
	{
		if (!currentBoard->currentPost->addComment(commentType, currentUser->userId, commentContent))
		{
			return false;
		}
		currentBoard->currentPost->countPopularity();
		currentBoard->savePostJSON();
		currentBoard->generateTopThree();
		return true;
	}

	//Can check if it is the currentUser or not
	//return false if it finds out that it is a different user
	//WARNING: THE CURRENT POST POINTER MUST BE FILLED
	bool editPostContent(string contentEdit)
	{
		if (currentUser->userId != currentBoard->currentPost->posterId)
		{
			return false;
		}
		else
		{
			currentBoard->currentPost->editPostContent(contentEdit);
			currentBoard->savePostJSON();
			return true;
		}
	}

	//WARNING: CURRENTPOST MUST BE FILLED
	//RETURN FALSE IF IT IS NOT THE CURRENTUSER
	//INDEX IS USED TO COUNT THE COMMENT STARTING FROM THE UPMOST COMMENT
	bool editComment(int index, string commentEdit)
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
	//Using the fact that currentBoard must be filled
	//Using the fact that currentPost of the Board must be filled
	//Directly check the currentUser for if it is admin or not
	//Automatically send notification to the deleted user (unless it is normal delete by himself)
	bool deletePost(DeleteReason reason)
	{
		if (checkPostSameUser())
		{
			bool deleteFound = false;
			if (!currentUser->deletePost(currentBoard->boardTitle + currentBoard->currentPost->postId))
			{
				return false;
			}
			else
			{
				//starting deleting the post
				for (int index = 0; index < currentBoard->collectionOfAllPost.size(); index++)
				{
					if (currentBoard->currentPost->postId == currentBoard->collectionOfAllPost[index].postId)
					{
						deleteFound = true;
						currentBoard->collectionOfAllPost.erase(currentBoard->collectionOfAllPost.begin() + index);
						break;
					}
				}
			}
			currentBoard->closeAPost();
			saveAll();
			currentBoard->generateTopThree();
			return deleteFound;
		}
		else if (currentUser->adminLevel == 0)
		{
			//not the user, but it is admin and need to give the correct reason
			User* notifTarget = NULL; 
			
			bool targetFound = false;
			for (int index = 0; index < allAdmins.size(); index++)
			{
				if (currentBoard->currentPost->posterId == allAdmins[index].userId)
				{
					notifTarget = &allAdmins[index];
					targetFound = true;
					break;
				}
			}

			if (!targetFound)
			{
				for (int index = 0; index < allMembers.size(); index++)
				{
					if (currentBoard->currentPost->posterId == allMembers[index].userId)
					{
						notifTarget = &allMembers[index];
						targetFound = true;
						break;
					}
				}

				if (!targetFound)
				{
					return false;
				}
			}
			bool deleteFound = false;
			//user has been found in notifTarget
			if (!notifTarget->deletePost(currentBoard->boardTitle + currentBoard->currentPost->postId))
			{
				return false;
			}
			else
			{
				//Construct notification for the user
				notifTarget->constructNewNotification(NotificationType::POST_STRIKE, currentBoard->currentPost->postId, currentBoard->currentPost->postTitle, reason);
				//starting deleting the post
				for (int index = 0; index < currentBoard->collectionOfAllPost.size(); index++)
				{
					if (currentBoard->currentPost->postId == currentBoard->collectionOfAllPost[index].postId)
					{
						deleteFound = true;
						currentBoard->collectionOfAllPost.erase(currentBoard->collectionOfAllPost.begin() + index);
						currentBoard->savePostJSON();
						break;
					}
				}
			}
			currentBoard->closeAPost();
			saveAll();
			currentBoard->generateTopThree();
			return deleteFound;
		}
		else
		{
			return false;
		}
	}

	//Using the fact that currentBoard must be filled
	//Using the fact that currentPost of the Board must be filled
	//Directly check the currentUser for if it is admin or not
	//Automatically send notification to the deleted user (unless it is normal delete by himself)
	bool deleteComment(int index, DeleteReason reason)

	{
		if (index >= currentBoard->currentPost->userComment.size())
		{
			return false;
		}
		Comment* targetComment = &(currentBoard->currentPost->userComment[index]);
		if (checkCommentSameUser(index))
		{
			bool deleteFound = true;
			//starting deleting the comment
			if ((currentBoard->currentPost->userComment.begin() + index)->commentType == 0)
			{
				//tuiwen
				auto pointer = find(currentBoard->currentPost->tuiwenUsers.begin(), currentBoard->currentPost->tuiwenUsers.end(), (currentBoard->currentPost->userComment.begin() + index)->userId);

				if (pointer == currentBoard->currentPost->tuiwenUsers.end())
				{
					cout << "Unexpected error occured while deleting tuiwen!\n";
					system("pause");
				}
				else
				{
					currentBoard->currentPost->tuiwenUsers.erase(pointer);
				}
			}
			else if ((currentBoard->currentPost->userComment.begin() + index)->commentType == 2)
			{
				//xuwen
				auto pointer = find(currentBoard->currentPost->xuwenUsers.begin(), currentBoard->currentPost->xuwenUsers.end(), (currentBoard->currentPost->userComment.begin() + index)->userId);

				if (pointer == currentBoard->currentPost->xuwenUsers.end())
				{
					cout << "Unexpected error occured while deleting xuwen!\n";
					system("pause");
				}
				else
				{
					currentBoard->currentPost->xuwenUsers.erase(pointer);
				}
			}
			currentBoard->currentPost->userComment.erase(currentBoard->currentPost->userComment.begin() + index);
			saveAll();
			return deleteFound;
		}
		else if (currentUser->adminLevel == 0)
		{
			//not the user, but it is admin and need to give the correct reason
			User* notifTarget = NULL;
			bool targetFound = false;
			for (int index = 0; index < allAdmins.size(); index++)
			{
				if (targetComment->userId == allAdmins[index].userId)
				{
					notifTarget = &allAdmins[index];
					targetFound = true;
					break;
				}
			}

			if (!targetFound)
			{
				for (int index = 0; index < allMembers.size(); index++)
				{
					if (targetComment->userId == allMembers[index].userId)
					{
						notifTarget = &allMembers[index];
						targetFound = true;
						break;
					}
				}

				if (!targetFound)
				{
					return false;
				}
			}
			bool deleteFound = true;
			//user has been found in notifTarget
			//Construct notification for the user
			notifTarget->constructNewNotification(NotificationType::COMMENT_STRIKE, currentBoard->currentPost->postId, currentBoard->currentPost->postTitle, reason);
			
			//starting deleting the comment, from xuwen tuiwen list
			if ((currentBoard->currentPost->userComment.begin() + index)->commentType == 0)
			{
				//tuiwen
				auto pointer = find(currentBoard->currentPost->tuiwenUsers.begin(), currentBoard->currentPost->tuiwenUsers.end(), (currentBoard->currentPost->userComment.begin() + index)->userId);

				if (pointer == currentBoard->currentPost->tuiwenUsers.end())
				{
					cout << "Unexpected error occured while deleting tuiwen!\n";
					system("pause");
				}
				else
				{
					currentBoard->currentPost->tuiwenUsers.erase(pointer);
				}
			}
			else if ((currentBoard->currentPost->userComment.begin() + index)->commentType == 2)
			{
				//xuwen
				auto pointer = find(currentBoard->currentPost->xuwenUsers.begin(), currentBoard->currentPost->xuwenUsers.end(), (currentBoard->currentPost->userComment.begin() + index)->userId);

				if (pointer == currentBoard->currentPost->xuwenUsers.end())
				{
					cout << "Unexpected error occured while deleting xuwen!\n";
					system("pause");
				}
				else
				{
					currentBoard->currentPost->xuwenUsers.erase(pointer);
				}
			}
			currentBoard->currentPost->userComment.erase(currentBoard->currentPost->userComment.begin() + index); 
			saveAll();
			return deleteFound;
		}
		else
		{
			return false;
		}
	}
	void goToBoard(BoardState scrn_Dest);
	void inputKey();
	void back();
	void save();
};
