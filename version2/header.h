#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

int count=0;
int numtrain=0;
int policy=SCHED_RR;
int nb_random;
int ajout;

int nb_TGV_oe;
int nb_TGV_eo;
int nb_TGV_oe_entrer=0;
int nb_TGV_oe_garage=0;
int nb_TGV_eo_entrer=0;
int nb_TGV_eo_garage=0;
int sensTGV=0;


int nb_GL_oe;
int nb_GL_eo;
int nb_GL_oe_entrer=0;
int nb_GL_oe_garage=0;
int nb_GL_eo_entrer=0;
int nb_GL_eo_garage=0;
int sensGL=0;

int nb_M_oe;
int nb_M_eo;




pthread_t train[100];

pthread_mutex_t P0=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t P1=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t P2=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t P3=PTHREAD_MUTEX_INITIALIZER;


sem_t sem_garageTGV;
pthread_mutex_t sensTGV_mut=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t entrer_TGV_oe_mut=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t garage_TGV_oe_mut=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t entrer_TGV_eo_mut=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t garage_TGV_eo_mut=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t   TGV_oe_cond=PTHREAD_COND_INITIALIZER;
pthread_cond_t   TGV_eo_cond=PTHREAD_COND_INITIALIZER;
pthread_cond_t   entrer_TGV_oe_cond=PTHREAD_COND_INITIALIZER;
pthread_cond_t   garage_TGV_oe_cond=PTHREAD_COND_INITIALIZER;
pthread_cond_t   entrer_TGV_eo_cond=PTHREAD_COND_INITIALIZER;
pthread_cond_t   garage_TGV_eo_cond=PTHREAD_COND_INITIALIZER;

sem_t sem_garageGL;
pthread_mutex_t sensGL_mut=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t entrer_GL_oe_mut=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t garage_GL_oe_mut=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t entrer_GL_eo_mut=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t garage_GL_eo_mut=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t   GL_oe_cond=PTHREAD_COND_INITIALIZER;
pthread_cond_t   GL_eo_cond=PTHREAD_COND_INITIALIZER;
pthread_cond_t   entrer_GL_oe_cond=PTHREAD_COND_INITIALIZER;
pthread_cond_t   garage_GL_oe_cond=PTHREAD_COND_INITIALIZER;
pthread_cond_t   entrer_GL_eo_cond=PTHREAD_COND_INITIALIZER;
pthread_cond_t   garage_GL_eo_cond=PTHREAD_COND_INITIALIZER;

sem_t sem_garageM_oe;
sem_t sem_garageM_eo;




pthread_mutex_t num_mut=PTHREAD_MUTEX_INITIALIZER;


