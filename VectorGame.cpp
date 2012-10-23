//
//  VectorGame.cpp
//  NE2_SIR
//
//  Created by Peter A. Kolski on 14.11.11.
//  Copyright (c) 2011 BildPeter Visuals. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <vector.h>

using namespace std;

int main( void ){

    vector< double > myVec;
    
    for (int i = 0; i < 10; i++) {
        myVec.push_back( i );
    }

    for ( vector< double >::iterator iter = myVec.begin(); iter != myVec.end() ; iter++) {
        cout << *iter << endl;
    }
    
    cout << endl;
    
    vector< double >::iterator iter = myVec.end();
    iter--;
    cout << *iter << endl;    
    iter--;
    cout << *iter << endl;    
    iter-- ; iter--;
    cout << *iter << endl;        
    
    return 0;
    

}