/***************************************************************
 *  File:        sga.h
 *  Author:      Lishan Li                                        
 *  Date:        May, 2003                                                 
 *
 *  Description: This SGA class is used to do pairwise alignment  
 *               between two fragments
 ***************************************************************/

#ifndef SGA_H
#define SGA_H

#include <iostream>
#include <string>
using namespace std;

class SGA
{
public:
	string align_s;  // to store sequence s after alignment
	string align_t;  // to store sequence t after alignment

	SGA() {}                                 // default constructor
	SGA(const string &is, const string &it); // user defined constructor
	~SGA();                                  // destructor
	void buildMatrix();
	int p(int i, int j);
	void align();
	void findBestScore();
	void printMatrix();
	void printAlign();
	void printInfo();
	int getBestRow()   { return bestRow;   }
	int getBestCol()   { return bestCol;   }
	int getBestScore() { return bestScore; }
private:
	int max(int x, int y, int z);
	void CalculateBest(int i, int j, int score);
	string s, t;
	int g;         // gap
	int m;         // length of s
	int n;         // length of t
	int bestRow;   // the row containning best score
	int bestCol;   // the column containing best score
	int bestScore; // the best score of the semiglobal alignment
//	int ** score;  // to store the scores for pairwise alignment 
	int overlapType;
};

#endif

/************************************ End of File **************************************/
