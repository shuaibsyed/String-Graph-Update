/* string-graph by Shuaib Syed */

/* I pledge on my honor that I have not given or received any
 * unauthorized assitance on this assignment. */

/* This program stores graphs using a array of linked lists as the
 * underlying data structure. */

#include "string-graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Helper Function Prototypes */
static Edge *find_edge_tail(const String_graph *const graph,
                            const char source[]);
static Edge *find_existing_edge(const String_graph *const graph,
                                const char source[], const char dest[]);
static Vertex *find_existing_vertex(const String_graph *const graph,
                                    const char name[]);
static int compare_chars(const void *a, const void *b);

/* This function intializes the graph as an empty graph with zero vertices
 * and edges. */
void graph_init(String_graph *const graph) {
    /* Variables */
    struct graph *g = NULL;

    /* Checks */
    if (!graph)
        return;
    if ((g = malloc(sizeof(String_graph)))) {
        g->vertex_count = 0;
        g->vertex_array = malloc(sizeof(struct vertex *));
        *graph = *g;
        free(g);
    }
}

/* This function adds a vertex to the graph without connecting it to any
 * other vertices. */
short add_vertex_to_graph(String_graph *const graph,
                          const char new_vertex[]) {
    /* Variables */
    struct vertex *vertex = NULL;
    char *name;

    /* Checks */
    if (!graph || !new_vertex || isa_vertex(graph, new_vertex))
        return 0;

    /* Allocate new name */
    if ((name = malloc(strlen(new_vertex) + 1)))
        strcpy(name, (new_vertex ? new_vertex : ""));
    else
        return 0;

    /* Allocate space for new vertex pointer */
    if (!(graph->vertex_array =
              realloc(graph->vertex_array, (graph->vertex_count + 1) *
                                               sizeof(struct vertex *))))
        return 0;

    /* Allocate space for new vertex */
    if ((vertex = malloc(sizeof(struct vertex) + sizeof(int) +
                         sizeof(char *) + sizeof(struct edge *)))) {
        vertex->edge_count = 0;
        vertex->name = name;
        vertex->edge_head = NULL;
    } else
        return 0;

    /* Default */
    graph->vertex_array[graph->vertex_count] = vertex;
    graph->vertex_count++;
    return 1;
}

/* This function returns the number of vertices in the graph. */
short vertex_count(const String_graph *const graph) {
    /* Checks */
    if (!graph || !graph->vertex_count || graph->vertex_count <= 0)
        return 0;

    /* Return */
    return graph->vertex_count;
}

/* This function returns 1 if the graph contains a vertex with the
 * parameter name. Returns 0 if the vertex is not in the graph. */
short isa_vertex(const String_graph *const graph, const char name[]) {
    /* Variables */
    int index;

    /* Checks */
    if (!graph || !name)
        return 0;
    if (!graph->vertex_array || !graph->vertex_count)
        return 0;

    /* Loops */
    for (index = 0; index < graph->vertex_count; index++) {
        if (strcmp(graph->vertex_array[index]->name, name) == 0)
            return 1;
    }

    /* Default */
    return 0;
}

/* This function returns a dynamically-allocated array of pointers to the
 * vertices in the graph. The pointers are sorted in alphabetical order. If
 * the graph is NULL then the function returns NULL. */
char **get_vertex_list(const String_graph *const graph) {
    /* Variables */
    char **verticies = NULL;
    int index;
    int array_size = graph->vertex_count + 1; /* Space for NULL */

    /* Checks */
    if (!(verticies = malloc((array_size) * sizeof(char *))))
        return NULL;
    if (!graph)
        return NULL;
    if (!graph->vertex_array) {
        verticies[array_size - 1] = NULL;
        return verticies;
    }

    /* Loops */
    for (index = 0; index < graph->vertex_count; index++) {
        /* Variables */
        char *name = NULL;

        /* Checks */
        if ((name = malloc(strlen(graph->vertex_array[index]->name) + 1)))
            strcpy(name, graph->vertex_array[index]->name);
        else
            return NULL;

        /* Append */
        verticies[index] = name;
    }

    /* Sort Array */
    qsort(verticies, array_size - 1, sizeof(char *), compare_chars);
    verticies[array_size - 1] = NULL; /* Null terminator */

    /* Return */
    return verticies;
}

/* This function connects the two parameter vertices with an edge of
 * certain weight. If the weight is negative the function returns 0. If the
 * vertices are not in the graph then they will be added first. If the edge
 * already exists then the weight will be updated and 1 will be returned.
 */
