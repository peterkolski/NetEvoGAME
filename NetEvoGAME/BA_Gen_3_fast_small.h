//
//  BA_Gen_3_fast_small.h
//  NetEvoGAME
//
//  Created by Peter A. Kolski on 23.01.13.
//  Copyright (c) 2013 Peter. All rights reserved.
//

#ifndef NetEvoGAME_BA_Gen_3_fast_small_h
#define NetEvoGAME_BA_Gen_3_fast_small_h


#include <netevo.h>
#include <iostream>
#include <set>
#include <unordered_set>

#include <lemon/random.h>
#include <lemon/smart_graph.h>


/*
 ToDo:
 
 */

using namespace std;
using namespace lemon;

class SystemSmallBA : public netevo::System{
//        SmartDigraph    tmpGraph;
public:
    bool BarabasiAlbertGraph( int nodes, double density, string defNodeDyn, string defEdgeDyn){
        
        
        if ( density * (double)( nodes - 1 ) < 1 ){
            cout << "WARNING!" << endl
            << "Graph not built" << endl
            << "Minimum density to recieve a connected Barabasi-Albert-Graph is not given." << endl
            << " Choose a density higher than: density * (nodes - 1)  >= 1" << endl;
            return false;
        }
        else
        {
            // Clear any existing structure
            clear();
            
            // for undirected graphs
            //TODO: choose directed/undirected - will impact the # of arcs
            int                                 arcPerAddition = ceil( density * (double)( nodes - 1 ) );
            Random                              mRandom;
            mRandom.seedFromTime();
            std::set<netevo::System::Node>    mTargets;
            netevo::System::Node                newNode;
            
            // add seed nodes and mark them as targets
            for(int i = 0; i<arcPerAddition;i++){
                newNode = this->addNode(defNodeDyn);
//                newNode = tmpGraph.addNode();
                mTargets.insert(newNode);
            }
            
            while (countNodes(*this) < nodes ) {
                // Add new node and connect to targets
                newNode =  this->addNode(defNodeDyn);
//                newNode =  tmpGraph.addNode();
                // Arc to add are chosen in the run before (see below)
                for(const auto &v : mTargets ){
                    this->addArc( newNode, v , defEdgeDyn );
//                    tmpGraph.addArc( newNode, v  );
                }
                
                // choose the nodes to which to connect the new node
                mTargets.clear();
                while (mTargets.size() < arcPerAddition) {
                    // choose arc
                    netevo::System::Arc currentArc = this->arcFromId( mRandom.integer( this->maxArcId() ) );
                    // choose source or target AND insert
                    if ( mRandom.boolean() )
                        mTargets.insert( this->target( currentArc ) );
                    else
                        mTargets.insert( this->source( currentArc ) );
                }
            }
            
            // copy the intermediate graph to the resulatin one
//            digraphCopy( tmpGraph, *this ).run();
//            tmpGraph.clear();
            
            // Update the state ID mapping
            refreshStateIDs();
//            this->setNodeDynamic_all(defNodeDyn);
//            this->setArcDynamic_all(defEdgeDyn);
            
            return true;
        }
    }
};



#endif
