#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>

typedef struct _task{
	int arr;
	int* ser;
	int num;
}task;
void _cal(task* task0, task* task1, int num0, int num1,double mu);
int _max(task f_task);
int _availt(int num, int f_time, int*pro);
int _selected_task(int* availt,int num,int n);
int _time(int time,task task_x);
int generate_rate(double inputRate);
int _min(task f_task);

int main(int argc , char** argv)
{
	if(argc == 5)
	{
		double lambda0 = atof(argv[1]);
		double lambda1 = atof(argv[2]);
		double mu = atof(argv[3]);
		int num_terms = atoi(argv[4]);
		int num_zeros=0;
		int num_ones=0;
		int subtask_num=0;
		task* task1 = malloc(sizeof(task) * num_terms);
		task* task0 = malloc(sizeof(task) * num_terms);
		task1[0].arr = 0;
		task0[0].arr = 0;

		subtask_num = (int)(rand() % 32+ 1);
		task0[num_zeros].arr = 1 / generate_rate(lambda0);
		task0[num_zeros].num = subtask_num;
		task0[num_zeros].ser = malloc(sizeof(int)*subtask_num);
		for(int i = 0; i <subtask_num ;i++)
		{
			task0[num_zeros].ser[i] =(int)(1 / generate_rate(mu));
		}
		num_zeros++;
		task1[num_ones].arr = 1/generate_rate(lambda1);
		task1[num_ones].num = subtask_num;
		task1[num_ones].ser = malloc(sizeof(int)*subtask_num);
		for(int i = 0; i <subtask_num ;i++)
		{
			task1[num_ones].ser[i] =(int)(1 / generate_rate(mu));
		}
		num_ones++;

		while(num_zeros < num_terms || num_ones <num_terms)
		{
			
			if(num_zeros < num_terms)
			{
				subtask_num = (int)(rand() % 32+ 1);
				task0[num_zeros].arr = 1/generate_rate(lambda0) + task0[num_zeros - 1].arr;
				task0[num_zeros].num = subtask_num;
				task0[num_zeros].ser = malloc(sizeof(int)*subtask_num);
				for(int i = 0; i <subtask_num ;i++)
				{
					task0[num_zeros].ser[i] =(int)(1/generate_rate(mu));
				}
				num_zeros++;
			}
			else if(num_ones < num_terms)
			{
				subtask_num = (int)(rand() % 32+ 1);
				task1[num_ones].arr = 1/generate_rate(lambda1) + task1[num_ones - 1].arr;
				task1[num_ones].num = subtask_num;
				task1[num_ones].ser = malloc(sizeof(int)*subtask_num);
				for(int i = 0; i <subtask_num ;i++)
				{
					task1[num_ones].ser[i] =(int)(1/generate_rate(mu));
				}
				num_ones++;
			} 
		}
		_cal(task0,task1,num_terms,num_terms,mu);
		for(int i = 0;i<num_terms;i++)
		{
			free(task1[i].ser);
		}
		free(task1);
		for(int i = 0;i<num_terms;i++)
		{
			free(task0[i].ser);
		}
		free(task0);
		return EXIT_SUCCESS;
	}
	if (argc == 2)
	{
		int i = 0;
		int m = 0;
		int number = 0;
		int arr_t = 0;
		int ser_t = 0;
		int type = 0;
		int num1 = 0;
		int num0 = 0;
		FILE*fp = fopen(argv[1],"r");
		while(fscanf(fp,"%d %d %d",&arr_t,&type,&number) != -1){
			if (type == 1){
				num1++;
			}
			else{
				num0++;
			}
			for(int j = 0; j < number; j++){
				fscanf(fp,"%d",&ser_t);
			}
		}
		task* task1 = malloc(sizeof(task) * num1);
		task* task0 = malloc(sizeof(task) * num0);
		fseek(fp,0L,SEEK_SET);
		while(fscanf(fp,"%d %d %d",&arr_t,&type,&number) != -1)
		{
			if(feof(fp)){
				break;
			}
			if(type == 1){
				task1[i].arr = arr_t;
				task1[i].num = number;
				task1[i].ser = malloc(sizeof(int) * number);
				for(int j = 0; j < number; j++){
					fscanf(fp,"%d",&ser_t);
					task1[i].ser[j] = ser_t;
				}
				i++;
			}
			else{
				task0[m].arr = arr_t;
				task0[m].num = number;
				task0[m].ser = malloc(sizeof(int) * number);
				for(int j = 0; j < number; j++){
					fscanf(fp,"%d",&ser_t);
					task0[m].ser[j] = ser_t;
				}
				m++;
			}
		}
		double mu1 = 0;
		_cal(task0, task1, num0, num1,mu1);
		for(int a = 0;a<num1;a++)
		{
			free(task1[a].ser);
		}
		free(task1);
		for(int a = 0;a<num0;a++)
		{
			free(task0[a].ser);
		}
		free(task0);
		fclose(fp);
		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}

void _update_cpu_time(task tas,int* pro,int time){   

	int last = 0;
	for(int i = 0;i< tas.num;i++)
	{
		for(int j = last; j < 64;j++)
		{
			if(time >= pro[j])
			{
				pro[j] = tas.ser[i] + time; 
				last = j + 1;
				break;
			}
		}
	}
}

void _cal(task* task0, task* task1, int num0, int num1, double mu1)
{
	double max = 0;
	double min = 0;
	int total = 0;
	int total_sub = 0;
	double factor= 0;
	for(int i = 0; i < num0;i++)
	{
		for(int j = 0; j < task0[i].num;j++)
		{
			total += task0[i].ser[j];
		}
		total_sub += task0[i].num;
	}
	for(int i = 0; i < num1;i++)
	{
		for(int j = 0; j < task1[i].num;j++)
		{
			total += task1[i].ser[j];
		}
		total_sub += task1[i].num;
	}
	double mu = 0;
	if(mu1 == 0)
	{
		mu =(double)(total_sub) / total;
	}
	else
	{
		mu = mu1;
	}
	for(int i = 0;i<num0;i++)
	{
		min = _min(task0[i]);
		max = _max(task0[i]);
		factor += (max - min) / (1.0 /mu);
	}
	for(int i = 0;i<num0;i++)
	{
		min = _min(task1[i]);
		max = _max(task1[i]);
		factor += (max - min) / (1.0 /mu);
	}
	double t_factor = factor / (num1 + num0); 
	double wait_t0 = 0;
	double wait_t1 = 0;
	int* pro = calloc(64,sizeof(int));
	int time = 0;
	int m = 0;
	int n = 0;
	while(m < num1 || n < num0)
	{
		int N0 = n;
		int N1 = m;
		int* avail_t0 = calloc(num0,sizeof(int));
		int* avail_t1 = calloc(num1,sizeof(int));
		////////find first available 0
		while(N0 < num0 && task0[N0].arr <= time)
		{
			int f_time = task0[N0].arr;
			avail_t0[N0] = _availt(task0[N0].num,f_time,pro);

			N0++;
		}
		N0 = _selected_task(avail_t0,N0,n);
		////////find first available 1
		while(N1 < num1 && task1[N1].arr <= time)
		{
			int f_time = task1[N1].arr;
			avail_t1[N1] = _availt(task1[N1].num,f_time,pro);

			N1++;
		}
		N1 = _selected_task(avail_t1,N1,m);
		/////////if N0 == n && N1 != m =====> only 1
		if(N0 == -1 && N1 != -1)
		{
			wait_t1 += avail_t1[N1] - task1[N1].arr;

			task fake = task1[N1];
			for(int i = N1;i > m;i--)
			{
				task1[i] = task1[i - 1];	
			}
			task1[m] = fake;
			_update_cpu_time(task1[m],pro,avail_t1[N1]); 
			if(time < _time(avail_t1[N1],task1[m]))
			{
				time = _time(avail_t1[N1],task1[m]);
			}
			m++;
			free(avail_t0);
			free(avail_t1);
			continue;
		}
		/////////if N0 != n && N1 == m =====> only 0
		if(N0 != -1 && N1 == -1)
		{
			wait_t0 += avail_t0[N0] - task0[N0].arr;
			task fake = task0[N0];
			for(int i = N0;i > n;i--)
			{
				task0[i] = task0[i - 1];	
			}
			task0[n] = fake;
			_update_cpu_time(task0[n],pro,avail_t0[N0]); 
			if(time < _time(avail_t0[N0],task0[n]))
			{
				time = _time(avail_t0[N0],task0[n]);
			}
			n++;
			free(avail_t0);
			free(avail_t1);
			continue;
		}
		/////////if N0 == n && N1 == m =====> task comes after time
		if(N0 == -1 && N1 == -1)
		{
			if(n == num0){

				_update_cpu_time(task1[m],pro,task1[m].arr); 
				if(time < _time(task1[m].arr,task1[m]))
				{		
					time = _time(task1[m].arr,task1[m]);
				}
				m++;
				free(avail_t0);
				free(avail_t1);
				continue;
			}
			if(m == num1){

				_update_cpu_time(task0[n],pro,task0[n].arr);  
				if(time < _time(task0[n].arr,task0[n]))
				{
					time = _time(task0[n].arr,task0[n]);
				}	
				n++;
				free(avail_t0);
				free(avail_t1);
				continue;
			}
			if(task0[n].arr > task1[m].arr)
			{
				_update_cpu_time(task1[m],pro,task1[m].arr);
				if(time < _time(task1[m].arr,task1[m]))
				{		
					time = _time(task1[m].arr,task1[m]);
				}
				m++;
			}
			else
			{
				_update_cpu_time(task0[n],pro,task0[n].arr);
				if(time < _time(task0[n].arr,task0[n]))
				{
					time = _time(task0[n].arr,task0[n]);
				}	
				n++;
			}
			free(avail_t0);
			free(avail_t1);
			continue;
		}	
		/////////if N0 != n && N1 != m =====> choose a task which can enter the 
		if(N0 != -1 && N1 != -1)
		{ 
			if(task0[N0].arr <= task1[N1].arr){
				wait_t0 += avail_t0[N0] - task0[N0].arr;
				if(N0 != n){
					task fake = task0[N0];
					for(int i = N0;i>n ; i--)
					{
						task0[i] = task0[i - 1];	
					}
					task0[n] = fake;
				}
				assert(n >= 0 && n < num0);
				_update_cpu_time(task0[n],pro,avail_t0[N0]);  
				if(time < _time(avail_t0[N0],task0[n]))
				{
					time = _time(avail_t0[N0],task0[n]);  
				}
				n++;
			}
			else
			{
				wait_t1 += avail_t1[N1] - task1[N1].arr;
				if(N1 != m){
					task fake = task1[N1];
					for(int i = N1;i>m ; i--)
					{
						task1[i] = task1[i - 1];	
					}
					task1[m] = fake;
				}
				assert(m >= 0 && m < num1);
				_update_cpu_time(task1[m],pro,avail_t1[N1]);   
				if(time < _time(avail_t1[N1],task1[m]))
				{
					time = _time(avail_t1[N1],task1[m]); 
				}
				m++;	
			}
		}
		free(avail_t0);
		free(avail_t1);
	}
	free(pro);
	double cpu = 0;
	cpu = ((total / 64.0)/time) * 100;
	double length = 0.0;
	length = (double)((wait_t0 + wait_t1) * 1.0) / (num0 + num1);
	fprintf(stdout,"Average waiting time for 'Priority 0' arrives: %.6f units of time\n",wait_t0 / num0);
	fprintf(stdout,"Average waiting time for 'Priority 1' arrives: %.6f units of time\n",wait_t1 / num1);
	fprintf(stdout,"Average Queue length: %.6f customers\n",length);
	fprintf(stdout,"CPU Utilization: %.6f%%\n",cpu);
	fprintf(stdout,"Average load balancing factor: %.6f\n",t_factor);
	return;
}

int _availt(int num, int f_time, int*pro)
{
	int time = f_time;
	while(true)
	{
		int a_n = 0;
		for(int i = 0; i < 64;i++)
		{
			if(pro[i] <= time)
			{
				a_n++;
			}	
		}	
		if(a_n >= num)
		{
			break;
		}
		else{
			time++;
		}
	}
	return time;
}

int _selected_task(int* availt,int num,int n)
{
	int l = -1;
	int t = INT_MAX;
	for(int i = n;i < num;i++)
	{
		if(t > availt[i])
		{
			t = availt[i];
			l = i;

		}
	}
	return l;
}
int _time(int time,task task_x)
{
	int max = 0;
	max = _max(task_x);
	time = time + max;
	return time;
}

int _max(task f_task)
{
	int i = 0;
	int max = f_task.ser[i];
	while(i < f_task.num - 1)
	{
		if(max < f_task.ser[i + 1]){
			max = f_task.ser[i + 1];
		}
		i++;
	}
	return max;
}
int _min(task f_task)
{
	int i = 0;
	int min = f_task.ser[i];
	while(i < f_task.num - 1)
	{
		if(min > f_task.ser[i + 1]){
			min = f_task.ser[i + 1];
		}
		i++;
	}
	return min;
}
int generate_rate(double inputRate)
{
	int rnd_value = rand();
	float unit_rand = rnd_value / (double) RAND_MAX;
	return ceil((-1 / inputRate)* log(1 - unit_rand));
}
