#include "btree.h"

btree::btree(){
    root = new bnode();
    height = 1;
}

btree::btree(int m){
    maxb = m;
    height = 1;
    root = new bnode();
}

int btree::getHeight(){
    return height;
}

/**
 * Insert a given key
 * 
 * @param key    the key to be inserted
 */
bool btree::insertKey(int key){
    bnode* rt = root;
    int depth = 0;
    while(depth < height-1){
        int i = 0;
        while(i < rt->keys.size() && key > rt->keys[i]) ++i;
        if(key == rt->keys[i])  return false;
        rt = rt->ptrs[i];
        ++depth;
    }
    rt->keys.push_back(key);
    insertion(rt->keys);
    rt->ptrs.push_back(NULL);

    /// check for overflow
    while(rt->keys.size() == maxb){
        if(!rt->prt){
            /// root node overflow, height increments
            rt->prt = new bnode(rt);
            root = rt->prt;
            ++height;
        }
        rt->prt->keys.push_back(rt->keys[maxb>>1]);
        rt->prt->ptrs.push_back(NULL);
        int idx = insertion(rt->prt->keys);
        insertionWithIdx(rt->prt->ptrs, idx+1);
        
        /// split node
        bnode* bn = new bnode();
        bn->keys = std::vector<int>(rt->keys.begin() + (maxb>>1) + 1, rt->keys.end());
        bn->ptrs = std::vector<bnode*>(rt->ptrs.begin() + (maxb>>1) + 1, rt->ptrs.end());
        for(auto ptr: bn->ptrs){
            /// update split parent of children
            if(ptr)  ptr->prt = bn;
        }
        bn->prt = rt->prt;
        rt->keys = std::vector<int>(rt->keys.begin(), rt->keys.begin() + (maxb>>1));
        rt->ptrs = std::vector<bnode*>(rt->ptrs.begin(), rt->ptrs.begin() + (maxb>>1) + 1);
        rt->prt->ptrs[idx+1] = bn;

        rt = rt->prt;
    }
    return true;
}


/**
 * Remove a given key
 * 
 * @param key    the key to be removed
 */
bool btree::removeKey(int key){
    bnode* rt = root;
    int depth = 0, idx = -1;
    while(depth < height){
        int i = 0;
        while(i < rt->keys.size() && key > rt->keys[i]) ++i;
        if(i < rt->keys.size() && key == rt->keys[i]){
            idx = i;
            break;
        }
        else  rt = rt->ptrs[i];
        ++depth;
    }
    if(depth == height)  return false;  // key not found, rt==NULL
    else if(depth < height-1){
        /// key is not on a leaf node, find the successive key
        bnode* node = rt->ptrs[idx+1];
        while((++depth) < height-1){
            node = node->ptrs[0];
        }
        rt->keys[idx] = node->keys[0];
        idx = 0;
        rt = node;
    }

    /// check for underflow
    deletionWithIdx(rt->keys, idx);
    rt->ptrs.pop_back();
    if(rt==root)  return true;  // only root node in the tree

    if(rt->keys.size() < ((maxb-1)>>1)){
        bnode *node = rt->prt;
        int i = 0;
        while(node->ptrs[i] != rt)  ++i;

        if(i+1 < node->ptrs.size() && node->ptrs[i+1]->keys.size() > ((maxb-1)>>1)){
            rt->keys.push_back(node->keys[i]);
            rt->ptrs.push_back(NULL);
            node->keys[i] = node->ptrs[i+1]->keys[0];
            deletionWithIdx(node->ptrs[i+1]->keys, 0);
            node->ptrs[i+1]->ptrs.pop_back();
        }
        else if(i-1 >= 0 && node->ptrs[i-1]->keys.size() > ((maxb-1)>>1)){
            rt->keys.push_back(node->keys[i-1]);
            rt->ptrs.push_back(NULL);
            insertionWithIdx(rt->keys, 0);
            node->keys[i-1] = node->ptrs[i-1]->keys[node->ptrs[i-1]->keys.size()-1];
            node->ptrs[i-1]->keys.pop_back();
            node->ptrs[i-1]->ptrs.pop_back();
        }
        else{
            /// neither left or right brother is able to spare a key
            /// merge the node with one of its brothers
            rotateAndMerge(rt, i);
            /// parent underflow?
            while(node->keys.size() < ((maxb-1)>>1)){
                if(node==root){
                    if(node->keys.empty()){
                        /// height changed
                        height--;
                        root = rt;
                    }
                    break;
                }
                rt = node;
                node = rt->prt;
                int i = 0;
                while(node->ptrs[i] != rt)  ++i;
                rotateAndMerge(rt, i);
            }
        }
    }
    return true;
}


