/*******************************
 File: Pinball.cpp

 Project: CMSC 341 Project 5, fall 2016

 Author: Harrison Mann

 Date: 11/30/16

 Section: 4

 Email: mannhar1@umbc.edu

This is the source file for the Pinball class. this
actually implements all of the necessary functions 
for the class and allows the hash table to have
functionality.  Also has a print functions for many
statistics.

*********************************/



/****************MY NOTES****************

we will use strcmp(const char * str1, const char * str2) for string comparisons
  -this compares 2 c-strings
  if return is...
  != 0 -----> then they do not match

  == 0 -----> then they do match!


the hash table should make a copy of the string inserted and not just store the pointer
copies can be made using the function strdup(const char * str)
  -this returns a c-string identical to the one passed in
  -this allocates memory using malloc(), NOT NEW, so when we want to deallocate we should use free(), NOT DELETE
  -stick to using malloc() and free() for the rest of the program aswell so things dont get complicated

********************************/
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Pinball.h"


using namespace std;


Pinball::Pinball(int n){
  //my given degree and ejection limit
  m_degree = 7;
  m_ejectLimit = 14;

  //capacity of n, but initially empty
  m_capacity = n;
  m_size = 0;

  //used to count how many ejections are done during an insertion
  m_numEjectsDone = 0;

  m_totalEjects = 0;
  m_maxEjectsDone = 0;

  //allocates space for n number of c-strings in the hash table H
  H = (char **)malloc(sizeof(char *) * n);
  for(int i = 0; i < n; i++){
    H[i] = NULL;
  }


  //fills in the vector of seeds with the "randomized" seeds for each slot
  for (int i = 0; i < n; i++){
    seeds.push_back(rand());
  }

}

Pinball::~Pinball(){
  //remember to use free(), not delete
  for (int i = 0; i < m_capacity; i++){
    free(H[i]);
  }
  free(H);
}

void Pinball::insert(const char * str){
  //first we have to check to see if this string is already in the hash table, it should not be added a second time
  if(find(str) != -1){
    return;
  }

  //then we have to check to see if the hash table is already full, this will throw a PinballHashFull exception
  if (m_size == m_capacity){
    throw PinballHashFull("Cannot insert into a full hash table!");
  }

  //now we find the hashValue of the string to insert and convert it to a primary slot
  unsigned int primaryVal = hashCode(str);


  int primarySlot = primaryVal % m_capacity;

  //if this slot is available, we insert it and return; to end the insertion
  if(H[primarySlot] == NULL){
    m_size = m_size + 1;
    H[primarySlot] = strdup(str);
    m_numEjectsDone = 0;//resets because this insertion is complete
    return;
  }

  //we must "save some randomness to allow us to get a random pick for the aux slot to eject
  int savedRand = rand();

  //if this slot is not available we srand() with the seed from the vector of seeds at the primarySlot index
  srand(seeds[primarySlot]);

  //then we will try the first 6 aux slots with rand() and see if any of them are avaialable for insertion
  //if any of these slots are available, again, we return; to end the insertion
  for (int i = 0; i < 6; i++){
    int auxVal = rand();
    int auxSlot = auxVal % m_capacity;
    auxSlots.push_back(auxSlot);

    if(H[auxSlot] == NULL){
      m_size = m_size + 1;
      H[auxSlot] = strdup(str);
      m_numEjectsDone = 0;//resets because this insertion is complete
      auxSlots.clear();
      return;
    }

  }


  //if all of the aux slots are full, we will seed srand with savedRand to get a "random" choice for which aux slot to eject
  //we use this number to determine which of the 6 aux slots to pick
  //whichever one is decided upon, we must re-seed with the seed from the vector and call rand() as many times as it takes to get back to that slot
  srand(savedRand);

  int randAuxSlot = (rand() % 6);


  int auxSlot = auxSlots[randAuxSlot];


  auxSlots.clear();
  


  //we will call remove on that spot to first remove the current string, then we will insert the new one into that slot, and call insert again with the removed string

  
  const char * toRemove = at(auxSlot);
  char * temp = remove(toRemove);
  


  m_size = m_size + 1;
  H[auxSlot] = strdup(str);
  
  m_numEjectsDone = m_numEjectsDone + 1;
  m_totalEjects = m_totalEjects + 1;
  if(m_numEjectsDone > m_maxEjectsDone){
    m_maxEjectsDone = m_numEjectsDone;
  }


  if(m_numEjectsDone > 14){
    free(temp);
    m_numEjectsDone = 0;


    return;
    //we return here to get back to the previous calls of insert so we can free temp
    //each time we will add one more to numEjectsDone, which should make it equal to 14 on the last call
    //if it equals 14, then we throw the exception...
  }


  insert(temp);
  free(temp);
  
  //this counts on the way back down through all the recursive calls
  m_numEjectsDone = m_numEjectsDone + 1;

  if(m_numEjectsDone == 14){
    throw PinballHashFull("*** Exception: Maximum ejections reached");
  }

}

