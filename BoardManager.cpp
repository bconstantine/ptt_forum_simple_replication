#include "BoardManager.h"

void BoardManager::startLoop()
{
	bool firstChat = true;
	loadAll();
	while (!this->isExit)
	{
		system("cls");
		saveAll();
		if (this->currentState == BoardState::MENU)
		{
			firstChat = true;

			Viewer::displayMenu(accessType, username, usernameTyped, password, passwordTyped);
			if (usernameTyped && passwordTyped)
			{
				if (accessType == "signin")
				{
					if (loginChecker())
					{
						goToBoard(BoardState::SELECT_BOARD);
						Viewer::showLoading();
					}
					else
					{
						accessType = "";
						username = "";
						usernameTyped = false;
						password = "";
						passwordTyped = false;
						system("cls");
						Viewer::colorText(L_RED, L_WHITE, "                  \n");
						Viewer::colorText(L_RED, L_WHITE, " User not Found!  \n");
						Viewer::colorText(L_RED, L_WHITE, " Please try again \n");
						Viewer::colorText(L_RED, L_WHITE, "                  \n");
						system("pause");
					}
				}
				else if(accessType == "signup")
				{
					if (usernameTyped && passwordTyped)
					{
						cout << endl;
						Viewer::colorText(L_GREEN, S_WHITE, "Create new Account? [Y/n]: ");
						string ans;
						getline(cin, ans);
						if (ans == "Y")
						{
							if (checkForDuplicateUsername())
							{
								createNewUser();
								saveUserJSON();
								goToBoard(BoardState::SELECT_BOARD);
								Viewer::showLoading();
							}
							else
							{
								accessType = "";
								username = "";
								usernameTyped = false;
								password = "";
								passwordTyped = false;
								system("cls");
								Viewer::colorText(L_RED, L_WHITE, "                                          \n");
								Viewer::colorText(L_RED, L_WHITE, " Username is used, please use another one \n");
								Viewer::colorText(L_RED, L_WHITE, "                                          \n");
								system("pause");
							}
						}
						else
						{
							accessType = "";
							username = "";
							usernameTyped = false;
							password = "";
							passwordTyped = false;
						}
					}
					
				}
				else if (accessType == "guest")
				{
					guessLogin();
					goToBoard(BoardState::SELECT_BOARD);
					Viewer::showLoading();
				}
				else if (accessType == "createAdmin")
				{
					if (usernameTyped && passwordTyped)
					{
						cout << endl;
						Viewer::colorText(L_GREEN, S_WHITE, "Create new Account? [Y/n]: ");
						string ans;
						getline(cin, ans);
						if (ans == "Y")
						{
							if (checkForDuplicateUsername())
							{
								createNewAdmin();
								saveUserJSON();
								goToBoard(BoardState::SELECT_BOARD);
								Viewer::showLoading();
							}
							else
							{
								accessType = "";
								username = "";
								usernameTyped = false;
								password = "";
								passwordTyped = false;
								system("cls");
								Viewer::colorText(L_RED, L_WHITE, "                                          \n");
								Viewer::colorText(L_RED, L_WHITE, " Username is used, please use another one \n");
								Viewer::colorText(L_RED, L_WHITE, "                                          \n");
								system("pause");
							}
						}
						else
						{
							accessType = "";
							username = "";
							usernameTyped = false;
							password = "";
							passwordTyped = false;
						}
					}
				}
			}
		}
		else
		{
			// ACCESS PTT
			if (this->currentState == BoardState::SELECT_BOARD)
			{

				bool isSignedOut = false;
				PageSelection chosenPage;
				Viewer::displaySelectBoard(currentUser, chosenPage);
				string ans;
				switch (chosenPage)
				{
				case PageSelection::BOARDS:
					goToBoard(BoardState::BOARD);
					break;
				case PageSelection::CHATS:
					if (currentUser->adminLevel == 2) // guest cannot access
					{
						system("cls");
						cout << " Sign up to see more!" << endl;
						system("pause");
					}
					else
					{
						goToBoard(BoardState::CHAT_PAGE);
					}
					break;
				case PageSelection::GAMES:
					goToBoard(BoardState::GAME_PAGE);
					break;
				case PageSelection::NOTIFICATIONS:
					if (currentUser->adminLevel == 2) // guest cannot access
					{
						system("cls");
						cout << " Sign up to see more!" << endl;
						system("pause");
					}
					else
					{
						goToBoard(BoardState::NOTIFICATION);
					}
					break;
				case PageSelection::EXIT:
					cout << endl;
					Viewer::colorText(L_RED, L_WHITE, " Do you really want to leave? [Y/n]");
					cout << ": ";
					cin >> ans;
					if (ans[0] == 'Y')
					{
						accessType = ""; //not save
						username = ""; //not save
						usernameTyped = false; //not save
						password = ""; //not save
						passwordTyped = false; //not save
						goToBoard(BoardState::MENU);
					}
					break;
				}
			}
			else if (this->currentState == BoardState::BOARD)
			{
				bool isBack = false;
				string cmd = "";
				int chosenBoard = -1;
				Viewer::displayBoard(currentUser, isBack, cmd, chosenBoard);

				if (!cmd.empty())
				{
					stringstream ss(cmd);
					string inputCommand;
					ss >> inputCommand;
					if (inputCommand == "add")
					{
						string newBoard = "";
						string tmp;
						ss >> newBoard;
						while (ss >> tmp)
						{
							newBoard += (" " + tmp);
						}
						if (!newBoard.empty() && newBoard.size() <=45)
						{
							addNewBoard(newBoard);
						}
						else if (newBoard.length() > 45)
						{
							Viewer::colorText(L_RED, L_WHITE, "\n Title too long(must <45)!");
							Sleep(1000);
						}
							
					}
					else if (inputCommand == "delete")
					{
						string boardName = "";
						string tmp;
						ss >> boardName;
						while (ss >> tmp)
						{
							boardName += (" " + tmp);
						}
						if (1) // if found
						{
							COORD c;

							c.X = 0;
							c.Y = 29;

							SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

							int spacingEnd = 63;
							Viewer::setColor(L_RED, S_WHITE);
							cout << " Do you want to delete this board permanently? [Y/n]: ";
							Viewer::resetColor();

							Viewer::setColor(L_WHITE, L_BLACK);
							for (int i = 0; i < spacingEnd; i++)
							{
								cout << " ";
							}

							c.X = 55;
							c.Y = 29;
							SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

							string input = "";
							char inputChar = _getch();
							bool first = false;
							while (inputChar != '\r')
							{
								if (inputChar == '\b')
								{
									if (input.length() > 0)
									{
										input.pop_back();
										cout << "\b \b";
									}
									else
									{
										// do nothing
									}
								}
								else
								{
									if (!first)
									{
										first = !first;
									}
									else
									{
										input += inputChar;
									}
								}

								c.X = 55;
								c.Y = 29;
								SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

								cout << input;

								inputChar = _getch();
							}
							Viewer::resetColor();

							if (input == "Y")
							{
								// delete this board
								if (deleteBoard(boardName))
								{
									Viewer::colorText(L_WHITE, L_BLACK, "  ");
									Viewer::setColor(L_GREEN, L_WHITE);
									cout << boardName << " deleted";
									Viewer::resetColor();
									Sleep(1000);
								}
								else
								{
									Viewer::colorText(L_WHITE, L_BLACK, "  ");
									Viewer::setColor(L_RED, L_WHITE);
									cout << "Board not Found!";
									Viewer::resetColor();
									Sleep(1000);
								}
							}
						}
					}
					cmd = "";
				}

				if (isBack)
				{
					back();
					continue;
				}
				
				if (chosenBoard != -1)
				{
					currentBoard = &collectionOfAllBoard[chosenBoard];
					goToBoard(BoardState::POST_LIST);
					chosenBoard = -1;
					continue;
				}
			}
			else if (this->currentState == BoardState::POST_LIST)
			{
				bool isBack = false;
				string cmd = "";
				int chosenPost = -1;
				Viewer::displayPostList(currentUser, currentBoard, isBack, chosenPost, cmd);

				string content = "";
				if (!cmd.empty())
				{
					stringstream ss(cmd);
					string inputCommand;
					ss >> inputCommand;
					if (inputCommand == "add")
					{
						string newPost = "";
						string tmp;
						ss >> newPost;
						while (ss >> tmp)
						{
							newPost += (" " + tmp);
						}

						if (!newPost.empty() && newPost.length() < 45)
						{
							system("cls");
							// headbar
							Viewer::colorText(
								L_YELLOW, 
								L_WHITE, 
								" (esc+A) Discard                                                                                        Proceed (esc+D) "
							);
							cout << endl;
							Viewer::resetColor();
							cout << "Title: " << newPost << endl << endl;
							// content
							cout << "What do you want to write?" << endl;
							char inputChar = '.';
							content = "";
							while (cmd != "")
							{
								inputChar = _getch();

								if (inputChar == -32)
								{
									char trash = _getch();
									continue;
								}

								COORD c;
								c.X = 0;
								c.Y = 4;
								SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

								for (int i = 0; i < content.length(); i++)
								{
									if (content[i] == '\n')
									{
										cout << "\n";
									}
									else
									{
										cout << " ";
									}
								}

								c.X = 0;
								c.Y = 4;
								SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

								if (inputChar == '\r')
								{
									content += "\n";
								}
								else if (inputChar == 8) // backspace
								{
									if (content.length() > 0)
									{
										content.pop_back();
									}
									else
									{
										// do nothing
									}
								}
								else
								{
									if (inputChar != '\033')
									{
										content.push_back(inputChar);
									}
									
								}
								cout << content;

								int spacingEnd = 79;
								char inChar;
								string cmds = "";
								COORD x;
								if (inputChar == '\033')
								{
									switch (_getch())
									{
									case 'a':
									case 'A': // cancel

										x.X = 0;
										x.Y = 29;

										SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), x);

										Viewer::setColor(L_RED, S_WHITE);
										cout << " Do you want to discard your post? [Y/n] ";
										Viewer::resetColor();

										Viewer::setColor(L_WHITE, L_BLACK);
										for (int i = 0; i < spacingEnd; i++)
										{
											cout << " ";
										}

										x.X = 42;
										x.Y = 29;
										SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), x);

										cmds = "";
										inChar = _getch();
										while (inChar != '\r')
										{
											if (inChar == '\b')
											{
												if (cmds.length() > 0)
												{
													cmds.pop_back();
													cout << "\b \b";
												}
												else
												{
													// do nothing
												}
											}
											else
											{
												cmds += inChar;
											}

											x.X = 42;
											x.Y = 29;
											SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), x);

											cout << cmds;

											inChar = _getch();
										}
										Viewer::resetColor();
										if (cmds == "Y")
										{
											system("cls");
											cmd = "";
										}
										else
										{
											x.X = 0;
											x.Y = 29;
											SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), x);
											for (int i = 0; i < 120; i++)
											{
												cout << " ";
											}
											content.pop_back();
										}
										break;
									case 'd':
									case 'D': // done

										x.X = 0;
										x.Y = 29;

										SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), x);

										Viewer::setColor(L_GREEN, S_WHITE);
										cout << " Do you want to save this post? [Y/n] ";
										Viewer::resetColor();

										Viewer::setColor(L_WHITE, L_BLACK);
										for (int i = 0; i < spacingEnd; i++)
										{
											cout << " ";
										}

										x.X = 39;
										x.Y = 29;
										SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), x);

										cmds = "";
										inChar = _getch();
										while (inChar != '\r')
										{
											if (inChar == '\b')
											{
												if (cmds.length() > 0)
												{
													cmds.pop_back();
													cout << "\b \b";
												}
												else
												{
													// do nothing
												}
											}
											else
											{
												cmds += inChar;
											}

											x.X = 39;
											x.Y = 29;
											SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), x);

											cout << cmds;

											inChar = _getch();
										}
										Viewer::resetColor();
										if (cmds == "Y")
										{
											system("cls");
											addPost(newPost, content);
											cmd = "";
										}
										else
										{
											x.X = 0;
											x.Y = 29;
											SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), x);
											for (int i = 0; i < 120; i++)
											{
												cout << " ";
											}
											content.pop_back();
										}
										break;
									}
								}
								

							}
						}
						else if (newPost.length() > 45)
						{
							Viewer::colorText(L_RED, L_WHITE, "\n Title too long(must <45)!");
							Sleep(1000);
						}
					}
				}

				if (isBack)
				{
					back();
					continue;
				}

				if (chosenPost != -1)
				{
					if (chosenPost < currentBoard->topThree.size()) // topthree
					{
						currentBoard->openAPostFromTopThree(chosenPost);
					}
					else
					{
						currentBoard->openAPost(chosenPost - currentBoard->topThree.size());
					}
					goToBoard(BoardState::POST);
					chosenPost = -1;
					continue;
				}
			}
			else if (this->currentState == BoardState::POST)
			{
				bool isBack = false;
				string comment = "";
				bool deleting = false;
				int deleteIndex = -1;
				bool deletingPost = false;
				bool editingPost = false;
				Viewer::displayPost(currentUser, currentBoard, isBack, comment, deleting, deleteIndex, deletingPost, editingPost);
				
				if (!comment.empty())
				{
					stringstream ss(comment);
					string tmpStatus;
					string getComment = "";
					ss >> tmpStatus;
					if (tmpStatus != "0" && tmpStatus != "1" && tmpStatus != "2")
					{
						Viewer::colorText(L_RED, L_WHITE, "\n Error comment syntax!");
						Sleep(1000);
					}
					else
					{
						ss >> getComment;
						string tmp;
						while (ss >> tmp)
						{
							getComment += (" " + tmp);
						}
						if (!addComment(stoi(tmpStatus), getComment)) {
							Viewer::colorText(L_RED, L_WHITE, "\n You can't contradict/chosen the same comment as your previous one!");
							Sleep(2000);
						}
					}
				}

				if (deleting)
				{
					if (!checkCommentSameUser(deleteIndex))
					{
						if (currentUser->adminLevel == 0) // is admin?
						{
							string recCmd = "";
							cout << "\n Why do you want to delete this comment? <";
							Viewer::colorText(L_BLACK, L_RED, "0");
							cout << ": INAPROPRIATE/ ";
							Viewer::colorText(L_BLACK, L_RED, "1");
							cout << ": SPAM/ ";
							Viewer::colorText(L_BLACK, L_RED, "2");
							cout << ": CANCEL>: ";
							cin >> recCmd;
							if (recCmd != "0" && recCmd != "1" && recCmd != "2")
							{
								Viewer::colorText(L_RED, L_WHITE, "\n Wrong input, please try again! (enter to continue)");
								Sleep(2000);
							}
							else 
							{
								if (recCmd == "0")
								{
									deleteComment(deleteIndex, DeleteReason::INAPPROPRIATE);
									Viewer::colorText(L_GREEN, L_WHITE, "\n Comment Deleted (enter to continue)");
									Sleep(2000);
								}
								else if (recCmd == "1")
								{
									deleteComment(deleteIndex, DeleteReason::SPAM);
									Viewer::colorText(L_GREEN, L_WHITE, "\n Comment Deleted (enter to continue)");
									Sleep(2000);
								}
								else if (recCmd == "2")
								{
									// do nothing
									Viewer::colorText(L_GREEN, L_WHITE, "\n Comment Deleted Canceled (enter to continue)");
									Sleep(2000);
								}
							}

						}
						else
						{
							Viewer::colorText(L_RED, L_WHITE, "\n This comment doesn't belongs to you. (enter to continue)");
							Sleep(2000);
						}
					}
					else
					{
						if (!deleteComment(deleteIndex, DeleteReason::USEROWNDELETE)) 
						{
							Viewer::colorText(L_RED, L_WHITE, "\n This comment doesn't belongs to you. (enter to continue)");
							Sleep(2000);
						}
						else
						{
							Viewer::colorText(L_GREEN, L_WHITE, "\n Comment Deleted (enter to continue)");
							Sleep(2000);
						}
					}
					deleting = false;
					deleteIndex = -1;
					continue;
				}
				else if (deletingPost)
				{
					bool deleteSucceed = false;
					if (currentUser->adminLevel == 0)
					{
						if (checkPostSameUser())
						{
							if (!deletePost(DeleteReason::USEROWNDELETE))
							{
								Viewer::colorText(L_RED, L_WHITE, "\n This Post doesn't belongs to you. (enter to continue)");
								Sleep(2000);
							}
							else
							{
								deleteSucceed = true;
								Viewer::colorText(L_GREEN, L_WHITE, "\n Post Deleted (enter to continue)");
								Sleep(2000);
							}
						}
						else
						{
							string recCmd = "";
							cout << "\n Why do you want to delete this Post? <";
							Viewer::colorText(L_BLACK, L_RED, "0");
							cout << ": INAPROPRIATE/ ";
							Viewer::colorText(L_BLACK, L_RED, "1");
							cout << ": SPAM/ ";
							Viewer::colorText(L_BLACK, L_RED, "2");
							cout << ": CANCEL>: ";
							cin >> recCmd;
							if (recCmd != "0" && recCmd != "1" && recCmd != "2")
							{
								Viewer::colorText(L_RED, L_WHITE, "\n Wrong input, please try again! (enter to continue)");
								Sleep(2000);
							}
							else
							{
								if (recCmd == "0")
								{
									deletePost(DeleteReason::INAPPROPRIATE);
									deleteSucceed = true;
									Viewer::colorText(L_GREEN, L_WHITE, "\n Post Deleted (enter to continue)");
									Sleep(2000);
								}
								else if (recCmd == "1")
								{
									deletePost(DeleteReason::SPAM);
									deleteSucceed = true;
									Viewer::colorText(L_GREEN, L_WHITE, "\n Post Deleted (enter to continue)");
									Sleep(2000);
								}
								else if (recCmd == "2")
								{
									// do nothing
									Viewer::colorText(L_GREEN, L_WHITE, "\n Post Deleted Canceled (enter to continue)");
									Sleep(2000);
								}
							}
						}
					}
					else
					{
						if (!deletePost(DeleteReason::USEROWNDELETE))
						{
							Viewer::colorText(L_RED, L_WHITE, "\n This Post doesn't belongs to you. (enter to continue)");
							Sleep(2000);
						}
						else
						{
							Viewer::colorText(L_GREEN, L_WHITE, "\n Post Deleted (enter to continue)");
							deleteSucceed = true;
							Sleep(2000);
						}
					}
					deletingPost = false;
					if (deleteSucceed)
					{
						goToBoard(BoardState::POST_LIST);
					}
					continue;
				}
				else if (editingPost)
				{
					string content = currentBoard->currentPost->postContent;
					system("cls");
					// headbar
					Viewer::colorText(
						L_YELLOW,
						L_WHITE,
						" (esc+A) Discard                                                                                        Proceed (esc+D) "
					);
					cout << endl;
					Viewer::resetColor();
					cout << "Title: " << currentBoard->currentPost->postTitle << endl << endl;
					// content
					cout << "What do you want to write?" << endl;
					string cmd = "Currently editing";
					char inputChar = '.';
					bool first = true;
					bool first1 = true;
					while (cmd != "")
					{
						if (first)
						{
							first = !first;
						}
						else
						{
							inputChar = _getch();
						}
						if (inputChar == -32)
						{
							char trash = _getch();
							continue;
						}

						COORD c;
						c.X = 0;
						c.Y = 4;
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

						for (int i = 0; i < content.length(); i++)
						{
							if (content[i] == '\n')
							{
								cout << "\n";
							}
							else
							{
								cout << " ";
							}
						}

						c.X = 0;
						c.Y = 4;
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

						if (inputChar == '\r')
						{
							content += "\n";
						}
						else if (inputChar == 8) // backspace
						{
							if (content.length() > 0)
							{
								content.pop_back();
							}
							else
							{
								// do nothing
							}
						}
						else
						{
							if (inputChar != '\033')
							{
								if (first1)
								{
									first1 = !first1;
								}
								else
								{
									content.push_back(inputChar);
								}
							}

						}
						cout << content;

						int spacingEnd = 79;
						char inChar;
						string cmds = "";
						COORD x;
						if (inputChar == '\033')
						{
							switch (_getch())
							{
							case 'a':
							case 'A': // cancel

								x.X = 0;
								x.Y = 29;

								SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), x);

								Viewer::setColor(L_RED, S_WHITE);
								cout << " Do you want to discard your post? [Y/n] ";
								Viewer::resetColor();

								Viewer::setColor(L_WHITE, L_BLACK);
								for (int i = 0; i < spacingEnd; i++)
								{
									cout << " ";
								}

								x.X = 42;
								x.Y = 29;
								SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), x);

								cmds = "";
								inChar = _getch();
								while (inChar != '\r')
								{
									if (inChar == '\b')
									{
										if (cmds.length() > 0)
										{
											cmds.pop_back();
											cout << "\b \b";
										}
										else
										{
											// do nothing
										}
									}
									else
									{
										cmds += inChar;
									}

									x.X = 42;
									x.Y = 29;
									SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), x);

									cout << cmds;

									inChar = _getch();
								}
								Viewer::resetColor();
								if (cmds == "Y")
								{
									system("cls");
									cmd = "";
								}
								else
								{
									x.X = 0;
									x.Y = 29;
									SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), x);
									for (int i = 0; i < 120; i++)
									{
										cout << " ";
									}
									content.pop_back();
								}
								break;
							case 'd':
							case 'D': // done

								x.X = 0;
								x.Y = 29;

								SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), x);

								Viewer::setColor(L_GREEN, S_WHITE);
								cout << " Do you want to save this post? [Y/n] ";
								Viewer::resetColor();

								Viewer::setColor(L_WHITE, L_BLACK);
								for (int i = 0; i < spacingEnd; i++)
								{
									cout << " ";
								}

								x.X = 39;
								x.Y = 29;
								SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), x);

								cmds = "";
								inChar = _getch();
								while (inChar != '\r')
								{
									if (inChar == '\b')
									{
										if (cmds.length() > 0)
										{
											cmds.pop_back();
											cout << "\b \b";
										}
										else
										{
											// do nothing
										}
									}
									else
									{
										cmds += inChar;
									}

									x.X = 39;
									x.Y = 29;
									SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), x);

									cout << cmds;

									inChar = _getch();
								}
								Viewer::resetColor();
								if (cmds == "Y")
								{
									system("cls");
									
									editPostContent(content);
									cmd = "";
								}
								else
								{
									x.X = 0;
									x.Y = 29;
									SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), x);
									for (int i = 0; i < 120; i++)
									{
										cout << " ";
									}
									content.pop_back();
								}
								break;
							}
						}
					}
						
				}

				if (isBack)
				{
					back();
					continue;
				}
			}
			else if (this->currentState == BoardState::NOTIFICATION) // notif
			{
				bool isBack = false;
				Viewer::displayNotification(currentUser, isBack);
				if (isBack)
				{
					back();
					continue;
				}
			}
			else if (this->currentState == BoardState::CHAT_PAGE)
			{
				User* user2 = NULL;
				string userTarget = "";
				bool isBack = false;
				if (firstChat)
				{
					firstChat = !firstChat;
					//cin.ignore();
				}
				Viewer::displayChatPage(isBack, userTarget);	

				if (!userTarget.empty())
				{
					if (userTarget == currentUser->userName)
					{
						cout << endl;
						Viewer::colorText(L_RED, L_WHITE, " You cannot chat with yourself!");
						cout << endl;
						system("pause");
						continue;
					}
					else
					{
						if (!userNameSearchForUser(user2, userTarget))
						{
							cout << endl;
							Viewer::colorText(L_RED, L_WHITE, " Username not Found!");
							cout << endl;
							system("pause");
							continue;
						}
						else
						{
							Viewer::colorText(L_GREEN, L_WHITE, " User Found! Proceeding...\n");
							system("pause");
						}
					}
				}
				else
				{
					back();
					continue;
				}

				Viewer::displayChat(isBack, currentUser, user2);
				if (isBack)
				{
					back();
					continue;
				}
			}
			else if (this->currentState == BoardState::GAME_PAGE)
			{
				bool isBack = false;
				Viewer::displayDungeon(isBack);
				if (isBack)
				{
					back();
					continue;
				}
			}
			else
			{
				cout << "Page Not Found" << endl;
				system("pause");
				this->currentState = BoardState::SELECT_BOARD;
			}
		}
	}
}

void BoardManager::goToBoard(BoardState scrn_Dest)
{
	currentState = scrn_Dest;
}

void BoardManager::back()
{
	switch (currentState)
	{
	case BoardState::BOARD:
	case BoardState::POST_LIST:
	case BoardState::POST:
	case BoardState::USER_POSTS:
		currentState = BoardState(currentState - 1);
		break;
	case BoardState::GAME_PAGE:
	case BoardState::CHAT_PAGE:
	case BoardState::NOTIFICATION:
		currentState = BoardState::SELECT_BOARD;
		break;
	default:
		break;
	}
}