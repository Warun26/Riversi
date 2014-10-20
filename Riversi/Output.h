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
    ofstream outputFile;
    void WriteNextState(State nextState);
    void WriteMinimaxTraverseLog(vector<Node> traverseLog);
    void WriteAlphaBetaTraverseLog(vector<AlphaBetaNode> traverseLog);
    
public:
    Output(State nextState, string outputFileName="output.txt");
    Output(State nextState, vector<Node>traverseLog, string outputFileName="output.txt");
    Output(State nextState, vector<AlphaBetaNode>traverseLog, string outputFileName="output.txt");
    Output(string nextMove, string outputFileName="output.txt");
};

//CONSTRUCTORS
Output::Output(State nextState, string outputFileName)
{
    char *fileName=new char[outputFileName.size()+1];
    fileName[outputFileName.size()]=0;
    memcpy(fileName,outputFileName.c_str(),outputFileName.size());
    this->outputFile.open(fileName);
    WriteNextState(nextState);
    this->outputFile.close();
}

Output::Output(State nextState, vector<Node>traverseLog, string outputFileName)
{
    char *fileName=new char[outputFileName.size()+1];
    fileName[outputFileName.size()]=0;
    memcpy(fileName,outputFileName.c_str(),outputFileName.size());
    this->outputFile.open(fileName);
    WriteNextState(nextState);
    WriteMinimaxTraverseLog(traverseLog);
    this->outputFile.close();
}

Output::Output(State nextState, vector<AlphaBetaNode>traverseLog, string outputFileName)
{
    char *fileName=new char[outputFileName.size()+1];
    fileName[outputFileName.size()]=0;
    memcpy(fileName,outputFileName.c_str(),outputFileName.size());
    this->outputFile.open(fileName);
    WriteNextState(nextState);
    WriteAlphaBetaTraverseLog(traverseLog);
    this->outputFile.close();
}

Output::Output(string nextMove, string outputFileName)
{
    char *fileName=new char[outputFileName.size()+1];
    fileName[outputFileName.size()]=0;
    memcpy(fileName,outputFileName.c_str(),outputFileName.size());
    this->outputFile.open(fileName);
    outputFile<<nextMove;
    this->outputFile.close();
}
//PRIVATE FUNCTIONS

void Output::WriteNextState(State nextState)
{
    for (int row=0; row<nextState.size(); row++)
    {
        for (int column=0; column<nextState[row].size(); column++)
            this->outputFile<<nextState[row][column];
        this->outputFile<<endl;
    }
}
void Output::WriteMinimaxTraverseLog(vector<Node> traverseLog)
{
    int logEntry;
    this->outputFile<<"Node,Depth,Value"<<endl;
    for (logEntry = 0; logEntry < traverseLog.size()-1; logEntry++)
    {
        this->outputFile<<traverseLog[logEntry].cell<<","<<traverseLog[logEntry].depth<<",";
        if (traverseLog[logEntry].value == NegativeInfinity) this->outputFile<<"-Infinity"<<endl;
        else if(traverseLog[logEntry].value == PositiveInfinity) this->outputFile<<"Infinity"<<endl;
        else this->outputFile<<traverseLog[logEntry].value<<endl;
    }
    //Hack to prevent new line at the end of file
    this->outputFile<<traverseLog[logEntry].cell<<","<<traverseLog[logEntry].depth<<",";
    if (traverseLog[logEntry].value == NegativeInfinity) this->outputFile<<"-Infinity";
    else if(traverseLog[logEntry].value == PositiveInfinity) this->outputFile<<"Infinity";
    else this->outputFile<<traverseLog[logEntry].value;
}

void Output::WriteAlphaBetaTraverseLog(vector<AlphaBetaNode> traverseLog)
{
    int logEntry;
    this->outputFile<<"Node,Depth,Value,Alpha,Beta"<<endl;
    for (logEntry = 0; logEntry < traverseLog.size()-1; logEntry++)
    {
        this->outputFile<<traverseLog[logEntry].cell<<","<<traverseLog[logEntry].depth<<",";
        if (traverseLog[logEntry].value == NegativeInfinity) this->outputFile<<"-Infinity"<<",";
        else if(traverseLog[logEntry].value == PositiveInfinity) this->outputFile<<"Infinity"<<",";
        else this->outputFile<<traverseLog[logEntry].value<<",";
        
        if (traverseLog[logEntry].alpha == NegativeInfinity) this->outputFile<<"-Infinity"<<",";
        else if(traverseLog[logEntry].alpha == PositiveInfinity) this->outputFile<<"Infinity"<<",";
        else this->outputFile<<traverseLog[logEntry].alpha<<",";
        
        if (traverseLog[logEntry].beta == NegativeInfinity) outputFile<<"-Infinity"<<endl;
        else if(traverseLog[logEntry].beta == PositiveInfinity)outputFile<<"Infinity"<<endl;
        else this->outputFile<<traverseLog[logEntry].beta<<endl;
    }
    this->outputFile<<traverseLog[logEntry].cell<<","<<traverseLog[logEntry].depth<<",";
    if (traverseLog[logEntry].value == NegativeInfinity) this->outputFile<<"-Infinity"<<",";
    else if(traverseLog[logEntry].value == PositiveInfinity) this->outputFile<<"Infinity"<<",";
    else this->outputFile<<traverseLog[logEntry].value<<",";
    
    if (traverseLog[logEntry].alpha == NegativeInfinity) this->outputFile<<"-Infinity"<<",";
    else if(traverseLog[logEntry].alpha == PositiveInfinity) this->outputFile<<"Infinity"<<",";
    else this->outputFile<<traverseLog[logEntry].alpha<<",";
    
    if (traverseLog[logEntry].beta == NegativeInfinity) outputFile<<"-Infinity";
    else if(traverseLog[logEntry].beta == PositiveInfinity)outputFile<<"Infinity";
    else this->outputFile<<traverseLog[logEntry].beta;
}
#endif
