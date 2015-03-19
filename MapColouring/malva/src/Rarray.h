/******************************************************************************
***									    ***
*** Template para el manejo dinámico de arrays				    ***
*** Añadido métodos para invertir todo o parte del array.		    ***
***									    ***
******************************************************************************/

#ifndef ARRAY_INC
#define ARRAY_INC 1
#include <iostream>
#include <assert.h>

using namespace std;

template<class E1> class Rarray
{
	private:
		E1 *first;
		int count;

	public:
		Rarray()
		{
			first = NULL;
			count = 0;
		}

		~Rarray()
		{
			remove();
		}

		E1* get_first() const
		{
			return first;
		}

		void message_a(int cod) const
		{
			switch (cod)
			{
				case 1: cout << endl << "The size of array must be upper that 0 !!!" ;
			}
		}

		Rarray(const int size_a)
		{
			if (size_a<0) message_a(1);
			first = new E1[size_a];
			count=size_a;
		}

		void remove()
		{
			if (count!=0)
				delete [] first;
		}

		int size() const
		{
			return count;
		}

		E1& operator[](int pos) const
		{
			return (E1&)(*(first + pos));
		}

		friend ostream& operator<< (ostream& os,const Rarray<E1>& a)
		{
			for (int i=0;i<a.size();i++)
				os << endl << a[i];
			return os;
		}

		Rarray<E1>& operator=(const Rarray<E1>& source)
		{
			remove();
			count = source.size();
			first = new E1[count];

			for (int i=0;i<count;i++)
				(*this)[i] = source[i];

			return (*this);
		}


		Rarray<E1>& invert()
		{
			return invert(0,count-1);
		}

		Rarray<E1>& invert(const int pos1, const int pos2)
		{
			int max,min,half,i,j;
			E1 aux;

			if(pos1 > pos2)
			{
				max = pos1;
				min = pos2;
			}
			else
			{
				max = pos2;
				min = pos1;
			}

			assert((min > 0) || (min < count-1));
			half = ((max-min)/2) + 1;

			for(i = min,j=max; i< half; i++,j--)
			{
				aux = first[min];
				first[min] = first[max];
				first[max] = aux;
			}

			return (*this);
		}

		Rarray<E1>& sort(int (*comp)(const E1 &,const E1 &))
		{
			E1 aux;
			int j;
	
			for (int i=1; i < count ; i++)
			{
				aux = first[i];
			  j = i - 1;
			  while ( (comp(aux,first[j])) && (j >= 0) )
			  {
				   first[j+1]= first[j];
				   j--;
			  }
		    first[j+1] = aux;
			}

			return (*this);
		}			

}; // end of class

#endif
