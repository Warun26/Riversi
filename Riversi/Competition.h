//
//  Competition.h
//  Riversi
//
//  Created by Varun Varadarajan on 10/18/14.
//  Copyright (c) 2014 Varun Varadarajan. All rights reserved.
//

#ifndef Riversi_Competition_h
#define Riversi_Competition_h

using namespace std;
class Competition
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
    float FindPieceDifference();
    int GetBoardWeights();
    int GetCornerOccupancy();
    double GetCornerClosenessOccupancy();
    int GetMobility();
    
public:
    //Constructors
    Competition(char player, char opponentPlayer, State state);
    //HelperFunctions
    vector<Cell> Actions();
    State Result(Cell action);
    bool TerminalTest();
    double UtilityFunction();
    //Getters
    State GetCurrentGameState();
    int GetCutOffDepth();
    char GetPlayer();
    char GetOpponentPlayer();
    
};

//CONSTRUCTOR
Competition::Competition(char player, char opponentPlayer, State state)
{
    this->player = player;
    this->opponentPlayer = opponentPlayer;
    this->initialState = initialState;
    this->boardSize = (int)initialState.size();
    this->cutOffDepth = 5;
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

//PUBLIC FUNCTIONS
double Competition::UtilityFunction()
{
    return (10*FindPieceDifference() + 500*GetCornerOccupancy() + 300*GetCornerOccupancy() + 80*GetMobility() + 10*GetBoardWeights());
}

bool Competition::TerminalTest()
{
    if (AllCellsFilled(initialState)) return true;
    vector<Cell> actions = Actions();
    if (!actions.size()) return true;
    return false;
}

State Competition::Result(Cell action)
{
    initialState[action.row][action.column] = this->player;
    FlipPieces(initialState, action);
    return initialState;
}

vector<Cell> Competition::Actions()
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
        
    }while(x<boardSize && y<boardSize && opponentPiece.row>=0 && opponentPiece.column >= 0);
    sort(actions.begin(), actions.end(),cellFunction);
    actions.erase(unique(actions.begin(), actions.end(), cellFunction.UniqueFunction), actions.end());
    return actions;
}

State Competition::GetCurrentGameState(){ return this->initialState; }
int Competition::GetCutOffDepth(){ return this->cutOffDepth; }
char Competition::GetPlayer() { return player; }
char Competition::GetOpponentPlayer() { return opponentPlayer; }

//PRIVATE FUNCTIONS
bool Competition::AllCellsFilled(State state)
{
    for(int row=0; row<boardSize; row++)
        for (int column=0; column<boardSize; column++)
            if (state[row][column] == '*') return false;
    return true;
}

Cell Competition::FindOpponentPiece(int startX, int startY)
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

vector<Cell> Competition::FindMovesAdjacentToCell(Cell cell)
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

void Competition::FlipPieces(State& state, Cell cell)
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

float Competition::FindPieceDifference()
{
    int me = 0, opponent = 0;
    for (int row = 0; row<boardSize; row++)
    {
        for (int column = 0; column < boardSize; column++)
        {
            if (initialState[row][column] == player) me++;
            else if (initialState[row][column] == opponentPlayer) opponent++;
        }
    }
    
    if (me == opponent)
        return 0;
    else if (me > opponent)
        return 100 * ((float)me / (me + opponent));
    else
        return -100 * ((float)me / (me + opponent));

}

int Competition::GetBoardWeights()
{
    int playerWeights = 0, opponentWeights=0;
    for(int row=0; row<initialState.size(); row++)
    {
        for (int column=0; column<initialState[row].size(); column++)
        {
            if (initialState[row][column] == player) playerWeights += boardWeights[row][column];
            else if (initialState[row][column] == opponentPlayer) opponentWeights += boardWeights[row][column];
        }
    }
    return playerWeights - opponentWeights;
}

int Competition::GetCornerOccupancy()
{
    int playerWeights = 0, opponentWeights = 0;
    if (initialState[0][0] == player) playerWeights++;
    else if(initialState[0][0] == opponentPlayer) opponentWeights++;
    if (initialState[0][boardSize-1] == player) playerWeights++;
    else if(initialState[0][boardSize-1] == opponentPlayer) opponentWeights++;
    if (initialState[boardSize-1][0] == player) playerWeights++;
    else if(initialState[boardSize-1][0] == opponentPlayer) opponentWeights++;
    if (initialState[boardSize-1][boardSize-1] == player) playerWeights++;
    else if(initialState[boardSize-1][boardSize-1] == opponentPlayer) opponentWeights++;
    return 25*(playerWeights-opponentWeights);
}

double Competition::GetCornerClosenessOccupancy()
{
    //Checking near corner [0,0]
    int playerWeights = 0, opponentWeights = 0;
    if (initialState[0][1] == player) playerWeights++;
    else if(initialState[0][1] == opponentPlayer) opponentWeights++;
    if (initialState[1][1] == player) playerWeights++;
    else if(initialState[1][1] == opponentPlayer) opponentWeights++;
    if (initialState[1][0] == player) playerWeights++;
    else if(initialState[1][0] == opponentPlayer) opponentWeights++;

    //checkning near corner [0,boardSize-1]
    if (initialState[0][boardSize-2] == player) playerWeights++;
    else if(initialState[0][boardSize-2] == opponentPlayer) opponentWeights++;
    if (initialState[1][boardSize-2] == player) playerWeights++;
    else if(initialState[1][boardSize-2] == opponentPlayer) opponentWeights++;
    if (initialState[1][boardSize-1] == player) playerWeights++;
    else if(initialState[1][boardSize-1] == opponentPlayer) opponentWeights++;
    
    //checking near corner [boardsize-1,0]
    if (initialState[boardSize-1][1] == player) playerWeights++;
    else if(initialState[boardSize-1][1] == opponentPlayer) opponentWeights++;
    if (initialState[boardSize-2][1] == player) playerWeights++;
    else if(initialState[boardSize-2][1] == opponentPlayer) opponentWeights++;
    if (initialState[boardSize-2][0] == player) playerWeights++;
    else if(initialState[boardSize-2][0] == opponentPlayer) opponentWeights++;
    
    //checking the last corner
    if (initialState[boardSize-1][boardSize-2] == player) playerWeights++;
    else if(initialState[boardSize-1][boardSize-2] == opponentPlayer) opponentWeights++;
    if (initialState[boardSize-2][boardSize-2] == player) playerWeights++;
    else if(initialState[boardSize-2][boardSize-2] == opponentPlayer) opponentWeights++;
    if (initialState[boardSize-2][boardSize-1] == player) playerWeights++;
    else if(initialState[boardSize-2][boardSize-1] == opponentPlayer) opponentWeights++;
    
    return 12.5*(opponentWeights-playerWeights);
}

int Competition::GetMobility()
{
    int myMoves = (int)(Actions().size());
    Competition newGame = *new Competition(opponentPlayer, player, initialState);
    int opponentMoves = (int)(newGame.Actions().size());
    if(myMoves == opponentMoves) return 0;
    else if(myMoves > opponentMoves) return 100*(myMoves/(myMoves+opponentMoves));
    else return -100 * (opponentMoves/(opponentMoves+myMoves));
}
#endif
