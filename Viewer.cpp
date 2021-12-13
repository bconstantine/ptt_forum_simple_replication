#include "Viewer.h"

vector<Admin> Viewer::fakeAdmins;
vector<Guest> Viewer::fakeGuests;
vector<Member> Viewer::fakeMembers;
vector<string> Viewer::fakeUsername;

bool is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

void Viewer::printGreetings(string username)
{
	int spaceLength = 118 - 4 - username.length();
	for (int i = 0; i < spaceLength; i++)
	{
		cout << " ";
	}
	cout << "Hi, " << username << endl;
}

void Viewer::printHeader(string title)
{
	int spaceLeft = 58;
	int spaceRight = 57;
	for (int i = 0; i < title.length(); i++)
	{
		(i % 2 == 0) ? spaceLeft-- : spaceRight--;
	}
	cout << " ╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗ " << endl;
	cout << " ║                                                                                                                   ║ " << endl;
	cout << " ║";
	for (int i = 0; i < spaceLeft; i++)
	{
		cout << " ";
	}
	cout << title;
	for (int i = 0; i < spaceRight; i++)
	{
		cout << " ";
	}
	cout << "║ " << endl;
	cout << " ║                                                                                                                   ║ " << endl;
	cout << " ╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝ " << endl;

}

void Viewer::printCenter(string text)
{
	int spaceLeft = 58;
	int spaceRight = 57;
	for (int i = 0; i < text.length(); i++)
	{
		(i % 2 == 0) ? spaceLeft-- : spaceRight--;
	}

	cout << "  ";
	for (int i = 0; i < spaceLeft; i++)
	{
		cout << " ";
	}
	cout << text;
	for (int i = 0; i < spaceRight; i++)
	{
		cout << " ";
	}
}

