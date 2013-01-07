//
//  BarabasiAlbert_Generator.cpp
//  NetEvoGAME
//
//  Created by Peter A. Kolski on 07.01.13.
//  Copyright (c) 2013 Peter. All rights reserved.
//


#include <netevo.h>
#include <iostream>
#include <lemon/full_graph.h>
#include <lemon/edge_set.h>
#include <lemon/maps.h>
#include <lemon/time_measure.h>

/*
    ToDo:
        Random seed;
        Calculate the probability
        Optimierung: Ohne die In/outDeg Map, sondern einzeln Degree berechnen
 */

using namespace std;
using namespace lemon;
using namespace netevo;

int main( void ){

    Timer   T(true);
    
    int init_nodes_num, final_nodes_num, edge_addition_num;
    
    init_nodes_num      = 10;
    final_nodes_num     = 10000;
    edge_addition_num   = 7;
    
    typedef ListEdgeSet< ListGraph >    EdgeSet;
    ListGraph                           mListGraph;
    EdgeSet                             mNewEdges( mListGraph );
    FullGraph                           fg(init_nodes_num);
    InDegMap<ListGraph>                 inDeg( mListGraph);
    OutDegMap<ListGraph>                outDeg( mListGraph);
    
    GraphCopy<FullGraph, ListGraph>     cg( fg, mListGraph); // Create the seed nodes
    cg.run();
    
    int randomNodeID = -1;
    int mNumEdges   = -1;
    int mNumNodes   = -1;
    EdgeSet::Edge e;
    lemon::Random   mRandom;
    ListGraph::Node newNode;
    
    for ( int i = init_nodes_num; i < final_nodes_num; i++){
        newNode     = mListGraph.addNode();
        mNumEdges   = countEdges( mListGraph );
        mNumNodes   = countNodes( mListGraph );
        mNewEdges.clear();
        
        while ( countEdges( mNewEdges ) != edge_addition_num ) {
            randomNodeID = mRandom.integer(0, mNumNodes - 1);
            // --- CALCULATE THE PROBABILITY
            e = findEdge( mNewEdges, newNode, mNewEdges.nodeFromId( randomNodeID ) ); // does the edge already exist?
            if ( e == INVALID){
                mNewEdges.addEdge(  newNode, mListGraph.nodeFromId( randomNodeID ) );
            }
        }
        // Create the new edges
        for (EdgeSet::EdgeIt e( mNewEdges ); e!=INVALID; ++e){
            mListGraph.addEdge( mNewEdges.u( e ), mNewEdges.v(e) );
        }
    }
    
    for (ListGraph::NodeIt n(mListGraph); n!=INVALID; ++n) {
        cout << "degree: " << inDeg[ n ] << endl;
    }
    cout << T.realTime() << endl;
    
    cout << countEdges( mListGraph) << endl;
    cout << countEdges( fg ) << endl;
}