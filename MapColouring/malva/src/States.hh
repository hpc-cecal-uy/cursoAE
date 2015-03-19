/********************************************************************************************************
***												      ***	
***			 Class StateCenter: Pool of state variables for skeletons		      ***	
***												      ***	
***			 Class State_Vble: State Variable of a skeleton				      ***	
***												      ***	
*********************************************************************************************************/

#ifndef STATE_CENTER
#define STATE_CENTER 1

#include "Rlist.h"
#include <iostream>

class StateCenter;

class State_Vble
{ 	
	private:
		char *name;
		unsigned long nitems;
		unsigned long length;
		char *content;	

	public:	
		// constructors
		State_Vble ();
		State_Vble (const char *st_name);
		State_Vble (const char *st_name,StateCenter& sc);
		State_Vble (const char *st_name,const char *new_contents, unsigned long new_nitems,  unsigned long new_length);
		State_Vble (const char *st_name,const char *new_contents, unsigned long new_nitems,  unsigned long new_length,StateCenter& sc);

 		void set_name (const char* st_name); // Set the name  of a state vble.
		char* get_name () const; // Get the name  of a state vble.
		unsigned long get_nitems() const;
		unsigned long get_length () const; // Get the total  number of bytes
		void set_contents (const char *new_contents, unsigned long new_nitems,  unsigned long new_length); 	// Fill up a state vble.
		void *get_contents (char *read_contents, unsigned long& read_nitems,  unsigned long& read_length) const; 	 // Obtain the contents of a state vble.
		friend ostream& operator<< (ostream& os,const State_Vble& st);
		~State_Vble ();
};

class StateCenter
{
	private:
		Rlist<State_Vble> state_variables;

	public:
		StateCenter();
		State_Vble  *find(const char *st_name) const; // search a state variable
		void add(State_Vble& st); // Add one state variable
		void add(State_Vble *st); // Add one state variable
		void remove(const char* st_name); // Remove one state variable
		void removeAll(); // Removes all variables
		void update(const char* st_name, const State_Vble& st) const; //  Update the contents of one vble.
		State_Vble& get(const char* st_name) const;  // Get a vble. with a given name
		State_Vble* get_next(const State_Vble& st) const; // Allows an easy  iterated extraction
		unsigned int size() const; // returns the number of state variables
		void get_contents_state_variable(const char *st_name,char *read_contents, unsigned long& read_nitems,  unsigned long& read_length) const; // Obtain the contents of a state vble. of name st_name
		void set_contents_state_variable(const char *st_name,const char *new_contents, unsigned long new_nitems,  unsigned long new_length) const; // Fill up a state vble.of name st_name
		~StateCenter();
};

#endif
