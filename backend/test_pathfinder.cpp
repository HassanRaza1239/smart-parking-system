#include "include/PathFinder.h"
#include "include/Zone.h"
#include <iostream>

int main() {
    std::cout << "=== Testing PathFinder with Dijkstra's Algorithm ===\n" << std::endl;
    
    // Create zones with connections (graph)
    std::cout << "Test 1: Creating City Graph..." << std::endl;
    
    Zone* zones[5];
    zones[0] = new Zone("ZA", "Zone A", 30, 5.0);
    zones[1] = new Zone("ZB", "Zone B", 25, 6.0);
    zones[2] = new Zone("ZC", "Zone C", 40, 4.5);
    zones[3] = new Zone("ZD", "Zone D", 35, 3.5);
    zones[4] = new Zone("ZE", "Zone E", 20, 7.0);
    
    // Create connections (weighted edges)
    // Zone A connections
    zones[0]->addConnection(new ZoneConnection("ZB", 500));  // A to B: 500m
    zones[0]->addConnection(new ZoneConnection("ZC", 800));  // A to C: 800m
    
    // Zone B connections
    zones[1]->addConnection(new ZoneConnection("ZA", 500));  // B to A: 500m
    zones[1]->addConnection(new ZoneConnection("ZC", 300));  // B to C: 300m
    zones[1]->addConnection(new ZoneConnection("ZD", 700));  // B to D: 700m
    
    // Zone C connections
    zones[2]->addConnection(new ZoneConnection("ZA", 800));  // C to A: 800m
    zones[2]->addConnection(new ZoneConnection("ZB", 300));  // C to B: 300m
    zones[2]->addConnection(new ZoneConnection("ZE", 400));  // C to E: 400m
    
    // Zone D connections
    zones[3]->addConnection(new ZoneConnection("ZB", 700));  // D to B: 700m
    zones[3]->addConnection(new ZoneConnection("ZE", 600));  // D to E: 600m
    
    // Zone E connections
    zones[4]->addConnection(new ZoneConnection("ZC", 400));  // E to C: 400m
    zones[4]->addConnection(new ZoneConnection("ZD", 600));  // E to D: 600m
    
    std::cout << "✅ Created 5 zones with weighted connections" << std::endl;
    
    // Create PathFinder
    std::cout << "\nTest 2: Creating PathFinder..." << std::endl;
    PathFinder pathFinder(10);
    std::cout << "✅ PathFinder created with Dijkstra's algorithm" << std::endl;
    
    // Test path finding
    std::cout << "\nTest 3: Finding Shortest Path..." << std::endl;
    std::vector<std::string> path = pathFinder.findShortestPath(zones[0], zones[4], zones, 5);
    
    if (!path.empty()) {
        std::cout << "✅ Found path from " << zones[0]->getZoneId() 
                  << " to " << zones[4]->getZoneId() << ": ";
        for (size_t i = 0; i < path.size(); i++) {
            std::cout << path[i];
            if (i < path.size() - 1) std::cout << " -> ";
        }
        std::cout << std::endl;
        
        // Calculate distance
        int distance = pathFinder.calculateDistance(path, zones, 5);
        std::cout << "Total distance: " << distance << " meters" << std::endl;
    } else {
        std::cout << "❌ No path found" << std::endl;
    }
    
    // Test nearest available zone
    std::cout << "\nTest 4: Finding Nearest Available Zone..." << std::endl;
    
    // Simulate some zones being full
    for (int i = 0; i < 30; i++) zones[0]->allocateSlot(); // Zone A full
    for (int i = 0; i < 20; i++) zones[2]->allocateSlot(); // Zone C partially full
    
    std::cout << "Zone A available: " << zones[0]->getAvailableSlots() << std::endl;
    std::cout << "Zone B available: " << zones[1]->getAvailableSlots() << std::endl;
    std::cout << "Zone C available: " << zones[2]->getAvailableSlots() << std::endl;
    std::cout << "Zone D available: " << zones[3]->getAvailableSlots() << std::endl;
    std::cout << "Zone E available: " << zones[4]->getAvailableSlots() << std::endl;
    
    std::vector<std::string> nearestPath;
    std::string nearestZone = pathFinder.findNearestAvailableZone("ZA", zones, 5, nearestPath);
    
    if (!nearestZone.empty()) {
        std::cout << "\n✅ Nearest available zone from Zone A: " << nearestZone << std::endl;
        std::cout << "Path: ";
        for (size_t i = 0; i < nearestPath.size(); i++) {
            std::cout << nearestPath[i];
            if (i < nearestPath.size() - 1) std::cout << " -> ";
        }
        std::cout << std::endl;
        
        int nearestDistance = pathFinder.calculateDistance(nearestPath, zones, 5);
        std::cout << "Distance: " << nearestDistance << " meters" << std::endl;
    } else {
        std::cout << "\n❌ No available zone found" << std::endl;
    }
    
    // Cleanup
    std::cout << "\nTest 5: Cleanup..." << std::endl;
    for (int i = 0; i < 5; i++) {
        delete zones[i];
    }
    
    std::cout << "\n=== All PathFinder Tests Complete! ===" << std::endl;
    return 0;
}