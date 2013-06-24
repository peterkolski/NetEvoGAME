//
//  AndreCompasion.cpp
//  NE2_SIR
//
//  Created by Peter A. Kolski on 11.11.11.
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
#include <netevo.h>
#include <iostream>
#include <lemon/lgf_reader.h>
#include <lemon/lgf_writer.h>
#include <lemon/adaptors.h>
#include <lemon/time_measure.h>

#include <algorithm>
//#include <boost/numeric/odeint.hpp>   // Der Linker beschwert sich
#include <boost/numeric/odeint/stepper/runge_kutta4.hpp>

using namespace netevo;
using namespace lemon;
using namespace boost::numeric;

void    initDegreeDistributed( System &sys, State &initial, int states, double degreeFactor );
void    sysDegreeDistributedParams( System &sys, double degreeFactor );
int     readGraph( ListDigraph &graph, const string &path );
bool    terminate( const State &toCheck, double threshold, int parameterNr, int statesAmount );
double  sizeOfEpi( const State &toCheck , int parameterNr, int statesAmount );
int     countNodeNoInArcs( const System &sys );
void    printCout( double var );


class SIRdynamics : public NodeDynamic {
public:
    string  getName()   { return "SIR_Dynamic"; }
    int     getStates() { return 3; }
    void    setDefaultParams( Node n, System &sys ){
        sys.nodeData( n ).dynamicParams.push_back( 2.0 );   // BETA
        sys.nodeData( n ).dynamicParams.push_back( 0.1 );   // LAMDA
    }
    
    void fn( Node n, System &sys, const State &x, State &dx, const double t ) {
        int nodeID = sys.stateID( n );
        vector< double > &nodeParams = sys.nodeData( n ).dynamicParams;
        double N = x[ nodeID ] + x[ nodeID + 1 ] + x[ nodeID + 2 ];
        
        dx[ nodeID     ] = ( (-1) * x[ nodeID ] * x[ nodeID + 1 ] * nodeParams[ 0 ]  ) / N;
        dx[ nodeID + 1 ] = ( x[ nodeID ] * x[ nodeID + 1 ] * nodeParams[ 0 ]  ) / N - nodeParams[ 1 ] * x[ nodeID + 1 ];
        dx[ nodeID + 2 ] = nodeParams[ 1 ] * x[ nodeID + 1 ];
    }
};


class InteractionMap    : public NodeDynamic {
public:
    string  getName()   { return "InteractionMap"; }
    int     getStates() { return 3; }
    void    setDefaultParams( Node n, System &sys ){
        sys.nodeData( n ).dynamicParams.push_back( 0.01 );  // Parameter um die Transport-Rate festzulegen
        //        sys.nodeData( n ).dynamicParams.push_back( 1 );  // Population size
    }
    
    void    fn( Node n, System &sys, const State &x, State &dx, double t ){
        
        double nodeID = sys.stateID( n );
        
        if ( countInArcs( sys, n ) > 0 ){
            double sourcePopulation, nodePopulation;
            double unchangedRest, demandRatio;
            
            double sumS = 0.0;
            double sumI = 0.0;
            double sumR = 0.0;
            
            
            // ----- Get all the States of the sources
            for (System::InArcIt Arc( sys, n ); Arc != INVALID; ++Arc ){
                sumS += x[ sys.stateID( sys.source( Arc ) )     ];
                sumI += x[ sys.stateID( sys.source( Arc ) ) + 1 ];
                sumR += x[ sys.stateID( sys.source( Arc ) ) + 2 ];
            }
            
            // ----- Population amount
            sourcePopulation = sumS + sumI + sumR;
            if ( sourcePopulation <= 0 ) cout << " FEHLER!! " << endl << endl;  // Fehler abfangen
            nodePopulation =  x[ nodeID ] + x[ nodeID + 1 ] + x[ nodeID + 2 ];            
            
            // ----- Amount of 'demand' for the current node
            unchangedRest  = 1 - sys.nodeData( n ).dynamicParams[ 0 ];
            demandRatio =     sys.nodeData( n ).dynamicParams[ 0 ];
            
            // ----- Only the 'demand' amount will be scaled and changed by the interaction
            dx[ nodeID     ] = x[ nodeID     ] * unchangedRest + ( sumS * nodePopulation / sourcePopulation * demandRatio );
            dx[ nodeID + 1 ] = x[ nodeID + 1 ] * unchangedRest + ( sumI * nodePopulation / sourcePopulation * demandRatio );
            dx[ nodeID + 2 ] = x[ nodeID + 2 ] * unchangedRest + ( sumR * nodePopulation / sourcePopulation * demandRatio );
        }
        else{
            dx[ nodeID     ] = x[ nodeID     ] ;
            dx[ nodeID + 1 ] = x[ nodeID + 1 ] ;
            dx[ nodeID + 2 ] = x[ nodeID + 2 ] ;
        }
    }
    
};


