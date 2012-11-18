#include <stdio.h>
#include <queue>
#include <stdlib.h>

#include <time.h>

using namespace std;

const int MAX_TIME = 120;
const int MAX_SEAT = 20;
const int BUSY_TIME_LEN = 5;
const int MAKER_NUM = 4;
const int STOP_ORDERING_TIME = 15; // minutes
const int HAPPY_WAIT_TIME = 15;
const int OVEN_NUM = 3;
const int OVEN_VOLUME = 15;
const int MAX_BAKEN_TIME = 12;
const int MAKE_TIME = 2;

int seat_taken = 0;

int coming_clients[MAX_TIME+1];


class Pizza{
public:
	int id;
	int baken_time;
	int maker;
	int make_time;
	Pizza(int _id):id(_id), baken_time(0), make_time(0){};
};

//enum SeatStatus{'O', 'M', 'H', 'A', 'E'};
class Seat{
public:
	char status;
	int wait_time;
	int eating_time;
	int max_eating_time; 
	Seat(){reset();}
	void reset(){wait_time=0; status='O'; eating_time=0;max_eating_time = rand()%31+10;}
};

deque<Pizza*> pizza_baking;
deque<Pizza*> pizza_making;
deque<Pizza*> pizza_serving;
deque<Pizza*> pizza_ordering;

char servant_status[MAX_TIME+1];
char maker_status[MAKER_NUM][MAX_TIME+1];
char seat_status[MAX_SEAT][MAX_TIME+1];
int oven_status[MAX_TIME+1];

Seat seats[MAX_SEAT];

void initialize(){
	for(int i=0; i<=MAX_TIME; i++) servant_status[i] = 'I';
	for(int i=0; i<MAKER_NUM; i++) 
		for(int j=0; j<=MAX_TIME; j++)
			maker_status[i][j] = 'I';
	for(int i=0; i<MAX_SEAT; i++){
		for(int j=0; j<=MAX_TIME; j++){
			seat_status[i][j] = 'O';
		}
	}
	for(int i=0; i<=MAX_TIME; i++) oven_status[i] = 0;
	
	coming_clients[0] = 0;
    for(int i=1; i<MAX_TIME; i++) coming_clients[i] = rand()%3;
    // check busy time restriction
    for(int i=1; i<MAX_TIME-BUSY_TIME_LEN+1; i++){
        int not_empty_time = -1;
        for(int j=i; j<i+BUSY_TIME_LEN; j++){
            if(coming_clients[j] != 0) not_empty_time = j;
        }
        if(not_empty_time != -1) i = not_empty_time+1;
        else{
            int add_client_time = rand()%5 + i;
            coming_clients[add_client_time] = rand()%2+1;
        }
    }

    for(int i=1; i<=MAX_TIME; i++){
        printf("%d", coming_clients[i]);
        if(i%5 == 0) printf(" ");
    }
    printf("\n");
}

int total_pizza_served = 0;
int total_profit = 0;
int come_leave_number = 0;

double happy_time; 
double angry_time;
double happiness_rate = 0.0;

#define start_time clock_gettime(CLOCK_MONOTONIC, &start);
#define end_time clock_gettime(CLOCK_MONOTONIC, &finish);

struct timespec start, finish;

