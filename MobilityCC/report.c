#include "cell.h"

extern double s_clock;
extern Cell *cell[10][10];

Data final;

void ReportGenerator()
{
    int i, j;
    
        /* Report generator */
    printf("simulation termination time = %10.7g\n\n", s_clock);
  
    printf("***************************************************\n");
    printf("* observation in cell [%d %d]\n", CELL_I, CELL_J);
    printf("***************************************************\n");
  
    printf("number of new calls = %d\n", cell[CELL_I][CELL_J]->nn);
    printf("number of new calls blocked = %d\n", cell[CELL_I][CELL_J]->ndrop);
    printf("call blocking prob = %-2g.\n\n",
           (double)cell[CELL_I][CELL_J]->ndrop/cell[CELL_I][CELL_J]->nn);
  
    printf("number of handoff calls = %d\n", cell[CELL_I][CELL_J]->nh);
    printf("number of handoff droped = %d\n", cell[CELL_I][CELL_J]->hdrop);
    printf("handoff dropping prob = %-2g.\n\n",
           (double)cell[CELL_I][CELL_J]->hdrop/cell[CELL_I][CELL_J]->nh);
  
    printf("availbale bandwidth = %d\n", CAPACITY -
           cell[CELL_I][CELL_J]->b_used - cell[CELL_I][CELL_J]->b_reserved);
    printf("reserved bandwidth = %d\n", cell[CELL_I][CELL_J]->b_reserved);
    printf("used bandwidth = %d\n", cell[CELL_I][CELL_J]->b_used);
    printf("Percentage of BW used at termination = %-2g.\n", 
           1.0*cell[CELL_I][CELL_J]->b_used/CAPACITY);
    printf("bandwidth utilization = %-2g.\n\n",
           (1.0 / s_clock) * (cell[CELL_I][CELL_J]->sa/CAPACITY));
  
    printf("remaining calls in cell = %d\n\n", cell[CELL_I][CELL_J]->num);
  
  
    printf("***************************************************\n");
    printf("*     aggregated performance over all 100 cells   *\n");
    printf("***************************************************\n");
    for(i=0;i<NUM_CELL;i++){
        for(j=0;j<NUM_CELL;j++){            
            final.num = final.num + cell[i][j]->num;
            final.b_reserved = final.b_reserved + cell[i][j]->b_reserved;
            final.b_used = final.b_used + cell[i][j]->b_used;
            final.nn = final.nn + cell[i][j]->nn;
            final.nh = final.nh + cell[i][j]->nh;
            final.ndrop = final.ndrop + cell[i][j]->ndrop;
            final.hdrop = final.hdrop  + cell[i][j]->hdrop;
            final.sa = final.sa + (cell[i][j]->sa / s_clock) / CAPACITY;
        }
    }
  
    final.time = s_clock;
  
    if (final.nn == 0) final.cbp = 0.0;
    else final.cbp = (double) final.ndrop/ (double)final.nn;
  
    if(final.nh == 0) final.cdp = 0.0;
    else final.cdp = (double)final.hdrop / (double)final.nh;
  
    final.util = final.sa / (NUM_CELL * NUM_CELL);
  

    printf("number of new calls = %d\n", final.nn);
    printf("number of new calls blocked = %d\n", final.ndrop);
    printf("call blocking prob = %-2g.\n\n",
           (double)final.ndrop/final.nn);
  
    printf("number of handoff calls = %d\n", final.nh);
    printf("number of handoff droped = %d\n", final.hdrop);
    printf("handoff dropping prob = %-2g.\n\n",
           (double)final.hdrop/final.nh);
  
    printf("availbale bandwidth = %d\n", 100*CAPACITY -
           final.b_used-final.b_reserved);
    printf("reserved bandwidth = %d\n", final.b_reserved);
    printf("used bandwidth = %d\n", final.b_used);
    printf("Percentage of BW used at termination = %-2g.\n", 
           1.0*final.b_used/(100*CAPACITY));
        // printf("sa = %-2g., sa/CAPACITY = %-2g.\n", final.sa,
        // 1.0 * final.sa / CAPACITY);
    printf("bandwidth utilization = %-2g.\n\n", final.util);
  
    printf("remaining calls in cell = %d\n\n", final.num);

}
