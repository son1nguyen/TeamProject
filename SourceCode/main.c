/****************************************************************************
 ** CIS 15C
 ** Fall  2013
 *************
 **
 ** Team#1: Team Project
 ** This program stores and manipulates a database of airports. It can also
 ** print to screen or file the database according to hash index, key, or
 ** as a tree. This program uses a self-adjusting linked-list to solve collisions
 ** that occur within the hash table. Any information that is searched is
 ** moved to the front of the collision linked list. This program is useful
 ** because you can use it to manage a database of airport for your travels.
 ** Latitude and longitude values are useful for airplane pilots who must
 ** navigate at night and must rely only on coordinates to pilot the plane.
 ** This information is processed in the program because of this importance.
 **
 **
 ***********************************
 **  Written By: Jimmy Ninh, Son Nguyen, Aashbir Basi, Ross Wilkinson
 **	Date completed:
 ********************************************************************************/


#include "header.h"
#ifdef _MSC_VER
#include <crtdbg.h>				// needed to check for memory leaks (Windows only!)
#endif



int main (void)
{
	//	Local Declarations
    HEAD* pHeader = NULL;
    
	//	Statements
    pHeader = buildHead(pHeader, "data.txt");
    getOption(pHeader);
    
	printf ("\nSaving data ... \n");
	if (outputFile(pHeader->pHash))
		printf ("Successfully saved\n");
	else
		printf ("Could not save to file.\n");
    pHeader = destroy(pHeader);
    
	
#ifdef _MSC_VER
	printf( _CrtDumpMemoryLeaks() ? "Memory Leak\n" : "No Memory Leak\n");
#endif
    return 0;
}	// main


/*	================== compareCode =================
 This function accepts two void pointers and casts
 them to DATA structure pointers, to compare the two
 keys of the data.
 Pre		airport1 is a void pointer to DATA
 airport2 is a void pointer to DATA
 Post
 Return	0 : if airport1 = airport2
 -1 : if airport1 < airport2
 1: if airport1 > airport2
 */
int  compareCode (void* airport1, void* airport2)
{
    
	DATA station1 = *((DATA*)airport1);
    DATA station2 = *((DATA*)airport2);
    
    return strcmp(station1.arpCode, station2.arpCode);
}	// compareCode


/*	================== buildHead =================
 This function creates the header structure that
 contains pointers to the tree and the hash table.
 It also calls other functions to read in the data
 file.
 Pre		pHeader - pointer to HEAD structure
 fileInput - name of the file
 Post		both the tree and the hash table are
 created.
 Return	pointer to create HEAD structure
 */
HEAD* buildHead (HEAD* pHeader, char* fileInput)
{
	//	Local Declarations
	DATA* newAirport;
    FILE* fpIn;
    
	//	Statements
    fpIn = fopen(fileInput, "r");
    if (!fpIn) {
        printf("Error opening input file\n");
        exit(101);
    }
    
    if ((pHeader = (HEAD*) malloc(sizeof(HEAD))))
    {
        pHeader->pHash = buildHash(2 * countLines(fileInput));
        pHeader->pTree = BST_Create(compareCode);
    }
    else{
        printf("Memory allocation error\n");
        exit(100);
    }
    
    while (getData(pHeader->pTree, fpIn, &newAirport)) {
        BST_Insert(pHeader->pTree, newAirport);
        insertHash(pHeader->pHash, newAirport);
    }
    
    return pHeader;
}	// buildHead


/*	================== getData =================
 This function reads in the data from the input
 file and stores it into a DATA structure.
 Pre		pTree - pointer to the tree
 fpIn - pointer to input file
 airport - pointer to another
 pointer, to DATA structure
 Post		stores data into DATA structure
 Return	true if success
 false if fails
 */
bool getData (BST_TREE* pTree, FILE* fpIn, DATA** airport)
{
	//	Local Declarations
	bool result = false;
	char airCode[4];
	char city[20];
    
	//	Statements
	fscanf (fpIn, "%s", airCode);
	if (!feof(fpIn))
	{
		result = true;
		fscanf (fpIn, "	%[^;]s", city);
        
		*airport = (DATA*) malloc(sizeof(DATA));
		if (!(*airport))
			printf ("Could not alloc memory for airport."), exit (800);
        
		strcpy ((*airport)->arpCode, airCode);
		(*airport)->city = (char*) malloc (strlen(city) + 1);
		strcpy ((*airport)->city, city);
		fscanf (fpIn, ";	%f%f", &(*airport)->latitude, &(*airport)->longitude);
	}
    return result;
}	// getData


/*	================== countLines =================
 This function counts the number of elements that
 are in the input file.
 Pre		fileName - name of file to be open
 Post
 Return	the number of elements in input file
 */
int countLines (char* fileName)
{
	//	Local Declarations
	int count = 0;
	char tempLine[128];
    FILE* fpIn;
    
	//	Statements
    fpIn = fopen(fileName, "r");
    if (!fpIn) {
        printf("Error opening input file\n");
        exit(104);
    }
    
    while (fgets(tempLine, sizeof(tempLine)-1, fpIn)) {
        count++;
    }
    
    return count;
}	// countLines


