/* hash.c
 This file contains the definitons of the functions to maintain
 and process a hash:
 
 Functions:
 buildHash
 insertHash
 checkHash
 findHash
 upsizeHash
 downHash
 deleteHash
 converter
 collisionSolver
 countCollision
 hashDemo
 
 */

#include "header.h"

/*	================== buildHash =================
 This function creates the hash table.
 Pre		sizeHash - size of required hash
 Post		hash table is initialized
 Return	pointer to the start of the hash
 table
 */
HASH* buildHash (int sizeHash)
{
    HASH* pHash=NULL;
    int i;
    if (!(pHash = (HASH*) malloc(sizeof(HASH))))
    {
        printf("Memory Allocation Error\n");
        exit(102);
    }
    
    pHash->pTable = NULL;
    pHash->arraySize = 0;
    pHash->countUsed = 0;
    
    if (!(pHash->pTable = (HASH_NODE*) calloc(sizeHash, sizeof(HASH_NODE)))) {
        printf("Not enought memory\n");
        exit(103);
    }
    pHash->arraySize = sizeHash;
    
    for (i=0; i < sizeHash; i++) {
        pHash->pTable[i].countCollision = 0;
        pHash->pTable[i].pCollision = NULL;
        pHash->pTable[i].pData = NULL;
    }
    
    return pHash;
}	// buildHash


/*	================== insertHash =================
 This function accepts a pointer to a DATA structure
 and inserts this into the hash table.
 Pre		pHash - pointer to start of hash table
 pDataIn - pointer to DATA structure to
 be inserted
 Post		DATA structure is inserted into Hash table
 Return	true if success
 false if fail
 */
bool insertHash(HASH* pHash, DATA* pDataIn)
{
	//	Local Declarations
	bool result = false;
	int index;
    
	//	Statements
	index = converter(pDataIn, pHash->arraySize);
	if (pHash->pTable[index].pData == NULL) {
		pHash->pTable[index].pData = pDataIn;
		pHash->countUsed++;
		result = true;
	}
	else{
		pHash->pTable[index].pCollision = collisionSolver(pHash->pTable[index].pCollision, pDataIn);
		pHash->pTable[index].countCollision++;
		result = true;
	}
    
	return result;
}	// insertHash


/*	================== checkHash =================
 This function checks the load factor of the hash
 table. It will return different value depending
 on whether the load factor is above/equal
 to 75% or below/equal 10%.
 Pre		pHash - pointer to the start of hash
 table
 Post
 Return	-1 : if percentFull <= 0.1
 1: if percentFull >= .75
 */
int checkHash (HASH* pHash)
{
	//	Local Declarations
    float percentFull = 0.00;
    
	//	Statements
    percentFull = (float) pHash->countUsed / pHash->arraySize;
    if (percentFull >= 0.75)
        return 1;
    if (percentFull <= 0.1)
        return -1;
    
    return 0;
}	// checkHash


/*	================== findHash =================
 This function will search hash for designated
 DATA structure, and return to user that position
 in the hash.
 Pre		pHash - pointer to start of hash table
 target - pointer to searched DATA
 structure
 Post
 Return	pointer in HASH table or
 pointer in collision linked-list or
 NULL if not found
 */
DATA* findHash (HASH* pHash, DATA* target)
{
	//	Local Declarations
    int index;
	DATA* pSwap = NULL;
	COLLISION* pWalker = NULL;
    
	//	Statements
    index = converter(target, pHash->arraySize);
    
    if (pHash->pTable[index].pData != NULL)
    {
        if (compareCode(pHash->pTable[index].pData, target) != 0)
        {
            pWalker = pHash->pTable[index].pCollision;
            
            while (pWalker!=NULL)
            {
                if (compareCode(pWalker->pData, target) == 0) {
                    pSwap = pHash->pTable[index].pCollision->pData;
                    pHash->pTable[index].pCollision->pData = pWalker->pData;
                    pWalker->pData = pSwap;
                    
                    return pHash->pTable[index].pCollision->pData;
                }
                else pWalker=pWalker->next;
            }
        }
        else {
            return pHash->pTable[index].pData;
        }
    }
    return NULL;
}	// findHash


/*	================== upsizeHash =================
 This function will resize the hash, by allocating
 double the size of current hash.
 Pre		pHash - pointer to start of hash table
 Post	    hash table is doubled in size
 Return	pointer to start of "bigger" hash table
 */
