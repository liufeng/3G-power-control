#include "cell.h"
extern Cell   *cell[10][10];

/* release bandwidth for OKS98 */
void release_a(int i_in, int j_in)
{

    int k;
    int i,j;
    int n_total;
    int bw;
    int n;
    int ni;
    int nj;
    int tmp;

    k=i=j=0;
    n_total = 0;
    bw = tmp = 0;
    n = ni = nj = 0;

    for (k=0;k<6;k++){
        i = cell[i_in][j_in]->neighbor_i[k];
        j = cell[i_in][j_in]->neighbor_j[k];

        if (i != 100 && j != 100) {
            for(n=0;n<6;n++){
                ni = cell[i][j]->neighbor_i[n];
                nj = cell[i][j]->neighbor_j[n];
                if(ni != 100 && nj != 100) n_total = n_total + cell[ni][nj]->num;
            }
                        
            bw = amount_reservation(n_total);
            tmp = CAPACITY - cell[i][j]->b_used;
            if ( tmp > bw ) cell[i][j]->b_reserved = bw;
            else cell[i][j]->b_reserved = tmp;
        }
        n_total = 0;
    }
}

void release_a_new(int i_in, int j_in)
{

    int k;
    int i,j;
    int n_total;
    int bw;
    int n;
    int tmp;

    k=i=j=0;
    n_total = 0;
    bw = tmp = 0;
    n = 0;

    for (k=0;k<6;k++){
        i = cell[i_in][j_in]->neighbor_i[k];
        j = cell[i_in][j_in]->neighbor_j[k];

        if (i != 100 && j != 100) {
            n_total = cell[i][j]->num;
            bw = amount_reservation(n_total);
            tmp = CAPACITY - cell[i][j]->b_used;
            if ( tmp > bw ) cell[i][j]->b_reserved = bw;
            else cell[i][j]->b_reserved = tmp;
        }
        n_total = 0;
    }
}

/* release bandwidth for the proposed scheme */
void release_p_new(int i_in, int j_in, int areano){

    int k;
    int i,j;
    int n_total;
    int n;
    int bw;
    int tmp;

    k=i=j=0;
    n_total = 0;
    n = 0 ;
    bw  = 0;
    tmp = 0;

    if(areano == 0){

        i = cell[i_in][j_in]->neighbor_i[5];
        j = cell[i_in][j_in]->neighbor_j[5];
        if (i != 100 && j!= 100){
            n_total = cell[i][j]->num;
            bw = amount_reservation(n_total);
            tmp = CAPACITY - cell[i][j]->b_used;
            if ( tmp > bw/2 ) cell[i][j]->b_reserved = bw/2;
            else cell[i][j]->b_reserved = tmp;
        }
        n_total = 0;

        for (k=0;k<=1;k++){
            i = cell[i_in][j_in]->neighbor_i[k];
            j = cell[i_in][j_in]->neighbor_j[k];
            if (i != 100 && j != 100){
                n_total = cell[i][j]->num;
                bw = amount_reservation(n_total);
                tmp = CAPACITY - cell[i][j]->b_used;
                if ( tmp > bw/2 ) cell[i][j]->b_reserved = bw/2;
                else cell[i][j]->b_reserved = tmp;
            }
            n_total = 0;
        }  /* end for */
    }  /* end if : areano = 0 */
        
    if(areano == 5){

        i = cell[i_in][j_in]->neighbor_i[0];
        j = cell[i_in][j_in]->neighbor_j[0];
        if (i != 100 && j != 100){
            n_total = cell[i][j]->num;
            bw = amount_reservation(n_total);
            tmp = CAPACITY - cell[i][j]->b_used;
            if ( tmp > bw/2 ) cell[i][j]->b_reserved = bw/2;
            else cell[i][j]->b_reserved = tmp;
        }
        n_total = 0;

        for (k=4;k<=5;k++){
            i = cell[i_in][j_in]->neighbor_i[k];
            j = cell[i_in][j_in]->neighbor_j[k];
            if (i != 100 && j!= 100){
                n_total = cell[i][j]->num;
                bw = amount_reservation(n_total);
                tmp = CAPACITY - cell[i][j]->b_used;
                if ( tmp > bw/2 ) cell[i][j]->b_reserved = bw/2;
                else cell[i][j]->b_reserved = tmp;
            }
            n_total = 0;
        }  /* end for */
    } /* end if areano = 5 */

    if (areano > 0 && areano <5){ 
        for (k=areano - 1;k<=areano +1;k++){
            i = cell[i_in][j_in]->neighbor_i[k];
            j = cell[i_in][j_in]->neighbor_j[k];
            if (i != 100 && j != 100){
                n_total = cell[i][j]->num;
                bw = amount_reservation(n_total);
                tmp = CAPACITY - cell[i][j]->b_used;
                if ( tmp > bw/2 ) cell[i][j]->b_reserved = bw/2;
                else cell[i][j]->b_reserved = tmp;
            }
            n_total = 0;
        } 
    }/* end of areano between 1 and 4 */
}

