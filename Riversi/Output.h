//
//  Output.h
//  Riversi
//
//  Created by Varun Varadarajan on 10/17/14.
//  Copyright (c) 2014 Varun Varadarajan. All rights reserved.
//

#ifndef Riversi_Output_h
#define Riversi_Output_h
#include <fstream>
#include <string>
#include <vector>
using namespace std;
class Input
{
private:
    ofstream outputFile;
    void WriteNextState();
    void WriteTraverseLog();
    
public:
    Output(string inputFileName="Output.txt", State nextState);
    Output(string inputFileName="Input.txt", State nextState, vector<Node>traverseLog);
    Output(string inputFileName="Input.txt", State nextState, vector<AlphaBetaNode>traverseLog);
};


#endif