void Viewer::displayMenu(string& accessType, string& username, bool& usernameTyped, string& password, bool& passwordTyped)
{
	while (!(usernameTyped && passwordTyped))
	{
		// print Menu page
		system("cls");
		cout << " ╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗ " << endl;
		cout << " ║                                                                                                                   ║ " << endl;
		cout << " ║ 	 ███████████  █████   ███   █████                                                                            ║ " << endl;
		cout << " ║ 	░░███░░░░░███░░███   ░███  ░░███                                                                             ║ " << endl;
		cout << " ║ 	 ░███    ░███ ░███   ░███   ░███                                                                             ║ " << endl;
		cout << " ║ 	 ░██████████  ░███   ░███   ░███                                                                             ║ " << endl;
		cout << " ║ 	 ░███░░░░░███ ░░███  █████  ███   ░▀█▀░█▀█░█░░░█░█░▀█▀░▀█▀░█▄█░█▀▀                                           ║ " << endl;
		cout << " ║ 	 ░███    ░███  ░░░█████░█████░    ░░█░░█▀█░█░░░█▀▄░░█░░░█░░█░█░█▀▀                                           ║ " << endl;
		cout << " ║ 	░███████████     ░░███ ░░███      ░░▀░░▀░▀░▀▀▀░▀░▀░░▀░░▀▀▀░▀░▀░▀▀▀                                           ║ " << endl;
		cout << " ║ 	 ░░░░░░░░░░░       ░░░   ░░░                                                                                 ║ " << endl;
		cout << " ║                                                                                                                   ║ " << endl;
		cout << " ╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝ " << endl;
		cout << endl << " Welcome to BW TALKTIME!" << endl;
		cout << " Please type [";
		setColor(L_BLACK, S_YELLOW);
		cout << "signin";
		resetColor();
		cout << "] to sign in, or [";
		setColor(L_BLACK, S_YELLOW);
		cout << "signup";
		resetColor();
		cout << "] to sign up, or [";
		setColor(L_BLACK, S_YELLOW);
		cout << "guest";
		resetColor();
		cout << "] as guest : ";

		if (accessType == "")
		{
			getline(cin, accessType);
			if (!(accessType == "signin" || accessType == "signup" || accessType == "guest" || accessType == "createAdmin"))
			{				
				accessType = "";
			}
		}
		else
		{
			setColor(S_WHITE, L_BLACK);
			cout << accessType << endl;
			resetColor();

			if (accessType == "signin")
			{
				cout << " Username: ";
				if (!usernameTyped)
				{
					cout << username;
					char inputChar = _getch();
					if (inputChar != '\r')
					{
						if (inputChar == '\b')
						{
							if (username.length() > 0)
							{
								username.pop_back();
							}
							else
							{
								// do nothing
							}
						}
						else
						{
							username += inputChar;
						}
						continue;
					}


					if (!User::validUserName(username))
					{
						username = ""; // reset username
					}
					else
					{
						usernameTyped = true;
					}
					continue;
				}
				else
				{
					colorText(S_WHITE, L_BLACK, username);
					cout << endl;
				}

				cout << " Password: ";
				if (!passwordTyped)
				{
					for (int i = 0; i < password.length(); i++)
					{
						cout << "*";
					}
					char inputChar = _getch();
					if (inputChar != '\r')
					{
						if ((int)inputChar == 8)
						{
							if (password.length() > 0)
							{
								password.pop_back();
							}
							else
							{
								// do nothing
							}
						}
						else
						{
							password += inputChar;
						}
						continue;
					}

					if (!User::validPassword(password))
					{
						password = ""; // reset password
					}
					else
					{
						passwordTyped = true;
					}
					continue;
				}
				else
				{
					for (int i = 0; i < password.length(); i++)
					{
						colorText(S_WHITE, L_BLACK, "*");
					}
					cout << endl;
				}
			}
			else if (accessType == "signup")
			{
				if (!usernameTyped)
				{
					cout << "  ";
					colorText(L_WHITE, L_BLACK, "          RULES           ");
					cout << endl;
					colorText(L_WHITE, L_BLACK, " 1. Length should be 8-12   ");
					cout << endl;
					colorText(L_WHITE, L_BLACK, " 2. Alphanumeric only       ");
					cout << endl;
					colorText(L_WHITE, L_BLACK, " 3. May contain underscore  ");
					cout << endl;
					colorText(L_WHITE, L_BLACK, "                          ");
					cout << endl;
				}
				cout << " Username: ";
				if (!usernameTyped)
				{
					cout << username;
					char inputChar = _getch();
					if (inputChar != '\r')
					{
						if ((int)inputChar == 8)
						{
							if (username.length() > 0)
							{
								username.pop_back();
							}
							else
							{
								// do nothing
							}
						}
						else
						{
							username += inputChar;
						}
						continue;
					}


					if (!User::validUserName(username))
					{
						username = ""; // reset username
					}
					else
					{
						usernameTyped = true;
					}
					continue;
				}
				else
				{
					colorText(S_WHITE, L_BLACK, username);
					cout << endl;
				}

				if (!passwordTyped)
				{
					cout << "  ";
					colorText(L_WHITE, L_BLACK, "           RULES            ");
					cout << endl;
					colorText(L_WHITE, L_BLACK, " 1. Length should more than 8 ");
					cout << endl;
					colorText(L_WHITE, L_BLACK, " 2. Alphanumeric only         ");
					cout << endl;
					colorText(L_WHITE, L_BLACK, " 3. May contain underscore    ");
					cout << endl;
					colorText(L_WHITE, L_BLACK, "                            ");
					cout << endl;
				}
				cout << " Password: ";
				if (!passwordTyped)
				{
					cout << password;
					char inputChar = _getch();
					if (inputChar != '\r')
					{
						if ((int)inputChar == 8)
						{
							if (password.length() > 0)
							{
								password.pop_back();
							}
							else
							{
								// do nothing
							}
						}
						else
						{
							password += inputChar;
						}
						continue;
					}

					if (!User::validPassword(password))
					{
						password = ""; // reset password
					}
					else
					{
						passwordTyped = true;
					}
					continue;
				}
				else
				{
					colorText(S_WHITE, L_BLACK, password);
					cout << endl;
				}
			}
			else if (accessType == "guest")
			{
				username = "__guest__";
				usernameTyped = true;
				passwordTyped = true;
			}
			else if (accessType == "createAdmin")
			{
				if (!usernameTyped)
				{
					cout << "  ";
					colorText(L_WHITE, L_BLACK, "          RULES           ");
					cout << endl;
					colorText(L_WHITE, L_BLACK, " 1. Length should be 8-12   ");
					cout << endl;
					colorText(L_WHITE, L_BLACK, " 2. Alphanumeric only       ");
					cout << endl;
					colorText(L_WHITE, L_BLACK, " 3. May contain underscore  ");
					cout << endl;
					colorText(L_WHITE, L_BLACK, "                          ");
					cout << endl;
				}
				cout << " Username: ";
				if (!usernameTyped)
				{
					cout << username;
					char inputChar = _getch();
					if (inputChar != '\r')
					{
						if ((int)inputChar == 8)
						{
							if (username.length() > 0)
							{
								username.pop_back();
							}
							else
							{
								// do nothing
							}
						}
						else
						{
							username += inputChar;
						}
						continue;
					}


					if (!User::validUserName(username))
					{
						username = ""; // reset username
					}
					else
					{
						usernameTyped = true;
					}
					continue;
				}
				else
				{
					colorText(S_WHITE, L_BLACK, username);
					cout << endl;
				}

				if (!passwordTyped)
				{
					cout << "  ";
					colorText(L_WHITE, L_BLACK, "           RULES            ");
					cout << endl;
					colorText(L_WHITE, L_BLACK, " 1. Length should more than 8 ");
					cout << endl;
					colorText(L_WHITE, L_BLACK, " 2. Alphanumeric only         ");
					cout << endl;
					colorText(L_WHITE, L_BLACK, " 3. May contain underscore    ");
					cout << endl;
					colorText(L_WHITE, L_BLACK, "                            ");
					cout << endl;
				}
				cout << " Password: ";
				if (!passwordTyped)
				{
					cout << password;
					char inputChar = _getch();
					if (inputChar != '\r')
					{
						if ((int)inputChar == 8)
						{
							if (password.length() > 0)
							{
								password.pop_back();
							}
							else
							{
								// do nothing
							}
						}
						else
						{
							password += inputChar;
						}
						continue;
					}

					if (!User::validPassword(password))
					{
						password = ""; // reset password
					}
					else
					{
						passwordTyped = true;
					}
					continue;
				}
				else
				{
					colorText(S_WHITE, L_BLACK, password);
					cout << endl;
				}
			}			
		}
	}
}