int Pinball::find(const char * str){
  //first we want to get the hash value of the str which will tell us the primary slot it would be placed in
  unsigned int primaryVal = hashCode(str);

  int primarySlot = primaryVal % m_capacity;

  //next we check this spot in the hash table to see if the item is present, if it is we return the index of that slot
  if(H[primarySlot] != NULL && strcmp(H[primarySlot], str) == 0){
    return primarySlot;
  }
  

  //then we get the seed from the vector of seeds that corresponds to that slot and we compute the 6 other slots this item could be in
  srand(seeds[primarySlot]);

  //next we continually check through these 6 slots, returning their index if the str is contained in any of them
  for (int i = 0; i < 6; i++){
    int auxVal = rand();
    int auxSlot = auxVal % m_capacity;

    if(H[auxSlot] != NULL && strcmp(H[auxSlot], str) == 0){
      return auxSlot;
    }

  }

  //if we havent found the value at this point, we return -1 indicating that it is not in the table
  return -1;
}

const char * Pinball::at(int index){

  if(index < 0 || index >= m_capacity){
    throw out_of_range("cannot find item in hash table at an index not in the table!");
  }
  
  return H[index];
  
}

char * Pinball::remove(const char * str){
  
  //first we find the slot the string is at in the hash table
  int itemSlot = find(str);


  //if we found that the string isnt in the hash table, we return NULL
  if (itemSlot == -1){
    return NULL; 
  }
  //otherwise, save the string from the hash table, then set the slot to null, then return the removed string
  else{
    
    m_size = m_size - 1;
    char * temp = strdup(H[itemSlot]);
    free(H[itemSlot]);
    H[itemSlot] = NULL;

    return temp;


  }
}

void Pinball::printStats(){

  int numPrimSlots = 0;
  for (int i = 0; i < m_capacity; i++){
    if(H[i] != NULL){
      unsigned int primVal = hashCode(H[i]);
      if (i == primVal % m_capacity){
	numPrimSlots = numPrimSlots + 1;
      }
    }
  }

  double avgHitsToPrim = ((double)m_size/(double)numPrimSlots);
  

  int numPrimHashes[m_capacity];
  for (int i = 0; i < m_capacity; i++){
    numPrimHashes[i] = 0;
  }


  for(int i = 0; i < m_capacity; i++){
    if(H[i] != NULL){
      unsigned int primVal = hashCode(H[i]);
      int primSlot = primVal % m_capacity;
      numPrimHashes[primSlot] = numPrimHashes[primSlot] + 1;
    }
  }
  


  int maxHitsToPrim = numPrimHashes[0];
  for(int i = 1; i < m_capacity; i++){
    if (numPrimHashes[i] > maxHitsToPrim){
      maxHitsToPrim = numPrimHashes[i];
    }
  }
  



  cout << "Hash statistics report:" << endl;
  cout << "   Randomness level: Pseudo-random" << endl;
  cout << "   Capacity: " << m_capacity << endl;
  cout << "   Size: " << m_size << endl;
  cout << "   Degree: " << m_degree << endl;
  cout << "   Ejection Limit: " << m_ejectLimit << endl;
  cout << "   Number of primary slots: " << numPrimSlots  << endl;
  cout << "   Average hits to primary slots: " << avgHitsToPrim  << endl;
  cout << "   Maximum hits to primary slots: " << maxHitsToPrim  << endl;
  cout << "   Total number of ejections: " << m_totalEjects  << endl;
  cout << "   Maximum number of ejections in a single insertion: " << m_maxEjectsDone  << endl;
}

void Pinball::calculateStats(){
  //randomness level is always pseudo-random
  //capacity = m_capacity
  //size = m_size -->size()
  //degree = m_degree
  //ejection limit = m_ejectLimit
  //number of primary slots = m_numPrimSlots****
  //average hits to prim slots = m_avgHitsToPrim*****
  //maximum hits to primary slots = m_maxHitsToPrim****
  //total number of ejections = m_totalEjects
  //maximum number of ejections in a single insert = m_maxEjectsDone
  
  m_numPrimSlots = 0;
  for (int i = 0; i < m_capacity; i++){
    if(H[i] != NULL){
      unsigned int primVal = hashCode(H[i]);
      if (i == primVal % m_capacity){
        m_numPrimSlots = m_numPrimSlots + 1;
      }
    }
  }

  m_avgHitsToPrim = ((double)m_size/(double)m_numPrimSlots);


  int numPrimHashes[m_capacity];
  for (int i = 0; i < m_capacity; i++){
    numPrimHashes[i] = 0;
  }


  for(int i = 0; i < m_capacity; i++){
    if(H[i] != NULL){
      unsigned int primVal = hashCode(H[i]);
      int primSlot = primVal % m_capacity;
      numPrimHashes[primSlot] = numPrimHashes[primSlot] + 1;
    }
  }


  m_maxHitsToPrim = numPrimHashes[0];
  for(int i = 1; i < m_capacity; i++){
    if (numPrimHashes[i] > m_maxHitsToPrim){
      m_maxHitsToPrim = numPrimHashes[i];
    }
  }

  
}
