#include "AllocationEngine.h"
#include <iostream>
#include <sstream>

// Constructor
AllocationEngine::AllocationEngine(Zone** zoneArray, int count)
    : zones(zoneArray), zoneCount(count) {
    pathFinder = new PathFinder(count * 2);
    std::cout << "Allocation Engine initialized with " << count << " zones" << std::endl;
}

// Destructor
AllocationEngine::~AllocationEngine() {
    delete pathFinder;
}

// Find available slot in specific zone
std::string AllocationEngine::findAvailableSlotInZone(const std::string& zoneId) {
    // Simplified - in full system would search through areas and slots
    // For now, generate slot ID based on zone
    static int slotCounter = 0;
    slotCounter++;
    
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i]->getZoneId() == zoneId) {
            if (zones[i]->getAvailableSlots() > 0) {
                std::stringstream ss;
                ss << zoneId << "-" << slotCounter;
                return ss.str();
            }
        }
    }
    return "";
}

// Calculate cost based on various factors
double AllocationEngine::calculateCost(
    const std::string& zoneId,
    int hours,
    bool crossZone,
    Vehicle* vehicle
) const {
    // Find zone rate
    double baseRate = 5.0; // Default
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i]->getZoneId() == zoneId) {
            baseRate = zones[i]->getHourlyRate();
            break;
        }
    }
    
    // Calculate base cost
    double cost = baseRate * hours;
    
    // Apply cross-zone penalty
    if (crossZone) {
        cost *= 1.5; // 50% penalty for cross-zone
    }
    
    // Apply vehicle type multiplier
    if (vehicle) {
        cost *= vehicle->getTypeMultiplier();
    }
    
    // Time-based pricing: first hour at full rate, additional hours at 80%
    if (hours > 1) {
        cost = baseRate + (baseRate * 0.8 * (hours - 1));
        if (crossZone) cost *= 1.5;
        if (vehicle) cost *= vehicle->getTypeMultiplier();
    }
    
    return cost;
}

// Main allocation method
bool AllocationEngine::allocateParking(
    ParkingRequest* request,
    Vehicle* vehicle,
    int durationHours,
    std::string& allocatedZone,
    std::string& allocatedSlot,
    std::vector<std::string>& optimalPath,
    double& totalCost
) {
    if (!request || !vehicle) {
        std::cout << "Invalid request or vehicle" << std::endl;
        return false;
    }
    
    std::string preferredZone = request->getPreferredZone();
    bool crossZone = false;
    
    // Try preferred zone first
    allocatedSlot = findAvailableSlotInZone(preferredZone);
    if (!allocatedSlot.empty()) {
        allocatedZone = preferredZone;
        optimalPath.clear();
        optimalPath.push_back(preferredZone);
    } else {
        // Preferred zone full, find nearest available zone
        std::cout << "Zone " << preferredZone << " is full. Finding nearest available zone..." << std::endl;
        allocatedZone = findNearestAvailableZone(preferredZone, optimalPath);
        crossZone = true;
        
        if (allocatedZone.empty()) {
            std::cout << "No available parking found in any zone!" << std::endl;
            return false;
        }
        
        allocatedSlot = findAvailableSlotInZone(allocatedZone);
        if (allocatedSlot.empty()) {
            std::cout << "Error: Could not allocate slot in zone " << allocatedZone << std::endl;
            return false;
        }
    }
    
    // Calculate cost
    totalCost = calculateCost(allocatedZone, durationHours, crossZone, vehicle);
    
    // Allocate slot in zone
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i]->getZoneId() == allocatedZone) {
            zones[i]->allocateSlot();
            break;
        }
    }
    
    // Update request
    if (!request->allocate(allocatedZone, allocatedSlot, totalCost, crossZone)) {
        std::cout << "Failed to update request state" << std::endl;
        return false;
    }
    
    std::cout << "✅ Successfully allocated parking!" << std::endl;
    std::cout << "   Zone: " << allocatedZone << std::endl;
    std::cout << "   Slot: " << allocatedSlot << std::endl;
    std::cout << "   Duration: " << durationHours << " hours" << std::endl;
    std::cout << "   Cost: $" << totalCost << std::endl;
    if (crossZone) {
        std::cout << "   Note: Cross-zone allocation (50% penalty)" << std::endl;
    }
    
    return true;
}

// Find nearest available zone
std::string AllocationEngine::findNearestAvailableZone(
    const std::string& preferredZone,
    std::vector<std::string>& path
) {
    return pathFinder->findNearestAvailableZone(preferredZone, zones, zoneCount, path);
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
    int totalSlots = getTotalCapacity();
    if (totalSlots == 0) return 0.0;
    
    int availableSlots = getTotalAvailableSlots();
    return ((totalSlots - availableSlots) * 100.0) / totalSlots;
}

// Display zone status
void AllocationEngine::displayZoneStatus() const {
    std::cout << "\n=== Parking Zone Status ===" << std::endl;
    std::cout << "Total Capacity: " << getTotalCapacity() << " slots" << std::endl;
    std::cout << "Available: " << getTotalAvailableSlots() << " slots" << std::endl;
    std::cout << "Overall Utilization: " << getOverallUtilization() << "%" << std::endl;
    
    std::cout << "\nZone Details:" << std::endl;
    for (int i = 0; i < zoneCount; i++) {
        std::cout << "  " << zones[i]->toString() << std::endl;
    }
    std::cout << "==========================\n" << std::endl;
}