#include "eigen.h"

const double rdecay = 4;
const double decay_exp = 2;
double sigma_func(double x) {
	return sigma0 * pow(x, sigma_index) * exp(-pow(x/rdecay,decay_exp));
}

double dlogsigma_func(double x) {
	return sigma_index - decay_exp * pow(x/rdecay,decay_exp);
}

double d2logsigma_func(double x) {
	return -decay_exp*decay_exp*pow(x/rdecay,decay_exp);
}


double temp_func(double x) {
	return h0*h0*pow(x,temp_index);
}

double dlogtemp_func(double x) {
	return temp_index;
}

double d2logtemp_func(double x) {
	return 0;
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
	return h0*x*pow(x,flare_index);
}

int analytic_potential(void) {
	return 0;
}

double omega_prec_grav_analytic(double x) {
	return 0;
}
