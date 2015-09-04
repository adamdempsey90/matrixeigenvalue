#include "eigen.h"

//#define ANALYTICPOTENTIAL

//#define SIGMALOGPROF

const double sigmin = 1e-5;


void calc_sig_params(const double x, double *xw, double *xm, double *s, int *flag) {

	if ( (x > ro) || (x < ri) ) {
		*flag = 1;
		*xw = 0;
		*s = 0;
	}
	else {
		*flag = 0;
#ifdef SIGMALOGPROF
		*xw = log10(x);
		*xm = .5*(log10(ri*ro));
		*s = log10(ro/ri)*log10(ro/ri);
#else
		*xw = x;
		*xm = .5*(ri+ro);
		*s = (ro-ri)*(ro-ri);
#endif
		*s *= .25/(sigma0 - sigmin);

	}
	return;
}

double sigma_func(double x) {
	double xw,xm, s, ans;
	int flag;

	calc_sig_params(x,&xw,&xm,&s,&flag);

	if (flag == 0) {
		ans = sigma0 - (xw - xm)*(xw-xm)/s;
	}
	else {
		ans = sigmin;
	}
	return ans;
}

double dlogsigma_func(double x) {
	double xw,xm, s, ans;
	int flag;

	calc_sig_params(x,&xw,&xm,&s,&flag);

	if (flag == 0) {
		ans = 2 * xw *(xw-xm)/((xw-xm)*(xw-xm) - s*sigma0);
	}
	else {
		ans = 0;
	}
	return ans;

}

double d2logsigma_func(double x) {
	double xw,xm, s, ans,fac,fac2;
	int flag;

	calc_sig_params(x,&xw,&xm,&s,&flag);

	if (flag == 0) {
		fac = (xw-xm)*(xw-xm);
		fac2 = (fac-s*sigma0)*(fac-s*sigma0);
		ans = -2*xw*fac*xm + 2*xw*(-2*xw+xm)*s*sigma0 / fac2;
	}
	else {
		ans = 0;
	}
	return ans;
}


double temp_func(double x) {
	double res;
#ifdef POLYTROPE
	res = flare_index*pow(sigma_func(x),flare_index-1);
//	printf("%.2e\t%.2e\n",sigma_func(x),res);
#else
	res = h0*h0*pow(x,temp_index);
#endif
	return res;
}

double dlogtemp_func(double x) {
	double res;
#ifdef POLYTROPE
	res = (flare_index - 1)*dlogsigma_func(x);
#else
	res = temp_index;
#endif
	return res;
}

double d2logtemp_func(double x) {
	double res;
#ifdef POLYTROPE
	res = (flare_index - 1)*d2logsigma_func(x);
#else
	res = 0;
#endif
	return res;
}

double omk_func(double x) {
	return pow(x,-1.5);
}

double dlogomk_func(double x) {
	return -1.5;
}

double d2logomk_func(double x) {
	return 0;
}

double scaleH_func(double x) {
	double res;
#ifdef POLYTROPE
	res = sqrt(flare_index)*pow(sigma_func(x),.5*(flare_index-1))*pow(omk_func(x),-1);
#else
	res =  h0*x*pow(x,flare_index);
#endif
	return res;
}


int analytic_potential(void) {
	return 0;
}

double omega_prec_grav_analytic(double x) {
	return 0;
}
