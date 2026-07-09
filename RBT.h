#pragma once
#include <string>
using namespace std;

#define RED true
#define BLACK false

// Red-Black Tree, recursive insertion, no parent pointers.
// Uses Okasaki's balancing algorithm: after inserting, each node on the
// way back up the recursion checks its own two children and grandchildren
// for a red-red violation, in any of four possible shapes (left-left,
// left-right, right-left, right-right), and restructures whichever one
// actually occurred. No preference for which side red links lean on.
class RBT{
    struct Node{
        int movie_id;
        string movie_name;
        int movie_score;
        bool color;
        Node* left;
        Node* right;
    };

    Node* root;

    // internal helpers
    bool is_red(Node* node); // treats nullptr as BLACK
    Node* balance(Node* h);
    Node* insert_helper(Node* h, int movie_id, string movie_name, int movie_score);
    int top_n_helper(Node* node, int n, int count);
    void destroy_helper(Node* node);

    public:
    RBT();
    ~RBT();

    void insert(int movie_id, string movie_name, int movie_score);
    void top_n(int n);
};