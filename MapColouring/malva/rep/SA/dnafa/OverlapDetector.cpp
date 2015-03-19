/***********************************************************************
 *  File:        OverlapDetector.cpp
 *  Author:      Lishan Li                                        
 *  Date:        May, 2003                                                 
 *
 *  Description: It provides implementation of all the functions declared   
 *               in the header file OverlapDetector.h.
 ***********************************************************************/

#include "OverlapDetector.h"

/**************************
 * Purpose: allocate memory 
 **************************/

OverlapDetector::OverlapDetector(int nof):nbOfFrag(nof)
{
	score = new int*[nof];
	for(int i = 0; i < nof; i++)
		score[i] = new int[nof];
}

/**************************
 * Purpose: release memory 
 **************************/

OverlapDetector::~OverlapDetector()
{
	for(int j = 0; j <  nbOfFrag; j++)
		delete [] score[j];
	delete [] score;
}

/**********************************************************
 * Purpose: construct a score table to store overlap length 
 *          of all possible pair of fragments 
 **********************************************************/

void OverlapDetector::buildScoresTable(vector<string>* fragments, vector<string>* rcFragments)
{
	for(int i = 0; i < nbOfFrag; i++)
	{
		string rcFrags = rcFragments->at(i);
	
		for(int j = i+1; j < nbOfFrag; j++)
		{
			SGA sga1(fragments->at(i), fragments->at(j)); // eg. w1 and w2
			sga1.buildMatrix();
			sga1.findBestScore();
			int score1 = sga1.getBestScore();

			SGA sga2(rcFrags, fragments->at(j)); // eg. rcw1 and w2
			sga2.buildMatrix();
			sga2.findBestScore();
			int score2 = sga2.getBestScore();

			if(score2 > score1)        // store the larger one
				score[j][i] = score[i][j] = -score2;
			else
				score[j][i] = score[i][j] = score1;
		}
	}
}

/***************************
 * Purpose: for testing use  
 ***************************/

void OverlapDetector::readScoreTable()  
{
	string inputFile = string(getenv("HOME")) + "/Mallba/ProblemInstances/DNAFA-instances/score.txt";
	ifstream inFile(inputFile.c_str(), ios::in); 
	if( !inFile )
	{
		cout << "\n\t\t*** The file " << inputFile 
			 << " doesn't exist! ***" << endl << endl;
		exit(1);
	}

/*	char seps[] = " \t\n";
	char oneLine[2000];
	int i = 0;
	inFile.getline(oneLine, 2000);
	while(inFile.getline(oneLine, 2000))
	{
		int j = 0;
		char * token = strtok(oneLine, seps);

		while (token != NULL)
		{
			score[i][j] = atoi(token);
			token = strtok(NULL, seps);
			j++;
		}
		i++;
	}*/
	for(int i = 0; i < nbOfFrag; i++)
		for(int j= 0; j < nbOfFrag; j++)
			inFile >> score[i][j];
	inFile.close();
}

/************************************
 * Purpose: print overlap score table 
 ************************************/

void OverlapDetector::printScoreTable()
{
//	cout << endl << "*** *** *** *** Overlap Score Table *** *** *** ***" << endl << endl;

//	cout << "   ";
//	for(int k = 0; k < nbOfFrag; k++)
//		cout << setw(5) << setiosflags(ios::right) << "F" << k;

	for(int i = 0; i < nbOfFrag; i++)
	{
		cout << endl;
//		cout << "F" << i << "  ";
		for(int j = 0; j < nbOfFrag; j++)
		{
			if(i == j)
				cout << setw(5) << setiosflags(ios::right) << "0 ";
			else
				cout <<  setw(5) << setiosflags(ios::right) << score[i][j] << " ";
		}
	}
//	cout << endl << endl << "*****************************************************" << endl;
}

/************************************ End of File **************************************/
