/************************************************
***	    			  	                      ***
***  Simulated Annealing Skeleton v1.0        ***
***  User-required classes and methods        ***
***  Developed by: Carlos Cotta Porras        ***
***  Tab size = 4                             ***
***					                          ***
***                                           ***
************************************************/

#include <iostream.h>
#include "SA.hh"
#include "Mallba/random.hh"
#include "StopCondition.hh"
#include <math.h>
#include "Util.h"
#include "AdjKeys.h"

skeleton SA {

	// Problem ---------------------------------------------------------------

    Problem::Problem ():numOfFragments(0),avgLength(0.0),cutoff(0),
                        fragmentLength(NULL),fragments(NULL),rcFragments(NULL),
                        description(NULL)
    {
    }
 
    ostream& operator<< (ostream& os, const Problem& pbm)
    {
        return os;
    }
 
    istream& operator>> (istream& is, Problem& pbm)
    {
        char buffer[MAX_BUFFER];
        int i;
 
 
        is.getline(buffer,MAX_BUFFER,'\n');
        sscanf(buffer,"%d",&pbm.cutoff);
 
        is.getline(buffer,MAX_BUFFER,'\n');
//cout << buffer;       
        ifstream f((string(getenv("HOME")) + buffer).c_str());
 
        int seqCount = -1;
        pbm.fragments = new vector<string>();
        pbm.description = new vector<string>();

        /* read fragments from input file and store its description in descr[]
           and its sequence in sequence[] */

        char oneLine[2000];
        string sequence;
        string upperSeq;
        while(f.getline(oneLine, 2000))
        {
//cout << oneLine;
            string temp = string(oneLine);
            int rightArrow = temp.find_first_of(">");
            if(rightArrow != -1) //rightArrow is found
            {
                seqCount++;
                if(seqCount != 0)
                    pbm.fragments->push_back(sequence);
                int space = temp.find(" ", rightArrow); 
                pbm.description->push_back(temp.substr(rightArrow+1, space));
                sequence = "";
            }
            else //line doesn't start with rightArrow
            {
                upperSeq = "";
                for(int i = 0; i < temp.length(); i++)
                    upperSeq += toupper(temp.at(i));

                sequence.append(upperSeq);
            }
        }
        pbm.fragments->push_back(sequence);
        f.close();
 
        pbm.numOfFragments = pbm.fragments->size();
        pbm.fragmentLength = new int[pbm.numOfFragments];

        pbm.rcFragments = new vector<string>();

        for(int j = 0; j < pbm.numOfFragments;  j++)
        {
            pbm.rcFragments->push_back(Util::rcSequence(pbm.fragments->at(j)));
//cout << pbm.description->at(j) << " " << pbm.fragments->at(j) << " " << pbm.rcFragments->at(j) << endl;
        }

        int sumLength = 0;
        for(int i = 0; i < pbm.numOfFragments;  i++)
        {
            pbm.fragmentLength[i] = pbm.fragments->at(i).length();
            sumLength += pbm.fragmentLength[i];
        }
        pbm.avgLength = (double)sumLength / (double)pbm.numOfFragments;
 
        pbm.detector = new OverlapDetector(pbm.numOfFragments);
//      pbm.detector->buildScoresTable(pbm.fragments, pbm.rcFragments);
        pbm.detector->readScoreTable();
//      pbm.detector->printScoreTable();
 
        return is;
    }
 
    bool Problem::operator== (const Problem& pbm) const
    {
        // Falta terminar
        return true;
    }
 
    bool Problem::operator!= (const Problem& pbm) const
    {
        return !(*this == pbm);
    }
 
    Direction Problem::direction() const
    {
        return maximize; // F1
//      return minimize; // F2
    }
 
    Problem::~Problem()
    {
        delete detector;
        delete fragments;
        delete rcFragments;
        delete description;
        delete [] fragmentLength;
    }
 
	// Solution --------------------------------------------------------------

    Solution::Solution (const Problem& pbm):_pbm(pbm),fragOrder(new int[pbm.numOfFragments])
    {}
 
    const Problem& Solution::pbm() const
    {
        return _pbm;
    }
 
    Solution::Solution(const Solution& sol):_pbm(sol.pbm())
    {
        if(fragOrder != NULL) delete [] fragOrder;
 
        fragOrder = new int [_pbm.numOfFragments];
 
        for(int i = 0; i < _pbm.numOfFragments; i++)
            fragOrder[i] = sol.fragOrder[i];
    }
 
    istream& operator>> (istream& is, Solution& sol)
    {
        for (int i=0;i<sol.pbm().numOfFragments;i++)
            is >> sol.fragOrder[i];
        return is;
    }
 
    ostream& operator<< (ostream& os, const Solution& sol)
    {
        for (int i=0;i<sol.pbm().numOfFragments;i++)
            os << " " << sol.fragOrder[i];
        return os;
    }
 
    NetStream& operator << (NetStream& ns, const Solution& sol)
    {
        for (int i=0;i<sol.pbm().numOfFragments;i++)
            ns << sol.fragOrder[i];
        return ns;
    }
 
    NetStream& operator >> (NetStream& ns, Solution& sol)
    {
        for (int i=0;i<sol.pbm().numOfFragments;i++)
            ns >> sol.fragOrder[i];
        return ns;
    }
 
    Solution& Solution::operator= (const Solution &sol)
    {
        if(fragOrder != NULL) delete [] fragOrder;
 
        fragOrder = new int [_pbm.numOfFragments];
 
        for(int i = 0; i < _pbm.numOfFragments; i++)
            fragOrder[i] = sol.fragOrder[i];
 
        return *this;
    }
 
    bool Solution::operator== (const Solution& sol) const
    {
        if (sol.pbm() != _pbm) return false;
        for(int i = 0; i < _pbm.numOfFragments; i++)
            if(fragOrder[i] != sol.fragOrder[i]) return false;
        return true;
    }
 
    bool Solution::operator!= (const Solution& sol) const
    {
        return !(*this == sol);
    }
 
    void Solution::initialize()
    {
        int * randNum = new int[_pbm.numOfFragments];
        
        for(int k = 0; k < _pbm.numOfFragments; k++)
        {
            int num = rand_int(0, _pbm.numOfFragments * 2);
            randNum[k] = num;
            fragOrder[k] = k;
        }

        // sort value and store index as fragment order
        for(int i = 0; i < _pbm.numOfFragments-1; i++)
        {
            for(int j = i+1; j < _pbm.numOfFragments; j++)
            {
                if( randNum[i] > randNum[j])
                {
                    int temp = randNum[i];
                    randNum[i] = randNum[j];
                    randNum[j] = temp;

                    temp = fragOrder[i];
                    fragOrder[i] = fragOrder[j];
                    fragOrder[j] = temp;
                }
            }
        }
        delete [] randNum;
    }
 
    double Solution::fitness ()
    {
        int ** score = _pbm.detector->getScoreTable();
        // F1 maximization
        int fit = 0;

        for(int k = 0; k < _pbm.numOfFragments-1; k++)
        {
            int i = fragOrder[k];
            int j = fragOrder[k+1];

            fit += abs(score[i][j]);
        }
        return (double) fit;
        // F2 minimization
/*      int fit = 0;
        int nof = _pbm.numOfFragments;

        for(int i = 0; i < nof; i++)
        {
            int m = fragOrder[i];
            for(int j = 0; j < nof; j++)
            {
                if(i != j)
                {
                    int n = fragOrder[j];
                    if((nof<m) || (nof<n) || (m<0) || (n<0))
                    {
                        cout << "Error en indices" << endl;
                        return infinity();
                    }
                    fit += abs(i-j) * abs(score[m][n]);
                }
            }
        }
        return (double)fit;
*/  }
 
    char *Solution::to_String() const
    {
        return (char *)fragOrder;
    }
 
    void Solution::to_Solution(char *_string_)
    {
        int *ptr=(int *)_string_;
 
        for (int i=0;i<_pbm.numOfFragments;i++)
        {
            fragOrder[i]=*ptr;
            ptr++;
        }
    }
 
    unsigned int Solution::size() const
    {
        return (_pbm.numOfFragments * sizeof(int));
    }
 
 
    int& Solution::fragment(const int index)
    {
        return fragOrder[index];
    }
 
 
    int* Solution::fragments()
    {
        return fragOrder;
    }
 
    Solution::~Solution()
    {
        delete [] fragOrder;
    }

	// UserStatistics -------------------------------------------------------

	UserStatistics::UserStatistics ()
	{}

	ostream& operator<< (ostream& os, const UserStatistics& userstat)
	{
		os << "\n---------------------------------------------------------------" << endl;
		os << "                   STATISTICS OF TRIALS                   	 " << endl;
		os << "------------------------------------------------------------------" << endl;

		for (int i=0;i< userstat.result_trials.size();i++)
		{
			os << endl
			   << "\t" <<  userstat.result_trials[i].trial
			   << "\t" << userstat.result_trials[i].best_cost_trial
			   << "\t\t" << userstat.result_trials[i].nb_evaluation_best_found_trial
			   << "\t\t" << userstat.result_trials[i].initial_temperature
			   << "\t\t" << userstat.result_trials[i].temperature_best_found_trial
			   << "\t\t" << userstat.result_trials[i].time_best_found_trial
			   << "\t\t" << userstat.result_trials[i].time_spent_trial;
		}

		os << endl << "------------------------------------------------------------------" << endl;
		return os;
	}


	UserStatistics& UserStatistics::operator= (const UserStatistics& userstats)
	{
		result_trials=userstats.result_trials;
		return (*this);
	}


	void UserStatistics::update(const Solver& solver)
	{
		if ((solver.pid()!=0) || (solver.end_trial()!=true) 
			|| ((solver.current_iteration()!=solver.setup().max_evaluations())
				&& !TerminateQ(solver.pbm(),solver,solver.setup())))
			return;
		
		struct user_stat *new_stat;
		if ((new_stat=(struct user_stat *)malloc(sizeof(struct user_stat)))==NULL)
			show_message(7);
		new_stat->trial         = solver.current_trial();
		new_stat->nb_evaluation_best_found_trial= solver.iteration_best_found_trial();
 		new_stat->initial_temperature=solver.initial_temperature_trial();
		new_stat->temperature_best_found_trial=solver.temperature_best_found_trial();
		new_stat->best_cost_trial      = solver.current_best_cost();
		new_stat->time_best_found_trial= solver.time_best_found_trial();
		new_stat->time_spent_trial  = solver.time_spent_trial();
		result_trials.append(*new_stat);
	}

	void UserStatistics::clear()
	{
		result_trials.remove();
	}

	UserStatistics::~UserStatistics()
	{
		result_trials.remove();
	}

// DefaultMove  -------------------------------------------------------


	DefaultMove::DefaultMove()
	{}

	DefaultMove::~DefaultMove()
	{}

	void DefaultMove::Apply (Solution& sol) const
	{
        int indsize = sol.pbm().numOfFragments;
        int r1 = rand_int(0, indsize-1);
        int r2 = rand_int(0, indsize-1);

        while(r2 == r1)
        {
            if(r1 == indsize-1) r2 = rand_int(0, indsize-2);
            else r2 = rand_int(r1, indsize-1);
        }

	/*    // swap
        int temp = sol.fragment(r1);
        sol.fragment(r1) = sol.fragment(r2);
        sol.fragment(r2) = temp;
*/
   	 // 2-opt
 	   if(r2 < r1)
   	   {
        int temp = r1;
        r1 = r2;
        r2 = temp;
       }
       while(r2 > r1)
    	{
        int temp = sol.fragment(r1);
        sol.fragment(r1) = sol.fragment(r2);
        sol.fragment(r2) = temp;
        ++r1;
        --r2;
    	}   
	}

	//------------------------------------------------------------------------
	// Specific methods ------------------------------------------------------
	//------------------------------------------------------------------------

	bool TerminateQ (const Problem& pbm, const Solver& solver,
			 const SetUpParams& setup)
	{

		StopCondition_3 stop;
		return stop.EvaluateCondition(pbm,solver,setup);
	}
}


