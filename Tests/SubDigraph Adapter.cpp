//
//  FromScratch.cpp
//  NE2_SimODE
//
//  Created by Peter A. Kolski on 27.09.11.
//  Copyright 2011 Peter A. Kolski. All rights reserved.
//


#include <netevo.h>
#include <lemon/adaptors.h>

using namespace netevo;
using namespace lemon;


class MyDynamics : public NodeDynamic {
public:
    
    string  getName()   { return "MyDyn"; }
    int     getStates() { return 1; }
    void    setDefaultParams( Node n, System &sys )  {  
        sys.nodeData( n ).dynamicParams.push_back( 2.0 );  // erster Parameter
        sys.nodeData( n ).dynamicParams.push_back( 1.0 );  // zweiter Parameter
    }
    
    void    fn ( Node n, System &sys, const State &x, State &dx, const double t){

        // Parameter abgreifen 
        vector< double > &myParams = sys.nodeData( n ).dynamicParams;
        
        // ZustandIndex abgreifen
        int nodeID  = sys.stateID( n );
        
        // Dynamik festlegen
        dx[ nodeID ] = myParams[ 0 ] * myParams[ 1 ] * x[ nodeID ];
        
    }

};



int main(){

    // systeme erstellen
    ListDigraph lemonGraph;
    System origSys, adapSys;
    
    // Topologie erstellen
    Node a = lemonGraph.addNode();
    Node b = lemonGraph.addNode();
    Node c = lemonGraph.addNode();
    
    lemonGraph.addArc( a, b );
    lemonGraph.addArc( b, c );
    lemonGraph.addArc( c, a );
    

    
    // Apadpter erstellen
    
    
    // Adapter kopieren
    origSys.copyDigraph( lemonGraph    );

    // FilterMap erstellen
    ListDigraph::NodeMap< bool >    mNodeFilterMap( origSys );
    ListDigraph::ArcMap< bool >     mArcFilterMap ( origSys );
    
    SubDigraph< ListDigraph >::SubDigraph subLemonGraph( origSys, mNodeFilterMap, mArcFilterMap );

//    adapSys.copyDigraph( subLemonGraph );   // Fuktioniert also nicht

    MyDynamics pDyn;

    
//    pSys.randomGraph( 0.2, 3, false, "MyDyn", "NoArcDynamic", false );
    
    vector< double >    initValues = State( origSys.totalStates(), 1.0 );
    SimObserverToStream obs( cout );
    ChangeLog   nullLog;
    
    
    SimulateOdeFixed simu = SimulateOdeFixed( RK_4, 0.005 );
    simu.simulate( origSys, 2, initValues, obs, nullLog );
    
    return 0;
}