int main(){
	initialize();


	start_time
    for(int time = 0; time <= MAX_TIME; time++){
		if(time != 0) oven_status[time] = oven_status[time-1];

		// iterate ordering queue, all orders taken at one time
		if(servant_status[time] == 'I'){
			while(!pizza_ordering.empty()){
//				if(MAX_TIME - time > STOP_ORDERING_TIME){
				seats[pizza_ordering.front()->id].status='H';
				pizza_making.push_back(pizza_ordering.front());
				pizza_ordering.pop_front();
//				}
			}
		}

		// update new comers
		if(coming_clients[time] != 0 && seat_taken != MAX_SEAT && MAX_TIME - time > STOP_ORDERING_TIME){
			for(int j=0; j<coming_clients[time]; j++){
//				for(int i=0; i<MAX_SEAT; i++)
//					printf("%c ", seats[i].status);
				if(seat_taken == MAX_SEAT){
				
					// critical
						come_leave_number++;
					continue;
				}
				int first_empty_seat=-1;
	//			printf("seat_taken: %d\n", seat_taken);
				for(int i=0; i<MAX_SEAT; i++){
					if(seats[i].status == 'O'){
						first_empty_seat = i;
						break;
					}
				}
				
//				if(first_empty_seat == -1) continue;
				seat_taken++;
	//			printf("After Added, seat_taken: %d\n", seat_taken);
				seats[first_empty_seat].status = 'M';
				
				servant_status[time] = 'T';
				
				Pizza* newpizza = new Pizza(first_empty_seat);
				
				// critical
				total_pizza_served++;
				
				pizza_ordering.push_back(newpizza);
			}
		}
		
		// update makers
		for(int i=0; i<MAKER_NUM; i++){
			if(time == 0) break;
			else if(maker_status[i][time-1] == 'S'){
				maker_status[i][time] = 'T';
			}else if(maker_status[i][time-1] == 'T'){
				maker_status[i][time]='I';
			}
		}

		// pizza from making to baking
		while(!pizza_making.empty()){
			if(pizza_making.front()->make_time >= 2){			// the pizza has been made
	//			printf("From making to baking, seats_taken: %d.\n", seat_taken);
				pizza_baking.push_back(pizza_making.front());
				pizza_making.pop_front();
				oven_status[time] ++;
			}else{
				break;
			}
		}
		
		int tmp_maker_count =0;
		for(deque<Pizza*>::iterator iter = pizza_making.begin(); iter != pizza_making.end(); iter++){
	//		printf("hello, maker #%d making No.%d pizza, pizza_making length: %d.\n", tmp_maker_count, (*iter)->id, pizza_making.size());
			tmp_maker_count ++;
			if((*iter)->make_time == 0){
				int available_maker = -1;
				for(int i=0; i<MAKER_NUM; i++) {
					if(maker_status[i][time] == 'I'){
						available_maker = i;
						break;
					}
				}
				if(available_maker == -1){
					break;
				}
				maker_status[available_maker][time]='S';
				(*iter)->make_time ++;
			}else if((*iter)->make_time == 1){
				(*iter)->make_time ++;
			}
			// or it will wait until being baken
		}		
		
		// pizza_baking queue
		int tmp_c = 0;
		for(deque<Pizza*>::iterator iter = pizza_baking.begin(); iter != pizza_baking.end(); iter++){
			(*iter)->baken_time ++ ;
			tmp_c ++;
	//		printf("%d baken_time: %d\n", tmp_c, (*iter)->baken_time);
		}
		while(!pizza_baking.empty()){
			if(pizza_baking.front()->baken_time >= MAX_BAKEN_TIME){
	//			printf("pizza_baking size: %d\n", pizza_baking.size());
	//			printf("from baking to serving, table: %d\n", pizza_baking.front()->id);
				pizza_serving.push_back(pizza_baking.front());
				pizza_baking.pop_front();
				oven_status[time] --;
			}
			else break;
		}
		
		// update eating time and check leaving
		#pragma omp parallel for private(i) shared(seat_taken) num_threads(4)
		for(int i=0; i<MAX_SEAT; i++){
			if(seats[i].status == 'E'){
				if(seats[i].eating_time >= seats[i].max_eating_time){
					seats[i].reset();
					#pragma omp critical
					seat_taken --;
				}
				else seats[i].eating_time ++;
			}
		}
		
		// update waiting 
		#pragma omp parallel for private(i) shared(angry_time, happy_time) num_threads(4)
		for(int i=0; i<MAX_SEAT; i++){
			if(seats[i].status == 'H' || seats[i].status == 'A'){
				seats[i].wait_time ++;
				if(seats[i].wait_time > HAPPY_WAIT_TIME){
					seats[i].status = 'A';
					
					// critical
					#pragma omp critical
					angry_time ++ ;
				}
				else{
					seats[i].status = 'H';
					
					// critical
					#pragma omp critical
					happy_time ++;
				}
			}
		}
	

		// iterate serving queue, all serving taken at one time
		if(servant_status[time] == 'I'){
			while(!pizza_serving.empty()){
				seats[pizza_serving.front()->id].status='E';
				pizza_serving.pop_front();
				servant_status[time]='S';
			}
		}
		

		for(int i=0; i<MAX_SEAT; i++){
			seat_status[i][time] = seats[i].status;
		}
	}
	end_time
	
	// print out results
	printf("Execution time:\t\t\t\t %lf\n", finish.tv_sec - start.tv_sec + (double)(finish.tv_nsec-start.tv_nsec)/1000000000.0);
	
	printf("Total number of pizza served: \t\t %d\n", total_pizza_served);
	printf("Total profit of for the day: \t\t %d\n", total_pizza_served*8-1000);
	printf("Number of customers came and leave: \t %d\n", come_leave_number);
	
	happiness_rate = happy_time / (happy_time + angry_time);
	printf("Rate of customer happiness: \t\t %lf\n", happiness_rate);
	
	for(int time=1; time<=MAX_TIME; time++){

		int hour = (time-1)/60 + 18;
		int minute = (time-1)%60+1;
		if(minute == 60){ hour++; minute = 0;}

		// time
		printf("%.2d:%.2d ", hour, minute);

		// status of makers
		printf("(");
		for(int i=0; i<MAKER_NUM; i++)
			printf("%c", maker_status[i][time]);
		printf(") ");

		// status of servant
		printf("(%c) ", servant_status[time]);

		// status of seats
		printf("(");
		for(int i=0; i<MAX_SEAT; i++){
			printf("%c", seat_status[i][time]);
			if((i+1)%5==0 && i!=MAX_SEAT-1) printf(" ");
		}
		printf(") ");

		// status of oven
		printf("(");
		int tmp=oven_status[time];
	//	printf("tmp: %d\n", tmp);
		int tmp_oven_cnt=0;
		while(tmp_oven_cnt < OVEN_NUM){
			int tmp_cnt = 0;
			if(tmp>=0){
				if(tmp >= OVEN_VOLUME) tmp_cnt = OVEN_VOLUME;
				else tmp_cnt = tmp;
				tmp = tmp-tmp_cnt;
			}
			if(tmp_oven_cnt != OVEN_NUM-1) printf("%d,", tmp_cnt);
			else printf("%d", tmp_cnt);
			tmp_oven_cnt ++;
		}
		printf(");\n");
	}
    
    return 0;
}
