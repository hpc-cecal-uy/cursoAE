/************************************************
***									  	      *** 
***  Simulated Annealing Skeleton v1.0        *** 
***  User-required classes and methods        ***
***  Developed by: Carlos Cotta Porras        ***
***										      ***
***                                           ***
************************************************/

#include <iostream.h>
#include "SA.hh"
#include "Mallba/random.hh"
#include "StopCondition.hh"

skeleton SA {


// Problem ---------------------------------------------------------------
	Problem::Problem ()
	{}

	ostream& operator<< (ostream& os, const Problem& pbm)
	{
		return os;
	}

	istream& operator>> (istream& is, Problem& pbm)
	{		
		return is;
	}

	Problem& Problem::operator=  (const Problem& pbm)
	{
		return *this;	
	}

	bool Problem::operator== (const Problem& pbm) const
	{
		return true;
	}

	bool Problem::operator!= (const Problem& pbm) const
	{
		return !(*this == pbm);
	}

	Direction Problem::direction() const
	{
		//return maximize;
		return minimize;
	}
	
	Problem::~Problem()
	{}

// Solution --------------------------------------------------------------
  	Solution::Solution (const Problem& pbm):_pbm(pbm)
	{}
                	
	const Problem& Solution::pbm() const
	{
		return _pbm;
	}
	
	Solution::Solution(const Solution& sol):_pbm(sol.pbm())
	{
		*this=sol;
	}

	istream& operator>> (istream& is, Solution& sol)
	{
		return is;
	}

	ostream& operator<< (ostream& os, const Solution& sol)
	{
		return os;
	}

	NetStream& operator << (NetStream& ns, const Solution& sol)
	{
		return ns;
	}
 	
	NetStream& operator >> (NetStream& ns, Solution& sol)
	{
		return ns;
	}

 	Solution& Solution::operator= (const Solution &sol)
	{
		return *this;
	}                            	

	bool Solution::operator== (const Solution& sol) const
	{
		if (sol.pbm() != _pbm) return false;		
		return true;
	}

	bool Solution::operator!= (const Solution& sol) const
	{
		return !(*this == sol);
	}		

	void Solution::initialize()
	{}
	
	double Solution::fitness () const
	{
		return 0.0;
	}

	char *Solution::to_String() const
	{
		return NULL;
	}

	void Solution::to_Solution(char *_routes_)
	{}

	unsigned int Solution::size() const
	{
		return 0;	
	}

	Solution::~Solution()
	{}

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
		// Body of operator
	}

	//------------------------------------------------------------------------
	// Specific methods ------------------------------------------------------
	//------------------------------------------------------------------------

	bool TerminateQ (const Problem& pbm, const Solver& solver,
			 const SetUpParams& setup)
	{

		StopCondition_1 stop;	
		return stop.EvaluateCondition(pbm,solver,setup);
	}
}