short create_graph_edge(String_graph *const graph, const char source[],
                        const char dest[], int weight) {
    /* Variables */
    struct edge *edge = find_existing_edge(graph, source, dest);
    struct edge *current_tail = find_edge_tail(graph, source);
    struct vertex *source_vertex;
    struct vertex *dest_vertex;
    char *dest_ptr;

    /* Check Input */
    if (!graph || !source || !dest || weight < 0)
        return 0;
    if (edge) {
        edge->weight = weight;
        return 1;
    }

    /* Check Verticies */
    if (!isa_vertex(graph, source) && !add_vertex_to_graph(graph, source))
        return 0;
    if (!isa_vertex(graph, dest) && !add_vertex_to_graph(graph, dest))
        return 0;
    if (!(source_vertex = find_existing_vertex(graph, source)))
        return 0;
    if ((dest_vertex = find_existing_vertex(graph, dest)))
        dest_ptr = dest_vertex->name;

    /* Check Memory Allocation */
    if (!(edge = malloc(sizeof(struct edge) + sizeof(int) +
                        sizeof(char *) + sizeof(struct edge *))))
        return 0;

    /* Find insert spot */
    if (!current_tail)
        source_vertex->edge_head = edge;
    else
        current_tail->next = edge;

    /* Return */
    edge->weight = weight;
    edge->dest = dest_ptr;
    edge->next = NULL;
    source_vertex->edge_count++;
    return 1;
}

/* This function returns the weight of the edge that runs from the two
 * vertices in the parameters. This returns -1 if graph doesn't have the
 * vertices, or the vertices are not connected, or if any parameter is
 * NULL. */
short get_graph_edge_weight(const String_graph *const graph,
                            const char source[], const char dest[]) {
    /* Variables */
    struct edge *edge = find_existing_edge(graph, source, dest);

    /* Checks */
    if (!graph || !edge)
        return -1;
    if (!source || !dest)
        return -1;

    /* Return */
    return edge->weight;
}

/* This function returns the number of neighbors that the vertex has. This
 * returns -1 if there is no vertex in the graph, or if the parameters are
 * NULL. */
short neighbor_count(const String_graph *const graph,
                     const char vertex[]) {
    /* Variables */
    struct vertex *v = find_existing_vertex(graph, vertex);

    /* Checks */
    if (!graph || !v || !vertex)
        return -1;
    if (!v->edge_count || !v->edge_head)
        return 0;

    /* Return */
    return v->edge_count;
}

/* This function compares characters. */
static int compare_chars(const void *a, const void *b) {
    return strcmp(*(char **)a, *(char **)b);
}

/* This function checks if the edge exists. */
static Edge *find_existing_edge(const String_graph *const graph,
                                const char source[], const char dest[]) {
    /* Variables */
    struct vertex *vertex = find_existing_vertex(graph, source);
    struct edge *current = NULL;

    /* Checks */
    if (!graph || !vertex || !vertex->edge_head)
        return NULL;
    else
        current = vertex->edge_head;

    /* Loops */
    while (current != NULL) {
        if (strcmp(current->dest, dest) == 0)
            return current;
        else
            current = current->next;
    }

    /* Default */
    return NULL;
}

/* This function finds the tail node of the edge. */
static Edge *find_edge_tail(const String_graph *const graph,
                            char const source[]) {
    /* Variables */
    struct vertex *vertex = find_existing_vertex(graph, source);
    struct edge *current = NULL;

    /* Checks */
    if (!graph || !vertex || !vertex->edge_head)
        return NULL;
    else
        current = vertex->edge_head;

    /* Loops */
    while (current && current->next)
        current = current->next;

    /* Return */
    return current;
}

/* This function checks if the vertex exists */
static Vertex *find_existing_vertex(const String_graph *const graph,
                                    const char name[]) {
    /* Variables */
    int index;

    /* Checks */
    if (!isa_vertex(graph, name))
        return NULL;
    if (!graph || !graph->vertex_array || !graph->vertex_count)
        return NULL;

    /* Loops */
    for (index = 0; index < graph->vertex_count; index++) {
        if (strcmp(graph->vertex_array[index]->name, name) == 0)
            return graph->vertex_array[index];
    }

    /* Default */
    return NULL;
}

/* This function retruns a new dynamically allocated array of pointers to
 * charcaters. These pointers point to copies of the names of the neigbors.
 * These pointers are sorted by alphabetcial order and the last element is
 * NULL. If the graph in the parameter is NULL or the graph does not have
 * that paramter vertex then the function returns NULL. */
