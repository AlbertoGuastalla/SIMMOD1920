#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define mat_elem(a, y, x, n) (a + ((y) * (n) + (x)))
#define A(y, x) (*mat_elem(a, y, x, n))
 
void swap_row(double *a, double *b, int r1, int r2, int n)
{
	double tmp, *p1, *p2;
	int i;
 
	if (r1 == r2) return;
	for (i = 0; i < n; i++) {
		p1 = mat_elem(a, r1, i, n);
		p2 = mat_elem(a, r2, i, n);
		tmp = *p1, *p1 = *p2, *p2 = tmp;
	}
	tmp = b[r1], b[r1] = b[r2], b[r2] = tmp;
}
 
void gauss_eliminate(double *a, double *b, double *x, int n)
{
	int i, j, col, row, max_row,dia;
	double max, tmp;
 
	for (dia = 0; dia < n; dia++) {
		max_row = dia, max = A(dia, dia);
 
		for (row = dia + 1; row < n; row++)
			if ((tmp = fabs(A(row, dia))) > max)
				max_row = row, max = tmp;
 
		swap_row(a, b, dia, max_row, n);
 
		for (row = dia + 1; row < n; row++) {
			tmp = A(row, dia) / A(dia, dia);
			for (col = dia+1; col < n; col++)
				A(row, col) -= tmp * A(dia, col);
			A(row, dia) = 0;
			b[row] -= tmp * b[dia];
		}
	}
	for (row = n - 1; row >= 0; row--) {
		tmp = b[row];
		for (j = n - 1; j > row; j--)
			tmp -= x[j] * A(row, j);
		x[row] = tmp / A(row, row);
	}
}

double* transposeMatrix (double m[], int n) {
	double *ret = (double*) malloc ((n * n) * sizeof(double));
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			ret[i * n + j] = m[i + j * n];
		}
	}
	
	return ret;
}

double* subractIdentityMatrix (double m[], int n) {
	double *ret = (double*) malloc ((n * n) * sizeof(double));	

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i == j) {
				ret[i * n + j] = m[i * n + j] - 1.0;
			} else {
				ret[i * n + j] = m[i * n + j];
			}
		}
	}
	
	return ret;
}

double* getVisitVector (double Q[], int stations) {
	double *b = (double*) calloc (stations, sizeof(double));
	double *x = (double*) calloc (stations, sizeof(double));
	double *m = subractIdentityMatrix(Q, stations);
	double *a = transposeMatrix(m, stations);
	double *A = (double*) malloc (stations * (stations - 1) * sizeof(double));
	
	for (int i = 0; i < stations; i++) {
		for (int j = 0; j < stations; j++) {
			if ((j + stations * i) % stations == 0) {
				b[i] = -a[(j + stations * i)];
				a[(j + stations * i)] = 0;	
			}
		}
	}
	
	for (int i = 0; i < stations; i++) {
		for (int j = 1; j < stations; j++) {
			A[i * (stations - 1) + (j - 1)] = a[i * stations + j];
		}
	}
	
	gauss_eliminate(A, b, x, stations - 1);
	
	for (int i = stations - 1; i > 0; i--) {
		x[i] = x[i - 1];
	}
	
	x[0] = 1;
		
	return x;
}	

double** MVA_LI_D (double *visits, double *sevices, int *stations, int* delayStationsTimes, int M, int N) {
	double** averagePerformanceIndices = (double**) malloc (4 * sizeof(double*));
	double *meanJobs = (double*) calloc (M, sizeof(double));
	double *throughputs = (double*) calloc (M, sizeof(double));
	double *utilizations = (double*) calloc (M, sizeof(double));
	double *meanWaitings = (double*) calloc (M, sizeof(double));
	
		//MODIFIED BY GB
		printf("Number of Customers in the System: %d\n\n", N);
	
	for (int i = 0; i < M; i++) {
		meanJobs[i] = 0.0;
	}
	
	for (int k = 1; k <= N; k++) {
		
		
		//MODIFIED BY GB
		double sum = 0.0;
		//int sum = 0.0;
		for (int i = 0; i < M; i++) {
			if (stations[i] == 0) {
				meanWaitings[i] = delayStationsTimes[i];
			} else {
				meanWaitings[i] = sevices[i] * (1 + meanJobs[i]);
			}
			
			sum += visits[i] * meanWaitings[i];
		
		}
		
		double Xref = k / sum;
		
		for (int i = 0; i < M; i++) {
			throughputs[i] = visits[i] * Xref;
			if (stations[i] == 0) {
				meanJobs[i] = delayStationsTimes[i] * throughputs[i];
				utilizations[i] = meanJobs[i] / k;
			} else {
				utilizations[i] = sevices[i] * throughputs[i];
				meanJobs[i] = utilizations[i] * (1 + meanJobs[i]);
			}
		}
	}
	
	averagePerformanceIndices[0] = meanJobs;
	averagePerformanceIndices[1] = throughputs;
	averagePerformanceIndices[2] = utilizations;
	averagePerformanceIndices[3] = meanWaitings;
	
	return averagePerformanceIndices;
}

int main (int argc, char *argv[]) {
	FILE* fp;
	
	if (argc > 1) {
		int stations = 5;
		double Q[] = {
			0.00, 1.00, 0.00,  0.00, 0.00,
			0.00, 0.00, 1.00,  0.00, 0.00,
			0.004, 0.006, 0.90,  0.065, 0.025,
			0.00, 0.00, 1.00,  0.00, 0.00,
			0.00, 0.00, 1.00,  0.00, 0.00
		};
		
		double *visits = getVisitVector(Q, stations);
		double sevices[] = { 0.0, 210.0, 2.7, 40.0, 180.0 };
		int stationsType[] = { 0, 1, 1, 1, 1 }; //0 Delay, 1 Normal
		int delayStationsTimes[] = { 5000, 0, 0, 0, 0 };  

		double** averagePerformanceIndices = MVA_LI_D(visits, sevices, stationsType, delayStationsTimes, stations, atoi(argv[1]));
		
		fp = fopen("mean_values.bin","wb");
		
		if (fp == NULL) {
      		printf("Error Occurred.\n"); 
      		return EXIT_FAILURE;             
   		} else {
   			fwrite(averagePerformanceIndices[0], sizeof(double), stations, fp);
		}
	
		for (int i = 0; i < stations; i++) {
			printf("Mean Value Jobs in station n.%d: %g \n", i, averagePerformanceIndices[0][i]);
			printf("throughput of station n.%d: %g \n", i, averagePerformanceIndices[1][i]);
			printf("utilization of station n.%d: %g \n", i, averagePerformanceIndices[2][i]);
			printf("Average waiting time of station n.%d: %g \n", i, averagePerformanceIndices[3][i]);
			printf("\n");
		}
	} else {
		printf("Missing parameter...");
	}
			
	return 0;
}
