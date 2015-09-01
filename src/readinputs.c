#include "eigen.h"
#include <string.h>

void rm_sub_string(char *substr,const char *toremove);
void set_secondary_inputs(void);
void read_input_file(char *fname);


void read_arguments(int argc, char *argv[]) {
/*  Read command line arguments.

  Nr
  ri
  ro
  Mdisk
  eps
  h0
  sigma_index
  flare_index
  alpha_s
  alpha_b
  nprocs
  adi_gam
  beta_cool
  tol
  Nplanets
  outputname







*/

    if (strstr(argv[1],"-i")) {
      read_input_file(argv[2]);
    }
    else {
    	N = atoi(argv[1]);
    	ri = atof(argv[2]);

    	ro = atof(argv[3]);

      Mdisk = atof(argv[4]);

      eps = atof(argv[5]);
  	  h0 = atof(argv[6]);
  	  sigma_index = atof(argv[7]);
  	  flare_index = atof(argv[8]);

  	  alpha_s = atof(argv[9]);
  	  alpha_b = atof(argv[10]);
      nprocs = atoi(argv[11]);
      adi_gam = atof(argv[12]);
    	beta_cool = atof(argv[13]);
  	  tol = atof(argv[14]);
      NP = atoi(argv[15]);
      sprintf(outputname,"%s.hdf5",argv[16]);

    }
    set_secondary_inputs();
    return;
}


void read_input_file(char *fname) {
  char garbage[100],inputstr[MAXSTRLEN];
	char *gchar;
  int read_res;
  FILE *f;

  printf("Reading input file %s...\n", fname);



  f = fopen(fname,"r");

  if (f==NULL) printf("\n\nERROR Can't Find Input File!\n\n");

	gchar=fgets(garbage,sizeof(garbage),f);	// Input Parameters
	read_res=fscanf(f,"nr = %d \n",&N);
  read_res=fscanf(f,"ri = %lg \n",&ri);
  read_res=fscanf(f,"ro = %lg \n",&ro);
  read_res=fscanf(f,"mdisk = %lg \n",&Mdisk);
  read_res=fscanf(f,"rs = %lg \n",&eps);
  read_res=fscanf(f,"h0 = %lg \n",&h0);
  read_res=fscanf(f,"sig_ind = %lg \n",&sigma_index);
  read_res=fscanf(f,"flare_ind = %lg \n",&flare_index);
  read_res=fscanf(f,"alpha_s = %lg \n",&alpha_s);
  read_res=fscanf(f,"alpha_b = %lg \n",&alpha_b);
  read_res=fscanf(f,"np = %d \n",&nprocs);
  read_res=fscanf(f,"gam = %lg \n",&adi_gam);
  read_res=fscanf(f,"beta = %lg \n",&beta_cool);
  read_res=fscanf(f,"tol = %lg \n",&tol);
  read_res=fscanf(f,"Nplanets = %d \n",&NP);
  read_res=fscanf(f,"outputname = %s",inputstr);
  sprintf(outputname,"%s.hdf5",inputstr);
  fclose(f);

  rm_sub_string(fname,"params.in");
  char *dirname = fname;
  sprintf(outputname,"%s%s.hdf5",dirname,inputstr);

// Leave me alone compiler
  read_res += 1; garbage[0] = gchar[0];



  return;




}


void set_secondary_inputs(void) {

  dlr = (log(ro) - log(ri))/((float) N);
  nrows = N; ncols = N;
#ifdef PLANETS
  nrows += NP;
  ncols += NP;
#else
  NP = 0;
#endif


#ifdef INPUTMASS
  sigma0 = 1;
#else
  sigma0 = Mdisk;
  Mdisk = 1;
#endif

  temp_index = 2*flare_index - 1;

#ifndef COOLING
  adi_gam = 1;
  beta_cool = 0;
#endif

  return;
}

void rm_sub_string(char *substr,const char *toremove) {
  while ( (substr=strstr(substr,toremove)) ) {
    memmove(substr,substr+strlen(toremove),1+strlen(substr+strlen(toremove)));
  }
  return;
}
