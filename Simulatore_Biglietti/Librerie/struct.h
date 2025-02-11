#ifndef struct_h
#define struct_h 
#include <string.h>

typedef struct nodeParam{
    double mean;
    int stream;
} nodeParam;

typedef struct nodeData{
    double node;                    
    double queue;                   
    double service;                
    int index;                     
    double current;
    double init_time;
    int serverNumber;
} nodeData;
 
 typedef struct output{
    char nome[128];
    double wait;
    double delay;
    double service;
    double numberNode;
    double numberQueue;
    double utilization;
    double job;
}output;

typedef struct event{     
    double clientArrival;
    int loginCompletion;
    int homeCompletion; 
    int womenCompletion;
    int awayCompletion;
    int womenPayCompletion;
    int homePayCompletion;
    int awayPayCompletion;
    double current;
    double next;
    double last;
} event;

typedef enum loginCode{
    codeWomen,
    codeJ1987,
    codeStadium,
    codeCard,
    codeFree,
    codeAway,
    none
} loginCode;

typedef enum paymentCode{
    codePayWomen,
    codePayHome,
    codePayAway,
    nothing
} paymentCode;

typedef struct multiserver{
    double service;
    int occupied;
    loginCode loginCode;
} multiserver;

 typedef struct paramService{
    double mean;
    int stream;
}paramService;

#endif