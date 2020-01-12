#include <stdio.h>
#include "rbtree.h"

int main(int argc, char **argv){
    rbtree *obj = new rbtree();
    for(int i = 1; i <= 8; ++i){
        obj->insertKey(i);
        obj->traversal();
    }
    obj->removeKey(4);
    obj->traversal();

    obj->removeKey(6);
    obj->traversal();

    obj->removeKey(7);
    obj->traversal();

    obj->removeKey(8);
    obj->traversal();

    obj->removeKey(1);
    obj->traversal();

    obj->removeKey(2);
    obj->traversal();

    obj->removeKey(3);
    obj->traversal();

    obj->removeKey(5);
    obj->traversal();

    obj->insertKey(10);
    obj->traversal();

    obj->insertKey(9);
    obj->traversal();
    return 0;
}