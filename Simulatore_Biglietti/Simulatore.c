#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Librerie/rngs.h"
#include "Librerie/struct.h"
#include "Simulatore.h"
#include "Librerie/utils.h"
#include "Librerie/rvms.h"



double clientArrival; 
nodeParam loginParam;      
nodeParam womenParam;
nodeParam homeParam;
nodeParam awayParam;
nodeParam womenPayParam;
nodeParam homePayParam;
nodeParam awayPayParam;

void initNodeParam(){
    loginParam.mean = MEAN_LOGIN;
    womenParam.mean = MEAN_WOMEN;
    homeParam.mean = MEAN_HOME;
    awayParam.mean = MEAN_AWAY;
    womenPayParam.mean = MEAN_WOMEN_PAY;
    homePayParam.mean = MEAN_HOME_PAY;
    awayPayParam.mean = MEAN_AWAY_PAY;
    
    loginParam.stream=0;
    womenParam.stream=1;
    homeParam.stream=2;
    awayParam.stream=3;
    womenPayParam.stream=4;
    homePayParam.stream=5;
    awayPayParam.stream=6;
}


double getClientArrival(){
/* ----------------------------
 * return the next arrival time
 * ----------------------------
 */
    SelectStream(7);
    clientArrival += Exponential(1/4.039);
    return clientArrival;
}

