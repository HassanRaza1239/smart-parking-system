#ifndef ALLOCATIONENGINE_H
#define ALLOCATIONENGINE_H

#include "Zone.h"
#include "ParkingRequest.h"
#include "PathFinder.h"
#include "Vehicle.h"
#include <string>
#include <vector>

class AllocationEngine {
private:
    Zone** zones;
    int zoneCount;
    PathFinder* pathFinder;
    
    // Find available slot in specific zone
    std::string findAvailableSlotInZone(const std::string& zoneId);
    
    // Calculate cost based on various factors
    double calculateCost(
        const std::string& zoneId,
        int hours,
        bool crossZone,
        Vehicle* vehicle
    ) const;
    
public:
    AllocationEngine(Zone** zoneArray, int count);
    ~AllocationEngine();
    
    // Main allocation method
    bool allocateParking(
        ParkingRequest* request,
        Vehicle* vehicle,
        int durationHours,
        std::string& allocatedZone,
        std::string& allocatedSlot,
        std::vector<std::string>& optimalPath,
        double& totalCost
    );
    
    // Find nearest available zone
    std::string findNearestAvailableZone(
        const std::string& preferredZone,
        std::vector<std::string>& path
    );
    
    // Getters
    int getTotalAvailableSlots() const;
    int getTotalCapacity() const;
    double getOverallUtilization() const;
    
    // Display
    void displayZoneStatus() const;
};

#endif