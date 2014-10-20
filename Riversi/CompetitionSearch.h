//
//  CompetitionSearch.h
//  Riversi
//
//  Created by Varun Varadarajan on 10/20/14.
//  Copyright (c) 2014 Varun Varadarajan. All rights reserved.
//

#ifndef Riversi_CompetitionSearch_h
#define Riversi_CompetitionSearch_h

#include <ctime>

class CompetitionSearch
{
private:
    clock_t tStart;
    double timeLimit;
    int currentDepthLimit;
    int maxDepthLimit;
    string AlphaBetaPruning(Competition competition);
    double MinValue(Competition competition, int depth, double alpha, double beta);
    double MaxValue(Competition competition, int depth, double alpha, double beta);
public:
     string IterativeDeepining(Competition competition)
    {
        tStart = clock();
        maxDepthLimit = 9;
        timeLimit = 1000;
        string nextMove = "pass";
        for (int depthLimit = 1; depthLimit<maxDepthLimit; depthLimit++)
        {
            currentDepthLimit = depthLimit;
            nextMove = AlphaBetaPruning(competition);
        }
        return nextMove;
    }
};

string CompetitionSearch::AlphaBetaPruning(Competition competition)
{
    string nextMove;
    vector<Cell> availableMoves = competition.Actions();
    int maxValue = NegativeInfinity;
    int minValue = PositiveInfinity;
    if (availableMoves.size()==0) return "pass";
    for (int move=0; move<availableMoves.size(); move++)
    {
        State newState = competition.Result(availableMoves[move]);
        Competition newGame = *new Competition(competition.GetPlayer(), competition.GetOpponentPlayer(), newState);
        string action(1,availableMoves[move].column + 'a');
        action += (availableMoves[move].row + 1) + '0';
        int value = MinValue(newGame, 0, maxValue, minValue);
        if (value > maxValue)
        {
            maxValue = value;
            nextMove = action;
        }
    }
    return nextMove;
}
double CompetitionSearch::MinValue(Competition game, int depth, double alpha, double beta)
{
    depth += 1;
    if (currentDepthLimit <= depth || game.TerminalTest() || (double)(clock() - tStart)/CLOCKS_PER_SEC >= timeLimit)
    {
        int utility = game.UtilityFunction();
        return utility;
    }
    double value = PositiveInfinity;
    Competition possibleGame = *new Competition(game.GetOpponentPlayer(), game.GetPlayer(), game.GetCurrentGameState());
    vector<Cell>availableMoves = possibleGame.Actions();
    for (int move=0; move<availableMoves.size(); move++)
    {
        State newState = possibleGame.Result(availableMoves[move]);
        Competition newGame = *new Competition(possibleGame.GetPlayer(), possibleGame.GetOpponentPlayer(), newState);
        value = min(value, MaxValue(newGame, depth, alpha, beta));
        if (value <= alpha)
        {
            return value;
        }
        beta = min(beta, value);
    }
    return value;
}

double CompetitionSearch::MaxValue(Competition game, int depth, double alpha, double beta)
{
    depth += 1;
    if (currentDepthLimit <= depth || game.TerminalTest() || (double)(clock() - tStart)/CLOCKS_PER_SEC >= timeLimit)
    {
        int utility = -1*game.UtilityFunction();
        return utility;
    }
    double value = NegativeInfinity;
    Competition possibleGame = *new Competition(game.GetOpponentPlayer(), game.GetPlayer(), game.GetCurrentGameState());
    vector<Cell>availableMoves = possibleGame.Actions();
    for (int move=0; move<availableMoves.size(); move++)
    {
        State newState = possibleGame.Result(availableMoves[move]);
        Competition newGame = *new Competition(possibleGame.GetPlayer(), possibleGame.GetOpponentPlayer(), newState);
        value = max(value, MinValue(newGame, depth, alpha, beta));
        if (value >= beta)
        {
            return value;
        }
        alpha = max(alpha, value);
    }
    return value;
}

#endif
