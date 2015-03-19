/***********************************************************************
 *  File:        ConsensusBuilder.cpp
 *  Author:      Lishan Li                                        
 *  Date:        May, 2003                                                 
 *
 *  Description: It provides implementation of all the functions declared   
 *               in the header file ConsensusBuilder.h.
 ***********************************************************************/

#include "ConsensusBuilder.h"

int Global_cutoff;

/*****************************************************
 * Purpose: build the initial consensuses from contigs 
 *****************************************************/

ConsensusBuilder::ConsensusBuilder(vector<string> &SACons) 
{
	consensus = SACons;
	scpList = new List1<SubConsensusPair>();
}

/*************************
 * Purpose: release memory 
 *************************/

ConsensusBuilder::~ConsensusBuilder()
{
	delete scpList;
}

/**********************************************
 * Purpose: postprocess four types of consensus 
 **********************************************/

void ConsensusBuilder::buildConsensus()
{
	if(consensus.size() > 1)
	{
//		cout << endl << "case FF" << endl;
/**/ cerr << "case FF" << endl;
		buildSubConsensus(FF);           // both from forward strand
/**/ cerr << "End case FF" << endl;
		if(consensus.size() > 1)
		{
//			cout << endl << "case FR" << endl;
/**/ cerr << "case FR" << endl;
			buildSubConsensus(FR);       // first from forward, second from reverse complement
/**/ cerr << "End case FR" << endl;
			if(consensus.size() > 1)
			{
//				cout << endl << "case RF" << endl;
/**/ cerr << "case RF" << endl;
				buildSubConsensus(RF);    // first from reverse complement, second from forward
/**/ cerr << "End case RF" << endl;
				if(consensus.size() > 1)
				{
//					cout << endl << "case RR" << endl;;
/**/ cerr << "case RR" << endl;
					buildSubConsensus(RR); // both from reverse complement
/**/ cerr << "End case RR" << endl;
				}
			}
		}
	}
}

/**********************************************
 * Purpose: initialize four types of consensus 
 **********************************************/

void ConsensusBuilder::initST(int type)
{
	vector<string> rc;
	for(int k = 0; k < consensus.size(); k++)
		rc.push_back(Util::rcSequence(consensus[k]));
	
	s.clear();
	t.clear();
	switch(type)
	{
		case FF:
			s = t = consensus;
			break;
		case FR:
			s = consensus;
			t = rc;
			break;
		case RF:
			s = rc;
			t = consensus;
			break;
		case RR:
			s = t = rc;
			break;
	}
}

/********************************************************
 * Purpose: detects the overlap for all possible pair of
 *          consensus and stores the overlap score in a 
 *          sorted linked list in descending order.
 ********************************************************/

void ConsensusBuilder::buildSubConsensus(int type)
{
//	cout << "\nCurrent consensus (" << type << ")" << endl;
	//print();
	initST(type);

	for(int i = 0; i < s.size(); i++)
	{
		for(int j = i+1; j < s.size(); j++)  // compute half of pairs only
		{
			SGA *sga = new SGA(s[i], t[j]); // eg. w1 and w2
/**/ cerr <<  "Building Matrix " << i << " " << j << endl;
			sga->buildMatrix();
			sga->findBestScore();
			int score = sga->getBestScore();
			delete sga;
/**/ cerr <<  "End Building Matrix " << i << " " << j << endl;

			if(score >= Global_cutoff) 
			{
				scpList->insert(SubConsensusPair(i, j, score));
				scpList->insert(SubConsensusPair(j, i, score));
			}
		}
	}
//	scpList->printList();
/**/ cerr <<  "Updating Consensus" << endl;
	updateConsensus(type);
/**/ cerr <<  "End Updating Consensus" << endl;
}

/************************************************************
 * Purpose: update consensus information by aligning possible
 *          overlapping consensuses.
 ************************************************************/

