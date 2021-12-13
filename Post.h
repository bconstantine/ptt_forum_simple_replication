#pragma once
#include "nlohmann/json.hpp"
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;
using json = nlohmann::json;

struct Comment
{
	string commentId; //based on date
	string comment;
	string userId; //based on date
	int commentType; //0 for usual comment, 1 for tuiwen, 2 for xuwen
};

enum DeleteReason
{
	FAILDELETE,
	INAPPROPRIATE,
	SPAM,
	USEROWNDELETE,
	NODELETE
};

class Post
{
public:
	//static json postsCollection put inside board
	string postTitle; 
	string postContent;
	vector <Comment> userComment;
	string postId; //based on date
	string posterId; //based on date
	int views;
	int popularityPoints;
	vector <string> tuiwenUsers;
	vector <string> xuwenUsers; 

	Post()
	{
		
	}

	Post(string postTitle, string postContent, string posterId, string boardTitle)
	{
		this->postTitle = postTitle;
		this->postContent = postContent;
		this->postId = boardTitle + constructPostId();
		this->posterId = posterId;
		views = 0;
		popularityPoints = 0;
	}

	static string constructPostId()
	{
		time_t currentTime;
		struct tm tPointer;
		time(&currentTime);
		localtime_s(&tPointer, &currentTime);
		string idString = to_string(tPointer.tm_year) + to_string(tPointer.tm_mon) + to_string(tPointer.tm_mday) + to_string(tPointer.tm_hour) + to_string(tPointer.tm_min) + to_string(tPointer.tm_sec);
		return idString;
	}

	void countPopularity()
	{
		//count popularity needs to be called every time a board is loaded
		popularityPoints = 0;
		//view = 1 points
		//tui = 2 points
		//xu = -2 points
		popularityPoints += views;
		popularityPoints += tuiwenUsers.size() * 2;
		popularityPoints += xuwenUsers.size() * -2;
	}

	void editPostTitle(string resultEdit)
	{
		this->postTitle = resultEdit;
	}

	void editPostContent(string resultEdit)
	{
		this->postContent = resultEdit;
	}

	bool addComment(int commentType, string userId, string commentContent)
	{
		if (commentType == 0 || commentType == 2)
		{
			if (find(tuiwenUsers.begin(), tuiwenUsers.end(), userId) != tuiwenUsers.end())
			{
				return false;
			}

			if (find(xuwenUsers.begin(), xuwenUsers.end(), userId) != xuwenUsers.end())
			{
				return false;
			}
		}

		Comment newComment;
		newComment.comment = commentContent;
		newComment.commentId = constructPostId();
		newComment.userId = userId; 
		newComment.commentType = commentType;
		userComment.push_back(newComment);
		if (commentType == 0)
		{
			tuiwenUsers.push_back(userId);
			countPopularity();
		}
		else if (commentType == 2)
		{
			xuwenUsers.push_back(userId);
			countPopularity();
		}
		return true;
	}

	bool editComment(string commentId, string userId, string resultEdit)
	{
		int currentIndex = 0;
		for (auto iterator = userComment.begin(); iterator != userComment.end(); iterator++)
		{
			if (iterator->commentId == commentId)
			{
				break;
			}
			currentIndex++;
		}
		//comment not exist
		if (currentIndex == userComment.size())
		{
			return false;
		}

		if (userComment[currentIndex].userId != userId)
		{
			return false;
		}
		else
		{
			userComment[currentIndex].comment = resultEdit; 
			return true;
		}
	}

	DeleteReason deleteComment(string commentId, string userId, bool isAdmin, DeleteReason reason)
	{
		int currentIndex = 0;
		for (auto iterator = userComment.begin(); iterator != userComment.end(); iterator++)
		{
			if (iterator->commentId == commentId)
			{
				break;
			}
			currentIndex++;
		}
		//comment not exist
		if (currentIndex == userComment.size())
		{
			return FAILDELETE;
		}

		//comment exist
		if (userComment[currentIndex].userId == userId)
		{
			if (userComment[currentIndex].commentType == 0)
			{
				auto iterator = find(tuiwenUsers.begin(), tuiwenUsers.end(), userId);
				tuiwenUsers.erase(iterator);
			}
			else if (userComment[currentIndex].commentType == 2)
			{
				auto iterator = find(xuwenUsers.begin(), xuwenUsers.end(), userId);
				xuwenUsers.erase(iterator);
			}

			//immediately delete
			userComment.erase(userComment.begin() + currentIndex);
			countPopularity();
			return USEROWNDELETE;
		}
		else if (isAdmin)
		{
			if (userComment[currentIndex].commentType == 0)
			{
				auto iterator = find(tuiwenUsers.begin(), tuiwenUsers.end(), userId);
				tuiwenUsers.erase(iterator);
			}
			else if (userComment[currentIndex].commentType == 2)
			{
				auto iterator = find(xuwenUsers.begin(), xuwenUsers.end(), userId);
				xuwenUsers.erase(iterator);
			}

			//immediately delete
			userComment.erase(userComment.begin() + currentIndex);
			countPopularity();
			return reason;
		}
		else
		{
			return FAILDELETE;
		}
	}
};
