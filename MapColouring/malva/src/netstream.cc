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
#include "netstream.hh"
// Default constructor
NetStream::NetStream()
{
	this->reset();	// Reset to default values member variables
	packin_buffer  = new char[MAX_PACK_BUFFER_SIZE];
	packout_buffer = new char[MAX_PACK_BUFFER_SIZE];
}
// Init the underlying communication library (MPI)
NetStream::NetStream (int argc, char ** argv)
{
	init(argc,argv);
	this->reset();
	packin_buffer  = new char[MAX_PACK_BUFFER_SIZE];
	packout_buffer = new char[MAX_PACK_BUFFER_SIZE];
}
// Default destructor
NetStream::~NetStream()
{
	delete [] packin_buffer;	delete [] packout_buffer;
	this->reset();
}        
// Give default values to member variables
void NetStream::reset(void)
{
	default_target       = default_source = 0;
	pack_in_progress     = false;
	packin_index         = packout_index = 0;
	pending_input_packet = false;
	pack_in              = pack_out = false;
	broadcast            = false;
	my_communicator      = MPI_COMM_WORLD;
}
// Init the communication system. Invoke it only ONCE 	
void NetStream::init(int argc, char** argv)
{
	static bool system_up = false; // Is MPI already running?
	if (!system_up) 
		{ MPI_Init(&argc,&argv);
		  system_up = true;
		}
}
// Shutdown the communication system. Invoke it ONCE 	
void NetStream::finalize(void)
{
	MPI_Finalize();		// Unconditional Finalization
}
// BASIC INPUT/OUTPUT SERVICES
// ===================================================================================
 
NetStream& NetStream::operator>> (bool& d)
{	rcv(&d,1,NET_BOOL,default_source);       return(*this);	}
	
NetStream& NetStream::operator<< (bool  d)
{	send(&d,1,NET_BOOL,default_target);      return(*this);	}
NetStream& NetStream::operator>> (char& d)                           
{	rcv(&d,1,NET_CHAR,default_source);       return(*this);	}
NetStream& NetStream::operator<< (char d)
{	send(&d,1,NET_CHAR,default_target);      return(*this);	}
NetStream& NetStream::operator>> (short& d)                           
{	rcv(&d,1,NET_SHORT,default_source);     return(*this);	}
NetStream& NetStream::operator<< (short d)
{	send(&d,1,NET_SHORT,default_target);    return(*this);	}
NetStream& NetStream::operator>> (int& d)                           
{	rcv(&d,1,NET_INT,default_source);        return(*this);	}
NetStream& NetStream::operator<< (int d)
{	send(&d,1,NET_INT,default_target);        return(*this);	}
NetStream& NetStream::operator>> (long& d)                           
{	rcv(&d,1,NET_LONG,default_source);       return(*this);	}
NetStream& NetStream::operator<< (long d)
{	send(&d,1,NET_LONG,default_target);      return(*this);	}
NetStream& NetStream::operator>> (float& d)                           
{	rcv(&d,1,NET_FLOAT,default_source);     return(*this);	}
NetStream& NetStream::operator<< (float d)
{	send(&d,1,NET_FLOAT,default_target);    return(*this);	}
NetStream& NetStream::operator>> (double& d)                           
{	rcv(&d,1,NET_DOUBLE,default_source);   return(*this);	}
NetStream& NetStream::operator<< (double d)
{	send(&d,1,NET_DOUBLE,default_target);  return(*this);	}
NetStream& NetStream::operator>> (char* d)
{	rcv(d,MAX_MSG_LENGTH,NET_CHAR,default_source);		return(*this);	}
NetStream& NetStream::operator<< (char* d)
{ 	send(d,strlen(d)+1,NET_CHAR,default_target);		return(*this);	}
NetStream& NetStream::operator>> (void* d) 		
{	rcv(d,MAX_MSG_LENGTH,NET_CHAR,default_source);		return(*this);	}
NetStream& NetStream::operator<< (void* d)
{	send(d,strlen((char*)d)+1,NET_CHAR,default_target);	return(*this);	}
// Extended data types from version 1.5 on
NetStream& NetStream::operator>> (unsigned char&   d)
{	rcv(&d,MAX_MSG_LENGTH,NET_UNSIGNED_CHAR,default_source);return(*this);	}
     	
NetStream& NetStream::operator<< (unsigned char   d)
{	send(&d,1,NET_UNSIGNED_CHAR,default_target);  		return(*this);	}
NetStream& NetStream::operator>> (unsigned short int&  d)   	       	
{	rcv(&d,MAX_MSG_LENGTH,NET_UNSIGNED_SHORT,default_source);return(*this);	}
NetStream& NetStream::operator<< (unsigned short int  d)
{	send(&d,1,NET_UNSIGNED_SHORT,default_target);  	 	return(*this);	}
NetStream& NetStream::operator>> (unsigned int&    d)
{	rcv(&d,MAX_MSG_LENGTH,NET_UNSIGNED,default_source);	return(*this);	}
   	       	
