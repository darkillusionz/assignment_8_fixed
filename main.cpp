/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>

#include "mbed.h"


//Assignment 8 Threads and mutexes

//Requirements for the assignment:
//1. 10% Create a struct with three variables: A minute counter, a second counter and a mutex
//2. 05% Create an instance of the struct in the main() function
//3. 20% Create three threads. Increase the priority of the first two
//4. 20% Lock the mutex in the three threads so only one thread can access it at a time
//5. 15% Increase the minute counter once per minute in the first thread
//6. 15% Increase the second counter once per second in the second thread
//7. 15% Print the current clock value twice per second in the third thread



//Struct for sharing data between threads


struct Data {
    Mutex mutex;
    uint8_t minute = 0;
    uint8_t second = 0;
} data;


//Thread 1 Minute_counter

void minutes (Data* data){
    while(true) {

        data->mutex.lock();


        data->minute += 1;

        //tiny little reset loop here

        if (data->minute == 60){
            data->minute = 0;
        }

        data->mutex.unlock();

        ThisThread::sleep_for(60s);
    }
}



//Thread 2 Second counter

void seconds (Data* data){
    while(true){

        data->mutex.lock();

        data->second += 1;

        //tiny reset if second is too much

        if (data->second == 60){
            data->second = 0;
        }
        
        data->mutex.unlock();

        ThisThread::sleep_for(1s);
    }
}


//Thread 3 Printing out the clock values

void printer (Data* data){
    while(true){
        data->mutex.lock();

        printf("%i : %i", data->minute, data->second);
        data->mutex.unlock();

        ThisThread::sleep_for(500ms);
    }
}


int main()
{

//Initializing the struct in main

    Data data;

//Create threads

    Thread minutes_thread;
    Thread seconds_thread;
    Thread printer_thread;

//Start threads

    minutes_thread.start(callback(minutes, &data));
    seconds_thread.start(callback(seconds, &data));
    printer_thread.start(callback(printer, &data));

//setting higher priority for the minute and second counter

    minutes_thread.set_priority(osPriorityAboveNormal);
    seconds_thread.set_priority(osPriorityAboveNormal);

while(true){
    ThisThread::sleep_for(1s);
}

}
