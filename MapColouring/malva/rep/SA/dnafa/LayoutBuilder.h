/*********************************************************************
 *  File:        LayoutBuilder.h
 *  Author:      Lishan Li                                        
 *  Date:        May, 2003                                                 
 *
 *  Description: This LayoutBuilder class builds a layout for a contig.
 *               The contig consists of an ordered set of contiguous 
 *               overlapping fragments.
 *********************************************************************/

#ifndef LAYOUTBUILDER_H
#define LAYOUTBUILDER_H

#pragma warning(disable: 4786)

#include <iostream>
#include <string>
#include <vector>
#include "sga.h"
#include "AlignedPair.h"

using namespace std;

class LayoutBuilder
{
public:
	LayoutBuilder(const vector<string> & frags);  // constructor
	~LayoutBuilder();                             // destructor
	void findAlignments();                        // compute the alignments using SGA
	void buildLayout();                           // progressively build layout
	void insertStartGaps(int id, int numOfGaps);  // insert '-' at the beginning of fragments
	void insertMiddleGaps(const string &str1, const string &str2, int k, int prev);
	void insertEndGaps(int id, int numOfGaps);    // insert '-' at the end of fragments
	string extractMiddleSeq(const string &str);
	bool isEndGap(int i, int col);
	void findMajority();                          // find majorities of the layout
	char majorityVote(int col);                   // find majority of a particular column
	void findConsensus();                           
	void printLayout();
	void printConsensus();
	void printAlignments();
	string getConsensus() { return consensus; }
private:
	vector<string> preLayoutFrags;               // ordered fragments
	AlignedPair * semiAlign;                     // store pairwise alignment result
	string * fragmentsLayout;                    // store layout
	string majority;
	string consensus;
};

#endif

/************************************ End of File **************************************/
