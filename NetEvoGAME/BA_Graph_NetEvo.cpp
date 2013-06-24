//
//  BA_Graph_NetEvo.cpp
//  NetEvoGAME
//
//  Created by Peter A. Kolski on 08.01.13.
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


#include <netevo.h>
#include <iostream>
#include <set>
#include <lemon/time_measure.h>
#include <lemon/random.h>


using namespace std;
using namespace lemon;
using namespace netevo;

class SystemPeter : public System{
public:
    void BarabasiAlbertGraph( int nodes, int arcPerAddition, string defNodeDyn, string defEdgeDyn){
        // Clear any existing structure
        clear();
        
        Random      mRandom;
        mRandom.seedFromTime();
        vector<int> nodeChoice;
        vector<int> targets(arcPerAddition, -1);
        set<int>    mRndNodes;
        int currentIndex = 0;
        
        // add seed nodes and mark them as targets
        for(auto &v : targets){
            v = currentIndex++;
            this->addNode(defNodeDyn);
        }
        
        while (countNodes(*this) < nodes ) {
            // Add new node and connect to targets
            currentIndex =  this->id( this->addNode(defNodeDyn) );
            for(const auto &v : targets ){
                this->addArc( this->nodeFromId( currentIndex ), this->nodeFromId( v ), defEdgeDyn );
            }
            // This determines the degree probability
            nodeChoice.insert(nodeChoice.end(), targets.begin(), targets.end() );
            nodeChoice.insert(nodeChoice.end(), arcPerAddition, currentIndex);  //degree of the new node
            
            mRndNodes.clear();
            while (mRndNodes.size() < arcPerAddition) {
                mRndNodes.insert( nodeChoice[ mRandom.integer( nodeChoice.size() ) ] );
            }
            targets.clear();
            targets.insert(targets.begin(), mRndNodes.begin(), mRndNodes.end() );
        }
        
        // Update the state ID mapping
        refreshStateIDs();
    }
    
    void BarabasiAlbertGraph( int nodes, int arcPerAddition ){
        BarabasiAlbertGraph(nodes, arcPerAddition, "NoNodeDynamic", "NoArcDynamic");
    }
};


int main( void ){
    
    Timer   T(true);
    
    
    SystemPeter mSys;
    mSys.BarabasiAlbertGraph( 1000, 7);
    
    
    cout << "time: " << T.realTime() << endl;
    cout << countNodes( mSys) << endl;
    cout << countArcs( mSys) << endl;
    
        
}


