/***************************************************************
 *  File:        list.h
 *  Author:      Lishan Li                                        
 *  Date:        May, 2003                                                 
 *
 *  Description: This List class is a template class. It builds a
 *               single linked list of any type of objects. 
 ***************************************************************/

#ifndef LIST_H
#define LIST_H

#include <assert.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

template <class T>
struct ListNode
{
	T data;                       // T could be any type of object
	ListNode<T> *next;
};

template <class T>
class List1
{
public:
	ListNode<T>* header;          // header is a dummy node
	ListNode<T>* current;
	int len;                      // length of the linked list

	List1();                       // constructor
	~List1();                      // destructor

	List1(const List1<T> & source); // copy constructor
	List1<T> & operator=(const List1<T> & source);
	void append(const T);         // append data at the end of list 
	void insert(const T);         // insert data in order 
	void insertPtr(const T);      // insert pointer type of object in order
	void insertFront(const T);    // insert data at the beginning of the list
	void remove(const T);         // delete data from list
	void next();
	void prev();
	void reset();
	void clear();
	int length() const { return len; }
	bool isEmpty() const;        // check whether list is empty
	bool isFull() const;         // check whether memory is allocated
	T value() const;             // return content of the data
	void printList();
	void printListPtr();
	ListNode<T>* findPrevious(const T);
	ListNode<T>* search(const T);
};

template <class T>
List1<T>::List1()
{
	header = new ListNode<T>;
	header->next = NULL;
	current = header;
	len = 0;
}

template <class T>
List1<T>::List1(const List1<T> & source)
{
	header = NULL; 
	*this = source;
}

template <class T>
List1<T> & List1<T>::operator=(const List1<T> & source)
{
	if(this != &source)
	{
		ListNode<T> * current = source.header;
		while(current != NULL)
		{
			insert(current->data);
			current = current->pnext;
		}
	}
	return *this;
}

template <class T>
void List1<T>::clear()
{
	ListNode<T> *tmp;
	ListNode<T> *previous = header;
	ListNode<T> *traverse = header->next;

	while(traverse != NULL)   // release memory from list
	{
		tmp = traverse;
		previous->next = traverse->next;
		traverse = traverse->next;
		delete tmp;
	}
	current = header;
	len = 0;
}

template <class T>
List1<T>::~List1()
{
	clear();
	delete header; 
}


template <class T>
void List1<T>::insert(const T item)
{
	assert(!isFull());
	ListNode<T> * newNode = new ListNode<T>;
	newNode->data = item;
	newNode->next = NULL;
	if(len == 0)  // no need to traverse
	{
		newNode->next = current->next;
		current->next = newNode;
	}
	else          // need to traverse
	{
		ListNode<T> * temp = header;
		while((temp->next != NULL) && (item < temp->next->data))
			temp = temp->next;
		newNode->next = temp->next;
		temp->next = newNode;
	}
	len++;
}

template <class T>
void List1<T>::insertPtr(const T item)
{
	assert(!isFull());
	ListNode<T> * newNode = new ListNode<T>;
	newNode->data = item;
	newNode->next = NULL;
	if(len == 0)  // no need to traverse
	{
		newNode->next = current->next;
		current->next = newNode;
	}
	else          // need to traverse
	{
		ListNode<T> * temp = header;
		while((temp->next != NULL) && (*item < *(temp->next->data)))  // pointer need dereference
			temp = temp->next;
		newNode->next = temp->next;
		temp->next = newNode;
	}
	len++;
}

template <class T>
void List1<T>::insertFront(const T item)
{
	ListNode<T> * newNode = new ListNode<T>;
	newNode->data = item;
	newNode->next = header->next;
	header->next = newNode;
	len++;
}

template <class T>
void List1<T>::append(const T item)
{
	assert(!isFull());
	ListNode<T> * newNode = new ListNode<T>;
	newNode->data = item;
	newNode->next = NULL;
	ListNode<T> *traverse = header;
	while(traverse->next != NULL)
		traverse = traverse->next;
	traverse->next = newNode;
	len++;	
}

template <class T>
ListNode<T>* List1<T>::findPrevious(const T target)
{
	ListNode<T> *temp;

	for(temp = header; temp->next != NULL; temp = temp->next)
		if(*(temp->next->data) == *target)
			return temp;

	return NULL;
}

template <class T>
void List1<T>::remove(const T target)
{
	if(len != 0)
	{
		ListNode<T> *p = findPrevious(target);
		if(p->next != NULL)
		{
			ListNode<T> *oldNode = p->next;
			p->next = p->next->next;
			delete oldNode;
			len--;
		}
	}
}

template <class T>
void List1<T>::next()
{
	current = current->next;
}

template <class T>
void List1<T>::prev()
{
	if(len > 1)
	{
		ListNode<T>* tmp = header;
		while(tmp->next != current)
			tmp = tmp->nect;

		current = tmp;
	}
}

template <class T>
void List1<T>::reset()
{
	current = header;
}

template <class T>
bool List1<T>::isEmpty() const
{
	return (len == 0);
}

template <class T>
bool List1<T>::isFull() const
{
	ListNode<T> *tmp = new ListNode<T>;
	if(tmp == NULL)
		return true;
	else
	{
		delete tmp;
		return false;
	}
}

template <class T>
T List1<T>::value() const
{
	return current->next->data;
}

template <class T>
ListNode<T>* List1<T>::search(const T target) 
{
	ListNode<T> *temp;
	
	for(temp = header->next; temp != NULL; temp = temp->next)
		if((*target) == *(temp->data))
			return temp;

	return NULL;
}

template <class T>
void List1<T>::printList()
{
	cout << endl;
	for(int i = 0; i < len; i++)
	{
		cout << value();
		next();
	}
	reset();
}

template <class T>
void List1<T>::printListPtr()
{
	cout << endl;
	for(int i = 0; i < len; i++)
	{
		cout << *(value());  // pointer need dereference
		next();
	}
	reset();
}

#endif

/************************************ End of File **************************************/
