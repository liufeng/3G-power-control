#ifdef NDEBUG 
#define assert(FOO) {if (!(FOO)) fprintf(stderr, "prog failed at line %d in file " __FILE__ ".\n", __LINE__), exit(-1);}
#else
#define assert(x) {}
#endif 

/*
 * Event types needed for the customer model
 */
typedef enum etype {C_ARRIVAL,
                    C_DEPARTURE, PRINT, RESET} EType;

/*
 * Get user-supplied event types
 */

typedef struct event {
        struct event *next;
        EType type;  /* What was it? */
        double time; /* When does this event happen? */
        int i_index;
        int j_index;
        int bw;
        int areano;
        double duration;
        double speed;
        int h_count;
		int direction;
} Event;

typedef unsigned char bool_t;

void insert_event(Event *event, Event **elist);
Event *extract_event(Event **elist);
Event* new_event(double time, EType eventtype, int i, int j, int bw, int areano, 
				 double duration, double speed, int h_count, int direction);




