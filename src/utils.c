#include "eigen.h"


double Dij(int i, int j) {
	
	if (i==0) {
		if (j==0) {
			return -1.0;
		}
		else if (j==1) {
			return 1.0;
		}
		else {
			return 0;
		}
	}
	else if (i==(N-1)) {
		if (j==(N-2)) {
			return -1.0;
		}
		else if (j==(N-1)) {
			return 1.0;
		}
		else {
			return 0;
		}
	}
	else {
		if (i == (j+1)) {
			return -.5;
		}
		else if (i==(j-1)) {
			return .5;
		}
		else {
			return 0;
		}
	}
		
}

double D2ij(int i, int j) {
	
	if (i==0) {
		if (j==0) {
			return 1.0;
		}
		else if (j==1) {
			return -2.0;
		}
		else if (j==2) {
		
			return 1.0;
		}
		else {
			return 0;
		}
	}
	else if (i==(N-1)) {
		
		if (j==(N-3)) {
			return 1.0;
		
		}
		else if (j==(N-2)) {
			return -2.0;
		}
		else if (j==(N-1)) {
			return 1.0;
		}
		else {
			return 0;
		}
	}
	else {
		if (i == (j+1)) {
			return 1.0;
		}
		else if (i==(j-1)) {
			return 1.0;
		}
		else if (i==j) {
			
			return -2.0;
		
		}
		else {
			return 0;
		}
	}
		
}

void calc_weights(void) {
/* O(N^(-4)) weights for numerical quadrature*/
	int i;
	for(i=0;i<N;i++) {
		if (i==0 || i==N-1) {
			weights[i] = 3./8 * dlr;
		}
		else if (i==1 || i==N-2) {
			weights[i] = 7./6 * dlr;
		}
		else if (i==2 || i==N-3) {
			weights[i] = 23./24 * dlr;
		}
		else {
			weights[i] = dlr;
		}
	
	}
	return;

}



void reigenvalues(double complex *A, double complex *Q, double complex *evals, double complex *evecs, int nA) 
{
/* Computes the eigenvalues and right eigenvectors of the complex matrix A which is NxN.
	A . evecs = evals Q . evecs 
	This is essentially a wrapper for the ZGEEV LAPACK routine.
	
	INPUTS:
		The matrices M, Q, and evecs and the vector evals which are all overwritten
		
	OUTPUTS:
		The eigenvalues are stored in the evals array.
		The eigenvectors are stored in the ROWS of the evecs matrix	
*/
	int i,j;
	char JOBVL = 'N';
	char JOBVR = 'V';
	int INFO; 
	int LDA = nA;
	int LDB = nA;
	int LDVL = nA;
	int LDVR = nA;
	int LWORK = 2*nA;
	
	double *RWORK = (double *)malloc(sizeof(double)*8*nA);
	double complex *CWORK = (double complex *)malloc(sizeof(double complex)*2*nA);
	
	double complex *tA = (double complex *)malloc(sizeof(double complex)*nA*nA);
	double complex *tQ = (double complex *)malloc(sizeof(double complex)*nA*nA);
	
	double complex *evals_alpha = (double complex *)malloc(sizeof(double complex)*nA);
	double complex *evals_beta = (double complex *)malloc(sizeof(double complex)*nA);

	for(i=0;i<nA;i++) {
		for(j=0;j<nA;j++) {
	  		tA[i+nA*j] = A[j+nA*i];
	  		tQ[i+nA*j] = Q[j+nA*i];
		}
	}


//	zgeev_( &JOBVL, &JOBVR, &nA, tA, &LDA, evals, tQ, &LDVL, evecs, &LDVR, CWORK, &LWORK, RWORK, &INFO );
	zggev_( &JOBVL, &JOBVR, &nA, tA, &LDA, tQ, &LDB, evals_alpha,evals_beta, NULL, &LDVL, evecs, &LDVR, CWORK, &LWORK, RWORK, &INFO );
	
	for(i=0;i<nA;i++) {
		if (cabs(evals_beta[i]) != 0) {
			evals[i] = evals_alpha[i]/evals_beta[i];
		}
	}
	free(tA); free(tQ);
	free(RWORK); free(CWORK);
	free(evals_alpha);
	free(evals_beta);
 	return;
}