class SimulateOdeStep   : public SimulateOdeFixed{
    public  :
    SimulateOdeStep( double StepSize );   // Konstruktor
    void simulateStep(System &sys, double time, State &inital );
    
    private :
    odeint::runge_kutta4< State > mRk4Stepper;
    double mStepSize;
};

SimulateOdeStep::SimulateOdeStep( double StepSize ) : SimulateOdeFixed( RK_4, StepSize ){
    mStepSize = StepSize;
}


void SimulateOdeStep::simulateStep(System &sys, double time, State &inital ){
    
    // Check to ensure that initial conditions are correct size
    //    int states = (countNodes(sys)*sys.nodeStates()) + (countArcs(sys)*sys.arcStates());
    //    if (inital.size() < states || inital.size() > states) {
    //        cerr << "Incorrect number of states for initial conditions (SimulateOdeFixed::simulate)" << endl;
    //        return;
    //    }	 
    
    // Check that the state IDs are correct, if not refresh
    if (!sys.validStateIDs()) { sys.refreshStateIDs(); }
    
    mRk4Stepper.do_step( Simulator(&sys), inital,  time, mStepSize ); 
    
}



int main(){
    ListDigraph     lemonGraph;
    System pSys,    mapSys;
    SIRdynamics     mDyn;
    InteractionMap  interDyn;
    
    // ----- Assign Dynamics to the Systems
    pSys.addNodeDynamic( &mDyn );
    mapSys.addNodeDynamic( &interDyn );
    
    // ----- Time measurement
    Timer T(true);
    Timer Tlocal(true);
    cout << "Start!" << endl;
    
    // ----- Read the Graph from file to a LEMON System
    // I use that, to have two Systems with identical Topologies
    readGraph( lemonGraph, "/Users/sonneundasche/Dropbox/NetEvo/21 LEMON Graphs/super_lemon_schaf.lgf");
    pSys.copyDigraph( lemonGraph, "SIR_Dynamic", "NoArcDynamic");
    mapSys.copyDigraph( lemonGraph, "InteractionMap", "NoArcDynamic");  
    cout << "Nodes: " << countNodes( pSys ) << endl;
    cout << "Arcs: "  << countArcs( pSys )  << endl;
    cout << "No In Arcs: " << countNodeNoInArcs( pSys ) << endl;
    
    // ----- Anfangsbedingungen abhängig von dem Grad des Knotens
    State   init = State( pSys.totalStates());
    initDegreeDistributed( pSys, init, mDyn.getStates(), 5.0 );
    init[ 3 * (int)(pSys.rnd() *  pSys.totalStates() / mDyn.getStates() ) + 1 ] = 5.0 ; // Zufällig den zweiten Wert (I) infizieren    
    
    // ----- Logger & Observer
    ChangeLog   nullLog;
    vector<double> tOut;
    vector<State>  xOut;
    SimObserverToVectors vectorObserver(xOut, tOut);
    SimObserverToStream obs( cout );
    SimObserver         nullObserver;
    
    
    // ----- Variables
    double StepSize = 0.001;
    bool endSim = false;
    double tMax = 5.0;
    int     globalSteps = 0;
    double t = 0.0;
    double thresholdTerminate = 0.1;
    double sizeOfEpidemic = 0.0;
    double lastSizeOfEpidemic = -1;
    int    StateParamsAmount  = mDyn.getStates();
    SimulateOdeFixed    ODE_Solver( RK_4, StepSize);
    SimulateMap         InterActionMap;
    SimulateOdeStep     ODE_Stepper( StepSize );
    
    cout << "Initialisation Time: " << T.realTime() << endl << endl;
    cout << "StepSize: " << StepSize << " --- Maximum time: " << tMax << " --- Steps: " << tMax / StepSize << endl << endl;
    cout << " --- Start Simulation!!! ---" << endl << endl;
    cout << "Initial Size of Epidemic: " << sizeOfEpi( init, 1, 3 ) << endl;
    T.restart();    // Start the timer
    
    /*
     // ----------------------- SIMULATION LOOP --------------------------
     // --- Swap sequentially between ODE Solution and Interaction Map ---    
     // ------------------------------------------------------------------
     while ( !endSim ) {
     t = 0.0;
     Tlocal.restart();
     
     // ----- ODE Simulation -----
     // 
     //        cout << "ODE Simulation: " << endl;
     //        while (  (t <= tMax) &&  !( endSim = terminate( init, thresholdTerminate, 1, StateParamsAmount ) )   ) {
     while (  t <= tMax    ) {        // Das macht den Stepper eigentlich überflüssig, so kann man simulate() benutzen
     ODE_Stepper.simulateStep( pSys, 0, init);
     //            cout << "t = " << t << " :: "; for_each ( init.begin(), init.end(), printCout ); cout << endl;
     t += StepSize;
     }        
     
     cout << "\tODE SimulationsZeit " << Tlocal.realTime() << " sek" << endl;
     Tlocal.restart();
     
     // ----- Interaction / Trade Simulation ------
     //        cout << "Interaction: " << endl;
     InterActionMap.simulate( mapSys, 1, init, nullObserver, nullLog );
     //        for_each( init.begin(), init.end(),printCout );
     cout << "Interaction SimulationsZeit:\t " << Tlocal.realTime() << " sek = " << Tlocal.realTime() / 60.0 << " min" << endl;
     Tlocal.restart();
     
     // ----- Size of Epidemic -----
     lastSizeOfEpidemic = sizeOfEpidemic;
     sizeOfEpidemic = sizeOfEpi( init, 1, 3 );
     cout << "Step = " << globalSteps << " calculation time: " << Tlocal.realTime() <<  " --- Size of Epidemic: " << sizeOfEpidemic << endl;  
     
     // ----- Termination Condition -----
     // Bis zur dritten NachkommaStelle gleich
     endSim = ( ((int)sizeOfEpidemic * 1000) == ((int)lastSizeOfEpidemic * 1000) ); 
     globalSteps++;
     }
     
     */
    
    ODE_Solver.simulate( pSys, tMax,  init,  nullObserver,  nullLog );
    
    cout << "t = " << t << " - Size of Epidemic: " << sizeOfEpi( init, 1, 3 ) << endl;
    
    cout << "Run Time: " << T.realTime() << " Sek -> " << T.realTime() / 60 << " min"<< endl;
    cout << "Runs: " << globalSteps / StepSize << endl;
    cout << endl << "Fertig, du alte Maus!" << endl;
    
    return 0;
}


