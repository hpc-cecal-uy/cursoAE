#ifndef INC_SA
#define INC_SA

#include "Mallba/mallba.hh"
#include "Mallba/States.hh"
#include "Mallba/Rarray.h"
#include "Mallba/time.hh"
#include "Mallba/netstream.hh"
#include <math.h>
#include <string.h>

skeleton SA
{

  provides class SetUpParams; 
  provides class Statistics; 
  provides class Move;
  provides class StopCondition;
  provides class Solver;
  provides class Solver_Seq; 
  provides class Solver_Lan; 
  provides class Solver_Wan; 

  requires class Problem;
  requires class Solution;
  requires class StopCondition_1;
  requires class StopCondition_2;
  requires class StopCondition_3;
  requires class DefaultMove;
  requires class UserStatistics;
  requires bool TerminateQ (const Problem& pbm, const Solver& solver, const SetUpParams& setup);

// Problem ----------------------------------------------------------------------------
 
  requires class Problem
  {
	public:
		Problem (); 
		~Problem ();
 
		friend ostream& operator<< (ostream& os, const Problem& pbm); 
		friend istream& operator>> (istream& is, Problem& pbm); 
 
		Problem& operator=  (const Problem& pbm); 
		bool     operator== (const Problem& pbm) const; 
		bool     operator!= (const Problem& pbm) const; 
 
 		Direction direction () const; 

   };

//Solution ----------------------------------------------------------------------------
 
  requires class Solution
  {
	public: 
		Solution (const Problem& pbm);
		Solution (const Solution& sol); 
		~Solution(); 
 
		friend ostream& operator<< (ostream& os, const Solution& sol); 
		friend istream& operator>> (istream& is, Solution& sol); 
		friend NetStream& operator << (NetStream& ns, const Solution& sol); 
		friend NetStream& operator >> (NetStream& ns, Solution& sol); 
 
		const Problem& pbm() const; 
 
		Solution& operator=  (const Solution& sol); 
		bool operator== (const Solution& sol) const; 
		bool operator!= (const Solution& sol) const; 
 
		char *to_String() const; 
		void to_Solution(char *_vertex_);
		unsigned int size() const;

		void initialize(); 
		double fitness () const;

	private:
		const Problem& _pbm;
 };

// UserStatistics ----------------------------------------------------------------------------
 
  requires class UserStatistics
  {
	private:
		struct user_stat
		{
			unsigned int trial;
			double initial_temperature;
			double temperature_best_found_trial;
			unsigned long nb_evaluation_best_found_trial;
			double best_cost_trial;
			float time_best_found_trial;
			float time_spent_trial;
		};

		Rlist<struct user_stat> result_trials;

	public:
		UserStatistics ();
		~UserStatistics(); 
 
		friend ostream& operator<< (ostream& os, const UserStatistics& usertats); 
 
	 	UserStatistics& operator= (const UserStatistics& userstats); 
		void update(const Solver& solver); 
		void clear();
  };

// Move ----------------------------------------------------------------------------------

  provides class Move
  {
	public:
		Move() {}
		virtual ~Move() {}

		virtual void Apply(Solution& sol) const = 0;
  };

// DefaultMove ---------------------------------------------------------------------------------- 
 
  requires class DefaultMove: public Move
  {
	public:
		DefaultMove();
		~DefaultMove();
 
		void Apply(Solution& sol) const;
  };

// SetUpParams -------------------------------------------------------------------------------

  provides class SetUpParams
  {
	private:
		unsigned int    _independent_runs;
		unsigned long    _max_evaluations;
		unsigned int    _MarkovChain_length;		
		double          _temperature_decay;	
		bool		_display_state;

		// for LAN execution configuration
		unsigned long	_refresh_global_state;
		bool		_synchronized;
		unsigned int    _cooperation; 

	public:
		SetUpParams ();

		friend ostream& operator<< (ostream& os, const SetUpParams& setup);
		friend istream& operator>> (istream& is, SetUpParams& setup);

		const unsigned int    independent_runs() const;
		const unsigned long   max_evaluations() const;
		const unsigned int    MarkovChain_length() const;
		const double          temperature_decay() const;
		const bool	      display_state() const;
		const unsigned long   refresh_global_state() const;
		const bool	      synchronized() const;
		const unsigned int    cooperation() const;

		void independent_runs(const unsigned int val);
		void max_evaluations(const unsigned long val);
		void MarkovChain_length(const unsigned int val);
		void temperature_decay(const double val);
		void display_state(const bool val);
		void refresh_global_state(const unsigned long val);
		void synchronized(const bool val);
		void cooperation(const unsigned int val);

		~SetUpParams();
  };

// Statistics ---------------------------------------------------------------------------------

  provides class Statistics
  {
	private:
		struct stat
		{
			unsigned int trial;
			unsigned long nb_evaluations;
			double best_cost;
			double current_cost;
		};

		Rlist<struct stat> stats_data;

	public:
		Statistics();

		friend ostream& operator<< (ostream& os, const Statistics& stats);

		Statistics& operator= (const Statistics& stats);
		void update(const Solver& solver);
		void clear();

		~Statistics();
  };

// Solver  ---------------------------------------------------------------------------------

  provides class Solver
  {
	protected:
		const Problem&     problem;
		const SetUpParams& params;
		UserStatistics 	  _userstat;
		Statistics        _stat;
		Move*  	           move;
		Solution           current;
		double		   curfit;
		Solution           tentative;
		double	           currentTemperature;
		unsigned int 	   k; // to control temperature update.
		StateCenter       _sc;

		float      	  total_time_spent;
		float      	  time_spent_in_trial;
		float 	   	  start_trial;
		float 	   	  start_global;

		bool 		  _end_trial;

		State_Vble        _current_trial;
		State_Vble  	  _current_iteration;
		State_Vble        _current_best_solution;
		State_Vble        _current_best_cost;
		State_Vble        _current_solution;
		State_Vble        _current_cost;

		State_Vble  	  _current_time_spent;
		State_Vble 	  _initial_temperature_trial;
		State_Vble  	  _time_best_found_trial;
		State_Vble	  _iteration_best_found_trial;
		State_Vble	  _temperature_best_found_trial;
		State_Vble  	  _time_spent_trial;
		
		State_Vble	  _trial_best_found;
		State_Vble	  _iteration_best_found;
		State_Vble        _global_best_solution;
		State_Vble        _global_best_cost;
		State_Vble  	  _time_best_found;

		State_Vble        _temperature;
		State_Vble        _display_state;

		const Direction   _direction;

		bool   AcceptQ(double tent, double cur, double temperature);
		double Set_Initial_Temperature(const Problem& pbm);
		void   KeepHistory(const Solution& sol, const double curfit,const float time_spent_trial,const float total_time_spent);

		double UpdateT(double temp, int K);

	public:
		// Constructor - Destructor -------------------------

		Solver (const Problem& pbm, const SetUpParams& setup); 
		virtual ~Solver ();
		virtual int pid() const;
		bool end_trial() const;

		// Execution methods --------------------------------

		// Full execution
		virtual void run () =0;
		virtual void run (unsigned long int nb_evaluations) =0;
		virtual void run (const Solution& sol, unsigned long int nb_evaluations) =0;

		virtual void run (const double initialTemperature) =0;
		virtual void run (const Solution& sol,const double initialTemperature) =0;
		virtual void run (const double initialTemperature, unsigned long int nb_evaluations) =0;
		virtual void run (const Solution& sol,const double initialTemperature, unsigned long int nb_evaluations) =0;

		// Partial execution
		virtual void StartUp () =0;
		virtual void StartUp (const Solution& sol) =0;
		virtual void StartUp (const double initialTemperature) =0;
		virtual void StartUp (const Solution& sol, const double initialTemperature) =0;
		virtual void DoStep () =0;

		// Statistics handling ------------------------------

		const Statistics& statistics() const;
		const UserStatistics& userstatistics () const;
		const SetUpParams& setup() const;
		const Problem& pbm() const;

		// State handling -----------------------------------

		void RefreshState();
		void UpdateFromState();
		StateCenter* GetState();

		unsigned int current_trial() const;
		unsigned long current_iteration() const;
		Solution current_best_solution() const;
		Solution current_solution() const;
		double current_best_cost() const;
		double current_cost() const;
		float  current_time_spent() const;
		float time_best_found_trial() const;
		double initial_temperature_trial() const;
		unsigned int iteration_best_found_trial() const;
		double temperature_best_found_trial() const;
		float time_spent_trial() const;	
		unsigned int trial_best_found() const;
		unsigned int iteration_best_found() const;
		Solution global_best_solution() const;
		double global_best_cost() const;
		float time_best_found() const;
		double temperature() const;
		int display_state() const;

		void current_trial(const unsigned int value);
		void current_iteration(const unsigned long value);
		void current_best_solution(const Solution& sol);
		void current_best_cost(const double value);
		void current_solution(const Solution& sol);
		void current_cost(const double value);
		void current_time_spent(const float value);
		void time_best_found_trial(const float value);
		void initial_temperature_trial(const double temperature);
		void iteration_best_found_trial(const unsigned int value);
		void temperature_best_found_trial(const double value);
		void time_spent_trial(const float value);	
		void trial_best_found(const unsigned int value);
		void iteration_best_found(const unsigned int value);
		void global_best_solution(const Solution& sol);
		void global_best_cost(const double value);
		void time_best_found(const float value);
		void temperature(const double value);
		void display_state(const int value);
		void show_state() const;

		// State handling -----------------------------------
		void SetMove(Move* mov);
  };

  provides class Solver_Seq: public Solver
  {
	public:
		// Constructor - Destructor -------------------------

		Solver_Seq ( const Problem& pbm, const SetUpParams& setup);
		virtual ~Solver_Seq ();

		// Execution methods --------------------------------

		// Full execution
		void run ();
		virtual void run (unsigned long int max_evaluations);
		virtual void run (const Solution& sol, unsigned long int max_evaluations);

		virtual void run (const double initialTemperature);
		virtual void run (const Solution& sol,const double initialTemperature);
		virtual void run (const double initialTemperature, unsigned long int nb_evaluations);
		virtual void run (const Solution& sol,const double initialTemperature, unsigned long int nb_evaluations);

		// Partial execution
		virtual void StartUp ();
		virtual void StartUp (const Solution& sol);
		virtual void StartUp (const double initialTemperature);
		virtual void StartUp (const Solution& sol, const double initialTemperature);
		virtual void DoStep ();  
  };

  provides class Solver_Lan: public Solver
  {
	private:
		NetStream _netstream;
		int mypid;

		void send_local_state_to(int _mypid);
		int receive_local_state_from(int source_pid);

		void check_for_refresh_global_state();
		 
		unsigned int _current_trial;
		unsigned int _current_iteration;
		double _best_cost_trial;
		Solution _best_solution_trial;
		float _time_best_found_in_trial;
		unsigned int _iteration_best_found_in_trial;
		double _temperature_best_found_in_trial;
		
		int cooperation();		
		// Termination phase //
		bool final_phase;
		int acum_evaluations;

	public: 
		Solver_Lan (const Problem& pbm, const SetUpParams& setup,int argc,char **argv);
		virtual ~Solver_Lan (); 
		virtual int pid() const;
		NetStream& netstream();

		// Execution methods --------------------------------

		// Full execution
		void run ();
		virtual void run (unsigned long int max_evaluations);
		virtual void run (const Solution& sol, unsigned long int max_evaluations);

		virtual void run (const double initialTemperature);
		virtual void run (const Solution& sol,const double initialTemperature);
		virtual void run (const double initialTemperature, unsigned long int nb_evaluations);
		virtual void run (const Solution& sol,const double initialTemperature, unsigned long int nb_evaluations);

		// Partial execution
		virtual void StartUp ();
		virtual void StartUp (const Solution& sol);
		virtual void StartUp (const double initialTemperature);
		virtual void StartUp (const Solution& sol, const double initialTemperature);
		virtual void DoStep ();  

		void reset();
  };

  provides class Solver_Wan: public Solver
  {
	private:
		NetStream _netstream;
		int mypid;

		void send_local_state_to(int _mypid);
		int receive_local_state_from(int source_pid);

		void check_for_refresh_global_state();
		 
		unsigned int _current_trial;
		unsigned int _current_iteration;
		double _best_cost_trial;
		Solution _best_solution_trial;
		float _time_best_found_in_trial;
		unsigned int _iteration_best_found_in_trial;
		double _temperature_best_found_in_trial;
		
		int cooperation();		
		// Termination phase //
		bool final_phase;
		int acum_evaluations;

	public: 
		Solver_Wan (const Problem& pbm, const SetUpParams& setup,int argc,char **argv);
		virtual ~Solver_Wan (); 
		virtual int pid() const;
		NetStream& netstream();

		// Execution methods --------------------------------

		// Full execution
		void run ();
		virtual void run (unsigned long int max_evaluations);
		virtual void run (const Solution& sol, unsigned long int max_evaluations);

		virtual void run (const double initialTemperature);
		virtual void run (const Solution& sol,const double initialTemperature);
		virtual void run (const double initialTemperature, unsigned long int nb_evaluations);
		virtual void run (const Solution& sol,const double initialTemperature, unsigned long int nb_evaluations);

		// Partial execution
		virtual void StartUp ();
		virtual void StartUp (const Solution& sol);
		virtual void StartUp (const double initialTemperature);
		virtual void StartUp (const Solution& sol, const double initialTemperature);
		virtual void DoStep ();  

		void reset();
  };

};

#endif
