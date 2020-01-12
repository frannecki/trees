#ifndef AVLTREE_H
#define AVLTREE_H

#include <cmath>
#include <stdio.h>
#include <algorithm>

struct avlnode{
    int val;
    avlnode *lc, *rc;  // left child and right child
    int bl;  // inbalance
    avlnode(int v=0, avlnode* lp=NULL, avlnode* rp=NULL): val(v), bl(0), lc(lp), rc(rp){};
};

class avltree{
public:
    avltree();
    bool insertKey(int);
    bool removeKey(int);
    void traversal();
    void checkForInbalance();

private:
    avlnode* root;
    void trav(avlnode*);
};

int backwardbl(avlnode*, avlnode*&);

avlnode*& searchForKey(avlnode*&, int);

#endif