/*********************************************************************
 *  File:        AdjKeys.h
 *  Author:      Lishan Li                                        
 *  Date:        May, 2003                                                 
 *
 *  Description: AdjKeys class is a helper class for Edge-recombination   
 *               crossover operator
 *********************************************************************/

#ifndef ADJKEYS_H
#define ADJKEYS_H

#include <iostream>
#include <string>

using namespace std;

class AdjKeys
{
public:
	int startKey;
	int * offSpring;      // store offspring generate by crossover operator

	AdjKeys(int * parent1, int * parent2, int nbOfFrag);
	~AdjKeys();
	void makeAdjKeysTable(int * parent1, int * parent2);
	void initAdjacentcy(int * parent, int pid);
	void recombine();
	void deleteTakenKey(int takenKey);
	int getRemaining(int id);
	void setSharing();
	bool hasSharing(int id);
	void decrementRemaining(int id);
	void incrementRemaining(int id);
	void printInfo();
private:
	int ** aKeys;        // store adjacent keys for both parents
	bool * share;        // store sharing info for both parents
	int * remaining;     // store remaining fragments
	int nof;
};

#endif

/************************************ End of File **************************************/
