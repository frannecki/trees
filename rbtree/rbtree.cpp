#include "rbtree.h"

/**
 * Constructor
 */
rbtree::rbtree(){
    root = NULL;
}


/**
 * Insert a key
 * 
 * @param key    the key to be inserted
 */
bool rbtree::insertKey(int key){
    rbnode*& node = searchForKey(root, key);
    if(node)  return false;
    else  node = new rbnode(key, true);
    /// check for double red (both the inserted and its parent are labeled red)
    checkForDoubleRed(key);
    root->red = false;  // ensure root is black
    return true;
}

/**
 * Check if two successive nodes are both red after an insertion, and adjust
 * 
 * @param root   the root of the tree
 * @param key    the key just inserted
 */
void rbtree::checkForDoubleRed(int key){
    rbnode *g, *p, *c;
    g = p = c = root;
    while(key != c->val){
        g = p;
        p = c;
        c = (key<c->val)?(c->lc):(c->rc);
    }
    if(c == root){
        c->red = false;
        return;
    }
    else if(p->red == false)  return;
    
    rbnode* uncle = (p==g->lc)?(g->rc):(g->lc);
    if(uncle != NULL && uncle->red){
        /// uncle node is red
        g->red = true;
        uncle->red = false;
        p->red = false;
        // double red may transfer to ancestor, check again
        checkForDoubleRed(g->val);
    }
    else{
        /// uncle node is black
        rbnode *x1, *x2, *x3, *x4;
        if(p == g->lc){
            if(c == p->lc){
                x1 = c->lc;  x2 = c->rc;  x3 = p->rc;
            }
            else{
                x1 = p->lc;  x2 = c->lc;  x3 = c->rc;
            }
            x4 = g->rc;
        }
        else{
            if(c == p->lc){
                x2 = c->lc;  x3 = c->rc;  x4 = p->rc;
            }
            else{
                x2 = p->lc;  x3 = c->lc;  x4 = c->rc;
            }
            x1 = g->lc;
        }
        int keys[3] = {g->val, p->val, c->val};
        std::sort(keys, keys+3);
        g->red = false;
        g->val = keys[1];
        g->lc = new rbnode(keys[0], true, x1, x2);
        g->rc = new rbnode(keys[2], true, x3, x4);
    }
}


/**
 * Remove a key
 * 
 * @param key    the key to be removed
 */
bool rbtree::removeKey(int key){
    rbnode*& node = searchForKey(root, key);
    if(!node)  return false;  // key not found

    ////////////////////////////////////////////SOLUTION///////////////////////////////////////////////
    /// remove the searched node or its in-order successor, and replace the removed node with its child;
    /// (1) if the removed is red, nothing to be done
    /// (2) if the removed is black
    /// (2.1) if its child is red, turn it to black
    /// (2.2) if its child is also black, take care of the 'double black' case
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool double_black = (!node->red && (!node->rc || !node->rc->red));  // both the node and its right child are black
    rbnode* rt = root;

    if(!(node->lc || node->rc)){
        node = NULL;
    }
    else if(!node->lc){
        if(node->rc && node->rc->red)  node->rc->red = node->red;  // (2.1)
        node = node->rc;
    }
    else if(!node->rc){
        if(node->lc && node->lc->red)  node->lc->red = node->red;  // (2.1)
        node = node->lc;
    }
    else{
        rbnode *tmp = node->rc, *tmpp = node;
        while(tmp->lc){
            tmpp = tmp;
            tmp = tmp->lc;
        }
        node->val = tmp->val;
        double_black = (!tmp->red && (!tmp->rc || !tmp->rc->red));  // succeessor removed
        if(tmp->rc && tmp->rc->red)  tmp->rc->red = tmp->red;  // (2.1)
        if(tmp == tmpp->lc)  tmpp->lc = tmp->rc;
        else  tmpp->rc = tmp->rc;
        rt = tmpp;
    }
    /// check for double black
    if(double_black){
        rbnode *p, *n, *tmp;
        bool check = true;
        p = n = rt;
        while(n){
            tmp = (key < n->val)?(n->lc):(n->rc);
            p = n;  n = tmp;
        }
        checkForDoubleBlack(p, n); // (2.2)
    }
    if(root)  root->red = false;  // ensure root is black
    return true;
}

