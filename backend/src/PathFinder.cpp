#include "PathFinder.h"
#include <iostream>
#include <climits>

// PathNode constructor
PathNode::PathNode(const std::string& id, int dist, const std::string& prev)
    : zoneId(id), distance(dist), previousZone(prev) {}

// Comparison operator for min-heap
bool PathNode::operator>(const PathNode& other) const {
    return distance > other.distance;
}

// MinHeap constructor
MinHeap::MinHeap(int maxSize) : heapSize(0), capacity(maxSize) {
    heap = new PathNode*[capacity + 1];
}

// MinHeap destructor
MinHeap::~MinHeap() {
    for (int i = 1; i <= heapSize; i++) {
        delete heap[i];
    }
    delete[] heap;
}

// Maintain heap property upward
void MinHeap::heapifyUp(int index) {
    while (index > 1 && *heap[index] > *heap[index / 2]) {
        std::swap(heap[index], heap[index / 2]);
        index /= 2;
    }
}

// Maintain heap property downward
void MinHeap::heapifyDown(int index) {
    while (2 * index <= heapSize) {
        int child = 2 * index;
        if (child + 1 <= heapSize && *heap[child + 1] > *heap[child]) {
            child++;
        }
        if (!(*heap[index] > *heap[child])) {
            break;
        }
        std::swap(heap[index], heap[child]);
        index = child;
    }
}

// Push node to heap
void MinHeap::push(PathNode* node) {
    if (heapSize == capacity) {
        // Double capacity
        capacity *= 2;
        PathNode** newHeap = new PathNode*[capacity + 1];
        for (int i = 1; i <= heapSize; i++) {
            newHeap[i] = heap[i];
        }
        delete[] heap;
        heap = newHeap;
    }
    
    heap[++heapSize] = node;
    heapifyUp(heapSize);
}

// Pop minimum node from heap
PathNode* MinHeap::pop() {
    if (heapSize == 0) return nullptr;
    
    PathNode* minNode = heap[1];
    heap[1] = heap[heapSize--];
    heapifyDown(1);
    
    return minNode;
}

// Check if heap is empty
bool MinHeap::isEmpty() const {
    return heapSize == 0;
}

// Update distance of a zone
void MinHeap::updateDistance(const std::string& zoneId, int newDistance, const std::string& previous) {
    // Simplified - in full implementation would find and update node
    // For now, push new node (old one will be ignored when popped)
    push(new PathNode(zoneId, newDistance, previous));
}

// PathFinder constructor
PathFinder::PathFinder(int maxZones) {
    priorityQueue = new MinHeap(maxZones);
}

// PathFinder destructor
PathFinder::~PathFinder() {
    delete priorityQueue;
}

// Dijkstra's algorithm implementation
std::vector<std::string> PathFinder::findShortestPath(
    Zone* startZone,
    Zone* targetZone,
    Zone** allZones,
    int zoneCount
) {
    std::vector<std::string> path;
    
    if (!startZone || !targetZone) {
        return path;
    }
    
    // Simplified Dijkstra's implementation
    // In full system, would implement complete algorithm
    
    // For now, return direct path if zones are connected
    ZoneConnection* conn = startZone->getConnections();
    while (conn != nullptr) {
        if (conn->connectedZoneId == targetZone->getZoneId()) {
            path.push_back(startZone->getZoneId());
            path.push_back(targetZone->getZoneId());
            break;
        }
        conn = conn->next;
    }
    
    return path;
}

// Calculate total distance of path
int PathFinder::calculateDistance(const std::vector<std::string>& path, Zone** allZones, int zoneCount) {
    if (path.size() < 2) return 0;
    
    int totalDistance = 0;
    
    // Find zones and sum distances between consecutive zones
    for (size_t i = 0; i < path.size() - 1; i++) {
        for (int j = 0; j < zoneCount; j++) {
            if (allZones[j]->getZoneId() == path[i]) {
                int distance = allZones[j]->getDistanceTo(path[i + 1]);
                if (distance > 0) {
                    totalDistance += distance;
                }
                break;
            }
        }
    }
    
    return totalDistance;
}

// Find nearest available zone using Dijkstra's algorithm
std::string PathFinder::findNearestAvailableZone(
    const std::string& startZoneId,
    Zone** allZones,
    int zoneCount,
    std::vector<std::string>& path
) {
    // Simplified implementation
    // In full system, would use Dijkstra to find nearest zone with available slots
    
    // Find start zone
    Zone* startZone = nullptr;
    for (int i = 0; i < zoneCount; i++) {
        if (allZones[i]->getZoneId() == startZoneId) {
            startZone = allZones[i];
            break;
        }
    }
    
    if (!startZone) return "";
    
    // Find nearest available zone (simplified - checks direct connections)
    ZoneConnection* conn = startZone->getConnections();
    Zone* nearestZone = nullptr;
    int minDistance = INT_MAX;
    
    while (conn != nullptr) {
        // Find the connected zone
        for (int i = 0; i < zoneCount; i++) {
            if (allZones[i]->getZoneId() == conn->connectedZoneId) {
                if (allZones[i]->getAvailableSlots() > 0) {
                    if (conn->distance < minDistance) {
                        minDistance = conn->distance;
                        nearestZone = allZones[i];
                    }
                }
                break;
            }
        }
        conn = conn->next;
    }
    
    if (nearestZone) {
        path.push_back(startZoneId);
        path.push_back(nearestZone->getZoneId());
        return nearestZone->getZoneId();
    }
    
    return "";
}