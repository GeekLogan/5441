//
// Compile using gcc -O3 -ftree-vectorizer-verbose=2 filename.c >& filename.gccvecrpt
// Compile using icc -O3 -qopt-report=2 -qopt-report-phase=vec filename.c ; Report will be placed in filename.optrpt

#define N 1024
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>

float w[N],x[N],y[N],z[N];
float a[N],b[N],c[N],d[N],e[N];
float sum, A[N][N], vsum[N];

int main(int argc, char *argv[]) {

int i,j,k;

for(i=0;i<N-1;i++)
{
	w[i+1] = x[i]+1; //   S1
	y[i] = 2*w[i];   //   S2
}


for(i=0;i<N-1;i++)
{
	w[i] = y[i]+1;     //   S1
	y[i+1] = 2*x[i];   //   S2
}


for(i=0;i<N-1;i++)
{
	y[i+1] = 2*x[i];   //   S2
	w[i] = y[i]+1;     //   S1
}


for (i=0; i<N-1; i++) {
	x[i+1] = y[i]+1; //   S1
	y[i+1] = 2*x[i]; //   S2
}

for (i=0; i<N-1; i++) {
	x[i+1] = y[i]+x[i];   //   S1
	y[i+1] = 2*y[i]+z[i]; //   S2
}

for (j=0; j<N; j++)
	for(i=1; i<N; i++)
		A[i][j] = A[i-1][j]+1; // S1


for (i=1; i<N; i++)
  for(j=0; j<N; j++)
    A[i][j] = A[i-1][j]+1; // S1

for (i=0; i<N; i++) {
	sum = 0.0;
	for(j=0; j<N; j++)
		sum += A[j][i]*A[j][i];
	x[i] = sum;
}

for (i=0; i<N; i++) vsum[i]=0;
for (i=0; i<N; i++)
	for(j=0; j<N; j++)
		vsum[j] += A[i][j]*A[i][j];
for (i=0; i<N; i++) x[i] = vsum[i];


for (i=1;i<N;i++){
	a[i] = b[i] + c[i];
	d[i] = a[i] + e[i-1];
	e[i] = d[i] + c[i];
}

for (i=1;i<N;i++)
	a[i] = b[i] + c[i];
for (i=1;i<N;i++){
	d[i] = a[i] + e[i-1];
	e[i] = d[i] + c[i];
}

}
