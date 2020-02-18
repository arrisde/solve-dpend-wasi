/* solve_dpend.c
 *
 * Example code to solve double pendulum ODEs using fourth order 
 * Runge-Kutta. 
 *
 * Parameters are passed in at the command line:
 * 
 * $./solve_dpend TMIN TMAX TH10 W10 TH20 W20 NSTEP > pendulum.txt 
 *
 * where TMIN and TMAX are the starting and ending times (in seconds), 
 * TH10 and TH20 are the initial angles (degrees), and W10 and W20 
 * are the initial angular velocities (degrees per second), and 
 * NSTEP is the number of integrations steps. This example illustrates 
 * using redirection to write the results to file in a file
 * pendulum.txt. Note that there is no checking for accuracy, so the
 * user needs to choose a suitable NSTEP. Also angles written to file
 * are in radians.
 *
 * As an example, the data for the first animated gif on the web page 
 * may be generated with   
 *
 * $./solve_dpend 0.0 10.0 90.0 0.00 -10.0 0.0 1000 > outfile.txt
 *
 * (only every fifth frame is used in the animation).
 * 
 * `, 2004
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// #include <time.h>
#define WASM_EXPORT __attribute__((used)) __attribute__((visibility("default")))

/* hardwired parameters */

#define PI 3.14159265
#define N 4    /* number of equations to be solved */
#define G 9.8  /* acc'n due to gravity, in m/s^2 */
#define L1 1.0 /* length of pendulum 1 in m */
#define L2 1.0 /* length of pendulum 2 in m */
#define M1 1.0 /* mass of pendulum 1 in kg */
#define M2 1.0 /* mass of pendulum 2 in kg */

void runge_kutta(float xin, float yin[], float yout[], float h);
void derivs(float xin, float yin[], float dydx[]);
void run(float TMIN, float TMAX, float TH10, float W10, float TH20, float W20, int NSTEP);

int main(int argc, char *argv[])
{
    int NSTEP;
    float TMIN, TMAX, TH10, W10, TH20, W20;

    /* obtain command line values (hardcoded for simplicity) */

    // TMIN = atof(argv[1]);
    // TMAX = atof(argv[2]);
    // TH10 = atof(argv[3]);
    // W10 = atof(argv[4]);
    // TH20 = atof(argv[5]);
    // W20 = atof(argv[6]);
    // NSTEP = atoi(argv[7]);

    TMIN = 0.0;
    TMAX = 10.0;
    TH10 = 90.0;
    W10 = 0.0;
    TH20 = -10.0;
    W20 = 0.0;
    NSTEP = 3000001;

    /* Currently, time.h does not work in node, so we measure execution time using 'time' bash builtin */
    // clock_t begin = clock();
    run(TMIN, TMAX, TH10, W10, TH20, W20, NSTEP);
    // clock_t end = clock();
    // float time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
    
    // printf("Done in %f s\n", time_spent);

    return 0;
}


WASM_EXPORT
void run(float TMIN, float TMAX, float TH10, float W10, float TH20, float W20, int NSTEP)
{
    int i = 0;
    float h;
    float yin[N], yout[N];
    float *t, *th1, *th2, *w1, *w2;

    /* allocate memory for arrays of values of time, angles 1 and 2,
     and angular velocities 1 and 2 respectively */

    t = (float *)malloc(NSTEP * sizeof(float));
    th1 = (float *)malloc(NSTEP * sizeof(float));
    w1 = (float *)malloc(NSTEP * sizeof(float));
    th2 = (float *)malloc(NSTEP * sizeof(float));
    w2 = (float *)malloc(NSTEP * sizeof(float));

    /* stepsize for integration */

    h = (TMAX - TMIN) / (NSTEP - 1.0);

    /* Define array of t values */

    for (i = 0; i < NSTEP; i++)
        t[i] = TMIN + h * i;

    /* initial values - convert all angles to radians */

    th1[0] = TH10 * PI / 180.0;
    w1[0] = W10 * PI / 180.0;
    th2[0] = TH20 * PI / 180.0;
    w2[0] = W20 * PI / 180.0;

    /* perform the integration */

    printf("%f %f %f %f %f\n", t[0], th1[0], w1[0], th2[0], w2[0]);
    for (i = 0; i < NSTEP - 1; i++)
    {
        yin[0] = th1[i];
        yin[1] = w1[i];
        yin[2] = th2[i];
        yin[3] = w2[i];
        runge_kutta(t[i], yin, yout, h);
        th1[i + 1] = yout[0];
        w1[i + 1] = yout[1];
        th2[i + 1] = yout[2];
        w2[i + 1] = yout[3];

        if (t[i+1] == roundf(t[i+1]))
            printf("%f %f %f %f %f\n", t[i + 1], th1[i + 1], w1[i + 1], th2[i + 1],
               w2[i + 1]);
    }
}


void derivs(float xin, float yin[], float dydx[])
{

    /* function to fill array of derivatives dydx at xin */

    float den1, den2, del;

    dydx[0] = yin[1];

    del = yin[2] - yin[0];
    den1 = (M1 + M2) * L1 - M2 * L1 * cos(del) * cos(del);
    dydx[1] = (M2 * L1 * yin[1] * yin[1] * sin(del) * cos(del) + M2 * G * sin(yin[2]) * cos(del) + M2 * L2 * yin[3] * yin[3] * sin(del) - (M1 + M2) * G * sin(yin[0])) / den1;

    dydx[2] = yin[3];

    den2 = (L2 / L1) * den1;
    dydx[3] = (-M2 * L2 * yin[3] * yin[3] * sin(del) * cos(del) + (M1 + M2) * G * sin(yin[0]) * cos(del) - (M1 + M2) * L1 * yin[1] * yin[1] * sin(del) - (M1 + M2) * G * sin(yin[2])) / den2;

    return;
}


void runge_kutta(float xin, float yin[], float yout[], float h)
{
    /* fourth order Runge-Kutta - see e.g. Numerical Recipes */

    int i;
    float hh, xh, dydx[N], dydxt[N], yt[N], k1[N], k2[N], k3[N], k4[N];

    hh = 0.5 * h;
    xh = xin + hh;

    derivs(xin, yin, dydx); /* first step */
    for (i = 0; i < N; i++)
    {
        k1[i] = h * dydx[i];
        yt[i] = yin[i] + 0.5 * k1[i];
    }

    derivs(xh, yt, dydxt); /* second step */
    for (i = 0; i < N; i++)
    {
        k2[i] = h * dydxt[i];
        yt[i] = yin[i] + 0.5 * k2[i];
    }

    derivs(xh, yt, dydxt); /* third step */
    for (i = 0; i < N; i++)
    {
        k3[i] = h * dydxt[i];
        yt[i] = yin[i] + k3[i];
    }

    derivs(xin + h, yt, dydxt); /* fourth step */
    for (i = 0; i < N; i++)
    {
        k4[i] = h * dydxt[i];
        yout[i] = yin[i] + k1[i] / 6. + k2[i] / 3. + k3[i] / 3. + k4[i] / 6.;
    }

    return;
}