void *TGV_oe( )
{
	

	pthread_mutex_lock(&sensTGV_mut);
	if (sensTGV<0) pthread_cond_wait(&TGV_oe_cond,&sensTGV_mut);
	sensTGV++;
	pthread_mutex_unlock(&sensTGV_mut);	
	
	pthread_mutex_lock(&entrer_TGV_oe_mut);
	nb_TGV_oe_entrer++;
	pthread_mutex_unlock(&entrer_TGV_oe_mut);	

	pthread_mutex_lock(&num_mut);
	int num=numtrain;	
	numtrain++;
	pthread_mutex_unlock(&num_mut);	

	
	pthread_mutex_lock(&P0);
	
	printf ("TGV-->%d attend\n",num);
	sleep(1);
	sem_wait(&sem_garageTGV);
	pthread_mutex_lock(&P1);
	printf("TGV-->%d départ Gare\n",num);
	pthread_mutex_unlock(&P0);
	sleep(1);

	pthread_mutex_lock(&garage_TGV_oe_mut);
	nb_TGV_oe_garage++;
	pthread_mutex_unlock(&garage_TGV_oe_mut);
	printf(" %28s%d %s\n","TGV-->",num,"arrive Garage");
	
	pthread_mutex_lock(&entrer_TGV_oe_mut);
	nb_TGV_oe_entrer--;
	if(nb_TGV_oe_entrer==0)pthread_cond_broadcast(&entrer_TGV_oe_cond);
	pthread_mutex_unlock(&entrer_TGV_oe_mut);
	
	pthread_mutex_unlock(&P1);
	
	//sleep(3); test sem
	pthread_mutex_lock(&P2);	
	pthread_mutex_lock(&P3);
	printf(" %28s%d %s\n","TGV-->",num,"départ Garage");
	
	pthread_mutex_lock(&garage_TGV_oe_mut);
	nb_TGV_oe_garage--;
	if(nb_TGV_oe_garage==0)pthread_cond_broadcast(&garage_TGV_oe_cond);
	pthread_mutex_unlock(&garage_TGV_oe_mut);
	sleep(1);
	sem_post(&sem_garageTGV);
	

	
	printf(" %53s%d %s\n","TGV-->",num,"dans Tunnel");
	sleep(1);
	printf(" %83s%d %s\n","TGV-->",num,"sort");
	sleep(1);
	pthread_mutex_unlock(&P2);
	pthread_mutex_unlock(&P3);

	pthread_mutex_lock(&sensTGV_mut);
	
	sensTGV--;
	if (sensTGV==0) pthread_cond_broadcast(&TGV_eo_cond);

	pthread_mutex_unlock(&sensTGV_mut);
	
	pthread_exit(NULL);

}



 
void *TGV_eo( )
{
	
	

	pthread_mutex_lock(&sensTGV_mut);
	if (sensTGV>0) pthread_cond_wait(&TGV_eo_cond,&sensTGV_mut);
	sensTGV--;
	pthread_mutex_unlock(&sensTGV_mut);
	
	pthread_mutex_lock(&entrer_TGV_eo_mut);
	nb_TGV_eo_entrer++;
	pthread_mutex_unlock(&entrer_TGV_eo_mut);	
	
	pthread_mutex_lock(&num_mut);
	int num=numtrain;
	numtrain++;
	pthread_mutex_unlock(&num_mut);	
	
	sem_wait(&sem_garageTGV);
	pthread_mutex_lock(&P2);	
	pthread_mutex_lock(&P3);
	
	printf(" %83s%d %s\n","TGV<--",num,"départ");
	sleep(1);
	printf(" %53s%d %s\n","TGV<--",num,"dans Tunnel");
	sleep(1);
	
	pthread_mutex_lock(&garage_TGV_eo_mut);
	nb_TGV_eo_garage++;
	pthread_mutex_unlock(&garage_TGV_eo_mut);
	printf(" %28s%d %s\n","TGV<--",num,"arrive Garage");
	
	
	pthread_mutex_lock(&entrer_TGV_eo_mut);
	nb_TGV_eo_entrer--;
	if(nb_TGV_eo_entrer==0)pthread_cond_broadcast(&entrer_TGV_eo_cond);
	pthread_mutex_unlock(&entrer_TGV_eo_mut);

	pthread_mutex_unlock(&P2);
	pthread_mutex_unlock(&P3);
	//sleep(3); test sem
	pthread_mutex_lock(&P0);
	pthread_mutex_lock(&P1);
	
	
	printf(" %28s%d %s\n","TGV<--",num,"départ Garage");
	
	
	pthread_mutex_lock(&garage_TGV_eo_mut);
	nb_TGV_eo_garage--;
	if(nb_TGV_eo_garage==0)pthread_cond_broadcast(&garage_TGV_eo_cond);
	pthread_mutex_unlock(&garage_TGV_eo_mut);
	sleep(1);

	sem_post(&sem_garageTGV);
	
	printf ("TGV<--%d attend\n",num);
	sleep(1);
	printf("TGV<--%d sort\n",num);
	sleep(1);
	pthread_mutex_unlock(&P0);
	pthread_mutex_unlock(&P1);	
	

	pthread_mutex_lock(&sensTGV_mut);
	
	sensTGV++;
	if (sensTGV==0) pthread_cond_broadcast(&TGV_oe_cond);
	pthread_mutex_unlock(&sensTGV_mut);

	pthread_exit(NULL);

}