/**
 * Check if both the removed and its right child are black, and adjust
 * 
 * @param p    the parenr of the removed key
 * @param n    the right child of the removed key (now in the place of its former parent)
 */
void rbtree::checkForDoubleBlack(rbnode* p, rbnode* n){

    ////////////////////////////////////////////////SOLUTION///////////////////////////////////////////////////
    /// since the removed and its child are both black, and the other child is NIL, 
    /// the selected must also be a NIL to satisfy the requirements of red black tree.
    /// the sibling of the removed node (named s here, exists only if the removed was not root) 
    /// must not be NIL due to black height balance
    /// (2.2.1) the removed node was root (tree empty now), nothing to be done
    /// (2.2.2) if s is red, its children and p must be black internal nodes (not NIL)
    /// (2.2.3) if s, p and the children of s are all black, its children must be NIL
    /// (2.2.4) if p is red and the children of s are black, s must also be black, and its children must be NIL
    /// (2.2.5) if s is black but its left child is red
    /// (2.2.6) if s is black but its right child is red
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if(n == root)  return;  // (2.2.1)
    rbnode* s = (n == p->lc)?(p->rc):(p->lc);
    if(s->red){
        // (2.2.2) rotate, and check again, with n's parent as the new p
        rbnode *np, *nn;
        if(s == p->lc){
            p->lc = s->lc;
            np = p->rc = new rbnode(p->val, true, s->rc, n);
            nn = np->rc;
        }
        else{
            np = p->lc = new rbnode(p->val, true, n, s->lc);
            p->rc = s->rc;
            nn = np->lc;
        }
        p->val = s->val;
        checkForDoubleBlack(np, nn);
    }
    else if(!p->red && !s->red && !(s->lc && s->lc->red) && !(s->rc && s->rc->red)){
        // (2.2.3) turn s to red, and check again, with p as the new n, and its parent as the new p
        s->red = true;
        p = n = root;
        while(n != p){
            p = n;
            n = (p->val < n->val)?(n->lc):(n->rc);  
        }
        checkForDoubleBlack(p, n);
    }
    else if(p->red && !(s->lc && s->lc->red) && !(s->rc && s->rc->red)){
        // (2.2.4) simply switch the colors of s and p
        s->red = true;
        p->red = false;
    }
    else if(!s->red && (s->lc && s->lc->red)){
        // (2.2.5) rotate
        s->lc->red = false;
        rbnode *x1, *x2, *x3, *x4;
        if(s == p->lc){
            x1 = s->lc->lc;  x2 = s->lc->rc;  x3 = s->rc;  x4 = n;
        }
        else{
            x1 = n;  x2 = s->lc->lc;  x3 = s->lc->rc;  x4 = s->rc;
        }
        int keys[3] = {s->lc->val, s->val, p->val};
        std::sort(keys, keys+3);
        p->val = keys[1];
        p->lc = new rbnode(keys[0], x1, x2);
        p->rc = new rbnode(keys[2], x3, x4);
    }
    else if(!s->red && (s->rc && s->rc->red)){
        // (2.2.6)
        s->rc->red = false;
        rbnode *x1, *x2, *x3, *x4;
        if(s == p->rc){
            x1 = n;  x2 = s->lc;  x3 = s->rc->lc;  x4 = s->rc->rc;
        }
        else{
            x1 = s->lc;  x2 = s->rc->lc;  x3 = s->rc->rc;  x4 = n;
        }
        int keys[3] = {s->rc->val, s->val, p->val};
        std::sort(keys, keys+3);
        p->val = keys[1];
        p->lc = new rbnode(keys[0], x1, x2);
        p->rc = new rbnode(keys[2], x3, x4);
    }
}


/**
 * Traverse the tree
 */
void rbtree::traversal(){
    trav(root);
    printf("\n");
}

void rbtree::trav(rbnode* rt){
    if(!rt)  return;
    printf((rt->red)?"R":"B");
    printf("%d ", rt->val);
    trav(rt->lc);
    trav(rt->rc);
}


/**
 * Search for a certain key in the tree
 * 
 * @param rt      the current node
 * @param key     the key to search
 * @return        the searched node
 */
rbnode*& searchForKey(rbnode* &rt, int key){
    if(!rt || rt->val == key)  return rt;
    else if(key < rt->val)  return searchForKey(rt->lc, key);
    else  return searchForKey(rt->rc, key);
}