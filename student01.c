#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "string-graph.h"

/**
 * This test adds and removes verticies multiple times
 * It also tests destroying/recreating a graph
 */

int main() {
  String_graph graph;
  char *vertices_to_add[]= {"koala", "quokka", "parrot", "zebra", "hedgehog",
                            "dog", "frog"};
  char *vertices_to_add2[]= {"koala", "quokka", "parrot", "zebra", "hedgehog",
                            "dog", "frog"};
  int i;

  graph_init(&graph);

  /* Add verticies */
  for (i = 0; i < 7; i++)
    add_vertex_to_graph(&graph, vertices_to_add[i]);
  assert(vertex_count(&graph) == 7);

  /* Remove them */
  for (i = 0; i < 7; i++)
    remove_vertex_from_graph(&graph, vertices_to_add[i]);
  assert(vertex_count(&graph) == 0);

  /* Destroy graph */
  graph_delete(&graph);

  /* Add Verticies */
 for (i = 0; i < 7; i++)
   add_vertex_to_graph(&graph, vertices_to_add2[i]);
 assert(vertex_count(&graph) == 7);

 /* Remove them */
 for (i = 0; i < 7; i++)
   remove_vertex_from_graph(&graph, vertices_to_add2[i]);
 assert(vertex_count(&graph) == 0);

  printf("Pass!\n");

  return 0;
}