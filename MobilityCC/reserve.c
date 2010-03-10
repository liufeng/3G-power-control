#include "cell.h"

extern Cell   *cell[10][10];

/* the amount of bandwidth reserved according to the number of connections in neighboring cells */
int amount_reservation(int count){  

    int bw;
    if(count >= 0 && count <= 5) bw = 512;
    else {
        if(count>=6 && count <= 10) bw = 1024;
        else {
            if (count >= 11 && count <=20) bw = 2048;
            else bw = 3072;
        }
    }

    return(bw);
}

/* reservation scheme for OKS98 model */
int reservationa(int i_in,int j_in ){

    int k;
    int i,j;
    int accept;
    int n_total;
    int bw;
    int ni;
    int nj;
    int n;

    n_total = 0;
    bw = 0;
    k=i=j=0;
    accept = 0;
	ni = nj = n = 0;

	for (k=0;k<6;k++) {
    	i = cell[i_in][j_in]->neighbor_i[k];  /* neighboring cell  */
	    j = cell[i_in][j_in]->neighbor_j[k];
        
        if (i != 100 && j!= 100){    /* if it is boundary cell, skip */

            for(n=0;n<6;n++){   /* count the total number of calls in neighboring cells */
                ni = cell[i][j]->neighbor_i[n];
                nj = cell[i][j]->neighbor_j[n];
                if(ni != 100 && nj != 100) n_total = n_total + cell[ni][nj]->num;
            }

			bw = amount_reservation(n_total);

			if (bw > (CAPACITY - cell[i][j]->b_used)){  
				accept = 0;
                return (accept);
            }
            else accept = 1;
        }
        n_total = 0;
    }
        /* if reservation is successful in all neighboring cells, reserve bandwidth */
        
    if (accept == 1){
        for (k=0;k<6;k++){
            i = cell[i_in][j_in]->neighbor_i[k];
            j = cell[i_in][j_in]->neighbor_j[k];
        
            if (i != 100 && j!= 100){
                for(n=0;n<6;n++){
                    ni = cell[i][j]->neighbor_i[n];
                    nj = cell[i][j]->neighbor_j[n];
                    if(ni != 100 && nj != 100) n_total = n_total + cell[ni][nj]->num;
                }
                bw = amount_reservation(n_total);
                cell[i][j]->b_reserved = bw;
            }
            n_total = 0;
        }

    }
        
    return(accept);
}

int reservationa_new(int i_in,int j_in ){

    int k;
    int i,j;
    int accept;
    int n_total;
    int bw;
    int ni;
    int nj;
    int n;
	int tmp;
    
    n_total = 0;
    bw = tmp = 0;
    k=i=j=0;
    accept = 0;
	ni = nj = n = 0;
    
	 
	for (k=0;k<6;k++) {
    	i = cell[i_in][j_in]->neighbor_i[k];  /* neighboring cell  */
	    j = cell[i_in][j_in]->neighbor_j[k];
        
        if (i != 100 && j!= 100){    /* if it is boundary cell, skip */
            n_total = cell[i][j]->num;
    		bw = amount_reservation(n_total);

			if (bw > (CAPACITY - cell[i][j]->b_used)){  
				accept = 0;
                return (accept);
            }
            else accept = 1;
		}
        n_total = 0;
	}
        /* if reservation is successful in all neighboring cells, reserve bandwidth */
        
	if (accept == 1){
        for (k=0;k<6;k++){
            i = cell[i_in][j_in]->neighbor_i[k];
            j = cell[i_in][j_in]->neighbor_j[k];
        
            n_total = cell[i][j]->num;
            bw = amount_reservation(n_total);
            tmp = CAPACITY - cell[i][j]->b_used;
            if ( tmp > bw) cell[i][j]->b_reserved = bw;
            else cell[i][j]->b_reserved = tmp;

        }
        n_total = 0;
	}
    return(accept);
}


