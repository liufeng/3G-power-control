#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
#include <math.h>
#include <time.h>
#include "sim.h"
#include "cell.h"
#include "variate.h"

#ifdef WIN32
  #define srand48(s) srand(s)
  #define drand48() (((double)rand())/((double)RAND_MAX))
#endif

//Data   *data[2000];
Cell   *cell[10][10];
double s_clock;     /* time of current event                 */
double last_event;
Event  *eventlist; /* list of events */
FILE   *out;

// int bw;
// int areano;
// int p;
// double speed;
// double duration;
// int h_count;
//int direction;

void Initialize();
Nextcell* choose_next_cell(int i,int j,int areano);
Nextcell* move_to_next_cell(int i,int j,int direction);
int amount_reservation(int count);
int reservationa(int i_in,int j_in );
int reservationa_new(int i_in,int j_in );
int reservationp(int i_in, int j_in, int areano);    

/*    
void txtprint()    file output  
{  
    int i;
    i =0;

    out = fopen("d:\\network\\output.txt","a");

    fprintf(out,"\n\t clock\t cdp\t cbp\t util\n");
    for (i=0;i<2000;i++){
        fprintf(out,"%12.5g\t %12.8g\t %12.8g\t %12.8g\n",
		data[i]->time,data[i]->cdp,
                data[i]->cbp,data[i]->util);
    }
    fclose(out);
} */

