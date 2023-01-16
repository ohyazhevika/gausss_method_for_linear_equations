#include <iostream>
#include <fstream>
#include "System_of_linear_equations.h"
using namespace std;

int main()
{
	srand(time(NULL));
	for (int i = 1; i <= 3; i++)
	{
		int matr_size=pow(10, i);
		cout << "Matrices (" << matr_size << " x " << matr_size << "):\n";
		for (int j = 1; j <= 3; j++)
		{
			int elem_range = pow(10, j);
			cout << "     Elements in the range from " << -elem_range << " to " << elem_range << ". ";
			double aver_acc_assessment = 0;
			double aver_relative_error = 0;
			for (int t = 0; t < 10; )
			{
				System sy;
				sy.GenerateSystem(elem_range, matr_size);
				if (matr_size == 10)
				{
					sy.SystemOutput();
					cout << endl;
					for (int i = 0; i < sy.size; i++)
						cout << "X[" << i << "] = " << sy.X_precise[i] << endl;
				}
				if (sy.Solve())
				{
					t++;
					aver_acc_assessment += sy.AccuracyAssessment();
					aver_relative_error += sy.RelativeError();
				}
			}
			aver_acc_assessment /= 10;
			aver_relative_error /= 10;
			cout << "Average error is " << aver_acc_assessment << ". Relative error is " << aver_relative_error << endl << endl;
		}
	}
	return 0;
}