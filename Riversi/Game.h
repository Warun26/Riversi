//
//  Game.h
//  Riversi
//
//  Created by Varun Varadarajan on 10/10/14.
//  Copyright (c) 2014 Varun Varadarajan. All rights reserved.
//

#ifndef __Riversi__Game__
#define __Riversi__Game__
#define State vector<vector<char> >
#include <algorithm>

using namespace std;

struct Cell
{
    int row;
    int column;
    int weight;
    Cell()
    {
        row=0;
        column=0;
    }
    Cell(int row, int column)
    {
        this->row = row;
        this->column = column;
    }
    Cell(int row, int column, int weight)
    {
        this->row = row;
        this->column = column;
        this->weight = weight;
    }
    bool operator() (Cell c1, Cell c2)
    {
        if(c1.row < c2.row) return true;
        else if(c1.row == c2.row) return c1.column < c2.column;
        return false;
    }
    bool static UniqueFunction(Cell c1, Cell c2)
    {
        return (c1.row == c2.row && c1.column == c2.column);
    }
}cellFunction;

class Game
{
private:
    State initialState;
    int boardSize;
    char player;
    char opponentPlayer;
    int cutOffDepth;
    vector<vector<int> > boardWeights;
    bool AllCellsFilled(State state);
    Cell FindOpponentPiece(int startX, int startY);
    vector<Cell>FindMovesAdjacentToCell(Cell cell);
    void FlipPieces(State& state, Cell cell);
    
public:
    //Constructors
    Game(char player, char opponentPlayer, State state);
    Game(char player, char opponentPlayer, State state, vector<vector<int> > positionWeights);
    Game(char player, char opponentPlayer, State state, int cuttOffDepth);
    Game(char player, char opponentPlayer, State state, int cutOffDepth, vector<vector<int> > positionWeights);
    //HelperFunctions
    vector<Cell> Actions(State state);
    vector<Cell> Actions();
    State Result(State state, Cell action);
    State Result(Cell action);
    bool TerminalTest(State state);
    bool TerminalTest();
    bool TerminalTest_DEPRECATED(State state);
    int UtilityFunction(State state, char player);
    int UtilityFunction();
    //Getters
    State GetCurrentGameState();
    int GetCutOffDepth();
    char GetPlayer();
    char GetOpponentPlayer();
};

//CONSTRUCTORS
Game::Game(char player, char opponentPlayer, State initialState)
{
    this->player = player;
    this->opponentPlayer = opponentPlayer;
    this->initialState = initialState;
    this->boardSize = (int)initialState.size();
    this->cutOffDepth = -1;
    int boardValues[8][8] = {{99,-8,8,6,6,8,-8,99},{-8,-24,-4,-3,-3,-4,-24,-8},{8,-4,7,4,4,7,-4,8},{6,-3,4,0,0,4,-3,6},{6,-3,4,0,0,4,-3,6},{8,-4,7,4,4,7,-4,8},{-8,-24,-4,-3,-3,-4,-24,-8},{99,-8,8,6,6,8,-8,99}};
    for (int row=0; row<8; row++)
    {
        this->boardWeights.push_back(vector<int>());
        for (int column=0; column<8; column++)
        {
            this->boardWeights[row].push_back(boardValues[row][column]);
        }
    }
}

Game::Game(char player, char opponentPlayer, State initialState, vector<vector<int> > positionWeights)
{
    this->player = player;
    this->opponentPlayer = opponentPlayer;
    this->initialState = initialState;
    this->cutOffDepth = -1;
    this->boardWeights = positionWeights;
}

Game::Game(char player, char opponentPlayer, State initialState, int cutOffDepth)
{
    this->player = player;
    this->opponentPlayer = opponentPlayer;
    this->initialState = initialState;
    this->boardSize = (int)initialState.size();
    this->cutOffDepth = cutOffDepth;
    int boardValues[8][8] = {{99,-8,8,6,6,8,-8,99},{-8,-24,-4,-3,-3,-4,-24,-8},{8,-4,7,4,4,7,-4,8},{6,-3,4,0,0,4,-3,6},{6,-3,4,0,0,4,-3,6},{8,-4,7,4,4,7,-4,8},{-8,-24,-4,-3,-3,-4,-24,-8},{99,-8,8,6,6,8,-8,99}};
    for (int row=0; row<8; row++)
    {
        this->boardWeights.push_back(vector<int>());
        for (int column=0; column<8; column++)
        {
            this->boardWeights[row].push_back(boardValues[row][column]);
        }
    }
}

