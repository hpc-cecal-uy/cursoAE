/******************************************************************************
*** 									    ***
*** Este fichero contiene funciones (en general inlines) para generar núme- ***
*** ros aleatorios y otras funciones relacionadas con ello		    ***
*** 									    ***
*******************************************************************************/

#ifndef INC_random_hh
#define INC_random_hh

#include <stdlib.h>


inline double undefined ()
{
	double zero = 0;
	return zero/zero;
}

// Returns a valeu greater than any other 
inline double infinity ()
{
	double one=1.0; 
	double zero=0.0; 
	return one/zero; 
}

// Returns a random number in [0,1].	
inline double rand01 ()
{
	return drand48();
}

// Returns a random number
inline int rand_int (float min,float max)
{
	int value=rand();
	int range= (int)(max - min);
	int order = value % (range+1);
	return ((int)min + order);
}

// selects a seed
inline void random_seed(long int seed)
{
	srand48(seed);
	srand(seed);
}

#endif
