#include <stdio.h>
#include "avltree.h"

int main(int argc, char **argv){
    avltree* obj = new avltree();
    for(int i = 1; i <= 10; ++i){
        obj->insertKey(i);
        obj->traversal();
    }
    obj->removeKey(11);
    obj->traversal();

    obj->removeKey(8);
    obj->traversal();

    obj->removeKey(4);
    obj->traversal();

    obj->removeKey(2);
    obj->traversal();

    obj->removeKey(7);
    obj->traversal();
    return 0;
}