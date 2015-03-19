/************************************************
***				  	     					  *** 
***  Simulated Annealing Skeleton v1.0        *** 
***  Provided classes and methods             ***
***  Developed by: Carlos Cotta Porras        *** 
***                                           *** 
************************************************/

#include <iostream.h>
#include <math.h>
#include "SA.hh"
#include "Mallba/random.hh"
#include "Mallba/time.hh"

skeleton SA
{

// SetUpParams -----------------------------------------------------------

	SetUpParams::SetUpParams ():
	 _independent_runs(0),
	 _max_evaluations(0),
	 _MarkovChain_length(0),
	 _temperature_decay(0),
	 _refresh_global_state(0),
	 _synchronized(0),
	 _display_state(0),
	 _cooperation(0)
	{}

	istream& operator>> (istream& is, SetUpParams& setup)
	{
		char buffer[MAX_BUFFER]; // current line in the setup file
		char command[50];
		int op;
		double dop;
		short int nb_param=0;
		short int nb_section=0;
		short int nb_LAN_param=0;

		while (is.getline(buffer,MAX_BUFFER,'\n'))
		{
			sscanf(buffer," %s ",command);

			if (!(strcmp(command,"General"))) nb_section=0;
			if (!(strcmp(command,"LAN-configuration"))) nb_section=1;

			if (nb_param==3 && nb_section==0)
			{
				dop=-1;
				sscanf(buffer," %lf ",&dop);
	 			if (dop<0) continue;
			}
			else
			{
				op=-1;
				sscanf(buffer," %ld%*s ",&op);
				if (op<0) continue;
			}

			switch (nb_section)
			{
				case 0: switch (nb_param)
					{
						case 0: setup.independent_runs(op); break;
						case 1: setup.max_evaluations(op); break;
 						case 2: setup.MarkovChain_length(op); break;
						case 3: setup.temperature_decay(dop); break;
						case 4: setup.display_state(op); break;
					}
					nb_param++;
					break;
				case 1: if (nb_LAN_param>=3) break;	
					if (nb_LAN_param==0)	setup.refresh_global_state(op);
					if (nb_LAN_param==1) setup.synchronized(op);
					if (nb_LAN_param==2) setup.cooperation(op);
					nb_LAN_param++;
					break;
			} // end switch
		} // end while
		return is;
	}

	ostream& operator<< (ostream& os, const SetUpParams& setup)
	{
		os << "CONFIGURATION -------------------------------------------" << endl << endl;
		os << "\t" << "Independent runs :   " << setup.independent_runs()    << endl
		   << "\t" << "Evaluation steps:    " << setup.max_evaluations() << endl
		   << "\t" << "Markov-Chain Length: " << setup.MarkovChain_length() << endl
		   << "\t" << "Temperature Decay:   " << setup.temperature_decay() << endl;
		
		if (setup.display_state())
			os << "\t" << "Display state" << endl;
		else
   			os << "\t" << "Not display state" << endl;
		os << endl << "\t" << "LAN configuration:" << endl	
		   << "\t" << "----------------------" << endl << endl
		   << "\t" << "Refresh global state in number of generations: " << setup.refresh_global_state() << endl;
		
		if (setup.synchronized())
			os << "\t" << "Running in synchronous mode" << endl;		
		else
			os << "\t" << "Running in asynchronous mode" << endl;		
		
		if (!setup.cooperation())
			os << "\t" << "Running without cooperation" << endl << endl;
		else
			os << "\t" << "Running with cooperation in " << setup.cooperation() << " iterations. " << endl << endl;		
		  
		os << endl << endl << "END CONFIGURATION -------------------------------------------" << endl << endl;
		return os;
	}

	const unsigned int SetUpParams::independent_runs() const
	{
		return _independent_runs;
	}

	const unsigned long SetUpParams::max_evaluations() const
	{
		return _max_evaluations;
	}

	const unsigned int SetUpParams::MarkovChain_length() const
	{
		return _MarkovChain_length;
	}

	const double  SetUpParams::temperature_decay() const
	{
		return _temperature_decay;
	}
	
	const bool SetUpParams::display_state() const
	{
		return _display_state;
	}

	const unsigned long SetUpParams::refresh_global_state() const
	{ 
		return _refresh_global_state;
	}

	const bool SetUpParams::synchronized() const
	{ 
		return _synchronized;
	}

	const unsigned int SetUpParams::cooperation() const
	{ 
		return _cooperation;
	}

	void SetUpParams::independent_runs(const unsigned int val)
	{
		_independent_runs = val;
	}

	void SetUpParams::max_evaluations(const unsigned long val)
	{
		_max_evaluations= val;
	}
	void SetUpParams::MarkovChain_length(const unsigned int val)
	{
		_MarkovChain_length= val;
	}
	void SetUpParams::temperature_decay(const double val)
	{
		_temperature_decay= val;
	}

	void SetUpParams::display_state(const bool val)
	{
		_display_state=val;
	}

	void SetUpParams::refresh_global_state(const unsigned long val)
	{
		_refresh_global_state=val;
	}

	void SetUpParams::synchronized(const bool val)
	{
		_synchronized=val;
	}

	void SetUpParams::cooperation(const unsigned int val)
	{
		_cooperation=val;
	}

	SetUpParams::~SetUpParams()
	{}

// Statistics ------------------------------------------------------

	Statistics::Statistics()
	{}

	ostream& operator<< (ostream& os, const Statistics& stats)
	{
		int j;
		os << "\n---------------------------------------------------------------" << endl;
		os << "                   STATISTICS OF CURRENT TRIAL                   " << endl;
		os << "------------------------------------------------------------------" << endl;
		for (int i=0;i< stats.stats_data.size();i++)
		{
			os << endl
			   << " Evaluations: " << stats.stats_data[i].nb_evaluations
			   << " Best:        " << stats.stats_data[i].best_cost
			   << " Current:     " << stats.stats_data[i].current_cost;
		}			
		os << endl << "------------------------------------------------------------------" << endl;
		return os;
	}

	Statistics& Statistics::operator= (const Statistics& stats)
	{
		stats_data = stats.stats_data;
		return *this;
	}

	void Statistics::update(const Solver& solver)
	{
		/* struct stat *new_stat;
		 if ((new_stat=(struct stat *)malloc(sizeof(struct stat)))==NULL)
			show_message(7);
		 new_stat->trial         = solver.current_trial(); 
		 new_stat->nb_evaluations= solver.current_iteration();
		 new_stat->best_cost     = solver.current_best_cost();
		 new_stat->current_cost  = solver.current_cost();
		 stats_data.append(*new_stat); */
	}

	Statistics::~Statistics()
	{ 
		stats_data.remove(); 		
	}
	
	void Statistics::clear()
	{
		stats_data.remove();
	}

// Solver (superclass)---------------------------------------------------

       Solver::Solver (const Problem& pbm, const SetUpParams& setup)
	: problem(pbm),
	  params(setup),
	 _stat(),
	 _userstat(),
	 _sc(),
	 _direction(pbm.direction()),
	  current(pbm),
	  tentative(pbm),
	  currentTemperature(0.0),
	  time_spent_in_trial(0.0),	
	  total_time_spent(0.0),
	  start_trial(0.0),
	  start_global(0.0),
	 _current_trial("_current_trial",_sc),
	 _current_iteration("_current_iteration",_sc),
	 _current_best_solution("_current_best_solution",_sc),
	 _current_best_cost("_current_best_cost",_sc),
	 _current_solution("_current_solution",_sc),
	 _current_cost("_current_cost",_sc),
	 _current_time_spent("_current_time_spent",_sc),
	 _initial_temperature_trial("_initial_temperature_trial",_sc),
	 _time_best_found_trial("_time_best_found_trial",_sc),
	 _iteration_best_found_trial("_iteration_best_found_trial",_sc),
	 _temperature_best_found_trial("_temperature_best_found_trial",_sc),
	 _time_spent_trial("_time_spent_trial",_sc),	
	 _trial_best_found("_trial_best_found",_sc),
	 _iteration_best_found("_iteration_best_found;",_sc),
	 _global_best_solution("_global_best_solution",_sc),
	 _global_best_cost("_global_best_cost",_sc),
	 _time_best_found("_time_best_found",_sc),
	 _temperature("_temperature",_sc),
	 _display_state("_display_state",_sc)
      	{
		current_trial(0);
		current_iteration(0);
		current_best_solution(current),
		current_best_cost((-1) * pbm.direction() * infinity());
		current_solution(current);
		current_cost((-1) * pbm.direction() * infinity());
		current_time_spent(total_time_spent);
		initial_temperature_trial(0);
		time_best_found_trial(time_spent_in_trial);	
		iteration_best_found_trial(0);
		temperature_best_found_trial(0.0);
		time_spent_trial(time_spent_in_trial);	
		trial_best_found(0);
		iteration_best_found(0);
		global_best_solution(current);
		global_best_cost((-1) * pbm.direction() * infinity());
		time_best_found(total_time_spent);
		temperature(currentTemperature);
		display_state(setup.display_state());	 

		move = new DefaultMove;
   	}

	int Solver::pid() const
	{
		return 0;
	}

	bool Solver::end_trial() const
	{
		return _end_trial;
	}

	unsigned int Solver::current_trial() const
	{
		unsigned int value=0;
		unsigned long nitems,length;
		_sc.get_contents_state_variable("_current_trial",(char *)&value, nitems, length);
		return value;
	}

	unsigned long Solver::current_iteration() const
	{
		unsigned long value=0;
		unsigned long nitems,length;
		_sc.get_contents_state_variable("_current_iteration",(char *)&value, nitems, length);
		return value;
	}

	Solution Solver::current_best_solution() const
	{
		Solution sol(problem);
		unsigned long nitems,length;
		char data_stored[_current_best_solution.get_nitems() + _current_best_solution.get_length()];
		_sc.get_contents_state_variable("_current_best_solution", data_stored, nitems, length);
		sol.to_Solution(data_stored);
		return sol;
	}

	double Solver::current_best_cost() const
	{
		double value=0.0;
		unsigned long nitems,length;
		_sc.get_contents_state_variable("_current_best_cost",(char *)&value, nitems, length);
		return value;
	}

	double Solver::current_cost() const
	{
		double value=0.0;
		unsigned long nitems,length;
		_sc.get_contents_state_variable("_current_cost",(char *)&value, nitems, length);
		return value;
	}

	Solution Solver::current_solution() const
	{
		Solution sol(problem);
		char data_stored[_current_solution.get_nitems() + _current_solution.get_length()];
		unsigned long nitems,length;
		_sc.get_contents_state_variable("_current_solution",data_stored, nitems, length);
		sol.to_Solution((char *)data_stored);
		return sol;
	}

	float  Solver::current_time_spent() const
	{
		float value=0.0;
		unsigned long nitems,length;
		_current_time_spent.get_contents((char *)&value, nitems, length);
		return value;
	}		

	float Solver::time_best_found_trial() const
	{
		float value=0.0;
		unsigned long nitems,length;
		_time_best_found_trial.get_contents((char *)&value, nitems, length);
		return value;
	}

	unsigned int Solver::iteration_best_found_trial() const
	{
		unsigned int value=0;
		unsigned long nitems,length;
		_iteration_best_found_trial.get_contents((char *)&value, nitems, length);
		return value;
	}

	double Solver::initial_temperature_trial() const
	{
		double value=0.0;
		unsigned long nitems,length;
		_initial_temperature_trial.get_contents((char *)&value, nitems, length);
		return value;
	}

	double Solver::temperature_best_found_trial() const
	{
		double value=0.0;
		unsigned long nitems,length;
		_temperature_best_found_trial.get_contents((char *)&value, nitems, length);
		return value;
	}

	float Solver::time_spent_trial() const
	{
		float value=0.0;
		unsigned long nitems,length;
		_time_spent_trial.get_contents((char *)&value, nitems, length);
		return value;
	}	

	unsigned int Solver::trial_best_found() const
	{
		unsigned int value=0;
		unsigned long nitems,length;
		_trial_best_found.get_contents((char *)&value, nitems, length);
		return value;
	}

	unsigned int Solver::iteration_best_found() const
	{
		unsigned int value=0;
		unsigned long nitems,length;
		_iteration_best_found.get_contents((char *)&value, nitems, length);
		return value;
	}

	Solution Solver::global_best_solution() const
	{
		Solution sol(problem);
		char data_stored[_global_best_solution.get_nitems() + _global_best_solution.get_length()];
		unsigned long nitems,length;
		_sc.get_contents_state_variable("_global_best_solution",data_stored, nitems, length);
		sol.to_Solution(data_stored);
		return sol;
	}

	double Solver::global_best_cost() const
	{
		double value=0.0;
		unsigned long nitems,length;
		_sc.get_contents_state_variable("_global_best_cost",(char *)&value, nitems, length);
		return value;
	}

	float Solver::time_best_found() const
	{
		float value=0.0;
		unsigned long nitems,length;
		_time_best_found.get_contents((char *)&value, nitems, length);
		return value;
	}

	double Solver::temperature() const
	{
		double value=0.0;
		unsigned long nitems,length;
		_sc.get_contents_state_variable("_temperature",(char *)&value, nitems, length);
		return value;
	}

	int Solver::display_state() const
	{
		int value=0;
		unsigned long nitems,length;
		_sc.get_contents_state_variable("_display_state",(char *)&value, nitems, length);
		return value;
	}

	void Solver::current_trial(const unsigned int value)
	{
		_sc.set_contents_state_variable("_current_trial",(char *)&value,1,sizeof(int));
	}

	void Solver::current_iteration(const unsigned long value)
	{
		_sc.set_contents_state_variable("_current_iteration",(char *)&value,1,sizeof(long));
	}

	void Solver::current_best_solution(const Solution& sol)
	{
		_sc.set_contents_state_variable("_current_best_solution",sol.to_String(),1,sol.size());
	}

	void Solver::current_best_cost(const double value)
	{
		_sc.set_contents_state_variable("_current_best_cost",(char *)&value,1,sizeof(double));
	}

	void Solver::current_cost(const double value)
	{
		_sc.set_contents_state_variable("_current_cost",(char *)&value,1,sizeof(double));
	}

	void Solver::current_solution(const Solution& sol)
	{
		_sc.set_contents_state_variable("_current_solution",sol.to_String(),1,sol.size());
	}

	void Solver::current_time_spent(const float value)
	{
		_current_time_spent.set_contents((char *)&value,1,sizeof(float));
	}

	void Solver::time_best_found_trial(const float value)
	{
		_time_best_found_trial.set_contents((char *)&value,1,sizeof(float));
	}

	void Solver::iteration_best_found_trial(const unsigned int value)
	{
		_iteration_best_found_trial.set_contents((char *)&value,1,sizeof(int));
	}	

	void Solver::initial_temperature_trial(const double value)
	{
		_initial_temperature_trial.set_contents((char *)&value,1,sizeof(double));
	}

	void Solver::temperature_best_found_trial(const double value)
	{
		_temperature_best_found_trial.set_contents((char *)&value,1,sizeof(double));
	}

	void Solver::time_spent_trial(const float value)	
	{
		_time_spent_trial.set_contents((char *)&value,1,sizeof(float));
	}
		
	void Solver::trial_best_found(const unsigned int value)
	{
		_trial_best_found.set_contents((char *)&value,1,sizeof(int));
	}

	void Solver::iteration_best_found(const unsigned int value)
	{
		_iteration_best_found.set_contents((char *)&value,1,sizeof(int));
	}

	void Solver::global_best_solution(const Solution& sol)
	{
		_sc.set_contents_state_variable("_global_best_solution",sol.to_String(),1,sol.size());
	}

	void Solver::global_best_cost(const double value)
	{
		_sc.set_contents_state_variable("_global_best_cost",(char *)&value,1,sizeof(double));
	}

	void Solver::time_best_found(const float value)
	{
		_time_best_found.set_contents((char *)&value,1,sizeof(float));
	}

	void Solver::temperature(const double value)
	{
		_sc.set_contents_state_variable("_temperature",(char *)&value,1,sizeof(double));
	}

	void Solver::display_state(const int value)
	{
		_sc.set_contents_state_variable("_display_state",(char *)&value,1,sizeof(int));
	}

	const Statistics& Solver::statistics() const
	{
		return _stat;
	}

	const UserStatistics& Solver::userstatistics() const
	{
		return _userstat;
	}

	const SetUpParams& Solver::setup() const
	{
		return params;
	}

	const Problem& Solver::pbm() const
	{
		return problem;
	}

	void Solver::KeepHistory(const Solution& sol, const double curfit,const float time_spent_in_trial,const float total_time_spent)
	{
                bool betterG=false;
		bool betterT=false;

		switch (_direction)
		{
			case minimize: betterG = (curfit < global_best_cost() || (curfit == global_best_cost() && time_spent_in_trial < time_best_found()));
				       betterT = (curfit < current_best_cost() || (curfit == current_best_cost() && time_spent_in_trial < time_best_found_trial()));
				       break;
			case maximize: betterG = (curfit > global_best_cost() || (curfit == global_best_cost() && time_spent_in_trial < time_best_found()));
				       betterT = (curfit > current_best_cost() || (curfit == current_best_cost() && time_spent_in_trial < time_best_found_trial()));
				       break;
		}

		if (betterT)
		{
			current_best_solution(sol);
			current_best_cost(curfit);
			time_best_found_trial(time_spent_in_trial);
			iteration_best_found_trial(current_iteration());
			temperature_best_found_trial(temperature());
			if (betterG)
			{
				trial_best_found(current_trial());
				iteration_best_found(current_iteration());
				global_best_solution(sol);
				global_best_cost(curfit);
				time_best_found(time_spent_in_trial);
			}
		}
	}

	double Solver::UpdateT(double temp, int K)
	{
		//return temp * params.temperature_decay(); // initial

		/*
		if(K == 1) return temp/log(2);
		else return temp * log(K) / log(K+1);
		*/
		/*
		if(K == 1) return temp/2;
		else return  (temp * K) / (K + 1);
		*/

		 if(K == 1) return temp / exp(2);
		 else return (temp * exp(K)) / exp(K+1);

	}

	StateCenter* Solver::GetState()
	{
		return &_sc;
	}

	void Solver::RefreshState()
	{
		current_solution(current);
		current_cost(curfit);
		current_time_spent(total_time_spent);
		time_spent_trial(time_spent_in_trial);
		temperature(currentTemperature); 
		
		KeepHistory(current,curfit,time_spent_in_trial,total_time_spent);
	}

	void Solver::UpdateFromState() 
	{
		current = current_solution();
		curfit = current_cost();
		total_time_spent=current_time_spent();
		time_spent_in_trial=time_spent_trial();
		currentTemperature = temperature();

		KeepHistory(current,curfit,time_spent_in_trial,total_time_spent);
	}

	void Solver::show_state() const
	{
		cout << endl << "Current trial:     " << current_trial();
		cout << endl << "Current iteration: " << current_iteration();
		cout << endl << "Current Temperature:       " << temperature ();
		cout << endl << "Current cost:      " << current_cost();
		cout << endl << "Best cost in trial:   " << current_best_cost();
		cout << endl << "Time of best solution found in trial: " << time_best_found_trial();
		cout << endl << "Iteration of best solution found in trial: " << iteration_best_found_trial();
		cout << endl << "Initial temperature in trial: " << initial_temperature_trial();
		cout << endl << "Temperature of best solution found in trial: " << temperature_best_found_trial();
		cout << endl << "Time spent in trial: " << time_spent_trial();
		cout << endl << "Global best cost:    " << global_best_cost();
		cout << endl << "Trial of best global solution found: " << trial_best_found();
		cout << endl << "Iteration of best global solution found: " << iteration_best_found();
		cout << endl << "Time of global best solution found:   " << time_best_found();
		// cout << endl << "Current solution:    " << current_solution();
		// cout << endl << "Best solution of trial: " << current_best_solution();
		// cout << endl << "Global solution:     " << global_best_solution() << endl;
		cout << endl << endl << "Current time spent (so far): " << current_time_spent() << endl;
	}

	Solver::~Solver()
	{
		_sc.removeAll();
		delete move;
	}

	bool Solver::AcceptQ (double tent, double cur, double temperature)
	{
		if (_direction==minimize)
			
			return  (tent < cur) ||
				((rand01()*(1+exp((tent-cur)/temperature)))<2.0);
			
		else

			return  (tent > cur) ||
				((rand01()*(1+exp((cur-tent)/temperature)))<2.0);					
	}

	double Solver::Set_Initial_Temperature(const Problem& pbm)
	{
		const double beta  = 1.05;
		const double test  = 10;
		const double acrat = .8;
		const double T     = 1.0;

		Solution current (pbm);
		Solution newsol (pbm);
		double ac;
		double fit;
		double temperature = T;

		do
		{
			temperature *= beta;
			ac = 0;
			current.initialize();
			fit = current.fitness();
			for (int i=0; i<test; i++)
			{
				newsol = current;
				move->Apply(newsol);
				if (AcceptQ(newsol.fitness(),fit,temperature))
					ac += 1.0/test;
			}
		} while (ac < acrat);

		initial_temperature_trial(temperature);
		return temperature;

	}

	void Solver::SetMove (Move* mov)
	{
		delete move;
		move = mov;
	}

	// Solver sequencial -----------------------------------------------------

	Solver_Seq::Solver_Seq (const Problem& pbm, const SetUpParams& setup)
	: Solver(pbm,setup)
	{
		random_seed(time(0));	
		_end_trial=true;
	}

	Solver_Seq::~Solver_Seq ()
	{}

	void Solver_Seq::StartUp()
	{
		Solution sol(problem);

		sol.initialize();

		StartUp(sol, Set_Initial_Temperature(problem));
	}

	void Solver_Seq::StartUp(const Solution& sol)
	{
		StartUp(sol, Set_Initial_Temperature(problem));
	}

	void Solver_Seq::StartUp(const double initialTemperature)
	{
		Solution sol(problem);

		sol.initialize();

		StartUp(sol, initialTemperature);
	}

	void Solver_Seq::StartUp(const Solution& sol, const double initialTemperature)
	{
		start_trial=_used_time();
		start_global=total_time_spent;

		current_trial(current_trial()+1);
		current_iteration(0);

		k = 0;
		time_spent_in_trial=0.0;
		current = sol;
	  	curfit = current.fitness();
		current_best_cost((-1) * problem.direction() * infinity());
		currentTemperature = initialTemperature;
		iteration_best_found_trial(0);
		temperature_best_found_trial(0);
		time_best_found_trial(0.0);	
		 
		RefreshState();

		_stat.update(*this);
		_userstat.update(*this);
		  
		if (display_state())
			show_state();
	}
	
	void Solver_Seq::DoStep()
	{
		current_iteration(current_iteration()+1);

		tentative = current;
		move->Apply(tentative);
		double tentfit = tentative.fitness();

		if (AcceptQ(tentfit,curfit, currentTemperature))
		{
			current = tentative;
		  	curfit = tentfit;
		}
		 
		k++;
		if (k>=params.MarkovChain_length())
		{
			currentTemperature = UpdateT(currentTemperature,current_iteration()/params.MarkovChain_length());
			k = 0;
		}

		time_spent_in_trial = _used_time(start_trial);
		total_time_spent    = start_global + time_spent_in_trial;

		RefreshState();

		_stat.update(*this);
		_userstat.update(*this);

		if (display_state())
			show_state();
	}


	void Solver_Seq::run (unsigned long int max_evaluations)
	{
		StartUp();

		while (current_iteration()<max_evaluations && !TerminateQ(problem, *this, params))
			DoStep();		
	}

	void Solver_Seq::run (const Solution& sol, unsigned long int max_evaluations)
	{
		StartUp(sol);

		while ((current_iteration()<max_evaluations) && !TerminateQ(problem, *this, params))
 			DoStep();		
	}

	void Solver_Seq::run (const double initialTemperature)
	{
		while (current_trial() < params.independent_runs())
			run(initialTemperature,params.max_evaluations());
	}

	void Solver_Seq::run (const Solution& sol,const double initialTemperature)
	{
		while (current_trial() < params.independent_runs())
			run(sol,initialTemperature,params.max_evaluations());
	}

	void Solver_Seq::run (const double initialTemperature, unsigned long int max_evaluations)
	{
		StartUp(initialTemperature);

		while ((current_iteration()<max_evaluations) && !TerminateQ(problem, *this, params))
 			DoStep();		
	}

	void Solver_Seq::run (const Solution& sol,const double initialTemperature, unsigned long int max_evaluations)
	{
		StartUp(sol,initialTemperature);

		while ((current_iteration()<max_evaluations) && !TerminateQ(problem, *this, params))
 			DoStep();
	}

	void Solver_Seq::run ()
	{
		while (current_trial() < params.independent_runs())
			run(params.max_evaluations());
	}
                	
	// Solver LAN ----------------------------------------------------------- 
 
	Solver_Lan::Solver_Lan (const Problem& pbm, const SetUpParams& setup,int argc,char **argv):
		    _best_solution_trial(pbm), 
	            Solver(pbm,setup),_netstream(),
			// Termination phase //
			final_phase(false),acum_evaluations(0)
	{
		NetStream::init(argc,argv);
		mypid=_netstream.my_pid();
		random_seed(time(0) + (mypid+1));
		if (mypid!=0)
			_netstream << set_source(0) << set_target(0);
	} 

	Solver_Lan::~Solver_Lan ()
	{
		NetStream::finalize();
	} 

	int Solver_Lan::pid() const
	{
		return mypid;
	}

	NetStream& Solver_Lan::netstream()
	{
		return _netstream;
	}	

	void Solver_Lan::StartUp()
	{
		Solution sol(problem);
		
		sol.initialize();

		StartUp(sol, Set_Initial_Temperature(problem));
	}

	void Solver_Lan::StartUp(const Solution& sol)
	{
		StartUp(sol, Set_Initial_Temperature(problem));
	}
	
	void Solver_Lan::StartUp(const double initialTemperature)
	{
		Solution sol(problem);
		
		sol.initialize();

		StartUp(sol, initialTemperature);
	}

	void Solver_Lan::StartUp(const Solution& sol, const double initialTemperature)
	{
		
		_netstream << barrier;

		start_trial=_used_time();
		start_global=total_time_spent;
		// Termination phase //		
		final_phase = false;
		acum_evaluations = 0;

		_end_trial=false;

		current_trial(current_trial()+1);
		current_iteration(0);

		k = 0;
		time_spent_in_trial=0.0;
		current_best_cost((-1) * problem.direction() * infinity());
		iteration_best_found_trial(0);
		temperature_best_found_trial(0);
		time_best_found_trial(0.0);	
		time_spent_trial(0.0);
		
		if (mypid!=0)
		{
                	current = sol;
		  	curfit = current.fitness();
			currentTemperature = initialTemperature;

			RefreshState();

			send_local_state_to(mypid);

			_stat.update(*this);
			_userstat.update(*this);			  	

			// if (display_state()) show_state();
		}
	}

	void update(Direction direction, Solution &solution_received,double cost_received, Solution &solution_to_send, double &best_cost)
	{
		switch (direction) 
		{
			case minimize:  if (cost_received < best_cost)
					{	
						solution_to_send=solution_received;
						best_cost=cost_received;
					}
			case maximize:  if (cost_received > best_cost)
					{	
						solution_to_send=solution_received;
						best_cost=cost_received;
					}
		}
	}

	int Solver_Lan::cooperation()
	{
		int received=false;
		Solution solution_received(problem), solution_to_send(problem);
		double cost_received=0, cost_to_send=0;
		int pending=false;
		int pid_source,pid_target;
		
		if (mypid!=0)
		{
			if (((int)current_iteration() % params.refresh_global_state()) ==0) // isnot the server
			{
				_netstream << set_target(0);
				send_local_state_to(mypid);
			}

			if (params.cooperation()==0) return received;
			pid_target=mypid+1;
			if (pid_target==_netstream.pnumber()) pid_target=1;
			_netstream << set_target(pid_target);

			pid_source=mypid-1;
			if (pid_source==0) pid_source=_netstream.pnumber()-1;
			_netstream << set_source(pid_source);
			
		 	if  ((((int)current_iteration() % params.cooperation())==0) && (params.max_evaluations()!=current_iteration()))
			{
				if (mypid==1)
					_netstream << current_best_cost() << current_best_solution();
			
				if (params.synchronized())
				{
					_netstream << wait(regular);
					_netstream >> cost_received >> solution_received;
					received=true;
				} 
			}

			if (!params.synchronized())
			{
				int pending=false;
				_netstream._probe(regular,pending);
				if (pending)
				{
					_netstream >> cost_received >> solution_received;
					received=true;
				} 	
			} 

			if (mypid!=1 && received)
			{
				solution_to_send = current_best_solution();
				cost_to_send=current_best_cost();

				if (received)
				{
					update(problem.direction(),solution_received, cost_received, solution_to_send,cost_to_send);
				}
				_netstream << cost_to_send << solution_to_send;
			}

			if (received)
			{
				tentative=solution_received;		  
				curfit=cost_received;
			}
		
			_netstream << set_target(0);
		}				

		return received;
	}		

	void Solver_Lan::DoStep()
	{
		current_iteration(current_iteration()+1);
		// Termination phase //
		_netstream << set_source(0);
		int pending;
		_netstream._probe(packed, pending);
		if(pending)
		{
				Solution sol(problem);
				 _netstream << pack_begin >> sol << pack_end;
				final_phase = true;
		}
		////////////////////////
		
		int received=cooperation();
		
		if (!received)
		{
			tentative = current;
			move->Apply(tentative);
		}

		double tentfit = tentative.fitness();

		if (AcceptQ(tentfit,curfit, currentTemperature))
		{
			current = tentative;
			curfit = tentfit;
		}

		k++;
		if (k>=params.MarkovChain_length())
		{
			currentTemperature = UpdateT(currentTemperature,current_iteration()/params.MarkovChain_length());
			k = 0;
		}

		time_spent_in_trial = _used_time(start_trial);
		total_time_spent    = start_global + time_spent_in_trial;

		RefreshState();

		_stat.update(*this);
		_userstat.update(*this);

		// if (display_state())
		//	show_state();
	}

	void Solver_Lan::send_local_state_to(int _mypid)
	{
		_netstream << set_target(0);
		_netstream << pack_begin
		 	   << _mypid
			   << current_trial()
			   << current_iteration()
			   << current_solution()
			   << current_cost()
			   << current_best_solution()
			   << current_best_cost()
			   << time_best_found_trial() 	
			   << iteration_best_found_trial() 	
			   << temperature_best_found_trial() 	
			   << temperature()
			   << pack_end;
	}

	int Solver_Lan::receive_local_state_from(int source_pid)
	{
		_netstream << set_source(source_pid);
		int received_pid=0;

		_netstream._wait(packed);
		_netstream << pack_begin
		  	   >> received_pid
		  	   >> _current_trial
			   >> _current_iteration 
		  	   >> current
		  	   >> curfit
			   >> _best_solution_trial
			   >> _best_cost_trial
			   >> _time_best_found_in_trial 
		  	   >> _iteration_best_found_in_trial
			   >> _temperature_best_found_in_trial
			   >> currentTemperature
			   << pack_end;

		return received_pid;
	}

	void Solver_Lan::check_for_refresh_global_state() // Executed in process with pid 0
	{
		unsigned int nb_finalized_processes=0;
		int received_pid;
		int nb_proc=_netstream.pnumber();	
			
  		while (!_end_trial)
		{
		 // checking for all processes
			
			received_pid=0;
			received_pid=receive_local_state_from(MPI_ANY_SOURCE);
			
			// refresh the global state with received data ( a local state )		
			current_trial(_current_trial);
			current_iteration(_iteration_best_found_in_trial);
			temperature(_temperature_best_found_in_trial);

			KeepHistory(_best_solution_trial,_best_cost_trial,_time_best_found_in_trial,start_global + _time_best_found_in_trial);	

			if (received_pid==-1) 
			{
				// Termination phase //
				if(!final_phase && TerminateQ(problem,*this,params))
				{
					Solution sol(problem);
					acum_evaluations = params.max_evaluations() * nb_finalized_processes;								
					for(int i = 1; i < _netstream.pnumber(); i++)
					{
						_netstream << set_target(i);
						_netstream << pack_begin << sol << pack_end;
					}
					final_phase = true;
				}
				nb_finalized_processes++;
				acum_evaluations += _iteration_best_found_in_trial;
			}
			
			if (nb_finalized_processes==nb_proc-1) _end_trial=true;

			current_iteration(_current_iteration);
			temperature(currentTemperature);

			time_spent_in_trial = _used_time(start_trial);
			total_time_spent    = start_global + time_spent_in_trial;
			
			RefreshState();

			_stat.update(*this);
			//_userstat.update(*this);			  	 	

			// display current global state 		  					
			if (display_state()) show_state();
		} // end while
		
		// Actualización de las estadísticas // Termination phase //
		iteration_best_found_trial(acum_evaluations/(_netstream.pnumber()-1));
		
		bool betterG=false;
		double best_cost = current_best_cost();
    	switch (problem.direction())
		{
			case minimize: betterG = (best_cost < global_best_cost() || (best_cost == global_best_cost() && time_best_found_trial() <= time_best_found()));				       
				       break;
			case maximize: betterG = (best_cost > global_best_cost() || (best_cost == global_best_cost() && time_best_found_trial() <= time_best_found()));
				       break;
		}

		if (betterG)
			iteration_best_found(iteration_best_found_trial());

		RefreshState();

		_stat.update(*this);
		_userstat.update(*this);
		// display the global state at the end of the current trial
		if (display_state()) show_state();
	}

	void Solver_Lan::run ()
	{
		while (current_trial() < params.independent_runs())
			run(params.max_evaluations());
	}
 
	void Solver_Lan::run (const unsigned long int  max_evaluations)
	{
		StartUp();
		if (mypid!=0)
		{
			while (!final_phase && (current_iteration() <  max_evaluations) && !(TerminateQ(problem,*this,params)))
				DoStep();
			send_local_state_to(-1);
		}
		else
		{
			check_for_refresh_global_state();
		}

		_netstream << barrier; 
		reset();
	}	

	void Solver_Lan::run (const Solution& sol, unsigned long int max_evaluations)
	{
		StartUp(sol);
		if (mypid!=0)
		{
			while (!final_phase && (current_iteration() < max_evaluations) && !(TerminateQ(problem,*this,params)))
				DoStep();
			send_local_state_to(-1);
		}
		else
		{
			check_for_refresh_global_state();
		}

		_netstream << barrier; 
		reset();
	}

	void Solver_Lan::run (const double initialTemperature)
	{
		while (current_trial() < params.independent_runs())
			run(initialTemperature,params.max_evaluations());
	}

	void Solver_Lan::run (const Solution& sol,const double initialTemperature)
	{
		while (current_trial() < params.independent_runs())
			run(sol,initialTemperature,params.max_evaluations());
	}

	void Solver_Lan::run (const double initialTemperature, unsigned long int max_evaluations)
	{
		StartUp(initialTemperature);
		if (mypid!=0)
		{
			while (!final_phase && (current_iteration() <  max_evaluations) && !(TerminateQ(problem,*this,params)))
				DoStep();
			send_local_state_to(-1);
		}
		else
		{
			check_for_refresh_global_state();
		}

		_netstream << barrier; 
		reset();
	}

	void Solver_Lan::run (const Solution& sol,const double initialTemperature, unsigned long int max_evaluations)
	{
		StartUp(sol,initialTemperature);
		if (mypid!=0)
		{
			while (!final_phase && (current_iteration() <  max_evaluations) && !(TerminateQ(problem,*this,params)))
				DoStep();
				send_local_state_to(-1);
		}
		else
		{
			check_for_refresh_global_state();
		}

		_netstream << barrier; 
		reset();
	}

	void Solver_Lan::reset()
	{
		Solution left_solution(problem);
		double left_cost;
		_netstream << set_source(MPI_ANY_SOURCE);
		if (mypid!=0)
		{			
			int pendingr = false;
			int pendingp = false;
			do
			{
				pendingr = false;
				pendingp = false;
			 	_netstream._probe(regular,pendingr);
				_netstream._probe(packed,pendingp);			 	
				if (pendingr) _netstream >> left_cost >> left_solution;				
				if (pendingp) _netstream << pack_begin >> left_solution << pack_end;
			 } while (pendingr || pendingp);
		}
		_netstream << barrier; 
	}	

	// Solver WAN ------------------------------------------------------------

	Solver_Wan::Solver_Wan (const Problem& pbm, const SetUpParams& setup,int argc,char **argv):
		    _best_solution_trial(pbm), 
	            Solver(pbm,setup),_netstream(),
			// Termination phase //
			final_phase(false),acum_evaluations(0)
	{
		NetStream::init(argc,argv);
		mypid=_netstream.my_pid();
		random_seed(time(0) + (mypid+1));
		if (mypid!=0)
			_netstream << set_source(0) << set_target(0);
	} 

	Solver_Wan::~Solver_Wan ()
	{
		NetStream::finalize();
	} 

	int Solver_Wan::pid() const
	{
		return mypid;
	}

	NetStream& Solver_Wan::netstream()
	{
		return _netstream;
	}	

	void Solver_Wan::StartUp()
	{
		Solution sol(problem);
		
		sol.initialize();

		StartUp(sol, Set_Initial_Temperature(problem));
	}

	void Solver_Wan::StartUp(const Solution& sol)
	{
		StartUp(sol, Set_Initial_Temperature(problem));
	}
	
	void Solver_Wan::StartUp(const double initialTemperature)
	{
		Solution sol(problem);
		
		sol.initialize();

		StartUp(sol, initialTemperature);
	}

	void Solver_Wan::StartUp(const Solution& sol, const double initialTemperature)
	{
		
		_netstream << barrier;

		start_trial=_used_time();
		start_global=total_time_spent;
		// Termination phase //		
		final_phase = false;
		acum_evaluations = 0;

		_end_trial=false;

		current_trial(current_trial()+1);
		current_iteration(0);

		k = 0;
		time_spent_in_trial=0.0;
		current_best_cost((-1) * problem.direction() * infinity());
		iteration_best_found_trial(0);
		temperature_best_found_trial(0);
		time_best_found_trial(0.0);	
		time_spent_trial(0.0);
		
		if (mypid!=0)
		{
                	current = sol;
		  	curfit = current.fitness();
			currentTemperature = initialTemperature;

			RefreshState();

			send_local_state_to(mypid);

			_stat.update(*this);
			_userstat.update(*this);			  	

			// if (display_state()) show_state();
		}
	}

	int Solver_Wan::cooperation()
	{
		int received=false;
		Solution solution_received(problem), solution_to_send(problem);
		double cost_received=0, cost_to_send=0;
		int pending=false;
		int pid_source,pid_target;
		
		if (mypid!=0)
		{
			if (((int)current_iteration() % params.refresh_global_state()) ==0) // isnot the server
			{
				_netstream << set_target(0);
				send_local_state_to(mypid);
			}

			if (params.cooperation()==0) return received;
			pid_target=mypid+1;
			if (pid_target==_netstream.pnumber()) pid_target=1;
			_netstream << set_target(pid_target);

			pid_source=mypid-1;
			if (pid_source==0) pid_source=_netstream.pnumber()-1;
			_netstream << set_source(pid_source);
			
		 	if  ((((int)current_iteration() % params.cooperation())==0) && (params.max_evaluations()!=current_iteration()))
			{
				if (mypid==1)
					_netstream << current_best_cost() << current_best_solution();
			
				if (params.synchronized())
				{
					_netstream << wait(regular);
					_netstream >> cost_received >> solution_received;
					received=true;
				} 
			}

			if (!params.synchronized())
			{
				int pending=false;
				_netstream._probe(regular,pending);
				if (pending)
				{
					_netstream >> cost_received >> solution_received;
					received=true;
				} 	
			} 

			if (mypid!=1 && received)
			{
				solution_to_send = current_best_solution();
				cost_to_send=current_best_cost();

				if (received)
				{
					update(problem.direction(),solution_received, cost_received, solution_to_send,cost_to_send);
				}
				_netstream << cost_to_send << solution_to_send;
			}

			if (received)
			{
				tentative=solution_received;		  
				curfit=cost_received;
			}
		
			_netstream << set_target(0);
		}				

		return received;
	}		

	void Solver_Wan::DoStep()
	{
		current_iteration(current_iteration()+1);
		// Termination phase //
		_netstream << set_source(0);
		int pending;
		_netstream._probe(packed, pending);
		if(pending)
		{
				Solution sol(problem);
				 _netstream << pack_begin >> sol << pack_end;
				final_phase = true;
		}
		////////////////////////
		
		int received=cooperation();
		
		if (!received)
		{
			tentative = current;
			move->Apply(tentative);
		}

		double tentfit = tentative.fitness();

		if (AcceptQ(tentfit,curfit, currentTemperature))
		{
			current = tentative;
			curfit = tentfit;
		}

		k++;
		if (k>=params.MarkovChain_length())
		{
			currentTemperature = UpdateT(currentTemperature,current_iteration()/params.MarkovChain_length());
			k = 0;
		}

		time_spent_in_trial = _used_time(start_trial);
		total_time_spent    = start_global + time_spent_in_trial;

		RefreshState();

		_stat.update(*this);
		_userstat.update(*this);

		// if (display_state())
		//	show_state();
	}

	void Solver_Wan::send_local_state_to(int _mypid)
	{
		_netstream << set_target(0);
		_netstream << pack_begin
		 	   << _mypid
			   << current_trial()
			   << current_iteration()
			   << current_solution()
			   << current_cost()
			   << current_best_solution()
			   << current_best_cost()
			   << time_best_found_trial() 	
			   << iteration_best_found_trial() 	
			   << temperature_best_found_trial() 	
			   << temperature()
			   << pack_end;
	}

	int Solver_Wan::receive_local_state_from(int source_pid)
	{
		_netstream << set_source(source_pid);
		int received_pid=0;

		_netstream._wait(packed);
		_netstream << pack_begin
		  	   >> received_pid
		  	   >> _current_trial
			   >> _current_iteration 
		  	   >> current
		  	   >> curfit
			   >> _best_solution_trial
			   >> _best_cost_trial
			   >> _time_best_found_in_trial 
		  	   >> _iteration_best_found_in_trial
			   >> _temperature_best_found_in_trial
			   >> currentTemperature
			   << pack_end;

		return received_pid;
	}

	void Solver_Wan::check_for_refresh_global_state() // Executed in process with pid 0
	{
		unsigned int nb_finalized_processes=0;
		int received_pid;
		int nb_proc=_netstream.pnumber();	
			
  		while (!_end_trial)
		{
		 // checking for all processes
			
			received_pid=0;
			received_pid=receive_local_state_from(MPI_ANY_SOURCE);
			
			// refresh the global state with received data ( a local state )		
			current_trial(_current_trial);
			current_iteration(_iteration_best_found_in_trial);
			temperature(_temperature_best_found_in_trial);

			KeepHistory(_best_solution_trial,_best_cost_trial,_time_best_found_in_trial,start_global + _time_best_found_in_trial);	

			if (received_pid==-1) 
			{
				// Termination phase //
				if(!final_phase && TerminateQ(problem,*this,params))
				{
					Solution sol(problem);
					acum_evaluations = params.max_evaluations() * nb_finalized_processes;								
					for(int i = 1; i < _netstream.pnumber(); i++)
					{
						_netstream << set_target(i);
						_netstream << pack_begin << sol << pack_end;
					}
					final_phase = true;
				}
				nb_finalized_processes++;
				acum_evaluations += _iteration_best_found_in_trial;
			}
			
			if (nb_finalized_processes==nb_proc-1) _end_trial=true;

			current_iteration(_current_iteration);
			temperature(currentTemperature);

			time_spent_in_trial = _used_time(start_trial);
			total_time_spent    = start_global + time_spent_in_trial;
			
			RefreshState();

			_stat.update(*this);
			//_userstat.update(*this);			  	 	

			// display current global state 		  					
			if (display_state()) show_state();
		} // end while
		
		// Actualización de las estadísticas // Termination phase //
		iteration_best_found_trial(acum_evaluations/(_netstream.pnumber()-1));
		
		bool betterG=false;
		double best_cost = current_best_cost();
    	switch (problem.direction())
		{
			case minimize: betterG = (best_cost < global_best_cost() || (best_cost == global_best_cost() && time_best_found_trial() <= time_best_found()));				       
				       break;
			case maximize: betterG = (best_cost > global_best_cost() || (best_cost == global_best_cost() && time_best_found_trial() <= time_best_found()));
				       break;
		}

		if (betterG)
			iteration_best_found(iteration_best_found_trial());

		RefreshState();

		_stat.update(*this);
		_userstat.update(*this);
		
		// display the global state at the end of the current trial
		if (display_state()) show_state();
	}

	void Solver_Wan::run ()
	{
		while (current_trial() < params.independent_runs())
			run(params.max_evaluations());
	}
 
	void Solver_Wan::run (const unsigned long int  max_evaluations)
	{
		StartUp();
		if (mypid!=0)
		{
			while (!final_phase && (current_iteration() <  max_evaluations) && !(TerminateQ(problem,*this,params)))
				DoStep();
			send_local_state_to(-1);
		}
		else
		{
			check_for_refresh_global_state();
		}

		_netstream << barrier; 
		reset();
	}	

	void Solver_Wan::run (const Solution& sol, unsigned long int max_evaluations)
	{
		StartUp(sol);
		if (mypid!=0)
		{
			while (!final_phase && (current_iteration() < max_evaluations) && !(TerminateQ(problem,*this,params)))
				DoStep();
			send_local_state_to(-1);
		}
		else
		{
			check_for_refresh_global_state();
		}

		_netstream << barrier; 
		reset();
	}

	void Solver_Wan::run (const double initialTemperature)
	{
		while (current_trial() < params.independent_runs())
			run(initialTemperature,params.max_evaluations());
	}

	void Solver_Wan::run (const Solution& sol,const double initialTemperature)
	{
		while (current_trial() < params.independent_runs())
			run(sol,initialTemperature,params.max_evaluations());
	}

	void Solver_Wan::run (const double initialTemperature, unsigned long int max_evaluations)
	{
		StartUp(initialTemperature);
		if (mypid!=0)
		{
			while (!final_phase && (current_iteration() <  max_evaluations) && !(TerminateQ(problem,*this,params)))
				DoStep();
			send_local_state_to(-1);
		}
		else
		{
			check_for_refresh_global_state();
		}

		_netstream << barrier; 
		reset();
	}

	void Solver_Wan::run (const Solution& sol,const double initialTemperature, unsigned long int max_evaluations)
	{
		StartUp(sol,initialTemperature);
		if (mypid!=0)
		{
			while (!final_phase && (current_iteration() <  max_evaluations) && !(TerminateQ(problem,*this,params)))
				DoStep();
				send_local_state_to(-1);
		}
		else
		{
			check_for_refresh_global_state();
		}

		_netstream << barrier; 
		reset();
	}

	void Solver_Wan::reset()
	{
		Solution left_solution(problem);
		double left_cost;
		_netstream << set_source(MPI_ANY_SOURCE);
		if (mypid!=0)
		{			
			int pendingr = false;
			int pendingp = false;
			do
			{
				pendingr = false;
				pendingp = false;
			 	_netstream._probe(regular,pendingr);
				_netstream._probe(packed,pendingp);			 	
				if (pendingr) _netstream >> left_cost >> left_solution;				
				if (pendingp) _netstream << pack_begin >> left_solution << pack_end;
			 } while (pendingr || pendingp);
		}
		_netstream << barrier; 
	}
};
	