/*	================== getOption =================
 This function reads in the user's desired chose
 of operation. Calls upon other functions to
 perform the procedure.
 Pre		pHeader - pointer to HEAD structure
 Post
 Return
 */
void getOption (HEAD* pHeader)
{
	//	Local Declarations
    char command;
    DATA target;
    DATA* airport = NULL;
	int i;
    
	//	Statements
    while ((command = menu()) != 'Q') {
        switch (command)
        {
            case 'A':
                if (addAirport(pHeader))
                {
                    while (checkHash(pHeader->pHash) == 1) {
                        pHeader->pHash = upsizeHash(pHeader->pHash);
                    }
					printf ("\n Succesfully added data.\n\n");
                }
                break;
            case 'D':
                printf("Enter the airport code: ");
                scanf(" %s", target.arpCode);
                
				if (deleteHash (pHeader, target))
                {
                    while (checkHash(pHeader->pHash) == -1) {
                        pHeader->pHash = downsizeHash(pHeader->pHash);
                    }
					printf ("\n Succesfully deleted data.\n\n");
                }
                
                break;
            case 'F':
                printf("Enter the airport code: ");
                scanf(" %s", target.arpCode);
				// fix sensitive input cases
				for (i = 0; i < strlen(target.arpCode); i++) {
					target.arpCode[i] = toupper(target.arpCode[i]);
				}
                airport = findHash(pHeader->pHash, &target);
                if (airport != NULL) {
                    processScreen(airport);
                }
                else printf("No airport exists\n");
                
                break;
            case 'L':
                printHash(pHeader->pHash);
                break;
            case 'K':
                BST_Traverse(pHeader->pTree, processScreen);
                break;
            case 'P':
                printTree(pHeader->pTree->root, 0);
				printf("\n");
                break;
            case 'W':
				outputFile (pHeader->pHash);
                break;
            case 'E':
				efficiency(pHeader->pHash);
                break;
			case 'H':
				pHeader->pHash = hashDemo(pHeader->pHash);
				break;
            default:
                printf("Invalid choice. Choose again\n");
                break;
        }
    }
    return;
}	// getOption


/*	================== addAirport =================
 This function adds another element of data into
 both the tree and hash table, through the user's
 input from keyboard.
 Pre		pHeader - pointer to HEAD structure
 Post
 Return	true if success
 false if fails
 */
bool addAirport (HEAD* pHeader)
{
	//	Local Declarations
    bool result = false;
    DATA tempAirport;
    DATA* newAirport = NULL;
    char tempCode [28];
    char tempName [128];
    int i;
    
	
	printf ("Enter airport code: ");
	scanf (" %s", tempCode);
    
    if (strlen(tempCode) != 3) {
        printf("Your airport code has to have 3 characters\n");
        return result;
    }
	
    for (i = 0; i<strlen(tempCode); i++) {
        tempCode[i] = toupper(tempCode[i]);
    }
    
    strcpy(tempAirport.arpCode, tempCode);
    newAirport = findHash(pHeader->pHash, &tempAirport);
    if (newAirport == NULL)
    {
        if (!(newAirport = (DATA*) malloc(sizeof(DATA)))) {
            printf("Error allocating new airport\n");
            exit(100);
        }
        strcpy(newAirport->arpCode, tempCode);
        printf("Enter airport city: ");
        scanf(" %[^\n]", tempName);
        newAirport->city = (char*) calloc(strlen(tempName) + 1, sizeof(char));
        strcpy(newAirport->city, tempName);
        
        printf("Enter airport latitude: ");
        while(!(scanf("%f", &newAirport->latitude)))
        {
            printf("Invalid input, please try entering the latitude again: ");
            while(getchar() != '\n');
        }
        printf("Enter airport longitude: ");
        while(!(scanf("%f", &newAirport->longitude)))
        {
            printf("Invalid input, please try entering the longitude again: ");
            while(getchar() != '\n');
        }
        
        insertHash(pHeader->pHash, newAirport);
        BST_Insert(pHeader->pTree, newAirport);
        result = true;
    }
    else{
        printf("This airport already exists\n");
        processScreen(newAirport);
    }
    return result;
}	// addAirport


/*	================== destroy =================
 This function frees everything that was allocated
 throughout the entire program.
 Pre		pHeader - pointer to HEAD structure
 Post		everything is freed
 Return	HEAD* = NULL
 */
HEAD* destroy (HEAD* pHeader)
{
	//	Local Declarations
	int i;
    
	//	Statements
    for (i = 0; i < pHeader->pHash->arraySize; i++)
    {
        while (pHeader->pHash->pTable[i].pData != NULL)
        {
            deleteHash(pHeader, *pHeader->pHash->pTable[i].pData);
        }
    }
	free (pHeader->pHash->pTable);
	free (pHeader->pHash);
	free (pHeader->pTree);
	free (pHeader);
    
    return NULL;
}	// destroy

