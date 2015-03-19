/*******************************************************************
 *  File:        scp.h
 *  Author:      Lishan Li                                        
 *  Date:        May, 2003                                                 
 *
 *  Description: This SubConsensusPair class is used to create 
 *               SubConsensusPair objects for ConsensusBuilder class
 *******************************************************************/

#ifndef SCP_H
#define SCP_H

#include <iostream>

using namespace std;

class SubConsensusPair  
{
public:
	int cid1;       // first consensus's ID
	int cid2;		// second consensus's ID
	int score;      // overlap score between two consensuses

	SubConsensusPair() { }                     // default constructor
	SubConsensusPair(int id1, int id2, int s)  // user defined constructor
	{
		cid1 = id1;
		cid2 = id2;
		score = s;
	}
	virtual ~SubConsensusPair() { }             // destructor

	bool operator<(const SubConsensusPair& source) const
	{
		if(score < source.score)
			return true;
		else
			return false;
	}
	/* overload operator<< for printing the contents of a SubConsensusPair object */
	friend ostream& operator<<(ostream& os, const SubConsensusPair &info)
	{
		os << "(" << info.cid1 << ", " << info.cid2 << ", " << info.score << ")" << endl;
		return os;
	}
};

	/* overload operator<< for printing the contents of a SubConsensusPair object */
	//ostream& operator<<(ostream& os, const SubConsensusPair& info)

#endif 

/************************************ End of File **************************************/