void *GL_oe( )
{
		
	pthread_mutex_lock(&sensGL_mut);
	if (sensGL<0) pthread_cond_wait(&GL_oe_cond,&sensGL_mut);
	sensGL++;
	pthread_mutex_unlock(&sensGL_mut);

	pthread_mutex_lock(&entrer_GL_oe_mut);
	nb_GL_oe_entrer++;
	pthread_mutex_unlock(&entrer_GL_oe_mut);

	pthread_mutex_lock(&num_mut);
	int num=numtrain;	
	numtrain++;
	pthread_mutex_unlock(&num_mut);
		
	pthread_mutex_lock(&entrer_TGV_oe_mut);
	if(nb_TGV_oe_entrer!=0)pthread_cond_wait(&entrer_TGV_oe_cond,&entrer_TGV_oe_mut);
	pthread_mutex_unlock(&entrer_TGV_oe_mut);

	pthread_mutex_lock(&garage_TGV_eo_mut);
	if(nb_TGV_eo_garage!=0)pthread_cond_wait(&garage_TGV_eo_cond,&garage_TGV_eo_mut);
	pthread_mutex_unlock(&garage_TGV_eo_mut);

			

	pthread_mutex_lock(&P0);
	printf ("GL-->%d attend\n",num);
	sleep(1);
	
	
	sem_wait(&sem_garageGL);
	pthread_mutex_lock(&P1);

	printf("GL-->%d départ Gare\n",num);
	pthread_mutex_unlock(&P0);
	
	sleep(1);
	pthread_mutex_lock(&garage_GL_oe_mut);
	nb_GL_oe_garage++;
	pthread_mutex_unlock(&garage_GL_oe_mut);
	printf(" %28s%d %s\n","GL-->",num,"arrive Garage");
	
	
	pthread_mutex_lock(&entrer_GL_oe_mut);
	nb_GL_oe_entrer--;
	if(nb_GL_oe_entrer==0)pthread_cond_broadcast(&entrer_GL_oe_cond);
	pthread_mutex_unlock(&entrer_GL_oe_mut);

	pthread_mutex_unlock(&P1);		

	//sleep(10); test sem

	pthread_mutex_lock(&garage_TGV_oe_mut);
	if(nb_TGV_oe_garage!=0)pthread_cond_wait(&garage_TGV_oe_cond,&garage_TGV_oe_mut);
	pthread_mutex_unlock(&garage_TGV_oe_mut);

	pthread_mutex_lock(&entrer_TGV_eo_mut);
	if(nb_TGV_eo_entrer!=0)pthread_cond_wait(&entrer_TGV_eo_cond,&entrer_TGV_eo_mut);
	pthread_mutex_unlock(&entrer_TGV_eo_mut);	


	pthread_mutex_lock(&P2);	
	pthread_mutex_lock(&P3);
	
	printf(" %28s%d %s\n","GL-->",num,"départ Garage");
	
	pthread_mutex_lock(&garage_GL_oe_mut);
	nb_GL_oe_garage--;
	if(nb_GL_oe_garage==0)pthread_cond_broadcast(&garage_GL_oe_cond);
	pthread_mutex_unlock(&garage_GL_oe_mut);	

	sem_post(&sem_garageGL);

	
	printf(" %53s%d %s\n","GL-->",num,"dans Tunnel");
	sleep(1);
	printf(" %83s%d %s\n","GL-->",num,"sort");
	sleep(1);
	pthread_mutex_unlock(&P2);
	pthread_mutex_unlock(&P3);

	pthread_mutex_lock(&sensGL_mut);
	
	sensGL--;
	if (sensGL==0) pthread_cond_broadcast(&GL_eo_cond);

	pthread_mutex_unlock(&sensGL_mut);
	
	pthread_exit(NULL);

}

