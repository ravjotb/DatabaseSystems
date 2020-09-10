//
//  ExtensibleHashTable.h
//  HashTable
//
//  Created by Ravjot Brar on 2020-06-16.
//  Copyright © 2020 Ravjot Brar. All rights reserved.
//

#ifndef ExtensibleHashTable_h
#define ExtensibleHashTable_h
#include "Bucket.h"

class ExtensibleHashTable {

private:
    
    int globalDepth;
    int maxKeys;
    int directorySize;
    Bucket** directory;
    
    void operator=(const ExtensibleHashTable&);
    void doubleSize(int);
    void splitBucket(int);
    int lastBits(int);
    
    
public:
    
    ExtensibleHashTable();
    ExtensibleHashTable(int);
    ExtensibleHashTable(const ExtensibleHashTable& e, int);
    ~ExtensibleHashTable();
    
    bool find(int);
    void insert(int);
    bool remove(int);
    void print();
    
};

#endif /* ExtensibleHashTable_h */
