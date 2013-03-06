#ifndef __POLYAGAMMAALT__
#define __POLYAGAMMAALT__

#include "RNG.hpp"
#include "Matrix.h"
#include <vector>
#include <iostream>
#include <stdexcept>

// The numerical accuracy of __PI will affect your distribution.
// const double __PI = 3.141592653589793238462643383279502884197;
const double PISQ = __PI * __PI;
const double HALFPI = 0.5 * __PI;
// const double HALFPISQ = 0.5 * __PI * __PI;
// const double FOURPISQ = 4 * __PI * __PI;

const double trunc_schedule[] = { // seq(1,4,by=0.01) -> 301 entries.
0.64, 0.68, 0.72, 0.75, 0.78, 0.8, 0.83, 0.85, 0.87, 0.89, 
0.91, 0.93, 0.95, 0.96, 0.98,   1, 1.01, 1.03, 1.04, 1.06, 
1.07, 1.09,  1.1, 1.12, 1.13, 1.15, 1.16, 1.17, 1.19,  1.2,
1.21, 1.23, 1.24, 1.25, 1.26, 1.28, 1.29,  1.3, 1.32, 1.33,
1.34, 1.35, 1.36, 1.38, 1.39,  1.4, 1.41, 1.42, 1.44, 1.45, 
1.46, 1.47, 1.48,  1.5, 1.51, 1.52, 1.53, 1.54, 1.55, 1.57, 
1.58, 1.59,  1.6, 1.61, 1.62, 1.63, 1.65, 1.66, 1.67, 1.68, 
1.69,  1.7, 1.71, 1.72, 1.74, 1.75, 1.76, 1.77, 1.78, 1.79, 
1.8 , 1.81, 1.82, 1.84, 1.85, 1.86, 1.87, 1.88, 1.89, 1.9, 
1.91, 1.92, 1.93, 1.95, 1.96, 1.97, 1.98, 1.99,    2, 2.01, 
2.02, 2.03, 2.04, 2.05, 2.07, 2.08, 2.09,  2.1, 2.11, 2.12, 
2.13, 2.14, 2.15, 2.16, 2.17, 2.18, 2.19, 2.21, 2.22, 2.23, 
2.24, 2.25, 2.26, 2.27, 2.28, 2.29,  2.3, 2.31, 2.32, 2.33, 
2.35, 2.36, 2.37, 2.38, 2.39,  2.4, 2.41, 2.42, 2.43, 2.44, 
2.45, 2.46, 2.47, 2.48, 2.49, 2.51, 2.52, 2.53, 2.54, 2.55, 
2.56, 2.57, 2.58, 2.59,  2.6, 2.61, 2.62, 2.63, 2.64, 2.65, 
2.66, 2.68, 2.69,  2.7, 2.71, 2.72, 2.73, 2.74, 2.75, 2.76, 
2.77, 2.78, 2.79,  2.8, 2.81, 2.82, 2.83, 2.84, 2.85, 2.87, 
2.88, 2.89,  2.9, 2.91, 2.92, 2.93, 2.94, 2.95, 2.96, 2.97, 
2.98, 2.99,    3, 3.01, 3.02, 3.03, 3.04, 3.06, 3.07, 3.08, 
3.09,  3.1, 3.11, 3.12, 3.13, 3.14, 3.15, 3.16, 3.17, 3.18, 
3.19,  3.2, 3.21, 3.22, 3.23, 3.24, 3.25, 3.27, 3.28, 3.29, 
 3.3, 3.31, 3.32, 3.33, 3.34, 3.35, 3.36, 3.37, 3.38, 3.39, 
 3.4, 3.41, 3.42, 3.43, 3.44, 3.45, 3.46, 3.47, 3.49,  3.5, 
3.51, 3.52, 3.53, 3.54, 3.55, 3.56, 3.57, 3.58, 3.59,  3.6, 
3.61, 3.62, 3.63, 3.64, 3.65, 3.66, 3.67, 3.68, 3.69, 3.71, 
3.72, 3.73, 3.74, 3.75, 3.76, 3.77, 3.78, 3.79,  3.8, 3.81, 
3.82, 3.83, 3.84, 3.85, 3.86, 3.87, 3.88, 3.89,  3.9, 3.91, 
3.92, 3.93, 3.95, 3.96, 3.97, 3.98, 3.99,    4, 4.01, 4.02, 
4.03, 4.04, 4.05, 4.06, 4.07, 4.08, 4.09,  4.1, 4.11, 4.12, 4.13};

//------------------------------------------------------------------------------

class PolyaGammaAlt
{

 public:

  // Draw.
  double draw(double h, double z, RNG& r, int max_inner=200);
  double draw_abridged(double h, double z, RNG& r, int max_inner=200);

  // Helper.
  double a_coef(int n, double x, double h);
  double a_coef_recursive(double n, double x, double h, double coef_h, double& gamma_nh_over_n);
  double g_tilde(double x, double h, double trunc);
  double pigauss(double x, double Z, double lambda);

  double rtigauss(double h, double z, double trunc, RNG& r);
  double w_left (double trunc, double h, double z);
  double w_right(double trunc, double h, double z);

};

