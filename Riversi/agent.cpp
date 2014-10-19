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
#include "AdversialSearchAlgorithms.h"
#include "Output.h"

int main(int argc, const char * argv[])
{
    Input input = *new Input("/Users/varun/Downloads/examplev2/Case4 Multiple pass states/input.txt");
    Game game = *new Game(input.GetPlayer(), input.GetOpponentPlayer(), input.GetBoard(), input.GetCutOffDepth());
    switch (input.GetSearchAlgorithm())
    {
        case 1:
        {
            State nextState = AdversialSearch::GreedySearch(game);
            Output output(nextState, "/Users/varun/Downloads/output.txt");
            break;
        }
        case 2:
        {
            vector<Node> traverseLog;
            State nextState = AdversialSearch::MiniMax(game, traverseLog);
            Output output(nextState, traverseLog, "/Users/varun/Downloads/output.txt");
            break;
        }
        case 3:
        {
            vector<AlphaBetaNode> traverseLog;
            State nextState = AdversialSearch::AlphaBetaSearch(game, traverseLog);
            Output output(nextState, traverseLog, "/Users/varun/Downloads/output.txt");
            break;
        }
        default:
            break;
    }
    return 0;
}
