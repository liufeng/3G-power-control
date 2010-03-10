#ifndef __variate_h
#define __variate_h

double expo(double mean); 
int find_cell(int num_cell);
int area_no(void);
int call_type(double prh);   // bernoulli trial
int call_class(void);
int bandwidth(int a, int b);  // discrete uniform [a..b]
double call_speed(int a, int b);

#endif