void Viewer::displaySelectBoard(User* user, PageSelection& chosen)
{

	PageSelection toggle = PageSelection::BOARDS;

	vector<string> options = 
	{
		"Boards",
		"Chats",
		"Games",
		"Notifications",
		"Exit"
	};

	char inputKey = '.';
	while (inputKey != '\r')
	{
		system("cls");
		switch (inputKey)
		{
		case 72:
			(toggle != 0) ? toggle = PageSelection(toggle - 1) : toggle = PageSelection::EXIT;
			break;
		case 80:
			(toggle != options.size() - 1) ? toggle = PageSelection(toggle + 1) : toggle = PageSelection::BOARDS;
			break;
		case 75:
			toggle = PageSelection::EXIT;
		default:
			break;
		}

		cout << endl;
		Viewer::printGreetings(user->userName);
		Viewer::printHeader("Welcome to BW TalkTime!");
		cout << endl << endl;
		cout << " What would you like to do?" << endl;
		for (int i = 0; i < options.size(); i++)
		{
			if (toggle == i)
			{
				cout << " > ";
				setColor(L_WHITE, L_BLACK);
			}
			else
			{
				cout << "   ";
			}
			cout << options[i];
			if (user->adminLevel == 2)
			{
				if (i == PageSelection::NOTIFICATIONS)
				{
					cout << " (Sign up to see more!)";
				}
				else if (i == PageSelection::CHATS)
				{
					cout << " (Sign up to see more!)";
				}
			}
			cout << endl;
			resetColor();
		}

		COORD v;
		v.X = 0;
		v.Y = 29;

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), v);
		colorText(
			L_YELLOW,
			S_WHITE,
			" (<-) Back     (Enter) Select     (arrow down/up) Move                                                                  "
		);
		inputKey = _getch();
	}
	
	chosen = toggle;
}

