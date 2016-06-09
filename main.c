#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int count=0;
int numtrain=0;
int policy=SCHED_RR;



int nb_TGV_oe=5;
int nb_TGV_eo=5;
int sensTGV=0;

int nb_GL_oe=5;
int nb_GL_eo=5;
int sensGL=0;





pthread_t train[100];

pthread_mutex_t P0=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t P1=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t P2=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t P3=PTHREAD_MUTEX_INITIALIZER;


pthread_mutex_t garageTGV_mut=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sensTGV_mut=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t   TGV_oe_cond=PTHREAD_COND_INITIALIZER;
pthread_cond_t   TGV_eo_cond=PTHREAD_COND_INITIALIZER;

pthread_mutex_t garageGL_mut=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sensGL_mut=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t   GL_oe_cond=PTHREAD_COND_INITIALIZER;
pthread_cond_t   GL_eo_cond=PTHREAD_COND_INITIALIZER;



pthread_mutex_t num_mut=PTHREAD_MUTEX_INITIALIZER;


void *TGV_oe( )
{
	pthread_mutex_lock(&num_mut);
	int num=numtrain;	
	numtrain++;
	pthread_mutex_unlock(&num_mut);
	
	pthread_mutex_lock(&sensTGV_mut);
	
	if (sensTGV<0) pthread_cond_wait(&TGV_oe_cond,&sensTGV_mut);
	sensTGV++;
	
	pthread_mutex_unlock(&sensTGV_mut);
	
	
	pthread_mutex_lock(&P1);
	pthread_mutex_lock(&P0);
	pthread_mutex_lock(&garageTGV_mut);
	printf ("TGV-->%d attend\n",num);
	sleep(1);
	printf("TGV-->%d départ Gare\n",num);
	sleep(1);
	printf(" %28s%d %s\n","TGV-->",num,"arrive Garage");
	
	
	pthread_mutex_unlock(&P1);
	pthread_mutex_unlock(&P0);
	
	printf(" %28s%d %s\n","TGV-->",num,"départ Garage");
	pthread_mutex_unlock(&garageTGV_mut);
	
	pthread_mutex_lock(&P2);	
	pthread_mutex_lock(&P3);
	sleep(1);
	
	printf(" %53s%d %s\n","TGV-->",num,"dans Tunnel");
	sleep(1);
	printf(" %83s%d %s\n","TGV-->",num,"sort");
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
	pthread_mutex_lock(&num_mut);
	int num=numtrain;
	numtrain++;
	pthread_mutex_unlock(&num_mut);
	
	pthread_mutex_lock(&sensTGV_mut);
	
	if (sensTGV>0) pthread_cond_wait(&TGV_eo_cond,&sensTGV_mut);
	
	sensTGV--;
	
	pthread_mutex_unlock(&sensTGV_mut);
	
	
	
	pthread_mutex_lock(&P2);	
	pthread_mutex_lock(&P3);
	pthread_mutex_lock(&garageTGV_mut);
	printf(" %83s%d %s\n","TGV<--",num,"départ");
	sleep(1);
	printf(" %53s%d %s\n","TGV<--",num,"dans Tunnel");
	sleep(1);
	printf(" %28s%d %s\n","TGV<--",num,"arrive Garage");
	pthread_mutex_unlock(&P2);
	pthread_mutex_unlock(&P3);
	
	pthread_mutex_lock(&P1);
	pthread_mutex_lock(&P0);
	
	printf(" %28s%d %s\n","TGV<--",num,"départ Garage");
	pthread_mutex_unlock(&garageTGV_mut);
	sleep(1);
	printf ("TGV<--%d attend\n",num);
	sleep(1);
	printf("TGV<--%d départ Gare\n",num);
	pthread_mutex_unlock(&P1);	
	pthread_mutex_unlock(&P0);

	pthread_mutex_lock(&sensTGV_mut);
	
	sensTGV++;
	if (sensTGV==0) pthread_cond_broadcast(&TGV_oe_cond);
	pthread_mutex_unlock(&sensTGV_mut);

	pthread_exit(NULL);

}

void *GL_oe( )
{
	pthread_mutex_lock(&num_mut);
	int num=numtrain;	
	numtrain++;
	pthread_mutex_unlock(&num_mut);
	
	pthread_mutex_lock(&sensGL_mut);
	
	if (sensGL<0) pthread_cond_wait(&GL_oe_cond,&sensGL_mut);
	sensGL++;
	
	pthread_mutex_unlock(&sensGL_mut);
	
	pthread_mutex_lock(&P1);
	pthread_mutex_lock(&P0);
	pthread_mutex_lock(&garageGL_mut);
	printf ("GL-->%d attend\n",num);
	sleep(1);
	printf("GL-->%d départ Gare\n",num);
	sleep(1);
	printf(" %28s%d %s\n","GL-->",num,"arrive Garage");
	pthread_mutex_unlock(&P1);
	pthread_mutex_unlock(&P0);
	
	
	
	printf(" %28s%d %s\n","GL-->",num,"départ Garage");
	pthread_mutex_unlock(&garageGL_mut);
	
	pthread_mutex_lock(&P2);	
	pthread_mutex_lock(&P3);
	sleep(1);
	
	printf(" %53s%d %s\n","GL-->",num,"dans Tunnel");
	sleep(1);
	printf(" %83s%d %s\n","GL-->",num,"sort");
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
	pthread_mutex_lock(&num_mut);
	int num=numtrain;
	numtrain++;
	pthread_mutex_unlock(&num_mut);
	
	pthread_mutex_lock(&sensGL_mut);
	
	if (sensGL>0) pthread_cond_wait(&GL_eo_cond,&sensGL_mut);
	
	sensGL--;
	
	pthread_mutex_unlock(&sensGL_mut);
	
	
	
	pthread_mutex_lock(&P2);	
	pthread_mutex_lock(&P3);
	pthread_mutex_lock(&garageGL_mut);
	printf(" %83s%d %s\n","GL<--",num,"départ");
	sleep(1);
	printf(" %53s%d %s\n","GL<--",num,"dans Tunnel");
	sleep(1);
	printf(" %28s%d %s\n","GL<--",num,"arrive Garage");
	pthread_mutex_unlock(&P2);
	pthread_mutex_unlock(&P3);
	
	pthread_mutex_lock(&P1);
	pthread_mutex_lock(&P0);
	
	printf(" %28s%d %s\n","GL<--",num,"départ Garage");
	pthread_mutex_unlock(&garageGL_mut);
	sleep(1);
	printf ("GL<--%d attend\n",num);
	sleep(1);
	printf("GL<--%d départ Gare\n",num);
	
	pthread_mutex_unlock(&P1);
	pthread_mutex_unlock(&P0);
	
	pthread_mutex_lock(&sensGL_mut);
	
	sensGL++;
	if (sensGL==0) pthread_cond_broadcast(&GL_oe_cond);
	pthread_mutex_unlock(&sensGL_mut);

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



int main()
{	
	
	
        printf("%10s%25s%25s%25s\n\n", "Gare", "Garage", "Tunnel","Voie");
	while(1)         
	{
		if(nb_GL_oe>0) thread_GL_oe();
		if(nb_GL_eo>0) thread_GL_eo();
		if(nb_TGV_oe>0) thread_TGV_oe();
		if(nb_TGV_eo>0) thread_TGV_eo();
		
	}

        printf("projet termine\n");
        return 0;
}