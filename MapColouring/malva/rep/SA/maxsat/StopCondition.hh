#ifndef stop_condition
#define stop_condition

#include "SA.hh"
skeleton SA
{

  provides class StopCondition
  {
	public:
		StopCondition();	
		virtual bool EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup)=0;
		~StopCondition();		
  };

  requires class StopCondition_1 : public StopCondition
  {
	public:
		StopCondition_1();	
		virtual bool EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup);
		~StopCondition_1();		
  };

  requires class StopCondition_2 : public StopCondition
  {
	public:
		StopCondition_2();	
		virtual bool EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup);
		~StopCondition_2();		
  };

  requires class StopCondition_3 : public StopCondition
  {
	public:
		StopCondition_3();	
		virtual bool EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup);
		~StopCondition_3();		
  };
}

#endif