void Viewer::displayBoard(User* user, bool& isBack, string& cmd, int& chosenBoard)
{
	int numberSpacing = 9;
	int nameSpacing = 74;
	int masterSpacing = 34;
	char inputKey = '.';

	int toggle = 0;

	int startChop = 0;
	int endChop = startChop + 10;
	if (endChop > Board::allTitleXBoardMakerId.size())
	{
		endChop = Board::allTitleXBoardMakerId.size();
	}

	while (inputKey != '\r' && !isBack)
	{
		switch (inputKey)
		{
		case 72:
			if (toggle > 0)
			{
				toggle--;
				if (toggle < startChop)
				{
					startChop -= 10;
					endChop = startChop +10;
				}
			}
			break;
		case 80:
			if (toggle < Board::allTitleXBoardMakerId.size() - 1)
			{
				toggle++;
				if (toggle > endChop - 1)
				{
					startChop += 10;
					endChop = startChop + 10;
					if (endChop > Board::allTitleXBoardMakerId.size())
					{
						endChop = Board::allTitleXBoardMakerId.size();
					}
				}
			}
			break;
		case 75:
			isBack = true;
			continue;
			break;
		default:
			break;
		}

		system("cls");
		cout << endl;
		printHeader("Board");
		setColor(L_WHITE, L_BLACK);
		cout << "   ";
		cout << " NO.";
		for (int i = 0; i < numberSpacing - 4; i++) cout << " ";
		cout << " Board Name";
		for (int i = 0; i < nameSpacing - 11; i++) cout << " ";
		cout << " Master";
		for (int i = 0; i < masterSpacing - 7; i++) cout << " ";
		cout << endl;
		resetColor();

		

		for (int i = startChop; i < endChop; i++)
		{
			if (toggle == i)
			{
				cout << " > ";
			}
			else
			{
				cout << "   ";
			}
			cout << " " << i + 1;
			for (int j = 0; j < numberSpacing - to_string(i + 1).length() - 1; j++) cout << " ";
			cout << " " << Board::allTitleXBoardMakerId[i].first;
			for (int j = 0; j < (nameSpacing - Board::allTitleXBoardMakerId[i].first.length() - 1); j++) cout << " ";
			Viewer::fakeLoadUserJSON();
			string tmp;
			if (!userNameSearch(Board::allTitleXBoardMakerId[i].second, tmp)) tmp = "Username not found!";
			cout << " " << tmp;
			for (int j = 0; j < masterSpacing - tmp.length() - 1; j++) cout << " ";
			cout << endl << endl;
		}


		COORD v;
		v.X = 0;
		v.Y = 29;

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), v);

		if (user->adminLevel == 0)
		{
			colorText(
				L_YELLOW,
				S_WHITE,
				" (<-) Back     (Enter) Select     (arrow down/up) Move     (/add <title>) add board     (/delete <title>) delete board  "
			);
		}
		else
		{
			colorText(
				L_YELLOW,
				S_WHITE,
				" (<-) Back     (Enter) Select     (arrow down/up) Move                                                                  "
			);
		}

		if (inputKey == '/' && user->adminLevel == 0)
		{
			COORD c;

			c.X = 0;
			c.Y = 29;

			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

			int spacingEnd = 100;
			setColor(L_YELLOW, S_WHITE);
			cout << " Type your command: ";
			resetColor();

			setColor(L_WHITE, L_BLACK);
			for (int i = 0; i < spacingEnd; i++)
			{
				cout << " ";
			}

			c.X = 21;
			c.Y = 29;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

			cmd = "";
			char inputChar = '.';
			while (inputChar != '\r')
			{
				inputChar = _getch();
				if (inputChar == -32)
				{
					char trash = _getch();
					continue;
				}
				if (inputChar == '\b')
				{
					if (cmd.length() > 0)
					{
						cmd.pop_back();
						cout << "\b \b";
					}
					else
					{
						// do nothing
					}
				}
				else
				{
					cmd += inputChar;
				}

				c.X = 21;
				c.Y = 29;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

				cout << cmd;

			}
			
			resetColor();
			if (cmd.length() > 0)
			{
				break;
			};
			inputKey = '.';
			continue;
		}


		inputKey = _getch();
	}
	if (cmd.length() > 0)
	{
		// break;
	}
	else
	{
		chosenBoard = toggle;
	}
}