int simulatore(output matrix[][14], int iteration, int finite){
    int currentJob[7]={0,0,0,0,0,0,0};
    int currentBatch[7]={0,0,0,0,0,0,0};

    nodeData loginStat;
    nodeData womenStat;
    nodeData homeStat;
    nodeData awayStat;
    nodeData womenPayStat;
    nodeData homePayStat;
    nodeData awayPayStat;

    nodeData homeJ1987Stat;
    nodeData homeStadiumStat;
    nodeData homeCardStat;
    nodeData homeFreeStat;



    initOutputStats(&loginStat,SERVERS_LOGIN);
    initOutputStats(&womenStat,SERVERS_WOMEN);
    initOutputStats(&homeStat,SERVERS_HOME);
    initOutputStats(&awayStat,SERVERS_AWAY);
    initOutputStats(&womenPayStat,SERVERS_WOMEN_PAY);
    initOutputStats(&homePayStat,SERVERS_HOME_PAY);
    initOutputStats(&awayPayStat,SERVERS_AWAY_PAY);
    
    initOutputStats(&homeJ1987Stat,SERVERS_HOME);
    initOutputStats(&homeStadiumStat,SERVERS_HOME);
    initOutputStats(&homeCardStat,SERVERS_HOME);
    initOutputStats(&homeFreeStat,SERVERS_HOME);

    initNodeParam();

    clientArrival = START;

    int loginAreaNumber=0;
    int womenAreaNumber=0;
    int awayAreaNumber=0;
    int womenPayAreaNumber=0;
    int homePayAreaNumber=0;
    int awayPayAreaNumber=0;

    int homeJ1987Number=0;     
    int homeStadiumNumber=0;     
    int homeCardNumber=0;     
    int homeFreeNumber=0;      
     
    int homeJ1987Service=0;  
    int homeStadiumService=0;      
    int homeCardService=0;    
    int homeFreeService=0;   
    
    event t;

    multiserver login[SERVERS_LOGIN];
    multiserver women[SERVERS_WOMEN];      
    multiserver home[SERVERS_HOME];
    multiserver away[SERVERS_AWAY];
    multiserver womenPay[SERVERS_WOMEN_PAY];
    multiserver homePay[SERVERS_HOME_PAY];
    multiserver awayPay[SERVERS_AWAY_PAY];

    initServerData(login, SERVERS_LOGIN);
    initServerData(women, SERVERS_WOMEN);
    initServerData(home, SERVERS_HOME);
    initServerData(away, SERVERS_AWAY);
    initServerData(womenPay, SERVERS_WOMEN_PAY);
    initServerData(homePay, SERVERS_HOME_PAY);
    initServerData(awayPay, SERVERS_AWAY_PAY);

    t.clientArrival=getClientArrival();
    t.loginCompletion=-1;
    t.womenCompletion=-1;
    t.homeCompletion=-1;
    t.awayCompletion=-1;
    t.womenPayCompletion=-1;
    t.homePayCompletion=-1;
    t.awayPayCompletion=-1;
    t.current=START;

    while((finite && t.clientArrival<STOP) ||
     (!finite && (condizioneBatchMeans(currentBatch,numBatch) || 
     condizioneBatchMeans(currentJob,numJobInBatch))) ||
    (finite && loginAreaNumber+womenAreaNumber+homeJ1987Number+homeStadiumNumber+homeCardNumber+homeFreeNumber+awayAreaNumber+womenPayAreaNumber+homePayAreaNumber+awayPayAreaNumber)>0){
        t.next=getSpecificMin(t.clientArrival, login, SERVERS_LOGIN);
        t.next=getSpecificMin(t.next, women, SERVERS_WOMEN);
        t.next=getSpecificMin(t.next, home, SERVERS_HOME);
        t.next=getSpecificMin(t.next, away, SERVERS_AWAY);
        t.next=getSpecificMin(t.next, womenPay, SERVERS_WOMEN_PAY);
        t.next=getSpecificMin(t.next, homePay, SERVERS_HOME_PAY);
        t.next=getSpecificMin(t.next, awayPay, SERVERS_AWAY_PAY);

        if((finite && loginAreaNumber>0) || (!finite && loginAreaNumber>0 && (currentBatch[0]<numBatch || currentJob[0]<numJobInBatch)))  statsUpdater(&loginStat, loginAreaNumber,t.current,t.next);
            
        if((finite && womenAreaNumber>0) || (!finite && womenAreaNumber>0 && (currentBatch[1]<numBatch || currentJob[1]<numJobInBatch)))  statsUpdater(&womenStat,womenAreaNumber,t.current,t.next);

        if((finite && homeJ1987Number+homeStadiumNumber+homeCardNumber+homeFreeNumber>0) || (!finite && homeJ1987Number+homeStadiumNumber+homeCardNumber+homeFreeNumber>0 && (currentBatch[2]<numBatch || currentJob[2]<numJobInBatch))) statsUpdater(&homeStat,homeJ1987Number+homeStadiumNumber+homeCardNumber+homeFreeNumber,t.current,t.next);

        if((finite && homeJ1987Number>0) || (!finite && homeJ1987Number>0 && (currentBatch[2]<numBatch || currentJob[2]<numJobInBatch)))  priorityStatsUpdater(&homeJ1987Stat,homeJ1987Number,homeJ1987Service,t.current,t.next);

        if((finite && homeStadiumNumber>0) || (!finite && homeStadiumNumber>0 && (currentBatch[2]<numBatch || currentJob[2]<numJobInBatch))) priorityStatsUpdater(&homeStadiumStat,homeStadiumNumber,homeStadiumService,t.current,t.next);

        if((finite && homeCardNumber>0) || (!finite && homeCardNumber>0 && (currentBatch[2]<numBatch || currentJob[2]<numJobInBatch))) priorityStatsUpdater(&homeCardStat,homeCardNumber,homeCardService, t.current, t.next);

        if((finite && homeFreeNumber>0) || (!finite && homeFreeNumber>0 && (currentBatch[2]<numBatch || currentJob[2]<numJobInBatch))) priorityStatsUpdater(&homeFreeStat,homeFreeNumber,homeFreeService, t.current, t.next);

        if((finite && awayAreaNumber>0) || (!finite && awayAreaNumber>0 && (currentBatch[3]<numBatch || currentJob[3]<numJobInBatch))) statsUpdater(&awayStat, awayAreaNumber, t.current, t.next);

        if((finite && womenPayAreaNumber>0) || (!finite && womenPayAreaNumber>0 && (currentBatch[4]<numBatch || currentJob[4]<numJobInBatch))) statsUpdater(&womenPayStat, womenPayAreaNumber, t.current, t.next);
        
        if((finite && homePayAreaNumber>0) || (!finite && homePayAreaNumber>0 && (currentBatch[5]<numBatch || currentJob[5]<numJobInBatch))) statsUpdater(&homePayStat, homePayAreaNumber, t.current, t.next);
        
        if((finite && awayPayAreaNumber>0) || (!finite && awayPayAreaNumber>0 && (currentBatch[6]<numBatch || currentJob[6]<numJobInBatch))) statsUpdater(&awayPayStat, awayPayAreaNumber, t.current, t.next);
        
        t.current=t.next;
        
        if(!finite)
        {
            if(numJobInBatch==currentJob[0] && currentBatch[0]<numBatch)
            {
                writeStats(matrix[currentBatch[0]],loginStat,0);
                initOutputStats(&loginStat,SERVERS_LOGIN);
                initTime(&loginStat,t.current); 
                currentJob[0]=0;
                currentBatch[0]++; 

            }
            if(numJobInBatch==currentJob[1] && currentBatch[1]<numBatch)
            {
                writeStats(matrix[currentBatch[1]],womenStat,1);
                initOutputStats(&womenStat,SERVERS_WOMEN);
                initTime(&womenStat,t.current); 
                currentJob[1]=0;
                currentBatch[1]++; 

            }
            
            if(numJobInBatch==currentJob[2] && currentBatch[2]<numBatch)
            {
                writeStats(matrix[currentBatch[2]],homeStat,2);
                initOutputStats(&homeStat,SERVERS_HOME);
                initTime(&homeStat,t.current); 
            
                writeStats(matrix[currentBatch[2]],homeJ1987Stat,4);
                writeStats(matrix[currentBatch[2]],homeStadiumStat,5);
                writeStats(matrix[currentBatch[2]],homeCardStat,6);
                writeStats(matrix[currentBatch[2]],homeFreeStat,7);

                initOutputStats(&homeJ1987Stat, SERVERS_HOME);
                initTime(&homeJ1987Stat,t.current);
                initOutputStats(&homeStadiumStat, SERVERS_HOME);
                initTime(&homeStadiumStat,t.current);
                initOutputStats(&homeCardStat, SERVERS_HOME);
                initTime(&homeCardStat,t.current);
                initOutputStats(&homeFreeStat, SERVERS_HOME);
                initTime(&homeFreeStat,t.current);

                currentJob[2]=0;
                currentBatch[2]++;  
            }

            if(numJobInBatch==currentJob[3] && currentBatch[3]<numBatch)
            {
                writeStats(matrix[currentBatch[3]],womenStat,3);
                initOutputStats(&awayStat,SERVERS_AWAY);
                initTime(&awayStat,t.current); 
                currentJob[3]=0;
                currentBatch[3]++; 

            }

            if(numJobInBatch==currentJob[4] && currentBatch[4]<numBatch)
            {
                writeStats(matrix[currentBatch[4]],womenPayStat,8);
                initOutputStats(&womenPayStat,SERVERS_WOMEN_PAY);
                initTime(&womenPayStat,t.current); 
                currentJob[4]=0;
                currentBatch[4]++; 

            }

            if(numJobInBatch==currentJob[5] && currentBatch[5]<numBatch)
            {
                writeStats(matrix[currentBatch[5]],homePayStat,9);
                initOutputStats(&homePayStat,SERVERS_HOME_PAY);
                initTime(&homePayStat,t.current); 
                currentJob[5]=0;
                currentBatch[5]++; 

            }

            if(numJobInBatch==currentJob[6] && currentBatch[6]<numBatch)
            {
                writeStats(matrix[currentBatch[6]],awayPayStat,10);
                initOutputStats(&awayPayStat,SERVERS_AWAY_PAY);
                initTime(&awayPayStat,t.current); 
                currentJob[6]=0;
                currentBatch[6]++; 

            }

            
        }
        //gestione arrivi
        if(t.current==t.clientArrival){
            loginAreaNumber++;
            loginStat.index++;
            if(!finite && currentJob[0]>=numJobInBatch) loginStat.index--;
            t.clientArrival=getClientArrival();
            if(finite && t.clientArrival>STOP){
                t.clientArrival=INF;
            }
            if(loginAreaNumber<=SERVERS_LOGIN){
                int index = FindOne(login); 
                login[index].service=t.current+getTimeService(loginParam);
                login[index].occupied=1;
                t.loginCompletion=NextEvent(login, SERVERS_LOGIN);
            }
        }
        //completamento login
        if(t.loginCompletion>-1 && t.current==login[t.loginCompletion].service){
            loginAreaNumber--;
            currentJob[0]++;
            if(loginAreaNumber-SERVERS_LOGIN>=0)
                modifyServerData(&login[t.loginCompletion], t.current+getTimeService(loginParam), 1);                 
            
            else
                modifyServerData(&login[t.loginCompletion], INF, 0);

            if(loginAreaNumber > 0)
               t.loginCompletion=NextEvent(login, SERVERS_LOGIN);
            else
                t.loginCompletion=-1;
            loginCode loginCode = assignLoginCode();
            

            switch (loginCode){
            case codeWomen:
                womenAreaNumber++;
                womenStat.index++;
                if(!finite && currentJob[1]>=numJobInBatch) womenStat.index--;
                if(womenAreaNumber<=SERVERS_WOMEN){
                    int index=FindOne(women);
                    modifyServerData(&women[index], t.current+getTimeServiceTrunc(womenParam), 1); 
                    t.womenCompletion = NextEvent(women, SERVERS_WOMEN);
                }
                break;

            case codeJ1987:
                homeJ1987Number++;
                homeStat.index++;         
                homeJ1987Stat.index++;
                if(!finite && currentJob[2]>=numJobInBatch) homeStat.index--;
                if(!finite && currentJob[2]>=numJobInBatch) homeJ1987Stat.index--;
                if(homeJ1987Number+homeStadiumNumber+homeCardNumber+homeFreeNumber<=SERVERS_HOME){
                    homeJ1987Service++;
                    int index = FindOne(home);
                    modifyServerDataHome(&home[index], t.current+getTimeServiceTrunc(homeParam), 1, codeJ1987); 
                    t.homeCompletion=NextEvent(home, SERVERS_HOME);
                }
                break;

            case codeStadium:
                homeStadiumNumber++;
                homeStat.index++;         
                homeStadiumStat.index++;
                if(!finite && currentJob[2]>=numJobInBatch) homeStat.index--;
                if(!finite && currentJob[2]>=numJobInBatch) homeStadiumStat.index--;
                if(homeJ1987Number+homeStadiumNumber+homeCardNumber+homeFreeNumber<=SERVERS_HOME){
                    if(homeJ1987Number-homeJ1987Service==0){
                        homeStadiumService++;
                        int index = FindOne(home);
                        modifyServerDataHome(&home[index], t.current+getTimeServiceTrunc(homeParam), 1, codeStadium); 
                        t.homeCompletion=NextEvent(home, SERVERS_HOME);
                    }
                }
                break;
            case codeCard:
                homeCardNumber++;
                homeStat.index++;         
                homeCardStat.index++;
                if(!finite && currentJob[2]>=numJobInBatch) homeStat.index--;
                if(!finite && currentJob[2]>=numJobInBatch) homeCardStat.index--;
                if(homeJ1987Number+homeStadiumNumber+homeCardNumber+homeFreeNumber<=SERVERS_HOME){
                    if((homeJ1987Number-homeJ1987Service==0) && (homeStadiumNumber-homeStadiumService==0)){
                        homeCardService++;
                        int index = FindOne(home); 
                        modifyServerDataHome(&home[index], t.current+getTimeServiceTrunc(homeParam), 1, codeCard); 
                        t.homeCompletion=NextEvent(home, SERVERS_HOME);
                    }
                }
                break;    
            case codeFree:
                homeFreeNumber++;
                homeStat.index++;         
                homeFreeStat.index++;
                if(!finite && currentJob[2]>=numJobInBatch) homeStat.index--;
                if(!finite && currentJob[2]>=numJobInBatch) homeFreeStat.index--;
                if(homeJ1987Number+homeStadiumNumber+homeCardNumber+homeFreeNumber<=SERVERS_HOME){
                    if((homeJ1987Number-homeJ1987Service==0) && (homeStadiumNumber-homeStadiumService==0) && (homeCardNumber-homeCardService==0)){
                        homeFreeService++;
                        int index = FindOne(home); 
                        modifyServerDataHome(&home[index], t.current+getTimeServiceTrunc(homeParam), 1, codeFree); 
                        t.homeCompletion=NextEvent(home, SERVERS_HOME);
                    }
                }
            case codeAway:
                awayAreaNumber++;
                awayStat.index++;
                if(!finite && currentJob[3]>=numJobInBatch) awayStat.index--;
                if(awayAreaNumber<=SERVERS_AWAY){
                    int index=FindOne(away);
                    modifyServerData(&away[index], t.current+getTimeServiceTrunc(awayParam), 1); 
                    t.awayCompletion = NextEvent(away, SERVERS_AWAY);
                }
                break;
        
            default:
                break;
            }
        }
        //completamento women
        else if(t.womenCompletion>-1 && t.current==women[t.womenCompletion].service){
            currentJob[1]++;
            womenAreaNumber--;
            if(womenAreaNumber-SERVERS_WOMEN>=0){
                modifyServerData(&women[t.womenCompletion], t.current+getTimeServiceTrunc(womenParam), 1);
            }
            else{
                modifyServerData(&women[t.womenCompletion], INF, 0);
            }
            if(womenAreaNumber > 0)
                t.womenCompletion=NextEvent(women, SERVERS_WOMEN);
            else
                t.womenCompletion=-1;
               
            paymentCode paymentCode = assignWomenPaymentCode();
            
            if (paymentCode==codePayWomen){
                womenPayAreaNumber++;
                womenPayStat.index++;
                if(!finite && currentJob[4]>=numJobInBatch) womenPayStat.index--;
                
                if(womenPayAreaNumber<=SERVERS_WOMEN_PAY){
                    int index=FindOne(womenPay);
                    modifyServerData(&womenPay[index], t.current+getTimeServiceTruncP(womenPayParam), 1); 
                    t.womenPayCompletion = NextEvent(womenPay, SERVERS_WOMEN_PAY);
                }
            }
            
        }

            //completamento home
        else if(t.homeCompletion>-1 && t.current==home[t.homeCompletion].service){
            currentJob[2]++;
            if(home[t.homeCompletion].loginCode==codeJ1987){
                homeJ1987Number--;
                homeJ1987Service--;
            }
            else if(home[t.homeCompletion].loginCode==codeStadium){
                homeStadiumNumber--;
                homeStadiumService--;
            }
            else if(home[t.homeCompletion].loginCode==codeCard){
                homeCardNumber--;
                homeCardService--;
            }
            else{
                homeFreeNumber--;
                homeFreeService--;
            }
            if(homeJ1987Number-homeJ1987Service>0){
                modifyServerDataHome(&home[t.homeCompletion], t.current+getTimeServiceTrunc(homeParam), 1, codeJ1987);
                homeJ1987Service++;
            }
            else if(homeStadiumNumber-homeStadiumService>0){
                modifyServerDataHome(&home[t.homeCompletion], t.current+getTimeServiceTrunc(homeParam), 1, codeStadium);
                homeStadiumService++;
            }
            else if(homeCardNumber-homeCardService>0){
                modifyServerDataHome(&home[t.homeCompletion], t.current+getTimeServiceTrunc(homeParam), 1, codeCard);
                homeCardService++;
            }
            else if(homeFreeNumber-homeFreeService>0){
                modifyServerDataHome(&home[t.homeCompletion], t.current+getTimeServiceTrunc(homeParam), 1, codeFree);
                homeFreeService++;
            }
            else{
                modifyServerDataHome(&home[t.homeCompletion], INF, 0, none);
            }

            if(homeJ1987Number+homeStadiumNumber+homeCardNumber+homeFreeNumber>0)
                t.homeCompletion = NextEvent(home, SERVERS_HOME);
            else
                t.homeCompletion=-1;

            paymentCode paymentCode = assignHomePaymentCode();

            if (paymentCode==codePayHome){
                homePayAreaNumber++;
                homePayStat.index++;
                if(!finite && currentJob[5]>=numJobInBatch) homePayStat.index--;
                
                if(homePayAreaNumber<=SERVERS_HOME_PAY){
                    int index=FindOne(homePay);
                    modifyServerData(&homePay[index], t.current+getTimeServiceTruncP(homePayParam), 1); 
                    t.homePayCompletion = NextEvent(homePay, SERVERS_HOME_PAY);
                }
            }
        }
        // completamento away
        else if(t.awayCompletion>-1 && t.current==away[t.awayCompletion].service){
            currentJob[3]++;
            awayAreaNumber--;
            if(awayAreaNumber-SERVERS_AWAY>=0){
                modifyServerData(&away[t.awayCompletion], t.current+getTimeServiceTrunc(awayParam), 1);
            }
            else{
                modifyServerData(&away[t.awayCompletion], INF, 0);
            }
            if(awayAreaNumber > 0)
                t.awayCompletion=NextEvent(away, SERVERS_AWAY);
            else
                t.awayCompletion=-1;

            paymentCode paymentCode = assignAwayPaymentCode();
  
            if (paymentCode==codePayAway){
                awayPayAreaNumber++;
                awayPayStat.index++;
                if(!finite && currentJob[6]>=numJobInBatch) awayPayStat.index--;
                
                if(awayPayAreaNumber<=SERVERS_AWAY_PAY){
                    int index=FindOne(awayPay);
                    modifyServerData(&awayPay[index], t.current+getTimeServiceTruncP(awayPayParam), 1); 
                    t.awayPayCompletion = NextEvent(awayPay, SERVERS_AWAY_PAY);
                }
            }
        }
        //completamento di womenpay
        if(t.womenPayCompletion>-1 && t.current==womenPay[t.womenPayCompletion].service){
            currentJob[4]++;
            womenPayAreaNumber--;
            if(womenPayAreaNumber-SERVERS_WOMEN_PAY>=0){
                modifyServerData(&womenPay[t.womenPayCompletion], t.current+getTimeServiceTruncP(womenPayParam),1);
            }
            else{
                modifyServerData(&womenPay[t.womenPayCompletion], INF, 0);
            }
            if(womenPayAreaNumber>0)
                t.womenPayCompletion=NextEvent(womenPay, SERVERS_WOMEN_PAY);
            else
                t.womenPayCompletion=-1;
        }

        //completamento di homepay
        if(t.homePayCompletion>-1 && t.current==homePay[t.homePayCompletion].service){
            currentJob[5]++;
            homePayAreaNumber--;
            if(homePayAreaNumber-SERVERS_HOME_PAY>=0){
                modifyServerData(&homePay[t.homePayCompletion], t.current+getTimeServiceTruncP(homePayParam),1);
            }
            else{
                modifyServerData(&homePay[t.homePayCompletion], INF, 0);
            }
            if(homePayAreaNumber>0)
                t.homePayCompletion=NextEvent(homePay, SERVERS_HOME_PAY);
            else
                t.homePayCompletion=-1;
        }

        //completamento di awaypay
        if(t.awayPayCompletion>-1 && t.current==awayPay[t.awayPayCompletion].service){
            currentJob[6]++;
            awayPayAreaNumber--;
            if(awayPayAreaNumber-SERVERS_AWAY_PAY>=0){
                modifyServerData(&awayPay[t.awayPayCompletion], t.current+getTimeServiceTruncP(awayPayParam),1);
            }
            else{
                modifyServerData(&awayPay[t.awayPayCompletion], INF, 0);
            }
            if(awayPayAreaNumber>0)
                t.awayPayCompletion=NextEvent(awayPay, SERVERS_AWAY_PAY);
            else
                t.awayPayCompletion=-1;
        }
    }
    
    writeStats(matrix[iteration],loginStat,0);
    writeStats(matrix[iteration],womenStat,1);
    writeStats(matrix[iteration],homeStat,2);
    writeStats(matrix[iteration],womenStat,3);
    writeStats(matrix[iteration],homeJ1987Stat,4);
    writeStats(matrix[iteration],homeStadiumStat,5);
    writeStats(matrix[iteration],homeCardStat,6);
    writeStats(matrix[iteration],homeFreeStat,7);
    writeStats(matrix[iteration],womenPayStat,8);
    writeStats(matrix[iteration],homePayStat,9);
    writeStats(matrix[iteration],awayPayStat,10);
    return 0;
}