#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int count=0;
int numtrain=0;
int nb_tgv_oe=5;

pthread_t tgv[5];
pthread_mutex_t P0;
pthread_mutex_t num_mut;
pthread_mutex_t P1;
pthread_mutex_t P2;
pthread_mutex_t P3;



void *tgv_oe( )
{
	int num=numtrain;	
	pthread_mutex_lock(&num_mut);
	numtrain++;
	pthread_mutex_unlock(&num_mut);
	
	pthread_mutex_lock(&P0);
	pthread_mutex_lock(&P1);
	printf ("TGV_OE%d attend\n",num);
	sleep(1);
	printf("TGV_OE%d départ Gare\n",num);
	sleep(1);
	printf(" %28s%d %s\n","TGV_OE",num,"arrive Garage");
	
	pthread_mutex_unlock(&P0);
	pthread_mutex_unlock(&P1);
	
	
	printf(" %28s%d %s\n","TGV_OE",num,"départ Garage");
	
	pthread_mutex_lock(&P2);	
	pthread_mutex_lock(&P3);
	sleep(1);
	
	printf(" %53s%d %s\n","TGV_OE",num,"dans Tunnel");
	sleep(1);
	printf(" %83s%d %s\n","TGV_OE",num,"sort");
	pthread_mutex_unlock(&P2);
	pthread_mutex_unlock(&P3);
	if (num==0)	pthread_exit(NULL);
	else 
	pthread_join(tgv[num--],NULL);
}


void thread_TGV_oe(void)
{
        int temp;
                
        if((temp = pthread_create(&tgv[count], NULL, tgv_oe, NULL)) != 0)      
                printf("echec de la création  !\n");
 	       else
	{
	       count++;
		nb_tgv_oe--;
       }
	
        
}




int main()
{	

        pthread_mutex_init(&num_mut,NULL);
	pthread_mutex_init(&P0,NULL);
	pthread_mutex_init(&P1,NULL);
	pthread_mutex_init(&P2,NULL);
	pthread_mutex_init(&P3,NULL);
	
	
        printf("%10s%25s%25s%25s\n\n", "Gare", "Garage", "Tunnel","Voie");
	while(nb_tgv_oe>0)         
	{
		thread_TGV_oe();
	}
	pthread_join(tgv[4],NULL);
        printf("projet termine\n");
        return 0;
}
