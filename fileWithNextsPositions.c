
/**
 * Genera HashTable y LinkedList
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


char sourcePath[100];
char destPath[100];
char hashPath[100];
FILE *sourceFile;
FILE *destFile;
FILE *hashTableFile;

struct Viaje {
   int origin;   
   int destine;
   //hod es hora
   int hod;
   double meanTravel;
};

struct Raw{
	int cabeza;
	int cola;
};

struct HashTable{
	struct Raw arreglo[1160];
};

bool createTrip(struct Viaje* viaje, FILE* sourceFile){
	bool lectura = true;
    char* separadores = (char*) malloc(11*sizeof(char));

    if(fscanf(sourceFile, "%d%c", &viaje->origin, separadores) == EOF){
        printf("el numero 1");
        lectura = false;
    }
    if(fscanf(sourceFile, "%d%c", &viaje->destine, separadores) == EOF){
        printf("el numero 2");
        lectura = false;
    }
    if(fscanf(sourceFile, "%d%c", &viaje->hod, separadores) == EOF){
        printf("el numero 3");
        lectura = false;
    }
    if(fscanf(sourceFile, "%lf%c", &viaje->meanTravel, separadores) == EOF){
        printf("el numero 4");
        lectura = false;
    }

    //Este fscanf lo que hace es saltarse esos datos restantes de la fila que no necesitamos
    fscanf(sourceFile, "%[^\n]", separadores);
    free(separadores);
    return lectura;
}

void readTitle(FILE* file){
    char* file_content = (char*) malloc(135*sizeof(char));
    if(file_content == NULL){
        printf("Error en la asignación de memoria del encabezado\n");
        exit(-1);
    }
    fscanf(file, "%[^\n]", file_content);
    for(int i = 0; i < 135; ++i){
        printf("%c", *(file_content+i));
    }
    free(file_content);
}


void insertarDato(FILE* hashTableFIle,FILE* destiFile,struct Viaje* viaje, struct HashTable* hashTable){
	//fseek lo que hace es ubicarnos en el archivo en cualquier parte que queramos
    fseek(destiFile,0,SEEK_END);
    //ftell nos indica en dónde estamos en el archivo actualmente
	int currentIndex = ftell(destFile);
	int next = -1;

    //Escribiendo en la linked list
	fwrite(&viaje->origin, sizeof(int), 1, destiFile);
	fwrite(&viaje->destine, sizeof(int), 1, destiFile);
	fwrite(&viaje->hod, sizeof(int), 1, destiFile);
	fwrite(&viaje->meanTravel, sizeof(double), 1, destiFile);
	fwrite(&next, sizeof(int), 1, destiFile);
	
    //Escribiendo en el archivo HashTable
    //Verificamos si ya habiamos visto el origen de este viaje
	if(hashTable->arreglo[viaje->origin-1].cabeza == -1){
		fseek(hashTableFile, 2*sizeof(int)*(viaje->origin-1),SEEK_SET);
		hashTable->arreglo[viaje->origin-1].cabeza = currentIndex;
		hashTable->arreglo[viaje->origin-1].cola = currentIndex;
		//fwrite y fread se le pasan posiciones de memoria, para decirle solo dónde está escrito, es más barato en memoria
        //Escribimos en la HashTable el IdOrigen y la primera posición donde se encuentre
		fwrite(&viaje->origin, sizeof(int), 1, hashTableFIle);
		fwrite(&currentIndex, sizeof(int), 1, hashTableFIle);
	}else{
		//SEEK_SET, SEEK_END Y SEEK_CUR
        //Nos ubicamos en la línea donde habíamos encontrado el último viaje con este origen
		fseek(destiFile, hashTable->arreglo[viaje->origin - 1].cola + (3*sizeof(int)) + sizeof(double), SEEK_SET);
		// el 1 es le numero de sizeof que voy a escribir
		fwrite(&currentIndex, sizeof(int), 1, destiFile);
		hashTable->arreglo[viaje->origin - 1].cola = currentIndex;
	}
}

//Función para dormir un proceso usleep()

//ftell() nos indica en dónde estamos en el archivo

int main(){

    /* Input path of files to copy */
    /*
    printf("Enter source file path: ");
    scanf("%s", sourcePath);
    printf("Enter destination linked file path: ");
    scanf("%s", destPath);
    printf("Enter destination hash file path: ");
    scanf("%s", hashPath);*/

    /* 
     * Open source file in 'r' and 
     * destination file in 'w' mode 
     */
    sourceFile  = fopen("./bogota1.dat", "rb");
    //DOnde se guarda la linked list
    destFile    = fopen("./file.dat",   "w+");
    hashTableFile    = fopen("./hash.dat",   "w+");

	if (hashTableFile == NULL){
        /* Unable to open file hence exit */
        printf("\nUnable to open hash file.\n");
        printf("Please check if file exists and you have read/write privilege.\n");

        exit(EXIT_FAILURE);
    }
	
	
    /* fopen() return NULL if unable to open file in given mode. */
    if (sourceFile == NULL || destFile == NULL)
    {
        /* Unable to open file hence exit */
        printf("\nUnable to open file.\n");
        printf("Please check if file exists and you have read/write privilege.\n");

        exit(EXIT_FAILURE);
    }

    //Se salta la primera línea que tiene la descripción de cada campo
    readTitle(sourceFile);

    
    struct HashTable* hashTable = (struct HashTable*) malloc(sizeof(struct HashTable));

    //origen1, primeraposicionde ese origen

    //Este solo inicializa la hashTable
    for(int o = 1; o <= 1160; ++o){
    	hashTable->arreglo[o-1].cabeza = -1;
    	hashTable->arreglo[o-1].cola = -1;
    }
    
    //EMpezamos a leer el archivo orginal
    //la funcion feof es para si ya llegó al final del archivo
    while(!feof(sourceFile)){

    	struct Viaje* viaje = (struct Viaje*) malloc(sizeof(struct Viaje));
	    if(!createTrip(viaje, sourceFile)){
		    //printf("\n Unable to read travel");
            exit(-1);            
	    }
	    insertarDato(hashTableFile, destFile,viaje, hashTable);			    	
        free(viaje);
    }     
    fclose(sourceFile);
    fclose(destFile);
    fclose(hashTableFile);
}
