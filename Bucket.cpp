//
//  Bucket.cpp
//  HashTable
//
//  Created by Ravjot Brar on 2020-06-16.
//  Copyright © 2020 Ravjot Brar. All rights reserved.
//

#include "Bucket.h"

void Bucket:: operator=(const Bucket &b)
{
    localDepth=b.localDepth;
    entries=b.entries;
    
    for(int i=0; i<4; i++){
        block[i]= b.block[i];
    }
}
/*-------------------------------------------------------------------------------------*/

Bucket:: Bucket()
:localDepth(1), entries(0) {
    for(int i=0; i<4; i++){
        block[i]= -1;
    }
}
/*-------------------------------------------------------------------------------------*/

Bucket:: Bucket(const Bucket& b){
    localDepth= b.localDepth;
    entries= b.entries;
    
    for(int i=0; i<4; i++){
        block[i]=b.block[i];
    }
}
/*-------------------------------------------------------------------------------------*/

Bucket:: ~Bucket()
{
}
/*-------------------------------------------------------------------------------------*/


