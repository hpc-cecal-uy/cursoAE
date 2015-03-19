/************************************************
***				  	      ***
***  Simulated Annealing Skeleton v1.0        *** 
***  User-required classes and methods        ***
***  Developed by: Carlos Cotta Porras        ***
***					      ***
***                                           ***
************************************************/

#include <iostream.h>
#include "SA.hh"
#include "Mallba/random.hh"
#include "StopCondition.hh"

skeleton SA {


// Problem ---------------------------------------------------------------
	Problem::Problem ():_nCustomers(0),_capacity(0),_distance(NULL),
			    _demand(NULL),_service_time(0),_maxRouteTime(0.0)
	{}

	ostream& operator<< (ostream& os, const Problem& pbm)
	{
		os << endl << endl << "  Vehicle capacity: " << pbm._capacity
		   << endl << "  Number of Customers: " << pbm._nCustomers  << endl
		   << "  Service time: " << pbm._service_time << endl
		   << "  Max Time for any route: " << pbm._maxRouteTime << endl
		   << "  Distances: " << endl;

		for(int i = 0; i < (pbm._nCustomers + 1) ; i++)
		{
			for(int j = 0; j < pbm._nCustomers; j++)
				os << " " << pbm.distance(i,j);
			os << endl;
		}

		os << endl << "  Demand for each customer: " << endl;

		for(int i = 0; i < (pbm._nCustomers + 1)  ; i++)
		{
			os << "  " << i << "\t" << pbm._demand[i] << endl;
		}

		return os;
	}

	istream& operator>> (istream& is, Problem& pbm)
	{
		char buffer[MAX_BUFFER];
		int *x;
		int *y;

                is.getline(buffer,MAX_BUFFER,'\n');
		sscanf(buffer," %d %d %lf %d %lf",&pbm._nCustomers,&pbm._capacity,&pbm._maxRouteTime,&pbm._service_time,&pbm._bestCost);

		if( ((pbm._distance = new double *[pbm._nCustomers + 1]) == NULL)
		 || ((pbm._demand = new int[pbm._nCustomers + 1]) == NULL)
		 || ((x = new int[pbm._nCustomers + 1]) == NULL)
		 || ((y = new int[pbm._nCustomers + 1]) == NULL))
			show_message(7);

		// Read depot coordenates
		if( ((pbm._distance[0] = new double[pbm._nCustomers+1]) == NULL))
			show_message(7);

	 	is.getline(buffer,MAX_BUFFER,'\n');
	 	sscanf(buffer," %d %d",&(x[0]),&(y[0]));
	 	pbm._demand[0] = 0;

	 	// Read customers coordenates
	 	for(int i = 1; i < (pbm._nCustomers + 1); i++)
		{
			if( ((pbm._distance[i] = new double[pbm._nCustomers+1]) == NULL))
				show_message(7);

			is.getline(buffer,MAX_BUFFER,'\n');
			sscanf(buffer," %d %d %d", &(x[i]),&(y[i]),&(pbm._demand[i]));
                }

                pbm.genDistances(x,y);

                delete [] x;
                delete [] y;

		return is;
	}

	void Problem::genDistances(int *x,int *y)
	{
                for(int i = 0; i < (_nCustomers + 1); i++)
                	for(int j = 0; j < (_nCustomers +1) ; j++)
                 		_distance[i][j] = sqrt( pow( (double) (x[j] - x[i]),2 ) +
						  pow( (double) (y[j] - y[i]),2 ) ) + _service_time ;
	}

	Problem& Problem::operator=  (const Problem& pbm)
	{
		if(_distance != NULL)
		{
			for(int i= 0;i < (_nCustomers+1); i++)
				if(_distance[i] != NULL) delete [] _distance[i];
			delete [] _distance;
		}

		if(_demand != NULL) delete [] _demand;

		if( ((_distance = new double*[pbm._nCustomers]) == NULL)
		 || ((_demand = new  int[pbm._nCustomers + 1]) == NULL))
		  	show_message(7);

		_nCustomers = pbm.nCustomers();
		_capacity = pbm.capacity();
		_service_time = pbm.service_time();
		_maxRouteTime = pbm.maxRouteTime();

		for(int i = 0; i < (_nCustomers+1); i++)
		{
			_demand[i] = pbm.demand(i);

			if( ((_distance[i] =  new double[_nCustomers+1]) == NULL))
		 		show_message(7);

			for(int j = 0; j < (_nCustomers+1); i++)
			{
				_distance[i][j] = pbm.distance(i,j);
			}
		}

		return *this;
	}

	bool Problem::operator== (const Problem& pbm) const
	{
		if( (_maxRouteTime != pbm.maxRouteTime()) || (_nCustomers != pbm.nCustomers())
		 || (_capacity != pbm.capacity()) || (_service_time != pbm.service_time()))
		 	return false;

		for(int i = 0; i < (_nCustomers+1); i++)
		{
			if(_demand[i] != pbm.demand(i))
			 	return false;

			for(int j = 0; j < (_nCustomers+1); i++)
			{
				if(distance(i,j) != pbm.distance(i,j)) return false;
			}
		}

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

	double Problem::maxRouteTime() const
	{
		return _maxRouteTime;
	}

	double Problem::bestCost() const
	{
		return _bestCost;
	}

	int Problem::nCustomers() const
	{
		return _nCustomers;
	}

	int Problem::capacity() const
	{
		return _capacity;
	}

	double Problem::distance(const int i, const int j) const
	{
		return _distance[i][j];
	}

	int Problem::demand(const int i) const
	{
		return _demand[i];
	}

	int Problem::service_time() const
	{
		return _service_time;
	}

	Problem::~Problem()
	{
		if(_distance != NULL)
		{
			for(int i= 0;i < (_nCustomers+1); i++)
				if(_distance[i] != NULL) delete [] _distance[i];
			delete [] _distance;
		}

		if(_demand != NULL) delete [] _demand;

	}

// Solution --------------------------------------------------------------
  	Solution::Solution (const Problem& pbm):_pbm(pbm),_routes(pbm.nCustomers())
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
		for (int i=0;i<sol.pbm().nCustomers(); i++)
		{
			is >> sol._routes[i];
		}
		return is;
	}

	ostream& operator<< (ostream& os, const Solution& sol)
	{
		for (int i=0;i<sol.pbm().nCustomers(); i++)
		{
			os << (sol._routes[i]<10?" ":"") << sol._routes[i] << " ";
		}

		return os;
	}

	void	Solution::print()
	{
		int ultimo = 0;
		double dist = 0.0;
		int cap = _pbm.capacity();

		register int c_actual;
		register int i = 0;

		int nRoute = 1;

		cout << " Ruta " << nRoute << " =  0 ";
		while(i < _pbm.nCustomers())
		{
			c_actual = _routes[i];

			if( ((dist + _pbm.distance(ultimo,c_actual) + _pbm.distance(c_actual,0)) > _pbm.maxRouteTime())
			 || (( cap - _pbm.demand(c_actual)) < 0)
			 // add in version 1.0.1
 			 || ((_pbm.distance(ultimo,0)  + _pbm.distance(0,c_actual)) < _pbm.distance(ultimo,c_actual))
 			 )
			{
					nRoute++;
					cout << "0 -> dist =  " << (dist  + _pbm.distance(ultimo,0)) << " capacidad sobra = " << cap
					   << endl << "Ruta " << nRoute << " = 0 ";
					ultimo = 0;
					dist = 0.0;
					cap = _pbm.capacity();
			}
			else
			{
				dist += _pbm.distance(ultimo,c_actual);
				cap -= _pbm.demand(c_actual);
				ultimo = c_actual;
				cout << ultimo << " ";
				i++;
			}
		}

		cout << "0 -> dist =  " << (dist  + _pbm.distance(ultimo,0)) << " capacidad sobra = " << cap
		     << endl;
	}

	NetStream& operator << (NetStream& ns, const Solution& sol)
	{
		for (int i=0;i<sol.pbm().nCustomers();i++)
			ns << sol._routes[i];
		return ns;
	}

	NetStream& operator >> (NetStream& ns, Solution& sol)
	{
		for (int i=0;i<sol.pbm().nCustomers();i++)
			ns >> sol._routes[i];
		return ns;
	}

 	Solution& Solution::operator= (const Solution &sol)
	{
		_routes = sol._routes;
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
		int aux,ind1,ind2;

		int max = _pbm.nCustomers();

		for(int i = 0; i < max; i++)
			_routes[i] = i+1;

		cout << endl;

		for(int i=0;i< (max*5) ; i++)
		{
			ind1 = rand_int(0,max-1);
			ind2 = rand_int(0,max-1);

			aux = _routes[ind1];
			_routes[ind1] = _routes[ind2];
			_routes[ind2] = aux;
		}
	}

	double Solution::fitness () const
	{
		double fitness = 0.0;
		int ultimo = 0;
		double dist = 0.0;
		int cap = _pbm.capacity();

		register int c_actual;
		register int i = 0;

		while(i < _pbm.nCustomers())
		{
			c_actual = _routes[i];

			if( ((dist + _pbm.distance(ultimo,c_actual) + _pbm.distance(c_actual,0)) >  _pbm.maxRouteTime())
			 || (( cap - _pbm.demand(c_actual)) < 0)
			 // Add on version 1.0.1
			 // New constrains: cost( Route + actual) < cost( new Route(actual))
			 || ((_pbm.distance(ultimo,0)  + _pbm.distance(0,c_actual)) < _pbm.distance(ultimo,c_actual))
			 )
			{
					fitness += dist + _pbm.distance(ultimo,0);
					ultimo = 0;
					dist = 0.0;
					cap = _pbm.capacity();
			}
			else
			{
				dist += _pbm.distance(ultimo,c_actual);
				cap -= _pbm.demand(c_actual);
				ultimo = c_actual;
				i++;
			}
		}

		fitness += dist + _pbm.distance(ultimo,0);

		return fitness;
	}

	char *Solution::to_String() const
	{
		return (char *)_routes.get_first();
	}

	void Solution::to_Solution(char *_routes_)
	{
		int *ptr=(int *)_routes_;

		for (int i=0;i<_pbm.nCustomers();i++)
		{
			_routes[i]=*ptr;
			ptr++;
		}
	}

	unsigned int Solution::size() const
	{
		return ( _pbm.nCustomers() * sizeof(int));
	}

	int & Solution::pos(const int index)
	{
		return _routes[index];
	}


 	Rarray<int>& Solution::routes()
	{
		return _routes;
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
		Solution aux(sol);
		const int max = sol.pbm().nCustomers();
		float fit = sol.fitness();
		float fit_a;
		Direction d = sol.pbm().direction();

		for(int i = 0; i < max; i++)
			for(int j = i+1; j < max; j++)
			{
				aux.routes().invert(i,j);
				fit_a = aux.fitness();
				if( ((d == minimize) && (fit_a < fit))
				 || ((d == minimize) && (fit_a > fit) && (rand01() < 0.3))
				 || ((d == maximize) && (fit_a > fit))
				 || ((d == maximize) && (fit_a < fit) && (rand01() < 0.3)))
				{
					fit = fit_a;
					sol = aux;
				}
				else aux = sol;
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


