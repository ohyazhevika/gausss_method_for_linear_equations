#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;


double Rand(int range)
{
	double ans = (double)(rand() % (2*abs(range) + 1) - abs(range));
	ans += ans * 0.012;
	return ans;
}
class System
{
public: double* a, * b, * c, * d, * e, 
	* f, //правая часть системы
	* X, //найденное решение системы
	* f_tilde, 
	* X_tilde, 
	*X_precise;//точное, заранее известное (генерируемое случайным образом) решение системы
	int size, k;
	System()
	{
		a = NULL;
		b = NULL;
		c = NULL;
		d = NULL;
		e = NULL;
		f = NULL;
		f_tilde = 0;
		X = NULL;
		X_tilde = 0;
		size = 0;
		k = -1;
		X_precise = NULL;
	}
	~System()
	{
		delete a;
		delete b;
		delete c;
		delete d;
		delete e;
		delete f;
		delete f_tilde;
		delete X;
		delete X_tilde;
		delete X_precise;
	}
//
	bool SystemInput(fstream& in)
	{
		if (!in.is_open())
		{
			cerr << "File not found.\n";
			return false;
		}
		in >> size >> k;
		if (size <= 0)
		{
			cout << "The size of the matrix is not a positive number.\n";
			return false;
		}
		a = new double[size-1];
		b = new double[size];
		c = new double[size-1];
		d = new double[size];
		e = new double[size];
		f = new double[size];
		f_tilde = new double[size];
		X = new double[size];
		X_tilde = new double[size];
		X_precise = new double[size];
		for (int i = 0; i < size; i++)
			f_tilde[i] = 0;
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				double elem;
				in >> elem;
				if (j == k - 1)
					d[i] = elem;
				if (j == k)
					e[i] = elem;
				if (i + j == size-1)
					b[i] = elem;
				if (i + j == size)
					a[i - 1] = elem;
				if (i + j == size - 2)
					c[i] = elem;
				f_tilde[i] += elem; //результат перемножения матрицы А заданной на единичный вектор
			}
		}
		for (int i = 0; i < size; i++)
		{
			in >> f[i];
		}
		return true;
	}

	void GenerateSystem(int range, int range_pow)
	{
		size = range_pow;
		b = new double[size];
		a = new double[size - 1];
		c = new double[size - 1];
		d = new double[size];
		e = new double[size];
		f = new double[size];
		f_tilde = new double[size];
		X = new double[size];
		X_precise = new double[size];
		X_tilde = new double[size];
		for (int i = 0; i < size; i++)
		{
			b[i] = Rand(range);
			d[i] = Rand(range);
			e[i] = Rand(range);
			X[i] = Rand(range);
			if (i != 0)
				a[i - 1] = Rand(range);
			if (i != size - 1)
				c[i] = Rand(range);
		}
		k = 6;
		b[size - k] = d[size - k];
		b[size - k - 1] = e[size - k - 1];
		a[size - k - 1] = e[size - k];
		a[size - k] = d[size - k + 1];
		c[size - k - 1] = d[size - k - 1];
		c[size - k - 2] = e[size - k - 2];
		f[0] = d[0] * X[k - 1] + e[0] * X[k] + c[0] * X[size - 2] + b[0] * X[size - 1];
		f_tilde[0] = d[0] + e[0] + c[0] + b[0];
		for (int i = 1; i <= size - k - 3; i++)
		{
			f[i] = d[i] * X[k - 1] + e[i] * X[k] + c[i] * X[size - 2 - i] + b[i] * X[size - 1 - i] + a[i - 1] * X[size - i];
			f_tilde[i] = d[i] + e[i] + c[i] + b[i] + a[i - 1];
		}
		f[size - k - 2] = d[size - k - 2] * X[k - 1] + e[size - k - 2] * X[k] + b[size - k - 2] * X[k + 1] + a[size - k - 3] * X[k + 2];
		f_tilde[size - k - 2] = d[size - k - 2] + e[size - k - 2] + b[size - k - 2]  + a[size - k - 3] ;
		f[size - k - 1] = d[size - k - 1] * X[k - 1] + e[size - k - 1] * X[k] + a[size - k - 2] * X[k + 1];
		f_tilde[size - k - 1] = d[size - k - 1] + e[size - k - 1] + a[size - k - 2];
		f[size - k] = c[size - k] * X[k - 2] + d[size - k] * X[k - 1] + e[size - k] * X[k];
		f_tilde[size - k] = c[size - k] + d[size - k] + e[size - k];
		f[size - k + 1] = c[size - k + 1] * X[k - 3] + b[size - k + 1] * X[k - 2] + d[size - k + 1] * X[k - 1] + e[size - k + 1] * X[k];
		f_tilde[size - k + 1] = c[size - k + 1] + b[size - k + 1] + d[size - k + 1] + e[size - k + 1];
		for (int i = size - k + 2; i < size -1 ; i++)
		{
			f[i] = c[i] * X[size-i-2] + b[i] * X[size-i-1] + a[i - 1] * X[size-i] + d[i] * X[k - 1]  + e[i] * X[k];
			f_tilde[i] = c[i] + b[i]  + a[i - 1]  + d[i]  + e[i];
		}
		f[size - 1] = b[size - 1] * X[0] + a[size - 2] * X[1] + d[size - 1] * X[k - 1] + e[size - 1] * X[k];
		f_tilde[size - 1] = b[size - 1] + a[size - 2] + d[size - 1] + e[size - 1];
		for (int i = 0; i < size; i++)
		{
			X_precise[i] = X[i];
			X[i] = 0;
		}
	}

	bool SystemOutput()
	{
		if (size <= 0)
		{
			cout << "The size of the matrix is not a positive number.\n";
			return false;
		}
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				double elem = 0;
				if (j == k - 1)
					elem = d[i];
				if (j == k)
					elem = e[i];
				if (i + j == size - 1)
					elem = b[i];
				if (i + j == size)
					elem = a[i - 1];
				if (i + j == size - 2)
					elem = c[i];
				cout << elem << ' ';
			}
			cout << "  " << f[i] << '\n';
		}
		cout << endl;
		return true;
	}
