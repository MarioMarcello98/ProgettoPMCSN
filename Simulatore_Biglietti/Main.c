#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

#include "Librerie/rngs.h"
#include "Librerie/rvms.h" 
#include "Librerie/struct.h"
#include "Librerie/utils.h"
#include "Simulatore.h"




void azzeraOutput(output matrix[][14],int dim){
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<dim;j++){
			matrix[i][j].wait=0.0;
			matrix[i][j].delay=0.0;
			matrix[i][j].service=0.0;
			matrix[i][j].numberNode=0.0;
			matrix[i][j].numberQueue=0.0;
			matrix[i][j].utilization=0.0;
			matrix[i][j].job=0;
		}
	}

}

void media(output matrix[N][14], double med[7][14],int dim)
{
	double somma1[dim];
	double somma2[dim];
	double somma3[dim];
	double somma4[dim];
	double somma5[dim];
	double somma6[dim];
	double somma7[dim];

	for(int j=0;j<dim;j++)
	{
		somma1[j]=0.0;
		somma2[j]=0.0;
		somma3[j]=0.0;
		somma4[j]=0.0;
		somma5[j]=0.0;
		somma6[j]=0.0;
		somma7[j]=0.0;
		int n1=0;
		int n2=0;
		int n3=0;
		for(int i=0;i<N;i++)
		{
			if(!isnan(matrix[i][j].wait))
				somma1[j]=somma1[j]+matrix[i][j].wait;
			else
				n1++;
			if(!isnan(matrix[i][j].delay))
				somma2[j]=somma2[j]+matrix[i][j].delay;
			else
				n2++;
			if(!isnan(matrix[i][j].service))
				somma3[j]=somma3[j]+matrix[i][j].service;
			else
				n3++;
			
			somma4[j]=somma4[j]+matrix[i][j].numberNode;
		
			somma5[j]=somma5[j]+matrix[i][j].numberQueue;
			
			somma6[j]=somma6[j]+matrix[i][j].utilization;

			somma7[j]=somma7[j]+matrix[i][j].job;
		}
		med[0][j]=somma1[j]/(N-n1);
		med[1][j]=somma2[j]/(N-n2);
		med[2][j]=somma3[j]/(N-n3);
		med[3][j]=somma4[j]/N;
		med[4][j]=somma5[j]/N;
		med[5][j]=somma6[j]/N;
		med[6][j]=somma7[j]/N;
	}
}

