//
//  VectorGame.cpp
//  NE2_SIR
//
//  Created by Peter A. Kolski on 14.11.11.
//  Copyright (c) 2011 Peter A. Kolski.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  For the GNU General Public License see <http://www.gnu.org/licenses/>.
//
//
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