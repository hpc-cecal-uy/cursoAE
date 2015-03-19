/******************************************************************************
***									    ***
*** Este template sirve para el manejo de listas din�micas		    ***
***									    ***
******************************************************************************/

#ifndef LIST_INC
#define LIST_INC 1
#include <iostream>
#include <stdlib.h>
#include <Messages.h>

using namespace std;

template<class E> class Rlist_item
{
	public:
		E *useful_data; // pointer to the structure stored in the list
		Rlist_item<E> *next;
		Rlist_item<E> *previous;

		Rlist_item(E& new_data)
		{
			useful_data=&new_data;	
			next=NULL;
			previous=NULL;	
		}

		Rlist_item(E *new_data)
		{
			useful_data=new_data;	
			next=NULL;
			previous=NULL;	
		}

		~Rlist_item()
		{
		}

		Rlist_item<E>& next_item()
		{
			return *(next);
		}

		Rlist_item<E>& previous_item()
		{
			return *(previous);
		}

		bool is_last()
		{
			return (next==NULL);
		}

		bool is_first()
		{
			return (previous==NULL);
		}

		E& data()
		{
			return *(useful_data);
		}
};

template<class E> class Rlist
{
	private:
		Rlist_item<E> *first; // first item in the list
		Rlist_item<E> *last;  // last item un the list
		int count; // number of items in the list

	public:
		// constructor
		Rlist()
		{
			first=NULL;
			last=NULL;
			count=0;
		}

		// destructor
		~Rlist()
		{
			remove();
		}

		// Return the size of the list
		int size() const
		{ return count; }

		// Go back to the initial state of the list
		void reset()
		{
			first=NULL;
			last=NULL;
			count=0;
		}

		// Add a item at the final of the list
		Rlist<E>& append(E& new_item)
		{
			Rlist_item<E> *new_Rlist_item=new Rlist_item<E>(new_item);
			if (first==NULL)
			{
				first=new_Rlist_item;
				new_Rlist_item->next=NULL;
				new_Rlist_item->previous=NULL;			
			}
			else
			{
				last->next=new_Rlist_item;
				new_Rlist_item->previous=last;
			}	
			last=new_Rlist_item;
			count++;
			return *this;
		}

		Rlist<E>& append(E *new_item)
		{
			append(*new_item);
			return (*this);
		}

		// Add a item in a position ( pos ) of the list
		Rlist<E>& add_pos(E&  new_item, const int pos)
		{
			if (pos==size()-1)
				return append(new_item);

			if (pos==-1)
				return add(new_item,NULL);
			else
				return add(new_item,get_at(pos).useful_data);
		}

		// Add a item in the list in the position next to "previous item"
		Rlist<E>& add(E& new_item,E *previous_item)
		{
			if (first==NULL)
				return append(new_item);

			Rlist_item<E> *new_Rlist_item=new Rlist_item<E>(new_item);

			if (previous_item==NULL) // Add the item like the first of the list
			{
				new_Rlist_item->next=first;
				new_Rlist_item->previous=NULL;
				first->previous=new_Rlist_item;
				first=new_Rlist_item; 	
			}
			else
			{
				int previous_position=get_position(*previous_item);
				if (previous_position==-1) return(*this);
				Rlist_item<E> *previous_Rlist_item = &( get_at(previous_position));
				new_Rlist_item->next=previous_Rlist_item->next;
				new_Rlist_item->previous=previous_Rlist_item;
				if (previous_Rlist_item->next!=NULL)
					(previous_Rlist_item->next)->previous=new_Rlist_item;
				else last=new_Rlist_item;
				previous_Rlist_item->next=new_Rlist_item;
			}
			count++;
			return *this;
		}

		// Return a pointer to the first item of the list
		Rlist_item<E> *get_first() const
		{ return first; }

		// Assign a item like the first item in the list
		void set_first(Rlist_item<E> *new_first)
		{ first=new_first; }

		// Return a pointer to the last item of the list
		Rlist_item<E> *get_last() const
		{ return last; }

		// Assign a item like the last item in the list
		void set_last(Rlist_item<E> *new_last)
		{ last=new_last; }

		// Return the item at position "pos"
		E& operator[](int pos) const
		{
			return *(get_at(pos).useful_data);
		}

		// Return the Rlist_item at position "pos"
		Rlist_item<E>& get_at(int pos) const
		{
			Rlist_item<E> *present=first;
			for (int k=0;k<size();k++)
				if (k==pos) return *present;
				else present=present->next;
		}

		// Return the item position in the list
		int get_position(const E& item) const // probado
		{
			Rlist_item<E> *present=first;
			int i=0;

			while(present!=NULL)
			{
				if (present->useful_data==&item) return i;
				i++;
				present=present->next; 	
			}
			return -1; // the object has not been found
		}

		// Delete a item of the list
                Rlist<E>& delete_item(E& item)
		{
			int position = get_position(item);

			if (position==-1) return *this;
			Rlist_item<E> *present=&(get_at(position));  	
 
			if (&item==first->useful_data) // is the first
			{
				if (&item==last->useful_data)
				{
					delete(first->useful_data);
					delete(first);
					first=NULL;
					last=NULL;
					count=0;	
				}
				else
				{
					first=first->next;
					first->previous=NULL;
					delete(present->useful_data);
					delete(present);
					count--;	
				}
			}
			else
			{
				if (&item==last->useful_data)
				{
					last=present->previous;
					last->next=NULL;
					delete(present->useful_data);
					delete(present);
					count--;	
				}
				else
				{
					(present->next)->previous=present->previous;
					(present->previous)->next=present->next;
					delete(present->useful_data);
					delete(present);	
					count--;	
				}
			}
			return *this;
		}

		// Delete a item of the list without free the useful_data
		Rlist<E>& delete_item_1(E& item)
		{
			int position = get_position(item);

			if (position==-1) return *this;
			Rlist_item<E> *present=&(get_at(position));  	
 
			if (&item==first->useful_data) // is the first
			{
				if (&item==last->useful_data)
				{
					delete(first);
					first=NULL;
					last=NULL;
					count=0;	
				}
				else
				{
					first=first->next;
					first->previous=NULL;
					delete(present);
					count--;	
				}
			}
			else
			{
				if (&item==last->useful_data)
				{
					last=present->previous;
					last->next=NULL;
					delete(present);
					count--;	
				}
				else
				{
					(present->next)->previous=present->previous;
					(present->previous)->next=present->next;
					delete(present);	
					count--;	
				}
			}
			return *this;
		}

		// Delete item at position "pos"
                Rlist<E>& delete_item_by_position(const int pos)
		{ return delete_item(*(get_at(pos).useful_data)); }

		// Delete the last item in the list
		Rlist<E>& delete_last()
		{ return delete_item(*(last->useful_data)); }

		// delete all items in the list
		Rlist<E>& remove()
		{
			Rlist_item<E> *next,*present=first;
			while (present!=NULL)
			{
				next=present->next;
				delete(present->useful_data);
				delete(present);
				present=next;
			}	
			first=NULL;
			last=NULL;
			count=0;
			return *this;
		}

		// Join a new list to this list
		Rlist<E>& join(Rlist<E>& new_list)
		{
			if (new_list.size()==0)
				return *this;

			if (first==NULL)
			{
				first=new_list.get_first();
				last=new_list.get_last();		
			}
			else
			{
				last->next=new_list.get_first();
				(new_list.get_first())->previous=last;	
				last = new_list.get_last();
			}
			count += new_list.size();
			new_list.reset();
			return *this;  	
		}

		// Show items of the list
		friend ostream& operator<<(ostream& os, const Rlist<E>& list)
		{
			Rlist_item<E> *present=list.get_first();
			if (list.get_first()==NULL) os << endl << "THE LIST IS EMPTY !!";
			while (present!=NULL)
			{
				os << endl << (*(present->useful_data));	
				// Falta el operador para stat.
				//  (habra que ver)
				present=present->next;
			}
			return os;
		}

		// Copy the list passed
		Rlist<E>& operator=(const Rlist<E>& source)
		{
			E *new_item;
			remove();
			if (source.first==NULL && source.last==NULL)
			{
				first=NULL;
				last=NULL;
				count=0;
			}	
			else
			{
				for (int i=0;i<source.size();i++)
				{
					if ((new_item=(E *)malloc(sizeof(E)))==NULL)
						show_message(7);	
					(*new_item)=*(source.get_at(i).useful_data);
					append(*new_item);
				}
			}	
			return *this;
		}

		// Invert the order of items in the list
		Rlist<E>& invert()
		{
			Rlist_item<E> *present,*interchange;
  
			present=first;	

			for (int i=0;i<size();i++)
			{
				interchange=present->next;
				present->next=present->previous;
				present->previous=interchange;	
				present=interchange;	
			}		
			interchange=first;
			first=last;
			last=interchange;
			return (*this);
		}
}; // end of class
#endif
