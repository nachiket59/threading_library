#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../np_threadlib.h"
pthread_mutex_t lock;
struct args{
	int **m1;
	int **m2;
	int **m3;
	int r1;
	int c1;
	int r2;
	int c2; 
	int p;
	int q; 
};
void* multiply(void* a){
	int i, j, k, sum;
	struct args *arg = (struct args*)a;
	//printf("in %d %d \n",arg->p,arg->q );
	for(i = 0; i< arg->r1; i++){
		//pthread_mutex_lock(&lock);
		for(j = arg->p; j< arg->q; j++){
			sum = 0;
            for (k = 0; k < arg->r2; k++){
                sum += arg->m1[i][k] * arg->m2[k][j];
            }
			arg->m3[i][j] = sum; 
		}
		//pthread_mutex_unlock(&lock);
	}
	return 0;
}

int main(){
	printf("Test Case: Creating 1000 threads doing 1000 * 1000 matrix multiplication.\n");
	printf("Both are unit matrices. Expected result is  1000 * 1000 matrix with all 1000s.\n");
	int thrd1,thrd2,thrd3;
	struct args* a1 = malloc(sizeof(struct args));
	struct args* a2 = malloc(sizeof(struct args));
	struct args* a3 = malloc(sizeof(struct args));
	int r1,c1,r2,c2;
	scanf("%d %d", &r1, &c1);
	int i, j, k;
	int **m1 = malloc(sizeof(int *)* r1);
	for(i = 0; i< r1; i++){
		m1[i] = malloc(sizeof(int) * c1);
	}
	
	for(i = 0; i<r1; i++){
		for(j = 0; j<c1; j++){
			scanf("%d", &m1[i][j]);
		}
	}
	scanf("%d %d", &r2, &c2);
	int **m2 = malloc(sizeof(int *) * r2);
	for(i = 0; i< r2; i++){
		m2[i] = malloc(sizeof(int) * c2);
	}
	for(i = 0; i<r2; i++){
		//printf("here\n");
		for(j = 0; j<c2; j++){
			scanf("%d", &m2[i][j]);
		}
	}

	int **m3 = malloc(sizeof(int *)* r1);
	for(i = 0; i< r1; i++){
		m3[i] = malloc(sizeof(int) * c2);
	}
	if(c2>3){
		int t1 = 0, t2 = 0, t3 = 0;
		int x = c2;
		int r = x % 3;
		t1 += r;
		x -= r;
		t1 += x/3;
		t2 = t1 + x/3;
		t3 = t2 + x/3;	
		
		a1->m1=m1,a1->m2=m2,a1->m3=m3,a1->r1=r1,a1->c1=c1,a1->r2=r2,a1->c2=c2,a1->p=0,a1->q=t1;
		if(thread_create(&thrd1,NULL,multiply,(void*)a1)!=0){
			printf("error\n");
		}
		
		a2->m1=m1,a2->m2=m2,a2->m3=m3,a2->r1=r1,a2->c1=c1,a2->r2=r2,a2->c2=c2,a2->p=t1,a2->q=t2;
		if(thread_create(&thrd2,NULL,multiply,(void*)a2)!=0){
			printf("error\n");
		}
		
		a3->m1=m1,a3->m2=m2,a3->m3=m3,a3->r1=r1,a3->c1=c1,a3->r2=r2,a3->c2=c2,a3->p=t2,a3->q=t3+1;
		if(thread_create(&thrd3,NULL,multiply,(void*)a3)!=0){
			printf("error\n");
		}
		thread_join(thrd1,NULL);
		thread_join(thrd2,NULL);
		thread_join(thrd3,NULL);
	}
	else{
		a1->m1=m1,a1->m2=m2,a1->m3=m3,a1->r1=r1,a1->c1=c1,a1->r2=r2,a1->c2=c2,a1->p=0,a1->q=r2;
		multiply((void*)a1);
	}	
	/*a->m1=m1,a->m2=m2,a->m3=m3,a->r1=r1,a->c1=c1,a->r2=r2,a->c2=c2,a->p=0,a->q=t1;	
	printf("%u %u\n",m3,a->m3 );
	multiply((void*)a);
	a->m1=m1,a->m2=m2,a->m3=m3,a->r1=r1,a->c1=c1,a->r2=r2,a->c2=c2,a->p=t1,a->q=t2;
	printf("%u %u\n",m3,a->m3 );
	multiply((void*)a);
	a->m1=m1,a->m2=m2,a->m3=m3,a->r1=r1,a->c1=c1,a->r2=r2,a->c2=c2,a->p=t2,a->q=t3+1;
	multiply((void*)a);*/
	printf("%d %d\n",r1, c2 );
	for(i = 0; i< r1 ; i++){
		for(j = 0; j<c2; j++){
			printf("%d ",m3[i][j]);
		}
		printf("\n");
	}
	
	return 0;
}