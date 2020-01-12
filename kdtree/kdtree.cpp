#include "kdtree.h"


bool Point::toLeft(Point &pt, int dimension){
	return coords[dimension] < pt.coords[dimension];
}

bool Point::toLeft(kdnode *rt, int dimension){
	return coords[dimension] < rt->coords[dimension];
}


kdtree::kdtree(){
	root = NULL;
}


kdtree::kdtree(std::vector<Point> &pts){
	root = NULL;
	if(pts.empty())  return;
	dim = pts[0].coords.size();
	root = buildKDTree(pts, 0);
}

kdnode* kdtree::buildKDTree(std::vector<Point> &pts, int d){
	if(pts.empty())  return NULL;
	int median = findMedian(pts, d);
	std::vector<Point> pts1, pts2;
	for(int i = 0; i < pts.size(); ++i){
		if(i==median)  continue;
		if(pts[i].toLeft(pts[median], d)){
			pts1.push_back(pts[i]);
		}
		else pts2.push_back(pts[i]);
	}
	kdnode* rt = new kdnode();
	rt->coords = pts[median].coords;
	rt->lc = buildKDTree(pts1, (d+1)%dim);
	rt->rc = buildKDTree(pts2, (d+1)%dim);
	return rt;
}


/**
 * Search for nodes within a given range
 * 
 * @param rect    the given range
 * @param res     array to store the found nodes
 */
void kdtree::searchRegion(RectArea &rect, std::vector<Point> &res){
	searchR(rect, root, res, 0);
}

void kdtree::searchR(RectArea &rect, kdnode* rt, std::vector<Point> &res, int d){
	if(!rt)  return;
	bool inside = true;
	for(int i = 0; i < dim; ++i){
		if(rt->coords[i] > rect.upperRight.coords[i] || rt->coords[i] < rect.lowerLeft.coords[i]){
			inside = false;
			break;
		}
	}
	if(inside)  res.push_back(Point(rt->coords));
	if(rt->coords[d] > rect.lowerLeft.coords[d]  &&  rt->coords[d] <= rect.upperRight.coords[d]){
		RectArea rectLeft = rect, rectRight = rect;
		rectLeft.upperRight.coords[d] = rt->coords[d];
		rectRight.lowerLeft.coords[d] = rt->coords[d];
		searchR(rectLeft, rt->lc, res, (d+1)%dim);
		searchR(rectRight, rt->rc, res, (d+1)%dim);
	}
	else if(rt->coords[d] <= rect.upperRight.coords[d]){
		searchR(rect, rt->rc, res, (d+1)%dim);
	}
	else{
		searchR(rect, rt->lc, res, (d+1)%dim);
	}
}


/**
 * Find the node closest to a given node
 * 
 * @param pt         the given node
 * @param nearest    the nearest node
 */
void kdtree::findNearest(Point &pt, kdnode* &nearest){
	findNN(pt, root, nearest, 0);
}

void kdtree::findNN(Point &pt, kdnode* rt, kdnode* &nearest, int d){
	if(!rt)  return;
	if(calcDist(pt, rt) < calcDist(pt, nearest)){
		nearest = rt;
	}
	if(pt.toLeft(rt, d)){
		findNN(pt, rt->lc, nearest, (d+1)%dim);
		if(calcDist(pt, nearest) > (pt.coords[d]-rt->coords[d]) * (pt.coords[d]-rt->coords[d])){
			findNN(pt, rt->rc, nearest, (d+1)%dim);
		}
	}
	else{
		findNN(pt, rt->rc, nearest, (d+1)%dim);
		if(calcDist(pt, nearest) > (pt.coords[d]-rt->coords[d]) * (pt.coords[d]-rt->coords[d])){
			findNN(pt, rt->lc, nearest, (d+1)%dim);
		}
	}
}


/**
 * Traversal the kdtree (inorder)
 * 
 * @param record    array to store the nodes in the tree
 */
void kdtree::traversal(std::vector<kdnode*> &record){
	trav(record, root);
}

void kdtree::trav(std::vector<kdnode*> &record, kdnode* rt){
	if(!rt)  return;
	record.push_back(rt);
	trav(record, rt->lc);
	trav(record, rt->rc);
}


/**
 * Find the node that ranked median by a given axis
 * 
 * @param pts          the coordinate all nodes
 * @param dimension    the given axis
 */
int findMedian(const std::vector<Point> &pts, int dimension){
	int cnt = 0;
	std::vector<cmp> coords_d;
	for(auto pt: pts){
		coords_d.push_back(cmp(cnt++, pt.coords[dimension]));
	}
	for(int i = (cnt-2)>>1; i >= 0; --i){
		adjustDown(coords_d, i, cnt);
	}

	int len = cnt;
	for(int i = 0; i < (cnt>>1); ++i){
		coords_d[0] = coords_d[--len];
		adjustDown(coords_d, 0, len);
	}
	return coords_d[0].idx;
}

void adjustDown(std::vector<cmp> &coords, int i, int len){
	cmp tmp = coords[i];
	int prt = i, child;
	while((child=2*prt+1) < len){
		if(coords[child] < coords[prt]){
			if(child+1 < len && coords[child+1] < coords[child]){
				child = child+1;
			}
			coords[prt] = coords[child];
		}
		else if(child+1 < len && coords[child+1] < coords[prt]){
			child = child+1;
			coords[prt] = coords[child];
		}
		else  break;
		prt = child;
	}
	coords[prt] = tmp;
}


int calcDist(Point &pt1, kdnode *nd){
	int res = 0;
	for(int i = 0; i < pt1.coords.size(); ++i){
		res += (pt1.coords[i]-nd->coords[i]) * (pt1.coords[i]-nd->coords[i]);
	}
	return res;
}

template<typename T>
void swap(T &a, T &b){
    T tmp = a;
    a = b;
    b = tmp;
}