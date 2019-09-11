#include <stdio.h>
#include <math.h>
#include <stdlib.h>
// This project is completed by ZIYU GUO.
// my login account is guo412
int generate_rate(double input);
void _wait(int* arr0, int* arr1,int* ser_t0,int* ser_t1, int num0, int num1);
void _result(double avg_w0, double avg_w1, double ult, double length);
int main(int argc, char** argv)
{
		if (argc == 5){
			int num = atoi(argv[4]);
			int *rarr0 = malloc(sizeof(int)*num);
			int *rarr1 = malloc(sizeof(int)*num);
			int *rser0 = malloc(sizeof(int)*num);
			int *rser1 = malloc(sizeof(int)*num);
			rarr0[0] = 0;
			rarr1[0] = 0;
			rser0[0] = generate_rate(atof(argv[3]));
			rser1[0] = generate_rate(atof(argv[3]));
			int i = 1;
			while (i < atoi(argv[4]))
			{
				rarr0[i] = generate_rate(atof(argv[1])) + rarr0[i - 1];
				rarr1[i] = generate_rate(atof(argv[2])) + rarr1[i - 1];
				rser0[i] = generate_rate(atof(argv[3]));
				rser1[i] = generate_rate(atof(argv[3]));
				i++;
			}
			_wait(rarr0,rarr1,rser0,rser1,num,num);
			free(rarr0);
			free(rarr1);
			free(rser0);
			free(rser1);
			return EXIT_SUCCESS;

		}
		else if (argc == 2) {
			FILE*fp = fopen(argv[1],"r");
			int num0 = 0;
			int num1 = 0;
			int arr = 0;
			int type = 0;
			int ser = 0;
			while(fscanf(fp,"%d %d %d",&arr,&type,&ser) != -1)
			{
				if(type == 1){
					num1++;
				}
				else{
					num0++;
				}
			}
			fclose(fp);
			int* arr0 = malloc(sizeof(int) * num0);
			int* arr1 = malloc(sizeof(int) * num1);
			int* ser_t0 = malloc(sizeof(int) * num0);
			int* ser_t1 = malloc(sizeof(int) * num1);
			FILE*fp1 = fopen(argv[1],"r");
			int i = 0;
			int j = 0;
			while(fscanf(fp1,"%d %d %d", &arr,&type,&ser) != -1){
				if(type){
					arr1[i] = arr;
					ser_t1[i] = ser;
					i++;
				}
				else{
					arr0[j] = arr;
					ser_t0[j] = ser;
					j++;
				}
			}
			_wait(arr0, arr1, ser_t0, ser_t1, num0, num1);
			free(arr0);
			free(arr1);
			free(ser_t0);
			free(ser_t1);
			fclose(fp1);
			return EXIT_SUCCESS;
		}
		else
			return EXIT_SUCCESS;
}



void _wait(int* arr0, int* arr1,int* ser_t0,int* ser_t1, int num0, int num1)
{
	double time = 0;
	double wait_t0 = 0;
	double wait_t1 = 0;
	int i = 0;
	int j = 0;
	int nwait = 0;
	double length = 0;
	if(arr0[i] > arr1[j]){
		time = arr1[j] + ser_t1[j];
		j = j + 1;
	}
	else{
		time = arr0[i] + ser_t0[i];
		i = i + 1;
	}
	while (i < num0 || j < num1){
		if(j >= num1){
			if(time > arr0[i]){
				wait_t0 +=time - arr0[i];
				time = time +ser_t0[i];
				i++;
			}
			else
			{
				time = arr0[i] +ser_t0[i];
				i++;
			}
		}
		else if(i >= num0){
			if(time > arr1[j]){
				wait_t1 +=time - arr1[j];
				time = time + ser_t1[j];
				j++;
			}
			else
			{
				time = arr1[j] + ser_t1[j];
				j++;
			}
		}
		else if(arr0[i] > arr1[j]){
			if(time <= arr1[j]){
				time = arr1[j] + ser_t1[j];
				j++;
			}
			else if (time >= arr0[i]){
				wait_t0 += time - arr0[i];
				time = time + ser_t0[i];
				i++;
			}
			else{
				wait_t1 += time - arr1[j];
				time = time + ser_t1[j];
				j++;
			}
		}
		else if(arr0[i] <= arr1[j]){
			if (time <= arr0[i]){
				time = arr0[i] + ser_t0[i];
				i++;
			}
			else if (time >= arr1[j]){
				wait_t0 += time - arr0[i];
				time = time + ser_t0[i];
				i++;
			}
			else{
				wait_t0 += time - arr0[i];
				time = time + ser_t0[i];
				i++;
			}
		}
	}
	double avg_w0 = wait_t0 / num0;
	double avg_w1 = wait_t1 / num1;
	double total_s = 0;
	int m = 0;
	int n = 0;
	for (m = 0; m < num1; m++){
		total_s += ser_t1[m];
	}
	for (n = 0; n < num0; n++){
		total_s += ser_t0[n];
	}
	double ult = total_s / time;
	length = (wait_t0 + wait_t1) / time;
	_result(avg_w0, avg_w1, ult,length);
	return;
}   

void _result(double avg_w0, double avg_w1, double ult, double length){
	fprintf(stdout,"average_wait0 = %f\naverage_wait1 = %f\naverage_CPU_utilization = %f\naverage_queue_length = %f\n",avg_w0, avg_w1, ult, length);
	return;
}

int generate_rate(double input){
	int r_value = rand();
	float unit_rand = r_value / (double)RAND_MAX;
	return ceil((-1/input)*log(1 - unit_rand));
}

