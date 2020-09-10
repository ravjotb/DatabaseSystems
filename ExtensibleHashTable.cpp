//
//  ExtensibleHashTable.cpp
//  HashTable
//
//  Created by Ravjot Brar on 2020-06-16.
//  Copyright © 2020 Ravjot Brar. All rights reserved.
//
#include <iostream>
#include "ExtensibleHashTable.h"


//overload the assignment operator
void ExtensibleHashTable:: operator=(const ExtensibleHashTable& e){
    globalDepth= e.globalDepth;
    maxKeys= e.maxKeys;
    directorySize=e.directorySize;

    for(int i=0; i < e.directorySize; i++){
        directory[i]=e.directory[i];
    }
}
/*-------------------------------------------------------------------------------------*/
ExtensibleHashTable:: ExtensibleHashTable() //default constructor
 :globalDepth(1), maxKeys(4), directorySize(2) {
     directory= new Bucket* [directorySize];
     Bucket *b0= new Bucket;
     Bucket *b1= new Bucket;
     directory[0]= b0;
     directory[1]= b1;
 }
/*-------------------------------------------------------------------------------------*/
ExtensibleHashTable:: ExtensibleHashTable(int n) //constructor that specifies max number of keys
 :globalDepth(1), maxKeys(n), directorySize(2){
     directory= new Bucket* [directorySize];
     Bucket *b0= new Bucket;
     Bucket *b1= new Bucket;
     directory[0]= b0;
     directory[1]= b1;
 }
/*-------------------------------------------------------------------------------------*/
ExtensibleHashTable:: ExtensibleHashTable(const ExtensibleHashTable& e, int newSize)// copy constructor
{
    globalDepth= e.globalDepth;
    maxKeys= e.maxKeys;
    directorySize=newSize;
    
    directory= new Bucket* [directorySize];
    
    for(int i=0; i < e.directorySize; i++){
        directory[i]=e.directory[i];
    }
    
    for(int i=e.directorySize; i<directorySize; i++){ //make all new pointers point to respective buckets
        directory[i]=e.directory[i-e.directorySize];
    }
    
}
/*-------------------------------------------------------------------------------------*/
ExtensibleHashTable:: ~ExtensibleHashTable() //deconstructor
{
    
    for(int i=directorySize-1; i>=0; i--){
        //if an entry points to a bucket that is already pointed to
        if((i >= (1<< directory[i]->localDepth)) && (directory[i]->localDepth<globalDepth) )
            continue; //then don't delete
        
        else {
            delete directory[i];
        }
    }
    delete[] directory;
}
/*-------------------------------------------------------------------------------------*/
int ExtensibleHashTable::lastBits(int n) //function to find value of the last i bits
{
    int lastbits= n % (1<<globalDepth);
    return lastbits;
}
/*-------------------------------------------------------------------------------------*/
void ExtensibleHashTable:: doubleSize(int n)// doubles the size of the directory
{
    globalDepth++; //increment global depth
    int oldSize=directorySize;
    directorySize*=2; //double the directory size variable
    
    Bucket** temp=directory;  //create a temporary directory that points to old directory
    
    directory= new Bucket* [directorySize]; //make current directory point to new directory that is double in size
    
    for(int i=0; i < oldSize; i++){ //set elements of new directory same as old directory
        directory[i]=temp[i];
    }
    
    for(int i=oldSize; i<directorySize; i++){ //make all new entries point to respective buckets
        directory[i]=temp[i-oldSize];
    }
    
    delete[] temp;

}
/*-------------------------------------------------------------------------------------*/

