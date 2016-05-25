#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int count=0;
int numtrain=0;
int nb_tgv_oe=5;

pthread_t tgv[5];
pthread_mutex_t tunnel;
pthread_mutex_t num_mut;
pthread_mutex_t voie_a2;
pthread_mutex_t voie;
pthread_mutex_t garageTGV;
pthread_mutex_t voie_c;



void *tgv_oe( )
{
	int num=numtrain;	
	pthread_mutex_lock(&num_mut);
	numtrain++;
	pthread_mutex_unlock(&num_mut);
	
	pthread_mutex_lock(&voie_c);
	pthread_mutex_lock(&voie_a2);
	pthread_mutex_lock(&garageTGV);
	printf ("tgv %d est entré et attend dans la gare\n",num);
	sleep(1);
	printf("tgv %d est parti vers la garage\n",num);
	
	sleep(1);
	printf("tgv %d est dans la garage\n",num);
	
	pthread_mutex_lock(&tunnel);
	pthread_mutex_lock(&voie);
	
	
	printf("tgv %d est parti travers le tunnel\n",num);
	
	pthread_mutex_unlock(&voie_c);	
	pthread_mutex_unlock(&voie_a2);
	pthread_mutex_unlock(&garageTGV);
	sleep(1);
	
	printf("tgv %d est sorti la gare est\n",num);
	pthread_mutex_unlock(&tunnel);
	pthread_mutex_unlock(&voie);
	if (num==0)	pthread_exit(NULL);
	else 
	pthread_join(tgv[num--],NULL);
}

void thread_TGV_oe(void)
{
        int temp;
                
        if((temp = pthread_create(&tgv[count], NULL, tgv_oe, NULL)) != 0)       //comment2
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
	pthread_mutex_init(&voie,NULL);
	pthread_mutex_init(&garageTGV,NULL);
	pthread_mutex_init(&voie_c,NULL);
	pthread_mutex_init(&voie_a2,NULL);
	pthread_mutex_init(&tunnel,NULL);
	
        printf("le projet commence\n");
	while(nb_tgv_oe>0)         
	{
		thread_TGV_oe();
	}
	pthread_join(tgv[4],NULL);
        printf("projet termine\n");
        return 0;
}
