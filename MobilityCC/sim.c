
#include <stdio.h>
#include <stdlib.h>
#include "sim.h"

void insert_event(Event *event, Event **elist)
{
    Event *epointer, *prevpointer;

    if(*elist) {
        epointer = *elist;
        prevpointer = (Event *) 0;

        while(epointer) {
            if(epointer->time > event->time)
                break;

            prevpointer = epointer;
            epointer = epointer->next;
        }

        event->next = epointer;
        if(epointer == *elist || prevpointer == (Event *) 0)
            *elist = event;
        else {
            prevpointer->next = event;
        }
    } else {
        event->next = (Event *) 0;
        *elist = event;
    }
}

/*
 * Grab the first event from the event list.  The memory containing 
 * the event must be freed at an appropriate time.
 */
Event* extract_event(Event **elist)
{
    Event *next_event;

    assert(elist);
    next_event = *elist;
    if(*elist)
        *elist = (*elist)->next;

    return(next_event);
}

Event* new_event(double time, EType eventtype, int i, int j, int bw, int areano, 
                                  double duration, double speed, int h_count, int direction) {
    Event* retval;

    retval = (Event *)malloc(sizeof(Event));
    assert(retval);  /* die if malloc fails */    
    retval->time = time;
    retval->type = eventtype;
    retval->bw = bw;
    retval->i_index = i;
    retval->j_index = j;
    retval->areano = areano;
    retval->duration = duration;
    retval->speed = speed;
    retval->h_count = h_count;
	retval->direction = direction;
    return retval; 
}