Game::Game(char player, char opponentPlayer, State initialState, int cutOffDepth, vector<vector<int> > positionWeights)
{
    this->player = player;
    this->opponentPlayer = opponentPlayer;
    this->initialState = initialState;
    this->boardWeights = positionWeights;
    this->cutOffDepth = cutOffDepth;
}

//PUBLIC FUNCTIONS
int Game::UtilityFunction(State state, char player)
{
    int playerWeights = 0, opponentWeights=0;
    for(int row=0; row<state.size(); row++)
    {
        for (int column=0; column<state[row].size(); column++)
        {
            if (state[row][column] == player) playerWeights += boardWeights[row][column];
            else if (state[row][column] == opponentPlayer) opponentWeights += boardWeights[row][column];
        }
    }
    return playerWeights - opponentWeights;
}

int Game::UtilityFunction()
{
    return UtilityFunction(this->initialState, this->player);
}

bool Game::TerminalTest(State state)
{
    Cell cell = FindOpponentPiece(0, 0);
    if (cell.row == -1 && cell.column == -1) return true;
    else return false;
}

bool Game::TerminalTest_DEPRECATED(State state)
{
    if (AllCellsFilled(state)) return true;
    vector<Cell> actions = Actions();
    Game newGame = *new Game(opponentPlayer, player, state, cutOffDepth);
    if (!actions.size() && !newGame.Actions().size()) return true;
    return false;
}

bool Game::TerminalTest()
{
    return TerminalTest(this->initialState);
}

State Game::Result(State state, Cell action)
{
    state[action.row][action.column] = this->player;
    FlipPieces(state, action);
    return state;
}

State Game::Result(Cell action)
{
    return Result(this->initialState, action);
}

vector<Cell> Game::Actions(State state)
{
    vector<Cell> actions;
    Cell opponentPiece(0,0);
    int x = 0, y=0;
    do
    {
        opponentPiece = FindOpponentPiece(x,y);
        if(opponentPiece.row >= 0 && opponentPiece.column >= 0)
        {
            vector<Cell> moves = FindMovesAdjacentToCell(opponentPiece);
            actions.insert(actions.end(), moves.begin(), moves.end());
        }
        x = opponentPiece.row;
        y = opponentPiece.column+1;
        if (y >= boardSize)
        {
            x = opponentPiece.row + 1;
            y = 0;
        }
        
    }while(x<boardSize && y<boardSize && opponentPiece.row>=0 && opponentPiece.column >= 0);
    sort(actions.begin(), actions.end(),cellFunction);
    actions.erase(unique(actions.begin(), actions.end(), cellFunction.UniqueFunction), actions.end());
    return actions;
}

vector<Cell> Game::Actions() { return Actions(this->initialState); }
State Game::GetCurrentGameState(){ return this->initialState; }
int Game::GetCutOffDepth(){ return this->cutOffDepth; }
char Game::GetPlayer() { return player; }
char Game::GetOpponentPlayer() { return opponentPlayer; }

//PRIVATE FUNCTIONS
bool Game::AllCellsFilled(State state)
{
    for(int row=0; row<boardSize; row++)
        for (int column=0; column<boardSize; column++)
            if (state[row][column] == '*') return false;
    return true;
}

Cell Game::FindOpponentPiece(int startX, int startY)
{
    for(int column=startY; column<boardSize; column++)
        if (initialState[startX][column] != player && initialState[startX][column] != '*')
            return *new Cell(startX,column);
    for (int row=startX + 1; row<boardSize; row++)
        for (int column=0; column<boardSize; column++)
            if (initialState[row][column] != player && initialState[row][column] != '*')
                return *new Cell(row,column);
    return *new Cell(-1,-1);
}

