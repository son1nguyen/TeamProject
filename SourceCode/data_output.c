/* data_output.c
 This file contains the definitons of the functions to
 process data onto the screen or to a file.
 
 Functions:
 menu
 printHash
 printTree
 processScreen
 outputFile
 processFile
 
 */

#include "header.h"

/*	================== menu =================
 This function will print out to the user the
 menu in which they can choose an option from,
 and scans in that value.
 Pre
 Post	    menu is printed on screen
 Return	user's option
 */
char menu (void)
{
	//	Local Declarations
    char choice;
    
	//	Statements
    printf("Enter 'A' to add new data\n");
    printf("      'D' to delete data\n");
    printf("      'F' to find data\n");
    printf("      'L' to list data in hash table sequence\n");
    printf("      'K' to list data in key sequence\n");
    printf("      'P' to print the tree\n");
    printf("      'W' to write data to a file\n");
    printf("      'E' to calculate efficiency\n");
    printf("      'Q' to quit\n");
    scanf(" %c", &choice);
    
    return toupper(choice);
}	// menu


/*	================== printHash =================
 This function will print out the hash according
 to the index.
 Pre		pHash - pointer to start of hash table
 Post	    hash is printed out by index
 Return
 */
void printHash (HASH* pHash)
{
	//	Local Declarations
    COLLISION* pWalker;
	int i;
    
	//	Statements
    for (i = 0; i < pHash->arraySize; i++)
    {
        if (pHash->pTable[i].pData != NULL) {
            printf("%3d ", i);
            processScreen(pHash->pTable[i].pData);
            
            pWalker = pHash->pTable[i].pCollision;
            while (pWalker != NULL) {
                printf("%3d ", i);
                processScreen(pWalker->pData);
                pWalker = pWalker->next;
            }
        }
    }
    return;
}	// printHash


/*	================== printTree =================
 This function will print out the tree horizontally,
 with tabs to indicate different levels and subtrees.
 Pre		root - pointer to root of tree
 level - level in which user would like
 to print from
 Post	    tree is printed onto screen
 Return
 */
void printTree (NODE* root, int level)
{
	// Local Declarations
	int child;
	int i;
    
	// Statements
	if (!root)
		return;
	if (root->right)
		printTree (root->right, level + 1);
    
	for (i = 0; i < level; i++)
		printf ("   ");
	printf("%s\n", ((DATA*) root->dataPtr)->arpCode);
    
	if (root->left)
        printTree (root->left, level + 1);
    
	return;
    
}	//	printTree


/*	================== processScreen =================
 This function will print out a DATA element onto the
 screen.
 Pre		data - void pointer to DATA structure
 Post	    elements of data printed onto screen
 Return
 */
void processScreen (void* data)
{
	//	Local Declarations
    DATA airport = *((DATA*) data);
    
	//	Statements
    printf("%s %-18s %-5.2f %-5.2f\n", airport.arpCode,
           airport.city, airport.latitude, airport.longitude);
    return;
}	// processAirport


/*	================== outputFile =================
 This function will create a new text file and
 write the data within the structures into this
 newly created file.
 Pre		pHash - pointer to the start of hash table
 Post	    new file is create containing data within
 the current hash table
 Return
 */
bool outputFile (HASH* pHash)
{
    
	//	Local Declarations
	FILE* fileOut;
    COLLISION* pWalker;
	int i;
	bool success = false;
    
	//	Statements
	if ((fileOut = fopen("outputFile.txt", "w")))
	{
		for (i = 0; i < pHash->arraySize; i++)
		{
			if (pHash->pTable[i].pData != NULL) {
				processFile(pHash->pTable[i].pData, fileOut);
				
				pWalker = pHash->pTable[i].pCollision;
				while (pWalker != NULL) {
					processFile(pWalker->pData, fileOut);
					pWalker = pWalker->next;
				}
			}
		}
		success = true;
	}
    
	return success;
}	// outputFile


/*	================== processFile =================
 This function will print out a DATA element into
 a file.
 Pre		data - void pointer to DATA structure
 fOut - file pointer to desired text file
 to be created
 Post	    elements of data printed into file
 Return
 */
void processFile (void* data, FILE* fOut)
{
	DATA airport = *((DATA*)data);
	fprintf(fOut, "%s\t%s;\t%.2f\t%.2f\n", airport.arpCode,
            airport.city, airport.latitude, airport.longitude);
	return;
}	// processFile
