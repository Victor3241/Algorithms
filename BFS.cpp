#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <queue>
#include <list>
#include "bfs.h"

int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4
    if (grid->mat[p.row][p.col] == 0) {
        int d_row[4] = { 1, 0, -1, 0 };
        int d_col[4] = { 0, -1, 0, 1 };
        int neighbour_size = 0;

        for (int i = 0; i <= 3; i++) {
            int row = d_row[i] + p.row;
            int col = d_col[i] + p.col;
            if ((row > 0 && row < grid->rows) && (col > 0 && col < grid->cols)) {
                if (grid->mat[row][col] == 0) {
                    neighb[neighbour_size].row = row;
                    neighb[neighbour_size].col = col;
                    neighbour_size++;
                }
            }
        }
        return neighbour_size;
    }
    else return 0;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph* graph, Node* s, Operation* op)
{
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    std::queue<Node*> q;
    for (int i = 0; i < graph->nrNodes; i++) {
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->dist = -1;
        graph->v[i]->parent = NULL;
    }
    s->color = COLOR_GRAY;
    s->dist = 0;
    Node* current = new Node();
    current = s;
    q.push(s);
    while (q.size()) {
        if (op != NULL) op->count();
        for (int i = 0; i < current->adjSize; i++) {
            if (op != NULL) op->count();
            if (current->adj[i]->color == COLOR_WHITE) {
                q.push(current->adj[i]);
                current->adj[i]->parent = current;
                current->adj[i]->color = COLOR_GRAY;
                current->adj[i]->dist = current->adj[i]->parent->dist + 1;
            }
        }
        current->color = COLOR_BLACK;
        q.pop();
        if (q.size()) {
            current = q.front();
        }
    }
}

void pretty_print(int p[], Point repr[], int spaces, int size, int root) {
    for (int i = 0; i < size; i++)
        if (p[i] == root) {
            for (int j = 0; j < spaces; j++) std::cout << '\t';
            std::cout << '(' << repr[i].row << ", " << repr[i].col << ')' << '\n';
            pretty_print(p, repr, spaces + 1, size, i);
        }
}


void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if(!err){
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
        pretty_print(p, repr, 0, n, -1);
    }

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000

    std::queue<Node*> q;
    for (int i = 0; i < graph->nrNodes; i++) {
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->dist = -1;
        graph->v[i]->parent = NULL;
    }
    start->color = COLOR_GRAY;
    start->dist = 0;
    Node* current = new Node();
    current = start;
    q.push(start);
    while (q.size()) {
        for (int i = 0; i < current->adjSize; i++) {
            if (current->adj[i]->color == COLOR_WHITE) {
                q.push(current->adj[i]);
                current->adj[i]->parent = current;
                current->adj[i]->color = COLOR_GRAY;
                current->adj[i]->dist = current->adj[i]->parent->dist + 1;
            }
        }
        if (current == end) break;
        current->color = COLOR_BLACK;
        q.pop();
        if (q.size()) {
            current = q.front();
        }
    }

    if (current != end) return -1;

    int size = current->dist;
    if (size != 0) {
        path[current->dist] = end;
        path[0] = start;
        while (current != start) {
            for (int i = 0; i < 4; i++) {
                if (current->adj[i]->dist != -1)
                    if(current->adj[i]->dist < current->dist)
                {
                    current = current->adj[i];
                    path[current->dist] = current;
                    i = 4;
                }
            }
        }
        return size;
    }
    return -1;
}


void performance()
{
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for(n=1000; n<=4500; n+=100){
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }

        // TODO: generate n random edges
        // make sure the generated graph is connected
        for (int i = 0; i < graph.nrNodes; i++) {
            graph.v[i]->position.col = i;
            graph.v[i]->position.row = i;
            graph.v[i]->adjSize = n / graph.nrNodes;
        }
        for (int i = 0; i < graph.nrNodes; i++) {
            graph.v[i]->adj = (Node**)malloc(graph.v[i]->adjSize * sizeof(Node*));
            for (int j = 0; j < graph.v[i]->adjSize; j++) {
                graph.v[i]->adj[j] = (Node*)malloc(sizeof(Node));
                graph.v[i]->adj[j] = graph.v[(i + j + 1) % graph.nrNodes];
            }
        }

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    

    // vary the number of vertices
    for(n=100; n<=200; n+=10){
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected
        
        for (int i = 0; i < graph.nrNodes; i++) {
            graph.v[i]->position.col = i;
            graph.v[i]->position.row = i;
            graph.v[i]->adjSize = n / graph.nrNodes;
        }
       int r = 4500 % graph.nrNodes;
       int nr = rand() % graph.nrNodes;

        for(int i = 0; i < graph.nrNodes; i++){
            graph.v[i]->adj = (Node**)malloc(graph.nrNodes * sizeof(Node*));
            int a[200] = { 0 };
            graph.v[i]->adj[0] = graph.v[(i + 1) % (graph.nrNodes)];
            for (int j = 1; j < graph.v[i]->adjSize; j++) {
                FillRandomArray(a, graph.v[i]->adjSize, 0, graph.nrNodes - 1, true, UNSORTED);
                std::cout << a[j] << '\n';
                graph.v[i]->adj[j] = (Node*)malloc(sizeof(Node));
                graph.v[i]->adj[j] = graph.v[a[j]];
            }
        }

        /*for (int i = 0; i < graph.nrNodes; i++) {
            graph.v[i]->position.col = i;
            graph.v[i]->position.row = i;
            graph.v[i]->adjSize = 4500 / graph.nrNodes;
        }
        std::cout << graph.v[0]->adjSize << '\n';
        for (int i = 0; i < graph.nrNodes; i++) {
            graph.v[i]->adj = (Node**)malloc(graph.v[i]->adjSize * sizeof(Node*));
            for (int j = 0; j < graph.v[i]->adjSize; j++) {
                graph.v[i]->adj[j] = (Node*)malloc(sizeof(Node));
                graph.v[i]->adj[j] = graph.v[(i + j + 1) % graph.nrNodes];
            }
        }*/

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
