#include "header.h"

int main()
{	
	initialiser();
	printf("%10s%25s%25s%25s\n\n", "Gare", "Garage", "Tunnel","Voie");
	while(1)         
	{
		if(nb_M_oe>0) thread_M_oe();
		if(nb_M_eo>0) thread_M_eo();	
		if(nb_GL_oe>0) thread_GL_oe();
		if(nb_GL_eo>0) thread_GL_eo();	
		if(nb_TGV_oe>0) thread_TGV_oe();
		if(nb_TGV_eo>0) thread_TGV_eo();
		
		if (count<100&&ajout==1) ajouter();
	}

	pthread_joint(train[100]);
    printf("projet termine\n");
    return 0;
}
