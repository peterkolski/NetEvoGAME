//
//  NE2_Reference_LEMON_Map.cpp
//
//  Created by Peter A. Kolski on 28.11.11.
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

#include <iostream>
#include <netevo.h>
#include <lemon/random.h>

using namespace netevo;
using namespace lemon;


/*
 In this example it is intended to copy the values from a node maps of a LEMON graph
 into a NetEvo system, which was copied from the original LEMON graph via .copyDigraph()
 
 To do this, a cross reference map stores the nodes of the original LEMON system, and 
 can be accessed by the current NetEvo system node.
 */


int main() {
    
    // ----- Create a LEMON graph and a node map to store values -----
    ListDigraph                     lemonSourceGraph;
    ListDigraph::NodeMap< int >     lemonSourceNodeMap( lemonSourceGraph );

    // ----- Do the same for a NetEvo system  -----
    System                          netevoSys;
    System::NodeMap< int >          netevoTargetNodeMap ( netevoSys );

    // ----- Create empty cross reference maps for the NetEvo system storing source nodes  -----
    ListDigraph::NodeMap< ListDigraph::Node >  refNodeMap( netevoSys );
    ListDigraph::ArcMap< ListDigraph::Arc >    refArcMap(  netevoSys ); 
    
    // ----- Create some values for the LEMON node map -----
    for ( ListDigraph::NodeIt lNode (lemonSourceGraph ); lNode != INVALID; ++lNode ){
        lemonSourceNodeMap[ lNode ] = rand();
    }

    // ----- Copy the LEMON graph into a NetEvo System, passing through the reference maps -----
    netevoSys.copyDigraph( lemonSourceGraph, "InteractionMap", "NoArcDynamic", refNodeMap, refArcMap);
    
    // ----- Assigning the source values to the new NetEvo system  -----
    for (System::NodeIt neNode( netevoSys ); neNode != INVALID; ++neNode ){
        netevoTargetNodeMap[ neNode ] = lemonSourceNodeMap [ refNodeMap[ neNode ] ] ;
        cout << netevoTargetNodeMap[ neNode ] << endl;
    }
    
    
    return 0;
}