double ltgamma(double shape, double rate, double trunc, RNG& r);
double rtinvchi2(double h, double trunc, RNG& r);
double igauss(double mu, double lambda, RNG& r);

//------------------------------------------------------------------------------

double ltgamma(double shape, double rate, double trunc, RNG& r)
{
  double a = shape;
  double b = rate * trunc;

  if (trunc <=0) {
    fprintf(stderr, "ltgamma: trunc = %g < 0\n", trunc);
    return 0;
  }
  if (shape < 1) {
    fprintf(stderr, "ltgamma: shape = %g < 1\n", shape);
    return 0;
  }

  if (shape ==1) return r.expon_rate(1) / rate + trunc;

  double d1 = b-a;
  double d3 = a-1;
  double c0 = 0.5 * (d1 + sqrt(d1*d1 + 4 * b)) / b;
   
  double x = 0.0;
  bool accept = false;

  while (!accept) {
    x = b + r.expon_rate(1) / c0;
    double u = r.unif();
    
    double l_rho = d3 * log(x) - x * (1-c0);
    double l_M   = d3 * log(d3 / (1-c0)) - d3;

    accept = log(u) <= (l_rho - l_M);
  }

  return trunc * (x/b);
}

double rtinvchi2(double h, double trunc, RNG& r)
{
  double h2 = h * h;
  double R = trunc / h2;
  double X = 0.0;
  // I need to consider using a different truncated normal sampler.
  double E1 = r.expon_rate(1.0); double E2 = r.expon_rate(1.0);
  while ( (E1*E1) > (2 * E2 / R)) {
    // printf("E %g %g %g %g\n", E1, E2, E1*E1, 2*E2/R);
    E1 = r.expon_rate(1.0); E2 = r.expon_rate(1.0);
  }
  // printf("E %g %g \n", E1, E2);
  X = 1 + E1 * R;
  X = R / (X * X);
  X = h2 * X;
  return X;
}

double igauss(double mu, double lambda, RNG& r)
{
  // See R code for specifics.
  double mu2 = mu * mu;
  double Y = r.norm(0.0, 1.0);
  Y *= Y;
  double W = mu + 0.5 * mu2 * Y / lambda;
  double X = W - sqrt(W*W - mu2);
  if (r.unif() > mu / (mu + X)) 
    X = mu2 / X;
  return X;
}

//------------------------------------------------------------------------------

double PolyaGammaAlt::a_coef(int n, double x, double h)
{
  double d_n = 2.0 * (double) n + h;
  double log_out = h * log(2) - RNG::Gamma(h, true) + RNG::Gamma(n+h, true) \
    - RNG::Gamma(n+1, true) + log(d_n)					\
    - 0.5 * log(2 * __PI * x * x * x) - 0.5 * d_n * d_n / x;
  double out = exp(log_out);
  // double out = exp(out) is a legal command.  Weird.
  return out;
}

double PolyaGammaAlt::a_coef_recursive(double n, double x, double h, double coef_h, double& gnh_over_gn1_gh)
{
  double d_n = 2.0 * (double) n + h;
  // gamma_nh_over_n *= (n + h - 1) / n;  // Can speed up further by separate function for a0 and an, n > 0.
  if (n != 0)
    gnh_over_gn1_gh *= (n + h - 1) / n;
  else
    gnh_over_gn1_gh  = 1.0;
  double coef       = coef_h * gnh_over_gn1_gh;
  double log_kernel = - 0.5 * (log(x * x * x) + d_n * d_n / x) + log(d_n);
  return coef * exp(log_kernel);
  // double out = exp(out) is a legal command.  Weird.
}

double PolyaGammaAlt::pigauss(double x, double z, double lambda)
{
  // z = 1 / mean
  double b = sqrt(lambda / x) * (x * z - 1);
  double a = sqrt(lambda / x) * (x * z + 1) * -1.0;
  double y = RNG::p_norm(b) + exp(2 * lambda * z) * RNG::p_norm(a);
  return y;
}

double PolyaGammaAlt::w_left(double trunc, double h, double z)
{
  double out = 0;
  if (z != 0) 
    out = exp(h * (log(2) - z)) * pigauss(trunc, z/h, h*h);
  else
    out = exp(h * log(2)) * (1.0 - RNG::p_gamma_rate(1/trunc, 0.5, 0.5*h*h));
  return out;
}

double PolyaGammaAlt::w_right(double trunc, double h, double z)
{
  double lambda_z = PISQ * 0.125 + 0.5 * z * z;
  double p = exp(h * log(HALFPI / lambda_z)) * (1.0-RNG::p_gamma_rate(trunc, h, lambda_z));
  return p;
}

double PolyaGammaAlt::rtigauss(double h, double z, double trunc, RNG& r)
{
  z = fabs(z);
  double mu = h/z;
  double X = trunc + 1.0;
  if (mu > trunc) { // mu > t
    double alpha = 0.0;
    while (r.unif() > alpha) {
      X = rtinvchi2(h, trunc, r);
      alpha = exp(-0.5 * z*z * X);
    }
    // printf("rtigauss, part i: %g\n", X);
  }
  else {
    while (X > trunc) {
      X = igauss(mu, h*h, r);
    }
    // printf("rtigauss, part ii: %g\n", X);
  }
  return X;
}

