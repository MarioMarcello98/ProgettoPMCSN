#ifndef utils_h
#define utils_h

#define START 0.0
#define STOP 1440
#define INF pow(1.79769, 308.0) //massimo double rappresentabile
#define numBatch 64
#define numJobInBatch 1024
#define N 64
#define ALFA 0.05

double Exponential(double m);
double Uniform(double a, double b);
double getSpecificMin(double currentMin, multiserver *mserver, int serverDim);
int NextEvent(multiserver event[], int length);
int FindOne(multiserver mserver[]);
double getTimeService(nodeParam service);
double getTimeServiceTrunc(nodeParam service);
double getTimeServiceTruncP(nodeParam service);
loginCode assignLoginCode();
paymentCode assignWomenPaymentCode();
paymentCode assignHomePaymentCode();
paymentCode assignAwayPaymentCode();
void initOutputStats(nodeData *node, int dim);
void writeStats(output out[],nodeData appoggio,int i);
void statsUpdater(nodeData *node, int num, double current, double next);
void priorityStatsUpdater(nodeData *node, int num, int servers, double current, double next);
void initServerData(multiserver *server, int dim);
void modifyServerData(multiserver *server, double service, int occupied);
void modifyServerDataHome(multiserver *server, double service, int occupied, loginCode loginCode);
void initTime(nodeData *node, double time);
int condizioneBatchMeans(int v[7],int n);

#endif