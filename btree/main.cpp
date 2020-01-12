#include <stdio.h>
#include "btree.h"

int main(int argc, char **argv){
    btree* obj = new btree(3);
    for(int i = 1; i <= 10; ++i){
        obj->insertKey(i);
        printf("height=%d\n", obj->getHeight());
        obj->traversal();
    }
    printf("\n");
    obj->removeKey(2);
    printf("height=%d\n", obj->getHeight());
    obj->traversal();

    obj->removeKey(7);
    printf("height=%d\n", obj->getHeight());
    obj->traversal();

    obj->removeKey(5);
    printf("height=%d\n", obj->getHeight());
    obj->traversal();
    return 0;
}