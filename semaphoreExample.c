//Steven Seiden 2022
//Compile using gcc semaphoreExample.c -lpthread -lrt

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

sem_t semaphore;
char data[512];


void readData(){
	printf("\n\n📖 Reading data.\n");
	printf("Current data: %s\n",data);

}

void writeData(int procNum){

	//This ensures that only one process can modify data at a time for mutual exclusion
	printf("\n\n⌛ Process %d waiting to write.\n",procNum);
	sem_wait(&semaphore);

	printf("⬀ Data opened by process %d!\n",procNum);
	printf("\n\033[31;5m✐ PROCESS %d WRITING DATA\033[0m\n",procNum);
	
	//Simulating writing large data
	char buffer[50];
	sprintf(buffer," + data from proc %d",procNum);
	strcat(data,buffer);
	sleep(4);

	//Signaling to the semaphore to let the next waiting process in
	printf("\n\033[32;5m✓ PROCESS %d FINISHED WRITING DATA\033[0m\n",procNum);
	sem_post(&semaphore);


	printf("⬂ Data closed by process %d!\n\n",procNum);
}

int main() {

	strcat(data,"Initial data");

	printf("Initiating the program\n");
	
	//Creating a semaphore
	sem_init(&semaphore, 0, 1);
	pthread_t threadOne, threadTwo;

	//Reading the data before modification
	readData();


	//Creating two threads that will execute writeData() in parallel
	//Whichever process gets to the semaphore first will write data first
	pthread_create(&threadOne, NULL, writeData, 1);
	pthread_create(&threadTwo, NULL, writeData, 2);
	pthread_join(threadOne, NULL);
	pthread_join(threadTwo, NULL);

	//Reading the data after modification
	readData();

	sem_destroy(&semaphore);

	return 0;

}