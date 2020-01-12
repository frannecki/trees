#ifndef RBTREE_H
#define RBTREE_H

#include <stdio.h>
#include <algorithm>

struct rbnode{
    int val;
    rbnode *lc, *rc;
    bool red;
    rbnode(int v = 0, bool r = true, rbnode* lp = NULL, rbnode* rp = NULL): val(v), red(r), lc(lp), rc(rp){};
};

class rbtree{
public:
    rbtree();
    bool insertKey(int);
    bool removeKey(int);
    void traversal();

private:
    rbnode* root;
    void trav(rbnode*);
    void checkForDoubleRed(int);
    void checkForDoubleBlack(rbnode*, rbnode*);
};

rbnode*& searchForKey(rbnode*&, int);

#endif