vector<Cell> Game::FindMovesAdjacentToCell(Cell cell)
{
    vector<Cell> moves;
    //Check if a new piece can be placed to the top left
    if (cell.row > 0 &&cell.column > 0 && initialState[cell.row-1][cell.column-1] == '*')
    {
        int columnChecker = cell.row + 1;
        int rowChecker = cell.column + 1;
        while (columnChecker < boardSize && rowChecker < boardSize && initialState[columnChecker][rowChecker] == initialState[cell.row][cell.column]) {
            columnChecker++; rowChecker++;
        }
        if (columnChecker<boardSize && rowChecker < boardSize && initialState[columnChecker][rowChecker]== player)
            moves.push_back(Cell(cell.row-1, cell.column-1, boardWeights[cell.row-1][cell.column-1]));
    }
    //Check if a new piece can be place above
    if (cell.row > 0 && initialState[cell.row-1][cell.column] == '*')
    {
        int columnChecker = cell.row + 1;
        while (columnChecker < boardSize && initialState[columnChecker][cell.column] == initialState[cell.row][cell.column]) columnChecker++;
        if (columnChecker<boardSize && initialState[columnChecker][cell.column]== player)
            moves.push_back(Cell(cell.row-1, cell.column, boardWeights[cell.row-1][cell.column]));
    }
    //Check if a new piece can be placed to the top right
    if (cell.row > 0 && cell.column < boardSize-1 && initialState[cell.row-1][cell.column+1] == '*')
    {
        int columnChecker = cell.row + 1;
        int rowChecker = cell.column - 1;
        while (columnChecker < boardSize && rowChecker >= 0 && initialState[columnChecker][rowChecker] == initialState[cell.row][cell.column]) {
            columnChecker++; rowChecker--;
        }
        if (columnChecker<boardSize && rowChecker >= 0 && initialState[columnChecker][rowChecker]== player)
            moves.push_back(Cell(cell.row-1, cell.column+1, boardWeights[cell.row-1][cell.column+1]));
    }
    //Check if a new piece can be placed on the left
    if (cell.column > 0 && initialState[cell.row][cell.column-1] == '*')
    {
        int rowChecker = cell.column + 1;
        while (rowChecker < boardSize && initialState[cell.row][rowChecker] == initialState[cell.row][cell.column]) rowChecker++;
        if (rowChecker<boardSize && initialState[cell.row][rowChecker]==player)
            moves.push_back(Cell(cell.row, cell.column-1, boardWeights[cell.row][cell.column-1]));
    }
    //Check if a new piece can be placed on the right
    if (cell.column<boardSize-1 && initialState[cell.row][cell.column+1] == '*')
    {
        int rowChecker = cell.column - 1;
        while (rowChecker >= 0 && initialState[cell.row][rowChecker] == initialState[cell.row][cell.column])
            rowChecker--;
        if (rowChecker>=0 && initialState[cell.row][rowChecker]==player)
            moves.push_back(Cell(cell.row, cell.column+1, boardWeights[cell.row][cell.column+1]));
    }
    //Check if a new piece can be placed to the bottom left
    if (cell.row < boardSize-1 && cell.column > 0 && initialState[cell.row+1][cell.column-1] == '*')
    {
        int columnChecker = cell.row - 1;
        int rowChecker = cell.column + 1;
        while (columnChecker >= 0 && rowChecker < boardSize && initialState[columnChecker][rowChecker] == initialState[cell.row][cell.column]) {
            columnChecker--; rowChecker++;
        }
        if (columnChecker>=0 && rowChecker < boardSize && initialState[columnChecker][rowChecker]== player)
            moves.push_back(Cell(cell.row+1, cell.column-1, boardWeights[cell.row+1][cell.column-1]));
    }
    //Check if a new piece can be place below
    if (cell.row < boardSize-1 && initialState[cell.row+1][cell.column] == '*')
    {
        int columnChecker = cell.row - 1;
        while (columnChecker >= 0 && initialState[columnChecker][cell.column] == initialState[cell.row][cell.column])
            columnChecker--;
        if (columnChecker>=0 && initialState[columnChecker][cell.column]== player)
            moves.push_back(Cell(cell.row+1, cell.column, boardWeights[cell.row+1][cell.column]));
    }
    //Check if a new piece can be placed to the bottom right
    if (cell.row <boardSize-1 && cell.column < boardSize-1 && initialState[cell.row+1][cell.column+1] == '*')
    {
        int columnChecker = cell.row - 1;
        int rowChecker = cell.column - 1;
        while (columnChecker >= 0 && rowChecker >= 0 && initialState[columnChecker][rowChecker] == initialState[cell.row][cell.column]) {
            columnChecker--; rowChecker--;
        }
        if (columnChecker>=0 && rowChecker >= 0 && initialState[columnChecker][rowChecker]== player)
            moves.push_back(Cell(cell.row+1, cell.column+1, boardWeights[cell.row+1][cell.column+1]));
    }
    return moves;
}

