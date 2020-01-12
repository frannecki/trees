#include "avltree.h"

avltree::avltree(){
    root = NULL;
}

bool avltree::insertKey(int key){
    avlnode* &node = searchForKey(root, key);
    if(node)  return false;
    node = new avlnode(key);

    /// check for inbalance
    checkForInbalance();
    return true;
}


bool avltree::removeKey(int key){
    avlnode* &node = searchForKey(root, key);
    if(!node)  return false;
    else{
        if(!(node->lc || node->rc)){
            node = NULL;
        }
        else if(!node->lc)  node = node->rc;
        else if(!node->rc)  node = node->lc;
        else{
            avlnode *tmp = node->rc, *tmpp = node;
            while(tmp->lc){
                tmpp = tmp;
                tmp = tmp->lc;
            }
            node->val = tmp->val;
            // succeessive node to be removed
            if(tmp == tmpp->lc)  tmpp->lc = tmp->rc;
            else  tmpp->rc = tmp->rc;
        }
    } 
    /// check for inbalance
    checkForInbalance();
    return true;
}


void avltree::traversal(){
    trav(root);
    printf("\n");
}

void avltree::trav(avlnode* rt){
    if(!rt)  return;
    printf("%d ", rt->val);
    trav(rt->lc);
    trav(rt->rc);
}


/**
 * Check for inbalance and restore
 */
void avltree::checkForInbalance(){
    avlnode* g = NULL;
    backwardbl(root, g);
    if(!g)  return;
    avlnode *p, *c;
    p = (g->bl > 0)?(g->lc):(g->rc);
    c = (p->bl > 0)?(p->lc):(p->rc);
    
    /// regroup 7 nodes
    avlnode *x1, *x2, *x3, *x4;
    if(p == g->lc){
        x4 = g->rc;
        if(c == p->lc){
            x1 = c->lc;  x2 = c->rc;  x3 = p->rc;
        }
        else{
            x1 = p->lc;  x2 = c->lc;  x3 = c->rc;
        }
    }
    else{
        x1 = g->lc;
        if(c == p->lc){
            x2 = c->lc;  x3 = c->rc;  x4 = p->rc;
        }
        else{
            x2 = p->lc;  x3 = c->lc;  x4 = c->rc;
        }
    }
    int keys[3] = {g->val, p->val, c->val};
    std::sort(keys, keys+3);
    g->val = keys[1];
    g->lc = new avlnode(keys[0], x1, x2);
    g->rc = new avlnode(keys[2], x3, x4);
}


/**
 * Update attribute 'bl' of nodes in the tree
 * 
 * @param rt      the current node
 * @param node    the lowest node with `abs(bl) > 1`
 */
int backwardbl(avlnode* rt, avlnode* &node){
    if(!rt)  return 0;
    else{
        int leftHeight = backwardbl(rt->lc, node);
        int rightHeight = backwardbl(rt->rc, node);
        rt->bl = leftHeight - rightHeight;
        if(node == NULL && abs(rt->bl) > 1)  node = rt;
        return 1 + std::max(leftHeight, rightHeight);
    }
}


/**
 * Search for a certain key in the tree
 * 
 * @param rt      the current node
 * @param key     the key to search
 * @return        the searched node
 */
avlnode*& searchForKey(avlnode* &rt, int key){
    if(!rt || rt->val == key)  return rt;
    else if(key < rt->val)  return searchForKey(rt->lc, key);
    else  return searchForKey(rt->rc, key);
}