void *GL_eo( )
{
	pthread_mutex_lock(&sensGL_mut);
	
	if (sensGL>0) pthread_cond_wait(&GL_eo_cond,&sensGL_mut);
	sensGL--;
	pthread_mutex_unlock(&sensGL_mut);	

	pthread_mutex_lock(&entrer_GL_eo_mut);
	nb_GL_eo_entrer++;
	pthread_mutex_unlock(&entrer_GL_eo_mut);

	pthread_mutex_lock(&num_mut);
	int num=numtrain;
	numtrain++;
	pthread_mutex_unlock(&num_mut);

	

	pthread_mutex_lock(&entrer_TGV_eo_mut);
	if(nb_TGV_eo_entrer!=0)pthread_cond_wait(&entrer_TGV_eo_cond,&entrer_TGV_eo_mut);
	pthread_mutex_unlock(&entrer_TGV_eo_mut);	

	pthread_mutex_lock(&garage_TGV_oe_mut);
	if(nb_TGV_oe_garage!=0)pthread_cond_wait(&garage_TGV_oe_cond,&garage_TGV_oe_mut);
	pthread_mutex_unlock(&garage_TGV_oe_mut);

	
	
	
		
	sem_wait(&sem_garageGL);
	pthread_mutex_lock(&P2);	
	pthread_mutex_lock(&P3);
	
	printf(" %83s%d %s\n","GL<--",num,"départ");
	sleep(1);
	printf(" %53s%d %s\n","GL<--",num,"dans Tunnel");
	sleep(1);
	
	pthread_mutex_lock(&garage_GL_eo_mut);
	nb_GL_eo_garage++;
	pthread_mutex_unlock(&garage_GL_eo_mut);
	printf(" %28s%d %s\n","GL<--",num,"arrive Garage");

	
	pthread_mutex_lock(&entrer_GL_eo_mut);
	nb_GL_eo_entrer--;
	if(nb_GL_eo_entrer==0)pthread_cond_broadcast(&entrer_GL_eo_cond);
	pthread_mutex_unlock(&entrer_GL_eo_mut);	

	sleep(1);
	
	//sleep(10); test sem
	pthread_mutex_unlock(&P2);
	pthread_mutex_unlock(&P3);

	pthread_mutex_lock(&entrer_TGV_oe_mut);
	if(nb_TGV_oe_entrer!=0)pthread_cond_wait(&entrer_TGV_oe_cond,&entrer_TGV_oe_mut);
	pthread_mutex_unlock(&entrer_TGV_oe_mut);

	pthread_mutex_lock(&garage_TGV_eo_mut);
	if(nb_TGV_eo_garage!=0)pthread_cond_wait(&garage_TGV_oe_cond,&garage_TGV_eo_mut);
	pthread_mutex_unlock(&garage_TGV_eo_mut);

	pthread_mutex_lock(&P0);	
	pthread_mutex_lock(&P1);


	printf(" %28s%d %s\n","GL<--",num,"départ Garage");

	pthread_mutex_lock(&garage_GL_eo_mut);
	nb_GL_eo_garage--;
	if(nb_GL_eo_garage==0)pthread_cond_broadcast(&garage_GL_eo_cond);
	pthread_mutex_unlock(&garage_GL_eo_mut);
	
	sem_post(&sem_garageGL);

	printf ("GL<--%d attend\n",num);
	sleep(1);
	printf("GL<--%d sort\n",num);
	sleep(1);
	pthread_mutex_unlock(&P0);
	pthread_mutex_unlock(&P1);
	
	
	
	pthread_mutex_lock(&sensGL_mut);
	sensGL++;
	if (sensGL==0) pthread_cond_broadcast(&GL_oe_cond);
	pthread_mutex_unlock(&sensGL_mut);

	pthread_exit(NULL);

}

