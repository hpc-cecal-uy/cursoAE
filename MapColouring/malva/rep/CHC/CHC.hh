/*********************************************************************************************************
***												       												   ***
***				      					CHC Skeleton v1.5	 			       						   ***
***				     		   Developed by: Gabriel Jesús Luque Polo			    				   ***
***												       												   ***
***									  Last Update: 30-01-2004										   ***
***												       												   ***
**********************************************************************************************************/

#ifndef INC_CHC
#define INC_CHC
#include "CHCstructures.hh"

skeleton CHC
{
#define MAX_USER_OP 5
#define MAX_PROB_PER_OP 5

  provides class SetUpParams;
  provides class Statistics;
  provides class Population;
  provides class Intra_Operator;
  provides class Crossover;
  provides class Diverge;
  provides class Inter_Operator;
  provides class Migration;
  provides class Selection;
  provides class Selection_New_Population;
  provides class Selection_Parents;
  provides class Operator_Pool;
  provides class Solver;
  provides class Solver_Seq;
  provides class Solver_Lan;
  provides class Solver_Wan;
  provides class StopCondition;

  requires class Problem;
  requires class Solution;
  requires class UserStatistics;
  requires class User_Operator;
  requires class StopCondition_1;
  requires bool terminateQ (const Problem& pbm, const Solver& solver, const SetUpParams& setup);

// Problem ----------------------------------------------------------------------------

  requires class Problem
  {
	public:
		Problem ();
		~Problem();

		friend ostream& operator<< (ostream& os, const Problem& pbm);
		friend istream& operator>> (istream& is, Problem& pbm);

		Problem& operator=  (const Problem& pbm);
		bool operator== (const Problem& pbm) const;
		bool operator!= (const Problem& pbm) const;

		Direction direction () const;
		int dimension() const;

	private:

		int _dimension;
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
		void to_Solution(char *_cadena_);

		void initialize();
		double fitness () const;
		unsigned int size() const;

		int lengthInBits() const;
		void flip(const int index);
		bool equalb(const int index,Solution &s);
		void swap(const int index, Solution &s);
		void invalid();

		int& var(const int index);
		Rarray<int>& array_var();

	private:
		Rarray<int> _var;
		const Problem& _pbm;

  };

// UserStatistics ----------------------------------------------------------------------------

  requires class UserStatistics
  {
	private:
		struct user_stat
		{
			unsigned int trial;
			unsigned long nb_evaluation_best_found_trial;
			unsigned long nb_iteration_best_found_trial;
			double best_cost_trial;
			double worst_cost_trial;
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

// Intra_Operator ( clase abstracta ) --------------------------------------------------------------

  provides class Intra_Operator
  {
	protected:
		unsigned int _number_operator;
		float *probability;

	public:
		Intra_Operator(const unsigned int _number_op);
		virtual ~Intra_Operator();

		static Intra_Operator *create(const unsigned int _number_op);
		friend ostream& operator<< (ostream& os, const Intra_Operator& intra);

		virtual void execute(Rarray<Solution*>& sols) const=0;
		virtual void setup(char line[MAX_BUFFER]) = 0;
		unsigned int number_operator() const;

		virtual void RefreshState(const StateCenter& _sc) const=0;
		virtual void UpdateFromState(const StateCenter& _sc)=0;
  };

// Crossover ----------------------------------------------------------------------------------

  provides class Crossover: public Intra_Operator
  {
	public:
		Crossover();
		virtual ~Crossover();

		friend ostream& operator << (ostream& os, const Crossover&  cross);

		void cross(Solution &sol1,Solution &sol2) const;
		virtual void execute(Rarray<Solution*>& sols) const;
		virtual void setup(char line[MAX_BUFFER]);

		virtual void RefreshState(const StateCenter& _sc) const;
		virtual void UpdateFromState(const StateCenter& _sc);
  };

// Diverge ----------------------------------------------------------------------------------

  provides class Diverge: public Intra_Operator
  {
	public:
		Diverge();
		virtual ~Diverge();

		friend ostream& operator<< (ostream& os, const Diverge&  diverge);

		void diverge(Solution& s) const;
		// applies mutation over all solutions in array sols
		virtual void execute(Rarray<Solution*>& sols) const;
		virtual void setup(char line[MAX_BUFFER]);

		virtual void RefreshState(const StateCenter& _sc) const;
		virtual void UpdateFromState(const StateCenter& _sc);
  };

// User_Operator ----------------------------------------------------------------------------------

  requires class User_Operator: public Intra_Operator
  {
	public:
		User_Operator(const unsigned int _number_op);
		virtual ~User_Operator();

		static Intra_Operator *create(const unsigned int _number_op);
		friend ostream& operator<< (ostream& os, const User_Operator&  User_Operator);

		// applies mutation over all solutions in array sols
		virtual void execute(Rarray<Solution*>& sols) const;
		virtual void setup(char line[MAX_BUFFER]);

		virtual void RefreshState(const StateCenter& _sc) const;
		virtual void UpdateFromState(const StateCenter& _sc);
  };

// StopCondition -----------------------------------------------------------------------------------
  provides class StopCondition
  {
	public:
		StopCondition();
		virtual bool EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup)=0;
		~StopCondition();
  };

// StopCondition_1 -----------------------------------------------------------------------------------
  requires class StopCondition_1 : public StopCondition
  {
	public:
		StopCondition_1();
		virtual bool EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup);
		~StopCondition_1();
  };

// SetUpParams -------------------------------------------------------------------------------

  provides class SetUpParams
  {
	private:
		unsigned int    _independent_runs;
		unsigned long   _nb_evolution_steps;
		unsigned long   _nb_iteration_steps;
		unsigned int    _population_size;		// number of individuals
		bool 			_display_state;

		unsigned long   _refresh_global_state;
		bool 			_synchronized;
		unsigned int 	_check_asynchronous;

		// selection of parents and offsprings
		mutable unsigned int _select_parents;
		mutable unsigned int _select_offsprings;

		Rlist<unsigned int> _intra_operators;
		Rlist<unsigned int> _inter_operators;

		Operator_Pool& _pool;

	public:
		SetUpParams (Operator_Pool& pool);
		Operator_Pool& pool() const;

 		friend ostream& operator<< (ostream& os, const SetUpParams& setup);
		friend istream& operator>> (istream& is, SetUpParams& setup);

		const unsigned int  independent_runs() const;
		const unsigned long nb_evolution_steps() const;
		const unsigned int  population_size() const;
		const bool 			display_state() const;
		const unsigned long refresh_global_state() const;
		const bool 			synchronized() const;
		const unsigned int  check_asynchronous() const;

		void independent_runs(const unsigned int val);
		void nb_evolution_steps(const unsigned long val);
		void population_size(const unsigned int val);
		void display_state(const bool val);
		void refresh_global_state(const unsigned long val);
		void synchronized(const bool val);
		void check_asynchronous(const unsigned int val);

        // gets the i-th operator of inter-population
        const unsigned int  inter_operator_index(const unsigned int index) const;
		const unsigned int  inter_operators_size() const;

        // gets the i-th operator of intra-population
		const unsigned int intra_operator_index(const unsigned int index) const;
		const unsigned int intra_operators_size() const;

		const unsigned int select_parents() const;
		const unsigned int select_offsprings() const;

		void select_parents(const unsigned int val);
		void select_offsprings(const unsigned int val);

		void RefreshState(const StateCenter& _sc) const;
		void UpdateFromState(const StateCenter& _sc) const;

		~SetUpParams();
  };

// Statistics ---------------------------------------------------------------------------------

  provides class Statistics
  {
	private:
		struct stat
		{
			unsigned int trial;
			unsigned long nb_generation;
			unsigned long nb_evaluation;
			double best_cost;
			double global_best_cost;
			double average_cost;
			double standard_deviation;
		};

		Rlist<struct stat> stats_data;

	public:
		Statistics();
		~Statistics();

		friend ostream& operator<< (ostream& os, const Statistics& stats);

	 	Statistics& operator= (const Statistics& stats);
		void update(const Solver& solver);
		void clear();
  };

// Population ---------------------------------------------------------------------------------

  provides class Population
  {
	private:
		Rarray<Solution*> _parents;     // individuals in population
		Rarray<Solution*> _offsprings;  // offsprings of current population
		Rarray<Solution*> _new_parents; // individuals of previous population
		Rarray<struct individual> _fitness_values;
		Rarray<struct individual> _fitness_aux;
		const SetUpParams& _setup;
		unsigned int _upper_cost,_lower_cost; // lower and upper fitness of individuals in population
		unsigned int _evaluations;
		double _average_cost;

	public:
		inline void Evaluate(Solution *s, struct individual & _f);
		
		Population(const Problem& pbm,const SetUpParams& setup); // crea un array de objetos population;
		~Population();

		friend ostream& operator<< (ostream& os, const Population& population);
		friend istream& operator>> (istream& is, Population& population);
		Population& operator= (const Population& pop);
		const SetUpParams& setup() const;
		const Problem& pbm() const;
	  	void initialize();

		// Generate a new pool of individuals in population
		void evolution();

		// interchange solutions between island
		void interchange(const unsigned long current_generation, NetStream& channel);

		// creates a array with fitness of all individuals in population and its position in the population
    	void evaluate_parents();

		// creates a array with fitness of all individuals and offsprings in population and its position in the population
		void evaluate_offsprings();

		// selects parents to creates offsprings
		void select_parents();

		// selects individuals for the new population
		void select_offsprings();

	 	const Rarray<Solution*>& parents() const;
		const Rarray<Solution*>& offsprings() const;
		Rarray<struct individual>& fitness_values();

		unsigned int upper_cost() const;
		unsigned int lower_cost() const;
		unsigned int evaluations() const;
		Solution& solution(const unsigned int index) const;
		double fitness(const unsigned int index) const;

		double best_cost() const;
		double worst_cost() const;
		Solution& best_solution() const;
		Solution& worst_solution() const;
		double average_cost() const;
		double standard_deviation() const;
  };

// Inter_Operator ( abstract )-----------------------------------------------------------

  provides class Inter_Operator
  {
	protected:
		unsigned int migration_rate;
		unsigned int migration_size;
		unsigned int migration_selection_1;
	   	unsigned int migration_selection_2;
	   	unsigned int migration_selection_conf_1;
	   	unsigned int migration_selection_conf_2;

		unsigned int _number_operator;
	   	const Direction direction;

	public:
		Inter_Operator(const unsigned int _number_op, const Direction dir);
	   	virtual ~Inter_Operator();

	   	friend ostream& operator<< (ostream& os, const Inter_Operator& inter);

	   	virtual void execute(Population& pop,const unsigned long current_generation,NetStream& _netstream,const bool synchronized,const unsigned int check_asyncrhonous) const=0;
	   	virtual void setup(char line[MAX_BUFFER]); 
	   	unsigned int number_operator() const;

	   	virtual void RefreshState(const StateCenter& _sc) const;
	   	virtual void UpdateFromState(const StateCenter& _sc);
  };

// Migration: public Inter_Operator -----------------------------------------------------------

  provides class Migration: public Inter_Operator
  {
	public:
		Migration(const Direction dir);
		virtual ~Migration();

		friend ostream& operator<< (ostream& os, const Migration& migration);

	   	virtual void execute(Population& pop,const unsigned long current_generation,NetStream& _netstream,const bool synchronized,const unsigned int check_asyncrhonous) const;
  };

// Selection ---------------------------------------------------------

  provides class Selection
  {
	protected:
		unsigned int _number_selection;
	  	const Direction direction;

	public:

		Selection(const Direction dir);
 	  	Selection(const unsigned int _number_sel, const Direction dir);
		virtual ~Selection();

		friend ostream& operator<< (ostream& os, const Selection& sel);

       	virtual void prepare(Rarray<struct individual>& fitness_values,const bool remplace); // const;
	  	virtual struct individual select_one(const Rarray<Solution*>& to_select_1,const Rarray<Solution*>& to_select_2,const Rarray<struct individual>& fitness_values,const unsigned int dummy,const bool remplace) const;
	  	unsigned int number_selection() const;

		virtual void setup(char line[MAX_BUFFER]);
		virtual void RefreshState(const StateCenter& _sc) const;
		virtual void UpdateFromState(const StateCenter& _sc);

  };

// Selection_Parents  ---------------------------------------------------------------------------------

  provides class Selection_Parents: public Selection
  {
	private:
		mutable unsigned int selection_position;

	public:
		Selection_Parents(const Direction dir);
		virtual ~Selection_Parents();

		friend ostream& operator<< (ostream& os, const Selection_Parents& sel);

        virtual void prepare(Rarray<struct individual>& fitness_values,const bool remplace); // const;

		virtual struct individual select_one(const Rarray<Solution*>& to_select_1,const Rarray<Solution*>& to_select_2,const Rarray<struct individual>& fitness_values,const unsigned int param,const bool remplace) const;
  };

// Selection_New_Population ---------------------------------------------------------------------------

  provides class Selection_New_Population: public Selection
  {
	private:
		mutable unsigned int selection_position;
		mutable int d;
		Intra_Operator *diverge;
		float r;

	public:
		Selection_New_Population(const Direction dir);
		virtual ~Selection_New_Population();

		friend ostream& operator<< (ostream& os, const Selection_New_Population& sel);

		virtual void prepare(Rarray<struct individual>& fitness_values,const bool remplace); // const;

		virtual struct individual select_one(const Rarray<Solution*>& to_select_1,const Rarray<Solution*>& to_select_2,const Rarray<struct individual>& fitness_values,const unsigned int param,const bool remplace) const;

		virtual void setup(char line[MAX_BUFFER]);
		virtual void RefreshState(const StateCenter& _sc) const;
		virtual void UpdateFromState(const StateCenter& _sc);
  };

// Selection_Tournament  ---------------------------------------------------------------------------------

  provides class Selection_Tournament: public Selection
  {
  	public:
		Selection_Tournament(const Direction dir);
		virtual ~Selection_Tournament();

		friend ostream& operator<< (ostream& os, const Selection_Tournament& sel);

		virtual struct individual select_one(const Rarray<Solution*>& to_select_1,const Rarray<Solution*>& to_select_2,const Rarray<struct individual>& fitness_values,const unsigned int tourment_size,const bool remplace) const;
  };

// Selection_Roulette_Wheel  ---------------------------------------------------------------------------------
  
  provides class Selection_Roulette_Wheel: public Selection
  {
	public:
		Selection_Roulette_Wheel(const Direction);
		virtual ~Selection_Roulette_Wheel();

		friend ostream& operator<< (ostream& os, const Selection_Roulette_Wheel& sel);

       	virtual void prepare(Rarray<struct individual>& fitness_values,const bool remplace); // const;	
		virtual struct individual select_one(const Rarray<Solution*>& to_select_1,const Rarray<Solution*>& to_select_2,const Rarray<struct individual>& fitness_values,const unsigned int dummy,const bool remplace) const;
  };

// Selection_Rank  ---------------------------------------------------------------------------------

  provides class Selection_Rank: public Selection
  {
	public:
		Selection_Rank(const Direction dir);
		Selection_Rank(const unsigned int _number_sel, const Direction dir);
		virtual ~Selection_Rank();

		friend ostream& operator<< (ostream& os, const Selection_Rank& sel);

        virtual void prepare(Rarray<struct individual>& fitness_values,const bool remplace); // const;
		virtual void reset();
	
		virtual struct individual select_one(const Rarray<Solution*>& to_select_1,const Rarray<Solution*>& to_select_2,const Rarray<struct individual>& fitness_values,const unsigned int portion,const bool remplace) const;
  };

// Selection_Best  ---------------------------------------------------------------------------------

  provides class Selection_Best: public Selection_Rank
  {
	private:
		mutable unsigned int selection_best_position;		

	public:
		Selection_Best(const Direction);
		virtual ~Selection_Best();

		friend ostream& operator<< (ostream& os, const Selection_Best& sel);

		virtual void reset();
       	virtual struct individual select_one(const Rarray<Solution*>& to_select_1,const Rarray<Solution*>& to_select_2,const Rarray<struct individual>& fitness_values,const unsigned int position,const bool remplace) const;
  };

// Selection_Worst  ---------------------------------------------------------------------------------

  provides class Selection_Worst: public Selection_Rank
  {
	private:
		mutable unsigned int selection_worst_position;		

	public:
		Selection_Worst(const Direction);
		virtual ~Selection_Worst();

		friend ostream& operator<< (ostream& os, const Selection_Worst& sel);

		virtual void reset();
		virtual struct individual select_one(const Rarray<Solution*>& to_select_1,const Rarray<Solution*>& to_select_2,const Rarray<struct individual>& fitness_values,const unsigned int position,const bool remplace) const;
  };

// Operator_Pool -------------------------------------------------------------------------

 // pool with all operators and selections that can be chosen in the setup file
  provides class Operator_Pool
  {
	private:
		mutable Rlist<Intra_Operator>	_intra_operators;
 		Rlist<Selection>				_selectors;
 		Rlist<Inter_Operator> 			_inter_operators;

	public:
		Operator_Pool(const Problem& pbm);
		~Operator_Pool();

		Intra_Operator& intra_operator(const unsigned int index) const;
		Rlist<Intra_Operator>& intra_operators() const;
		Selection& selector(const unsigned int index) const;
		const Rlist<Selection>& selectors() const;
		Inter_Operator& inter_operator(const unsigned int index) const;
		const Rlist<Inter_Operator>& inter_operators() const;

  };

// Solver  ---------------------------------------------------------------------------------

  provides class Solver
  {
	protected:
		const Problem&     problem;
		const SetUpParams& params;
 		UserStatistics 	   _userstat;
		Statistics         _stat;
		Population 		   current_population;
		StateCenter		   _sc;

		double 	   best_cost;
		double 	   worst_cost;
		Solution   best_solution;
		double     average_cost;
		double     standard_deviation;
		float      total_time_spent;
		float      time_spent_in_trial;
		float 	   start_trial;
		float 	   start_global;

		bool 	    _end_trial;

		State_Vble  _current_trial;
		State_Vble  _current_iteration;
		State_Vble  _current_evaluations;

		State_Vble  _current_best_solution;
		State_Vble  _current_best_cost;
		State_Vble  _current_worst_cost;
		State_Vble  _current_average_cost;
		State_Vble  _current_standard_deviation;
		State_Vble  _current_time_spent;

	 	State_Vble  _best_solution_trial;
		State_Vble  _best_cost_trial;
		State_Vble  _worst_cost_trial;
		State_Vble  _iteration_best_found_in_trial;
		State_Vble  _evaluations_best_found_in_trial;
		State_Vble  _time_best_found_trial;
		State_Vble  _time_spent_trial;

		State_Vble  _trial_best_found;
		State_Vble  _iteration_best_found;
		State_Vble  _evaluations_best_found;
		State_Vble  _global_best_solution;
		State_Vble  _global_best_cost;
		State_Vble  _global_worst_cost;
		State_Vble  _time_best_found;

		State_Vble _crossover_probability; // probability of applying the operator over population
		State_Vble _diverge_probability; // probability of applying the operator over population
		State_Vble _user_op_probability[MAX_USER_OP]; // probabilities of user operators
	   	State_Vble _migration_rate;
		State_Vble _migration_size;
		State_Vble _migration_selection_1;
		State_Vble _migration_selection_2;
		State_Vble _migration_selection_conf_1;
		State_Vble _migration_selection_conf_2;
		State_Vble _select_parents;
		State_Vble _select_offsprings;
		State_Vble _parameter_select_new_pop;

		State_Vble _display_state;

	public:
		Solver (const Problem& pbm, const SetUpParams& setup);
		virtual ~Solver ();

		virtual int pid() const;
		bool end_trial() const;
		void end_trial(bool et);

		// Execution methods -----------------------------------------------------------------------

		// Full execution
		virtual void run () =0;
		virtual void run (const unsigned long int nb_generations) =0;
		virtual void run (const Population& pop,const unsigned long int nb_generations) =0;

		//Partial execution
		virtual void StartUp()=0;
		virtual void StartUp(const Population& pop)=0;

		virtual void DoStep()=0;

		// Statistics handling ----------------------------------------------------------------------

		Statistics& statistics();
 		UserStatistics& userstatistics ();
		Population& population();
		const SetUpParams& setup() const;
		const Problem& pbm() const;

		// State handling ---------------------------------------------------------------------------

		void RefreshState();
		void RefreshCfgState();
		void UpdateFromState();
		void UpdateFromCfgState();
		StateCenter* GetState();

		unsigned int current_trial() const;
		unsigned long current_iteration() const;
		unsigned long current_evaluations() const;
		Solution current_best_solution() const;
		double current_best_cost() const;
		double current_worst_cost() const;
		double current_average_cost() const;
		double current_standard_deviation() const;
		float  current_time_spent() const;
		Solution  best_solution_trial() const;
		double best_cost_trial() const;
		double worst_cost_trial() const;
		unsigned int iteration_best_found_in_trial() const;
		unsigned int evaluations_best_found_in_trial() const;
		float time_best_found_trial() const;
		float time_spent_trial() const;
		unsigned int trial_best_found() const;
		unsigned int iteration_best_found() const;
		unsigned int evaluations_best_found() const;
		Solution global_best_solution() const;
		double global_best_cost() const;
		double global_worst_cost() const;
		float time_best_found() const;
		int display_state() const;

		float *crossover_probability() const;
		float *diverge_probability() const;
		float *user_op_probability(const int index) const;
        unsigned int migration_rate() const;
        unsigned int migration_size() const;
        unsigned int migration_selection_1() const;
        unsigned int migration_selection_2() const;
        unsigned int migration_selection_conf_1() const;
        unsigned int migration_selection_conf_2() const;
		unsigned int select_parents() const;
 		unsigned int select_offprings() const;
		float parameter_select_new_pop() const;

		void current_trial(const unsigned int value);
		void current_iteration(const unsigned long value);
		void current_evaluations(const unsigned long value);
		void current_best_solution(const Solution& sol);
		void current_best_cost(const double value);
		void current_worst_cost(const double value);
		void current_average_cost(const double value);
		void current_standard_deviation(const double value);
		void current_time_spent(const float value);
		void best_solution_trial(const Solution& sol);
		void best_cost_trial(const double value);
		void worst_cost_trial(const double value);
		void iteration_best_found_in_trial(const unsigned int value);
		void evaluations_best_found_in_trial(const unsigned int value);
		void time_best_found_trial(const float value);
		void time_spent_trial(const float value);
		void trial_best_found(const unsigned int value);
		void iteration_best_found(const unsigned int  value);
		void evaluations_best_found(const unsigned int  value);
		void global_best_solution(const Solution& sol);
		void global_best_cost(const double value);
		void global_worst_cost(const double value);
		void time_best_found(const float value);
		void display_state(const int value);

		void crossover_probability(const float *probability);
		void diverge_probability(const float *probability);
		void user_op_probability(const int index,const float *probability);
		void migration_rate(const unsigned int rate);
		void migration_size(const unsigned int size);
		void migration_selection_1(const unsigned int seleciton_1);
		void migration_selection_2(const unsigned int selection_2);
		void migration_selection_conf_1(const unsigned int selection_conf_1);
		void migration_selection_conf_2(const unsigned int selection_conf_2);
		void select_parents(const unsigned int selection);
 		void select_offsprings(const unsigned int selection);
		void parameter_select_new_pop(const float value);

		void KeepHistory(const Solution& best_sol,const double best_cost,const double worst_cost,const float time_spent_trial,const float total_time_spent);

	 	void show_state() const;
  };

  provides class Solver_Seq: public Solver
  {
	public:
		Solver_Seq ( const Problem& pbm, const SetUpParams& setup);
		virtual ~Solver_Seq ();

		// Execution methods -----------------------------------------------------------------------

		// Full execution
		virtual void run ();
		virtual void run (const unsigned long int nb_generations);
		virtual void run (const Population& pop,const unsigned long int nb_generations);

		//Partial execution
		virtual void StartUp();
		virtual void StartUp(const Population& pop);

		virtual void DoStep();
  };

  provides class Solver_Lan: public Solver
  {
	private:
		NetStream _netstream;
		int mypid;

		void send_local_state_to(int _mypid);
		int receive_local_state();

		void check_for_refresh_global_state();

		unsigned int _current_trial;
 		unsigned long _current_iteration;
 		unsigned long _current_evaluations;
		double _best_cost_trial;
		Solution _best_solution_trial;
		double _worst_cost_trial;
		float _time_best_found_in_trial;
		unsigned long _iteration_best_found_in_trial;
		unsigned long _evaluations_best_found_in_trial;

		// Termination phase //
		bool final_phase;
		int acum_evaluations;
		int acum_iterations;

	public:
		Solver_Lan (const Problem& pbm, const SetUpParams& setup,int argc,char **argv);
		virtual ~Solver_Lan ();
		virtual int pid() const;
		NetStream& netstream();

		// Execution methods -----------------------------------------------------------------------

		// Full execution
		virtual void run ();
		virtual void run (const unsigned long int nb_generations);
		virtual void run (const Population& pop,const unsigned long int nb_generations);

		//Partial execution
		virtual void StartUp();
		virtual void StartUp(const Population& pop);

		virtual void DoStep();

		void reset();
  };

  provides class Solver_Wan: public Solver
  {
	private:
		NetStream _netstream;
		int mypid;

		void send_local_state_to(int _mypid);
		int receive_local_state();

		void check_for_refresh_global_state();

		unsigned int _current_trial;
 		unsigned long _current_iteration;
 		unsigned long _current_evaluations;
		double _best_cost_trial;
		Solution _best_solution_trial;
		double _worst_cost_trial;
		float _time_best_found_in_trial;
		unsigned long _iteration_best_found_in_trial;
		unsigned long _evaluations_best_found_in_trial;

		// Termination phase //
		bool final_phase;
		int acum_evaluations;
		int acum_iterations;

	public:
		Solver_Wan (const Problem& pbm, const SetUpParams& setup,int argc,char **argv);
		virtual ~Solver_Wan ();
		virtual int pid() const;
		NetStream& netstream();

		// Execution methods -----------------------------------------------------------------------

		// Full execution
		virtual void run ();
		virtual void run (const unsigned long int nb_generations);
		virtual void run (const Population& pop,const unsigned long int nb_generations);

		//Partial execution
		virtual void StartUp();
		virtual void StartUp(const Population& pop);

		virtual void DoStep();

		void reset();
  };

}

#endif
