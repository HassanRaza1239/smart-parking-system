#include "../include/PathFinder.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <unordered_map>

// PathNode constructor
PathNode::PathNode(const std::string& id, int dist, const std::string& prev)
    : zoneId(id), distance(dist), previousZone(prev) {}

// Comparison operator for min-heap
bool PathNode::operator>(const PathNode& other) const {
    return distance > other.distance;
}

// MinHeap constructor
MinHeap::MinHeap(int maxSize) : heapSize(0), capacity(maxSize) {
    heap = new PathNode*[capacity];
}

// MinHeap destructor
MinHeap::~MinHeap() {
    for (int i = 0; i < heapSize; i++) {
        delete heap[i];
    }
    delete[] heap;
}

// Heapify up
void MinHeap::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (*heap[index] > *heap[parent]) {
            break;
        }
        
        // Swap
        PathNode* temp = heap[index];
        heap[index] = heap[parent];
        heap[parent] = temp;
        index = parent;
    }
}

// Heapify down
void MinHeap::heapifyDown(int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    
    if (left < heapSize && *heap[left] > *heap[smallest]) {
        smallest = left;
    }
    
    if (right < heapSize && *heap[right] > *heap[smallest]) {
        smallest = right;
    }
    
    if (smallest != index) {
        // Swap
        PathNode* temp = heap[index];
        heap[index] = heap[smallest];
        heap[smallest] = temp;
        heapifyDown(smallest);
    }
}

// Push node to heap
void MinHeap::push(PathNode* node) {
    if (heapSize >= capacity) {
        // Resize heap (simplified)
        return;
    }
    
    heap[heapSize] = node;
    heapSize++;
    heapifyUp(heapSize - 1);
}

// Pop node from heap
PathNode* MinHeap::pop() {
    if (heapSize == 0) return nullptr;
    
    PathNode* root = heap[0];
    heap[0] = heap[heapSize - 1];
    heapSize--;
    heapifyDown(0);
    
    return root;
}

// Check if heap is empty
bool MinHeap::isEmpty() const {
    return heapSize == 0;
}

// Update distance of a node
void MinHeap::updateDistance(const std::string& zoneId, int newDistance, const std::string& previous) {
    // Find the node
    for (int i = 0; i < heapSize; i++) {
        if (heap[i]->zoneId == zoneId) {
            heap[i]->distance = newDistance;
            heap[i]->previousZone = previous;
            heapifyUp(i);
            break;
        }
    }
}

// PathFinder constructor
PathFinder::PathFinder(int maxZones) {
    priorityQueue = new MinHeap(maxZones);
}

// PathFinder destructor
PathFinder::~PathFinder() {
    delete priorityQueue;
}

