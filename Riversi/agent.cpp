//
//  main.cpp
//  Riversi
//
//  Created by Varun Varadarajan on 10/10/14.
//  Copyright (c) 2014 Varun Varadarajan. All rights reserved.
//

#include <iostream>
#include "Input.h"
#include "Game.h"
#include "Competition.h"
#include "AdversialSearchAlgorithms.h"
#include "CompetitionSearch.h"
#include "Output.h"

int main(int argc, const char * argv[])
{
    Input input = *new Input("/Users/varun/Documents/Artificial Intelligence/Riversi/Riversi/input.txt");
    Game game = *new Game(input.GetPlayer(), input.GetOpponentPlayer(), input.GetBoard(), input.GetCutOffDepth());
    switch (input.GetSearchAlgorithm())
    {
        case 1:
        {
            State nextState = AdversialSearch::GreedySearch(game);
            Output output(nextState);
            break;
        }
        case 2:
        {
            vector<Node> traverseLog;
            State nextState = AdversialSearch::MiniMax(game, traverseLog);
            Output output(nextState, traverseLog);
            break;
        }
        case 3:
        {
            vector<AlphaBetaNode> traverseLog;
            State nextState = AdversialSearch::AlphaBetaSearch(game, traverseLog);
            Output output(nextState, traverseLog);
            break;
        }
        case 4:
        {
            Competition competition(input.GetPlayer(), input.GetOpponentPlayer(), input.GetBoard());
            string nextMove = CompetitionSearch().IterativeDeepining(competition);
            Output output(nextMove);
            break;
        }
        default:
            break;
    }
    return 0;
}
