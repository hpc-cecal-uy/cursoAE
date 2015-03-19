#include <iostream>
#include <fstream>
#include "CHC.hh"

using namespace std;

int main (int argc, char** argv)
{
	using skeleton CHC;
	char path[MAX_BUFFER] = "";

	system("clear");

	strcat(path,argv[1]);
	ifstream f(path);
	if(!f) show_message(10);

	f.getline(path,MAX_BUFFER,'\n');
	ifstream f1(path);
	if(!f1)	show_message(11);

	f.getline(path,MAX_BUFFER,'\n');
	ifstream f2(path);
	if(!f2) show_message(12);

	Problem pbm;
	f2 >> pbm;

	Operator_Pool pool(pbm);
	SetUpParams cfg(pool);
	f1 >> cfg;

	Solver_Lan solver(pbm,cfg,argc,argv);
	solver.run();

	if (solver.pid()==0)
	{
		solver.show_state();
		cout << "Solucion: " << solver.global_best_solution() << " Fitness: " << solver.global_best_solution().fitness();

		f.getline(path,MAX_BUFFER,'\n');
	  	ofstream fexit(path);
	  	if(!fexit) show_message(13);
	  	fexit << solver.userstatistics();

		cout << endl << endl << " :( ---------------------- THE END --------------- :) " << endl;
	}
	return(0);
}
