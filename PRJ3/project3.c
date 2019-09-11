#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
typedef struct _Node{
	char* city;
	struct _Node* next;
}Node;

typedef struct supp{
	int num;
	Node* root;
}sup;

typedef struct _spj{
	int sid;
	int pid;
	int jid;
	int q;
}spj;

typedef struct _part{
	int num;
	int col;
	char* city;
}par;

typedef struct _job{
	int num;
	char* city;
}job;

void append(Node** root, char* city){
	Node* now = *root;
	Node* new = malloc(sizeof(*new));
	new -> city = city;
	new -> next = NULL;
	if(!now){ *root = new; return;}
	while(now -> next){
	//	fprintf(stdout,"%p a%p ",(void*)now,(void*)now->next);
		now = now->next;
	}
	now -> next = new;
	return;
}

job* read_job(char* file)
{
	FILE*fp = fopen(file,"r");
	job* _job = malloc(sizeof(job) * 100000);
	for(int i = 0;i<100000;i++)
	{
		_job[i].num = 0;
		_job[i].city = NULL;
	}
	int i = 0;
	while(fscanf(fp, "%d",&_job[i].num) != -1)
	{
		fgetc(fp);
		_job[i].city = malloc(sizeof(char) * 13);
		fscanf(fp,"%s",_job[i].city);
//		fprintf(stdout,"%s ",_job[i].city);
		i++;
	}
	fclose(fp);
	return _job;
}
spj* read_spj(char* file)
{
	FILE*fp = fopen(file,"r");
	spj* _spj = malloc(sizeof(spj) * 100000);
	for(int i = 0; i < 100000; i++)
	{
		_spj[i].sid = 0;
	    _spj[i].pid = 0;
		_spj[i].jid = 0;
		_spj[i].q = 0;
	}
	int i = 0;
	while(fscanf(fp, "%d",&_spj[i].sid) != -1)
	{
		fgetc(fp);
		fscanf(fp,"%d",&_spj[i].pid);
		fgetc(fp);
		fscanf(fp,"%d",&_spj[i].jid);
		fgetc(fp);
		fscanf(fp,"%d",&_spj[i].q);
//		fprintf(stdout,"%d\n",_spj[i].sid);
		i++;
	}
	fclose(fp);
	return _spj;
}

par* read_par(char* file)
{
	FILE*fp = fopen(file,"r");
	par* _par = malloc(sizeof(par) * 100000);
	for(int i = 0; i < 100000; i++)
	{
		_par[i].num = 0;
		_par[i].col = 0;
		_par[i].city = NULL;
	}
	int i = 0;
	while(fscanf(fp, "%d",&_par[i].num) != -1)
	{
		fgetc(fp);
		fscanf(fp,"%d",&_par[i].col);
		fgetc(fp);
		_par[i].city = malloc(sizeof(char) * 13);
		fscanf(fp,"%s",_par[i].city);
		i++;
	}
	fclose(fp);
	return _par;
}

sup* read_sup(char* file)
{
	FILE*fp = fopen(file,"r");
	sup* _sup = malloc(sizeof(sup) * 100000);
	for(int n = 0;n < 100000;n++)
	{
		_sup[n].root = NULL;
		_sup[n].num = 0;
	//	_sup[n].root->city = malloc(sizeof(char)*13);
	}
	int m = 0;
	while(fscanf(fp,"%d",&_sup[m].num) != -1)
	{
	//	fprintf(stdout,"%d\n",_sup[m].num);
		fgetc(fp);
		while(true)
		{
			
			char* str = malloc(sizeof(char) * 13);
			int c = fgetc(fp);
			bool flag = false;
			int i = 0;
			while(true){
				if(c != ',' && c != '\n' && c != '\r'  && c != EOF)
				{	
					str[i] = c;
					i++;
					c = fgetc(fp);
				}	
				else
				{
					if(c == '\n' || c == '\r' ||  c == EOF)flag = true;		
					str[i] = '\0';
					append(&(_sup[m].root),str);
					break;
				}
			}
			if(flag) break;
		}
		m++;
	//	fgetc(fp);
	}
//	fprintf(stdout,"%d ",m);
//	_print(_sup);
	fclose(fp);
	return _sup;
		
}