void ConsensusBuilder::updateConsensus(int type)
{
	vector<string> tempLayout;
	vector<string> tepCon = consensus;
	consensus.clear();
	
	ListNode<SubConsensusPair>* tempNode = scpList->header->next;

	while(tempNode != NULL) 
	{
		int cid1 = tempNode->data.cid1;
		int cid2 = tempNode->data.cid2;
		char direction = 'F';  // forward

		tempLayout.push_back(s[cid1]);
		tempLayout.push_back(t[cid2]);
		if(type == FR)
			direction = 'R';   // reverse complement

		tepCon.at(cid1) = "";
		tepCon.at(cid2) = "";
		remove(scpList, cid1);
		bool found = true;
		while(found) 
		{
			ListNode<SubConsensusPair>* nextNode = search(scpList, cid2);
			if(nextNode == NULL)
				found = false;
			else
			{
				cid1 = cid2;
				cid2 = nextNode->data.cid2;
			
				if(type == FR && direction == 'R')
				{
					tempLayout.push_back(s[cid2]); // s[cid2] =	Util::rcSequence(t[cid2])
					direction = 'F';
				}
				else if (type == FR && direction == 'F')
				{
					tempLayout.push_back(t[cid2]);
					direction = 'R';
				}
				else if (type == RF && direction == 'F')
				{
					tempLayout.push_back(s[cid2]);
					direction = 'R';
				}
				else
				{
					tempLayout.push_back(t[cid2]);
					direction = 'F';
				}

				tepCon.at(cid2) = "";
				remove(scpList, cid1);			
			}
		}

		if(tempLayout.size() > 1)
		{
			LayoutBuilder layoutBuilder(tempLayout);
			layoutBuilder.findAlignments();
		//	layoutBuilder.printAlignments();
			layoutBuilder.buildLayout();
//			cout << endl << "Merging =>"<< endl << endl;
			layoutBuilder.printLayout();
			layoutBuilder.findConsensus();
			layoutBuilder.printConsensus();
			consensus.push_back(layoutBuilder.getConsensus());
		}
		else
			consensus.push_back(tempLayout.at(0));
		
		tempLayout.clear();
		tempNode = scpList->header->next;
	}

	for(int m = 0; m < tepCon.size(); m++)
	{
		if(tepCon.at(m) != "")
			consensus.push_back(tepCon.at(m));
	}
}

/************************************************************
 * Purpose: removes SubConsensusPair objects that contain the 
 *          given id from the linked list. 
 ************************************************************/

void ConsensusBuilder::remove(List1<SubConsensusPair>* list, int cid)
{
	List1<SubConsensusPair>* temp = list;
	ListNode<SubConsensusPair>* tempCurr = temp->header->next;
	ListNode<SubConsensusPair>* tempPrev = temp->header;

	while(tempCurr != NULL)
	{
		if(tempCurr->data.cid1 == cid || tempCurr->data.cid2 == cid )
		{
			ListNode<SubConsensusPair>* oldNode = tempCurr;
			tempPrev->next = tempCurr->next;
			tempCurr = tempCurr->next;
			delete oldNode;
			(temp->len)--;
		}
		else
		{
			tempPrev = tempCurr;
			if(tempCurr != NULL)
				tempCurr = tempCurr->next;
		}
	}
}

/************************************************************
 * Purpose: find the SubConsensusPair object that contain the 
 *          given id in the linked list. 
 ************************************************************/

ListNode<SubConsensusPair>* ConsensusBuilder::search(List1<SubConsensusPair>* list, int cid)
{
	ListNode<SubConsensusPair>* curr;
	
	for(curr = list->header->next; curr != NULL; curr = curr->next)
	{
		if(curr->data.cid1 == cid)  // found
			return curr;
	}
	return NULL;
}

/***********************************
 * Purpose: prints current consensus 
 ***********************************/

void ConsensusBuilder::print()
{
/*	for(int j = 0; j < consensus.size(); j++)
		cout << endl << "Cons #" << j << " => " << consensus.at(j);
	cout << endl;
*/}

/************************************ End of File **************************************/