char **get_neighbor_names(const String_graph *const graph,
                          const char vertex[]) {
    /* Variables */
    char **verticies = NULL;
    int index = 0;
    int array_size = 0;
    struct vertex *v = find_existing_vertex(graph, vertex);
    struct edge *e = NULL;

    /* Checks */
    if (!graph || !vertex || !v)
        return NULL;
    else
        array_size = v->edge_count + 1;
    if (!(verticies = malloc((array_size) * sizeof(char *))))
        return NULL;
    else
        e = v->edge_head;

    while (e != NULL) {
        char *name = NULL;
        /* Checks */
        if ((name = malloc(strlen(e->dest) + 1))) {
            strcpy(name, e->dest);
        } else {
            return NULL;
        }
        verticies[index++] = name;
        e = e->next;
    }

    /* Sort Array */
    qsort(verticies, (array_size - 1), sizeof(char *), compare_chars);
    verticies[array_size - 1] = NULL; /* Null terminator */

    /* Return */
    return verticies;
}

/* This function removes the edge going between the 2 vertices. Any
 * dynamically allocated memeory is also freed and returns 1. If eitehr of
 * the vetrices do not exist or the edge does not exist then function does
 * nothing and returns 0. */
int remove_graph_edge(String_graph *const graph, const char source[],
                      const char dest[]) {

    struct vertex *vertex = find_existing_vertex(graph, source);
    struct edge *edge = find_existing_edge(graph, source, dest);
    struct edge *prev = NULL;
    struct edge *curr = NULL;

    /* Checks */
    if (!graph || !source || !dest)
        return 0;
    if (!isa_vertex(graph, source))
        return 0;
    if (!isa_vertex(graph, dest))
        return 0;
    if (!edge || !vertex)
        return 0;
    else
        curr = vertex->edge_head;

    /* Loops */
    while (curr) {
        if (curr == edge) {
            struct edge *temp = curr;
            /* Checks */
            if (edge == vertex->edge_head)
                vertex->edge_head = temp->next;
            else
                prev->next = temp->next;

            /* Free Memory */
            free(temp);

            vertex->edge_count--;
            break;
        }

        prev = curr;
        curr = curr->next;
    }

    return 1;
}

/* This function removes the vertex from the graph as well as all of the
 * edges it has and returns 1. Any dynamically allocated memeory is also
 * freed. If the vertex does not exist the the funtion does nothing and
 * returns 0. */
int remove_vertex_from_graph(String_graph *const graph,
                             const char vertex[]) {
    /* Variables */
    struct vertex *v = NULL;
    struct edge *e = NULL;
    struct vertex *current = NULL;
    int index = 0;

    /* Checks */
    if (!graph || !vertex || !graph->vertex_array)
        return 0;
    else
        v = find_existing_vertex(graph, vertex);
    if (!v)
        return 0;
    else
        e = v->edge_head;

    /* Remove edges */
    while (e) {
        remove_graph_edge(graph, vertex, e->dest);
        e = e->next;
    }

    /* Find vertex element*/
    current = graph->vertex_array[0];
    while (current != v)
        current = graph->vertex_array[++index];

    /* Reset vertex*/
    while (current && index < (graph->vertex_count - 1) &&
           graph->vertex_array[index + 1]) {
        graph->vertex_array[index] = graph->vertex_array[index + 1];
        current = graph->vertex_array[++index];
    }

    free(v->name);
    free(v);
    graph->vertex_count--;
    if (!(graph->vertex_array =
              realloc(graph->vertex_array,
                      graph->vertex_count * sizeof(struct vertex *))))
        return 0;

    return 1;
}

/* This function deallocates alll dynamically allocated memory that is
 * being used deleting the graph and all of its data. */
void graph_delete(String_graph *const graph) {
    /* Variables */
    struct vertex *current_vertex = NULL;
    struct edge *current_edge = NULL;
    int index;

    /* Checks */
    if (!graph) {
        return;
    }

    for (index = 0; index < graph->vertex_count; index++) {
        current_vertex = graph->vertex_array[index];
        current_edge = current_vertex->edge_head;
        while (current_edge) {
            struct edge *temp = current_edge;
            current_edge = temp->next;
            free(temp);
        }

        free(current_vertex->name);
        free(current_vertex);
        free(current_edge);
    }

    /* Free Memory */
    free(graph->vertex_array);
}

/* This function frees all the dynamically allocated memory for the strings
 * in the array and then free the array itself. */
void free_vertex_name_list(char **const names) {
    int index = 0;

    if (!names)
        return;

    while (names[index]) {
        free(names[index++]);
    }

    free(names);
}