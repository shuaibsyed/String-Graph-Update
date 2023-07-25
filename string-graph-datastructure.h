/* string-graph-datastructure by Shuaib Syed*/

/* This header file defines the data structure that will be used for
 * storing graphs. 
 */

/* Structure for edge */
#ifndef edge_def
#define edge_def
typedef struct edge {
    int weight;
    char *dest;
    struct edge *next;
} Edge;
#endif

/* Structure for vertex */
#ifndef vertex_def
#define vertex_def
typedef struct vertex {
    int edge_count;
    char *name;
    struct edge *edge_head; /* Linked list of edges */
} Vertex;
#endif

/* Structure for graph */
#ifndef graph_def
#define graph_def
typedef struct graph {
    int vertex_count;
    struct vertex **vertex_array; /* Array of vertices */
} String_graph;
#endif