/***********************************************************************
 *  File:        LayoutBuilder.cpp
 *  Author:      Lishan Li                                        
 *  Date:        May, 2003                                                 
 *
 *  Description: It provides implementation of all the functions declared   
 *               in the header file LayoutBuilder.h.
 ***********************************************************************/

#include "LayoutBuilder.h"

/*********************************************
 * Purpose: allocate memory and initialization
 *********************************************/

LayoutBuilder::LayoutBuilder(const vector<string> & frags)
{
	preLayoutFrags = frags;
	fragmentsLayout = new string[frags.size()];
	semiAlign = new AlignedPair[frags.size()-1];
}

/*************************
 * Purpose: release memory 
 *************************/

LayoutBuilder::~LayoutBuilder()
{
	delete [] semiAlign;
	delete [] fragmentsLayout;
}

/************************************************************
 * Purpose: count number of gaps in front of a given fragment
 ************************************************************/

int countStartGaps(const string &str)
{
	int count = 0;
	for(int i = 0; i < str.length(); i++)
	{
		if(str.at(i) != '-')
			break;
		else
			count++;
	}
	return count;
}

/**************************************************************
 * Purpose: count number of gaps at the end of a given fragment
 **************************************************************/

int countEndGaps(const string &str)
{
	int count = 0;
	for(int i = str.length() - 1; i >= 0; i--)
	{
		if(str.at(i) != '-')
			break;
		else
			count++;
	}
	return count;
}

/**********************************************************
 * Purpose: count number of nucleotides in a given fragment
 **********************************************************/

int countNonGapChar(const string &str)
{
	int count = 0;
	for(int i = 0; i < str.length(); i++)
	{
		if(str.at(i) != '-')
			count++;
	}
	return count;
}

/****************************************
 * Purpose: create a given number of gaps 
 ****************************************/

string addGaps(int num)
{
	string gapStr = "";
	for(int i = 0; i < num; i++)
		gapStr += "-";

	return gapStr;
}

/**********************************************************
 * Purpose: count number of middle gaps in a given fragment
 **********************************************************/

int* findMiddleGaps(const string &str, int count)
{
	int *midGapCounts = new int[count];
	int len = str.length();

	for(int i = 0; i < count; i++)
	{
		midGapCounts[i] = 0;
	}

	int k = 0;
	for(int j = 0; j < len; j++)
	{
		if(str[j+1] == '-')
		{
			midGapCounts[k]++;
		}
		else
			k++;		
	}

	return midGapCounts;
}

/********************************************
 * Purpose: return the first non-gap position 
 ********************************************/

int findFirstNonGapPos(const string &str)
{
	for(int i = 0; i < str.length(); i++)
	{
		if(str.at(i) != '-')
			return i;
	}
	return -1;
}

/******************************************************************
 * Purpose: compute pairwise alignments for given ordered fragments
 ******************************************************************/

void LayoutBuilder::findAlignments()
{
	for(int k = 0; k < preLayoutFrags.size()-1; k++)
	{
		string s = preLayoutFrags[k];
		string t = preLayoutFrags[k+1];
		SGA sga(s, t);
		sga.buildMatrix();
		sga.findBestScore();
		sga.align();
		
		AlignedPair aResult(sga.align_s, sga.align_t);
		semiAlign[k] = aResult;
	}
}

/*************************************************************
 * Purpose: insert number of gaps in front of a given fragment
 *************************************************************/

void LayoutBuilder::insertStartGaps(int id, int numOfGaps)
{
	string temp = fragmentsLayout[id];
	string startGaps = "";
	for(int i = 0; i < numOfGaps; i++)
		startGaps += "-";
	fragmentsLayout[id] = startGaps + temp;
}

/***********************************************************
 * Purpose: insert number of gaps in the middle of fragments
 ***********************************************************/

void LayoutBuilder::insertMiddleGaps(const string &str1, const string &str2, int k, int prev)
{
	int nonGapCount = countNonGapChar(str1);
	int *midGapCounts1 = findMiddleGaps(str1, nonGapCount-1);
	int *midGapCounts2 = findMiddleGaps(str2, nonGapCount-1);

	int pos = findFirstNonGapPos(fragmentsLayout[prev]);

	for(int w = 0; w < nonGapCount-1; w++)
	{
		pos += 1;
		if(midGapCounts1[w] != 0 || midGapCounts2[w] != 0)
		{
			int gapDiff = abs(midGapCounts1[w] - midGapCounts2[w]);
			int move = 0;
			string gaps = "";
			for(int u = 0; u < gapDiff; u++)
				gaps += "-";
		
			if(midGapCounts1[w] > midGapCounts2[w])
			{
				fragmentsLayout[k].insert(pos, gaps);
				move = midGapCounts1[w];
			}
			else if (midGapCounts1[w] < midGapCounts2[w])
			{
				for(int v = 0; v < k; v++)
					fragmentsLayout[v].insert(pos, gaps);
				move = midGapCounts2[w];
			}
			pos += move;
		}
	}
	delete [] midGapCounts1;
	delete [] midGapCounts2;
}

/***************************************************************
 * Purpose: insert number of gaps at the end of a given fragment
 ***************************************************************/

void LayoutBuilder::insertEndGaps(int id, int numOfGaps)
{
	string temp = fragmentsLayout[id];
	string endGaps = "";
	for(int i = 0; i < numOfGaps; i++)
		endGaps += "-";
	fragmentsLayout[id] = temp + endGaps;
}

