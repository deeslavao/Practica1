#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <unistd.h>

FILE *linkedFile;
FILE *hashFile;
int requestedOrigin;
int requestedDestination;
int requestedHour;

bool isTheRequestedTrip(int origin, int destination, int hour){
  if(origin != requestedOrigin){
    return false;
  }
  if(destination != requestedDestination){
    return false;
  }
  if(hour != requestedHour){
    return false;
  }
  return true;;  
}


struct vueloType
{
    
    int originId;
    int destinationId;
    int hour;
    

};

int main(){
      char *path = "./buscador.c";
      	FILE *data;
	data = fopen("vueloGuardado.dat", "r");
	
    int originId;
    int destinationId;
    int hour;
    
    struct vueloType *vuelo;

    key_t Clave = 123;
	int Id_Memoria;
	int *Memoria = NULL;
//Conseguimos una clave para la memoria compartida

    //Clave = ftok("vueloGuardado.dat", 33);
    //printf("%s \n",path);
    //printf("%d \n",Clave);
	if(Clave == -1)
	{
	//printf("No consigo clave para memoria compartida\n");
	exit(0);
	}
//Creamos la memoria con la clave conseguida

	Id_Memoria = shmget(Clave, sizeof(struct vueloType)+sizeof(int), 0666);
	if (Id_Memoria == -1)
	{
    //printf("no consigo ID para memoria compartida\n");
	exit(0);
	}
//Ya creada la memoria compartida apuntamos hacia ella con un puntero 
	Memoria = (int*)shmat (Id_Memoria, (int *)0, 0);
	if (Memoria == NULL)
	{
	//printf("No consigo memoria compartida\n");
	exit(0);
	}
	
	//printf("%ls",Memoria);
	
	fseek(data,0, SEEK_SET);
	fread(&vuelo->originId, sizeof(int), 1, data);
	fread(&vuelo->destinationId, sizeof(int), 1, data);
	fread(&vuelo->hour, sizeof(int), 1, data);
	//printf("%d - %d - %d", originId, destinationId, hour);
	fclose(data);	

/*  
	for (int i=0; i<10; i++)
	{
		for (int j=0; j<100; j++)
		{
		Memoria[j]=i;
		}
		printf("Escrito %d", i);
		sleep (1);
	}
*/	


//Termina de usar la memoria y la libera
/*
    	if(Id_Memoria != -1)
	{
	shmdt((char *)Memoria);
	}
*/

	int currentOriginInHash;
  int currentFirst;

  int currentOrigin;
  int currentDestination;
  int currentHour;
  double currentMeanTime;
  int currentNext;
  int previousNext;
  

  requestedOrigin = 583;
  requestedDestination = 836;
  requestedHour = 15;

  linkedFile = fopen("./file.dat",   "rb");
  hashFile = fopen("./hash.dat",   "rb");
  
  if (linkedFile == NULL || hashFile == NULL){
      /* Unable to open file hence exit */
     // printf("\nUnable to open file.\n");
      //printf("Please check if file exists and you have read/write privilege.\n");
      exit(EXIT_FAILURE);
  }

  //Generamos un identificador para el proceso actual
  key_t key = ftok("buscador",65);
  //Evaluamos que no haya ocurrido un error
  if(key == -1){
    perror("Error en la creación del identificador del proceso");
    exit(EXIT_FAILURE);
  }

  //Generamos la memoria compartida y el identificador
    int shmid = shmget(key,1024,0666|IPC_CREAT);
	if(shmid == -1)
	{
	//printf("No Consigo id para memoria compartida\n");
	exit(0);
	}
  //NOs ubicamos en la fila del origen ingresado
  fseek(hashFile, 2*sizeof(int)*(requestedOrigin-1),SEEK_SET);
  fread(&currentOriginInHash, sizeof(int), 1, hashFile);
  fread(&currentFirst, sizeof(int), 1, hashFile);
  
 // printf("%d - %d \n", currentOriginInHash, currentFirst);

  // + (3*sizeof(int)) + sizeof(double)
  fseek(linkedFile, currentFirst , SEEK_SET);
  fread(&currentOrigin, sizeof(int), 1, linkedFile);
  fread(&currentDestination, sizeof(int), 1, linkedFile);
  fread(&currentHour, sizeof(int), 1, linkedFile);
  fread(&currentMeanTime, sizeof(double), 1, linkedFile);
  fread(&currentNext, sizeof(int), 1, linkedFile);

 // printf("%d - %d - %d - %f - %d\n", currentOrigin, currentDestination, currentHour, currentMeanTime, currentNext);  
  
  bool isThereOtherTrip = (!(currentFirst == currentNext)) ;

  while(!isTheRequestedTrip(currentOrigin, currentDestination, currentHour) && isThereOtherTrip){
    previousNext = currentNext;
    fseek(linkedFile, currentNext , SEEK_SET);
    if(fread(&currentOrigin, sizeof(int), 1, linkedFile) != 1){
   //   printf("Reached end of file, there is no trip with such attributes1");
      break;
    }else if(fread(&currentDestination, sizeof(int), 1, linkedFile) != 1){
   //   printf("Reached end of file, there is no trip with such attributes2");
      break;
    }else if(fread(&currentHour, sizeof(int), 1, linkedFile) != 1){
   //   printf("Reached end of file, there is no trip with such attributes3");
      break;
    }else if(fread(&currentMeanTime, sizeof(double), 1, linkedFile) != 1){
   //   printf("Reached end of file, there is no trip with such attributes4");
      break;
    }else if(fread(&currentNext, sizeof(int), 1, linkedFile) != 1){
  //    printf("Reached end of file, there is no trip with such attributes5");
      break;
    }
  }

  if(isTheRequestedTrip(currentOrigin, currentDestination, currentHour)){
   printf("El tiempo medio del viaje ingresado es %f \n", currentMeanTime);
  }

  fclose(linkedFile);
  fclose(hashFile);
}