void Viewer::displayPostList(User* user, Board* currentBoard, bool& isBack, int& chosenPost, string& cmd)
{
	int numberSpacing = 9;
	int nameSpacing = 59;
	int masterSpacing = 34;
	int popSpacing = 14;

	int toggle = 0;

	char inputKey = '.';

	int totalList = currentBoard->topThree.size() + currentBoard->collectionOfAllPost.size();

	int startChop = 0;
	int endChop = startChop + 10;
	if (endChop > totalList)
	{
		endChop = totalList;
	}

	while (inputKey != '\r' && !isBack)
	{
		switch (inputKey)
		{
		case 72:
			if (toggle > 0)
			{
				toggle--;
				if (toggle < startChop)
				{
					startChop -= 10;
					endChop = startChop + 10;
				}
			}
			break;
		case 80:
			if (toggle < totalList - 1)
			{
				toggle++;
				if (toggle > endChop - 1)
				{
					startChop += 10;
					endChop = startChop + 10;
					if (endChop > totalList)
					{
						endChop = totalList;
					}
				}
			}
			break;
		case 75:
			isBack = true;
			continue;
			break;
		default:
			break;
		}
		system("cls");
		cout << endl;
		printHeader(currentBoard->boardTitle + "'s posts");

		setColor(L_WHITE, L_BLACK);
		cout << "   ";
		cout << " NO.";
		for (int i = 0; i < numberSpacing - 4; i++) cout << " ";
		cout << " Popular";
		for (int i = 0; i < popSpacing - 8; i++) cout << " ";
		cout << " Post Title";
		for (int i = 0; i < nameSpacing - 11; i++) cout << " ";
		cout << " Created by";
		for (int i = 0; i < masterSpacing - 11; i++) cout << " ";
		cout << endl;
		resetColor();

		for (int i = startChop; i < endChop; i++)
		{
			if (toggle == i)
			{
				cout << " > ";
			}
			else
			{
				cout << "   ";
			}

			currentBoard->generateTopThree();
			if (i < currentBoard->topThree.size()) // topThree list
			{
				setColor(L_BLACK, S_YELLOW);
				cout << " " << i + 1;
				for (int j = 0; j < numberSpacing - to_string(i + 1).length() - 1; j++) cout << " ";
				cout << " " << currentBoard->topThree[i]->popularityPoints;
				for (int j = 0; j < popSpacing - to_string(currentBoard->topThree[i]->popularityPoints).length(); j++) cout << " ";
				cout << " " << currentBoard->topThree[i]->postTitle;
				switch (i)
				{
				case 0:
					cout << " (Top 1)";
					break;
				case 1:
					cout << " (Top 2)";
					break;
				case 2:
					cout << " (Top 3)";
					break;
				}
				string pop = to_string(currentBoard->topThree[i]->popularityPoints);
				for (int j = 0; j < nameSpacing - currentBoard->topThree[i]->postTitle.length() - 1 - 8; j++) cout << " ";
				Viewer::fakeLoadUserJSON();
				string tmp;
				if (!userNameSearch(currentBoard->topThree[i]->posterId, tmp)) tmp = "Username not found!";
				cout << " " << tmp;
				for (int j = 0; j < masterSpacing - tmp.length() - 1; j++) cout << " ";
				cout << endl;
				resetColor();
				if (i == currentBoard->topThree.size() - 1)
				{
					setColor(L_WHITE, L_BLACK);
					cout << " Regular List:                                                                                                          ";
					resetColor();
					cout << endl;
				}
				else
				{
					cout << endl;
				}
			}
			else if(i < currentBoard->collectionOfAllPost.size() + currentBoard->topThree.size()) // normal list
			{
				cout << " " << i - currentBoard->topThree.size() + 1;
				for (int j = 0; j < numberSpacing - to_string(i - currentBoard->topThree.size() + 1).length() - 1; j++) cout << " ";
				cout << " ";
				colorText(L_BLACK, S_PINK, to_string(currentBoard->collectionOfAllPost[i - (currentBoard->topThree.size())].popularityPoints));
				for (int j = 0; j < popSpacing - to_string(currentBoard->collectionOfAllPost[i - (currentBoard->topThree.size())].popularityPoints).length(); j++) cout << " ";
				cout << " " << currentBoard->collectionOfAllPost[i - (currentBoard->topThree.size())].postTitle;
				for (int j = 0; j < nameSpacing - currentBoard->collectionOfAllPost[i - currentBoard->topThree.size()].postTitle.length() - 1; j++) cout << " ";
				string tmp;
				if (!userNameSearch(currentBoard->collectionOfAllPost[i - currentBoard->topThree.size()].posterId, tmp)) tmp = "Username not found!";
				cout << " " << tmp;
				for (int j = 0; j < masterSpacing - tmp.length() - 1; j++) cout << " ";
				cout << endl << endl;
			}
		}
		COORD v;

		v.X = 0;
		v.Y = 29;

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), v);

		if (user->adminLevel == 0 || user->adminLevel == 1)
		{
			colorText(
				L_YELLOW,
				S_WHITE,
				" (<-) Back     (Enter) Select     (arrow down/up) Move     (/add <title>) add newpost                                   "
			);
		}
		else
		{
			colorText(
				L_YELLOW,
				S_WHITE,
				" (<-) Back     (Enter) Select     (arrow down/up) Move                                                                  "
			);
		}


		if (inputKey == '/' && user->adminLevel  < 2)
		{
			COORD c;

			c.X = 0;
			c.Y = 29;

			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

			int spacingEnd = 100;
			setColor(L_YELLOW, S_WHITE);
			cout << " Type your command: ";
			resetColor();

			setColor(L_WHITE, L_BLACK);
			for (int i = 0; i < spacingEnd; i++)
			{
				cout << " ";
			}

			c.X = 21;
			c.Y = 29;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

			cmd = "";
			char inputChar = '.';
			while (inputChar != '\r')
			{
				inputChar = _getch();
				if (inputChar == -32)
				{
					char trash = _getch();
					continue;
				}
				if (inputChar == '\b')
				{
					if (cmd.length() > 0)
					{
						cmd.pop_back();
						cout << "\b \b";
					}
					else
					{
						// do nothing
					}
				}
				else
				{
					cmd += inputChar;
				}

				c.X = 21;
				c.Y = 29;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

				cout << cmd;
			}
			resetColor();
			if (cmd.length() > 0)
			{
				break;
			};
			inputKey = '.';
			continue;
		}

		inputKey = _getch();
	}
	if (cmd.length() > 0)
	{
		// break;
	}
	else
	{
		if (currentBoard->topThree.size() != 0 || currentBoard->collectionOfAllPost.size() != 0)
		{
			chosenPost = toggle;
		}
	}
}