/**************************************************************
 * Purpose: extract fragment excluding starting and ending gaps
 **************************************************************/

string LayoutBuilder::extractMiddleSeq(const string &str)
{
	int startGapCount = countStartGaps(str);
	int endGapCount = countEndGaps(str);
	int num = str.length() - startGapCount - endGapCount;
	return str.substr(startGapCount, num); 
}

/************************************
 * Purpose: check if it is ending gap
 ************************************/

bool LayoutBuilder::isEndGap(int i, int col)
{
	string middle = extractMiddleSeq(fragmentsLayout[i]);
	int firstNonGap = findFirstNonGapPos(fragmentsLayout[i]);

	if(col < firstNonGap || col > firstNonGap + middle.length() - 1)
		return true;
	else
		return false;
}

/************************************************
 * Purpose: progressively build a fragment layout
 ************************************************/

void LayoutBuilder::buildLayout()
{
	int j = 0;
	fragmentsLayout[j++] = semiAlign[0].s;
	fragmentsLayout[j++] = semiAlign[0].t;

	for(int i = 1; i < preLayoutFrags.size()-1; i++)
	{
		fragmentsLayout[j] = semiAlign[i].t;
		
		/* handle starting gaps */

		string s_temp = semiAlign[i].s;

		int s_startGaps = countStartGaps(fragmentsLayout[j-1]);
		int stemp_startGaps = countStartGaps(s_temp);
		int startGapDiff = abs(s_startGaps - stemp_startGaps);
		
		if(startGapDiff > 0)
		{
			if(stemp_startGaps > s_startGaps)
			{
				for(int m = 0; m < j; m++)
					insertStartGaps(m, startGapDiff);
			}
			else
				insertStartGaps(j, startGapDiff);
		}

		/* handle middle gaps */

		string s_middle = extractMiddleSeq(fragmentsLayout[j-1]);
		string stemp_middle = extractMiddleSeq(s_temp);
	
		insertMiddleGaps(s_middle, stemp_middle, j, j-1);
	
		/* handle end gaps */
	
		int s_endGaps = countEndGaps(fragmentsLayout[j-1]);
		int stemp_endGaps = countEndGaps(s_temp);
		int endGapDiff = abs(s_endGaps - stemp_endGaps);

		if(endGapDiff > 0)
		{
			if(stemp_endGaps > s_endGaps)
			{
				for(int m = 0; m < j; m++)
					insertEndGaps(m, endGapDiff);
			}
			else 
				insertEndGaps(j, endGapDiff);
		}
		j++;
	}
}

/***********************************************
 * Purpose: find majority of a particular column
 ***********************************************/

char LayoutBuilder::majorityVote(int col)
{
	int count[5] = { 0, 0, 0, 0, 0 };
	char nucleotide[5] = {'A', 'T', 'C', 'G', '-'};
	char majority1 = 'A';
	char majority2 = 'A';

	for(int i = 0; i < preLayoutFrags.size(); i++)
	{
		if(fragmentsLayout[i].at(col) == 'A')
			count[0]++;
		else if(fragmentsLayout[i].at(col) == 'T')
			count[1]++;
		else if(fragmentsLayout[i].at(col) == 'C')
			count[2]++;
		else if(fragmentsLayout[i].at(col) == 'G')
			count[3]++;
		else if(fragmentsLayout[i].at(col) == '-')
			count[4]++;
	}
	int max = count[0];
	for(int j = 1; j < 5; j++)
	{
		if(count[j] > max)
		{
			majority2 = majority1;
			max = count[j];
			majority1 = nucleotide[j];
		}
	}
	bool endGap = true;
	if(majority1 == '-')
	{
		for(int i = 0; i < preLayoutFrags.size() && endGap; i++)
		{
			if(fragmentsLayout[i].at(col) == '-')
			{
				endGap = isEndGap(i, col);
			}
		}
		
		if(endGap)
			return majority2;
	}
	return majority1;
}

/***************************************
 * Purpose: find majority of all columns
 ***************************************/

void LayoutBuilder::findMajority()
{
	int length = fragmentsLayout[0].length();

	majority = "";
	for(int i = 0; i < length; i++)
	{
		char c = majorityVote(i);
		majority += c;
	}
}

/********************************************
 * Purpose: determine sequence for the layout
 ********************************************/

void LayoutBuilder::findConsensus()
{
	findMajority();
	consensus = "";
	for(int i = 0; i < majority.length(); i++)
	{
		if(majority.at(i) != '-')
			consensus += majority.at(i);
	}
}

/***************************************************
 * Purpose: print info about all pairs of alignments
 ***************************************************/

void LayoutBuilder::printAlignments()
{
	for(int j = 0; j < preLayoutFrags.size()-1; j++)
		semiAlign[j].printAlign();
}

/***********************
 * Purpose: print layout
 ***********************/

void LayoutBuilder::printLayout()
{
    for(int i = 0; i < preLayoutFrags.size(); i++)
		cout << fragmentsLayout[i] << endl;
	cout << endl;
}

/**************************
 * Purpose: print consensus 
 **************************/

void LayoutBuilder::printConsensus()
{
	cout << consensus << endl;
}

/************************************ End of File **************************************/
