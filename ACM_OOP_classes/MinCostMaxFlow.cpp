#include <bits/stdc++.h>

using namespace std;

class MinCostMaxFlow
{
private:

    static const int INF = numeric_limits<int>::max();

    class Edge
    {
    public:

        int flow;
        int capacity;
        int cost;

        int node;
        int next;

        Edge(int f, int cap, int cst, int _node, int _next) : flow(f), capacity(cap),
            cost(cst), node(_node), next(_next) {
        }
    };

    class Node
    {
    public:

        int distance;
        int node;

        Node(int d, int _node) : distance(d), node(_node){
        }

        bool operator < (const Node &X) const
        {
            return distance > X.distance;
        }
    };

    vector<Edge> graph;
    vector<int> head, father, pointer;
    vector<int> oldDist, realDist, newDist;
    int N;

    void bellmanFord(int source)
    {
        fill(oldDist.begin(), oldDist.end(), INF);

        vector<bool> inQueue(N, false);
        queue<int> Q;

        Q.push(source);
        inQueue[source] = true;
        oldDist[source] = 0;

        while (!Q.empty())
        {
            int node = Q.front();
            Q.pop();
            inQueue[node] = false;

            for (int p = head[node]; p != NIL; p = graph[p].next)
            {
                Edge &e = graph[p];

                if (e.flow < e.capacity && oldDist[e.node] > oldDist[node] + e.cost)
                {
                    oldDist[e.node] = oldDist[node] + e.cost;

                    if (!inQueue[e.node])
                    {
                        inQueue[e.node] = true;
                        Q.push(e.node);
                    }
                }
            }
        }
    }

    bool dijkstra(int S, int T)
    {
        fill(newDist.begin(), newDist.end(), INF);
        fill(realDist.begin(), realDist.end(), INF);
        fill(father.begin(), father.end(), NIL);
        fill(pointer.begin(), pointer.end(), NIL);

        priority_queue<Node> minHeap;
        father[S] = S;
        realDist[S] = newDist[S] = 0;
        minHeap.push({0, S});

        while (!minHeap.empty())
        {
            Node _node = minHeap.top();
            minHeap.pop();

            int node = _node.node;

            if (newDist[node] != _node.distance)
                continue;

            for (int p = head[node]; p != NIL; p = graph[p].next)
            {
                Edge &e = graph[p];
                int son = e.node;

                int new_distance = oldDist[node] - oldDist[son] + newDist[node] + e.cost;

                if (e.flow < e.capacity && newDist[son] > new_distance)
                {
                    newDist[son] = new_distance;
                    realDist[son] = realDist[node] + e.cost;

                    pointer[son] = p;
                    father[son] = node;

                    minHeap.push({newDist[son], son});
                }
            }
        }

        copy(realDist.begin(), realDist.end(), oldDist.begin());

        return realDist[T] != INF;
    }

public:

    static const int NIL;

    MinCostMaxFlow(int _N) : N(_N)
    {
        head.resize(N, NIL);
        father.resize(N);
        pointer.resize(N);
        oldDist.resize(N);
        realDist.resize(N);
        newDist.resize(N);
    }

    void addEdge(int from, int to, int capacity, int cost)
    {
        from--; to--;

        graph.push_back(Edge(0, capacity, cost, to, head[from]));
        head[from] = graph.size() - 1;
    }

    void addDoubleEdge(int from, int to, int capacity, int cost)
    {
        addEdge(from, to, capacity, cost);
        addEdge(to, from, 0, -cost);
    }

    pair<int,int> minCostMaxFlow(int S, int T)
    {
        S--; T--;
        assert(0 <= S && 0 <= T && S < N && T < N);
        assert(S != T);

        int totalFlow = 0;
        int costTotalFlow = 0;

        bellmanFord(S);

        while (dijkstra(S, T))
        {
            int fmin = INF;
            int node = T;

            while (node != S)
            {
                fmin = min(fmin, graph[pointer[node]].capacity - graph[pointer[node]].flow);
                node = father[node];
            }

            node = T;

            while (node != S)
            {
                graph[pointer[node]].flow += fmin;
                graph[pointer[node] ^ 1].flow -= fmin;
                node = father[node];
            }

            totalFlow += fmin;
            costTotalFlow += fmin * realDist[T];
        }

        return make_pair(totalFlow, costTotalFlow);
    }
};

const int MinCostMaxFlow::NIL = -1;

int main()
{

    return 0;
}
