#include "CoordinateIndex.h"

#include "SystemManager.h"

using namespace std;

CoordinateIndex::CoordinateIndex()
{
}

void CoordinateIndex::CreatePRQuadtree()
{ 
    this->minLat = -90;   // Minimum latitude value
    this->maxLat = 90;    // Maximum latitude value
    this->minLon = -180;  // Minimum longitude value
    this->maxLon = 180;   // Maximum longitude value

    this->root = new Node(minLat, maxLat, minLon, maxLon);
}

void CoordinateIndex::Insert(Point& point)
{
    this->root->insert(point);
}

void Node::insert(Point point)
{
    // If the node is a leaf and has fewer than 4 points, add the point to the list of points
    if (isLeaf() && points.size() < 4) {
        bool dup = false;
        for (auto& i : points)
        {
            if (i.lat == point.lat && i.lon == point.lon)
            {
                for (auto& j : point.offsets)
                    i.offsets.push_back(j);
                return;
            }
        }
        points.push_back(point);
        return;
    }

    // Calculate the midpoint of this node's bounds
    float latMid = (minLat + maxLat) / 2;
    float lonMid = (minLon + maxLon) / 2;

    // If the node is a leaf and has 4 points, split it into quadrants and insert the points into the appropriate quadrant
    if (isLeaf()) {
        // Create the child nodes for the bottom-left, bottom-right, top-left, and top-right quadrants
        southWest = new Node(minLat, latMid, minLon, lonMid);
        southEast = new Node(minLat, latMid, lonMid, maxLon);
        northWest = new Node(latMid, maxLat, minLon, lonMid);
        northEast = new Node(latMid, maxLat, lonMid, maxLon);

        // Insert the points that were previously in this node into the appropriate quadrants
        for (Point& p : points) {
            insert(p);
        }

        // Clear the list of points in this node
        points.clear();
    }

    // Determine which child node the new point belongs in and insert it into that quadrant
    if (point.lat <= latMid && point.lon <= lonMid) {
        // Point belongs in the bottom-left quadrant
        southWest->insert(point);
    }
    else if (point.lat <= latMid && point.lon > lonMid) {
        // Point belongs in the bottom-right quadrant
        southEast->insert(point);
    }
    else if (point.lat > latMid && point.lon <= lonMid) {
        // Point belongs in the top-left quadrant
        northWest->insert(point);
    }
    else if (point.lat > latMid && point.lon > lonMid) {
        // Point belongs in the top-right quadrant
        northEast->insert(point);
    }
}

// https://www.tutorialspoint.com/floating-point-comparison-in-cplusplus#:~:text=To%20compare%20two%20floating%20point%20values%2C%20we%20have%20to%20consider,0.001%20they%20are%20not%20same.

bool compare_float(float x, float y, float epsilon = 0.001f)
{
    if (fabs(x - y) < epsilon)
        return true; //they are same
    return false; //they are not same
}

// Search for points at location
vector<int> CoordinateIndex::search(float lat, float lon)
{
    vector<int> foundPoints;
    // Call the search function on the root node
    root->search(lat, lon, foundPoints);

    return foundPoints;
}

// Function to search for points at location
void Node::search(float lat, float lon, vector<int>& foundPoints)
{
    if (isLeaf()) {
        for (Point& p : points) {
            if (compare_float(p.lat, lat) && compare_float(p.lon, lon)) {
                // Point was found
                for (auto& i : p.offsets)
                    foundPoints.push_back(i);
            }
        }
        // Point was not found in this leaf
    }

    // Calculate the midpoint of this node's bounds
    float latMid = (minLat + maxLat) / 2;
    float lonMid = (minLon + maxLon) / 2;

    // Determine which child node the point belongs in and search it
    if (lat <= latMid && lon <= lonMid && southWest) {
        // Point belongs in the bottom-left quadrant
        southWest->search(lat, lon, foundPoints);
    }
    else if (lat <= latMid && lon > lonMid && southEast) {
        // Point belongs in the bottom-right quadrant
        southEast->search(lat, lon, foundPoints);
    }
    else if (lat > latMid && lon <= lonMid && northWest) {
        // Point belongs in the top-left quadrant
        northWest->search(lat, lon, foundPoints);
    }
    else if (lat > latMid && lon > lonMid && northEast) {
        // Point belongs in the top-right quadrant
        northEast->search(lat, lon, foundPoints);
    }
}

// Search for points in the given bounding box
vector<int> CoordinateIndex::findArea(float lat, float lon, float width, float height)
{
    vector<int> foundPoints;

    float minLat = lat - height;
    float maxLat = lat + height;
    float minLon = lon - width;
    float maxLon = lon + width;

    // Call the area function on the root node
    root->findArea(minLat, maxLat, minLon, maxLon, foundPoints);

    return foundPoints;
}

// Function to search for points within the given bounding box
void Node::findArea(float minLat, float maxLat, float minLon, float maxLon, vector<int>& foundPoints)
{
    // Check if this node intersects with the given bounds
    if (minLat <= this->maxLat && maxLat >= this->minLat && minLon <= this->maxLon && maxLon >= this->minLon) {
        // This node intersects with the given bounds, so check the points in this node
        for (Point& p : points) {
            if (p.lat >= minLat && p.lat <= maxLat && p.lon >= minLon && p.lon <= maxLon) {
                // Point is within the given bounds, so output it
                for (auto& i : p.offsets)
                    foundPoints.push_back(i);
            }
        }

        // Recursively call the search function on the child nodes
        if (southWest) {
            southWest->findArea(minLat, maxLat, minLon, maxLon, foundPoints);
        }
        if (southEast) {
            southEast->findArea(minLat, maxLat, minLon, maxLon, foundPoints);
        }
        if (northWest) {
            northWest->findArea(minLat, maxLat, minLon, maxLon, foundPoints);
        }
        if (northEast) {
            northEast->findArea(minLat, maxLat, minLon, maxLon, foundPoints);
        }
    }
}

void CoordinateIndex::Print()
{
    // Call the recursive visualize function on the root node
    root->Print(0);
}

// Function to recursively visualize the PR quadtree
void Node::Print(int depth)
{

    SystemManager& systemmanager = SystemManager::GetInstance();
    ostringstream os;

    // Output the points contained in this node
    if (!points.empty())
        for (int i = 0; i <= depth; i++)
            os << "-";
    for (Point& p : points)
    {
        os << "(" << p.lat << ", " << p.lon << ")[";

        for (auto i : p.offsets)
            os << i << "] ";
    }
    systemmanager.m_Logger->LogString(os.str());

    // Recursively call the display function on the child nodes
    if (southWest) {
        southWest->Print(depth + 1);
    }
    if (southEast) {
        southEast->Print(depth + 1);
    }
    if (northWest) {
        northWest->Print(depth + 1);
    }
    if (northEast) {
        northEast->Print(depth + 1);
    }
}