// Dijkstra's algorithm to find shortest path
std::vector<std::string> PathFinder::findShortestPath(
    Zone* startZone,
    Zone* targetZone,
    Zone** allZones,
    int zoneCount
) {
    if (!startZone || !targetZone) return {};
    
    // Create distance map and previous zone map
    std::unordered_map<std::string, int> distances;
    std::unordered_map<std::string, std::string> previous;
    std::unordered_map<std::string, bool> visited;
    
    // Initialize distances
    for (int i = 0; i < zoneCount; i++) {
        std::string zoneId = allZones[i]->getZoneId();
        distances[zoneId] = std::numeric_limits<int>::max();
        previous[zoneId] = "";
        visited[zoneId] = false;
        
        // Add to priority queue
        int dist = (zoneId == startZone->getZoneId()) ? 0 : std::numeric_limits<int>::max();
        priorityQueue->push(new PathNode(zoneId, dist, ""));
    }
    
    // Set start zone distance to 0
    distances[startZone->getZoneId()] = 0;
    priorityQueue->updateDistance(startZone->getZoneId(), 0, "");
    
    // Main Dijkstra loop
    while (!priorityQueue->isEmpty()) {
        PathNode* current = priorityQueue->pop();
        std::string currentId = current->zoneId;
        
        if (visited[currentId]) {
            delete current;
            continue;
        }
        
        visited[currentId] = true;
        delete current;
        
        // If we reached target zone, reconstruct path
        if (currentId == targetZone->getZoneId()) {
            break;
        }
        
        // Find current zone object
        Zone* currentZone = nullptr;
        for (int i = 0; i < zoneCount; i++) {
            if (allZones[i]->getZoneId() == currentId) {
                currentZone = allZones[i];
                break;
            }
        }
        
        if (!currentZone) continue;
        
        // Explore neighbors
        ZoneConnection* connections = currentZone->getConnections();
        ZoneConnection* currentConn = connections;
        
        while (currentConn != nullptr) {
            std::string neighborId = currentConn->connectedZoneId;
            int edgeWeight = currentConn->distance;
            
            if (!visited[neighborId]) {
                int newDist = distances[currentId] + edgeWeight;
                
                if (newDist < distances[neighborId]) {
                    distances[neighborId] = newDist;
                    previous[neighborId] = currentId;
                    priorityQueue->updateDistance(neighborId, newDist, currentId);
                }
            }
            
            currentConn = currentConn->next;
        }
    }
    
    // Reconstruct path from target to start
    std::vector<std::string> path;
    std::string current = targetZone->getZoneId();
    
    while (!current.empty()) {
        path.push_back(current);
        current = previous[current];
    }
    
    // Reverse to get start → target path
    std::reverse(path.begin(), path.end());
    
    // Check if path was found
    if (path.size() == 1 && path[0] == targetZone->getZoneId()) {
        // No path found
        return {};
    }
    
    return path;
}

// Calculate total distance of path
int PathFinder::calculateDistance(const std::vector<std::string>& path, 
                                  Zone** allZones, int zoneCount) {
    if (path.size() < 2) return 0;
    
    int totalDistance = 0;
    
    for (size_t i = 0; i < path.size() - 1; i++) {
        std::string from = path[i];
        std::string to = path[i + 1];
        
        // Find 'from' zone
        Zone* fromZone = nullptr;
        for (int j = 0; j < zoneCount; j++) {
            if (allZones[j]->getZoneId() == from) {
                fromZone = allZones[j];
                break;
            }
        }
        
        if (!fromZone) continue;
        
        // Find distance to 'to' zone
        ZoneConnection* connections = fromZone->getConnections();
        ZoneConnection* current = connections;
        
        while (current != nullptr) {
            if (current->connectedZoneId == to) {
                totalDistance += current->distance;
                break;
            }
            current = current->next;
        }
    }
    
    return totalDistance;
}

// Find nearest available zone from start zone
std::string PathFinder::findNearestAvailableZone(
    const std::string& startZoneId,
    Zone** allZones,
    int zoneCount,
    std::vector<std::string>& path
) {
    // Find start zone
    Zone* startZone = nullptr;
    for (int i = 0; i < zoneCount; i++) {
        if (allZones[i]->getZoneId() == startZoneId) {
            startZone = allZones[i];
            break;
        }
    }
    
    if (!startZone) return "";
    
    // Find all zones with available slots
    std::vector<Zone*> availableZones;
    for (int i = 0; i < zoneCount; i++) {
        if (allZones[i]->getAvailableSlots() > 0 && 
            allZones[i]->getZoneId() != startZoneId) {
            availableZones.push_back(allZones[i]);
        }
    }
    
    if (availableZones.empty()) return "";
    
    // Find nearest available zone using Dijkstra
    std::string nearestZone;
    int minDistance = std::numeric_limits<int>::max();
    std::vector<std::string> bestPath;
    
    for (Zone* targetZone : availableZones) {
        std::vector<std::string> currentPath = findShortestPath(startZone, targetZone, allZones, zoneCount);
        
        if (!currentPath.empty()) {
            int distance = calculateDistance(currentPath, allZones, zoneCount);
            
            if (distance < minDistance) {
                minDistance = distance;
                nearestZone = targetZone->getZoneId();
                bestPath = currentPath;
            }
        }
    }
    
    path = bestPath;
    return nearestZone;
}