void *M_oe( )
{
	pthread_mutex_lock(&num_mut);
	int num=numtrain;	
	numtrain++;
	pthread_mutex_unlock(&num_mut);	
	
	pthread_mutex_lock(&entrer_TGV_oe_mut);
	if(nb_TGV_oe_entrer!=0)pthread_cond_wait(&entrer_TGV_oe_cond,&entrer_TGV_oe_mut);
	pthread_mutex_unlock(&entrer_TGV_oe_mut);

	pthread_mutex_lock(&garage_TGV_eo_mut);
	if(nb_TGV_eo_garage!=0)pthread_cond_wait(&garage_TGV_eo_cond,&garage_TGV_eo_mut);
	pthread_mutex_unlock(&garage_TGV_eo_mut);

	pthread_mutex_lock(&entrer_GL_oe_mut);
	if(nb_GL_oe_entrer!=0)pthread_cond_wait(&entrer_GL_oe_cond,&entrer_GL_oe_mut);
	pthread_mutex_unlock(&entrer_GL_oe_mut);

	pthread_mutex_lock(&garage_GL_eo_mut);
	if(nb_GL_eo_garage!=0)pthread_cond_wait(&garage_GL_eo_cond,&garage_GL_eo_mut);
	pthread_mutex_unlock(&garage_GL_eo_mut);
	
	
	
	
	sem_wait(&sem_garageM_oe);
	pthread_mutex_lock(&P1);
	
	printf("M-->%d départ Gare\n",num);
	sleep(1);
	printf(" %28s%d %s\n","M-->",num,"arrive Garage");
	
	pthread_mutex_unlock(&P1);
	//sleep(10); test sem
	pthread_mutex_lock(&garage_TGV_oe_mut);
	if(nb_TGV_oe_garage!=0)pthread_cond_wait(&garage_TGV_oe_cond,&garage_TGV_oe_mut);
	pthread_mutex_unlock(&garage_TGV_oe_mut);

	pthread_mutex_lock(&entrer_TGV_eo_mut);
	if(nb_TGV_eo_entrer!=0)pthread_cond_wait(&entrer_TGV_eo_cond,&entrer_TGV_eo_mut);
	pthread_mutex_unlock(&entrer_TGV_eo_mut);

	pthread_mutex_lock(&garage_GL_oe_mut);
	if(nb_GL_oe_garage!=0)pthread_cond_wait(&garage_GL_oe_cond,&garage_GL_oe_mut);
	pthread_mutex_unlock(&garage_GL_oe_mut);

	pthread_mutex_lock(&entrer_GL_eo_mut);
	if(nb_GL_eo_entrer!=0)pthread_cond_wait(&entrer_GL_eo_cond,&entrer_GL_eo_mut);
	pthread_mutex_unlock(&entrer_GL_eo_mut);

	pthread_mutex_lock(&P2);	
	pthread_mutex_lock(&P3);
	printf(" %28s%d %s\n","M-->",num,"départ Garage");
	
	sem_post(&sem_garageM_oe);
	sleep(1);
	
	printf(" %53s%d %s\n","M-->",num,"dans Tunnel");
	sleep(1);
	printf(" %83s%d %s\n","M-->",num,"sort");
	sleep(1);
	pthread_mutex_unlock(&P2);
	pthread_mutex_unlock(&P3);
	
	pthread_exit(NULL);

}