void Game::FlipPieces(State& state, Cell cell)
{
    //Checking pieces to the right of currentCell
    {
        bool flag=false;
        int column = cell.column + 1;
        while (column < boardSize && state[cell.row][column] != '*')
        {
            if (state[cell.row][column]==player) {
                flag = true;
                break;
            }
            column++;
        }
        if (flag) for (int c=cell.column+1; c<column; c++) state[cell.row][c]=player;
    }
    //Checking pieces to the left of currentCell
    {
        bool flag=false;
        int column = cell.column - 1;
        while (column >= 0 && state[cell.row][column] != '*')
        {
            if (state[cell.row][column]==player) {
                flag = true;
                break;
            }
            column--;
        }
        if (flag) for (int c=cell.column-1; c>column; c--) state[cell.row][c]=player;
    }
    //Checking pieces to below the currentCell
    {
        bool flag=false;
        int row = cell.row+1;
        while (row <boardSize && state[row][cell.column] != '*')
        {
            if (state[row][cell.column]==player) {
                flag = true;
                break;
            }
            row++;
        }
        if (flag) for (int c=cell.row+1; c<row; c++) state[c][cell.column]=player;
    }
    //Checking pieces to above the currentCell
    {
        bool flag=false;
        int row = cell.row-1;
        while (row >=0 && state[row][cell.column] != '*')
        {
            if (state[row][cell.column]==player) {
                flag = true;
                break;
            }
            row--;
        }
        if (flag) for (int c=cell.row-1; c>row; c--) state[c][cell.column]=player;
    }
    //Checking pieces on the primary diagnol downwards from the currentCell
    {
        bool flag=false;
        int row = cell.row+1;
        int column = cell.column+1;
        while (row < boardSize && column<boardSize && state[row][column] != '*')
        {
            if (state[row][column]==player) {
                flag = true;
                break;
            }
            row++; column++;
        }
        if (flag) for (int c=cell.row+1, d=cell.column+1; c<row && d<column; c++, d++) state[c][d]=player;
    }
    //Checking pieces on the primary diagnol upwards from the currentCell
    {
        bool flag=false;
        int row = cell.row-1;
        int column = cell.column-1;
        while (row >= 0 && column>=0 && state[row][column] != '*')
        {
            if (state[row][column]==player) {
                flag = true;
                break;
            }
            row--; column--;
        }
        if (flag) for (int c=cell.row-1, d=cell.column-1; c>row && d>column; c--, d--) state[c][d]=player;
    }
    //Checking pieces on the secondary diagnol downwards from the currentCell
    {
        bool flag=false;
        int row = cell.row+1;
        int column = cell.column-1;
        while (row < boardSize && column>=0 && state[row][column] != '*')
        {
            if (state[row][column]==player) {
                flag = true;
                break;
            }
            row++; column--;
        }
        if (flag) for (int c=cell.row+1, d=cell.column-1; c<row && d>column; c++, d--) state[c][d]=player;
    }
    //Checking pieces on the secondary diagnol upwards from the currentCell
    {
        bool flag=false;
        int row = cell.row-1;
        int column = cell.column+1;
        while (row >= 0 && column<boardSize && state[row][column] != '*')
        {
            if (state[row][column]==player) {
                flag = true;
                break;
            }
            row--; column++;
        }
        if (flag) for (int c=cell.row-1, d=cell.column+1; c>row && d<column; c--, d++) state[c][d]=player;
    }
}
#endif /* defined(__Riversi__Game__) */
