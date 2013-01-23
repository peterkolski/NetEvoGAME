//
//  BA_GenCompare.cpp
//  NetEvoGAME
//
//  Created by Peter A. Kolski on 23.01.13.
//  Copyright (c) 2013 Peter. All rights reserved.
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