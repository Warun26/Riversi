//
//  Output.h
//  Riversi
//
//  Created by Varun Varadarajan on 10/17/14.
//  Copyright (c) 2014 Varun Varadarajan. All rights reserved.
//

#ifndef Riversi_Output_h
#define Riversi_Output_h
using namespace std;
class Output
{
private:
    static ofstream outputFile;
    void WriteNextState(State nextState);
    void WriteMinimaxTraverseLog(vector<Node> traverseLog);
    void WriteAlphaBetaTraverseLog(vector<AlphaBetaNode> traverseLog);
    
public:
    Output(State nextState, string outputFileName="output.txt");
    Output(State nextState, vector<Node>traverseLog, string outputFileName="output.txt");
    Output(State nextState, vector<AlphaBetaNode>traverseLog, string outputFileName="output.txt");
};

//CONSTRUCTORS
Output::Output(State nextState, string outputFileName)
{
    char *fileName=new char[outputFileName.size()+1];
    fileName[outputFileName.size()]=0;
    memcpy(fileName,outputFileName.c_str(),outputFileName.size());
    outputFile.open(fileName);
    WriteNextState(nextState);
    outputFile.close();
}

Output::Output(State nextState, vector<Node>traverseLog, string outputFileName)
{
    char *fileName=new char[outputFileName.size()+1];
    fileName[outputFileName.size()]=0;
    memcpy(fileName,outputFileName.c_str(),outputFileName.size());
    outputFile.open(fileName);
    WriteNextState(nextState);
    WriteMinimaxTraverseLog(traverseLog);
    outputFile.close();
}

Output::Output(State nextState, vector<AlphaBetaNode>traverseLog, string outputFileName)
{
    char *fileName=new char[outputFileName.size()+1];
    fileName[outputFileName.size()]=0;
    memcpy(fileName,outputFileName.c_str(),outputFileName.size());
    outputFile.open(fileName);
    WriteNextState(nextState);
    WriteAlphaBetaTraverseLog(traverseLog);
    outputFile.close();
}

//PRIVATE FUNCTIONS

void Output::WriteNextState(State nextState)
{
    for (int row=0; row<nextState.size(); row++)
    {
        for (int column=0; column<nextState[row].size(); column++)
            outputFile<<nextState[row][column];
        outputFile<<endl;
    }
}
void Output::WriteMinimaxTraverseLog(vector<Node> traverseLog)
{
    for (int logEntry = 0; logEntry < traverseLog.size(); logEntry++)
    {
        outputFile<<traverseLog[logEntry].cell<<","<<traverseLog[logEntry].depth<<",";
        if (traverseLog[logEntry].value == static_cast<int>(Infinity::NegativeInfinity)) outputFile<<"-Infinity"<<endl;
        else if(traverseLog[logEntry].value == static_cast<int>(Infinity::PositiveInfinity)) outputFile<<"Infinity"<<endl;
        else outputFile<<traverseLog[logEntry].value<<endl;
    }
}

void Output::WriteAlphaBetaTraverseLog(vector<AlphaBetaNode> traverseLog)
{
    for (int logEntry = 0; logEntry < traverseLog.size(); logEntry++)
    {
        outputFile<<traverseLog[logEntry].cell<<","<<traverseLog[logEntry].depth<<",";
        if (traverseLog[logEntry].value == static_cast<int>(Infinity::NegativeInfinity)) outputFile<<"-Infinity"<<",";
        else if(traverseLog[logEntry].value == static_cast<int>(Infinity::PositiveInfinity))outputFile<<"Infinity"<<",";
        else outputFile<<traverseLog[logEntry].value<<",";
        
        if (traverseLog[logEntry].alpha == static_cast<int>(Infinity::NegativeInfinity)) outputFile<<"-Infinity"<<",";
        else if(traverseLog[logEntry].alpha == static_cast<int>(Infinity::PositiveInfinity))outputFile<<"Infinity"<<",";
        else outputFile<<traverseLog[logEntry].alpha<<",";
        
        if (traverseLog[logEntry].beta == static_cast<int>(Infinity::NegativeInfinity)) outputFile<<"-Infinity"<<endl;
        else if(traverseLog[logEntry].beta == static_cast<int>(Infinity::PositiveInfinity))outputFile<<"Infinity"<<endl;
        else outputFile<<traverseLog[logEntry].beta<<endl;
    }
}
#endif
