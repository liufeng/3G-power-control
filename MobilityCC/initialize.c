#include <stdio.h>
#include "cell.h"
#include "sim.h"
#include "variate.h"

extern Cell *cell[10][10];
extern double s_clock;     /* time of current event                 */
extern double last_event;
extern Event  *eventlist; /* list of events */
//extern Data *data[2000];

void Initialize()
{
    int i,j,k,ii,jj,i_index,j_index;    
    int bw;
    int traffic_class;
    int h_count;
    int direction;
    double tt;
    
    /* Initialization */
    s_clock = last_event = 0.0; 
    eventlist = 0;
    
    bw = traffic_class = h_count = direction = 0;
    
    i = j = k = 0;
    ii = jj = 0;
    i_index = j_index = 0;
    
    for (i=0;i<NUM_CELL;i++){   /* initialization of cells */
        for(j=0;j<NUM_CELL;j++){

                // cell[i][j] = (Cell *)malloc(sizeof(Cell));

            cell[i][j]->b_reserved = 0;
            cell[i][j]->b_used = 0;

            cell[i][j]->nn = 0;
            cell[i][j]->nh = 0;
            cell[i][j]->hdrop = 0;
            cell[i][j]->ndrop = 0;
                        
            cell[i][j]->sa = 0;
            cell[i][j]->num = 0;
            cell[i][j]->status = 0;

            if(i==0){        /* for boundary cells, 
                                set 100 as a null neighbor ( no neighbor in that direction) */
                cell[i][j]->neighbor_i[0] = 100;       /* North     = 0  */
                cell[i][j]->neighbor_j[0] = 100;}      /* Northeast = 1  */
            else{                                      /* Southeast = 2  */                             
                cell[i][j]->neighbor_i[0] = i-1;       /* South     = 3  */
                cell[i][j]->neighbor_j[0] = j;         /* Southwest = 4  */
            }                                          /* Nowthwest = 5  */

            if(i==9){
                cell[i][j]->neighbor_i[3] = 100;
                cell[i][j]->neighbor_j[3] = 100;}
            else{
                cell[i][j]->neighbor_i[3] = i+1;
                cell[i][j]->neighbor_j[3] = j;
            }
                
            if(j % 2 == 0){                            /* when the column is even */
                                
                if (j==0){  /* no neighbor in SW and NW for left boundary cells*/           
                    cell[i][j]->neighbor_i[4] = 100; 
                    cell[i][j]->neighbor_j[4] = 100;
                    cell[i][j]->neighbor_i[5] = 100;
                    cell[i][j]->neighbor_j[5] = 100;
                }
                else{ 
                    cell[i][j]->neighbor_i[4] = i;
                    cell[i][j]->neighbor_j[4] = j-1;
                    cell[i][j]->neighbor_i[5] = i-1;
                    cell[i][j]->neighbor_j[5] = j-1;
                }

                cell[i][j]->neighbor_i[1] = i-1;
                cell[i][j]->neighbor_j[1] = j+1;
                cell[i][j]->neighbor_i[2] = i;
                cell[i][j]->neighbor_j[2] = j+1;

                if (i==0){                             /* no neighbor in NW and NE for top */
                    cell[i][j]->neighbor_i[5] = 100;
                    cell[i][j]->neighbor_j[5] = 100;
                    cell[i][j]->neighbor_i[1] = 100;
                    cell[i][j]->neighbor_j[1] = 100;
                }
            }
            else{
                if(j==9){                              /* no neighbor in NE and SE for right */
                    cell[i][j]->neighbor_i[1] = 100;
                    cell[i][j]->neighbor_j[1] = 100;
                    cell[i][j]->neighbor_i[2] = 100;
                    cell[i][j]->neighbor_j[2] = 100;
                }
                else{
                    cell[i][j]->neighbor_i[1] = i;
                    cell[i][j]->neighbor_j[1] = j+1;
                    cell[i][j]->neighbor_i[2] = i+1;
                    cell[i][j]->neighbor_j[2] = j+1;
                }

                if(i==9){                            /* no neighbor in SE and SW  for bottom */
                    cell[i][j]->neighbor_i[2] = 100;
                    cell[i][j]->neighbor_j[2] = 100;
                    cell[i][j]->neighbor_i[4] = 100;
                    cell[i][j]->neighbor_j[4] = 100;
                }
                else{
                    cell[i][j]->neighbor_i[4] = i+1;
                    cell[i][j]->neighbor_j[4] = j-1;
                }

                cell[i][j]->neighbor_i[5] = i;
                cell[i][j]->neighbor_j[5] = j-1;
            }

        }
    }

/*    for (i=0;i<2000;i++){  
        data[i] = (Data *)malloc(sizeof(Data));
        data[i]->time  = 0;        
        data[i]->num = 0;
        data[i]->b_reserved = 0;
        data[i]->b_used = 0;
        data[i]->nn =0;
        data[i]->nh =0;
        data[i]->hdrop = 0;
        data[i]->ndrop =0;
        data[i]->sa = 0.0;
        data[i]->cbp = 0.0;
        data[i]->cdp = 0.0;
        data[i]->util = 0.0;
    } */
    
    i_index = find_cell(NUM_CELL);  /* determine a local cell */
    j_index = find_cell(NUM_CELL);
   
    traffic_class = call_class();         /* determine traffic type */
    if (traffic_class == 1) bw = 30;  /* for voice service, B = 30 Kbps */
    else{
        if(traffic_class ==2) bw = 256;  /* for video phone, B = 256 Kbps */
        else bw = bandwidth(MIN_B, MAX_B); /* for multimedia */
    } 

    tt = expo(ARRIVAL_MEAN);
    printf("tt = %f\n", tt);

    insert_event(new_event(tt, C_ARRIVAL, i_index, j_index, bw, area_no(), 
                           call_speed(MIN_SPEED, MAX_SPEED), expo(DURATION), h_count, direction), &eventlist);

/*    insert_event(new_event(s_clock+TIME_INTERVAL, PRINT, i_index, j_index, bw, areano, 
      speed, duration, h_count, direction), &eventlist); */

    fprintf(stderr, "Initialization complete.\n");

    /* end of initialization */

}
