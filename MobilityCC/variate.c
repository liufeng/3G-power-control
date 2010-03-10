#include <stdlib.h>
#include <math.h>
#include "sim.h"

#ifdef WIN32
  #define srand48(s) srand(s)
  #define drand48() (((double)rand())/((double)RAND_MAX))
#endif

double expo(double mean)   /* exponential duration */
{
    double u, num;
	 
    u = (double)drand48();
    num = -1.0 * mean * log(u);
    assert(num > 0);

    return(num);
}


int find_cell(int num_cell)
{
    double u;
        int index;
    u = (double)drand48();
    index = (int)(u * num_cell);

    if (index == 10) index = 0;

    assert(index >= 0);

    return(index);
}

int area_no(void)
{
    double u;
    int area;
    u = (double)drand48();
    area = (int)(u * 6);

    if (area == 6) area = 0;

    assert(area >= 0);

    return(area);
}


int call_type(double prh)   // bernoulli trial
{
    double u;
    int type;
    u = (double)drand48();

    if (u < prh) type = 1;  /* prh : hanoff probability*/
    else type = 0;          /* new connection  */

    assert(type >= 0);
    return(type);
}

int call_class(void)
{
    double u;
    int type;
    u = (double)drand48();
  
//  if (u <= 0.33) type = 1;       /* voice service */
        //else if (u <= 0.66) type  = 2; /* video phone */
        //else type = 3;             /* interact multimedia & video on Demand */
  
    if (u < 0.5) type = 1;       /* voice service */
    else type  = 2; /* video phone */
  
    assert(type > 0);
    return(type);
}

int bandwidth(int a, int b)   // discrete uniform [a..b]
{
    double u;
    int bw;
    u = (double)drand48();
    bw = (int) (a + floor((b - a + 1) * u));
  
    return(bw);
}

double call_speed(int a, int b){

    double u;
    double speed;
    
	u = (double)drand48();
    speed = (a + floor((b - a + 1) * u));
	return(speed);
}

