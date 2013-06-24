//
//  BarabasiAlbert_Generator.cpp
//  NetEvoGAME
//
//  Created by Peter A. Kolski on 07.01.13.
//  Copyright (c) 2013 Peter A. Kolski. All rights reserved.
//


//#include <netevo.h>
#include <iostream>
#include <lemon/list_graph.h>
#include <lemon/full_graph.h>
#include <lemon/edge_set.h>
#include <lemon/maps.h>
#include <lemon/time_measure.h>
#include <lemon/random.h>

/*
    ToDo:
        Random seed;
        Calculate the probability
        Optimierung: Ohne die In/outDeg Map, sondern einzeln Degree berechnen
 */

using namespace std;
using namespace lemon;
//using namespace netevo;

int main( void ){

    Timer   T(true);
    
    int init_nodes_num, final_nodes_num, edge_addition_num;
    
    init_nodes_num      = 10;
    final_nodes_num     = 50;
    edge_addition_num   = 7;
    
    typedef ListEdgeSet< ListGraph >    EdgeSet;
    ListGraph                           mListGraph;
    EdgeSet                             mNewEdges( mListGraph );
    FullGraph                           fg(init_nodes_num);
    
    GraphCopy<FullGraph, ListGraph>     cg( fg, mListGraph); // Create the seed nodes
    cg.run();
    
    int mNumEdges   = countEdges( mListGraph );
    EdgeSet::Edge e;
    lemon::Random   mRandom;
    mRandom.seedFromTime();
    ListGraph::Node newNode, randNode;
    
    // new edges will be saved seperatly in an EdgeSet, not to change the original node degrees
    for ( int i = init_nodes_num; i < final_nodes_num; i++){
        mNumEdges   = countEdges( mListGraph );
        mNewEdges.clear();
        newNode     = mListGraph.addNode();

        while ( countEdges( mNewEdges ) != edge_addition_num ) {
            randNode = mListGraph.nodeFromId( mRandom[ mListGraph.maxNodeId() ] ) ;
            // --- CALCULATE THE PROBABILITY
            if ( mRandom.real() < (( (double)(countIncEdges(mListGraph, randNode)) / (double)( 2*mNumEdges ) )) ){
                if ( findEdge( mNewEdges, newNode, randNode ) == INVALID){ // does the edge already exist?
                    mNewEdges.addEdge(  newNode, randNode );
                }
            }
        }
        
        // Create the new edges in the original graph
        for (EdgeSet::EdgeIt e( mNewEdges ); e!=INVALID; ++e){
            mListGraph.addEdge( mNewEdges.u( e ), mNewEdges.v(e) );
        }
    }
    
    cout << T.realTime() << endl;
    
    cout << countEdges( mListGraph) << endl;
    cout << countEdges( fg ) << endl;
    
 }