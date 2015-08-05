#include "eigen.h"
#include <string.h>

#if !defined(DERIVSECOND) && !defined(DERIVFOURTH) && !defined(DERIVSIXTH) && !defined(DERIVEIGHTH)
#include "derivs/d2.h"
#else
#ifdef DERIVSECOND
#include "derivs/d2.h"
#endif
#ifdef DERIVFOURTH
#include "derivs/d4.h"
#endif
#ifdef DERIVSIXTH
#include "derivs/d6.h"
#endif
#ifdef DERIVEIGHTH
#include "derivs/d8.h"
#endif
#endif


void fillD1(double *dmat, int size) {
  int i,j,im,jm;
  int indx;
/* Initialize everything to zero */
  memset(dmat, 0, size*size*sizeof(double));

/* Fill first DACC/2 rows with forward diff  */
  for(i=0;i<DACC/2;i++) {
    im = i;
    for(j=0;j<D1SIZE;j++) {
      jm = im+d1off[i][j];
      indx = jm + im*size;
      dmat[indx] = d1cnum[i][j]/d1cden[i][j];
    }
  }

/* Fill last DACC/2 rows with backward diff */

  for(i=D1SIZE-DACC/2;i<D1SIZE;i++) {
    im = size - D1SIZE + i;
    for(j=0;j<D1SIZE;j++) {
      jm = im + d1off[i][j];
      indx = jm + im*size;
      dmat[indx] = d1cnum[i][j]/d1cden[i][j];
    }
  }



/* Fill the rest with the centered diff */

  i = DACC/2;
  for(im=DACC/2;im<(size-DACC/2);im++) {
    for(j=0;j<D1SIZE;j++) {
      jm = im + d1off[i][j];
      indx = jm + im*size;
      dmat[indx] = d1cnum[i][j]/d1cden[i][j];
    }
  }

  return;

}

void fillD2(double *dmat, int size) {
  int i,j,im,jm;
  int indx;
/* Initialize everything to zero */
  memset(dmat, 0, size*size*sizeof(double));

/* Fill first DACC/2 rows with forward diff  */
  for(i=0;i<DACC/2;i++) {
    im = i;
    for(j=0;j<D2SIZE;j++) {
      jm = im+d2off[i][j];
      indx = jm + im*size;
      dmat[indx] = d2cnum[i][j]/d2cden[i][j];
    }
  }

/* Fill last DACC/2 rows with backward diff */

  for(i=D1SIZE-DACC/2;i<D1SIZE;i++) {
    im = size - D1SIZE + i;
    for(j=0;j<D2SIZE;j++) {
      jm = im + d2off[i][j];
      indx = jm + im*size;
      dmat[indx] = d2cnum[i][j]/d2cden[i][j];
    }
  }



/* Fill the rest with the centered diff */

  i = DACC/2;
  for(im=DACC/2;im<(size-DACC/2);im++) {
    for(j=0;j<D2SIZE;j++) {
      jm = im + d2off[i][j];
      indx = jm + im*size;
      dmat[indx] = d2cnum[i][j]/d2cden[i][j];
    }
  }

  return;

}


void init_derivatives(void) {
	int i,j,indx;
	double dfac = 1.0/dlr;
	double d2fac = dfac*dfac;

	fillD1(D,N);
	fillD2(D2,N);

  for(i=0;i<N;i++) {
    for(j=0;j<N;j++) {
      indx = j+i*N;
      if (i==j) {
        Identity[indx] = 1;
      }
      else {
        Identity[indx] = 0;
      }
      D[indx] *= dfac;
      D2[indx] *= d2fac;
    }
  }


	return;

}