void Viewer::displayPost(User* user, Board* currentBoard, bool& isBack, string& finalComment, bool& deleting, int& deleteIndex, bool& deletingPost, bool& editingPost)
{
	char inputKey = '.';
	string comment = "";
	bool first = true;
	bool onEdit = false;
	bool onDelete = false;
	bool onEditPost = false;
	bool onDeletePost = false;
	bool first1 = true;
	while (inputKey != '\r')
	{
		if (first1)
		{
			first1 = !first1;
		}
		else
		{
			inputKey = _getch();
		}
		if (inputKey == -32)
		{
			char trash = _getch();
			continue;
		}
		if (user->adminLevel == 2)
		{
			comment = " (Sign up to comment)";
			if (inputKey == '\033')
			{
				switch (_getch())
				{
				case 'a':
				case 'A':
					isBack = true;
					break;
				}
			}
		}
		else
		{
			if (inputKey == '\033')
			{
				switch (_getch())
				{
				case 'a':
				case 'A':
					isBack = true;
					break;
				case 'e':
				case 'E':
					onEdit = true;
					break;
				case 'd':
				case 'D':
					onDelete = true;
					break;
				case 'p':
				case 'P':
					onEditPost = true;
					break;
				case 'l':
				case 'L':
					onDeletePost = true;
					break;
				}
			}
			else if (inputKey == '\b')
			{
				if (!comment.empty())
				{
					comment.pop_back();
				}
			}
			else
			{
				if (first)
				{
					first = !first;
				}
				else
				{
					comment += inputKey;
				}

			}
		}
		system("cls");
		if (user->adminLevel == 0 || user->adminLevel == 1)
		{
			colorText(L_YELLOW, S_WHITE, " (esc+A) Back  (esc+E+<index_of_comment>) Edit Comment  (esc+D+<index_of_comment>) Delete Comment                       ");
		}
		else
		{
			colorText(L_YELLOW, S_WHITE, " (esc+A) Back                                                                                                           ");
		}
		cout << endl;
		if (checkPostSameUser(user, currentBoard))
		{
			colorText(L_YELLOW, S_WHITE, "                (esc+P) Edit Post                       (esc+L) Delete Post                                             ");
			cout << endl;
		}
		else if(user->adminLevel == 0)
		{
			colorText(L_YELLOW, S_WHITE, "                                                        (esc+L) Delete Post                                             ");
			cout << endl;
		}
		printHeader(currentBoard->currentPost->postTitle);
		cout << "═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════ " << endl;
		cout << endl;
		cout << currentBoard->currentPost->postContent;
		cout << endl;
		cout << endl;
		cout << "══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝ " << endl;
		colorText(L_BLACK, S_YELLOW, "    Views: " + to_string(currentBoard->currentPost->views));
		cout << "      ";
		colorText(L_BLACK, S_PINK, " Popularity:" + to_string(currentBoard->currentPost->popularityPoints));
		cout << endl << endl;;
		cout << " Comments:" << endl;
		if (currentBoard->currentPost->userComment.size() == 0)
		{
			cout << "\n\n\n" << endl;
		}
		else
		{
			// print comments
			int indexSpacing = 3;
			int nameSpacing = 20;
			for (int i = 0; i < currentBoard->currentPost->userComment.size(); i++)
			{
				cout << " ";
				string tmpInt = to_string(i);
				for (int x = 0; x < indexSpacing - tmpInt.length(); x++)
				{
					cout << " ";
				}
				cout << i << " ";
				switch (currentBoard->currentPost->userComment[i].commentType)
				{
				case 0:
					colorText(L_BLACK, L_GREEN, "LIKE    ");
					break;
				case 1:
					colorText(L_BLACK, L_YELLOW, "NEUTRAL ");
					break;
				case 2:
					colorText(L_BLACK, L_RED, "DISLIKE ");
					break;
				default:
					break;
				}
				string tmp;
				fakeLoadUserJSON();
				if (!userNameSearch(currentBoard->currentPost->userComment[i].userId, tmp)) tmp = "Username not Found!";

				cout << tmp;
				for (int x = 0; x < nameSpacing - tmp.length(); x++)
				{
					cout << " ";
				}
				cout << "\t";
				cout << ": ";
				cout << currentBoard->currentPost->userComment[i].comment << endl;
			}
		}
		cout << endl;
		POINT p;
		GetCursorPos(&p);
		cout << "************************************************************************************************************************" << endl;
		colorText(L_BLACK, S_WHITE, " Comment Syntax: <");
		colorText(L_BLACK, S_RED, "0");
		colorText(L_BLACK, S_WHITE, ":LIKE/");
		colorText(L_BLACK, S_RED, "1");
		colorText(L_BLACK, S_WHITE, ":NEUTRAL/");
		colorText(L_BLACK, S_RED, "2");
		colorText(L_BLACK, S_WHITE, ":DISLIKE>");
		colorText(L_BLACK, S_WHITE, "(space)");
		colorText(L_BLACK, S_WHITE, "<");
		colorText(L_BLACK, S_RED, "type_comment");
		colorText(L_BLACK, S_WHITE, "> and press ENTER");
		cout << endl;

		cout << " Your Comment: ";
		cout << comment;

		if (isBack)
		{
			break;
		}
		else if (onEdit)
		{
			string newComment = "";
			string indexOfComment = "";
			int indexRes;
			cout << endl << " Comment Index: ";
			cin >> indexOfComment;
			if (!is_number(indexOfComment))
			{
				onEdit = false;
				Viewer::colorText(L_RED, L_WHITE, "\n Please enter a digit!");
				continue;
			}
			indexRes = stoi(indexOfComment);
			if (indexRes >= 0 && indexRes < currentBoard->currentPost->userComment.size())
			{
				// found
				cout << " Replace by: ";
				getchar();
				getline(cin, newComment);
				if (!editComment(indexRes, newComment, currentBoard, user)) {
					Viewer::colorText(L_RED, L_WHITE, "\n This comment doesn't belongs to you. (enter to continue)");
				}
				else
				{
					Viewer::colorText(L_GREEN, L_WHITE, "\n Your comment has changed! (enter to continue)");

				}
			}
			else
			{
				// not found
				Viewer::colorText(L_RED, L_WHITE, "\n Index not Found! (enter to continue)");
			}
			onEdit = false;
			continue;
		}
		else if (onDelete)
		{
			string indexOfComment = "";
			int indexRes;
			cout << endl << " Delete at Index: ";
			cin >> indexOfComment;
			if (!is_number(indexOfComment))
			{
				onEdit = false;
				Viewer::colorText(L_RED, L_WHITE, "\n Please enter a digit!");
				continue;
			}
			indexRes = stoi(indexOfComment);
			if (indexRes >= 0 && indexRes < currentBoard->currentPost->userComment.size())
			{
				// found
				getchar();
				onDelete = false;
				deleting = true;
				deleteIndex = indexRes;
				break;
			}
			else
			{
				// not found
				Viewer::colorText(L_RED, L_WHITE, "\n Index not Found! (enter to continue)");
			}
			onDelete = false;
			continue;
		}
		else if (onEditPost)
		{
			onEditPost = false;
			editingPost = true;
			break;
		}
		else if (onDeletePost)
		{
			deletingPost = true;
			onDeletePost = false;
			break;
		}
		
	}

	if (user->adminLevel == 2)
	{
		comment = "";
	}
	finalComment = comment;


}

