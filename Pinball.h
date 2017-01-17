/*******************************
 File: Pinball.h

 Project: CMSC 341 Project 5, fall 2016

 Author: Harrison Mann

 Date: 11/30/16

 Section: 4

 Email: mannhar1@umbc.edu

header file for the pinball class, sets up the functions
and provides the PinballHashFull exception as well as
the hashCode function

*********************************/


#ifndef _PINBALL_H_
#define _PINBALL_H_

#include <string>
#include <stdexcept>
#include <vector>

using namespace std ;

class PinballHashFull : public std::out_of_range {

 public:
 PinballHashFull(const string& what) : std::out_of_range(what) { }
} ;


class Pinball {

 public:
  Pinball(int n=1019) ;    // default capacity is prime
  ~Pinball() ;

  void insert(const char *str) ;
  int find(const char *str) ;
  const char * at(int index) ;
  char * remove(const char *str) ;
  void printStats() ;

  void calculateStats();


  //getters for data for myDriver to write to output file
  int capacity() { return m_capacity; }
  int size() { return m_size ; }
  int degree() { return m_degree; }
  int ejectLimit() { return m_ejectLimit; }
  int numPrimSlots() { return m_numPrimSlots; }
  double avgHitsToPrim() { return m_avgHitsToPrim; }
  int maxHitsToPrim() { return m_maxHitsToPrim; }
  int totalEjects() { return m_totalEjects; }
  int maxEjectsDone() { return m_maxEjectsDone; }



 private:


  vector<int> seeds; //vector corresponding to hash table with the seed for each slot
  vector<int> auxSlots; //vector containing the 6 aux slots for a given value


  
  char ** H ;        // the actual hash table
  int m_size ;       // number of items stored in H
  int m_capacity ;   // number slots allocated in H

  int m_degree ;     // use degree assigned to you
  int m_ejectLimit ; // use ejection limit assigned to you

  int m_numEjectsDone;

  //statistics
  int m_totalEjects;
  int m_maxEjectsDone;
  int m_numPrimSlots;
  double m_avgHitsToPrim;
  int m_maxHitsToPrim;


  // Made inline definition of hashCode() so everyone uses
  // the same function.
  //
  unsigned int hashCode(const char *str) {

    unsigned int val = 0 ; 
    const unsigned int thirtyThree = 33 ;  // magic number from textbook

    int i = 0 ;
    while (str[i] != '\0') {
      val = val * thirtyThree + str[i] ;
      i++ ;
    }
    return val ; 
  }


  // add private data members and 
  // private member functions as needed
} ;


#endif
