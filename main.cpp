//
//  main.cpp
//  HashTable
//
//  Created by Ravjot Brar on 2020-06-16.
//  Copyright © 2020 Ravjot Brar. All rights reserved.
//

#include <iostream>
#include "ExtensibleHashTable.h"
#include <stdlib.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    ExtensibleHashTable test;
    
    for(int i=0; i<1000; i++){
        test.insert(rand()%10000);
    }
    test.print();
    test.remove(0);
    test.remove(1);
    std::cout<<"\n After Removal: "<<"\n"<<std::endl;
    test.print();
}
