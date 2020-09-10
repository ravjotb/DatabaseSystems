//
//  Bucket.h
//  HashTable
//
//  Created by Ravjot Brar on 2020-06-16.
//  Copyright © 2020 Ravjot Brar. All rights reserved.
//

#ifndef Bucket_h
#define Bucket_h

class Bucket {
    
public:
    int block[4];
    int localDepth;
    int entries;
    
    void operator=(const Bucket& b);
    Bucket();
    Bucket(const Bucket& b);
    ~Bucket();
    
};


#endif /* Bucket_h */
