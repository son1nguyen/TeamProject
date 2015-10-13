/*	header.h
 
 Header file for main, hash, and data_output. Contains data
 structure definitions and prototype declarations for these
 files. This header file can be used by the other files to
 access the structures used within the program.
 
 The functions within main are used to process the menu and
 reads in the desired instructions from the user. Main also
 will free all the memory that was allocated throughout the
 program, within the destroy function.
 
 The hash functions are used to store data in the hash table,
 process the hash table, and manipulate the overall size of
 the hash table. These factors will contribute into calculating
 the efficiency of the program.
 
 The data_ouput functions are used to process the output either
 into an output file, or onto the screen. The program will
 automatically process an output file containing the updated
 data, if the user does not manually choose to write to file
 throughout the program. The output file is formated in the same
 way as original input file, so the user can use the output file
 as the input file to run the program if they choose to.
 
 */

#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)	// needed for my version of microsoft visual C++ 2010 to work
#include <ctype.h>
#include <string.h>
//#include <stdbool.h>
typedef enum { false, true} bool;

// Structure Definitions
typedef struct{
    char arpCode[4];
    char* city;
    float latitude;
    float longitude;
}DATA;

typedef struct collision{
    DATA* pData;
    struct collision* next;
}COLLISION;


typedef struct{
    int countCollision;
    DATA* pData;
    COLLISION* pCollision;
}HASH_NODE;


typedef struct hash{
    int arraySize;
    int countUsed;
    HASH_NODE* pTable;
}HASH;

typedef struct node
{
    void*        dataPtr;
    struct node* left;
    struct node* right;
}NODE;

typedef struct
{
    int   count;
    int  (*compare) (void* argu1, void* argu2);
    NODE*  root;
}BST_TREE;

typedef struct{
    HASH* pHash;
    BST_TREE* pTree;
}HEAD;


// main: Prototype Declarations
HEAD* buildHead (HEAD* header, char* fileInput);
bool getData (BST_TREE* tree, FILE* fpIn, DATA** airport);
int countLines (char* fileName);
void getOption (HEAD* pHeader);
bool addAirport (HEAD* pHeader);
void efficiency(HASH* pHash);
HEAD* destroy (HEAD* pHeader);

//	hash: Prototype Declarations
HASH* buildHash (int sizeHash);
bool insertHash (HASH* hashTable, DATA* pData);
bool deleteHash (HEAD* pHeader, DATA target);
int checkHash (HASH* header);
int converter(DATA* pData, int sizeHash);
HASH* upsizeHash (HASH* pHash);
HASH* downsizeHash (HASH* pHash);
COLLISION* collisionSolver (COLLISION* pList, DATA* pData);
DATA* findHash (HASH* pHash, DATA* target);
int countCollision (HASH* pHash);
HASH* hashDemo (HASH* pHash);

//	data_output: Prototype Declarations
char menu (void);
void printHash (HASH* pHash);
void printTree (NODE* root, int level);
void processScreen (void* data);
bool outputFile (HASH* pHash);
void processFile (void* data, FILE* fOut);

//	BST: Prototype Declarations for public functions
BST_TREE* BST_Create (int (*compare) (void* argu1, void* argu2));
BST_TREE* BST_Destroy (BST_TREE* tree);

bool  BST_Insert   (BST_TREE* tree, void* dataPtr);
bool  BST_Delete   (BST_TREE* tree, void* dltKey);
void* BST_Retrieve (BST_TREE* tree, void* keyPtr);
void  BST_Traverse (BST_TREE* tree,
                    void (*process)(void* dataPtr));

bool BST_Empty (BST_TREE* tree);
bool BST_Full  (BST_TREE* tree);
int  BST_Count (BST_TREE* tree);
