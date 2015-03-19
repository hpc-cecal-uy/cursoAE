/***********************************************************************
 *  File:        AdjKeys.cpp
 *  Author:      Lishan Li                                        
 *  Date:        May, 2003                                                
 *
 *  Description: It provides implementation of all the functions declared   
 *               in the header file AdjKeys.h.
 ***********************************************************************/

#include "AdjKeys.h"

/*********************************************
 * Purpose: allocate memory and initialization
 *********************************************/

AdjKeys::AdjKeys(int * parent1, int * parent2, int nbOfFrag):nof(nbOfFrag)
{
	offSpring = new int[nof];
	share = new bool[nof];
	remaining = new int[nof];

	aKeys = new int *[nof];
	for(int i = 0; i < nof; i++)
	{
		aKeys[i] = new int[4];    // [0] and [1] for parent1; [2] and [3] for parent2
		for(int j = 0; j < 4; j++)
			aKeys[i][j] = -1;     // initialize to -1
	}

	for(int k = 0; k < nof; k++)
	{
		offSpring[k] = -1;
		share[k] = false;
		remaining[k] = 0;
	}
	startKey = parent1[0];        // start key point to the first fragment of parent1
	makeAdjKeysTable(parent1, parent2);
}

/*************************
 * Purpose: release memory 
 *************************/

AdjKeys::~AdjKeys()
{
	for(int i = 0; i < nof; i++)
		delete [] aKeys[i];	
	
	delete [] aKeys;
	delete [] remaining;
	delete [] share;
	delete [] offSpring;
}

/****************************************************
 * Purpose: create an adjacency table for two parents
 ****************************************************/

void AdjKeys::makeAdjKeysTable(int * parent1, int * parent2)
{
	initAdjacentcy(parent1, 1);
	initAdjacentcy(parent2, 2);

	setSharing();
//	printInfo();
}

/*******************************************
 * Purpose: initialize a parent's adjacency
 *******************************************/

void AdjKeys::initAdjacentcy(int * parent, int pid)
{
	int indStart;
	if(pid == 1)
		indStart = 0; // because aKeys[][0] and aKeys[][1] for parent1
	else
		indStart = 2; // because aKeys[][2] and aKeys[][3] for parent2

	int m = 0;
	for(int i = 0; i < nof; i++)
	{
		int key = parent[m];

		if(m == 0)  // deal with the first fragment
		{
			aKeys[key][indStart] = parent[m+1];
			incrementRemaining(key);
		}
		else if(m == nof-1) // deal with the last fragment
		{
			aKeys[key][indStart] = parent[m-1];
			incrementRemaining(key);
		}
		else     // deal with the middle fragments
		{
			aKeys[key][indStart] = parent[m-1];
			incrementRemaining(key);
			aKeys[key][indStart+1] = parent[m+1];
			incrementRemaining(key);
		}
		m++;
	}
}

/***********************************************
 * Purpose: return number of remaining fragments 
 ***********************************************/

int AdjKeys::getRemaining(int id)
{
	return remaining[id];
}

/**************************************************
 * Purpose: decrement number of remaining fragments 
 **************************************************/

void AdjKeys::decrementRemaining(int id)
{
	remaining[id]--;
}

/**************************************************
 * Purpose: increment number of remaining fragments 
 **************************************************/

void AdjKeys::incrementRemaining(int id)
{
	remaining[id]++;
}

/****************************************************
 * Purpose: set the share to be true if the fragments
 *          are ajdacent to both parents.
 ****************************************************/

void AdjKeys::setSharing()
{
	for(int i = 0; i < nof; i++)
	{
		if(aKeys[i][0] == aKeys[i][2] || aKeys[i][0] == aKeys[i][3])
		{
			share[i] = true;
			remaining[i]--;
		}
		if(aKeys[i][1] != -1)
		{
			if(aKeys[i][1] == aKeys[i][2] || aKeys[i][1] == aKeys[i][3])
			{
				share[i] = true;
				remaining[i]--;
			}
		}
	}
}

/************************************
 * Purpose: check the sharing status
 ************************************/

bool AdjKeys::hasSharing(int id)
{
	return share[id];
}

/*****************************************************
 * Purpose: print the detail information in this class
 *****************************************************/

void AdjKeys::printInfo()
{
	for(int i = 0; i < nof; i++)
	{
		cout << aKeys[i][0] << ", " << aKeys[i][1] << ", "
			 << aKeys[i][2] << ", " << aKeys[i][3] << "\t";
	
		cout << "sharing = " << share[i] << ", " << " remain = " << remaining[i] << endl;
	}
}

/*****************************************************************
 * Purpose: reset the adjacent key to be -1 if it is already taken
 *****************************************************************/

void AdjKeys::deleteTakenKey(int takenKey)
{
	int p, q;
	for(p = 0; p < nof; p++)
	{
		if(takenKey != p)
		{
			bool exist = false;
			for(q = 0; q < 4; q++)
			{
				if(aKeys[p][q] != -1 && aKeys[p][q] == takenKey)
				{
					aKeys[p][q] = -1;
					exist = true;
				}
			}
			if(exist)
				decrementRemaining(p);
		}
	}
}

/***********************************************************
 * Purpose: performs the actual edge-recombination operation 
 ***********************************************************/

void AdjKeys::recombine()
{
	int currKey = startKey;
	offSpring[0] = currKey;
	deleteTakenKey(currKey);

	int i;
	// produce offspring based on adjacent keys

	for(i = 1; i < nof; i++)
	{
		int flag = false;
		int tempKeys[4] = {-1, -1, -1, -1};
		int w = 0;
		for(int j = 0; j < 4; j++)
		{
			int t = aKeys[currKey][j];
			if(t != -1 && hasSharing(t))
			{
				currKey = t;
				flag = true;
				break;
			}
			else
			{
				if(t != -1)
					tempKeys[w++] = t;
				continue;
			}
		}
		if(!flag)
		{
			if(w != 0)
			{
				int tk = tempKeys[0];
				int moreAdjLeft = getRemaining(tk);
				currKey = tk;
				for(int v = 1; v < w; v++)
				{
					tk = tempKeys[v];
					int adjLeft = getRemaining(tk);
					if(adjLeft > moreAdjLeft)
					{
						moreAdjLeft = adjLeft;
						currKey = tk;
					}
				}
			}
			else
				break;
		}
		offSpring[i] = currKey;
		deleteTakenKey(currKey);
	}	

	if(i < nof)  // handle special case
	{
		for(int x = 0; x < nof; x++) 
		{
			if(getRemaining(x) != 0)
			{
				for(int y = 0; y < 4; y++)
				{
					if(aKeys[x][y] != -1)
					{
						offSpring[i++] = aKeys[x][y];
						deleteTakenKey(aKeys[x][y]);
					}
				}
			}
		}
	}
}

/************************************ End of File **************************************/