//sort spj
void change(spj* a,spj* b)
{
	spj c = *a;
	*a = *b;
	*b = c;
	
}
int part(spj list[],int lower,int higher)
{
	int mid = (lower + higher) / 2;
	if (list[lower].sid > list[higher].sid)
	{
		change(&list[lower],&list[higher]);
	}
	if (list[lower].sid > list[mid].sid)
	{
		change(&list[lower],&list[mid]);
	}
	if (list[mid].sid > list[higher].sid)
	{
		change(&list[mid],&list[higher]);
	}
	change(&list[mid],&list[lower]);
	int low = lower;
	int high = higher;
	spj pivot = list[lower];
	while (low < high)
	{
		while(list[low].sid <= pivot.sid && low < higher)
		{
			low++;
		}
		while(list[high].sid > pivot.sid)
		{	
			high--;
		}
		if (low < high)
		{
			change(&list[low],&list[high]);
		}	
	}
	list[lower] = list[high];
	list[high] = pivot;
	
	return high;
}

spj* qksort(spj* list, int lower, int higher)
{
	if(lower < higher){
		int pivot = part(list,lower,higher);//
			int m = pivot - 1;
			for(int i = lower; i <= pivot - 1;i++)
			{
				if(i >= m)
				{ 
					break;
				}
				if(list[i].sid == list[pivot].sid)
				{ 
					change(&list[i],&list[m]);
					m--;
				}
			}
				qksort(list,lower,m);

			qksort(list, pivot + 1, higher);
	}
	return list;
	
}
void sort(spj list[],int n)
{
	int lower = 0;
	int higher = n-1;
	int m = 0;
	int k = m + 1;
	while(list[k].sid <= list[m].sid && k < n)
	{
		m++;
		if(k + 1 == n)
		{
			return;
		}
	}
	qksort(list,lower,higher);
	return;
	
}

int get_num(spj* _spj)
{
	int num = 0;
	for(int i = 0;i < 100000;i++)
	{
		while(_spj[i].sid < 100000)
		{
			i++;	
		}
		num++;
	}
	return num;
}

int get_scity(spj* _spj,sup* _sup,int num)
{
	int total_city = 0;
	int total_q = 0;
	double avg = 0;
	sup* fake = _sup;
	for(int i = 100000 - num; i < 100000;i++)
	{
		int code = _spj[i].sid;
		for(int m = 0; m < 100000; m++)
		{
			if(code == fake[m].num)
			{
	        	
				total_city++;
				total_q += _spj[i].q;
			}
			while((i + 1) < 100000 && code == _spj[i + 1].sid)
			{
				i++;;
				total_q += _spj[i].q;
			}
			if(fake[m].num < 100000)
			{
				break;
			}
		}
		fake = _sup;
	}
	avg = (double)total_q / total_city;
	return avg;
}



void output1(spj* _spj, sup* _sup, double avg,int num)
{
	FILE*fp = fopen("output1.txt","w");
	int total = 1;
	for(int i = 100000 - num; i <100000;i++)
	{
		int code = _spj[i].sid;
		for(int m = 0; m < 100000;m++)
		{
			if(code == _sup[m].num)
			{
				total = _spj[i].q;
				while((i + 1) < 100000 && _spj[i + 1].sid == code)
				{
					i++;
					total += _spj[i].q;
				}
				if(total > avg)
				{
					fprintf(fp,"%d\n",code);
				}
			}
			if(_sup[m].num < 100000)
			{
				break;
			}
		}
	}
	fclose(fp);
}
char* g_jcity(int code,job* _job)
{
	for(int i = 0; i < 100000;i++)
	{
		if (code == _job[i].num)
		{
			char* n_job;
			n_job = _job[i].city;
//			fprintf(stdout,"%s ",n_job);
			return n_job;
		}
	}
	return "abc";
}
void n_job(spj* _spj,job* _job, par* _par, sup* _sup, int num)
{
	FILE*fp = fopen("output2.txt","w");
	for(int i = 100000 - num; i < 100000;i++)
	{
		char* n_job;
		int code = _spj[i].sid;
	//	int flag = 0;
	//	while(_spj[i+1].sid == code)
	//	{
	//		i++;
	//	}
	//	fprintf(stdout,"%d ",code);
	//	for(int m = 100000 - num; m < 100000; m++)
	//	{
	//		if(code == _spj[m].jid)
	//		{
	//			while(_spj[m + 1].jid == code)
	//			{
	//				m++;
	//			}
				n_job = g_jcity(code, _job);
			//	fprintf(stdout,"%s ",n_job);
				if(strcmp(n_job,"abc") != 0)
				{
				//	int flag = 0;
				//	fprintf(stdout,"%d ",code);
				//	for(int n = 100000 - num; n < 100000; n++)
				//	{
				//		if(code == _spj[n].pid)
				//		{
				//			flag = 1;
				//			while(_spj[n + 1].pid == code)
				//			{
				//				n++;
				//			}
					//	fprintf(stdout,"%d ",code);
							for(int a = 0;a < 100000;a++)
							{
								if(_spj[i].pid == _par[a].num)
								{
									if(_par[a].col != 2)
									{
										for(int b = 0; b < 100000; b++)
										{
								//			fprintf(stdout,"%d ",code);
											if(_spj[i].jid == _sup[b].num)
											{
										//		while(_sup[b+1].num == code)
										//		{
										//			b++;
										//		}
										//		fprintf(stdout,"%d %d ",code, _spj[i].jid);
												Node* fake = _sup[b].root;
												while(fake != NULL)
												{
												//	fprintf(stdout,"%s\n",fake->city);
													if(strcmp(n_job,fake->city) == 0)
													{		
														fprintf(fp,"%d\n", _spj[i].jid);
													//	flag = 1;
														break;
													}	
													else 
													{
														fake = fake->next;
													}
												}
											}
										
										}
									}
									break;
								}
							}
					//	}
					//	if(flag == 1)
					//	{
					//		break;
					//	}
				//	}
				}
			//	break;
		//	}
	//	}
//	if(strcmp(n_job,"abc") != 0)
//	{
//		free(n_job);
//	}
	}
	fclose(fp);
}