void *M_eo( )
{
	pthread_mutex_lock(&num_mut);
	int num=numtrain;	
	numtrain++;
	pthread_mutex_unlock(&num_mut);	

	pthread_mutex_lock(&entrer_TGV_eo_mut);
	if(nb_TGV_eo_entrer!=0)pthread_cond_wait(&entrer_TGV_eo_cond,&entrer_TGV_eo_mut);
	pthread_mutex_unlock(&entrer_TGV_eo_mut);	

	pthread_mutex_lock(&garage_TGV_oe_mut);
	if(nb_TGV_oe_garage!=0)pthread_cond_wait(&garage_TGV_oe_cond,&garage_TGV_oe_mut);
	pthread_mutex_unlock(&garage_TGV_oe_mut);

	pthread_mutex_lock(&entrer_GL_eo_mut);
	if(nb_GL_eo_entrer!=0)pthread_cond_wait(&entrer_GL_eo_cond,&entrer_GL_eo_mut);
	pthread_mutex_unlock(&entrer_GL_eo_mut);	

	pthread_mutex_lock(&garage_GL_oe_mut);
	if(nb_GL_oe_garage!=0)pthread_cond_wait(&garage_GL_oe_cond,&garage_GL_oe_mut);
	pthread_mutex_unlock(&garage_GL_oe_mut);

		

	
	
	sem_wait(&sem_garageM_eo);
	pthread_mutex_lock(&P2);	
	pthread_mutex_lock(&P3);	
	
	printf(" %83s%d %s\n","M<--",num,"départ");
	sleep(1);
	printf(" %53s%d %s\n","M<--",num,"dans Tunnel");
	sleep(1);
	printf(" %28s%d %s\n","M<--",num,"arrive Garage");
	sleep(1);
	pthread_mutex_unlock(&P2);
	pthread_mutex_unlock(&P3);
	//sleep(10); test sem

	pthread_mutex_lock(&entrer_TGV_oe_mut);
	if(nb_TGV_oe_entrer!=0)pthread_cond_wait(&entrer_TGV_oe_cond,&entrer_TGV_oe_mut);
	pthread_mutex_unlock(&entrer_TGV_oe_mut);
	
	pthread_mutex_lock(&garage_TGV_eo_mut);
	if(nb_TGV_eo_garage!=0)pthread_cond_wait(&garage_TGV_oe_cond,&garage_TGV_eo_mut);
	pthread_mutex_unlock(&garage_TGV_eo_mut);

	pthread_mutex_lock(&entrer_GL_oe_mut);
	if(nb_GL_oe_entrer!=0)pthread_cond_wait(&entrer_GL_oe_cond,&entrer_GL_oe_mut);
	pthread_mutex_unlock(&entrer_GL_oe_mut);
	
	pthread_mutex_lock(&garage_GL_eo_mut);
	if(nb_GL_eo_garage!=0)pthread_cond_wait(&garage_GL_oe_cond,&garage_GL_eo_mut);
	pthread_mutex_unlock(&garage_GL_eo_mut);
	
	pthread_mutex_lock(&P1);
	
	printf(" %28s%d %s\n","M<--",num,"départ Garage");
	sem_post(&sem_garageM_eo);
	sleep(1);
	printf("M<--%d sort\n",num);
	pthread_mutex_unlock(&P1);
	pthread_exit(NULL);

}






void thread_TGV_oe(void)
{
        	
	int temp;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	struct sched_param param;
	param.sched_priority = 1;
	pthread_attr_setschedpolicy(&attr,policy);
	pthread_attr_setschedparam (&attr, &param);

                
        if((temp = pthread_create(&train[count],&attr, TGV_oe, NULL)) != 0)      
                printf("echec de la création  !\n");
 	       else
	{
		pthread_attr_destroy(&attr);
	        count++;
		nb_TGV_oe--;
        }
	
}


void thread_TGV_eo(void)
{
        int temp;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	struct sched_param param;
	param.sched_priority = 1;
	pthread_attr_setschedpolicy(&attr,policy);
	pthread_attr_setschedparam (&attr, &param);

        if((temp = pthread_create(&train[count], &attr, TGV_eo, NULL)) != 0)      
                printf("echec de la création  !\n");
 	       else
	{
	       pthread_attr_destroy(&attr);
		count++;
		nb_TGV_eo--;
        }
	
}


