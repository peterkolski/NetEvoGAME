//
//  BA_GenCompare.cpp
//  NetEvoGAME
//
//  Created by Peter A. Kolski on 23.01.13.
//  Copyright (c) 2013 Peter A. Kolski.
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

#include "BA_Gen_3_fast_small.h"


#include <netevo.h>
#include <iostream>
#include <set>
#include <unordered_set>
#include <lemon/list_graph.h>

#include <lemon/edge_set.h>
#include <lemon/maps.h>
#include <lemon/time_measure.h>
#include <lemon/random.h>


using namespace lemon;
using namespace std;
using namespace netevo;

int main(){

    SystemSmallBA   mNewSystem;
    System          mSys;
    Timer   Tm(true);
    
    Tm.restart();
    mSys.BarabasiAlbertGraph(50000, 0.01);
    cout    << Tm.realTime() << endl;
    cout << "Arcs"<< countArcs( mSys ) << endl << endl;
    mSys.clear();

    Tm.restart();
    mNewSystem.BarabasiAlbertGraph(50000, 0.01, "NoNodeDynamic", "NoArcDynamic");
    cout    << Tm.realTime() << endl;
    cout << "Arcs"<< countArcs( mNewSystem ) << endl << endl;
    
//    ListDigraph mCopy;
//    
//    Tm.restart();
//    digraphCopy( mNewSystem, mCopy).run();
//    cout    << Tm.realTime()        << endl;
//    
//    usleep(4000000);
//    cout << "Arcs"<< countArcs( mCopy ) << endl << endl;



    
}