/*--------------------------------------------------------------------------------------------------------
 ----------------------------------------------------------------------------------------------------- */


void initDegreeDistributed( System &sys, State &initial, int states, double degreeFactor ){
    int nodeNr = 0;     // Index des Knoten, um keinen Iterator zu benutzen
    
    for ( System::NodeIt n (sys); n != INVALID; ++n ){
        unsigned int degree = 0;
        for ( System::InArcIt a( sys, n ); a != INVALID; ++a) {
            degree++;
        }
        //        initial[ (nodeNr * states ) ] = degree * degreeFactor + 10;   //Anzahl des Grades 
        initial[ (nodeNr * states ) ] = 30;   //Anzahl des Grades 
        nodeNr++;
    }
    //    for_each( initial.begin(), initial.end(),printCout );
}

void sysDegreeDistributedParams( System &sys, double degreeFactor ){
    
    for ( System::NodeIt n (sys); n != INVALID; ++n ){
        unsigned int degree = 0;
        for ( System::InArcIt a( sys, n ); a != INVALID; ++a) {
            degree++;
        }
        sys.nodeData( n ).dynamicParams[ 1 ] = degree * degreeFactor;   //Anzahl des Grades 
    }
    
    //    for_each( initial.begin(), initial.end(),printCout );
}



int readGraph( ListDigraph &graph, const string &path ){
    try {
        digraphReader( graph, path)
        .run();
    } catch (Exception& error) { // check if there was any error
        std::cerr << "Error: " << error.what() << std::endl;
        return -1;
    }
    return 0;
}


bool terminate( const State &toCheck, double threshold, int parameterNr, int statesAmount ){
    
    unsigned int  size   = toCheck.size();
    unsigned int  i  = 0;
    unsigned int  ZugriffsIndex = 0;
    bool allCheckedAreBelow = true;
    
    do {
        ZugriffsIndex = i * statesAmount + parameterNr;
        allCheckedAreBelow = ( threshold > toCheck[ ZugriffsIndex ]  );
        i++;
    } while  ( allCheckedAreBelow && ( ZugriffsIndex < size ) );
    
    return allCheckedAreBelow;
}

// ------
// Sums the second elements of the 3-state SIR vector
double sizeOfEpi( const State &toCheck , int parameterNr, int statesAmount ){
    double  sumEpi = 0.0;
    int     nodesAmound = toCheck.size() / statesAmount;
    for (int i = 0; i < nodesAmound; i++ )
        sumEpi += toCheck[ i * statesAmount + parameterNr ]; 
    
    return sumEpi;
}

int countNodeNoInArcs( const System &sys ){
    int InArcsCount;
    int noIAcount = 0;
    for ( ListDigraph::NodeIt n( sys ); n != INVALID; ++n ){
        InArcsCount = 0;
        for ( ListDigraph::InArcIt a( sys, n ); a != INVALID; ++a )  InArcsCount++;
        if ( InArcsCount == 0 ) noIAcount++;
    }
    
    return noIAcount;
}

void printCout( double var ){
    cout << var << " : ";
}
