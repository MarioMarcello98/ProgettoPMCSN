#ifndef Simulatore_h
#define Simulatore_h

#include <stdio.h>
#define SEED 133334457
#define MEAN_LOGIN 0.05
#define MEAN_WOMEN 12.00
#define MEAN_HOME 12.00
#define MEAN_AWAY 12.00
#define MEAN_WOMEN_PAY 4.00
#define MEAN_HOME_PAY 4.00
#define MEAN_AWAY_PAY 4.00

#define SERVERS_LOGIN 4 
#define SERVERS_HOME 39
#define SERVERS_AWAY 9
#define SERVERS_WOMEN 4
#define SERVERS_WOMEN_PAY 3
#define SERVERS_HOME_PAY 16
#define SERVERS_AWAY_PAY 5

int simulatore(output matrix[][14], int iteration, int finite);

#endif