int main(int argc, char **argv)
{
    Event *tmpevent;  /* temporary event */
    Customer *tmpcus;
    Nextcell *next_cell;

    int    accept;
    double cell_time;
    double next_speed;
    int    traffic_class;
    int    direction;
    int    ii, jj, i, j, i_index, j_index;
    time_t t1;
    
    accept = traffic_class = direction = 0;
    cell_time = next_speed = 0.0;
    ii = jj = i = j = i_index = j_index = 0;
    
    //(void) time(&t1);
    //srand48((long) t1);
    srand48(45984395);
    
    for (i=0;i<NUM_CELL;i++){   /* initialization of cells */
        for(j=0;j<NUM_CELL;j++){
            cell[i][j] = (Cell *)malloc(sizeof(Cell));
        }
    }

    Initialize();
    
        /* Main loop*/
    while(s_clock < 4000.) {
        assert(eventlist); /* there must always be an event */
        tmpevent = extract_event(&eventlist); 
        assert(tmpevent->time >= s_clock); /* never go back in time */
                
        s_clock = tmpevent->time;
	//printf ("s_clock = %12.5g\n", s_clock);
        
        for (ii = 0; ii < NUM_CELL; ii++)
            for (jj = 0; jj < NUM_CELL; jj++)
                cell[ii][jj]->sa += (s_clock - last_event) * cell[ii][jj]->b_used;
    
        last_event = s_clock;
    
        tmpcus = (Customer *)malloc(sizeof(Customer));
    
        i = tmpevent->i_index;  
        j = tmpevent->j_index;
        
        switch(tmpevent->type) {
            case C_ARRIVAL: 
                    //fprintf(stderr, "Arrival 1 : time = %10.7g.\n", s_clock);

		next_cell = 0;

                i_index = find_cell(NUM_CELL);  /* determine a local cell */
                j_index = find_cell(NUM_CELL);
                next_speed = call_speed(MIN_SPEED, MAX_SPEED);
             
                traffic_class = call_class();         /* determine traffic type */
                if (traffic_class == 1) tmpcus->bw = 30;  /* for voice service, B = 30 Kbps */
                else{
                    if(traffic_class ==2) tmpcus->bw = 256;  /* for video phone, B = 256 Kbps */
                    else tmpcus->bw = bandwidth(MIN_B, MAX_B); /* for multimedia */
                } 
			 
                insert_event(new_event(s_clock+expo(ARRIVAL_MEAN), C_ARRIVAL, i_index, j_index, tmpcus->bw,
                                       area_no(), expo(DURATION),next_speed,0, direction), &eventlist);

                if (tmpevent->speed != 0) cell_time = CELL_SIZE*1.0*60*60/tmpevent->speed; /*multiply by 360sec*/       
                else cell_time = 100000;    /* when speed = 0, this call doesn't handoff */

                if (tmpevent->h_count == 0)                                 
                        // tmpcus->type = call_type(PRH);    /* determine new or handoff call*/
                    tmpcus->type = 0; /* new call  */
                else { 
                    tmpcus->type = 1;  /* hand_off call */
                }

                if (tmpcus->type ==1) cell[i][j]->nh = cell[i][j]->nh +1; /* increase number of handoff calls */
                else cell[i][j]->nn = cell[i][j]->nn +1;                  /* increase number of new calls  */
             
                if (CAPACITY == cell[i][j]->b_used) cell[i][j]->status = 1;
                else cell[i][j]->status = 0;
             
           
             
                if (cell[i][j]->status == 0) {
               
                    if (tmpcus->type == 0){  /* new call */
                        if (tmpcus->bw <= (CAPACITY - cell[i][j]->b_used - cell[i][j]->b_reserved)){
                                /* accept only if Bn <= Bavailable  */
                   
                                /* check neighboring cells  and reserve bandwidth in neighboring cells */
                            if(SCHEME == 0) accept = reservationa_new(i,j);      /* for OKS98 */
                            else accept = reservationp(i,j,tmpevent->areano);  /* for proposed scheme */
                   
                            if (accept == 1){  /* reservation is successful in all neighboring cels */
                                cell[i][j]->b_used = cell[i][j]->b_used + tmpcus->bw; /* increase B_used */
                                cell[i][j]->num = cell[i][j]->num +1;  /* increase # of calls in local cell */
                     
                                if (cell_time < tmpevent->duration){
                                    tmpevent->duration = tmpevent->duration - cell_time;
                                    next_cell = choose_next_cell(i,j,tmpevent->areano); /* If the i,i of next cell = 100, */ 
						                           
                                    if (next_cell->next_i !=100 && next_cell->next_j !=100){
                                        tmpevent->h_count = tmpevent->h_count +1;
                                        insert_event(new_event(s_clock + cell_time, C_ARRIVAL,next_cell->next_i, 
                                                               next_cell->next_j,tmpcus->bw, area_no(), tmpevent->duration,
                                                               tmpevent->speed,(tmpevent->h_count +1),next_cell->direction), 
                                                     &eventlist); }  
						 
                                    else {insert_event(new_event(s_clock + tmpevent->duration, C_DEPARTURE, i, j, 
                                                                 tmpcus->bw, tmpevent->areano, tmpevent->duration, 
                                                                 tmpevent->speed, tmpevent->h_count,tmpevent->direction), 
                                                       &eventlist); }/*If there is no neighbor in the direction, */
                                }                                   /* schedule a departure event after call duration */
                                else {  /* If cell residence time is longer than call duration, schedule departure */
                                    insert_event(new_event(s_clock + tmpevent->duration, C_DEPARTURE,i,j,tmpcus->bw, 
                                                           tmpevent->areano, tmpevent->duration,tmpevent->speed,
                                                           tmpevent->h_count,tmpevent->direction), &eventlist); 
                                }
                            }
                            else cell[i][j]->ndrop = cell[i][j]->ndrop +1;  
                                /* increase # of new calls dropped */
                        }
                        else cell[i][j]->ndrop = cell[i][j]->ndrop +1;
                            /* increase # of new calls dropped */
                    }         
                    else {  /* handoff call  */
                        if (tmpevent->bw <= (CAPACITY - cell[i][j]->b_used)) {
                   
                            if(SCHEME == 0) accept = reservationa_new(i,j);
                            else accept = reservationp(i,j,tmpevent->areano);
                   
                            if (accept == 1){
                                    /*if available bandwidth is not enough, allocate reserved bandwidth */        
                                    /*   Bh  <= B_reserved + B available and Bh >= B_available   */
                                if(tmpevent->bw  >= (CAPACITY - cell[i][j]->b_reserved - cell[i][j]->b_used)) {
                                    if((CAPACITY - cell[i][j]->b_used - tmpevent->bw) > 0) 
                                        cell[i][j]->b_reserved = CAPACITY - cell[i][j]->b_used - tmpevent->bw; 
                                    else cell[i][j]->b_reserved = 0; 
                                }

                                cell[i][j]->b_used = cell[i][j]->b_used + tmpevent->bw; 
                                cell[i][j]->num = cell[i][j]->num +1;   
                     
                                if (cell_time < tmpevent->duration) {
                                    tmpevent->duration = tmpevent->duration - cell_time;
                                        //next_cell = choose_next_cell(i,j,tmpevent->areano);
                                    next_cell = move_to_next_cell(i,j,tmpevent->direction);

                                    if (next_cell->next_i !=100 && next_cell->next_j !=100) {
                                        tmpevent->h_count  = tmpevent->h_count +1;

                                        insert_event(new_event(s_clock + cell_time, C_ARRIVAL, next_cell->next_i, 
                                                               next_cell->next_j, tmpevent->bw,area_no(), tmpevent->duration, 
                                                               tmpevent->speed, tmpevent->h_count,tmpevent->direction), 
                                                     &eventlist); }
                                    else{
                                        insert_event(new_event(s_clock + cell_time, C_DEPARTURE, i, j, tmpevent->bw, 
                                                               tmpevent->areano, tmpevent->duration, tmpevent->speed, 
                                                               tmpevent->h_count,tmpevent->direction), &eventlist); }
                                }
                                else {
                                    insert_event(new_event(s_clock + cell_time, C_DEPARTURE, i, j, tmpevent->bw, 
                                                           tmpevent->areano, tmpevent->duration, tmpevent->speed, 
                                                           tmpevent->h_count,tmpevent->direction), &eventlist); 
                       
                                }
                            }
                            else cell[i][j]->hdrop = cell[i][j]->hdrop + 1;
                                /* increase # of handoff calls dropped  */
                        }
                        else cell[i][j]->hdrop = cell[i][j]->hdrop + 1;
                    }
                }
                else {
                    if (tmpcus->type ==1) cell[i][j]->hdrop = cell[i][j]->hdrop +1;
                    else cell[i][j]->ndrop = cell[i][j]->ndrop +1;
                }        
             
                accept = 0;

                if (!next_cell) free(next_cell);
             
                break;
             
            case C_DEPARTURE:
                    //fprintf(stderr, "Departure: time = %10.7g.\n", s_clock);
                cell[i][j]->status = 0;  

                if ((cell[i][j]->b_used - tmpevent->bw) > 0) 
                    cell[i][j]->b_used = cell[i][j]->b_used - tmpevent->bw;
                else cell[i][j]->b_used = 0; 

                cell[i][j]->num = cell[i][j]->num -1;
         
                                /* release reserved bandwidth in neighboring cells  */
                if (SCHEME==0) release_a_new(i,j);
                else release_p_new(i,j,tmpevent->areano);
         
                break;
            case RESET:
         
                    /*     for (i=0;i<NUM_CELL;i++){
                    for(j=0;j<NUM_CELL;j++){
                        cell[i][j]->hdrop = 0;
                        cell[i][j]->ndrop = 0;
                        cell[i][j]->nn = 0;
                        cell[i][j]->nh = 0;
                    }
                }
                                   
                for(p=0;p<2000;p++){
                    data[p]->time = 0.0;
                    data[p]->num = 0;
                    data[p]->b_reserved = 0;
                    data[p]->b_used = 0;
                    data[p]->nn = 0;
                    data[p]->nh = 0;
                    data[p]->ndrop = 0;
                    data[p]->hdrop = 0;
                    data[p]->cdp = 0.0;
                    data[p]->cbp = 0.0;
                    data[p]->util = 0.0;
                }
                                   
                p = 0;
                i = 0;
                j = 0;    */
         
                break;
            case PRINT:  /*
                    
                if(p < 2000) {
                    for(i=0;i<NUM_CELL;i++){
                        for(j=0;j<NUM_CELL;j++){
                            data[p]->num = data[p]->num + cell[i][j]->num;
                            data[p]->b_reserved = data[p]->b_reserved + cell[i][j]->b_reserved;
                            data[p]->b_used = data[p]->b_used + cell[i][j]->b_used;
                            data[p]->nn = data[p]->nn + cell[i][j]->nn;
                            data[p]->nh = data[p]->nh + cell[i][j]->nh;
                            data[p]->ndrop = data[p]->ndrop + cell[i][j]->ndrop;
                            data[p]->hdrop = data[p]->hdrop  + cell[i][j]->hdrop;
                        }
                    }
           
                    data[p]->time = s_clock;
           
                    if (data[p]-> nn == 0) data[p]->cbp = 0.0;
                    else data[p]->cbp = (double) data[p]->ndrop/ (double)data[p]->nn;
           
                    if(data[p]->nh == 0) data[p]->cdp = 0.0;
                    else data[p]->cdp = (double)data[p]->hdrop / (double)data[p]->nh;
           
                    data[p]->util = (double)data[p]->b_used /(double) (CAPACITY * 100);
           
                    insert_event(new_event(s_clock+TIME_INTERVAL, PRINT, i_index, j_index, 
                                           bw, areano, speed, duration, h_count,direction), 
                                 &eventlist);

                    p = p+1;
           
                    } */
         
                break;

        } /* end switch */

        free(tmpevent);
        free(tmpcus);
    }

    ReportGenerator();
    
//txtprint();   /* file output */
  
        /* clean up mess */
    while (eventlist) free(extract_event(&eventlist));
    printf("Cleared out remaining events.\n");
  
    for (i=0; i<NUM_CELL; i++)
        for (j=0; j<NUM_CELL; j++)
            free(cell[i][j]);
  
    return 0;
}
