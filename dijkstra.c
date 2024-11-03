#include <stdio.h>
#define INFINITY 9999
#define MAX 100 

void Dijkstra(int Graph[MAX][MAX], int n, int start, int destination);

void Dijkstra(int Graph[MAX][MAX], int n, int start, int destination) {
    int cost[MAX][MAX], distance[MAX], pred[MAX];
    int visited[MAX], count, mindistance, nextnode, i, j;

    // Khoi tao ma tran chi phí 
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (Graph[i][j] == 0)
                cost[i][j] = INFINITY;
            else
                cost[i][j] = Graph[i][j];

    // Khoi tao cac mang distance, pred và visited  
    // distance: luu khoang cach ngan nhat tu node bat dau den node [i] 
    for (i = 0; i < n; i++) {
        distance[i] = cost[start][i];
        pred[i] = start;
        visited[i] = 0;
    }

    distance[start] = 0;
    visited[start] = 1;
    count = 1;

    while (count < n - 1) {
        mindistance = INFINITY;

        // TTim node tiep theo co khoang cach nho nhat 
        for (i = 0; i < n; i++)
            if (distance[i] < mindistance && !visited[i]) {
                mindistance = distance[i];
                nextnode = i;
            }

        visited[nextnode] = 1;

        // Cap nhat khoang cach 
        for (i = 0; i < n; i++)
            if (!visited[i])
                if (mindistance + cost[nextnode][i] < distance[i]) {
                    distance[i] = mindistance + cost[nextnode][i];
                    pred[i] = nextnode;
                }
        count++;
    }

    // In ra khoang cach va duong di ngan nhat toi node dich 
    printf("\nDistance from Node %d to Node %d: %d\n", start + 1, destination + 1, distance[destination]);
    printf("Path: ");

    // In các nodes trên duong di 
    int path[MAX];
    int path_index = 0;
    j = destination;
    while (j != start) {
        path[path_index++] = j;
        j = pred[j];
    }
    path[path_index] = start;

    // duong di tu node bat dau den node ket thuc ngan nhat  
    for (i = path_index; i >= 0; i--) {
        printf("Node %d", path[i] + 1);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

int main() {
    int Graph[MAX][MAX], i, j, n, start, destination;

    printf("Enter the number of vertices (1 to %d): ", MAX);
    scanf("%d", &n);

    if (n > MAX || n < 1) {
        printf("Invalid number of vertices. Must be between 1 and %d.\n", MAX);
        return 1;
    }

    // Nhap ma tran du lieu cac node 
    printf("Enter the adjacency matrix in the format (Vertices: 4 -> Node 1: 1 2 4 6):\n");
    for (i = 0; i < n; i++) {
        printf("Node %d: ", i + 1);
        for (j = 0; j < n; j++) {
            scanf("%d", &Graph[i][j]);
        }
    }

    // chon diem bat dau 
    printf("Enter the starting node index (1 to %d): ", n);
    scanf("%d", &start);
    //chon diem ket thuc  
    printf("Enter the destination node index (1 to %d): ", n);
    scanf("%d", &destination);

    // Chuyen các chi so ve mang bat dau tu 0 
    start--;
    destination--;

    Dijkstra(Graph, n, start, destination);

    return 0;
}