NetStream& NetStream::operator<< (unsigned int    d)
{	send(&d,1,NET_UNSIGNED,default_target);  	 	return(*this);	}
NetStream& NetStream::operator>> (unsigned long int&   d)  
{	rcv(&d,MAX_MSG_LENGTH,NET_UNSIGNED_LONG,default_source);return(*this);	} 	
       	
NetStream& NetStream::operator<< (unsigned long int  d)
{	send(&d,1,NET_UNSIGNED_LONG,default_target);  	 	return(*this);	}
NetStream& NetStream::operator>> (long double&     d) 
{	rcv(&d,MAX_MSG_LENGTH,NET_LONG_DOUBLE,default_source);	return(*this);	}
 	       	
NetStream& NetStream::operator<< (long double     d)
{	send(&d,1,NET_LONG_DOUBLE,default_target);  	 	return(*this);	}
// SET-GET TARGET AND SOURCE PROCESSES
NetStream& __set_target(NetStream& n, const int p) { return n._set_target(p); }
NetStream& NetStream::_set_target(const int p)
{ assert(p>=0); default_target = p; return (*this); }
NetStream& __get_target(NetStream& n, int* p) { return n._get_target(p); }
NetStream& NetStream::_get_target(int* p)
{ *p = default_target; return (*this); }
NetStream& __set_source(NetStream& n, const int p) { return n._set_source(p); }
NetStream& NetStream::_set_source(const int p)
{ /*assert(p>=0);*/ default_source = p; return (*this); }
NetStream& __get_source(NetStream& n, int* p) { return n._get_source(p); }
NetStream& NetStream::_get_source(int* p)
{ *p = default_source; return (*this); }
// Get the number of processes involved in the communications
int NetStream::pnumber(void)
{       int numprocs, rvalue;
	rvalue = MPI_Comm_size(my_communicator,&numprocs);
        assert(rvalue==MPI_SUCCESS);
	return numprocs;
}
// MANIPULATORS: SYNCHRONIZATION AND PACKING SERVICES
// ===================================================================================
// Get the process ID [0, 1, 2,  ...] fro the calling process
NetStream& __my_pid(NetStream& n, int* pid)
{
	return n._my_pid(pid);
}
NetStream& NetStream::_my_pid(int* pid)
{
	MPI_Comm_rank(my_communicator,pid);
	return (*this);
}
// EASY access to rank - Returns the process ID of the calling process
int NetStream::my_pid(void)
{	int pid;
	this->_my_pid(&pid);
	return pid;
}
// Sit and wait until all processes are in the same barrier
// Can be used as a MANIPULATOR
NetStream& barrier(NetStream& n)
{
	return n._barrier();
}
NetStream& NetStream::_barrier(void)
{	int status;
	status = MPI_Barrier(my_communicator);
	assert(status==MPI_SUCCESS);
	return (*this);
}
// Wait for an incoming message in any input stream
NetStream& NetStream::_wait(const int stream_type)      // class
{   
    int rvalue;
    MPI_Status status;
    assert(stream_type==regular||stream_type==packed||stream_type==any);
    if( ((stream_type==packed) || (stream_type==any) )&& (pending_input_packet) )
    //if( (stream_type==packed) && (pending_input_packet) )
        return (*this);     // wait befor packet_begin when already received
    rvalue = MPI_Probe(default_source,stream_type,my_communicator,&status);
    assert(rvalue==MPI_SUCCESS);
    return (*this);
}

