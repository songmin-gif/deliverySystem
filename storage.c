#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

/* 
  definition of storage cell structure ----
  members :
  int building : building number of the destination
  int room : room number of the destination
  int cnt : number of packages in the cell
  char passwd[] : password setting (4 characters)
  char *contents : package context (message string)
*/
typedef struct {
	int building;
	int room;
	int cnt;
	char passwd[PASSWD_LEN+1];
	
	char *context;
} storage_t;


static storage_t** deliverySystem; 			//deliverySystem
static int storedCnt = 0;					//number of cells occupied
static int systemSize[2] = {0, 0};  		//row/column of the delivery system
static char masterPassword[PASSWD_LEN+1];	//master password


static void printStorageInside(int x, int y) {
	printf("\n------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n");
	if (deliverySystem[x][y].cnt > 0)
		printf("<<<<<<<<<<<<<<<<<<<<<<<< : %s >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", deliverySystem[x][y].context);
	else
		printf("<<<<<<<<<<<<<<<<<<<<<<<< empty >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		
	printf("------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n\n");
}

//initialize the storage
//set all the member variable as an initial value
//and allocate memory to the context pointer
//int x, int y : cell coordinate to be initialized
static void initStorage(int x, int y) {
	deliverySystem[x][y].building = 0;
    deliverySystem[x][y].room = 0;
    strcpy(deliverySystem[x][y].passwd, "login"); // initialize password to "login"
   		deliverySystem[x][y].cnt = 0;
   		deliverySystem[x][y].context = (char*)malloc(sizeof(char)*MAX_MSG_SIZE); // create context malloc  
   	strcpy(deliverySystem[x][y].context, "context"); // initialize context to "context"
	
}

static int inputPasswd(int x, int y) {
   char p[PASSWD_LEN+1];;  //variable p is inputPasswd
   
   printf(" input your password : ");
   scanf("%s",&p);
   
   if( p[PASSWD_LEN+1] == passwd[PASSWD_LEN+1]) // if input passwd and the origianl match
   {
      return 0;
   }

   else // if not 
   {
      return -1; 
   }	
}
// ------- API function for main.c file ---------------

//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(char* filepath) {
	
	FILE *fp;
	int i,j;

   fp = fopen(filepath, "w");
   
   if(fp == NULL) // if file backup is failed
   {
      return -1;
   }
   
   fprintf(fp, "%d %d\n", systemSize[0], systemSize[1]); //row. column
   fprintf(fp, "%s\n", masterPassword); // masterPassword
   
   for(i=0;i<systemSize[0]; i++)
   {
      for(j=0;j<systemSize[1]; j++)
      {
         if(deliverySystem[i][j].cnt > 0) // if deliverySystem[i][j] has a package
         {
            fprintf(fp, "%d %d %d %d %s\n ",i,j,deliverySystem[i][j].building, deliverySystem[i][j].room, deliverySystem[i][j].passwd);
            fprintf(fp, "%s\n",deliverySystem[i][j].context);
         }  
      }
    }
   
   fclose(fp);
   return 0;
}

//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {
   FILE *fp;
   int i,j;
   
    FILE *fp = fopen("filepath", "r"); //filepath
   struct filepath {
      char row[4];
      char column[6];
      char nBuilding;
      char nRoom;
      char passwd;
      char contexts;
	};
   
   void createSystem (void){
   struct filepath *p;
    
   fscanf(fp,"%d %d",&systemSize[0], systemSize[1]);

   deliverySystem = (storage_t**)malloc(sizeof(storage_t*)*systemSize[0]);  //create row malloc
   for(i=0;i<systemSize[0];i++) { 
   deliverySystem[i] = (storage_t*)malloc(sizeof(storage_t)*systemSize[1]); //create column malloc
   
   if (fp==NULL){ // if allocating memory is failed
      printf("error in allocating memory\n");
      return -1;
   }
   fclose(fp);
   return; 
}
}
      fscanf(fp,"%d %d",&i,&j);
      fscanf(fp,"%d ",&deliverySystem[i][j].building,&deliverySystem[i][j].room);
      fscanf(fp,"%s %s",deliverySystem[i][j].passwd,deliverySystem[i][j].context);
   }
}

