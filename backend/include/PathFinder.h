#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Zone.h"
#include <string>
#include <vector>

// Custom priority queue node for Dijkstra's algorithm
struct PathNode {
    std::string zoneId;
    int distance;
    std::string previousZone;
    
    PathNode(const std::string& id, int dist, const std::string& prev = "");
    bool operator>(const PathNode& other) const;
};

// Custom min-heap for Dijkstra's algorithm
class MinHeap {
private:
    PathNode** heap;
    int heapSize;
    int capacity;
    
    void heapifyUp(int index);
    void heapifyDown(int index);
    
public:
    MinHeap(int maxSize);
    ~MinHeap();
    
    void push(PathNode* node);
    PathNode* pop();
    bool isEmpty() const;
    void updateDistance(const std::string& zoneId, int newDistance, const std::string& previous);
};

// PathFinder class implementing Dijkstra's algorithm
class PathFinder {
private:
    MinHeap* priorityQueue;
    
public:
    PathFinder(int maxZones);
    ~PathFinder();
    
    // Dijkstra's algorithm implementation
    std::vector<std::string> findShortestPath(
        Zone* startZone,
        Zone* targetZone,
        Zone** allZones,
        int zoneCount
    );
    
    // Calculate total distance
    int calculateDistance(const std::vector<std::string>& path, Zone** allZones, int zoneCount);
    
    // Find nearest available zone
    std::string findNearestAvailableZone(
        const std::string& startZoneId,
        Zone** allZones,
        int zoneCount,
        std::vector<std::string>& path
    );
};

#endif