HASH* upsizeHash (HASH* pHash)
{
	//	Local Declarations
	COLLISION* pWalker;
    COLLISION* pPre;
	HASH* newHash = NULL;
	int i;
    int newArraySize = pHash->arraySize * 2;
    
	//	Statements
    newHash = buildHash(newArraySize);
    for (i = 0; i < pHash->arraySize; i++) {
        if (pHash->pTable[i].pData != NULL)
        {
            insertHash(newHash, pHash->pTable[i].pData);
            
            pWalker = pHash->pTable[i].pCollision;
            while (pWalker != NULL) {
                pPre=pWalker;
                insertHash(newHash, pWalker->pData);
                pWalker=pWalker->next;
                //Free pPre to free the collision node that we allocated
                free(pPre);
            }
        }
    }
    free(pHash->pTable);
    free(pHash);
    
    return newHash;
}	// upsizeHash


/*	================== downHash =================
 This function will resize the hash, by allocating
 only half of the size of the current hash.
 Pre		pHash - pointer to start of hash table
 Post	    hash table is halfed in size
 Return	pointer to start of "smaller" hash table
 */
HASH* downsizeHash (HASH* pHash)
{
	//	Local Declarations
	COLLISION* pWalker;
    COLLISION* pPre;
	HASH* newHash = NULL;
	int i;
    int newArraySize = pHash->arraySize / 2;
    
	//	Statements
    newHash = buildHash(newArraySize);
    
    for (i = 0; i<pHash->arraySize; i++) {
        if (pHash->pTable[i].pData != NULL)
        {
            insertHash(newHash, pHash->pTable[i].pData);
            
            pWalker = pHash->pTable[i].pCollision;
            while (pWalker != NULL) {
                pPre = pWalker;
                insertHash(newHash, pWalker->pData);
                pWalker = pWalker->next;
                //Free pPre to free the collision node that we allocated
                free(pPre);
            }
        }
    }
    free(pHash->pTable);
    free(pHash);
    
    return newHash;
}	// downsizeHash


/*	================== deleteHash =================
 This function will delete an element that is within
 the hash table or within the collision linked-list.
 Pre		pHash - pointer to start of hash table
 DATA - data structure to be deleted
 Post	    element is delete from hash table or
 the collision linked-list
 Return	true if success
 false if fail
 */
bool deleteHash (HEAD* pHeader, DATA target)
{
	//	Local Declarations
    bool result = false;
    DATA* delAirport = NULL;
    int index = 0;
    COLLISION* pPre = NULL;
    COLLISION* pCur = NULL;
	int i;
    
	//	Statements
    
	// fix sensitive input cases
	for (i = 0; i < strlen(target.arpCode); i++) {
		target.arpCode[i] = toupper(target.arpCode[i]);
	}
    
    delAirport = (DATA*) BST_Retrieve(pHeader->pTree, &target);
    if (delAirport == NULL)
    {
        printf("Your enter wrong airport code\n");
        return result;
    }
    else{
        result = true;
        index = converter(delAirport, pHeader->pHash->arraySize);
        //Delete a data which is in the Hash table
        if (compareCode(delAirport, pHeader->pHash->pTable[index].pData) == 0)
        {
            free(delAirport->city);
            BST_Delete(pHeader->pTree, delAirport);
            if (pHeader->pHash->pTable[index].pCollision != NULL)
            {
                pHeader->pHash->pTable[index].pData = pHeader->pHash->pTable[index].pCollision->pData;
				pCur = pHeader->pHash->pTable[index].pCollision;
                pHeader->pHash->pTable[index].pCollision = pHeader->pHash->pTable[index].pCollision->next;
                pHeader->pHash->pTable[index].countCollision--;
				free(pCur);
            }
            else{
                pHeader->pHash->pTable[index].pData = NULL;
                pHeader->pHash->countUsed--;
            }
        }
        //Delete a data which is in the collision linked pist
        else{
            pCur = pHeader->pHash->pTable[index].pCollision;
            if (pCur->next == NULL) {
                free(pCur->pData->city);
                BST_Delete(pHeader->pTree, delAirport);
                free(pCur);
                pHeader->pHash->pTable[index].pCollision = NULL;
                pHeader->pHash->pTable[index].countCollision = 0;
            }
            else{
                if (compareCode(pCur->pData, delAirport) == 0) {
                    pHeader->pHash->pTable[index].pCollision = pCur->next;
                    free(pCur->pData->city);
                    BST_Delete(pHeader->pTree, delAirport);
                    free(pCur);
                    pHeader->pHash->pTable[index].countCollision--;
                }
                else{
                    while (compareCode(pCur->pData, delAirport) != 0 || pCur->next != NULL) {
                        pPre = pCur;
                        pCur = pCur->next;
                    }
                    pPre->next = pCur->next;
                    free(pCur->pData->city);
                    BST_Delete(pHeader->pTree, delAirport);
                    free(pCur);
                    pHeader->pHash->pTable[index].countCollision--;
                }
            }
        }
        
    }
    return result;
}	// deleteHash