void varianza(output matrix[][14],double med[7][14],double var[7][14],double omega[7][14],int dim){
	
	double tstar=idfStudent(N-1,1-ALFA/2);
	double somma1[dim];
	double somma2[dim];
	double somma3[dim];
	double somma4[dim];
	double somma5[dim];
	double somma6[dim];
	double somma7[dim];
	int n1=0;
	int n2=0;
	int n3=0;
	for(int j=0;j<dim;j++)
	{
		somma1[j]=0.0;
		somma2[j]=0.0;
		somma3[j]=0.0;
		somma4[j]=0.0;
		somma5[j]=0.0;
		somma6[j]=0.0;
		somma7[j]=0.0;
		for(int i=0;i<N;i++)
		{
			if(!isnan(matrix[i][j].wait))
				somma1[j]=somma1[j]+ (matrix[i][j].wait-med[0][j])*(matrix[i][j].wait-med[0][j]);
			else
				n1++;

			if(!isnan(matrix[i][j].delay))
				somma2[j]=somma2[j]+(matrix[i][j].delay-med[1][j])*(matrix[i][j].delay-med[1][j]);
			else
				n2++;

			if(!isnan(matrix[i][j].service))
				somma3[j]=somma3[j]+(matrix[i][j].service-med[2][j])*(matrix[i][j].service-med[2][j]);
			else
				n3++;

			somma4[j]=somma4[j]+(matrix[i][j].numberNode-med[3][j])*(matrix[i][j].numberNode-med[3][j]);
		
			somma5[j]=somma5[j]+(matrix[i][j].numberQueue-med[4][j])*(matrix[i][j].numberQueue-med[4][j]);
			
			somma6[j]=somma6[j]+(matrix[i][j].utilization-med[5][j])*(matrix[i][j].utilization-med[5][j]);

			somma7[j]=somma7[j]+(matrix[i][j].job-med[6][j])*(matrix[i][j].job-med[6][j]);
		}
		var[0][j]=somma1[j]/(N-n1);
		var[1][j]=somma2[j]/(N-n2);
		var[2][j]=somma3[j]/(N-n3);
		var[3][j]=somma4[j]/N;
		var[4][j]=somma5[j]/N;
		var[5][j]=somma6[j]/N;
		var[6][j]=somma7[j]/N;	
		omega[0][j]=tstar*sqrt(var[0][j])/sqrt(N-n1-1);
		omega[1][j]=tstar*sqrt(var[1][j])/sqrt(N-n2-1);
		omega[2][j]=tstar*sqrt(var[2][j])/sqrt(N-n3-1);
		omega[3][j]=tstar*sqrt(var[3][j])/sqrt(N-1);
		omega[4][j]=tstar*sqrt(var[4][j])/sqrt(N-1);
		omega[5][j]=tstar*sqrt(var[5][j])/sqrt(N-1);
		omega[6][j]=tstar*sqrt(var[6][j])/sqrt(N-1);
		printf("\n");
		printf("\t %s\n",matrix[0][j].nome);    
		printf("\t wait %f ± %f\n",med[0][j],omega[0][j]);
		printf("\t delay %f ± %f\n",med[1][j],omega[1][j]);
		printf("\t service %f ± %f\n",med[2][j],omega[2][j]);
		printf("\t numberNode %f ± %f\n",med[3][j],omega[3][j]);
		printf("\t numberQueue %f ± %f\n",med[4][j],omega[4][j]);
		printf("\t utilization %f ± %f\n",med[5][j],omega[5][j]);
		printf("\t job %f ± %f\n",med[6][j],omega[6][j]);
		printf("\t\n");
	}
}
void incrementalMean(output matrix[N][14],int j, int i, output *out){
	double sum_w=0.0;
	double sum_d=0.0;
	double sum_s=0.0;
	double sum_nn=0.0;
	double sum_nq=0.0;
	double sum_u=0.0;
	double sum_j=0.0;
	int n=0;
	for(int k=0;k<=i;k++){
		if(matrix[k][j].wait==matrix[k][j].wait){
			sum_w+=matrix[k][j].wait;
			sum_d+=matrix[k][j].delay;
			sum_s+=matrix[k][j].service;
			sum_nn+=matrix[k][j].numberNode;
			sum_nq+=matrix[k][j].numberQueue;
			sum_u+=matrix[k][j].utilization;
			sum_j+=matrix[k][j].job;
			n++;
		}
	}
	out->wait=sum_w/n;
	out->delay=sum_d/n;
	out->service=sum_s/n;
	out->numberNode=sum_nn/(i+1);
	out->numberQueue=sum_nq/(i+1);
	out->utilization=sum_u/(i+1);
	out->job=sum_j/(i+1);
}
void writeFileCSV(output matrix[N][14], char *path,int dim){
	DIR *dir=opendir("./statistiche");
	if(dir){
		closedir(dir);
	}
	else if(ENOENT==errno){
		if(mkdir("./statistiche",0777)<0)
		{
			printf("error\n");
			exit(-1);
		}
	}
	output out;
	FILE* file=fopen(path,"w+");
	if(file==NULL)
	{
		printf("error\n");
		exit(-1);
	}
	fprintf(file, "j,i,wait,delay,service,n_node,n_queue,rho,job\n");
	for(int j=0;j<dim;j++){
		for(int i=0;i<N;i++){
			incrementalMean(matrix,j,i,&out);
			fprintf(file, "%d,%d,%f,%f,%f,%f,%f,%f,%f\n",j,i,out.wait,out.delay,out.service,out.numberNode,out.numberQueue,out.utilization,out.job);
		}
	}
	fclose(file);

}

int main(){ 
	//simulazione del transiente;
	printf("FINITE HORIZON SIMULATION\n");
	output matrix[N][14];
	strcpy(matrix[0][0].nome,"Login");
	strcpy(matrix[0][1].nome,"Women");
	strcpy(matrix[0][2].nome,"Home");
	strcpy(matrix[0][3].nome,"Away");
	strcpy(matrix[0][4].nome,"J1987");
	strcpy(matrix[0][5].nome,"Stadium");
	strcpy(matrix[0][6].nome,"Card");
	strcpy(matrix[0][7].nome,"Free");
	strcpy(matrix[0][8].nome,"Pagamento Women");
	strcpy(matrix[0][9].nome,"Pagamento Home");
	strcpy(matrix[0][10].nome,"Pagamento Away");
	double med[7][14];
	double var[7][14];
	double omega[7][14];
	PlantSeeds(SEED);
	azzeraOutput(matrix,11);
	for(int i=0;i<N;i++){
		simulatore(matrix,i,1);	
	}
	writeFileCSV(matrix, "./statistiche/transiente.csv",11);
	media(matrix,med,11);
	varianza(matrix,med,var,omega,11);
	
	//simulazione dello stazionario
	printf("INFINITE HORIZON SIMULATION\n");
	PlantSeeds(SEED);
	azzeraOutput(matrix,11);
	simulatore(matrix, numBatch-1, 0);
	writeFileCSV(matrix, "./statistiche/steady_state.csv",11);
	media(matrix,med,11);
	varianza(matrix,med,var,omega,11);
	return 0;
}