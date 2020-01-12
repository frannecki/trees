#include <stdio.h>
#include "kdtree.h"

int main(int argc, char **argv){
    int coords[4][2] = {{4,7},{3,6}, {9,12}, {0, 54}};
    std::vector<Point> pts;
    for(int i = 0; i < 4; ++i){
        pts.push_back(Point(std::vector<int>(coords[i], coords[i]+2)));
    }
    kdtree kt(pts);
    std::vector<kdnode*> trav;
    kt.traversal(trav);
    for(auto nd: trav){
        printf("(%d,%d) ", nd->coords[0], nd->coords[1]);
    }
    printf("\n");
    Point pt(std::vector<int>({3, 10}));
    kdnode* node = new kdnode();
    node->coords = std::vector<int>({10, 14});
    kt.findNearest(pt, node);
    printf("Nearest: {%d,%d}\n", node->coords[0], node->coords[1]);

    Point lowerLeft(std::vector<int>({2, 0}));
    Point upperRight(std::vector<int>({5, 13}));
    RectArea rect(lowerLeft, upperRight);
    std::vector<Point> nodesWithinRange;
    kt.searchRegion(rect, nodesWithinRange);
    for(auto point: nodesWithinRange){
        printf("(%d,%d) ", point.coords[0], point.coords[1]);
    }
    printf("\n");
    return 0;
}