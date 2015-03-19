/******************************************************************************
     Modified by Carlos Cotta Porras
     April 2001

     Modified by G.J.L.P
******************************************************************************/


#include <string.h>
#include "States.hh"

// Methods of class State_Vble

	// constructors
	State_Vble ::State_Vble ()
	{
		name=NULL;
		nitems=0;
		length=0;
		content=NULL;
	}

	State_Vble ::State_Vble (const char *st_name)
	{
		name=strdup(st_name);
		nitems=0;
		length=0;
		content=NULL;
	}

	State_Vble::State_Vble (const char *st_name,StateCenter& sc)
	{
		name=strdup(st_name);
		nitems=0;
		length=0;
		content=NULL;
		sc.add(*this);
	}

	State_Vble ::State_Vble (const char *st_name,const char *new_contents, unsigned long new_nitems,  unsigned long new_length)
	{
		name=strdup(st_name);
		nitems=0;
		length=0;
		content=NULL;
		set_contents(new_contents,new_nitems,new_length);
	}

	State_Vble ::State_Vble (const char *st_name,const char *new_contents, unsigned long new_nitems,  unsigned long new_length,StateCenter& sc)
	{
		name=strdup(st_name);
		nitems=0;
		length=0;
		content=NULL;
		set_contents(new_contents,new_nitems,new_length);
		sc.add(*this);
	}


	// Set the name  of a state vble.
 	void State_Vble ::set_name (const char* st_name) // Set the name  of a state vble.
	{
		if (name!=NULL)
			free(name);
		name=strdup(st_name);
	}

	// Get the name  of a state vble.
	char* State_Vble ::get_name () const // Get the name  of a state vble.
     	{
		return name;
	}

	// Number of basic items
	unsigned long State_Vble ::get_nitems() const // Number of basic  items
	{
		return nitems;
	}

	// Get the total number of bytes
	unsigned long State_Vble ::get_length () const // Get the total  number of bytes
	{
		return length;
	}

	// Fill up a state vble.
	void State_Vble ::set_contents (const char *new_contents, unsigned long new_nitems,  unsigned long new_length)
	{
		if (content!=NULL)
			free(content);
		content=(char *)malloc(new_nitems * new_length);
		memcpy(content,new_contents,(new_nitems*new_length));
	 	nitems=new_nitems;
	 	length=new_length;
	}

	// Obtain the contents of a state vble.
	void *State_Vble ::get_contents (char *read_contents, unsigned long& read_nitems,  unsigned long& read_length) const
	{
		memcpy(read_contents,content,nitems * length);
		read_nitems=nitems;
		read_length=length;
		return NULL;
	}

	ostream& operator<< (ostream& os,const State_Vble& st)
	{
		os << endl << st.name
		     << endl << st.nitems
		     << endl << st.length
		     << endl << st.content;

		return os;
	}

	State_Vble ::~State_Vble ()
	{
		free(content);
		free(name);
	}

// Methods of class StateCenter

	StateCenter::StateCenter():state_variables()
	{}

	// searchs a state variable
	State_Vble  *StateCenter::find(const char *st_name) const
	{
		Rlist_item<State_Vble> *current_state=state_variables.get_first();

		for (int i=0;i<state_variables.size();i++)
		{
			if (!(strcmp(current_state->data().get_name(),st_name)))
				return &(current_state->data());
			current_state=&current_state->next_item();
		}
		return NULL;
	}

        // Add one state variable
	void StateCenter::add(State_Vble& st)
	{
		State_Vble  *found_state=find(st.get_name());
		if (found_state==NULL)
			state_variables.append(st);
		else
			cout << endl << "You are trying to introduce a state variable that is yet used in the skeleton !!" << st.get_name();
	}

	void StateCenter::add(State_Vble *st)
	{
		State_Vble  *found_state=find(st->get_name());
		if (found_state==NULL)
			state_variables.append(*st);
		else
			cout << endl << "You are trying to introduce a state variable that is yet used in the skeleton !!" << st->get_name();
	}

	// Remove one state variable
	void StateCenter::remove(const char* st_name)
	{
		State_Vble *found_state=find(st_name);
		if (found_state!=NULL)
			state_variables.delete_item_1(*found_state);
	}

	//  Update the contents of one vble.
	void StateCenter::update(const char* st_name, const State_Vble& st) const //  Update the contents of one vble.
	{
		State_Vble  *found_state=find(st_name);
		if (found_state!=NULL)
		{
			char *a=(char *)malloc(found_state->get_nitems() * found_state->get_length());
			unsigned long nitems,length;
			st.get_contents(a,nitems,length);
			found_state->set_contents(a,nitems,length);
			free(a);
		}
	}

	// Get a vble. with a given name
	State_Vble& StateCenter::get(const char* st_name) const  // Get a vble. with a given name
	{
		State_Vble  *found_state=find(st_name);
		return *found_state;
	}

	// Allows an easy  iterated extraction
	State_Vble* StateCenter::get_next(const State_Vble& st) const
	{
		State_Vble  *found_state=find(st.get_name());
		if (found_state==NULL) return NULL;
		if ( state_variables.get_at(state_variables.get_position(*found_state)).is_last()) return NULL;
		else return &(state_variables.get_at(state_variables.get_position(*found_state)).next_item().data());
	}

	// returns the number of state variables
	unsigned int StateCenter::size() const
	{
		return state_variables.size();
	}

        // Obtain the contents of a state vble. of name st_name
	void StateCenter::get_contents_state_variable(const char *st_name,char *read_contents, unsigned long& read_nitems,  unsigned long& read_length) const
	{
		get(st_name).get_contents(read_contents,read_nitems,read_length);
	}

	// Fill up a state vble.of name st_name
	void StateCenter::set_contents_state_variable(const char *st_name,const char *new_contents, unsigned long new_nitems,  unsigned long new_length) const
	{
		get(st_name).set_contents(new_contents,new_nitems,new_length);
	}

	void StateCenter::removeAll()
	{
 		while(state_variables.get_first())
		{
			Rlist_item<State_Vble>* v = state_variables.get_first();
			remove(v->useful_data->get_name());
		}
  	}

	StateCenter::~StateCenter()
	{
		removeAll();
	}
