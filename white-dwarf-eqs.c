#include <math.h>

#include <gsl/gsl_errno.h>

int dwarf_eqs (double r, const double y[], double f[], void *params)
{
    double rho_c = *(double *) params;
    
    double m = y[0];
    double rho = y[1];
    
    if (rho > 0)
    {
        double p = pow(rho, 2./3);
        double gamma = p/(3*sqrt(1. + p));
        f[0] = rho*r*r;
        if (r > 1.e-6)
        {
            f[1] = -m*rho/(gamma*r*r);
        }
        else
        {
            f[1] = -rho_c/3.*r*rho/gamma;
        }
    }
    else
    {
        f[0] = 0.;
        f[1] = 0.;
    }
    
    return GSL_SUCCESS;
}