void matmat(double  *A, double *B, double *C, 
					double alpha, double beta, int nA) 
{
/* Performs \alpha * A.B + \beta * C and stores the output in C.
	A,B, and C are all matrices.
	This is essenitally a wrapper for the ZGEMM BLAS routine 
*/
	int i,j;
	char TRANSA = 't';
	char TRANSB = 't';
	int m = nA;
	int n = nA;
	int k = nA;
	int LDA = nA;
	int LDB = nA;
	int LDC = nA;
		 
	
	for(i=0;i<nA;i++) {
		for(j=0;j<nA;j++) work[i+N*j] = C[j + nA*i];
	}
	
	for(i=0;i<nA;i++) {
		for(j=0;j<nA;j++)	C[i + nA*j] = work[i+N*j];
	}
	
	dgemm_(&TRANSA, &TRANSB, &m,&n,&k,&alpha,A,&LDA,B,&LDB,&beta,C,&LDC);


	for(i=0;i<nA;i++) {
		for(j=0;j<nA;j++)  work[i+N*j] = C[j + nA*i];
	}
	
	for(i=0;i<nA;i++) {
		for(j=0;j<nA;j++)	C[i + nA*j] = work[i+N*j];
	}
	return;

}
void cmatmat(double complex *A, double complex *B, double complex *C, 
					double complex alpha, double complex beta, int nA) 
{
/* Performs \alpha * A.B + \beta * C and stores the output in C.
	A,B, and C are all matrices.
	This is essenitally a wrapper for the ZGEMM BLAS routine 
*/
	int i,j;
	char TRANSA = 't';
	char TRANSB = 't';
	int m = nA;
	int n = nA;
	int k = nA;
	int LDA = nA;
	int LDB = nA;
	int LDC = nA;
		 
	
	for(i=0;i<nA;i++) {
		for(j=0;j<nA;j++) cwork[i+N*j] = C[j + nA*i];
	}
	
	for(i=0;i<nA;i++) {
		for(j=0;j<nA;j++)	C[i + nA*j] = cwork[i+N*j];
	}
	
	zgemm_(&TRANSA, &TRANSB, &m,&n,&k,&alpha,A,&LDA,B,&LDB,&beta,C,&LDC);


	for(i=0;i<nA;i++) {
		for(j=0;j<nA;j++)  cwork[i+N*j] = C[j + nA*i];
	}
	
	for(i=0;i<nA;i++) {
		for(j=0;j<nA;j++)	C[i + nA*j] = cwork[i+N*j];
	}
	return;

}

void matvec(double  *A, double*B, double *C, 
					double alpha, double beta, int nB) 
{
/* Performs \alpha * A.B + \beta * C and stores the output in C. 
	A is a matrix, B and C are vectors.
	This is essenitally a wrapper for the ZGEMV BLAS routine 
*/

	char TRANS = 't';
	int m = nB;
	int n = nB;
	int LDA = nB;
	int INCX = 1;
	int INCY = 1;
		 
	

	dgemv_(&TRANS, &m,&n,&alpha,A,&LDA,B,&INCX,&beta,C,&INCY);

	return;

}

void cmatvec(double  complex *A, double complex *B, double complex *C, 
					double complex alpha, double complex beta, int nB) 
{
/* Performs \alpha * A.B + \beta * C and stores the output in C. 
	A is a matrix, B and C are vectors.
	This is essenitally a wrapper for the ZGEMV BLAS routine 
*/

	char TRANS = 't';
	int m = nB;
	int n = nB;
	int LDA = nB;
	int INCX = 1;
	int INCY = 1;
		 
	

	zgemv_(&TRANS, &m,&n,&alpha,A,&LDA,B,&INCX,&beta,C,&INCY);

	return;

}


