#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EVENTS 100

typedef struct {
    int priority;
    char lane[20];
    char action[20];
    int time;
} TrafficEvent;

int compareEvents(const void *a, const void *b) {
    return ((TrafficEvent *)a)->priority - ((TrafficEvent *)b)->priority;
}

int main() {
    FILE *file;
    TrafficEvent events[MAX_EVENTS];
    int event_count = 0;
    int total_time = 0;
    int main_lane_time = 0, diversion_lane_time = 0;
    int is_main_lane_active = 1;

    file = fopen("traffic.dat", "r");
    if (file != NULL) {

	    while (fscanf(file, "%d %s %s %d", 
	                  &events[event_count].priority, 
	                  events[event_count].lane, 
	                  events[event_count].action, 
	                  &events[event_count].time) == 4) {
	        event_count++;
	    }
	    fclose(file);
	
	    qsort(events, event_count, sizeof(TrafficEvent), compareEvents);
	
	    int i;
	    for (i = 0; i < event_count; i++) {
	        if (events[i].priority == 3 && 
	            strcmp(events[i].lane, "Main") == 0 && 
	            strcmp(events[i].action, "Pedestrian") == 0) {
	            break;
	        }
	
	        if (is_main_lane_active && strcmp(events[i].lane, "Main") == 0) {
	            main_lane_time += events[i].time;
	        } else if (!is_main_lane_active && strcmp(events[i].lane, "Diversion") == 0) {
	            diversion_lane_time += events[i].time;
	        }
	
	        if (i < event_count - 1 && strcmp(events[i].lane, "Main") == 0 && strcmp(events[i+1].lane, "Diversion") == 0) {
	            is_main_lane_active = 0;
	        } else if (i < event_count - 1 && strcmp(events[i].lane, "Diversion") == 0 && strcmp(events[i+1].lane, "Main") == 0) {
	            is_main_lane_active = 1;
	        }
	    }
	
	    total_time = main_lane_time + diversion_lane_time;
	    printf("Total time before a main lane pedestrian can cross: %d seconds\n", total_time);
	
	    return 0;
	}
}

