#include "../include/AllocationEngine.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>

// AllocationEngine constructor
AllocationEngine::AllocationEngine(Zone** zoneArray, int count)
    : zones(zoneArray), zoneCount(count) {
    pathFinder = new PathFinder(count);
}

// AllocationEngine destructor
AllocationEngine::~AllocationEngine() {
    delete pathFinder;
}

// Find available slot in specific zone
std::string AllocationEngine::findAvailableSlotInZone(const std::string& zoneId) {
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i]->getZoneId() == zoneId) {
            if (zones[i]->getAvailableSlots() > 0) {
                // Allocate a slot
                if (zones[i]->allocateSlot()) {
                    // Generate slot ID
                    static int slotCounter = 1;
                    return zoneId + "-" + std::to_string(slotCounter++);
                }
            }
            break;
        }
    }
    return "";
}

// Calculate parking cost
double AllocationEngine::calculateCost(
    const std::string& zoneId,
    int hours,
    bool crossZone,
    Vehicle* vehicle
) const {
    // Find zone to get hourly rate
    double hourlyRate = 0.0;
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i]->getZoneId() == zoneId) {
            hourlyRate = zones[i]->getHourlyRate();
            break;
        }
    }
    
    if (hourlyRate == 0.0) return 0.0;
    
    // Calculate cost: base rate × hours × vehicle multiplier × cross-zone multiplier
    double cost = hourlyRate * hours;
    cost *= vehicle->getTypeMultiplier();
    
    if (crossZone) {
        cost *= 1.5; // 50% penalty for cross-zone parking
    }
    
    return cost;
}

// Main allocation function
bool AllocationEngine::allocateParking(
    ParkingRequest* request,
    Vehicle* vehicle,
    int durationHours,
    std::string& allocatedZone,
    std::string& allocatedSlot,
    std::vector<std::string>& optimalPath,
    double& totalCost
) {
    std::string preferredZone = request->getPreferredZone();
    
    // Step 1: Try to allocate in preferred zone
    allocatedSlot = findAvailableSlotInZone(preferredZone);
    if (!allocatedSlot.empty()) {
        allocatedZone = preferredZone;
        optimalPath.push_back(preferredZone);
        totalCost = calculateCost(preferredZone, durationHours, false, vehicle);
        
        // Update request
        return request->allocate(allocatedZone, allocatedSlot, totalCost, false);
    }
    
    // Step 2: Preferred zone is full, find nearest available zone
    allocatedZone = findNearestAvailableZone(preferredZone, optimalPath);
    if (!allocatedZone.empty()) {
        allocatedSlot = findAvailableSlotInZone(allocatedZone);
        if (!allocatedSlot.empty()) {
            totalCost = calculateCost(allocatedZone, durationHours, true, vehicle);
            
            // Update request
            return request->allocate(allocatedZone, allocatedSlot, totalCost, true);
        }
    }
    
    // Step 3: No zones available
    return false;
}

// Find nearest zone with available slots
std::string AllocationEngine::findNearestAvailableZone(
    const std::string& preferredZone,
    std::vector<std::string>& path
) {
    std::string nearestZone;
    int minDistance = std::numeric_limits<int>::max();
    
    // Check all zones
    for (int i = 0; i < zoneCount; i++) {
        std::string zoneId = zones[i]->getZoneId();
        
        // Skip preferred zone (already checked and full)
        if (zoneId == preferredZone) continue;
        
        // Check if zone has available slots
        if (zones[i]->getAvailableSlots() > 0) {
            // Find shortest path to this zone
            std::vector<std::string> currentPath = 
                pathFinder->findShortestPath(
                    zones[0], // Start from first zone (simplified)
                    zones[i], 
                    zones, 
                    zoneCount
                );
            
            // Calculate total distance
            int distance = pathFinder->calculateDistance(currentPath, zones, zoneCount);
            
            // Update nearest zone if this one is closer
            if (distance < minDistance && distance > 0) {
                minDistance = distance;
                nearestZone = zoneId;
                path = currentPath;
            }
        }
    }
    
    return nearestZone;
}

// Get total available slots
int AllocationEngine::getTotalAvailableSlots() const {
    int total = 0;
    for (int i = 0; i < zoneCount; i++) {
        total += zones[i]->getAvailableSlots();
    }
    return total;
}

// Get total capacity
int AllocationEngine::getTotalCapacity() const {
    int total = 0;
    for (int i = 0; i < zoneCount; i++) {
        total += zones[i]->getTotalSlots();
    }
    return total;
}

// Get overall utilization
double AllocationEngine::getOverallUtilization() const {
    int totalCapacity = getTotalCapacity();
    if (totalCapacity == 0) return 0.0;
    
    int usedSlots = totalCapacity - getTotalAvailableSlots();
    return (static_cast<double>(usedSlots) / totalCapacity) * 100.0;
}

// Display zone status
void AllocationEngine::displayZoneStatus() const {
    std::cout << "\n=== ALLOCATION ENGINE - ZONE STATUS ===" << std::endl;
    std::cout << "Total Zones: " << zoneCount << std::endl;
    std::cout << "Total Capacity: " << getTotalCapacity() << " slots" << std::endl;
    std::cout << "Available Slots: " << getTotalAvailableSlots() << std::endl;
    std::cout << "Overall Utilization: " << std::fixed << std::setprecision(1) 
              << getOverallUtilization() << "%" << std::endl;
    
    for (int i = 0; i < zoneCount; i++) {
        std::cout << zones[i]->getZoneId() << ": " 
                  << zones[i]->getAvailableSlots() << "/" 
                  << zones[i]->getTotalSlots() << " available ("
                  << std::fixed << std::setprecision(1) 
                  << zones[i]->getUtilizationRate() << "%)" << std::endl;
    }
}

