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
    Input input("/Users/varun/Documents/Artificial Intelligence/Riversi/Input/input.txt");
    Game game = *new Game(input.GetPlayer(), input.GetOpponentPlayer(), input.GetBoard(), input.GetCutOffDepth());
    //vector<vector<char> > nextState = AdversialSearch::GreedySearch(game);
    vector<Node> traverseLog;
    vector<AlphaBetaNode> traverse;
    //State nextState = AdversialSearch::MiniMax(game, traverseLog);
    State nextState = AdversialSearch::AlphaBetaSearch(game, traverse);
    return 0;
}
