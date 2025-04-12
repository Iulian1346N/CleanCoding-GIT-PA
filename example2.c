#include <stdio.h>
#include <stdlib.h>

// Structuri
typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct Graph {
    int numberOfVertices;
    int *visited;
    Node **adjacencyLists;
} Graph;

// Funcții utilitare
Node *createNode(int value) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

Graph *createGraph(int numberOfVertices) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numberOfVertices = numberOfVertices;
    graph->adjacencyLists = (Node **)malloc(numberOfVertices * sizeof(Node *));
    graph->visited = (int *)malloc(numberOfVertices * sizeof(int));

    for (int i = 0; i < numberOfVertices; i++) {
        graph->adjacencyLists[i] = NULL;
        graph->visited[i] = 0;
    }

    return graph;
}

void addEdge(Graph *graph, int source, int destination) {
    if (source < 0 || source >= graph->numberOfVertices || destination < 0 || destination >= graph->numberOfVertices) {
        printf("Eroare: Nod invalid (%d, %d). Nodurile trebuie sa fie intre 0 si %d.\n", source, destination, graph->numberOfVertices - 1);
        return;
    }

    Node *newNode = createNode(destination);
    newNode->next = graph->adjacencyLists[source];
    graph->adjacencyLists[source] = newNode;

    newNode = createNode(source);
    newNode->next = graph->adjacencyLists[destination];
    graph->adjacencyLists[destination] = newNode;
}

void insertEdges(int numberOfVertices, int numberOfEdges, Graph *graph) {
    int source, destination;

    printf("Adauga %d muchii (de la 0 la %d)\n", numberOfEdges, numberOfVertices - 1);
    for (int i = 0; i < numberOfEdges; i++) {
        scanf("%d %d", &source, &destination);
        addEdge(graph, source, destination);
    }
}

// Utilitare pentru BFS
int isEmpty(Node *queue) {
    return queue == NULL;
}

void enqueue(Node **queue, int data) {
    Node *newNode = createNode(data);

    if (isEmpty(*queue)) {
        *queue = newNode;
    } else {
        Node *temp = *queue;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

int dequeue(Node **queue) {
    int data = (*queue)->data;
    Node *temp = *queue;
    *queue = (*queue)->next;
    free(temp);
    return data;
}

void printGraph(Graph *graph) {
    for (int i = 0; i < graph->numberOfVertices; i++) {
        printf("Vertex %d: ", i);
        Node *temp = graph->adjacencyLists[i];
        while (temp) {
            printf("%d ", temp->data);
            temp = temp->next;
        }
        printf("\n");
    }
}

void printQueue(Node *queue) {
    while (queue != NULL) {
        printf("%d ", queue->data);
        queue = queue->next;
    }
}

void resetVisitedList(Graph *graph) {
    for (int i = 0; i < graph->numberOfVertices; i++) {
        graph->visited[i] = 0;
    }
}

// Parcurgeri
void dfs(Graph *graph, int vertex) {
    if (vertex < 0 || vertex >= graph->numberOfVertices) {
        printf("Eroare: Nod de start invalid pentru DFS (%d).\n", vertex);
        return;
    }

    Node *adjList = graph->adjacencyLists[vertex];
    graph->visited[vertex] = 1;
    printf("%d -> ", vertex);

    Node *temp = adjList;
    while (temp != NULL) {
        int connectedVertex = temp->data;
        if (graph->visited[connectedVertex] == 0) {
            dfs(graph, connectedVertex);
        }
        temp = temp->next;
    }
}

void bfs(Graph *graph, int startVertex) {
    if (startVertex < 0 || startVertex >= graph->numberOfVertices) {
        printf("Eroare: Nod de start invalid pentru BFS (%d).\n", startVertex);
        return;
    }

    Node *queue = NULL;
    graph->visited[startVertex] = 1;
    enqueue(&queue, startVertex);

    while (!isEmpty(queue)) {
        int currentVertex = dequeue(&queue);
        printf("%d ", currentVertex);

        Node *temp = graph->adjacencyLists[currentVertex];
        while (temp) {
            int adjacentVertex = temp->data;
            if (graph->visited[adjacentVertex] == 0) {
                graph->visited[adjacentVertex] = 1;
                enqueue(&queue, adjacentVertex);
            }
            temp = temp->next;
        }
    }
}

// Eliberare memorie
void freeGraph(Graph *graph) {
    for (int i = 0; i < graph->numberOfVertices; i++) {
        Node *temp = graph->adjacencyLists[i];
        while (temp) {
            Node *toFree = temp;
            temp = temp->next;
            free(toFree);
        }
    }
    free(graph->adjacencyLists);
    free(graph->visited);
    free(graph);
}

int main() {
    int numberOfVertices;
    int numberOfEdges;
    int startVertex;

    printf("Cate noduri are graful? ");
    scanf("%d", &numberOfVertices);

    printf("Cate muchii are graful? ");
    scanf("%d", &numberOfEdges);

    Graph *graph = createGraph(numberOfVertices);
    insertEdges(numberOfVertices, numberOfEdges, graph);

    printf("De unde plecam in DFS? ");
    scanf("%d", &startVertex);
    printf("Parcurgere cu DFS: ");
    dfs(graph, startVertex);

    resetVisitedList(graph);
    printf("\n");

    printf("De unde plecam in BFS? ");
    scanf("%d", &startVertex);
    printf("Parcurgere cu BFS: ");
    bfs(graph, startVertex);

    freeGraph(graph);
    return 0;
}