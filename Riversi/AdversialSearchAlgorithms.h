//
//  AdversialSearchAlgorithms.h
//  Riversi
//
//  Created by Varun Varadarajan on 10/11/14.
//  Copyright (c) 2014 Varun Varadarajan. All rights reserved.
//

#ifndef Riversi_AdversialSearchAlgorithms_h
#define Riversi_AdversialSearchAlgorithms_h

using namespace std;

enum Infinity
{
    PositiveInfinity = 65535,
    NegativeInfinity = -65535
};

struct Node
{
    string cell;
    int depth;
    int value;
    Node(string cellName, int depth, int value)
    {
        this->cell = cellName;
        this->depth = depth;
        this->value = value;
    }
};

struct AlphaBetaNode
{
    string cell;
    int depth;
    int value;
    int alpha;
    int beta;
    AlphaBetaNode(string cellName, int depth, int value, int alpha, int beta)
    {
        this->cell = cellName;
        this->depth = depth;
        this->value = value;
        this->alpha = alpha;
        this->beta = beta;
    }
};

class AdversialSearch
{
private:
    static int MinValue(Game game, string action, int depth, vector<Node>& traverseLog);
    static int MaxValue(Game game, string action, int depth, vector<Node>& traverseLog);
    static int PruneMinValue(Game game, string action, int depth, int alpha, int beta, vector<AlphaBetaNode>& traverseLog);
    static int PruneMaxValue(Game game, string action, int depth, int alpha, int beta, vector<AlphaBetaNode>& traverseLog);
public:
    static State GreedySearch(Game game);
    static State MiniMax(Game game, vector<Node>& traverseLog);
    static State AlphaBetaSearch(Game game, vector<AlphaBetaNode>& traverseLog);
};

//PUBLIC FUNCTIONS
State AdversialSearch::GreedySearch(Game game)
{
    State nextState = game.GetCurrentGameState();
    vector<Cell> availableMoves = game.Actions();
    int maxUtility = NegativeInfinity;
    for (int move=0; move<availableMoves.size(); move++)
    {
        Game newGame = *new Game(game);
        State newState = newGame.Result(newGame.GetCurrentGameState(), availableMoves[move]);
        int payOff = newGame.UtilityFunction(newState, newGame.GetPlayer());
        if(payOff > maxUtility)
        {
            maxUtility = payOff;
            nextState = newState;
        }
    }
    return nextState;
}

State AdversialSearch::MiniMax(Game game, vector<Node>& traverseLog)
{
    State nextState;
    vector<Cell> availableMoves = game.Actions();
    if (game.TerminalTest())
    {
        int utility = game.UtilityFunction();
        traverseLog.push_back(Node("root", 0, utility));
        return game.GetCurrentGameState();
    }
    int maxValue = NegativeInfinity;
    traverseLog.push_back(Node("root", 0, maxValue));
    if (availableMoves.size()==0)
    {
        nextState = game.GetCurrentGameState();
        Game newGame = *new Game(game.GetPlayer(), game.GetOpponentPlayer(), game.GetCurrentGameState(), game.GetCutOffDepth());
        string action ="pass";
        int value = MinValue(newGame, action, 0, traverseLog);
        if (value > maxValue) maxValue = value;
        traverseLog.push_back(Node("root", 0, maxValue));
    }
    for (int move=0; move<availableMoves.size(); move++)
    {
        State newState = game.Result(game.GetCurrentGameState(), availableMoves[move]);
        Game newGame = *new Game(game.GetPlayer(), game.GetOpponentPlayer(), newState, game.GetCutOffDepth());
        string action(1,availableMoves[move].column + 'a');
        action += (availableMoves[move].row+1) + '0';
        int value = MinValue(newGame, action, 0, traverseLog);
        if (value > maxValue)
        {
            maxValue = value;
            nextState = newState;
        }
        traverseLog.push_back(Node("root", 0, maxValue));
    }
    return nextState;
}

