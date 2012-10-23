#  include  <iostream>
#  include  <list>
#  include  <vector>
#  include  <boost/graph/adjacency_list.hpp>
//#  include  "timer.hpp"
#include <lemon/time_measure.h>
#include <lemon/list_graph.h>
#include <lemon/smart_graph.h>


/*  ____ NOTES ______
 *
 *  • the mere access of vertex properties is as fast (even faster?)
 *    as accessing a list members
 *  • the complex way of accessing a vertex property gets completely
 *    optimzed away by the compiler
 *  • all of the slow down is in the vertex iterator facade
 *
 */




typedef
boost::adjacency_list <
   boost::listS,
   boost::listS,
   boost::bidirectionalS,
   double >
Graph;

typedef
boost::graph_traits< Graph > :: vertex_iterator
VertexIterator;

typedef
boost::graph_traits< Graph > :: vertex_descriptor
Vertex;

using namespace lemon;
using namespace std;


int main() {

    int runsNr  = 100000;
    int nodesNr = 100000;
    int trash;
    
   Graph  graph   ( nodesNr );
   double a = .0123;
    
    Timer T(true);

    T.restart();
    /*
   for ( int n = 0 ; n < runsNr ; ++n ) {

      VertexIterator   v , v_end;
      boost::tie( v , v_end ) = vertices(graph);

//      for ( int m = 0 ; m < 100000 ; ++m )   graph[*v] = (a *= 4.*(1.-a));

      while ( v != v_end ) {
//         graph[*v] = (a *= 4.*(1.-a));
//         get ( boost::vertex_bundle , graph )[*v] = (a *= 4.*(1.-a));
//         ((Graph::stored_vertex*)*v)->m_property = (a *= 4.*(1.-a));  // worx w/ list only!!
         ++v;
        trash++;
      }

   }
    
    cout << "Boost Zeit: " << T.realTime() << endl;
    T.restart();
     

//   for ( int n = 0 ; n < 100 ; ++n ) {
//      
//      auto
//         v      =  graph.vertex_set().begin(),
//         v_end  =  graph.vertex_set().end();
//      
//      while ( v != v_end ) {
////         graph[*v] = (a *= 4.*(1.-a));
//         ++v;
//      }
//   }



   std::list<double> liste ( nodesNr );


   for ( int n = 0 ; n < runsNr ; ++n ) {
      std::list<double>::iterator
         l      = liste.begin(),
         l_end  = liste.end();

//      for ( int m = 0 ; m < 100000 ; ++m )   *l = (a *= 4.*(1.-a));

      while ( l != l_end ) {
//         *l = (a *= 4.*(1.-a));
         ++l;
          trash++;
      }

   }
    cout << "List Zeit: " << T.realTime() << endl;


    
    T.restart();
    ListGraph myGraph;
    for (int i = 0; i < nodesNr; i++) myGraph.addNode();
    cout << "ListDigraph Generation Zeit: " << T.realTime() << endl;
    
    T.restart();
    for (int i = 0; i < runsNr; i++){
        ListGraph::NodeIt nd( myGraph );
        while ( nd != INVALID ) {
//            trash = myGraph.id( nd );
            trash++;
            ++nd;
        }
    }
        
    cout << "ListDigraph Zeit: " << T.realTime() << endl;    

    */
    
    T.restart();
    SmartDigraph mySmart;
    for (int i = 0; i < nodesNr; i++) mySmart.addNode();
    int trashSmart = 0;
    cout << "SmartDigraph Generation Zeit: " << T.realTime() << endl;
    
    T.restart();
    for (int i = 0; i < runsNr; i++) {
        SmartDigraph::NodeIt ns( mySmart );
        while (ns != INVALID) {
            trash = mySmart.id( ns ) * 4;
            trashSmart ++;
            ++ns;
        }
    }
    cout << "SmartDigraph Zeit: " << T.realTime() << endl;
    cout << "trash: " << trashSmart << " | SmartNodes: " << countNodes( mySmart ) << endl;
    
    
}


