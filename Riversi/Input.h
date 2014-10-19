//
//  Input.h
//  Riversi
//
//  Created by Varun Varadarajan on 10/10/14.
//  Copyright (c) 2014 Varun Varadarajan. All rights reserved.
//

#ifndef Riversi_Input_h
#define Riversi_Input_h

#include <fstream>
#include <string>
#include <vector>
using namespace std;
class Input
{
private:
    int boardSize;
    int searchAlgorithm;
    char player;
    char opponentPlayer;
    int cutOffDepth;
    vector<vector<char> > board;
    void ReadInput(string fileName);
    void SetOpponentPlayer();
    
public:
    Input(string inputFileName="input.txt");
    Input(int boardSize, string inputFileName="input.txt");
    int GetBoardSize();
    char GetPlayer();
    int GetCutOffDepth();
    int GetSearchAlgorithm();
    vector<vector<char> >GetBoard();
    char GetOpponentPlayer();
};

//PRIVATE FUNCTIONS
void Input::ReadInput(string inputFileName)
{
    ifstream inputFile;
    char *fileName=new char[inputFileName.size()+1];
    fileName[inputFileName.size()]=0;
    memcpy(fileName,inputFileName.c_str(),inputFileName.size());
    inputFile.open(fileName);
    inputFile>>searchAlgorithm;
    inputFile>>player;
    inputFile>>cutOffDepth;
    for (int row=0; row<boardSize; row++)
    {
        string boardLine;
        inputFile>>boardLine;
        board.push_back(vector<char>());
        for (int column=0; column<boardSize; column++)
        {
            board[row].push_back(boardLine[column]);
        }
    }
}

void Input::SetOpponentPlayer()
{
    for (int row=0; row<boardSize; row++)
    {
        for (int column=0; column<boardSize; column++)
        {
            if (board[row][column] != player && board[row][column] != '*')
            {
                this->opponentPlayer = board[row][column];
                return;
            }
        }
    }
}

//CONSTRUCTORS
Input::Input(string inputFileName)
{
    boardSize = 8;
    ReadInput(inputFileName);
    SetOpponentPlayer();
}
Input::Input(int boardSize, string inputFileName)
{
    this->boardSize = boardSize;
    ReadInput(inputFileName);
    SetOpponentPlayer();
}

//PUBLIC FUNCTIONS
int Input::GetBoardSize() { return boardSize; }
int Input::GetSearchAlgorithm() { return searchAlgorithm; }
char Input::GetPlayer() { return player; }
int Input::GetCutOffDepth() { return cutOffDepth; }
vector<vector<char> > Input::GetBoard(){ return board; }
char Input::GetOpponentPlayer() {return opponentPlayer; }
#endif
