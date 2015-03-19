/***********************************************************************
 *  File:        sga.cpp
 *  Author:      Lishan Li                                        
 *  Date:        May, 2003                                                 
 *
 *  Description: It provides implementation of all the functions declared   
 *               in the header file sga.h.
 ***********************************************************************/

#include "sga.h"

/*********************************************
 * Purpose: allocate memory and initialization
 *********************************************/

SGA::SGA(const string &is, const string &it)
{
	g = -2;       // gap penalty = -2
	s = is;
	t = it;
	m = s.length();
	n = t.length();	
// /**/ cout << "\t size " << m+1 << " x " << n+1 << endl;
	bestScore = 0;
}

/*************************
 * Purpose: release memory
 *************************/

SGA::~SGA()
{
}


/*******************************************************
 * Purpose: Build a score matrix to store the scores of
 *          pairwise semiglobal alignment
 *******************************************************/

void SGA::CalculateBest(int i, int j, int score)
{
//cout << "(" << i<<","<< j<<","<<score<<")"<<endl;
	if(j == n)
	{
		if((score == n) && (score > bestScore)) // -------
		{										//   ---
			bestScore = score;
			overlapType = 1;
		}
		else if((score == i) && (score > bestScore)) //    -------
		{											 // -------
			bestScore = score;
			overlapType = 2;
		}
	}
	
	if(i == m)
	{
		if((score == m) && (score > bestScore)) //   ---
		{										// ---------
			bestScore = score;
			overlapType = 3;
		}
		else if((score == j) && (score > bestScore)) // -------
		{											 //    -------
			bestScore = score;
			overlapType = 4;
		}
	}
}

void SGA::buildMatrix()
{
	// Primero la mitad superior
	int i,j,k;
	int score;
	bestScore = 0;
	overlapType = 3;
	for(k = 1; k <= m; k++)
	{
		score = 0;
		i = k;
		j = 1;
		while((j <=n) && (i <=m))
		{
			if(p(i,j) == 1)
				score = score + 1;
			else
				score = 0;
			j++;
			i++;
		}
		i--;j--;
		CalculateBest(i,j,score);
	}

	for(k = 2; k <= n; k++)
	{
		score = 0;
		j = k;
		i = 1;
		while(j <=n && i <=m)
		{
			if(p(i,j) == 1)
				score = score + 1;
			else
				score = 0;
			j++;
			i++;
		}
		i--;j--;
		CalculateBest(i,j,score);
	}
}

/***************************
 * match score = 1
 * mismatch score = -1
 ***************************/

int SGA::p(int i, int j)
{
	if (s[i-1] == t[j-1])  // match
		return 1;
	else
		return -1;
}

/****************************************************
 * Purpose: find the maximum number of x, y, and z
 ****************************************************/

int SGA::max(int x, int y, int z)
{
	int temp = x;
	if (x < y)
		temp = y;
	if (temp < z)
		temp = z;
	return temp;
}

/********************************************************************
 * Purpose: find the best score for the pairwise semiglobal alignment
 ********************************************************************/

void SGA::findBestScore()
{	
}


/**********************************************************************
 * Purpose: Align the two sequence starting from the the row and column
 *          with the best score
 **********************************************************************/

void SGA::align()
{
	int i;
	int start;
//cout << s << endl << t << endl << bestScore << " " << overlapType << endl;
//exit(-1);
	string preGaps = "";
	string postGaps = "";
	if((start = s.find(t)) != string::npos)	// t is substring of s
	{
		align_s = s;
		for(i = 0; i < start; i++)
			preGaps += "-";
		align_t = preGaps + t;

		for(i = start+bestScore; i < m; i++)
			postGaps += "-";
		align_t += postGaps;
	}
	else if((start = t.find(s)) != string::npos) // s is substring of t
	{
		align_t = t;
		for(i = 0; i < start; i++)
			preGaps += "-";
		align_s = preGaps + s;

		for(i = start+bestScore; i < n; i++)
			postGaps += "-";
		align_s += postGaps;
	}
	else if(overlapType == 4)
	{
		int preNumGaps = m - bestScore;
		for(int i = 0; i < preNumGaps; i++)
			preGaps += "-";
		align_t = preGaps + t;

		int postNumGaps = align_t.length() - m;
		for(int j = 0; j < postNumGaps; j++)
			postGaps += "-";
		align_s = s + postGaps;
	}
	else
	{
		int preNumGaps = n - bestScore;
		for(int i = 0; i < preNumGaps; i++)
			preGaps += "-";
		align_s = preGaps + s;

		int postNumGaps = align_s.length() - n;
		for(int j = 0; j < postNumGaps; j++)
			postGaps += "-";
		align_t = t + postGaps;
	}
}

/************************************************************
 * Purpose: Print the score matrix for the pairwise alignment
 ************************************************************/

void SGA::printMatrix()
{
/*	for (int i = 0; i <= m; i++)
	{
		cout << endl;
		for (int j = 0; j <= n; j++)
		{
			printf("%3d", score[i][j]);
		}
	}
	cout << endl << endl;
	cout << "The best score is: " << bestScore << endl << endl;*/
}

/***************************************
 * Purpose: Print the pairwise alignment
 ***************************************/

void SGA::printAlign()
{
	cout << align_s << endl;
	cout << align_t << endl << endl;
}

/*******************************
 * Purpose: Print Info for debug
 *******************************/
void SGA::printInfo()
{
	cout << "m = " << m << " n = " << n << " g = " << g
		 << " bestRow = " << bestRow << " bestCol = " << bestCol << " bestscore = " << bestScore
		 << endl;
	cout << "s = " << align_s << " t = " << align_t << endl;

//	printMatrix();
}

/************************************ End of File **************************************/

