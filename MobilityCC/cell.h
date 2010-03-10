#ifndef __cell_h
#define __cell_h
/*
 * Definitions for the customer service simulation.
 * Written by Jason Neudorf on January 27, 1997
 * Based on code by David Evans -- March 7th, 1996
 */

/* input parameters */
#define ARRIVAL_MEAN    100   /* Mean interarrival time between calls*/
#define MAX_SPEED       120   /* 60km/h */
#define MIN_SPEED       80    /* 0 km/h */
#define DURATION        120   /* mean of call duration */

#define TIME_INTERVAL   50    /* monitoring time /second */
#define CAPACITY        30000 /* capacity of a cell : 30 Mbps */
#define MIN_B           100  /* minimum size of bandwidth of multimedia*/
#define MAX_B           300  /* maximum size of bandwidth of multimedia*/
#define NUM_CELL        10    /* number of cells: 10 * 10 -> 100 cells */
#define SCHEME          1     /* 0 for OKS98, 1 for proposed scheme   */
#define PRH             0.5   /* probability of handoff calls 0.25/0.5/0.75 */ 
#define CELL_SIZE       1     /* the diameter of a cell(m) : 1km  */

#define L               37000 /* transient interval */
#define CELL_I          4  /* i index of a local cell observed */
#define CELL_J          4  /* j index of a local cell observed */

typedef struct customer_t {
    struct customer_t *next;
    double arrival_time;
    int type;
    int areano;
    int bw;
    int i;
    int j;
    int h_count;
    int duration;
    double speed;
    int direction;

} Customer;

typedef struct cell_info {
    int neighbor_i[6];
    int neighbor_j[6];
    int b_reserved;  /* amount of bandwidth reserved */
    int b_used;
    int i_index;
    int j_index;
    int num;        /* number of connections in current cell */
    int nn;         /* number of new calls arrivals */
    int nh;         /* number of handoff calls arrivals  */
    int hdrop;
    int ndrop;
    double sa;
    int status;
} Cell;

typedef struct nextcell_info {
    int next_i;
    int next_j;
    int direction;
} Nextcell;

typedef struct print_info {
    double time;
    int b_reserved;  /* amount of bandwidth reserved */
    int b_used;
    int num;        /* number of connections */
    int nn;         /* number of new calls */
    int nh;         /* number of handoff calls */
    int hdrop;
    int ndrop;
    double cdp;
    double cbp;
    double sa;
    double util;

} Data; 

#endif