void Viewer::displayNotification(User* user, bool& isBack)
{
	char inputKey = '.';

	while (inputKey != '\033')
	{
		switch (inputKey)
		{
		case 75:
			isBack = true;
			break;
		default:
			break;
		}
		system("cls");
		printHeader("Notifications");


		if (user->notification.size() == 0)
		{
			cout << endl;
			cout << " ║ No notification for you currently..." << endl;
		}
		else
		{
			for (auto iterator = user->notification.begin(); iterator != user->notification.end(); iterator++)
			{
				stringstream ss(*iterator);
				string line1, line2, line3;
				if (iterator->find("has sent you a message, kindly check your mail.") != string::npos)
				{
					// mail
					cout << " ║  ";
					cout << ss.str() << endl;
				}
				else
				{
					getline(ss, line1, '\n');
					getline(ss, line2, '\n');
					getline(ss, line3, '\n');
					cout << " ║  ";
					cout << line1 << endl;;
					cout << " ║  ";
					cout << line2 << endl;;
					cout << " ║  ";
					cout << line3 << endl;;
				}
				cout << endl;
			}
		}

		COORD x;

		x.X = 0;
		x.Y = 29;

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), x);
		colorText(
			L_YELLOW,
			L_WHITE,
			" (<-) Back     (Esc) Clear                                                                                              "
		);

		if (isBack)
		{
			break;
		}

		inputKey = _getch();
	}

	string inputAns = "";
	if (inputKey == '\033')
	{
		COORD c;

		c.X = 0;
		c.Y = 29;

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

		int spacingEnd = 72;
		setColor(L_YELLOW, S_WHITE);
		cout << " Do you want to clear your notification?: [Y/n] ";
		resetColor();

		setColor(L_WHITE, L_BLACK);
		for (int i = 0; i < spacingEnd; i++)
		{
			cout << " ";
		}

		c.X = 49;
		c.Y = 29;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

		inputAns = "";
		char inputChar = _getch();
		while (inputChar != '\r')
		{
			if (inputChar == '\b')
			{
				if (inputAns.length() > 0)
				{
					inputAns.pop_back();
					cout << "\b \b";
				}
				else
				{
					// do nothing
				}
			}
			else
			{
				inputAns += inputChar;
			}

			c.X = 49;
			c.Y = 29;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

			cout << inputAns;

			inputChar = _getch();
		}
		resetColor();
		if (inputAns == "Y")
		{
			user->clearNotification();
		}
	}
}

