#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>

struct vueloType
{
    
    int originId;
    int destinationId;
    int hour;
    

};

int main(){

    sleep(5);

    int originId;
    int destinationId;
    int hour;

	struct vueloType *vuelo;
	

	//la memoria compartida del menú no está funcionando aún
	//porque los procesos no se están ejecutando en simultaneo aún


  key_t Clave = 123;
  

	int Id_Memoria;
	int *Memoria = NULL;
	
	int i,j;
	char *path ="./buscador.c";
	
	


//La clave igual al del buscador

	//Clave = ftok("vueloGuardado.dat", 33);
	if (Clave == -1)
	{
	printf("No Consigo clave para memoria compartida");
	exit(0);
	}

	Id_Memoria = shmget(Clave, sizeof(struct vueloType)+sizeof(int),0666 | IPC_CREAT);
	if(Id_Memoria == -1)
	{
	printf("No Consigo id para memoria compartida");
	exit(0);
	}
	

/*	
	for(i=0; i<10; i++)
	{
	printf("Leido",Memoria[0]);
	sleep(1);
	}
*/





/*
int cargar(void *Memoria)
{
	struct vuelo *datos;
	datos = Memoria;	
}
*/
    int choice;
    
    do {
	    printf("Menu:\n\n");
      	printf("1. Ingresar origen\n");
    	printf("2. Ingresar destino\n");
    	printf("3. Ingresar hora\n");
    	printf("4. Buscar tiempo de viaje medio\n");
    	printf("5. Salir\n");
    	
    	scanf("%d",&choice);
  
	    switch (choice){
        	case 1: 
	            printf("Ingrese ID del origen: \n");
	            scanf("%d",&originId);
	            if(originId<1 ){
	                printf("El id ingresado es menor a 1, porfavor, ingrese otro valor\n");
	            }else if(originId>1160){
	                printf("El id ingresado es mayor a 1160, porfavor, ingrese otro valor\n");
	            }
    	        break;
	        case 2: 
    	    	printf("Ingrese ID del destino: \n");
        	    scanf("%d",&destinationId);
	            if(destinationId<1 ){
    	            printf("El id ingresado es menor a 1, porfavor, ingrese otro valor\n");
	            }else if(destinationId>1160){
    	            printf("El id ingresado es mayor a 1160, porfavor, ingrese otro valor\n");
	            }
    	        break;
	        case 3: 
    	    	printf("Ingrese hora del día: \n");
	            scanf("%d",&hour);
    	        if(hour<0 ){
	                printf("La hora es menor a 0, porfavor, ingrese otro valor\n");
	            }else if(hour>23){
    	            printf("La hora es mayor a 23, porfavor, ingrese otro valor\n");
	            }
    	        break;
	      	case 4:
	      	printf("El tiempo de vuelo medio es: \n");
	     	
	        Memoria = (int *) shmat(Id_Memoria, NULL,0);
		if (Memoria == NULL)
		{
		printf("No Consigo memoria compartida");
		exit(0);
		
		}
		
		//dprintf(Memoria,"%d : %d : %d", originId,destinationId,hour)
		
		
		shmdt((char *)Memoria);
		shmctl(Id_Memoria, IPC_RMID, (struct shmid_ds *)NULL);
		
			
		printf("%d\n",originId);
		printf("%d\n",destinationId);
		printf("%d\n",hour);
		
		
        
    	    	break;
		    case 5:
	        	printf("Bye\n");
	        	break;
	        default: printf("Invalid choice!\n\n\n");
            break;
	    } 
	    
 
    }while (choice != 5);
    
    
	

}