void ExtensibleHashTable:: splitBucket(int value){
    
    //create a new bucket using the bucket constructor
    Bucket *newBucket= new Bucket;
    int lastBitsValue= lastBits(value);
    int oldHashKey= value % (1 << (directory[lastBitsValue]->localDepth)); //find out the oldBucket's hash key
    directory[oldHashKey]->localDepth++;
    newBucket->localDepth= directory[oldHashKey]->localDepth; //set the local depth of new bucket to the same as old bucket
    directory[oldHashKey]->entries= newBucket->entries; //set entries to 0 since we need to redistribute
    
    //move corresponding values to each bucket
    
    int j=0; // keeps track of the insertion index on newBucket
    int k=0; // keeps track of the insertion index on oldBucket
    
    for(int i=0; i<maxKeys; i++){
        int elementKey= directory[oldHashKey]->block[i] % (1<<(directory[oldHashKey]->localDepth));
        if(elementKey!=oldHashKey){
            newBucket->block[j]= directory[oldHashKey]->block[i];
            j++;
            newBucket->entries++;
        }
        else{
            directory[oldHashKey]->block[k]= directory[oldHashKey]->block[i]; //otherwise put it in the old bucket
            k++;
            directory[oldHashKey]->entries++;
        }
    }
    
    //readjust pointer to new bucket
    int newEntryValue= oldHashKey + (1<< (newBucket->localDepth-1)); //index of pointer to new bucket
    directory[newEntryValue]= newBucket;
    
    int difference= globalDepth - directory[oldHashKey]->localDepth; //find the difference between global and local of bucket
    
    if(difference>0){ //if the difference is greater then 0, then we have to adjust more pointers
        for(int i=newEntryValue+1; i<directorySize; i++){
            int directoryKey= i% (1<< (directory[oldHashKey]->localDepth));
            if(directoryKey==oldHashKey){
                directory[i]=directory[oldHashKey];
            }
            else if(directoryKey== newEntryValue)
                directory[i]= newBucket;
        }
    }
    //insert the value that overflowed original bucket
    insert(value);

}
/*-------------------------------------------------------------------------------------*/

bool ExtensibleHashTable:: find(int n) {
    if(n <  0) return false; //if n is negative we know it won't be in the table
    
    int key= lastBits(n);   //get the key value using lastBits function
    
    for (int i=0; i<maxKeys; i++){  //iterate through the block to find value
        if(directory[key]->block[i]==n) return true;    //if found then return true
    }
    
    return false;   //else return false
}
/*-------------------------------------------------------------------------------------*/

void ExtensibleHashTable:: insert(int n){
    int key= lastBits(n);
    
    if(directory[key]->entries<maxKeys){
        directory[key]->block[directory[key]->entries]=n;
        directory[key]->entries++;
    }
    
    else{
        int duplicates=0;
        for(int i=0; i<maxKeys; i++){
            if(directory[key]->block[i]==n) duplicates++;
        }
        if(duplicates==maxKeys) throw std::runtime_error("Bucket contains all duplicate values");
        
        if(globalDepth == (directory[key]->localDepth)){ //if local depth is same as global depth
            doubleSize(n);   //double the directory size
            splitBucket(n);   //split the bucket and rearrange values/pointers
        }
        else if(globalDepth> (directory[key]->localDepth)){ //if local depth is less than global depth
            splitBucket(n);
        }
    }
    
}
/*-------------------------------------------------------------------------------------*/

bool ExtensibleHashTable:: remove(int n) {
    int key= lastBits(n);
    
    if(find(n)==false) return false;    //if the search key is not in the block, return false
    
    else{
        int j=0; // keeps track of index for the block for rearranging elements
        
        for(int i=0; i<maxKeys; i++){
            if(directory[key]->block[i]==n){    //if the element is found
                directory[key]->block[i]=-1;    //set it to -1 (not necessary, but just in case)
                directory[key]->entries--;      //decrement number of entries in the block
            }
            
            else{
                directory[key]->block[j]=directory[key]->block[i];  //rearrange other elements
                j++;
            }
        }
         
        return true;
    }
}
/*-------------------------------------------------------------------------------------*/

void ExtensibleHashTable:: print(){
    for(int i=0; i<directorySize; i++){
        std::cout<<i<<": "<< std::hex<< directory[i]<<std::dec<<" --> ";
        
        //if the entry value is greater than 2^localDepth and localdepth< globaldepth
        if((i>= (1<< directory[i]->localDepth)) && (directory[i]->localDepth<globalDepth) ){
            std::cout<<std::endl;
            continue; //then we don't print any values
        }
        std::cout<<"["; //print the opening bracket
        
        for(int j=0; j<directory[i]->entries; j++){
            if(j!=(maxKeys-1))
            std::cout<<directory[i]->block[j]<<","; //print all entries
            else std::cout<<directory[i]->block[j];
                
        }
        
        for(int k=directory[i]->entries; k<maxKeys; k++){
            std::cout<<"-";     //print all empty spaces
            if( k < (maxKeys-1) ) std::cout<<","; //print commas for every entry except last
        }
        
        std::cout<<"] ("<<directory[i]->localDepth<<")"<<std::endl;
    }
}

