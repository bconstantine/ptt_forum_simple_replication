#include <fcntl.h>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include "BoardManager.h"

using namespace std;

void errorMsg(string errorMessage);

int main()
{
	BoardManager bm;
	/*string newTitle = "betaTesting";
	string BoardMakerId = "1214281865";
	Board::allTitleXBoardMakerId.push_back(make_pair(newTitle, BoardMakerId));
	Board::saveAllTitleJSON();
	cout << "size: " << Board::allTitleXBoardMakerId.size() << endl;
	Board newBoard;
	newBoard.boardTitle = newTitle;
	newBoard.boardMakerId = BoardMakerId;
	bm.collectionOfAllBoard.push_back(newBoard);
	bm.collectionOfAllBoard[0].savePostJSON();
	system("pause");*/
	//saving the board
	bm.startLoop();

}

