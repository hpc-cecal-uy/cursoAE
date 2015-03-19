/**************************************************************
 *  File:        Util.h													    
 *  Author:      Lishan Li                                        
 *  Date:        May, 2003                                                 
 *
 *  Description: This Util class consists of utility functions
 *               used by other classes
 **************************************************************/

#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>  
#include <vector>

using namespace std;

class Util
{
public:
	/* returns a randomly generated floating point number ranged [min, max] */
//	static double randDouble(double min, double max)
//	{
//	  return (1.0*rand()*(max-min)/RAND_MAX + min);
//	}

	/* returns a randomly generated interge number ranged [min, max] */
//	static int randInt(int min, int max)
//	{
//		return (int)randDouble(min, max);
//	}

	/* returns a string that is the reverse of a given DNA sequence */
	static string revSequence(const string &s)
	{
		string revSeq = s;
		int j = 0;
		for(int i = s.length()-1; i >= 0; i--)
			revSeq[j++] = s.at(i);

		return revSeq;
	}

	/* returns a string that is the complement of a given DNA sequence */
	static string complSequence(const string &s)
	{
		string compSeq = s;
		int j = 0;
		for(unsigned int i = 0; i < s.length(); i++)
		{
			if(s.at(i) == 'A')
				compSeq[j++] = 'T';
			else if(s.at(i) == 'T')
				compSeq[j++] = 'A';
			else if(s.at(i) == 'C')
				compSeq[j++] = 'G';
			else if(s.at(i) == 'G')
				compSeq[j++] = 'C';
			else
				compSeq[j++] = s.at(i);
		}
		return compSeq;
	}

	/* returns a string that is the reverse complement of a given DNA sequence */
	static string rcSequence(const string &s)
	{
		return complSequence(revSequence(s));
	}
};
#endif