double PolyaGammaAlt::g_tilde(double x, double h, double trunc)
{
  double out = 0;
  if (x > trunc) 
    out = exp(h * log(0.5 * __PI) + (h-1) * log(x) - PISQ * 0.125 * x - RNG::Gamma(h, true));
  else 
    out = h * exp( h * log(2) - 0.5 * log(2 * __PI * x * x * x) - 0.5 * h * h / x);
    // out = h * pow(2, h) * pow(2 * __PI * pow(x,3), -0.5) * exp(-0.5 * pow(h,2) / x);
  return out;
}

////////////////////////////////////////////////////////////////////////////////
				  // Sample //
////////////////////////////////////////////////////////////////////////////////

double PolyaGammaAlt::draw_abridged(double h, double z, RNG& r, int max_inner)
{
  if (h < 1 || h > 4) {
    fprintf(stderr, "PolyaGammaAlt::draw h = %g must be in [1,4]\n", h);
    return 0;
  }

  // Change the parameter.
  z = fabs(z) * 0.5;
  
  int    idx   = floor((h-1)*100);
  double trunc = trunc_schedule[idx];

  // Now sample 0.25 * J^*(1, z := z/2).
  double rate_z       = 0.125 * __PI*__PI + 0.5 * z*z;
  double weight_left  = w_left (trunc, h, z);
  double weight_right = w_right(trunc, h, z);
  double prob_right   = weight_right / (weight_right + weight_left);

  // printf("prob_right: %g\n", prob_right);
  
  double coef1_h = exp(h * log(2) - 0.5 * log(2 * __PI));
  // double gamma_nh_over_n = RNG::Gamma(h);
  double gnh_over_gn1_gh = 1.0; // Will fill in value on first call to a_coef_recursive.

  int num_trials = 0;
  int total_iter = 0;

  while (num_trials < 10000) {
    num_trials++;

    double X = 0.0;
    double Y = 0.0;

    // if (r.unif() < p/(p+q))
    double uu = r.unif();
    if ( uu < prob_right )
      X = ltgamma(h, rate_z, trunc, r);
    else
      X = rtigauss(h, z, trunc, r);

    // double S  = a_coef(0, X, h);
    double S = a_coef_recursive(0.0, X, h, coef1_h, gnh_over_gn1_gh);
    double a_n = S;
    // double a_n2 = S2;
    // printf("a_n=%g, a_n2=%g\n", a_n, a_n2);
    double gt =  g_tilde(X, h, trunc);
    Y = r.unif() * gt;

    // printf("test gt: %g\n", g_tilde(trunc * 0.1, h, trunc));
    // printf("X, Y, S, gt: %g, %g, %g, %g\n", X, Y, S, gt);

    bool decreasing = false;

    int  n  = 0;
    bool go = true;

    // Cap the number of iterations?
    while (go && n < max_inner) {
      total_iter++;

      // Break infinite loop.  Put first so it always checks n==0.
      #ifdef USE_R
      if (n % 1000 == 0) R_CheckUserInterrupt();
      #endif

      ++n;
      double prev = a_n;
      // a_n  = a_coef(n, X, h);
      a_n = a_coef_recursive((double)n, X, h, coef1_h, gnh_over_gn1_gh);
      // printf("a_n=%g, a_n2=%g\n", a_n, a_n2);
      decreasing = a_n <= prev;

      if (n%2==1) {
	S = S - a_n;
	if ( Y<=S && decreasing) return 0.25 * X;
      }
      else {
	S = S + a_n;
	if ( Y>S && decreasing) go = false;
      }

    }
    // Need Y <= S in event that Y = S, e.g. when X = 0.

  }
  
  // We should never get here.
  return -1.0;
} // draw

double PolyaGammaAlt::draw(double h, double z, RNG& r, int max_inner)
{
  if (h < 1) {
    fprintf(stderr, "PolyaGammaAlt::draw h = %g must be >= 1\n", h);
    return 0;
  }

  double n = floor( (h-1.0) / 4.0 );
  double remain = h - 4.0 * n;

  double x = 0.0;

  for (int i = 0; i < (int)n; i++) 
    x += draw_abridged(4.0, z, r);
  if (remain > 4.0)
    x += draw_abridged(0.5 * remain, z, r) + draw_abridged(0.5 * remain, z, r);
  else
    x += draw_abridged(remain, z, r);

  return x;
}

////////////////////////////////////////////////////////////////////////////////
			       // END OF CLASS //
////////////////////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////////////////////
				 // APPENDIX //
////////////////////////////////////////////////////////////////////////////////

// We should only have to calculate Gamma(h) once.  We can then get Gamma(n+h)
// from the recursion Gamma(z+1) = z Gamma(z).  Not sure how that is in terms of
// stability, but that should save us quite a few computations.  This affects
// a_coef and g_tilde.
