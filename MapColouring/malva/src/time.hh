/******************************************************************************
***									    ***
*** Este fichero incluye algunas funciones útiles para la medicion de tiem- ***
*** pos.								    ***
***									    ***
******************************************************************************/

#ifndef INC_time_hh
#define INC_time_hh

#define MAXTIME 4294

#include <time.h>
#include <sys/time.h>
#include <unistd.h>

// return time in microseconds
inline float _used_time()
{
	struct timeval tv ;
	static long tiempo = -1;
	float  u_clock;

	gettimeofday(&tv,NULL);

	if(tiempo < 0) tiempo = tv.tv_sec;

	u_clock = ((float)(tv.tv_sec - tiempo)*1000000.0) + ((float)tv.tv_usec);
	return u_clock;

}


inline float _used_time(float time)
{
	 float dif=_used_time() - time;
	 if (dif<0) dif = 0;	
	 return dif;	
}



#endif
