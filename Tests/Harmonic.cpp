//
//  FromScratch2.cpp
//  NE2_SimODE
//
//  Created by Peter A. Kolski on 28.09.11.
//  Copyright 2011 BildPeter Visuals. All rights reserved.
//

#include <netevo.h>


using namespace netevo;
using namespace lemon;


class peterDynamic : public NodeDynamic {
public:
    string  getName()   { return "peterDynamic"; }
    int     getStates() { return 2; }
    void    setDefaultParams( Node n, System &sys ) {
        sys.nodeData( n ).dynamicParams.push_back( 0.15 );   // gamma
    }

    void    fn( Node n, System &sys, const State &x, State &dx, const double t ){
        
        int nID = sys.stateID( n );  // Dies gibt mir die ID des aktuellen Knotens wider
        vector< double >    &gamma = 
        
        dx[ nID    ] =  x[ nID + 1 ] ;
        dx[ nID +1 ] = -x[ nID     ] - x[ nID + 1 ] ;
    }
};



int main(){
    System pSys;
    peterDynamic pDyn;
    pSys.addNodeDynamic( &pDyn );
    
    pSys.ringGraph( 10, 1, false );
    
    State inital = State( pSys.totalStates(), 10.0 );
    ChangeLog   nullLog;
    SimObserverToStream observer( cout );
    SimulateOdeFixed peteSim = SimulateOdeFixed( RK_4,  0.01 );
    
    peteSim.simulate( pSys, 10, inital, observer, nullLog );
 
    return 0;
}