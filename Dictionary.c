// Rhea Rupani
// rrupani
// 12B PA5
// Dictionary.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Dictionary.h"

const int tableSize = 101;

// Node Object

typedef struct NodeObj{
	char* key;
	char* value;
	struct NodeObj* next;
} NodeObj;
typedef NodeObj* Node;

// constructor
Node newNode(char* k, char* v){
	Node N = malloc(sizeof(Node));
	assert(N!=NULL);
	N->key = k;
	N->value = v;
	N->next = NULL;
	return (N);
}

// free memory
void freeNode(Node* pN){
	if(pN != NULL && *pN != NULL){
		free(*pN);
		*pN = NULL;
	}
}

// create data type
typedef struct DictionaryObj{
	Node *front;
	int numItems;
	
} DictionaryObj;

// rotate_left()
// rotate the bits is an unsigned int
unsigned int rotate_left(unsigned int value, int shift) {
   int sizeInBits = 8*sizeof(unsigned int);
   shift = shift & (sizeInBits - 1);  // remainder on division by sizeInBits
   if ( shift == 0 )
      return value;
   return (value << shift) | (value >> (sizeInBits - shift));
}
// pre_hash()
// turn a string into an unsigned int
unsigned int pre_hash(char* input) {  
   unsigned int result = 0xBAE86554;  
   while (*input) {                   
      result ^= *input++;                 
                                         
      result = rotate_left(result, 5);   
   }
   return result;  
}

// hash()
// turns a string into an int in the range 0 to tableSize-1
int hash(char* key){
   return pre_hash(key)%tableSize;
}
// newDictionary()
// constructor for the Dictionary type
Dictionary newDictionary(){
	Dictionary D = malloc(sizeof(Dictionary));
	assert(D!=NULL);
	D->front = calloc(tableSize, sizeof(Node));
	assert(D->front != NULL);
	D->numItems = 0;
	return D;
}

// freeDictionary()
// destructor for the Dictionary type
void freeDictionary(Dictionary* pD){
	free(*pD);
	*pD = NULL;
}


// isEmpty()
// returns 1 (true) if S is empty, 0 (false) otherwise
// pre: none
int isEmpty(Dictionary D){
	if(D == NULL){
		fprintf(stderr, "Dictionary Error: calling isEmpty() on NULL Dictionary reference\n" );
		exit(EXIT_FAILURE);
	}
	if(D->numItems>0){
		return 0;
	}
	return 1;
}

// size()
// returns the number of (key, value) pairs in D
// pre: none
int size(Dictionary D){
	return D->numItems;
}

// findKey()

Node findKey(Dictionary D, char* k){
  Node N;
  N = D->front[hash(k)];
  while(N !=NULL){
    if(strcmp(N->key,k)== 0)
      break;
    N = N->next;
  }
  return N;
}

// lookup()
// returns the value v such that (k, v) is in D, or returns NULL if no 
// such value v exists.
// pre: none
char* lookup(Dictionary D, char* k){
	if( D == NULL ){
      fprintf(stderr, "Dictionary Error: calling lookup() on NULL Dictionary\n");
      exit(EXIT_FAILURE);
   }
   if(D->numItems == 0){
      fprintf(stderr, "Dictionary Error: calling lookup() on empty Dictionary reference");
      exit(EXIT_FAILURE);
   }
   if(findKey(D,k) == NULL)
      return NULL;
   else
      return findKey(D,k)->value;
}

// insert()
// inserts new (key,value) pair into D
// pre: lookup(D, k)==NULL
void insert(Dictionary D, char* k, char* v){
	Node N;
   int i = hash(k);
   if(D == NULL){
      fprintf(stderr, "Dictionary Error: calling insert on NULL Dictionary\n");
      exit(EXIT_FAILURE);
   }
   if( findKey(D, k)!= NULL){
      fprintf(stderr, "Dictionary Error: calling insert() on a pre-existing key");
      exit(EXIT_FAILURE);
   } 
   if(D->front[i] == NULL){
      D->front[i]  = newNode(k, v);
      D->numItems++; 
   }else{
      N = newNode(k, v);
      N->next = D->front[i];
      D->front[i] = N;
      D->numItems++;
   }
}

// delete()
// deletes pair with the key k
// pre: lookup(D, k)!=NULL
void delete(Dictionary D, char* k){
	Node N;
   Node A;
   if( findKey(D,k) == NULL ){
      fprintf(stderr, "errror: key not found\n");
      exit(EXIT_FAILURE);
   }
   N = findKey(D,k);
   if(N == D->front[hash(k)] && N->next == NULL){
      N = NULL;
      freeNode(&N);
   }else if(N == D->front[hash(k)]){
      D->front[hash(k)] = N->next;
      N = NULL;
      freeNode(&N);
   }else{
      A = D->front[hash(k)];
      while(A->next != N){
         A = A->next;
      }
      A->next = N->next;
      freeNode(&N);

   }
   D->numItems--;
}

// makeEmpty()
// re-sets D to the empty state.
// pre: none
void makeEmpty(Dictionary D){
	for(int i = 0; i<tableSize; i++){
      while(D->front[i] != NULL){
         Node N;
         N = D->front[i];
         D->front[i]=N->next;
         freeNode(&N);
         N = NULL;
      }
   }D->numItems = 0;
}

// printDictionary()
// pre: none
// prints a text representation of D to the file pointed to by out
void printDictionary(FILE* out, Dictionary D){
	Node N;
   if( D==NULL ){
      fprintf(stderr, "Dictionary Error: calling printDictionary() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   for(int i = 0; i < tableSize; i++){
      N = D->front[i];
      while(N != NULL){
         fprintf(out, "%s %s \n" , N->key, N->value);
         N = N->next;
      }
}
}
