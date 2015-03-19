/***************************************************************************
 ***                              netstream.cc                           ***
 ***                            v1.6 - July 2001                         ***
 **								         ***
 ***			      v1.5 - March 2001				 ***
 ***                          v1.0 - November 2000                       *** 
 ***                                                                     *** 
 ***   v1.5 extends v1.0:						 ***
 ***	    .- Changes metods init() and finalize() to be static	 ***
 ***	    .- Incorporates process group management		 	 ***
 ***	    .- Do not consider LEDA anymore				 ***
 ***	    .- Contains a method "int my_pid()" for easy invokations  	 ***
 ***	    .- Adds "unsigned" and "long double" input/output        	 ***
 ***									 ***
 ***   v1.6 extends v1.5:						 ***
 **	    .- Internal in/out buffers for packed separated		 ***
 ***                                                                     ***
 ***   Communication services for LAN/WAN use following the message      ***
 ***   passing paradigm.                                                 ***
 ***                             STREAM C++ VERSION                      ***
 ***                             MPI implementation                      ***
 ***                          Developed by Erique Alba                   ***
 ***************************************************************************/
#ifndef INC_netstream
#define INC_netstream
#include "mpi.h"
#include <assert.h>
#include <string.h>
// Class NetStream allows to define and use network streams trhough LAN and WAN
#define REGULAR_STREAM_TAG	0	// Used for tagging MPI regular messages
#define PACKED_STREAM_TAG	1	// Used for tagging MPI packet messages
#define NET_TYPE		MPI_Datatype	// Network allowable data types
#define NET_BOOL		MPI_CHAR	// Bools like chars
#define NET_CHAR                MPI_CHAR
#define NET_SHORT		MPI_SHORT
#define NET_INT			MPI_INT
#define NET_LONG                MPI_LONG
#define NET_UNSIGNED_CHAR       MPI_UNSIGNED_CHAR
#define NET_UNSIGNED_SHORT      MPI_UNSIGNED_SHORT
#define NET_UNSIGNED            MPI_UNSIGNED
#define NET_UNSIGNED_LONG       MPI_UNSIGNED_LONG
#define NET_FLOAT               MPI_FLOAT
#define NET_DOUBLE              MPI_DOUBLE
#define NET_LONG_DOUBLE         MPI_LONG_DOUBLE
#define NET_BYTE                MPI_BYTE
#define NET_PACKED 		MPI_PACKED
#define NET_Comm		MPI_Comm
#define MAX_MSG_LENGTH		204800		// Max length of a message
#define MAX_PACK_BUFFER_SIZE	204800		// Max length of a packed message
// Help structure for manipulators having one int& argument
class NetStream;
struct smanip1c		// "const int"
{	NetStream& (*f)(NetStream&, const int);				// The ONE argument function
	int i;								// The argument
	smanip1c( NetStream&(*ff)(NetStream&,const int), int ii) : f(ff), i(ii) {}	// Constuctor
};
struct smanip1		// "int*"	note: references do not work! "int&"
{	NetStream& (*f)(NetStream&, int*);				// The ONE argument function
	int* i;								// The argument
	smanip1( NetStream&(*ff)(NetStream&, int*), int* ii) : f(ff), i(ii) {}	// Constuctor
};
// Tags for the available streams
const int any	  = MPI_ANY_TAG;		// Tag value valid for any stream
const int regular = REGULAR_STREAM_TAG;	// Tag value for regular stream of data
const int packed  = PACKED_STREAM_TAG;	// Tag value for packed stream of data
// Tags for sources
const int any_source = MPI_ANY_SOURCE;  // Tag value valid for any source
class NetStream
{
    public:
	NetStream ();  			// Default constructor
                                        // Constructor with source integer left unchanged
	NetStream (int, char **);	// Init the communications
	~NetStream ();                  // Default destructor
	static void init(int,char**);	// Init the communication system. Invoke it only ONCE 	
 	static void finalize(void);	// Shutdown the communication system. Invoke it ONCE
	// GROUP management
	void set_communicator(NET_Comm comm); 	  // Set the netstream to a new communicator
	NET_Comm get_communicator(void);	  // Get the present communicator in this netstream
  	static NET_Comm create_group(NET_Comm comm, int color, int key); // Create a new group inside the present communicator
		// Create a bridge between local and remote MATCHING call
	static NET_Comm create_inter_group(NET_Comm lcomm, int lrank, NET_Comm bcomm, int rrank, int strtrype);
							
// BASIC INPUT SERVICES			     <comments>		BASIC OUTPUT SERVICES
// ============================================================================================================
   NetStream& operator>> (bool&   d);				NetStream& operator<< (bool   d);
   NetStream& operator>> (char&   d);                           NetStream& operator<< (char   d);
   NetStream& operator>> (short&  d);                           NetStream& operator<< (short  d);
   NetStream& operator>> (int&    d);                           NetStream& operator<< (int    d);
   NetStream& operator>> (long&   d);                           NetStream& operator<< (long   d);
   NetStream& operator>> (float&  d);                           NetStream& operator<< (float  d);
   NetStream& operator>> (double& d);                           NetStream& operator<< (double d);
   NetStream& operator>> (char*   d);    /*NULL terminated*/	NetStream& operator<< (char*  d);
   NetStream& operator>> (void*   d); 	 /*NULL terminated*/	NetStream& operator<< (void*  d);
   // Extended data types from version 1.5 on
   NetStream& operator>> (unsigned char&       d);   	       	NetStream& operator<< (unsigned char       d);
   NetStream& operator>> (unsigned short int&  d);   	       	NetStream& operator<< (unsigned short int  d);
   NetStream& operator>> (unsigned int&        d);   	       	NetStream& operator<< (unsigned int        d);
   NetStream& operator>> (unsigned long int&   d);   	       	NetStream& operator<< (unsigned long int   d);
   NetStream& operator>> (long double&         d);  	       	NetStream& operator<< (long double         d);
	int pnumber(void);		// Returns the number of processes
	bool broadcast;			// Determines whether the next sent message is for broadcasting
	// Input MANIPULATORS for modifying the behavior of the channel on the fly
	// NO ARGUMENTS
	NetStream& operator<< (NetStream& (*f)(NetStream& n)) { return f(*this); } // NO arguments
	NetStream& _barrier(void);	// Sit and wait until all processes are in barrier
	NetStream& _pack_begin(void);	// Marks the beginning of a packed information
	NetStream& _pack_end(void);	// Marks the end of a packed and flush it to the net
	NetStream& _probe(const int stream_type, int& pending);	// Check whether there are awaiting data
	NetStream& _broadcast(void);	// Broadcast a message to all the processes
	// ONE ARGUMENT
	// "const int"
	NetStream& operator<< (smanip1c m) { return m.f((*this),m.i); }// ONE int& argument constant
	// "int*"
	NetStream& operator<< (smanip1 m)  { return m.f((*this),m.i); }// ONE int& argument
	// BASIC CLASS METHODS FOR MANIPULATORS
	NetStream& _my_pid(int* pid);			// Returns the process ID of the calling process
	NetStream& _wait(const int stream_type);	// Wait for an incoming message in the specified stream
    NetStream& _wait2(const int stream_type, int& tipo);
    NetStream& _set_target(const int p);		// Stablish "p" as the default receiver
    NetStream& _get_target(int* p);			// Get into "p" the default receiver
	NetStream& _set_source(const int p); 		// Stablish "p" as the default transmitter
 	NetStream& _get_source(int* p);   		// Get into "p" the default transmitter
	// AUXILIAR PUBLIC METHODS FOR ALLOWING EASY MANAGEMENTS OF NETSTREAMS
	int my_pid(void);				// Returns the process ID of the calling process
   private:
	int default_target, default_source; // Default process IDs to send-recv data to-from
	bool pack_in_progress;	// Defines whether a packet is being defined with "pack_begin-pack_end"
	int packin_index;	// Index to be used for extracting from a  IN  packed message  - v1.6
	int packout_index;	// Index to be used for adding     to   an OUT packed message  - v1.6
	int pending_input_packet;//Is there a pending packet already read into the IN buffer?  - v1.6
	char* packin_buffer;	// Buffer to temporary storage of the IN  packed being defined - v1.6
	char* packout_buffer;	// Buffer to temporary storage of the OUT packed being defined - v1.6
	bool pack_in, pack_out;	// Define whether input-output packed message is being used
	void reset(void);	// Reset member variables of this class
	NET_Comm my_communicator;	// Communicator of this netstream
        void send(void* d, const int len, const NET_TYPE type, const int target);
        void rcv (void* d, const int len, const NET_TYPE type, const int source); 
}; // class NetStream
	// MANIPULATORS (must be static or non-member methods in C++ -mpiCC only allows non-member!-)
	// NO ARGUMENTS
	NetStream& barrier(NetStream& n);	// Sit and wait until all processes are in barrier
	NetStream& broadcast(NetStream& n);	// Broadcast a message to all the processes
	NetStream& pack_begin(NetStream& n);	// Marks the beginning of a packed information
	NetStream& pack_end(NetStream& n);	// Marks the end of a packed and flush it to the net
	// ONE ARGUMENT
		NetStream& __my_pid(NetStream& n, int* pid); // Returns the process ID of the calling process
        inline smanip1 my_pid(int* pid){ return smanip1(__my_pid,pid); } // manipulator
		NetStream& __wait(NetStream& n, const int stream_type);// Wait for an incoming message - helper
	inline smanip1c wait(const int stream_type){ return smanip1c(__wait,stream_type); } // manipulator
		NetStream& __set_target(NetStream& n, const int p); // Stablish "p" as the default receiver
        inline smanip1c set_target(const int p){ return smanip1c(__set_target,p); } // manipulator
		NetStream& __get_target(NetStream& n, int* p); // Get into "p" the default receiver
        inline smanip1 get_target(int* p){ return smanip1(__get_target,p); } // manipulator
		NetStream& __set_source(NetStream& n, const int p); // Stablish "p" as the default transmitter
        inline smanip1c set_source(const int p){ return smanip1c(__set_source,p); } // manipulator
		NetStream& __get_source(NetStream& n, int* p); // Get into "p" the default transmitter
        inline smanip1 get_source(int* p){ return smanip1(__get_source,p); } // manipulator
	// TWO ARGUMENTS - not used yet
	NetStream& probe(NetStream& n, const int stream_type, int& pending); // Check whether there are awaiting data
#endif
