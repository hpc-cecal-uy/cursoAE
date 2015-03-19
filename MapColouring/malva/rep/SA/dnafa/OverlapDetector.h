/***************************************************************
 *  File:        OverlapDetector.h
 *  Author:      Lishan Li                                        
 *  Date:        May, 2003                                                
 *
 *  Description: This OverlapDetector class is used to detect the 
 *               similarity of all possible pair of fragments. The 
 *               overlap length between any two fragments is stored
 *               in a two dimentional array called score.
 ***************************************************************/

#ifndef OVERLAPDETECTOR_H
#define OVERLAPDETECTOR_H

#pragma warning(disable: 4786)

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <vector>

#include "sga.h"

using namespace std;


class OverlapDetector
{
public:
	OverlapDetector(int nof);        // constructor
	~OverlapDetector();       // destructor
	void buildScoresTable(vector<string>*, vector<string>*);  // build overlap score table for fragments
	void readScoreTable();
	void printScoreTable();
	int ** getScoreTable() { return score; }
private:
	int ** score;             // to store the overlap length of all possible pair of fragments
	int nbOfFrag;			  // Number of Fragments
};

#endif

/************************************ End of File **************************************/
