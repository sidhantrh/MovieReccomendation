#include "RBT.h"
#include <iostream>
using namespace std;

RBT::RBT(){
    root = nullptr;
}

void RBT::destroy_helper(Node* node){
    if(node != nullptr){
        destroy_helper(node->left);
        destroy_helper(node->right);
        delete node;
    }
}

RBT::~RBT(){
    destroy_helper(root);
}

// nullptr counts as black
bool RBT::is_red(Node* node){
    if(node == nullptr){
        return false;
    }
    return node->color == RED;
}

RBT::Node* RBT::balance(Node* h){
    // Left-Left
    if(is_red(h->left) && is_red(h->left->left)){
        Node* child = h->left;
        Node* grandchild = h->left->left;
        grandchild->color = BLACK;
        h->left = child->right; // h takes child's old right subtree
        h->color = BLACK;
        child->left = grandchild;
        child->right = h;
        child->color = RED;
        return child;
    }
    // Right-Right
    if(is_red(h->right) && is_red(h->right->right)){
        Node* child = h->right;
        Node* grandchild = h->right->right;
        h->right = child->left; 
        h->color = BLACK;
        grandchild->color = BLACK;
        child->left = h;
        child->right = grandchild;
        child->color = RED;
        return child;
    }
    // Left-Right
    if(is_red(h->left) && is_red(h->left->right)){
        Node* child = h->left;
        Node* grandchild = h->left->right;
        child->right = grandchild->left;
        child->color = BLACK;
        h->left = grandchild->right;
        h->color = BLACK;
        grandchild->left = child;
        grandchild->right = h;
        grandchild->color = RED;
        return grandchild;
    }
    // Right-Left
    if(is_red(h->right) && is_red(h->right->left)){
        Node* child = h->right;
        Node* grandchild = h->right->left;
        h->right = grandchild->left;
        h->color = BLACK;
        child->left = grandchild->right;
        child->color = BLACK;
        grandchild->left = h;
        grandchild->right = child;
        grandchild->color = RED;
        return grandchild;
    }
    return h; 
}

RBT::Node* RBT::insert_helper(Node* h, int movie_id, string movie_name, int movie_score){
    if(h == nullptr){
        Node* node = new Node;
        node->movie_id = movie_id;
        node->movie_name = movie_name;
        node->movie_score = movie_score;
        node->left = nullptr;
        node->right = nullptr;
        node->color = RED; // new nodes are always inserted red
        return node;
    }

    if(movie_score > h->movie_score){
        h->left = insert_helper(h->left, movie_id, movie_name, movie_score);
    } else {
        h->right = insert_helper(h->right, movie_id, movie_name, movie_score);
    }

    return balance(h);
}

void RBT::insert(int movie_id, string movie_name, int movie_score){
    root = insert_helper(root, movie_id, movie_name, movie_score);
    root->color = BLACK; // root is always black
}

int RBT::top_n_helper(Node* node, int n, int count){
    if(node == nullptr || count >= n){
        return count;
    }
 
    count = top_n_helper(node->left, n, count);
    if(count >= n){
        return count; 
    }
 
    count++;
    cout << count << ". " << node->movie_name << " (Score: " << node->movie_score << ")" << endl;
 
    count = top_n_helper(node->right, n, count);
    return count;

}

void RBT::top_n(int n){
    if(n <= 0){
        return;
    }
    top_n_helper(root, n, 0);
}