/* release bandwidth for the proposed scheme */
void release_p(int i_in, int j_in, int areano){

    int k;
    int i,j;
    int n_total;
    int n;
    int ni;
    int nj;
    int bw;
    int tmp;

    k=i=j=0;
    n_total = 0;
    n = ni = nj = 0 ;
    bw  = tmp = 0;

    if(areano == 0){

        i = cell[i_in][j_in]->neighbor_i[5];
        j = cell[i_in][j_in]->neighbor_j[5];
        if (i != 100 && j!= 100){
            for(n=0;n<6;n++){
                ni = cell[i][j]->neighbor_i[n];
                nj = cell[i][j]->neighbor_j[n];
                if(ni != 100 && nj != 100) 
		    n_total = n_total + cell[ni][nj]->num;
            }
            bw = amount_reservation(n_total);
            tmp = CAPACITY - cell[i][j]->b_used;
            if ( tmp > bw/2 ) cell[i][j]->b_reserved = bw/2;
            else cell[i][j]->b_reserved = tmp;
        }
        n_total = 0;

        for (k=0;k<=1;k++){
            i = cell[i_in][j_in]->neighbor_i[k];
            j = cell[i_in][j_in]->neighbor_j[k];
            if (i != 100 && j != 100){
                for(n=0;n<6;n++){
                    ni = cell[i][j]->neighbor_i[n];
                    nj = cell[i][j]->neighbor_j[n];
                    if(ni != 100 && nj != 100) 
		        n_total = n_total + cell[ni][nj]->num;
                }
                bw = amount_reservation(n_total);
                cell[i][j]->b_reserved = (int)bw/2;
            }
            n_total = 0;
        }  /* end for */
    }  /* end if : areano = 0 */
        
    if(areano == 5){

        i = cell[i_in][j_in]->neighbor_i[0];
        j = cell[i_in][j_in]->neighbor_j[0];
        if (i != 100 && j != 100){
            for(n=0;n<6;n++){
                ni = cell[i][j]->neighbor_i[n];
                nj = cell[i][j]->neighbor_j[n];
                if(ni != 100 && nj != 100) 
		    n_total = n_total + cell[ni][nj]->num;
            }
            bw = amount_reservation(n_total);
            cell[i][j]->b_reserved = (int)bw/2;
        }
        n_total = 0;

        for (k=4;k<=5;k++){
            i = cell[i_in][j_in]->neighbor_i[k];
            j = cell[i_in][j_in]->neighbor_j[k];
            if (i != 100 && j!= 100){
                for(n=0;n<6;n++){
                    ni = cell[i][j]->neighbor_i[n];
                    nj = cell[i][j]->neighbor_j[n];
                    if(ni != 100 && nj != 100) 
		        n_total = n_total + cell[ni][nj]->num;
                }
                bw = amount_reservation(n_total);
                cell[i][j]->b_reserved = (int)bw/2;
            }
            n_total = 0;
        }  /* end for */
    }
    if (areano > 0 && areano <5){ 
        for (k=areano - 1;k<=areano +1;k++){
            i = cell[i_in][j_in]->neighbor_i[k];
            j = cell[i_in][j_in]->neighbor_j[k];
            if (i != 100 && j != 100){
                for(n=0;n<6;n++){
                    ni = cell[i][j]->neighbor_i[n];
                    nj = cell[i][j]->neighbor_j[n];
                    if(ni != 100 && nj != 100) 
		        n_total = n_total + cell[ni][nj]->num;
                }
                bw = amount_reservation(n_total);
                cell[i][j]->b_reserved = (int) bw/2;
            }
            n_total = 0;
        } 
    }
}


