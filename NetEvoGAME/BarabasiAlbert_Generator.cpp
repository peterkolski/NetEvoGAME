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

/*
    ToDo:
        Random seed;
        Calculate the probability
 */

using namespace std;
using namespace lemon;
using namespace netevo;

int main( void ){

    
    int init_num_nodes, max_nodes, edge_addition_num;
    
    init_num_nodes      = 10;
    max_nodes           = 100;
    edge_addition_num   = 7;
    
    typedef ListEdgeSet< ListGraph >    EdgeSet;
    ListGraph                           mListGraph;
    EdgeSet                             mNewEdges( mListGraph );
    FullGraph                           fg(init_num_nodes);
    
    GraphCopy<FullGraph, ListGraph>     cg( fg, mListGraph); // Create the seed nodes
    cg.run();
    
    int randomNodeID = -1;
    int mNumEdges   = -1;
    int mNumNodes   = -1;
    EdgeSet::Edge e;
    lemon::Random   mRandom;
    ListGraph::Node newNode;
    
    for ( int i = init_num_nodes; i < max_nodes; i++){
        newNode     = mListGraph.addNode();
        mNumEdges   = countEdges( mListGraph );
        mNumNodes   = countNodes( mListGraph );
        mNewEdges.clear();
        
        while ( countEdges( mNewEdges ) != edge_addition_num ) {
            randomNodeID = mRandom.integer(0, mNumNodes - 1);
            e = findEdge( mNewEdges, newNode, mNewEdges.nodeFromId( randomNodeID ) ); // does it exist already?
            if ( e == INVALID){
                mNewEdges.addEdge(  newNode, mListGraph.nodeFromId( randomNodeID ) );
            }
        }
        // Create the new edges
        for (EdgeSet::EdgeIt e( mNewEdges ); e!=INVALID; ++e){
            mListGraph.addEdge( mNewEdges.u( e ), mNewEdges.v(e) );
        }
    }
    
    cout << countEdges( mListGraph) << endl;
    cout << countEdges( fg ) << endl;
}