//
	bool VectorsOutput()
	{
		cout << "d: ";
		for (int i = 0; i < size; i++)
			cout << d[i] << ' ';
		cout << endl;
		cout << "e: ";
		for (int i = 0; i < size; i++)
			cout << e[i] << ' ';
		cout << endl;
		cout << "f: ";
		for (int i = 0; i < size; i++)
			cout << f[i] << ' ';
		cout << endl;
		cout << "b: ";
		for (int i = 0; i < size; i++)
			cout << b[i] << ' ';
		cout << endl;
		cout << "a: ";
		for (int i = 0; i < size-1; i++)
			cout << a[i] << ' ';
		cout << endl;
		cout << "c: ";
		for (int i = 0; i < size-1; i++)
			cout << c[i] << ' ';
		cout << endl;
		return true;
	}

	bool SolutionOutput()
	{
		if (size > 0)
			for (int i = 0; i < size; i++)
				cout << "X[" << i << "] = " << X[i] << '\n';
		return true;
	}
	//
private: bool DivideEquation(int i, double div)
	{
		if (div == 0)
		{
	//		cout << "One of the divisers was 0. The programme is terminated.\n";
			return false;
		}
		div = 1 / div;
		d[i] = (d[i] == div) ? 1 : d[i] * div;
		e[i] = (e[i] == div) ? 1 : e[i] * div;
		b[i] = (b[i] == div) ? 1 : b[i] * div;
		f[i] = (f[i] == div) ? 1 : f[i] * div;
		f_tilde[i] = (f_tilde[i] == div) ? 1 : f_tilde[i] * div;
		if (i != size - 1&&c[i]!=0)
			c[i] = (c[i] == div) ? 1 : c[i] * div;
		if (i!=0&&a[i-1]!=0)
			a[i-1] = (a[i-1] == div) ? 1 : a[i-1] * div;
		if (size - k - 2 >= 0 && i == size - k - 2)
			c[i] = e[i];
		if (size - k - 1 >= 0 && i == size - k - 1)
			c[i] = d[i];
		if (size - k == i)
			a[size - k - 1] = e[i];
		if (size - k + 1 == i)
			a[size - k] = d[i];
		return true;
	}
	//
	bool AddEquations1(int This, int toThis, double mul=1)
	{
		d[toThis] += d[This]*mul;
		e[toThis] += e[This]*mul;
		a[This] = 0;
		b[toThis] += c[This]*mul;
		f[toThis] += f[This]*mul;
		f_tilde[toThis] += f_tilde[This] * mul;
		if (size-k-2>=0&& toThis == size-k-2)
			c[toThis] = e[toThis];
		if (size - k - 1 >= 0 && toThis == size - k - 1)
			c[toThis] = d[toThis];
		if (size - k == toThis)
			a[size - k - 1] = e[toThis];
		if (size - k + 1 == toThis)
			a[size - k] = d[toThis];
		return true;
	}
	//
	bool AddEquations2(int This, int toThis, double mul = 1)
	{
		d[toThis] += d[This] * mul;
		e[toThis] += e[This] * mul;
		f[toThis] += f[This] * mul;
		f_tilde[toThis] += f_tilde[This] * mul;
		c[toThis] = 0;
		if (size - k == toThis)
			a[size - k - 1] = e[toThis];
		if (size - k + 1 == toThis)
			a[size - k] = d[toThis];
		if (size - k - 2 >= 0 && toThis == size - k - 2)
			c[toThis] = e[toThis];
		if (size - k - 1 >= 0 && toThis == size - k - 1)
			c[toThis] = d[toThis];
		return true;
	}
	//
	bool AddEquations3(int This, int toThis, double mul = 1)
	{
		d[toThis] += d[This] * mul;
		e[toThis] += e[This] * mul;
		f[toThis] += f[This] * mul;
		f_tilde[toThis] += f_tilde[This] * mul;
		c[toThis] = 0;
		b[toThis] += a[toThis] * mul;
		if (size - k == toThis)
			a[size - k - 1] = e[toThis];
		if (size - k + 1 == toThis)
			a[size - k] = d[toThis];
		if (size - k - 2 >= 0 && toThis == size - k - 2)
			c[toThis] = e[toThis];
		if (size - k - 1 >= 0 && toThis == size - k - 1)
			c[toThis] = d[toThis];
		return true;
	}
	//
	bool AddEquations4(int This, int toThis, double mul = 1)
	{
		d[toThis] += d[This] * mul;
		e[toThis] += e[This] * mul;
		f[toThis] += f[This] * mul;
		f_tilde[toThis] += f_tilde[This] * mul;
		a[This] = 0;
		if (size - k - 2 >= 0 && toThis == size - k - 2)
			c[toThis] = e[toThis];
		if (size - k - 1 >= 0 && toThis == size - k - 1)
			c[toThis] = d[toThis];
		if (size - k == toThis)
			a[size - k - 1] = e[toThis];
		if (size - k + 1 == toThis)
			a[size - k] = d[toThis];
		return true;
	}
	//
	bool getX()
	{
		for (int i = 0; i < size; i++)
			if (e[i] == 0)
			{
	//			cout << "The given system of linear equations cannot be solved.\n";
				return false;
			}
		double div = 1 / e[size - k - 1];
		X[k] = f[size - k -1 ] *div;
		X_tilde[k] = f_tilde[size - k - 1] *div;
		for (int i = 0; i < size; i++)
		{
			if (i != size - k - 1)
			{
				X[size - i -1 ] = f[i]-e[i]*X[k];
				X_tilde[size - i - 1] = f_tilde[i] - e[i] * X_tilde[k];
			}
		}
	}
	//