void output3(sup* _sup,spj* _spj, job* _job, par* _par,int num)
{
	FILE* fp = fopen("output3.txt","w");
	for(int i = 100000 - num;i < 100000;i++)
	{
		int code3 = _spj[i].sid;
		char* n_job = g_jcity(code3,_job);
		for(int a = 0;a < 100000;a++)
		{
			if(_spj[i].pid == _par[a].num)
			{
				char* c_par = _par[a].city;
				for(int b = 0;b< 100000;b++)
				{
					if(_spj[i].jid == _sup[b].num)
					{
						Node* fake = _sup[b].root;
						while(fake != NULL)
						{
							if(strcmp(fake->city,c_par) == 0)
							{
								if(strcmp(c_par,n_job)!=0)
								{
									fprintf(fp,"%d,%s\n",_spj[i].sid,c_par);
									break;
								}
								else
								{
									break;
								}
							}
							else
							{
								fake = fake->next;
							}
						}
					}
				}
			}
		}
	}
	fclose(fp);

}
int main(int argc, char *argv[])
{
	double avg;
	int num = 0;
	sup* _sup = read_sup(argv[1]);
	par* _par = read_par(argv[2]);
	job* _job = read_job(argv[3]);
	spj* _spj = read_spj(argv[4]);
	//	fprintf(stdout,"%d %d",*_sup[1].root->next->city,*_sup[1].root->city);
	_spj = qksort(_spj,0,99999);
	num = get_num(_spj);
	avg = get_scity(_spj,_sup,num);
//	fprintf(stdout,"%f ",avg);
	output1(_spj,_sup,avg,num);
	
	for(int i = 0;i<100000;i++)
	{
		while(_sup[i].root != NULL &&_sup[i].root->next != NULL)
		{
			_sup[i].root = _sup[i].root->next;
		}
		if(_sup[i].root != NULL)
		{
			free(_sup[i].root->city);
			free(_sup[i].root);
		}
	}
	free(_sup);
	sup* _sup1 = read_sup(argv[1]);
	for(int i = 100000 - num;i < 100000;i++)
	{
		int a = _spj[i].jid;
		_spj[i].jid = _spj[i].sid;
		_spj[i].sid = a;
	}
	_spj = qksort(_spj,0,99999);
	n_job(_spj,_job,_par,_sup1,num);
	for(int i = 0;i<100000;i++)
	{
		while(_sup1[i].root != NULL &&_sup1[i].root->next != NULL)
		{
			_sup1[i].root = _sup1[i].root->next;
		}
		if(_sup1[i].root != NULL)
		{
			free(_sup1[i].root->city);
			free(_sup1[i].root);
		}
	}
	free(_sup1);
	sup* _sup2 = read_sup(argv[1]);
	output3(_sup2,_spj,_job,_par,num);
	for(int i = 0;i<100000;i++)
	{
		while(_sup2[i].root != NULL &&_sup2[i].root->next != NULL)
		{
			_sup2[i].root = _sup2[i].root->next;
		}
		if(_sup2[i].root != NULL)
		{
			free(_sup2[i].root->city);
			free(_sup2[i].root);
		}
	}
	free(_sup2);
	free(_spj);
	for(int i = 0;i < 100000;i++)
	{
		if(_par[i].city != NULL)
		{
			free(_par[i].city);
		}
	}
	free(_par);
	for(int i = 0; i < 100000;i++)
	{
		if(_job[i].city != NULL)
		{
			free(_job[i].city);
		}
	}
	free(_job);

	
	return EXIT_SUCCESS;
}