void Viewer::displayChatPage(bool& isBack, string& targetUser)
{
	// set target
	system("cls");
	char inputChar = '.';
	bool first = true;
	bool first1 = true;
	string getUsername = "";
	printHeader("Chat Time!");
	colorText(L_BLACK, S_GREEN, " Type and Press ENTER to proceed!");
	cout << endl;
	cout << " Who do you want to chat with? : (type username)";
	getline(cin, getUsername);

	targetUser = getUsername;

}

void Viewer::displayChat(bool& isBack, User* user1, User* user2)
{
	Chat chatList(user1, user2, user1);
	string username1, username2;

	if (chatList.starterMessage == 1)
	{
		username1 = chatList.userPointer1->userName;
		username2 = chatList.userPointer2->userName;
	}
	else if (chatList.starterMessage == 2)
	{
		username1 = chatList.userPointer2->userName;
		username2 = chatList.userPointer1->userName;
	}

	
	while (1)
	{
		string textGo = "";
		system("cls");
		cout << "========================================================================================================================" << endl;
		cout << "||      Chat with: ";
		cout << user2->userName << endl;
		cout << "========================================================================================================================" << endl;
		
		if (chatList.messages.size() > 0)
		{
			for (int i = 0; i < chatList.messages.size(); i++)
			{
				cout << " ";
				if (i % 2 == 0)
				{
					colorText(L_BLACK, S_GREEN, username1 + "\t: ");
				}
				else
				{
					colorText(L_BLACK, S_YELLOW, username2 + "\t: ");
				}
				stringstream ss(chatList.messages[i]);
				string getMsg;
				bool first = true;
				while (getline(ss, getMsg, '\n'))
				{
					if (first)
					{
						first = !first;
					}
					else
					{
						if (i % 2 == 0)
						{
							colorText(L_BLACK, S_GREEN, "\t\t: ");
						}
						else
						{
							colorText(L_BLACK, S_YELLOW, "\t\t: ");
						}
					}
					cout << getMsg << endl;
				}
			}
		}
		else
		{
			cout << "\n\n\n\n";
		}

		cout << "************************************************************************************************************************" << endl;
		setColor(L_BLACK, S_GREEN);
		cout << " Enter to send; send nothing to get back to previous screen :p";
		resetColor();
		cout << endl;
		cout << " Your message: ";
		getline(cin, textGo);

		if (!textGo.empty())
		{
			chatList.writeMessage(textGo, user1);
		}
		else
		{
			cout << endl;
			colorText(L_RED, L_WHITE, "Do you want to leave this chat? [Y/n]: ");
			string tmpAns;
			getline(cin, tmpAns);
			if (tmpAns == "Y")
			{
				isBack = true;
				break;
			}
		}
	}

}

void Viewer::showLoading()
{
	cout << endl; 
	COORD v;

	setColor(L_BLACK, S_GREEN);
	for (int i = 1; i <= 15; i++)
	{
		v.X = 0;
		v.Y = 20;

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), v);
		cout << " Please wait";
		if (i % 3 == 1)
		{
			cout << " .    ";
		}
		else if (i % 3 == 2)
		{
			cout << "   .  ";
		}
		else if (i % 3 == 0)
		{
			cout << "     .";
		}
		Sleep(100);
	}
	resetColor();
}