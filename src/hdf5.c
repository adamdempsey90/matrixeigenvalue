#include "eigen.h"
#ifdef HDF5_OUTPUT

#include <hdf5.h>
#include <string.h>

#define MAXCOLS 30

typedef struct param_t {
	int nr;
	double ri;
	double ro;
	double mdisk;
	double rs;
	double h0;
	double sig_ind;
	double flare_ind;
	double alpha_s;
	double alpha_b;
	int np;
	double gam;
	double beta;
	double tol;
	int Nplanets;

} param_t;


typedef struct {
   double r;   /*real part */
   double i;   /*imaginary part */
} complex_t;


hid_t cdatatype, strdatatype;

hid_t file_id, root_id, matrices_id, results_id, globals_id, params_id;




void create_complex_type(void);
void write_hdf5_results(double complex *evecs, double complex *evals);
void write_hdf5_globals(void) ;
void write_hdf5_matrices(double complex *mat, double complex *bcmat);
void write_hdf5_double(double *data, hsize_t *shape, int ndims, hid_t group_path, char *name);
void write_hdf5_complex(double complex *data, hsize_t *shape, int ndims, hid_t group_path, char *data_name);
void write_hdf5_params(void);
void write_hdf5_defines(void);
void write_hdf5_strings(char *data[], hsize_t *dims, int ndims,hid_t group_path, char *name);

