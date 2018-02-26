#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <papi.h>
#define threshold 0.0000001
#define N 128
#define T 32

#define PAPI_ERROR_CHECK(X) \
        if((X)!=PAPI_OK) \
    {fprintf(stderr,"PAPI Error \n"); exit(-1);}

double A[N][N][N], C[N][N], B[N][N][N], CC[N][N];

int main(){
double rtclock();
void pa1p3(int n, double x[n][n][n], double y[n][n][n], double z[n][n]);
void pa1p3opt(int n, double x[n][n][n], double y[n][n][n], double z[n][n]);
void compare(int n, double wref[n][n], double w[n][n]);
void papi_print_helper(const char* msg, long long *values);

double clkbegin, clkend;
double t;
double rtclock();

int i,j,k;

  for(i=0;i<N;i++)
   for(j=0;j<N;j++) {
    C[i][j] =0; 
    for(k=0;k<N;k++) {
     A[i][j][k] = (i+2*j+3*k)/(3*N);
     B[i][j][k] = (i-2*j+k)/(3*N);
    }
   }

#ifdef ENABLE_PAPI
    int event_set = PAPI_NULL;
    PAPI_library_init(PAPI_VER_CURRENT);
    PAPI_ERROR_CHECK(PAPI_create_eventset(&event_set));
    PAPI_ERROR_CHECK(PAPI_add_event(event_set, PAPI_DP_OPS));
    PAPI_ERROR_CHECK(PAPI_add_event(event_set, PAPI_VEC_DP));
    PAPI_ERROR_CHECK(PAPI_add_event(event_set, PAPI_L3_TCM));
    PAPI_ERROR_CHECK(PAPI_add_event(event_set, PAPI_RES_STL));
    long_long papi_values[4];
    PAPI_ERROR_CHECK(PAPI_start(event_set));
#endif

  clkbegin = rtclock();
  pa1p3(N,A,B,C);
  clkend = rtclock();

#ifdef ENABLE_PAPI
    PAPI_ERROR_CHECK(PAPI_stop(event_set, papi_values));
    papi_print_helper("Base Version",papi_values);
#endif

  t = clkend-clkbegin;
  if (C[N/2][N/2]*C[N/2][N/2] < -100.0) printf("%f\n",C[N/2][N/2]);
  printf("Problem 3 Reference Version: Tensor Size = %d; %.2f GFLOPS; Time = %.3f sec; \n",
          N,2.0*1e-9*N*N*N*N/t,t);

  for(i=0;i<N;i++)
   for(j=0;j<N;j++) 
    CC[i][j] =0; 

#ifdef ENABLE_PAPI
    PAPI_ERROR_CHECK(PAPI_start(event_set));
#endif

  clkbegin = rtclock();
  pa1p3opt(N,A,B,CC);
  clkend = rtclock();

#ifdef ENABLE_PAPI
    PAPI_ERROR_CHECK(PAPI_stop(event_set, papi_values));
    papi_print_helper("Optimized Version",papi_values);
#endif

  t = clkend-clkbegin;
  if (CC[N/2][N/2]*CC[N/2][N/2] < -100.0) printf("%f\n",CC[N/2][N/2]);
  printf("Problem 3 Optimized Version: Tensor Size = %d; %.2f GFLOPS; Time = %.3f sec; \n",
          N,2.0*1e-9*N*N*N*N/t,t);

  compare(N,C,CC);

  return 0; // add return to satisfy make
}

void pa1p3(int n, double x[n][n][n], double y[n][n][n], double z[n][n])
{ int i,j,k,l;
  for(i=0;i<n;i++)
   for(j=0;j<n;j++)
    for(k=0;k<n;k++)
     for(l=0;l<n;l++)
        z[l][k] += x[l][i][j]*y[i][j][k];

}

void pa1p3opt(int n, double x[n][n][n], double y[n][n][n], double z[n][n])
// Initially identical to reference; make your changes to optimize this code
{
	int i, j, k, l;

	for( i=0; i<n; i++ ) { // Permute ijkl -> iljk to maximize locality
		for( l=0; l<n/2; l++ ) // Split loop to quickly tile
			for( j=0; j<n; j++ )
				for( k=0; k<n; k++ )
					z[l][k] += y[i][j][k]*x[l][i][j];

		for( l=n/2; l<n; l++ ) // Split loop to quickly tile
			for( j=0; j<n; j++ ) 
				for( k=0; k<n; k++ )
					z[l][k] += y[i][j][k]*x[l][i][j];
	}
}

double rtclock()
{
  struct timezone Tzp;
  struct timeval Tp;
  int stat;
  stat = gettimeofday (&Tp, &Tzp);
  if (stat != 0) printf("Error return from gettimeofday: %d",stat);
  return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}

void compare(int n, double wref[n][n], double w[n][n])
{
double maxdiff,this_diff;
int numdiffs;
int i,j;
  numdiffs = 0;
  maxdiff = 0;
  for (i=0;i<n;i++)
   for (j=0;j<n;j++)
    {
     this_diff = wref[i][j]-w[i][j];
     if (this_diff < 0) this_diff = -1.0*this_diff;
     if (this_diff>threshold)
      { numdiffs++;
        if (this_diff > maxdiff) maxdiff=this_diff;
      }
    }
   if (numdiffs > 0)
      printf("%d Diffs found over threshold %f; Max Diff = %f\n",
               numdiffs,threshold,maxdiff);
   else
      printf("No differences found between base and test versions\n");
}

void papi_print_helper(const char* msg, long long *values)
{
    printf("\n=====================PAPI COUNTERS==========================\n\n");
    printf("(%s): DP operations : %.2f G\n",          msg, values[0]*1e-9);
    printf("(%s): DP vector instructions : %.2f M\n", msg, values[1]*1e-6);
    printf("(%s): L3 cache misses : %.2f M\n",              msg, values[2]*1e-6);
    printf("(%s): Resource Stall Cycles: %.2f M\n",         msg, values[3]*1e-6);
    printf("=================PAPI COUNTERS END==========================\n\n");
}

