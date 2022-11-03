#include "branchAndBound.hpp"
#include <climits>
#include <queue>
#include <unordered_set>

#define VERBOSE false

int *BranchAndBound::minWeights;

GraphMatrix *BranchAndBound::graph;
int BranchAndBound::startingVertex;

int BranchAndBound::upperBound;

int BranchAndBound::firstImprovement;

class Node
{
public:
    std::vector<int> path;
    // Set of all vertices not present in the path
    std::unordered_set<int> availableVertices;
    int vertex;
    int lowerBound;

    Node(int vertex, std::vector<int> path, std::unordered_set<int> availableVertices, int lowerBound)
    {
        this->vertex = vertex;
        this->path = path;
        this->availableVertices = availableVertices;
        this->lowerBound = lowerBound;
    }
};

// Comparison object to order the heap
struct comp
{
    bool operator()(const Node *lhs, const Node *rhs) const
    {
        return lhs->lowerBound > rhs->lowerBound;
    }
};

Path BranchAndBound::execute(GraphMatrix *graph, int startingVertex)
{
    std::priority_queue<Node *, std::vector<Node *>, comp> pq;
    std::vector<int> bestPath;

    const int graphSize = graph->getVertexCount();
    minWeights = new int[graphSize];

    // The lower bound of the starting vertex
    int lowerBound = 0;
    for (int i = 0; i < graphSize; ++i)
    {
        int minWeight = INT_MAX;
        for (int j = 0; j < graphSize; ++j)
        {
            if (i != j)
            {
                int weight = graph->getWeight(i, j);
                if (weight < minWeight)
                {
                    minWeight = weight;
                }
            }
        }
        minWeights[i] = minWeight;
        lowerBound += minWeight;
    }

    // root node with empty path and all vertices
    std::unordered_set<int> availableVertices;
    for (int i = 0; i < graphSize; ++i)
    {
        if (i != startingVertex)
        {
            availableVertices.insert(i);
        }
    }
    std::vector<int> path = {startingVertex};
    Node *root = new Node(startingVertex, path, availableVertices, lowerBound);

    int upperBound = INT_MAX;

    // get the lower bound of the path starting at node 0

    // Add root to the list of live nodes
    pq.push(root);

    // Finds a live node with the least cost, adds its children to the list of
    // live nodes, and finally deletes it from the list
    while (!pq.empty())
    {
        // Find a live node with the least estimated cost
        Node *node = pq.top();
        // The found node is deleted from the list of live nodes
        pq.pop();

        // `i` stores the current city number
        int i = node->vertex;

        // Reached the leaf node (last vertex of the path)
        if (node->path.size() == graphSize)
        {
            // return to starting city
            node->path.push_back(0);

            // Lower bound from the last vertex back to the startingVertex
            node->lowerBound = node->lowerBound - minWeights[node->vertex] + graph->getWeight(node->vertex, startingVertex);
            if (node->lowerBound < upperBound)
            {
                // update the upper bound
                upperBound = node->lowerBound;
                bestPath = node->path;
            }
        }

        // printf("%i\n", node->path.size());

        for (int childVertex : node->availableVertices)
        {
            lowerBound = node->lowerBound - minWeights[node->vertex] + graph->getWeight(node->vertex, childVertex);

            if (lowerBound < upperBound)
            {
                // create a child node and calculate its cost
                std::vector<int> path(node->path);
                path.push_back(childVertex);
                std::unordered_set<int> availableVertices(node->availableVertices);
                availableVertices.erase(childVertex);
                Node *child = new Node(childVertex, path, availableVertices, lowerBound);

                pq.push(child);
            }
            else
            {
                break;
            }
        }
        delete node;
    }
    if (VERBOSE)
    {

        printPath(bestPath);
        printf("weight: %i\n", upperBound);
    }

    delete[] minWeights;
    return Path(bestPath, upperBound);

    // free node as we have already stored edges `(i, j)` in vector.
    // So no need for a parent node while printing the solution.
    // delete node;
}

void BranchAndBound::printMinWeights()
{
    printf("Min weights: ");
    for (int i = 0; i < graph->getVertexCount(); ++i)
    {
        printf("%i, ", minWeights[i]);
    }
    printf("\n");
}

void BranchAndBound::printPath(std::vector<int> path)
{
    for (int vertex : path)
    {
        printf("%i -> ", vertex);
    }
    printf("\n");
    // printf("%i\n", startingVertex);
}

void BranchAndBound::printImprovement(int weight)
{
    const int optimum = graph->getOptimum();
    const float prd = (100.0 * (weight - optimum)) / optimum;
    printf("%4i %.2f%%\n", weight, prd);
}
