/*********************************************************************
 *  File:        AlignedPair.h
 *  Author:      Lishan Li                                        
 *  Date:        May, 2003                                                 
 *
 *  Description: AlignedPair class is a helper class that wraps an    
 *               alignment of two sequences
 *********************************************************************/

#ifndef ALIGNEDPAIR_H
#define ALIGNEDPAIR_H

#include <iostream>
#include <string>

using namespace std;

class AlignedPair
{
public:
	string s;  // to store sequence s after alignment
	string t;  // to store sequence t after alignment

	AlignedPair() {}
	AlignedPair(const string &s, const string &t)
	{
		this->s = s;
		this->t = t;
	}

	void printAlign()
	{
		cout << s << "\n" << t << endl << endl;
	}
};

#endif

/************************************ End of File **************************************/
