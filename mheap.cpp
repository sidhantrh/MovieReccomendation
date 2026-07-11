#include "mheap.h"
#include <iostream>
#include <utility>
#include <stdexcept>
using namespace std;

MaxHeap::MaxHeap() {
}

int MaxHeap::parent(int index) const {
    return (index - 1) / 2;
}

int MaxHeap::leftChild(int index) const {
    return 2 * index + 1;
}

int MaxHeap::rightChild(int index) const {
    return 2 * index + 2;
}

bool MaxHeap::higherPriority(const Node& a, const Node& b) const {
    if (a.movie_score != b.movie_score) {
        return a.movie_score > b.movie_score;
    }

    return a.movie_name < b.movie_name;
}

void MaxHeap::heapifyUp(int index) {
    while (index > 0) {
        int parentIndex = parent(index);

        if (higherPriority(heap[index], heap[parentIndex])) {
            std::swap(heap[index], heap[parentIndex]);
            index = parentIndex;
        } else {
            break;
        }
    }
}

void MaxHeap::heapifyDown(int index) {
    int heapSize = static_cast<int>(heap.size());

    while (true) {
        int left = leftChild(index);
        int right = rightChild(index);
        int largest = index;

        if (left < heapSize && higherPriority(heap[left], heap[largest])) {
            largest = left;
        }

        if (right < heapSize && higherPriority(heap[right], heap[largest])) {
            largest = right;
        }

        if (largest == index) {
            break;
        }

        std::swap(heap[index], heap[largest]);
        index = largest;
    }
}

MaxHeap::Node MaxHeap::extractMax() {
    if (heap.empty()) {
        throw runtime_error("Cannot extract from an empty heap.");
    }

    Node maxMovie = heap[0];

    heap[0] = heap[heap.size() - 1];
    heap.pop_back();

    if (!heap.empty()) {
        heapifyDown(0);
    }

    return maxMovie;
}

void MaxHeap::insert(string movie_id, string movie_name, int movie_score) {
    Node node;
    node.movie_id = movie_id;
    node.movie_name = movie_name;
    node.movie_score = movie_score;

    heap.push_back(node);
    heapifyUp(static_cast<int>(heap.size()) - 1);
}

void MaxHeap::top_n(int n) {
    if (n <= 0) {
        return;
    }

    MaxHeap copy = *this;
    int count = 0;

    while (!copy.empty() && count < n) {
        Node topMovie = copy.extractMax();

        count++;

        cout << count << ". " << topMovie.movie_name
             << " (Score: " << topMovie.movie_score << ")" << endl;
    }
}

bool MaxHeap::empty() const {
    return heap.empty();
}

int MaxHeap::size() const {
    return static_cast<int>(heap.size());
}
