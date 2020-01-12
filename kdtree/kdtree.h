#ifndef KDTREE_H
#define KDTREE_H
#include <vector>
#include <algorithm>

struct cmp{
	int idx, val;
	cmp(int i, int v): idx(i), val(v){};
	friend bool operator < (const cmp &c1, const cmp &c2){
		return c1.val < c2.val;
	}
};

class kdnode;

class Point{
public:
    std::vector<int> coords;
    Point(){};
    Point(std::vector<int> crds): coords(crds){};
    bool toLeft(Point&, int);
    bool toLeft(kdnode*, int);
};

struct RectArea{
	Point lowerLeft, upperRight;
    RectArea();
	RectArea(const Point &ll, const Point &ur): lowerLeft(ll), upperRight(ur){};
};

class kdnode: public Point{
public:
	kdnode *lc, *rc;
	kdnode(): lc(NULL), rc(NULL){};
};

class kdtree {
public:
    kdtree();
	kdtree(std::vector<Point>&);
    void searchRegion(RectArea&, std::vector<Point>&);
	void findNearest(Point&, kdnode*&);
    void traversal(std::vector<kdnode*>&);

private:
    int dim;
    kdnode* root;
	kdnode* buildKDTree(std::vector<Point>&, int);
    void searchR(RectArea&, kdnode*, std::vector<Point>&, int);
    void findNN(Point&, kdnode*, kdnode*&, int d);
    void trav(std::vector<kdnode*>&, kdnode*);
};

int findMedian(const std::vector<Point>&, int);

void adjustDown(std::vector<cmp>&, int, int);

int calcDist(Point&, kdnode*);

template<typename T>
void swap(T&, T&);

#endif