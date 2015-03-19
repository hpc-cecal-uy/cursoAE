/*********************************************************************
 *  File:        ConsensusBuilder.h
 *  Author:      Lishan Li                                        
 *  Date:        May, 2003                                                 
 *
 *  Description: This ConsensusBuilder class is used to determine the   
 *               sequence for contigs.
 *********************************************************************/

#ifndef CONSENSUSBUILDER_H
#define CONSENSUSBUILDER_H

#pragma warning(disable: 4786)

#include <iostream>
#include <string>
#include <vector>
#include "list.h"
#include "Util.h"
#include "ContigBuilder.h"
#include "LayoutBuilder.h"
#include "scp.h"

using namespace std;

class ConsensusBuilder
{
public:
	// F - forward; R - reverse complement
	enum TableType { FF, FR, RF, RR };

	ConsensusBuilder(vector<string> &SACons); 
	~ConsensusBuilder();

	void initLayout();                // initialize layout
	void buildConsensus();
	void updateConsensus(int type);
	void remove(List1<SubConsensusPair> * list, int cid);
	ListNode<SubConsensusPair> * search(List1<SubConsensusPair> * list, int cid);
	vector<string> getConsensus() { return consensus; }
	void print();
private:
	void buildSubConsensus(int type);
	void initST(int type);

	List1<SubConsensusPair>* scpList;  // a list of SubConsensusPair objects    
	vector<string> consensus;
	vector<string> s, t;
};

#endif

/************************************ End of File **************************************/
