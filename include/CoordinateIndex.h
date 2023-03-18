#ifndef COORDINATEINDEX_H
#define COORDINATEINDEX_H

#include "DataStructures.h"

#include <string>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Quadtree node class
class Node
{
    public:
        // coordinates of the bounding box of this node
        float minLat, maxLat, minLon, maxLon;

        // pointers to the child nodes (null if the node is a leaf node)
        Node* northWest;
        Node* northEast;
        Node* southWest;
        Node* southEast;

        // list of points contained in this node (empty for non-leaf nodes)
        vector<Point> points;

        Node(float minLat, float maxLat, float minLon, float maxLon)
        {
            this->minLat = minLat;
            this->maxLat = maxLat;
            this->minLon = minLon;
            this->maxLon = maxLon;
            this->northWest = nullptr;
            this->northEast = nullptr;
            this->southWest = nullptr;
            this->southEast = nullptr;
        }

        void insert(Point point);
        void search(float lat, float lon, vector<int>& foundPoints);
        void findArea(float minLat, float maxLat, float minLon, float maxLon, vector<int>& foundPoints);
        
        // helper function to determine if the given point is contained in this node
        bool isLeaf() {
            // A node is a leaf if it has no child nodes
            return !northWest && !northEast && !southWest && !southEast;
        }

        void Print(int depth);
};

class CoordinateIndex
{
    const int MAX_POINTS = 4;

    public:
        float minLat, maxLat, minLon, maxLon;

        Node* root; // root node of the quadtree

        CoordinateIndex();

        void CreatePRQuadtree();

        void Insert(Point& point);
        vector<int> search(float lat, float lon);
        vector<int> findArea(float lat, float lon, float width, float height);
        
        void Print();

    private:
        ;
};

#endif