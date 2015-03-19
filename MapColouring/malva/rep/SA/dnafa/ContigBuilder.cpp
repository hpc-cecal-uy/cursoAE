/***********************************************************************
 *  File:        ContigBuilder.cpp
 *  Author:      Lishan Li                                        
 *  Date:        May, 2003                                                
 *
 *  Description: It provides implementation of all the functions declared   
 *               in the header file ContigBuilder.h.
 ***********************************************************************/

#include "ContigBuilder.h"

/******************************************************
 * Purpose: initialize fragment order and overlap score 
 ******************************************************/

ContigBuilder::ContigBuilder(int * order, int ** s, const SA::Problem *pbm):_pbm(pbm)
{
	fragOrder = order;
	score = s;
}

/*******************************************************
 * Purpose: construct contigs based on the overlap score
 *          of adjacent fragments
 *******************************************************/

void ContigBuilder::buildContigs()
{
	vector<int> tempOrder;

	int id1 = fragOrder[0];
	tempOrder.push_back(id1+1);  // +1 because 0 has no negative number
	int direction = 1;

	for(int i = 1; i < _pbm->numOfFragments; i++)
	{
		int id2 = fragOrder[i];
	//	cout << "score[" << id1 << "][" << id2 << "] = " << score[id1][id2] << endl;

		int sign = direction * score[id1][id2];

		if(abs(score[id1][id2]) >= _pbm->cutoff)  // default cutoff = 30bps
		{
			if(sign > 0)  // from same strand
			{
				tempOrder.push_back(id2+1);
				direction = 1;
			}
			else          // from different strand
			{
				tempOrder.push_back(-(id2+1));
				direction = -1;
			}
			id1 = id2;
		}
		else
		{
			allContigs.push_back(tempOrder);  // a new contig is formed
			tempOrder.clear();
			id1 = id2;
			tempOrder.push_back(id1+1);
			direction = 1;
		}
	}
	allContigs.push_back(tempOrder);
}

/*******************************************************
 * Purpose: merge two contigs and update the orientation 
 *          accordingly
 *******************************************************/

void ContigBuilder::mergeTwoContig(int cid1, int cid2)
{
	vector<int> fids1 = allContigs.at(cid1);
	vector<int> fids2 = allContigs.at(cid2);

	int fid1 = fids1.at(fids1.size()-1);
	int fid2 = fids2.at(0);
	int sign = fid1 * fid2;

	bool type = score[abs(fid1)-1][abs(fid2)-1] > 0;

	bool flag = (!type && sign < 0) || (type && sign > 0); // flag = true => not change

	for(int i = 0; i < fids2.size(); i++)
	{
		if(flag) // don't change
			allContigs.at(cid1).push_back(fids2.at(i));
		else     // change
			allContigs.at(cid1).push_back(-fids2.at(i));
	}

	allContigs.erase(allContigs.begin() + cid2);
}

/*****************************************************
 * Purpose: merge contigs that satisfy the predefined 
 *          threshold
 *****************************************************/

void ContigBuilder::mergeContigs()
{
	while(allContigs.size() >= 2)
	{
		int bestScore = _pbm->cutoff;
		int cid_i = -1;
		int cid_j = -1;

	    for(int i = 0; i < allContigs.size()-1; i++)
		{
			vector<int> tempi = allContigs.at(i);
			int fidi_First = abs(tempi.at(0)) - 1;
			int fidi_Last = abs(tempi.at(tempi.size()-1)) - 1;
		
			for(int j = i+1; j < allContigs.size(); j++)
			{
				vector<int> tempj = allContigs.at(j);

				int fidj_First = abs(tempj.at(0)) - 1;
				int fidj_Last = abs(tempj.at(tempj.size()-1)) - 1;

				if(abs(score[fidi_Last][fidj_First]) >= bestScore) // put contig_j after contig_i 
				{
					bestScore = abs(score[fidi_Last][fidj_First]);
					cid_i = i; 
					cid_j = j;
				}
				if(abs(score[fidj_Last][fidi_First]) > bestScore) // put contig_j before contig_i
				{
					bestScore = abs(score[fidj_Last][fidi_First]);
					cid_i = j; 
					cid_j = i;
				}
			}
		}

		if(cid_i != -1 && cid_j != -1)
			mergeTwoContig(cid_i, cid_j);
		else
			break;
	}
	cerr << endl << "*** *** *** *** SA Contigs ( " << allContigs.size() << " ) *** *** *** ***" << endl << endl;
}

/*******************************************
 * Purpose: build consensus for each contigs
 *******************************************/

void ContigBuilder::buildConsensus()
{
	vector< vector<string> > fragLayouts;
	for(int i = 0; i < allContigs.size(); i++)
		fragLayouts.push_back(getPreLayoutFrags(allContigs.at(i)));

	vector<string> SACons;
	cerr << endl << "*** *** *** *** SA Contigs ( " << fragLayouts.size() << " ) *** *** *** ***" << endl << endl;

	for(int j = 0; j < fragLayouts.size(); j++)
	{
		vector<string> layout = fragLayouts.at(j);
		
//		cout << endl << "Fragments Layout (" << j << ") =>" << endl << endl;

		if(layout.size() > 1)
		{
			LayoutBuilder layoutBuilder(layout);
			layoutBuilder.findAlignments();
		//	layoutBuilder.printAlignments();
			layoutBuilder.buildLayout();
			layoutBuilder.printLayout();
			layoutBuilder.findConsensus();
			layoutBuilder.printConsensus();
			SACons.push_back(layoutBuilder.getConsensus());
		}
		else
		{
			SACons.push_back(layout.at(0));
//			cout << layout.at(0) << endl;
		}
	}
	ConsensusBuilder * cb = new ConsensusBuilder(SACons);
/**/ cerr << "Building Consensus" << endl;
	cb->buildConsensus();
/**/ cerr << "End Building Consensus" << endl;

	vector<string> finalCons = cb->getConsensus();
	cerr << endl << "*** *** *** *** Final Contigs ( " << finalCons.size() << " ) *** *** *** ***" << endl << endl;
/*	for(int k = 0; k < finalCons.size(); k++)
	{
		cout << ">Contig #" << k << endl;
		cout << finalCons.at(k) << endl;
	}
	cout << endl << "*****************************************************" << endl;
*/	delete cb;
}

/********************************************************
 * Purpose: returns a vector of fragments associated with 
 *          the fragment order
 ********************************************************/

vector<string> ContigBuilder::getPreLayoutFrags(vector<int> order)
{
	vector<string> preLayoutFrags;

	for(int i = 0; i < order.size(); i++)
	{
		int fid = order.at(i);
		if( fid > 0)		// -1 because +1 when insert
			preLayoutFrags.push_back(_pbm->fragments->at(fid-1));
		else
			preLayoutFrags.push_back(_pbm->rcFragments->at(abs(fid)-1));
	}
//	cout << endl;
	return preLayoutFrags;
}

/***********************************************
 * Purpose: print fragment order for each contig
 ***********************************************/

void ContigBuilder::printOrder()
{
/*	cout << endl << "Contig size = " << allContigs.size() << endl;

	for(int i = 0; i < allContigs.size(); i++)
	{
		vector<int> temp = allContigs.at(i);
		for(int j = 0; j < temp.size(); j++)
			cout << temp.at(j) << ", ";
		cout << endl;
	}
	cout << endl;
*/}

/************************************ End of File **************************************/