State AdversialSearch::AlphaBetaSearch(Game game, vector<AlphaBetaNode>& traverseLog)
{
    State nextState;
    vector<Cell> availableMoves = game.Actions();
    int maxValue = NegativeInfinity;
    int minValue = PositiveInfinity;
    if (game.TerminalTest())
    {
        int utility = game.UtilityFunction();
        traverseLog.push_back(AlphaBetaNode("root", 0, utility, maxValue, minValue));
        return game.GetCurrentGameState();
    }
    traverseLog.push_back(AlphaBetaNode("root", 0, maxValue, maxValue, minValue));
    if (availableMoves.size()==0)
    {
        nextState = game.GetCurrentGameState();
        Game newGame = *new Game(game.GetPlayer(), game.GetOpponentPlayer(), game.GetCurrentGameState(), game.GetCutOffDepth());
        string action ="pass";
        int value = PruneMinValue(newGame, action, 0, maxValue, minValue, traverseLog);
        if (value > maxValue) maxValue = value;
        traverseLog.push_back(AlphaBetaNode("root", 0, maxValue, maxValue, minValue));
    }
    for (int move=0; move<availableMoves.size(); move++)
    {
        State newState = game.Result(game.GetCurrentGameState(), availableMoves[move]);
        Game newGame = *new Game(game.GetPlayer(), game.GetOpponentPlayer(), newState, game.GetCutOffDepth());
        string action(1,availableMoves[move].column + 'a');
        action += (availableMoves[move].row + 1) + '0';
        int value = PruneMinValue(newGame, action, 0, maxValue, minValue, traverseLog);
        if (value > maxValue)
        {
            maxValue = value;
            nextState = newState;
        }
        traverseLog.push_back(AlphaBetaNode("root", 0, maxValue, maxValue, minValue));
    }
    return nextState;
}

//PRIVATE FUNCTIONS
int AdversialSearch::MinValue(Game game, string action, int depth, vector<Node>& traverseLog)
{
    depth += 1;
    Game possibleGame = *new Game(game.GetOpponentPlayer(), game.GetPlayer(), game.GetCurrentGameState(), game.GetCutOffDepth());
    vector<Cell>availableMoves = possibleGame.Actions();
    if (game.GetCutOffDepth() <= depth || game.TerminalTest() || (availableMoves.size() == 0 && traverseLog[traverseLog.size()-1].cell.compare("pass") == 0))
    {
        int utility = game.UtilityFunction();
        traverseLog.push_back(Node(action, depth, utility));
        return utility;
    }
    int value = PositiveInfinity;
    traverseLog.push_back(Node(action, depth, value));
    if (availableMoves.size() == 0)
    {
        State newState = possibleGame.GetCurrentGameState();
        Game newGame = *new Game(possibleGame.GetPlayer(), possibleGame.GetOpponentPlayer(), newState, possibleGame.GetCutOffDepth());
        string newAction = "pass";
        value = min(value, MaxValue(newGame, newAction, depth, traverseLog));
        traverseLog.push_back(Node(action, depth, value));
    }
    for (int move=0; move<availableMoves.size(); move++)
    {
        State newState = possibleGame.Result(availableMoves[move]);
        Game newGame = *new Game(possibleGame.GetPlayer(), possibleGame.GetOpponentPlayer(), newState, possibleGame.GetCutOffDepth());
        string newAction(1,availableMoves[move].column + 'a');
        newAction += (availableMoves[move].row+1) +'0';
        value = min(value, MaxValue(newGame, newAction, depth, traverseLog));
        traverseLog.push_back(Node(action, depth, value));
    }
    return value;
}

int AdversialSearch::MaxValue(Game game, string action, int depth, vector<Node>& traverseLog)
{
    depth += 1;
    Game possibleGame = *new Game(game.GetOpponentPlayer(), game.GetPlayer(), game.GetCurrentGameState(), game.GetCutOffDepth());
    vector<Cell>availableMoves = possibleGame.Actions();
    if (game.GetCutOffDepth() <= depth || game.TerminalTest() || (availableMoves.size() == 0 && traverseLog[traverseLog.size()-1].cell.compare("pass") == 0))
    {
        int utility = -1*game.UtilityFunction();
        traverseLog.push_back(Node(action, depth, utility));
        return utility;
    }
    int value = NegativeInfinity;
    traverseLog.push_back(Node(action, depth, value));
    if (availableMoves.size() == 0)
    {
        State newState = possibleGame.GetCurrentGameState();
        Game newGame = *new Game(possibleGame.GetPlayer(), possibleGame.GetOpponentPlayer(), newState, possibleGame.GetCutOffDepth());
        string newAction = "pass";
        value = max(value, MinValue(newGame, newAction, depth, traverseLog));
        traverseLog.push_back(Node(action, depth, value));
    }
    for (int move=0; move<availableMoves.size(); move++)
    {
        State newState = possibleGame.Result(availableMoves[move]);
        Game newGame = *new Game(possibleGame.GetPlayer(), possibleGame.GetOpponentPlayer(), newState, possibleGame.GetCutOffDepth());
        string newAction(1,availableMoves[move].column + 'a');
        newAction += (availableMoves[move].row+1) + '0';
        value = max(value, MinValue(newGame, newAction, depth, traverseLog));
        traverseLog.push_back(Node(action, depth, value));
    }
    return value;
}

