#include "StopCondition.hh"
skeleton SA
{

// StopCondition -------------------------------------------------------------------------------------

	StopCondition::StopCondition()
	{}
	
	StopCondition::~StopCondition()		
	{}
        	
// StopCondition_1 -------------------------------------------------------------------------------------

	StopCondition_1::StopCondition_1():StopCondition()
	{}

	bool StopCondition_1::EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup)
	{
		return false;
	}

	StopCondition_1::~StopCondition_1()
	{}	

// StopCondition_2 -------------------------------------------------------------------------------------
	
	StopCondition_2::StopCondition_2():StopCondition()
	{}

	bool StopCondition_2::EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup)
	{
		return  (solver.global_best_cost()>8.5);
	}

	StopCondition_2::~StopCondition_2()
	{}	

// StopCondition_3 -------------------------------------------------------------------------------------

	StopCondition_3::StopCondition_3():StopCondition()
	{}

	bool StopCondition_3::EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup)
	{
		return ((solver.current_trial()==setup.independent_runs()) && (solver.current_iteration()==setup.max_evaluations()));
	}

	StopCondition_3::~StopCondition_3()
	{}	

}
