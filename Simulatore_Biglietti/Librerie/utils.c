#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rngs.h"
#include "struct.h"
#include "utils.h"
#include "rvms.h"

double Exponential(double m){
    return (-m * log(1.0 - Random()));
}

double Uniform(double a, double b){
    return (a + (b - a) * Random());
}

double getSpecificMin(double currentMin, multiserver *mserver, int serverDim){
    double min=currentMin;
    for(int i=0; i<serverDim; i++){
        if(mserver[i].service<min)  min = mserver[i].service;
    }
    return min;
}

int NextEvent(multiserver event[], int length){
    int e = 0;                                      
    int i = 1;
    while (i < length) { 
        if ((event[i].occupied == 1) && (event[i].service < event[e].service))
            e = i;
        i++; 
    }
    if(event[e].occupied==0){
        return -1;
    }
    return (e);
}

int FindOne(multiserver mserver[]){
    int index = 0;
    while (mserver[index].occupied == 1)      
        index++;                      
    return (index);
}

double getTimeService(nodeParam service){
    SelectStream(service.stream);
    return Exponential(service.mean);
}

double getTimeServiceTrunc(nodeParam service){
    SelectStream(service.stream);
    return idfTruncatedNormal(12, 2, 7, 15, Random());
} 

double getTimeServiceTruncP(nodeParam service){
    SelectStream(service.stream);
    return idfTruncatedNormal(4, 1, 2, 5, Random());
} 

loginCode assignLoginCode(){
    SelectStream(8);
    double x = Uniform(0,100);
    if (x<4.07)
        return codeJ1987;
    if (x<16.27)
        return codeStadium;
    if (x<40.66)
        return codeCard;
    if (x<81.31)
        return codeFree;
    if (x<86.15)
        return codeWomen;
    else                                         
        return codeAway;
}

paymentCode assignWomenPaymentCode(){
    SelectStream(9);
    double x = Uniform(0,100);
    if (x<95.00)
        return codePayWomen;
    else   
        return nothing;
}

paymentCode assignHomePaymentCode(){
    SelectStream(10);
    double x = Uniform(0,100);
    if (x<90.0)
        return codePayHome;
    else   
        return nothing;
}

paymentCode assignAwayPaymentCode(){
    SelectStream(11);
    double x = Uniform(0,100);
    if (x<70.0)
        return codePayAway;
    else   
        return nothing;
}

void initOutputStats(nodeData *node, int dim)
{
    node->node=0.0;
    node->queue=0.0;
    node->service=0.0;
    node->index=0;
    node->init_time=0.0;
    node->serverNumber=dim;
}

void writeStats(output out[],nodeData nodo,int i){ 
    out[i].job=nodo.index;
    out[i].wait=(nodo.node / nodo.index);
    out[i].delay=(nodo.queue / nodo.index);
    out[i].service=(nodo.service / nodo.index);
    out[i].numberNode=(nodo.node / (nodo.current-nodo.init_time));
    out[i].numberQueue=(nodo.queue / (nodo.current-nodo.init_time));
    out[i].utilization=((nodo.service/nodo.serverNumber) / (nodo.current-nodo.init_time));
}


void statsUpdater(nodeData *node, int num, double current, double next){
    node->node  += (next - current) * num;
    if(num>node->serverNumber)
        node->service += (next - current)*node->serverNumber;
    else
        node->service += (next - current)*num;
    node->current=current;
    if(num>node->serverNumber-1)
        node->queue   += (next - current) * (num - node->serverNumber);
}

void priorityStatsUpdater(nodeData *node, int num, int servers, double current, double next){
    node->node += (next - current) * num;
    node->service += (next - current)*servers;
    node->current=current;
    if(num>servers-1)
        node->queue += (next - current) * (num - servers);
}

void initServerData(multiserver *server, int dim){
    for(int i=0; i<dim; i++){
        server[i].occupied=0;
        server[i].service=INF;
        server[i].loginCode=none;
    }
}
void initTime(nodeData *node, double time){
    node->init_time=time;
}

void modifyServerData(multiserver *server, double service, int occupied){
    server->service=service;
    server->occupied=occupied;
}

void modifyServerDataHome(multiserver *server, double service, int occupied, loginCode loginCode){
    modifyServerData(server, service, occupied);
    server->loginCode=loginCode;
}

int condizioneBatchMeans(int v[7],int n){
    for (int i = 0; i < 7; i++)
    {
        if(v[i]<n) return 1;
    }
return 0;
}