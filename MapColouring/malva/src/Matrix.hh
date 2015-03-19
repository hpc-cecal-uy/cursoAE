/*****************************************************************************
***									   ***
*** Este fichero decribe el template Matrix, para el manejo de matrices.   ***
*** Tambien permite el manejo de vectores, los que trata como matrices cuya***
*** primera dimension es 1.						   ***
***									   ***
*****************************************************************************/
#ifndef _MATRIX
#define _MATRIX

#include "Messages.h"
#include <assert.h>

template <class T> class Matrix
{
	private:
		T *_matrix;
		T nulo,neutro,inverso;
		int _dimX,_dimY;

	public:
		Matrix()
		{
			_dimX = _dimY = 0;
			_matrix = NULL;
		}

		Matrix(const int x,const int y,const T nulo = 0.0,const T neutro = 1.0,const T inverso = -1.0)
		{
			assert(x >= 1);
			assert(y >= 1);

			int k = 0;

			_dimX = x;
			_dimY = y;

			this->nulo = nulo;
			this->neutro = neutro;
			this->inverso = inverso;

			_matrix = new T [_dimX*_dimY];
			if(!_matrix) show_message(7);

			for(int i = 0; i < x; i++)
			{
				for(int j = 0; j < y ; j++)
				{
					_matrix[k] = (i!=j?nulo:neutro);
					k++;
				}

			}
		}

		Matrix(const int y,const T nulo = 0.0,const T neutro = 1.0,const T inverso = -1.0)
		{
			assert(y >= 1);

			_dimX = 1;
			_dimY = y;

			this->nulo = nulo;
			this->neutro = neutro;
			this->inverso = inverso;

			_matrix = new T [_dimY];
			if(!_matrix) show_message(7);

			_matrix[0] = neutro;
			for(int j = 1; j < y ; j++)
				_matrix[j] = nulo;
		}

		Matrix(const Matrix<T> &m)
		{

			_dimX = m.dimX();
			_dimY = m.dimY();

			nulo = m.nulo;
			neutro = m.neutro;
			inverso = m.inverso;

			_matrix = new T [_dimX*_dimY];
			if(!_matrix) show_message(7);

			for(int i = 0; i < (_dimX*_dimY); i++)
			{
				_matrix[i] = m[i];
			}
		}


		~Matrix()
		{
			remove();
		}

		T &operator()(const int x,const int y) const
		{
			if((x >= _dimX) || (y >= _dimY))
				show_message(14);

			return (T&)(*(_matrix + (x*_dimX) + y));
		}

		T &operator[](const int y) const
		{
			if(y >= (_dimX*_dimY))
				show_message(14);

			return (T&)(*(_matrix + y));
		}

		T &operator()(const int y) const
		{
			if(y >= (_dimX*_dimY))
				show_message(14);

			return (T&)(*(_matrix + y));
		}

		Matrix<T> &operator=(const Matrix<T> &m)
		{
			remove();

			_dimX = m.dimX();
			_dimY = m.dimY();

			_matrix = new T [_dimX*_dimY];
			if(!_matrix) show_message(7);

			for(int i = 0; i < (_dimX*_dimY); i++)
			{
				_matrix[i] = m[i];
			}

			return (*this);
		}

		bool operator==(const Matrix<T> &m) const
		{
			if((_dimX != m.dimX()) || (_dimY != m.dimY()))
				return false;

			for(int i = 0; i < (_dimX*_dimY); i++)
				if(_matrix[i] != m[i]) return false;

			return true;
		}

		bool operator!=(const Matrix<T> &m) const
		{
			return !((*this) == m);
		}

		Matrix<T> operator*(const Matrix<T> &m)
		{
			int x = (m.dimX()!=_dimY?0:_dimX);
			int y = (x!=0?m.dimY():0);
			T acum = nulo;

			Matrix<T> res(x,y);

			for(int i = 0; i < _dimX; i++)
				for(int j = 0; j < m.dimY(); j++)
				{
					acum = nulo;
					for( int k = 0; k < _dimY; k++)
						acum += (*this)(i,k)* m(k,j);
					res(i,j) = acum;
				}

			return Matrix<T>(res);

		}

		Matrix<T> &operator*=(const Matrix<T> &m)
		{
			int x = (m.dimX()!=_dimY?0:_dimX);
			int y = (x!=0?0:m.dimY());
			T acum = nulo;

			Matrix<T> res(x,y);

			for(int i = 0; i < _dimX; i++)
				for(int j = 0; j < m.dimY(); j++)
				{
					acum = nulo;
					for( int k = 0; k < _dimY; k++)
						acum += (*this)(i,k)*m(k,j);
					res(i,j) = acum;
				}

			(*this) = res;

			return (*this);

		}

		Matrix<T> operator*(const T &elem)
		{
			Matrix<T> res(_dimX,_dimY);

			for(int i = 0; i < (_dimX*_dimY); i++)
				res[i] = _matrix[i] * elem;

			return Matrix(res);
		}

		Matrix<T> &operator*=(const T &elem)
		{
			for(int i = 0; i < (_dimX*_dimY); i++)
				_matrix[i] *= elem;

			return (*this);
		}

		Matrix<T> operator+(const Matrix<T> &m)
		{
			int x = (m.dimX()!=_dimX?0:_dimX);
			int y = (m.dimY()!=_dimY?0:_dimY);

			Matrix<T> res(x,y);

			for(int i = 0; i < (x*y); i++)
					res[i] = _matrix[i] + m[i];

			return Matrix<T>(res);
		}

		Matrix<T> &operator+=(const Matrix<T> &m)
		{
			int x = (m.dimX()!=_dimX?0:_dimX);
			int y = (m.dimY()!=_dimY?0:_dimY);

			for(int i = 0; i < (x*y); i++)
				_matrix[i] += m[i];

			return (*this);
		}

		Matrix<T> operator-(const Matrix<T> &m)
		{
			Matrix<T> res();

			res = m * inverso;
			return (*this) + res;
		}


		Matrix<T> &operator-=(const Matrix<T> &m)
		{
			Matrix<T> res();

			res = m * inverso;
			(*this) += res;

			return (*this);
		}

		Matrix<T> Traspuesta()
		{
			Matrix<T> res(_dimY,_dimX);

			for(int i = 0; i < _dimX; i++)
				for(int j = 0; j < _dimY; j++)
					res(j,i) = (*this)(i,j);

			return Matrix<T>(res);
		}

		Matrix<T> &nula()
		{
			for(int i = 0; i < (_dimX*dimY); i++)
				_matrix[i] = nulo;

			return (*this);
		}

		Matrix<T> &identity()
		{
			register int k = 0;

			for(int i = 0; i < _dimX; i++)
				for(int j = 0; j < _dimY; j++)
				{
					_matrix[k] = (i!=j?nulo:neutro);
					k++;
				}

			return (*this);
		}

		unsigned int size() const
		{
			return (sizeof(T)*_dimX*_dimY);
		}

		char *to_string() const
		{
			return (char *) _matrix;
		}

		Matrix<T> &to_Matrix(char *_cadena)
		{
			T *ptr = (T *)_cadena;

			for(int i = 0; i < (_dimX*_dimY) ; i++)
			{
				_matrix[i] = *ptr;
				ptr++;
			}

			return (*this);
		}


		int dimX() const
		{
			return _dimX;
		}

		int dimY() const
		{
			return _dimY;
		}

		void remove()
		{
			if(_matrix != NULL)
				delete [] _matrix;
		}
};

#endif