void output_hdf5_file(double complex *mat,double complex *bcmat,double complex *evecs,double complex *evals) {
  herr_t status;
  printf("Outputting Results to %s...\n",outputname);
  create_complex_type();
  file_id = H5Fcreate(outputname, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  root_id = H5Gcreate(file_id,"/Mateig",0);
  globals_id = H5Gcreate(root_id,"Globals",0);
  matrices_id = H5Gcreate(root_id,"Matrices",0);
  results_id = H5Gcreate(root_id,"Results",0);
  params_id = H5Gcreate(root_id,"Parameters",0);

  write_hdf5_globals();
  write_hdf5_matrices(mat,bcmat);
  write_hdf5_results(evecs,evals);
  write_hdf5_params();
	write_hdf5_defines();

  status = H5Tclose(cdatatype);
	if (status < 0) printf("HDF5 error\n");
  status = H5Gclose(globals_id);
	if (status < 0) printf("HDF5 error\n");
  status = H5Gclose(matrices_id);
	if (status < 0) printf("HDF5 error\n");
  status = H5Gclose(results_id);
	if (status < 0) printf("HDF5 error\n");
  status = H5Gclose(params_id);
	if (status < 0) printf("HDF5 error\n");
  status = H5Gclose(root_id);
	if (status < 0) printf("HDF5 error\n");
  status = H5Fclose(file_id);
	if (status < 0) printf("HDF5 error\n");


  return;


}

void create_complex_type(void) {
/* Create the complex data type using the complex_t struct */

  complex_t tmp;
  cdatatype = H5Tcreate (H5T_COMPOUND, sizeof tmp);
  H5Tinsert (cdatatype, "r", HOFFSET(complex_t,r),
            H5T_NATIVE_DOUBLE);
  H5Tinsert (cdatatype, "i", HOFFSET(complex_t,i),
            H5T_NATIVE_DOUBLE);


  return;
}

void write_hdf5_params(void) {
  hid_t memtype,dspc_id, dset_id;
  herr_t status;
  hsize_t dims[1] = {1};

  param_t params;


  params.nr = N;
  params.ri = ri;
  params.ro = ro;

#ifdef INPUTMASS
  params.mdisk = Mdisk;
#else
  params.mdisk = sigma0;
#endif

  params.rs = eps;
  params.h0 = h0;
  params.sig_ind = sigma_index;
  params.flare_ind = flare_index;
  params.alpha_s = alpha_s;
  params.alpha_b = alpha_b;
  params.np = nprocs;
  params.gam = adi_gam;
  params.beta = beta_cool;
  params.tol = tol;
  params.Nplanets = NP;


  memtype = H5Tcreate (H5T_COMPOUND, sizeof (param_t));
  status = H5Tinsert (memtype, "nr", HOFFSET (param_t, nr), H5T_NATIVE_INT);
	if (status < 0) printf("HDF5 error\n");
  status = H5Tinsert (memtype, "ri", HOFFSET (param_t, ri), H5T_NATIVE_DOUBLE);
	if (status < 0) printf("HDF5 error\n");
  status = H5Tinsert (memtype, "ro", HOFFSET (param_t, ro), H5T_NATIVE_DOUBLE);
	if (status < 0) printf("HDF5 error\n");
  status = H5Tinsert (memtype, "mdisk", HOFFSET (param_t, mdisk), H5T_NATIVE_DOUBLE);
	if (status < 0) printf("HDF5 error\n");
  status = H5Tinsert (memtype, "rs", HOFFSET (param_t, rs), H5T_NATIVE_DOUBLE);
	if (status < 0) printf("HDF5 error\n");
  status = H5Tinsert (memtype, "h0", HOFFSET (param_t, h0), H5T_NATIVE_DOUBLE);
	if (status < 0) printf("HDF5 error\n");
  status = H5Tinsert (memtype, "sig_ind", HOFFSET (param_t, sig_ind), H5T_NATIVE_DOUBLE);
	if (status < 0) printf("HDF5 error\n");
  status = H5Tinsert (memtype, "flare_ind", HOFFSET (param_t, flare_ind), H5T_NATIVE_DOUBLE);
	if (status < 0) printf("HDF5 error\n");
  status = H5Tinsert (memtype, "alpha_s", HOFFSET (param_t, alpha_s), H5T_NATIVE_DOUBLE);
	if (status < 0) printf("HDF5 error\n");
  status = H5Tinsert (memtype, "alpha_b", HOFFSET (param_t, alpha_b), H5T_NATIVE_DOUBLE);
	if (status < 0) printf("HDF5 error\n");
  status = H5Tinsert (memtype, "np", HOFFSET (param_t, np), H5T_NATIVE_INT);
	if (status < 0) printf("HDF5 error\n");
  status = H5Tinsert (memtype, "gam", HOFFSET (param_t, gam), H5T_NATIVE_DOUBLE);
	if (status < 0) printf("HDF5 error\n");
  status = H5Tinsert (memtype, "beta", HOFFSET (param_t, beta), H5T_NATIVE_DOUBLE);
	if (status < 0) printf("HDF5 error\n");
  status = H5Tinsert (memtype, "tol", HOFFSET (param_t, tol), H5T_NATIVE_DOUBLE);
	if (status < 0) printf("HDF5 error\n");
  status = H5Tinsert (memtype, "Nplanets", HOFFSET (param_t, Nplanets), H5T_NATIVE_INT);
	if (status < 0) printf("HDF5 error\n");



  dspc_id = H5Screate_simple(1,dims,NULL);
  dset_id = H5Dcreate(params_id,"Parameters",memtype,dspc_id,H5P_DEFAULT);

  status = H5Dwrite(dset_id,memtype,H5S_ALL,H5S_ALL,H5P_DEFAULT,&params);
	if (status < 0) printf("HDF5 error\n");
  status = H5Sclose(dspc_id);
	if (status < 0) printf("HDF5 error\n");
  status = H5Dclose(dset_id);
	if (status < 0) printf("HDF5 error\n");

  return;

}





void write_hdf5_results(double complex *evecs, double complex *evals) {

  hsize_t dims2[2] = {nrows,ncols};
  hsize_t dims1[1] = {nrows};

  write_hdf5_complex(evecs,dims2,2,results_id,"Evecs");
  write_hdf5_complex(evals,dims1,1,results_id,"Evals");

  return;


}


void write_hdf5_globals(void) {

  hsize_t dims[1] = {N};


  write_hdf5_double(lr,dims,1,globals_id,"lr");
  write_hdf5_double(r,dims,1,globals_id,"r");
  write_hdf5_double(omega,dims,1,globals_id,"Omega");
  write_hdf5_double(c2,dims,1,globals_id,"c2");
  write_hdf5_double(sigma,dims,1,globals_id,"Sigma");
  write_hdf5_double(aspect_ratio,dims,1,globals_id,"hor");
  write_hdf5_double(pres,dims,1,globals_id,"P");
  write_hdf5_double(temp,dims,1,globals_id,"T");
  write_hdf5_double(omega_prec,dims,1,globals_id,"wp");
  write_hdf5_double(dldc2,dims,1,globals_id,"dc2");
  write_hdf5_double(dlds,dims,1,globals_id,"ds");
  write_hdf5_double(dldpres,dims,1,globals_id,"dp");
  write_hdf5_double(dldtemp,dims,1,globals_id,"dt");
  write_hdf5_double(d2lds,dims,1,globals_id,"d2s");
  write_hdf5_double(d2ldpres,dims,1,globals_id,"d2p");
  write_hdf5_double(d2ldtemp,dims,1,globals_id,"d2t");






  return;
}


void write_hdf5_matrices(double complex *mat, double complex *bcmat) {
/* Write all of the matrices */
  int i,j;
  double complex *coeffs_mat = (double complex *)malloc(sizeof(double complex)*N*3);

  hsize_t dims1[1], dims2[2];


  dims2[0] = N; dims2[1]= N;
  dims1[0] = N;


  write_hdf5_double(D, dims2, 2, matrices_id, "D");
  write_hdf5_double(D2, dims2, 2, matrices_id, "D2");
  write_hdf5_double(weights, dims1, 1, matrices_id, "Weights");


#ifdef SELFGRAVITY
  write_hdf5_double(kernel,dims2,2,matrices_id,"Kernel");
#endif

  dims2[0] = nrows; dims2[1] = ncols;
  write_hdf5_complex(bcmat,dims2,2,matrices_id,"BC");
  write_hdf5_complex(mat,dims2,2,matrices_id,"Matrix");





  dims2[0] = N; dims2[1] = 3;

  for(i=0;i<N;i++) {
      for(j=0;j<3;j++) {
        coeffs_mat[0 + 3*i] = coeffs_A[i];
        coeffs_mat[1 + 3*i] = coeffs_B[i];
        coeffs_mat[2 + 3*i] = coeffs_C[i];
      }
  }

  write_hdf5_complex(coeffs_A,dims2,2,matrices_id,"Coeffs");
  free(coeffs_mat);

  return;
}

void write_hdf5_defines(void) {
  int i,size;
  FILE *f;

  char input[MAXSTRLEN];
	char tok;
  char plus = '+';
  char *defs[MAXCOLS];

  size = 0;
	printf("Reading params.opt file to store options...\n");
	f = fopen("params.opt","r");
	if (f==NULL) {
    printf("\tCant find params.opt file!");
    return;
  }

  while (fscanf(f, "%c%s\n",&tok,input) != EOF) {
		if (tok == plus) {
	    if (size > MAXCOLS) {
	      printf("\tExceeded maximum defines of %d. Recompile with higher MAXCOLS.",MAXCOLS);
	      break;
	    }
	    defs[size] = malloc(sizeof(input) + 1);
	    strcpy(defs[size],input);
	    size++;
		}
  }
  fclose(f);

  hsize_t dims[] = {size};

	write_hdf5_strings(defs,dims, 1, params_id,"Defines");

	printf("Found the defines:\n");
  for(i=0;i<size;i++) {

		printf("\t%s\n",defs[i]);
		free(defs[i]);
	}


	return;
}


void write_hdf5_double(double *data, hsize_t *dims, int ndims, hid_t group_path, char *name) {
  hid_t dspc_id, dset_id;
  herr_t status;

  dspc_id = H5Screate_simple(ndims,dims,NULL);
  dset_id = H5Dcreate(group_path,name,H5T_NATIVE_DOUBLE,dspc_id,H5P_DEFAULT);

  status = H5Dwrite(dset_id,H5T_NATIVE_DOUBLE,H5S_ALL,H5S_ALL,H5P_DEFAULT,data);
	if (status < 0) printf("HDF5 error\n");
  status = H5Sclose(dspc_id);
	if (status < 0) printf("HDF5 error\n");
  status = H5Dclose(dset_id);
	if (status < 0) printf("HDF5 error\n");



  return;


}



void write_hdf5_complex(double complex *data, hsize_t *dims, int ndims, hid_t group_path, char *name) {

  hid_t dspc_id, dset_id;
  herr_t status;

  dspc_id = H5Screate_simple(ndims,dims,NULL);
  dset_id = H5Dcreate(group_path,name,cdatatype,dspc_id,H5P_DEFAULT);

  status = H5Dwrite(dset_id,cdatatype,H5S_ALL,H5S_ALL,H5P_DEFAULT,data);
	if (status < 0) printf("HDF5 error\n");
  status = H5Sclose(dspc_id);
	if (status < 0) printf("HDF5 error\n");
  status = H5Dclose(dset_id);
	if (status < 0) printf("HDF5 error\n");



  return;


}

void write_hdf5_strings(char *data[], hsize_t *dims, int ndims,hid_t group_path, char *name) {
	hid_t dspc_id, dset_id;
	herr_t status;

	strdatatype = H5Tcopy (H5T_C_S1);
  status = H5Tset_size (strdatatype,H5T_VARIABLE);
	if (status < 0) printf("HDF5 error\n");

	dspc_id = H5Screate_simple(ndims,dims,NULL);
	dset_id = H5Dcreate(group_path,name,strdatatype,dspc_id,H5P_DEFAULT);

	status = H5Dwrite(dset_id,strdatatype,H5S_ALL,H5S_ALL,H5P_DEFAULT,data);
	if (status < 0) printf("HDF5 error\n");
	status = H5Tclose(strdatatype);
	if (status < 0) printf("HDF5 error\n");
	status = H5Sclose(dspc_id);
	if (status < 0) printf("HDF5 error\n");
	status = H5Dclose(dset_id);
	if (status < 0) printf("HDF5 error\n");
	return;
}






#endif