/**
 * Seize a key from parent node and merge with brother
 * 
 * @param rt      the node with underflow
 * @param node    parent node
 */
void rotateAndMerge(bnode* rt, int i){
    bnode* node = rt->prt;
    if(i < node->keys.size()){
        rt->keys.push_back(node->keys[i]);
        rt->ptrs.push_back(NULL);
        /// merge to right brother
        mergeNode(rt, node->ptrs[i+1]);
        deletionWithIdx(node->keys, i);
        deletionWithIdx(node->ptrs, i+1);
    }
    else{
        rt->keys.push_back(node->keys[i-1]);
        insertionWithIdx(rt->keys, 0);
        node->ptrs[i-1]->ptrs.push_back(NULL);
        /// annex left brother
        mergeNode(node->ptrs[i-1], rt);
        deletionWithIdx(node->keys, i-1);
        deletionWithIdx(node->ptrs, i);
    }
}

void mergeNode(bnode* bn1, bnode* bn2){
    bn1->ptrs.pop_back();
    for(int i = 0; i < bn2->keys.size(); ++i){
        bn1->keys.push_back(bn2->keys[i]);
    }
    for(int i = 0; i < bn2->ptrs.size(); ++i){
        if(bn2->ptrs[i])  bn2->ptrs[i]->prt = bn1;  // do not forget to update abbributes of child nodes
        bn1->ptrs.push_back(bn2->ptrs[i]);
    }
}


void btree::traversal(){
    std::queue<bnode*> Q;
    Q.push(root);
    int remains = 1;
    while(!Q.empty()){
        bnode* node = Q.front();
        if(!node)  break;
        Q.pop();
        --remains;
        printf("(");
        for(int i = 0; i < node->keys.size(); ++i){
            printf("%d,", node->keys[i]);
        }
        printf("),");
        for(auto ptr: node->ptrs)  Q.push(ptr);
        if(remains==0){
            remains = Q.size();
            printf(";");
        }
    }
    printf("\n");
}

void btree::trav(bnode* rt){
    if(!rt)  return;
    printf("(");
    for(int i = 0; i < rt->keys.size(); ++i){
        printf("%d,", rt->keys[i]);
    }
    printf(");");
    for(int i = 0; i < rt->ptrs.size(); ++i){
        trav(rt->ptrs[i]);
    }
}


int insertion(std::vector<int>& arr){
    int idx = arr.size()-1;
    int tmp = arr[idx];
    while(idx-1 >= 0 && tmp < arr[idx-1]){
        arr[idx--] = arr[idx];
    }
    arr[idx] = tmp;
    return idx;
}

template<typename T>
void insertionWithIdx(std::vector<T> &arr, int idx){
    int i = arr.size()-1;
    T tmp = arr[i];
    for(; i > idx; --i){
        arr[i--] = arr[i];
    }
    arr[i] = tmp;
}

template<typename T>
void deletionWithIdx(std::vector<T> &arr, int idx){
    for(int i = idx; i < arr.size()-1; ++i){
        arr[i] = arr[i+1];
    }
    arr.pop_back();
}