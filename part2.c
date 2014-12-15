/*
 * find the final mass and radius for white dwarf stars with central densities ranging from 0.08 to 1000000
 */
#include <stdio.h>
#include <math.h>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>

int dwarf_eqs (double r, const double y[], double f[], void *params);

int main (void)
{
    double rho_min = 0.08; /* lower limit for central density */
    double rho_max = 1000000.; /* upper limit for central density */
    int np = 100; /* number of points to calculate */
    double factor = exp(log(rho_max/rho_min)/(np - 1)); /* the factor to get a geometric progression */
    double rho_c = rho_min/factor; /* divide by the factor first to start at rho_min in while loop */
    
    int i;
    for (i = 0; i < np; i++)
    {
        rho_c *= factor;                        /* central density */
        size_t neqs = 2;                        /* number of equations */
        double eps_abs = 1.e-8, eps_rel = 0.;   /* desired precision */
        double stepsize = 1e-6;                 /* initial integration step */
        double r = 0, r1 = 100.;                /* radius */
        int status;
        /*
         * Initial conditions
         */
        double y[2] = { 0, rho_c };

        /*
         * Explicit embedded Runge-Kutta-Fehlberg (4,5) method.
         * This method is a good general-purpose integrator.
         */
        gsl_odeiv2_step *s = gsl_odeiv2_step_alloc (gsl_odeiv2_step_rkf45, neqs);
        gsl_odeiv2_control *c = gsl_odeiv2_control_y_new (eps_abs,
                                eps_rel);
        gsl_odeiv2_evolve *e = gsl_odeiv2_evolve_alloc (neqs);

        gsl_odeiv2_system sys = { dwarf_eqs, NULL, neqs, &rho_c };

        /*
         * Evolution loop
         */
        while ((r < r1) && (y[1] > 0))
        {
            status = gsl_odeiv2_evolve_apply (e, c, s, &sys, &r, r1, &stepsize, y);
            if (status != GSL_SUCCESS)
            {
                printf ("Troubles: % .5e  % .5e  % .5e\n", r, y[0], y[1]);
                break;
            }
        }
        printf ("% .5e  % .5e  % .5e  % .5e\n", r, y[0], y[1], rho_c);

        gsl_odeiv2_evolve_free (e);
        gsl_odeiv2_control_free (c);
        gsl_odeiv2_step_free (s);
    }
    return 0;
}