//free the memory of the deliverySystem 
void str_freeSystem(void) {
	int i;
	int j;
	
	for(i=0;i<systemSize[0];i++) //context free function
	{		
		for(j=0;j<systemSize[1];j++)
			{			
				free(deliverySystem[i][j].context);
			}
	}		
	for(i=0;i<systemSize[0];i++) //deliverySystem free function
		{
			free(deliverySystem[i]); 
		} 
	free(deliverySystem);
	
}
//print the current state of the whole delivery system (which cells are occupied and the destination of the each occupied cells)
void str_printStorageStatus(void) {
	int i, j;
	printf("----------------------------- Delivery Storage System Status (%i occupied out of %i )-----------------------------\n\n", storedCnt, systemSize[0]*systemSize[1]);
	
	printf("\t");
	for (j=0;j<systemSize[1];j++)
	{
		printf(" %i\t\t",j);
	}
	printf("\n-----------------------------------------------------------------------------------------------------------------\n");
	
	for (i=0;i<systemSize[0];i++)
	{
		printf("%i|\t",i);
		for (j=0;j<systemSize[1];j++)
		{
			if (deliverySystem[i][j].cnt > 0)
			{
				printf("%i,%i\t|\t", deliverySystem[i][j].building, deliverySystem[i][j].room);
			}
			else
			{
				printf(" -  \t|\t");
			}
		}
		printf("\n");
	}
	printf("--------------------------------------- Delivery Storage System Status --------------------------------------------\n\n");
}



//check if the input cell (x,y) is valid and whether it is occupied or not
int str_checkStorage(int x, int y) {
	if (x < 0 || x >= systemSize[0])
	{
		return -1;
	}
	
	if (y < 0 || y >= systemSize[1])
	{
		return -1;
	}
	
	return deliverySystem[x][y].cnt;	
}


//put a package (msg) to the cell
//input parameters
//int x, int y : coordinate of the cell to put the package
//int nBuilding, int nRoom : building and room numbers of the destination
//char msg[] : package context (message string)
//char passwd[] : password string (4 characters)
//return : 0 - successfully put the package, -1 - failed to put
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char msg[MAX_MSG_SIZE+1], char passwd[PASSWD_LEN+1]) {
	if(deliverySystem[x][y].cnt == 0) // if storage is empty
	{
		deliverySystem[x][y].building = nBuilding; //input user information in the storage 
		deliverySystem[x][y].room = nRoom;
		deliverySystem[x][y].context = msg;
		for (i=0;i<sizeof(passwd[PASSWD_LEN+1]);i++)
		{
			deliverySystem[x][y].passwd[i] = passwd[i]; // input password
		}
		
		deliverySystem[x][y].cnt=1;
		storedCnt++;
	}
	else
	{
		return -1; //when the storage is not empty 
	}
	return 0;
}

int str_extractStorage(int x, int y) {
	char inputPasswd[PASSWD_LEN+1]; // inputPasswd is the one that user input
	
	printf("input password for (%d, %d) storage :", x, y);
	scanf("%s", inputPasswd);

	if(strcmp(deliverySystem[x][y].passwd, inputPasswd)) // if user input is correct
	{
		printf("extracting the storage (%d, %d)...\n", x, y);
		printStorageInside(x, y);
		storedCnt--; 
		initStorage(x, y); // initialize the storage
		return 0;
	}
	else 
		return -1;

}

int str_findStorage(int nBuilding, int nRoom) {
	{
	
	int i, j;
	int check = 0; 
	
	for (i=0;i<systemSize[0];i++)
	{
		for (j=0;j<systemSize[1];j++)
		{
			if(nBuilding == deliverySystem[i][j].building && nRoom == deliverySystem[i][j].room) // if user input information is same
			{
				printf(" Found package in (%d, %d)\n", i, j);
				check++;
			}
		}
	}
	if(check == 0) // if not same
	{
		return 0;
	}
	
	return 1;
}
	
	return cnt;
}