/* reservation for the proposed scheme */
int reservationp(int i_in, int j_in, int areano){

    int k;
    int i,j;
    int accept;
    int bw;
    int n_total;
    int ni;
    int nj;
    int n;
	int tmp;

    k=i=j=0;
    bw = 0;
    accept = 0;
    n_total = 0;
    ni = nj = n = 0;
	tmp = 0;

    if(areano == 0){  /* when a call is in north direction, its neighboring cells are NW(5),N(0),NE(1) */
        i = cell[i_in][j_in]->neighbor_i[5];   /*  NW  */
        j = cell[i_in][j_in]->neighbor_j[5];

        if(i!= 100 && j != 100){
             
            n_total = cell[i][j]->num;
            bw = amount_reservation(n_total);
            if (bw > (CAPACITY - cell[i][j]->b_used)){
                accept = 0;
                return (accept);
            }
            else accept = 1;        
            n_total =0;
		}   

        for (k=0;k<=1;k++){
            i = cell[i_in][j_in]->neighbor_i[k];   /* for N and NE */
            j = cell[i_in][j_in]->neighbor_j[k];
            if(i!= 100 && j != 100){
                n_total = cell[i][j]->num;
                bw = amount_reservation(n_total);
                if (bw > (CAPACITY - cell[i][j]->b_used)){
                    accept = 0;
                    return (accept);
                }
                else accept = 1;
		    }
            n_total = 0;
        }               /* if reservation is available in all neighboring cells, reserve bw */

        if (accept ==1){
            i = cell[i_in][j_in]->neighbor_i[5];  /* NW  */
            j = cell[i_in][j_in]->neighbor_j[5];
            if(i!= 100 && j != 100){
                n_total = cell[i][j]->num;
                bw = amount_reservation(n_total);
				tmp = CAPACITY - cell[i][j]->b_used;
                if ( tmp > (bw/2)) cell[i][j]->b_reserved = (int) (bw/2);
		        else cell[i][j]->b_reserved = tmp;
            }
                
            for (k=0;k<=1;k++){
                i = cell[i_in][j_in]->neighbor_i[k];   /* N, NE */
                j = cell[i_in][j_in]->neighbor_j[k];
                if(i!= 100 && j != 100){
                    n_total = cell[i][j]->num;
                    bw = amount_reservation(n_total);
                    tmp = CAPACITY - cell[i][j]->b_used;
                    if ( tmp > bw/2) cell[i][j]->b_reserved = bw/2;
                    else cell[i][j]->b_reserved = tmp;
                }
                n_total =0;
            } /* end for */
        }  /*end if : accept = 1*/
    }  /* end if : areano = 0 */
        
    if(areano == 5){        /* if a call is Northwest, neighboring cells are SW(4), NW(5), N(0) */
        i = cell[i_in][j_in]->neighbor_i[0];   /* North */
        j = cell[i_in][j_in]->neighbor_j[0];
        if(i!= 100 && j != 100){
            n_total = cell[i][j]->num;
            bw = amount_reservation(n_total);
            if (bw > (CAPACITY - cell[i][j]->b_used)){
                accept = 0;
                return (accept);
            }
            else accept = 1;
            n_total =0;                                
        }

        for(k=4;k<=5;k++){
            i = cell[i_in][j_in]->neighbor_i[k];   /* SW, NW */
            j = cell[i_in][j_in]->neighbor_j[k];
            if(i!= 100 && j != 100){
                n_total = cell[i][j]->num;
                bw = amount_reservation(n_total);
                if (bw > (CAPACITY - cell[i][j]->b_used)){
                    accept = 0;
                    return (accept);
                }
                else accept = 1;
            }
            n_total = 0;
        }
          
        if (accept ==1){   /* if reservation is available in all neighboring cells, reserve bw */
            i = cell[i_in][j_in]->neighbor_i[0];
            j = cell[i_in][j_in]->neighbor_j[0];   /* N */
            if(i!= 100 && j != 100){
                n_total = cell[i][j]->num;
                bw = amount_reservation(n_total);
                tmp = CAPACITY - cell[i][j]->b_used;
                if ( tmp > bw/2) cell[i][j]->b_reserved = bw/2;
                else cell[i][j]->b_reserved = tmp;
            }
            n_total = 0; 
                                    
            for(k=4;k<=5;k++){           /* SW, NW */
                i = cell[i_in][j_in]->neighbor_i[k];
                j = cell[i_in][j_in]->neighbor_j[k];
                if(i!= 100 && j != 100){
                    n_total = cell[i][j]->num;
                    bw = amount_reservation(n_total);
                    tmp = CAPACITY - cell[i][j]->b_used;
                    if ( tmp > bw/2) cell[i][j]->b_reserved = bw/2;
                    else cell[i][j]->b_reserved = tmp;
                }
                n_total = 0;
            } /* end for */
        } /*end accept = 1 */
    }/* end if : areano =5*/

    if (areano >0 && areano <5){  /* when a call is NE(1),SE(2),S(3),or SW(4) */
        for(k=areano - 1;k<=areano +1;k++){   /* i.e., areano = 1, neighboring cells are NE(1),SE(2),S(3) */
            i = cell[i_in][j_in]->neighbor_i[k];
            j = cell[i_in][j_in]->neighbor_j[k];
            if(i!= 100 && j != 100){
                n_total = cell[i][j]->num;
                bw = amount_reservation(n_total);
                if (bw > CAPACITY - cell[i][j]->b_used){
                    accept = 0;
                    return (accept);  }
                else accept = 1;
            }
            n_total = 0;
        } /* end for */
  
        if (accept ==1){
            for(k=areano - 1;k<=areano +1;k++){
                i = cell[i_in][j_in]->neighbor_i[k];
                j = cell[i_in][j_in]->neighbor_j[k];
                if(i!= 100 && j != 100){
                    n_total = cell[i][j]->num;
                    bw = amount_reservation(n_total);
                    tmp = CAPACITY - cell[i][j]->b_used;
                    if ( tmp > bw/2 ) cell[i][j]->b_reserved = bw/2;
                    else cell[i][j]->b_reserved = tmp;
                }
                n_total = 0;
            } /* end for */
        }  /* end if accept */
    } /* end if areano between 1 to 4 */
    return(accept);
}
