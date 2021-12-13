#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include "Post.h"
#include "nlohmann/json.hpp"
#include <algorithm>
#include "User.h"
#include <iomanip>
using json = nlohmann::json;

using namespace std;

class Board
{
public:
	Post* currentPost = NULL; //always set to null when not opening a post
	vector<Post> collectionOfAllPost;
	string boardTitle; 
	static vector<pair<string, string>> allTitleXBoardMakerId; 
	vector<Post*> topThree;
	string boardMakerId; //not yet made
	string toBePrinted;
	string getContent();

	void openAPost(int index)
	{
		currentPost = &collectionOfAllPost[index];
		collectionOfAllPost[index].views++;
		collectionOfAllPost[index].countPopularity();
		savePostJSON();
		generateTopThree();
	}

	void openAPostFromTopThree(int index)
	{
		currentPost = topThree[index];
		topThree[index]->views++;
		topThree[index]->countPopularity();
		savePostJSON();
		generateTopThree();
	}
	void closeAPost()
	{
		currentPost = nullptr;
	}
	void loadPostJSON()
	{
		json postJson;
		postJson.clear();
		collectionOfAllPost.clear();
		topThree.clear();
		fstream file("save_data/"+boardTitle + "Post.json", ios::in);
		if (!file)
		{
			savePostJSON();
		}
		else
		{
			file >> postJson;
			file.close();
			int currentIndex = 0;
			for (auto iterator = postJson.rbegin(); iterator != postJson.rend(); iterator++)
			{
				Post temporaryPost;
				temporaryPost.postId = iterator.key();
				for (auto innerIterator = iterator->begin(); innerIterator != iterator->end(); innerIterator++)
				{
					if (innerIterator.key() == "postContent")
					{
						temporaryPost.postContent = innerIterator.value();
					}
					else if (innerIterator.key() == "postTitle")
					{
						temporaryPost.postTitle = innerIterator.value();
					}
					else if (innerIterator.key() == "posterId")
					{
						temporaryPost.posterId = innerIterator.value();
					}
					else if (innerIterator.key() == "views")
					{
						temporaryPost.views = innerIterator.value();
					}
					else if (innerIterator.key() == "popularityPoints")
					{
						temporaryPost.popularityPoints = innerIterator.value();
					}
					else if (innerIterator.key() == "tuiwenUsers")
					{
						temporaryPost.tuiwenUsers.clear();
						for (auto insideIterator = innerIterator->begin(); insideIterator != innerIterator->end(); insideIterator++)
						{
							temporaryPost.tuiwenUsers.push_back(insideIterator.value());
						}
					}
					else if (innerIterator.key() == "xuwenUsers")
					{
						temporaryPost.xuwenUsers.clear();
						for (auto insideIterator = innerIterator->begin(); insideIterator != innerIterator->end(); insideIterator++)
						{
							temporaryPost.xuwenUsers.push_back(insideIterator.value());
						}
					}
					else if (innerIterator.key() == "userComment")
					{
						temporaryPost.userComment.clear();
						for (auto insideIterator = innerIterator->begin(); insideIterator != innerIterator->end(); insideIterator++)
						{
							Comment tempComment;
							tempComment.commentId = insideIterator.key();
							for (auto anotherIterator = insideIterator->begin(); anotherIterator != insideIterator->end(); anotherIterator++)
							{
								if (anotherIterator.key() == "commentType")
								{
									tempComment.commentType = anotherIterator.value();
								}
								else if (anotherIterator.key() == "comment")
								{
									tempComment.comment = anotherIterator.value();
								}
								else if (anotherIterator.key() == "userId")
								{
									tempComment.userId = anotherIterator.value();
								}
							}
							temporaryPost.userComment.push_back(tempComment);
						}
					}
				}
				temporaryPost.countPopularity();
				collectionOfAllPost.push_back(temporaryPost);
				currentIndex++;
			}
			generateTopThree();
		}
		
	}
	void savePostJSON()
	{
		json postJson;
		postJson.clear();
		for (auto iterator = collectionOfAllPost.begin(); iterator != collectionOfAllPost.end(); iterator++)
		{
			json everyThingExceptPostId;
			everyThingExceptPostId["postTitle"] = iterator->postTitle;
			everyThingExceptPostId["postContent"] = iterator->postContent;
			everyThingExceptPostId["posterId"] = iterator->posterId;
			everyThingExceptPostId["views"] = iterator->views;
			everyThingExceptPostId["popularityPoints"] = iterator->popularityPoints;
			json allComment;
			everyThingExceptPostId["tuiwenUsers"] = iterator->tuiwenUsers;
			everyThingExceptPostId["xuwenUsers"] = iterator->xuwenUsers;
			//do the comment
			for (auto innerIterator = iterator->userComment.begin(); innerIterator != iterator->userComment.end(); innerIterator++)
			{
				json innerComment;
				innerComment["comment"] = innerIterator->comment;
				innerComment["userId"] = innerIterator->userId;
				innerComment["commentType"] = innerIterator->commentType;
				allComment[innerIterator->commentId] = innerComment;
			}
			//arrange everything
			everyThingExceptPostId["userComment"] = allComment;
			postJson[iterator->postId] = everyThingExceptPostId;
		}
		fstream file("save_data/" + boardTitle + "Post.json", ios::out);
		file << postJson;
		file.close();
	}
	static void loadAllTitleJSON()
	{
		fstream file("save_data/ALLTITLE.json", ios::in);
		json jsonRep;
		file >> jsonRep;
		allTitleXBoardMakerId.clear();
		for (auto iterator = jsonRep.begin(); iterator != jsonRep.end(); iterator++)
		{
			allTitleXBoardMakerId.push_back(make_pair(iterator.key(), iterator.value()));
		}
	}
	static void saveAllTitleJSON()
	{
		fstream file("save_data/ALLTITLE.json", ios::out);
		json jsonRep;
		for (auto iterator = allTitleXBoardMakerId.begin(); iterator != allTitleXBoardMakerId.end(); iterator++)
		{
			jsonRep[iterator->first] = iterator->second;
		}
		file << jsonRep; 
	}
	void generateTopThree()
	{
		topThree.clear();
		for (auto iterator = collectionOfAllPost.begin(); iterator != collectionOfAllPost.end(); iterator++)
		{
			if (topThree.size() < 3)
			{
				Post* postAddress = &(*iterator);
				topThree.push_back(postAddress);
			}
			else
			{

				if (iterator->popularityPoints > topThree[2]->popularityPoints)
				{
					Post* postAddress = &(*iterator);
					topThree[2] = postAddress;
				}
				else if (iterator->popularityPoints == topThree[2]->popularityPoints)
				{
					if (iterator->postId > topThree[2]->postId)
					{
						Post* postAddress = &(*iterator);
						topThree[2] = postAddress;
					}
				}
			}

			//do sort everytime at the end of the topThree append
			for (int iteration = 0; iteration < topThree.size() - 1; iteration++)
			{
				for (int index = iteration; index < topThree.size() - 1; index++)
				{
					if (topThree[index]->popularityPoints < topThree[index + 1]->popularityPoints)
					{
						swap(topThree[index], topThree[index + 1]);
					}
				}
			}
		}
		
	}
	void draw(); //for drawing the board state
	void createPost(unsigned long long userId);
};
