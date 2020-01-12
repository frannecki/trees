#ifndef BTREE_H
#define BTREE_H
#include <queue>
#include <vector>
#include <stdio.h>

struct bnode{
    std::vector<int> keys;
    bnode* prt;  // parent
    std::vector<bnode*> ptrs;  // children
    bnode(bnode* child=NULL): prt(NULL){
        ptrs.push_back(child);
    };
};

class btree{
public:
    btree();
    btree(int);
    bool insertKey(int);
    bool removeKey(int);
    void traversal();
    int getHeight();

private:
    bnode* root;
    int maxb, height;
    void trav(bnode*);
};

int insertion(std::vector<int>&);

template<typename T>
void insertionWithIdx(std::vector<T>&, int);

template<typename T>
void deletionWithIdx(std::vector<T>&, int);

void rotateAndMerge(bnode*, int);

void mergeNode(bnode*, bnode*);

#endif