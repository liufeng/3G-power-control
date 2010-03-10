#include <math.h>
#include "cell.h"

extern Cell *cell[10][10];

Nextcell* choose_next_cell(int i,int j,int areano)
{
    Nextcell* next;
    double u;
    int a;
    int area;

    next = (Nextcell *)malloc(sizeof(Nextcell));

    u = (double)drand48();   /* straight Pr0.5, left and right 0.15 Bleft and BRight 0.075 BS 0.05 */
    if (u < 0.5) {                             
        next->next_i = cell[i][j]->neighbor_i[areano];  /* neighboring cell  */
        next->next_j = cell[i][j]->neighbor_j[areano];
	    next->direction = areano;                       /* moving direction */
    }
    else if (u >= 0.5 && u < 0.65) {
        if(areano > 0 ) {
            a = areano -1;
            next->next_i = cell[i][j]->neighbor_i[a];  /* areano between 1 and 5 */
            next->next_j = cell[i][j]->neighbor_j[a];
            next->direction = a;

        }
        else {
            next->next_i = cell[i][j]->neighbor_i[5];  /* areano = 0  */
            next->next_j = cell[i][j]->neighbor_j[5];
            next->direction = 5;         
        }

    }
    else if (u <= 0.65 && u > 0.80) {
        if(areano < 5 ) {
            a = areano + 1;
            next->next_i = cell[i][j]->neighbor_i[a];  /* areano between 0 and 4 */
            next->next_j = cell[i][j]->neighbor_j[a];
            next->direction = a;
	    }
        else {
            next->next_i = cell[i][j]->neighbor_i[0];  /* areano = 0  */
            next->next_j = cell[i][j]->neighbor_j[0];
            next->direction = 0;
        }

    }
    else if ( u <= 0.80 && u > 0.875){
        if(areano < 4 ) {
            a = areano - 2;
            area = abs(a);
            next->next_i = cell[i][j]->neighbor_i[area];  /* areano between 0 and 4 */
            next->next_j = cell[i][j]->neighbor_j[area];
            next->direction = area;
        }
        else {
            a = areano - 4;
            area = abs(a);
            next->next_i = cell[i][j]->neighbor_i[area];  /* areano = 4 and 5  */
            next->next_j = cell[i][j]->neighbor_j[area];
            next->direction = area;
        }
    }
    else if (u <=0.875 && u < 0.95) {
        if(areano < 2 ) {
            a = areano - 4;
            area = abs(a);
            next->next_i = cell[i][j]->neighbor_i[area];  /* areano = 0 and 1 */
            next->next_j = cell[i][j]->neighbor_j[area];
            next->direction = area;
        }
        else {
            a = areano - 2;
            area = abs(a);
            next->next_i = cell[i][j]->neighbor_i[area];  /* areano between 2 and 5  */
            next->next_j = cell[i][j]->neighbor_j[area];
            next->direction = area;

        }
    }
    else {
        a = areano - 3;
        area = abs(a);
        next->next_i = cell[i][j]->neighbor_i[area];  /* neighboring cell  */
        next->next_j = cell[i][j]->neighbor_j[area];
        next->direction = area;
    }
        
    return(next);
}

/* get the index of next cell. The direction was determined in the first handoff and fixed*/
Nextcell* move_to_next_cell(int i,int j,int direction){
    
    Nextcell* next;
    next = (Nextcell *)malloc(sizeof(Nextcell));
                                     
    next->next_i = cell[i][j]->neighbor_i[direction];  /* neighboring cell  */
    next->next_j = cell[i][j]->neighbor_j[direction];
	next->direction = direction;
     
    return(next);
}