int AdversialSearch::PruneMinValue(Game game, string action, int depth, int alpha, int beta, vector<AlphaBetaNode>& traverseLog)
{
    depth += 1;
    Game possibleGame = *new Game(game.GetOpponentPlayer(), game.GetPlayer(), game.GetCurrentGameState(), game.GetCutOffDepth());
    vector<Cell>availableMoves = possibleGame.Actions();
    if (game.GetCutOffDepth() <= depth || game.TerminalTest() || (availableMoves.size() == 0 && traverseLog[traverseLog.size()-1].cell.compare("pass") == 0))
    {
        int utility = game.UtilityFunction();
        traverseLog.push_back(AlphaBetaNode(action, depth, utility, alpha, beta));
        return utility;
    }
    int value = PositiveInfinity;
    traverseLog.push_back(AlphaBetaNode(action, depth, value, alpha, beta));
    if (availableMoves.size() == 0)
    {
        State newState = possibleGame.GetCurrentGameState();
        Game newGame = *new Game(possibleGame.GetPlayer(), possibleGame.GetOpponentPlayer(), newState, possibleGame.GetCutOffDepth());
        string newAction = "pass";
        value = min(value, PruneMaxValue(newGame, newAction, depth, alpha, beta, traverseLog));
        if (value <= alpha) {
            traverseLog.push_back(AlphaBetaNode(action , depth, value, alpha, beta));
            return value;
        }
        beta = min(beta, value);
        traverseLog.push_back(AlphaBetaNode(action, depth, value, alpha, beta));
    }
    for (int move=0; move<availableMoves.size(); move++)
    {
        State newState = possibleGame.Result(availableMoves[move]);
        Game newGame = *new Game(possibleGame.GetPlayer(), possibleGame.GetOpponentPlayer(), newState, possibleGame.GetCutOffDepth());
        string newAction(1,availableMoves[move].column + 'a');
        newAction += (availableMoves[move].row+1) + '0';
        value = min(value, PruneMaxValue(newGame, newAction, depth, alpha, beta, traverseLog));
        if (value <= alpha)
        {
            traverseLog.push_back(AlphaBetaNode(action, depth, value, alpha, beta));
            return value;
        }
        beta = min(beta, value);
        traverseLog.push_back(AlphaBetaNode(action, depth, value, alpha, beta));
    }
    return value;
}

int AdversialSearch::PruneMaxValue(Game game, string action, int depth, int alpha, int beta, vector<AlphaBetaNode>& traverseLog)
{
    depth += 1;
    Game possibleGame = *new Game(game.GetOpponentPlayer(), game.GetPlayer(), game.GetCurrentGameState(), game.GetCutOffDepth());
    vector<Cell>availableMoves = possibleGame.Actions();
    if (game.GetCutOffDepth() <= depth || game.TerminalTest() || (availableMoves.size() == 0 && traverseLog[traverseLog.size()-1].cell.compare("pass") == 0))
    {
        int utility = -1*game.UtilityFunction();
        traverseLog.push_back(AlphaBetaNode(action, depth, utility, alpha, beta));
        return utility;
    }
    int value = NegativeInfinity;
    traverseLog.push_back(AlphaBetaNode(action, depth, value, alpha, beta));
    if (availableMoves.size() == 0)
    {
        State newState = possibleGame.GetCurrentGameState();
        Game newGame = *new Game(possibleGame.GetPlayer(), possibleGame.GetOpponentPlayer(), newState, possibleGame.GetCutOffDepth());
        string newAction = "pass";
        value = max(value, PruneMinValue(newGame, newAction, depth, alpha, beta, traverseLog));
        if (value >= beta) {
            traverseLog.push_back(AlphaBetaNode(action , depth, value, alpha, beta));
            return value;
        }
        alpha = max(alpha, value);
        traverseLog.push_back(AlphaBetaNode(action, depth, value, alpha, beta));
    }
    for (int move=0; move<availableMoves.size(); move++)
    {
        State newState = possibleGame.Result(availableMoves[move]);
        Game newGame = *new Game(possibleGame.GetPlayer(), possibleGame.GetOpponentPlayer(), newState, possibleGame.GetCutOffDepth());
        string newAction(1,availableMoves[move].column + 'a');
        newAction += (availableMoves[move].row+1) + '0';
        value = max(value, PruneMinValue(newGame, newAction, depth, alpha, beta, traverseLog));
        if (value >= beta)
        {
            traverseLog.push_back(AlphaBetaNode(action, depth, value, alpha, beta));
            return value;
        }
        alpha = max(alpha, value);
        traverseLog.push_back(AlphaBetaNode(action, depth, value, alpha, beta));
    }
    return value;
}

#endif