public:	bool Solve()
	{
		if (size <= 0)
		{
			cout << "The size of the matrix is not a positive number.\n";
			return false;
		}
		for (int i = 0; i <= size - k - 2; i++)
		{
			if (DivideEquation(i, b[i]))
				AddEquations1(i, i + 1, -a[i]);
			else return false;
		}
		for (int i = size - k - 2; i > 0; i--)
		{
			AddEquations2(i, i - 1, -c[i - 1]);
		}
		for (int i = size - 1; i >= size - k + 1; i--)
		{
			if (DivideEquation(i, b[i]))
				AddEquations3(i, i - 1, -c[i - 1]);
			else
				return false;
		}
		for (int i = size - k + 1; i < size - 1; i++)
			AddEquations4(i, i + 1, -a[i]);
		if (d[size - k] == 0)
		{
		//	cout << "One of the divisers was 0. The programme is terminated.\n";
			return false;
		}
		double div = 1 / d[size - k];
		e[size - k] *= div;
		f[size - k] *= div;
		f_tilde[size - k] *= div;
		a[size - k-1] = e[size - k];
		d[size - k] = b[size-k]=1;
		for (int i = 0; i < size; i++)
		{
			if (i != size - k)
			{
				e[i] += e[size - k] * (-d[i]);
				f[i] += f[size - k] * (-d[i]);
				f_tilde[i] += f_tilde[size - k] * (-d[i]);
				d[i] = 0;
				if (i == size - k - 1)
				{
					c[i] = d[i];
					b[i] = e[i];
				}
				if (i == size - k - 2)
					c[i] = e[i];
				if (i == size - k + 1 && i >= 1)
					a[i - 1] = d[i];
			}
		}
		a[size - k] = d[size-k+1] = 0;
		c[size - k - 1] = d[size-k-1] = 0;
		return getX();
	}
//

	  double RelativeError() 
	  {
		  double ans = (X_precise[0] == 0) ? fabs(X_precise[0] - X[0]) : fabs((X_precise[0] - X[0]) / X_precise[0]);
		  for (int i = 1; i < size; i++)
		  {
			  double error = (X_precise[i] == 0) ? fabs(X_precise[i] - X[i]) : fabs((X_precise[i] - X[i]) / X_precise[i]);
			  if (error > ans)
				  ans = error;
		  }
		  return ans;
	  }
	  double AccuracyAssessment()
	  {
		  double ans = fabs(X_tilde[0] - 1);
		  for (int i = 1; i < size; i++)
		  {
			  double error = fabs(X_tilde[i] - 1);
			  if (error > ans)
				  ans = error;
		  }
		  return ans;
	  }
	  //
	  void Get_X_tilde()
	  {
		  for (int i = 0; i < size; i++)
		  {
			  cout << "X_tilde[" << i << "] = " << X_tilde[i] << endl;
		  }
	  }
	  void Get_X_precise()
	  {
		  for (int i = 0; i < size; i++)
		  {
			  cout << "X_precise[" << i << "] = " << X_precise[i] << endl;
		  }
	  }
};