#include "eigen.h"


void init_globals(double ri, double ro);

void init_weights(void);
double calc_total_disk_mass(void);
double omega_prec_pres(double x);

int init(double ri,double ro) {
	int i;
	init_derivatives();


	init_weights();

	init_globals(ri,ro);

#ifdef PLANETS
		printf("Initializing %d Planets...\n",NP);
		init_planets();
#endif

/* Initialize Kernels if we're not reading it from a file */

	printf("Calculating Kernels\n");

#ifdef SELFGRAVITY

	compute_kernels();

#ifndef INFINITEDISK
#ifndef NOOMEGAPRECGRAV
	calc_omega_prec_grav();
#endif
#endif
#endif

#ifndef NOPRESSURE
#ifndef NOOMEGAPRECPRES
	for(i=0;i<N;i++) {
		omega_prec[i] += omega_prec_pres(r[i]);
	}
#endif
#endif
#ifdef EXTERNPREC
	for(i=0;i<N;i++) {
		omega_prec[i] += omega_prec_extern(r[i]);
	}
#endif



	return 0;
}

double omega_prec_pres(double x) {
	double delta = dlogtemp_func(x);
	double mu = dlogsigma_func(x);
	double delta_p = d2logtemp_func(x);
	double mu_p = d2logsigma_func(x);
	double fac;

#if defined(ISOTHERMAL) || defined(COOLING)

//	fac = (mu+delta)*(delta+1)*temp_func(x);
	fac = -temp_func(x) * ( (mu+delta)*(1+delta) + mu_p + delta_p);
#endif

#ifdef COOLING
	fac *= adi_gam;
#endif

// #ifdef COOLING
//
// 	fac =  (mu+delta)*(delta+1)*temp_func(x);
//
// #endif

#ifdef BAROTROPIC

//	fac = mu*(delta + 1)*temp_func(x)*adi_gam;
	fac = -temp_func(x)*( mu*(1+delta) + mu_p);
#endif

#if defined(INFINITEDISK) && defined(SELFGRAVITY)
	fac += sigma_func(x)*(1+mu)*(2+mu)*x*27.5;
#endif

	return fac /(2*omk_func(x)*x*x);
}
void init_globals(double ri, double ro) {
	int i;
	for(i=0;i<N;i++) {


		lr[i] = log(ri) + i*dlr;
		r[i] = exp(lr[i]);

		omega[i] = omk_func(r[i]);
		dldom[i] = dlogomk_func(r[i]);
		d2ldom[i] = d2logomk_func(r[i]);

		scaleH[i] = scaleH_func(r[i]);
		aspect_ratio[i] = scaleH[i]/r[i];

		sigma[i] = sigma_func(r[i]);
		dlds[i] = dlogsigma_func(r[i]);
		d2lds[i] = d2logsigma_func(r[i]);

		temp[i] = temp_func(r[i]);
		dldtemp[i] = dlogtemp_func(r[i]);
		d2ldtemp[i] = d2logtemp_func(r[i]);



		c2[i] = adi_gam * temp[i];
		dldc2[i] = adi_gam * dldtemp[i];
		d2ldc2[i] = adi_gam * d2ldtemp[i];

		pres[i] = sigma[i] * temp[i];
		dldpres[i] = dlds[i] + dldtemp[i];
		d2ldpres[i] = d2lds[i] + d2ldtemp[i];
		omega_prec[i] = 0;



	}

#ifdef INPUTMASS
	sigma0 = Mdisk/calc_total_disk_mass();
	for(i=0;i<N;i++) {
		sigma[i] *= sigma0;
	}
#else
	Mdisk = calc_total_disk_mass();
#endif

	}



double calc_total_disk_mass(void) {
	int i;
	double res;
	res=0;
	for(i=0;i<N;i++) {
		res += weights[i]*sigma[i]*r[i]*r[i];
	}
	return res*2*M_PI;
}

void init_weights(void) {

	int i;

#ifdef CONSTWEIGHTS
	for(i=0;i<N;i++) weights[i] = dlr;
#endif
#ifdef COMPSIMPS
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


#endif

#ifdef COMPTRAPZ

	weights[0] = .5*dlr;
	weights[N-1] = .5*dlr;

	for(i=1;i<N-1;i++) {
		weights[i] = dlr;
	}

#endif



	return;

}
