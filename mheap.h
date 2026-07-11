#pragma once
#include <string>
#include <vector>
using namespace std;

class MaxHeap {
    struct Node {
        string movie_id;
        string movie_name;
        int movie_score;
    };

    vector<Node> heap;

    int parent(int index) const;
    int leftChild(int index) const;
    int rightChild(int index) const;

    bool higherPriority(const Node& a, const Node& b) const;
    void heapifyUp(int index);
    void heapifyDown(int index);
    Node extractMax();

public:
    MaxHeap();

    void insert(string movie_id, string movie_name, int movie_score);
    void top_n(int n);

    bool empty() const;
    int size() const;
};