NetStream& NetStream::_wait2(const int stream_type, int& tipo)      // class
{   
    int rvalue;
    MPI_Status status;
    assert(stream_type==regular||stream_type==packed||stream_type==any);
    if( ((stream_type==packed) || (stream_type==any) )&& (pending_input_packet) )
    //if( (stream_type==packed) && (pending_input_packet) )
        return (*this);     // wait befor packet_begin when already received
    rvalue = MPI_Probe(default_source,stream_type,my_communicator,&status);
    assert(rvalue==MPI_SUCCESS);
    if (status.MPI_SOURCE == 0){
        tipo = 1;
    }
    return (*this);
}
NetStream& __wait(NetStream& n, const int stream_type) 	// helper
{
	return n._wait(stream_type);
}
// Marks the beginning of a packed information
NetStream& pack_begin(NetStream& n)
{
	return n._pack_begin();
}
NetStream& NetStream::_pack_begin(void)
{
	int rvalue=MPI_SUCCESS;
	MPI_Status status;
	if(!pack_in_progress)
	{	pack_in_progress             = true;
		packin_index = packout_index = 0;
		pack_in                      = false;
		pack_out                     = false;
		if (!pending_input_packet)
		{	_probe(packed,pending_input_packet);
			if(pending_input_packet)
				rvalue = MPI_Recv(packin_buffer, MAX_PACK_BUFFER_SIZE, NET_PACKED,
			 	     default_source, PACKED_STREAM_TAG, my_communicator, &status);
		}
	}
	return (*this);
}
// Marks the end of a packed and flush it to the net
NetStream& pack_end(NetStream& n)
{
	return n._pack_end();
}
NetStream& NetStream::_pack_end(void)
{
	int rvalue, mypid;
	if (pack_in_progress)
	{
          	if(pack_out)
		{  if(broadcast)	// Packet broadcast
		   {	broadcast = false;
			_my_pid(&mypid);
			rvalue = MPI_Bcast(packout_buffer,packout_index,NET_PACKED,
					   mypid,my_communicator);
			assert(rvalue==MPI_SUCCESS);
		   }
		   else
		   {	rvalue = MPI_Send(packout_buffer, packout_index, NET_PACKED,
					  default_target,PACKED_STREAM_TAG,my_communicator);
			assert(rvalue==MPI_SUCCESS);
		   }
                }
		pack_in_progress                   = false;
		pack_in            = pack_out      = false;
		packin_index       = packout_index = 0 ;
	}
	return (*this);	
}
// Check whether there are awaiting data
NetStream& probe(NetStream& n, const int stream_type, int& pending)
{
	return n._probe(stream_type, pending);
}
NetStream& NetStream::_probe(const int stream_type, int& pending)
{
	MPI_Status status;
	int rvalue;
	assert(stream_type==regular||stream_type==packed||stream_type==any);
	rvalue = MPI_Iprobe(default_source,stream_type,my_communicator,&pending,&status);
	assert(rvalue==MPI_SUCCESS);
	return (*this);
}
// Broadcast a message to all the processes
NetStream& broadcast(NetStream& n)
{
	return n._broadcast();
}
NetStream& NetStream::_broadcast(void)
{
	broadcast = true;
	return (*this);
}
// PRIVATE SERVICES
// ===================================================================================
// Usually, the length is the number of bytes for every net type
// When packing we must use in the pack calls the numer of items (length divided by type size)
// Any char is encoded with a leading field of length
void NetStream::send(void* d, const int len, const NET_TYPE type, const int target)
{
	int rvalue = MPI_SUCCESS, length = len;
	// PACKING SERVICE
	if(pack_in_progress)
	{
	  pack_out = true;
	  assert(pack_out!=pack_in);	// Error condition
	  if(type==NET_CHAR)
	  	send(&length,sizeof(NET_INT),NET_INT,target); // Recursive call to store string length
	  else
		length = 1;
	  rvalue = MPI_Pack(d,length,type,packout_buffer,MAX_PACK_BUFFER_SIZE,&packout_index,my_communicator);
	  assert(rvalue==MPI_SUCCESS);
	  return;
	}
	if(broadcast)     	// Regular broadcast, packed broadcast managed in _pack_end()
	{       int mypid;

 		broadcast = false;
		_my_pid(&mypid);
		rvalue = MPI_Bcast(d,len,type,mypid,my_communicator);
		assert(rvalue==MPI_SUCCESS);
		return;
	}
	rvalue = MPI_Send(d,len,type,target,REGULAR_STREAM_TAG,my_communicator);
	assert(rvalue==MPI_SUCCESS);
}
void NetStream::rcv (void* d, const int len, const NET_TYPE type, const int source)
{ 	MPI_Status status;
	int rvalue = MPI_SUCCESS, length=len;
	if(pack_in_progress)
	{
//	  if(!pack_in && !pending_input_packet)
//	  	rvalue = MPI_Recv(packin_buffer, MAX_PACK_BUFFER_SIZE, NET_PACKED,
//		                  default_source, PACKED_STREAM_TAG, my_communicator, &status);
	  pack_in              = true;
	  pending_input_packet = false;
	  assert(pack_out!=pack_in);
	  if(type==NET_CHAR)
		rcv(&length,sizeof(NET_INT),NET_INT,source); // Gets the string length
	  else
		length = 1;
	  rvalue = MPI_Unpack(packin_buffer, MAX_PACK_BUFFER_SIZE, &packin_index, d,
			      length, type, my_communicator);
	  assert(rvalue==MPI_SUCCESS);
	  return;
	}

	rvalue=MPI_Recv(d,len,type,source,REGULAR_STREAM_TAG,my_communicator,&status);
	assert(status.MPI_ERROR==MPI_SUCCESS);
	assert(rvalue==MPI_SUCCESS);
}
///////////////////////////////////////   GROUP MANAGEMENT   ////////////////////////////////
// Set the netstream to a new communicator
void NetStream::set_communicator(NET_Comm comm)
{
	my_communicator = comm;
}
// Get the present communicator in this netstream
NET_Comm NetStream::get_communicator(void)
{
	return my_communicator;
}
// Create a new group inside the present communicator 
NET_Comm NetStream::create_group(NET_Comm comm, int color, int key)
{	int rvalue;
	NET_Comm newcomm;
	rvalue=MPI_Comm_split(comm,color,key,&newcomm);
	assert(rvalue==MPI_SUCCESS);
	return newcomm;
}
// Create a bridge between local and remote MATCHING call 	
NET_Comm NetStream::create_inter_group(NET_Comm lcomm, int lrank, NET_Comm bcomm, int rrank, int strtype)
{	int rvalue;
	NET_Comm newcomm;
	rvalue=MPI_Intercomm_create(lcomm,lrank,bcomm,rrank,strtype,&newcomm);
	assert(rvalue==MPI_SUCCESS);
	return newcomm;
}