void thread_GL_oe(void)
{
        	
	int temp;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	struct sched_param param;
	param.sched_priority = 50;
	pthread_attr_setschedpolicy(&attr,policy);
	pthread_attr_setschedparam (&attr, &param);

                
        if((temp = pthread_create(&train[count],&attr, GL_oe, NULL)) != 0)      
                printf("echec de la création  !\n");
 	       else
	{
		pthread_attr_destroy(&attr);
	        count++;
		nb_GL_oe--;
        }
	
}


void thread_GL_eo(void)
{
        int temp;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	struct sched_param param;
	param.sched_priority = 50;
	
	pthread_attr_setschedpolicy(&attr,policy);
	pthread_attr_setschedparam (&attr, &param);
	
                
        if((temp = pthread_create(&train[count], &attr, GL_eo, NULL)) != 0)      
                printf("echec de la création  !\n");
 	       else
	{
	       pthread_attr_destroy(&attr);
		count++;
		nb_GL_eo--;
        }
	
}

void thread_M_eo(void)
{
        int temp;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	struct sched_param param;
	param.sched_priority = 99;
	
	pthread_attr_setschedpolicy(&attr,policy);
	pthread_attr_setschedparam (&attr, &param);
	
                
        if((temp = pthread_create(&train[count], &attr, M_eo, NULL)) != 0)      
                printf("echec de la création  !\n");
 	       else
	{
	       pthread_attr_destroy(&attr);
		count++;
		nb_M_eo--;
        }
	
}

void thread_M_oe(void)
{
        int temp;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	struct sched_param param;
	param.sched_priority = 99;
	
	pthread_attr_setschedpolicy(&attr,policy);
	pthread_attr_setschedparam (&attr, &param);
	
                
        if((temp = pthread_create(&train[count], &attr, M_oe, NULL)) != 0)      
                printf("echec de la création  !\n");
 	       else
	{
	       pthread_attr_destroy(&attr);
		count++;
		nb_M_oe--;
        }
	
}

void ajouter( )
{
	//printf("la fonction ajouter marche\n");	
	nb_random=rand() % 6;			
	sleep(5);			
	switch(nb_random)
	{
		case 0:  nb_TGV_eo++;
				
				break;
		case 1:  nb_TGV_oe++;
				
				break;
		case 2:  nb_GL_eo++;
				
				break;
		case 3:  nb_GL_oe++;
				
				break;
		case 4:  nb_M_eo++;
				
				break;
		case 5:  nb_M_oe++;
				
				break;
		defaut: printf("wrong");
				break;				
	}
}

void initialiser()
{
	sem_init(&sem_garageTGV, 0, 3);
	sem_init(&sem_garageGL, 0, 3);
	sem_init(&sem_garageM_oe,0,3);
	sem_init(&sem_garageM_eo,0,3);	

	printf("Saisir le nombre de TGV de sens Ouest--->Est (entre 0 et 20)\n");
	scanf("%d",&nb_TGV_oe);
	printf("Saisir le nombre de TGV de sens Est--->Ouest (entre 0 et 20)\n");
	scanf("%d",&nb_TGV_eo);
	printf("Saisir le nombre de GL de sens Ouest--->Est (entre 0 et 20)\n");
	scanf("%d",&nb_GL_oe);
	printf("Saisir le nombre de GL de sens Est--->Ouest (entre 0 et 20)\n");
	scanf("%d",&nb_GL_eo);
	printf("Saisir le nombre de M de sens Ouest--->Est (entre 0 et 20)\n");
	scanf("%d",&nb_M_oe);
	printf("Saisir le nombre de M de sens Est--->Ouest (entre 0 et 20)\n");
	scanf("%d",&nb_M_eo);
	printf("Voulez vous ajouter les nouveux trains aléatoires? (1=oui 0=non)\n");
	scanf("%d",&ajout);
}	

