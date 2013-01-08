//
//  BA_Gen_Two.cpp
//  NetEvoGAME
//
//  Created by Peter A. Kolski on 08.01.13.
//  Copyright (c) 2013 Peter. All rights reserved.
//



//#include <netevo.h>
#include <iostream>
#include <set>
#include <unordered_set>
#include <lemon/list_graph.h>
#include <lemon/full_graph.h>
#include <lemon/edge_set.h>
#include <lemon/maps.h>
#include <lemon/time_measure.h>
#include <lemon/random.h>
#include <lemon/lgf_writer.h>


/*
 ToDo:

 */

using namespace std;
using namespace lemon;
//using namespace netevo;

int main( void ){
    
    Timer   T(true);
    
    int final_nodes_num, edge_addition_num;
    
    final_nodes_num     = 1000000;
    edge_addition_num   = 7;
    
    ListGraph                           mGr;
    
    lemon::Random   mRandom;
    mRandom.seedFromTime();
    vector<int> nodeChoice;
//    set<int>    mRndNodes;
    unordered_set<int> mRndNodes;
    vector<int> targets(edge_addition_num, -1);
    int currentIndex = 0;
    
    // First targets are all nodes
    for(auto &v : targets){
        v = currentIndex++;
        mGr.addNode();
    }

    while (countNodes(mGr)<final_nodes_num ) {
        // Add new node and connect to targets
        currentIndex =  mGr.id( mGr.addNode() );
        for(const auto &v : targets ){
            mGr.addEdge( mGr.nodeFromId( currentIndex ), mGr.nodeFromId( v ) );
        }
        // Add the nodes, which were connented again
        nodeChoice.insert(nodeChoice.end(), targets.begin(), targets.end() );
        nodeChoice.insert(nodeChoice.end(), edge_addition_num, currentIndex);
        
        mRndNodes.clear();
        while (mRndNodes.size() < edge_addition_num) {
            mRndNodes.insert( nodeChoice[ mRandom.integer( nodeChoice.size() ) ] );
        }
        targets.clear();
        targets.insert(targets.begin(), mRndNodes.begin(), mRndNodes.end() );
        
    }
    
    cout << "time: " << T.realTime() << endl;
    cout << countNodes( mGr) << endl;
    cout << countEdges( mGr) << endl;
    
    
    graphWriter( mGr, "/Users/sonneundasche/Documents/FLI/DATA/05 LEMON Graphs/BaraBasiTEST.txt")
    .run();
    
    InDegMap<ListGraph>     inDeg(mGr);

    graphWriter( mGr, "/Users/sonneundasche/Documents/FLI/DATA/05 LEMON Graphs/BaraBasi_Degree_TEST.txt")
    .nodeMap("degree", inDeg)
    .skipEdges()
    .run();
    
  }