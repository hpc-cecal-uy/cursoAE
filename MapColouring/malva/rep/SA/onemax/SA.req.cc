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

skeleton SA {

	// Problem ---------------------------------------------------------------

	Problem::Problem ():_dimension(0)
	{}

	ostream& operator<< (ostream& os, const Problem& pbm)
	{
		os << endl << endl << "Number of Variables " << pbm._dimension
		   << endl;
		return os;
	}

	istream& operator>> (istream& is, Problem& pbm)
	{
		char buffer[MAX_BUFFER];
		int i;

		is.getline(buffer,MAX_BUFFER,'\n');
		sscanf(buffer,"%d",&pbm._dimension);

		return is;
	}

	bool Problem::operator== (const Problem& pbm) const
	{
		if (_dimension!=pbm.dimension()) return false;
		return true;
	}

	bool Problem::operator!= (const Problem& pbm) const
	{
		return !(*this == pbm);
	}

	Direction Problem::direction() const
	{
		return maximize;
		//return minimize;
	}

	int Problem::dimension() const
	{
		return _dimension;
	}

	double Problem::infinity() const
	{
	    double one=1.0;
	    double zero=0.0;
	    return 0.0 - one/zero;
	}

	Problem::~Problem()
	{
	}

	// Solution --------------------------------------------------------------

	Solution::Solution (const Problem& pbm):_pbm(pbm),_var(pbm.dimension())
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
		for (int i=0;i<sol.pbm().dimension();i++)
			is >> sol._var[i];
		return is;
	}

	ostream& operator<< (ostream& os, const Solution& sol)
	{
		for (int i=0;i<sol.pbm().dimension();i++)
			os << " " << sol._var[i];
		return os;
	}

	NetStream& operator << (NetStream& ns, const Solution& sol)
	{
		for (int i=0;i<sol._var.size();i++)
			ns << sol._var[i];
		return ns;
	}

	NetStream& operator >> (NetStream& ns, Solution& sol)
	{
		for (int i=0;i<sol._var.size();i++)
			ns >> sol._var[i];
		return ns;
	}

 	Solution& Solution::operator= (const Solution &sol)
	{
		_var=sol._var;
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
	{
		for (int i=0;i<_pbm.dimension();i++)
			_var[i]=rand_int(0,1);
	}

	double Solution::fitness () const
	{
        double fitness = 0.0;

		for (int i=0;i<_var.size();i++)
			fitness += _var[i];

		return fitness;
	}

	char *Solution::to_String() const
	{
		return (char *)_var.get_first();
	}

	void Solution::to_Solution(char *_string_)
	{
		int *ptr=(int *)_string_;
		for (int i=0;i<_pbm.dimension();i++)
		{
			_var[i]=*ptr;
			ptr++;
		}
	}

	unsigned int Solution::size() const
	{
		return (_pbm.dimension() * sizeof(int));
	}


	int& Solution::var(const int index)
	{
		return _var[index];
	}


	Rarray<int>& Solution::array_var()
	{
		return _var;
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
		const float probability = 0.03;

		for (int i=0;i<sol.pbm().dimension();i++)
		{
			if (rand01()<=probability)
			{
				if (sol.var(i)==1) sol.var(i)=0;
			 	else sol.var(i)=1;
			}
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