/*	================== converter =================
 This function acts as the hash function and
 calculates the key in which information should
 be inserted into.
 Pre		pData - pointer to DATA structure
 sizeHash - size of the Hash Table
 Post
 Return	calculated key
 */
int converter(DATA* pData, int sizeHash)
{
	//	Local Declarations
    int index = 1;
	int i;
    
	//	Statements
    for (i = 0; i < 3; i++) {
        index *= (pData->arpCode[i] - 64 + 7);
    }
    index = index % sizeHash;
    return index;
}	// converter


/*	================== collisionSolver =================
 This function will place a collision key into a linked
 list for that index of the hash table.
 Pre		pList - pointer to linked-list
 pData - pointer to DATA structure
 Post
 Return	pointer position in collision linked list
 */
COLLISION* collisionSolver (COLLISION* pList, DATA* pData)
{
	//	Local Declarations
    COLLISION* pInsert = NULL;
    
	//	Statements
    if ((pInsert = (COLLISION*) malloc(sizeof(COLLISION)))) {
        pInsert->next = NULL;
        pInsert->pData = pData;
    }
    else{
        printf("Error allocating collision node\n");
        exit(106);
    }
    
    if (pList == NULL) {
        pList = pInsert;
    }
    else{
        pInsert->next = pList;
        pList = pInsert;
    }
    
    return pList;
}	// collisionSolver


/*	================== hashDemo =================
 This is a developer hiden option that activates within
 the menu when the user press "h". It will demonstrate
 the upsizeHash and downsizeHash functions.
 
 Pre		pHash - pointer to start of hash table
 Post	    hash table will be resized
 Return	pointer to resized hash table
 */
HASH* hashDemo (HASH* pHash)
{
	//	Local Declaration
	char command;
    
	//	Statements
	printf("Type 'I' to increase the size of the hash or 'D' to decrease the size of the hash: ");
	scanf(" %c", &command);
	if (toupper(command) == 'I')
		return upsizeHash(pHash);
	else if (toupper(command) == 'D')
		return downsizeHash(pHash);
	else
		printf("Invalid command, nothing was changed.\n");
    
	return pHash;
}	// hashDemo


/*	================== efficiency =================
 This function calculates the load factor of the
 hash table, finds the longest length of collision
 linked-list and the average number of nodes that
 are in linked-lists. Will print out these values,
 along with other useful information for user to
 figure out the efficiency of the hash.
 Pre		pHash - pointer to HEAD structure
 Post		prints : load factor
 longest linked-list
 average collision nodes
 Return
 */
void efficiency(HASH* pHash)
{
	// Local Declarations
	float loadFactor = 0;
	int longestList = 0;
	float avgList = 0;
	int i = 0;
	int j = 0;
	int collisionCount = 0;
    
	// Statements
    
	loadFactor = (float) pHash->countUsed / (float) pHash->arraySize * 100;
	for (i = 0; i < pHash->arraySize; i++)
	{
		if (pHash->pTable[i].countCollision > 0)
		{
			j++;
			if (pHash->pTable[i].countCollision > longestList)
				longestList = pHash->pTable[i].countCollision;
			collisionCount = collisionCount + pHash->pTable[i].countCollision;
		}
	}
    
	if (j > 0)
		avgList = collisionCount / (float) j;
	
	printf("\nThe load factor is %.2f%%.\n", loadFactor);
	printf("The number of hash nodes used is %d.\n", pHash->countUsed);
	printf("The total size of hash is %d.\n",pHash->arraySize);
	printf("The total collision count is %d.\n", collisionCount);
	printf("The longest linked list is %d nodes long.\n", longestList);
	printf("The average number of nodes in a list is %.2f.\n\n", avgList);
	
	return;
}	// efficiency