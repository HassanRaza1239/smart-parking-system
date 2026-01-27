#ifndef PARKINGSYSTEM_H
#define PARKINGSYSTEM_H

#include "Zone.h"
#include "Vehicle.h"
#include "ParkingRequest.h"
#include "AllocationEngine.h"
#include "RollbackManager.h"
#include "Analytics.h"
#include <vector>
#include <string>

class ParkingSystem {
private:
    void updateAnalytics();
    
    Zone** zones;
    int zoneCount;
    std::vector<Vehicle*> vehicles;
    std::vector<ParkingRequest*> requests;
    
    AllocationEngine* allocator;
    RollbackManager* rollbackManager;
    Analytics* analytics;
    
    // Helper methods
    Vehicle* findVehicle(const std::string& vehicleId) const;
    ParkingRequest* findRequest(const std::string& requestId) const;
    Zone* findZone(const std::string& zoneId) const;
    
public:
    ParkingSystem();
    ~ParkingSystem();
    
    // Setup methods
    void initializeZones();
    void addZone(Zone* zone);
    void addVehicle(Vehicle* vehicle);
    
    // Core operations
    std::string requestParking(const std::string& vehicleId, 
                              const std::string& preferredZone,
                              int durationHours);
    
    bool cancelParking(const std::string& requestId);
    bool occupyParking(const std::string& requestId);
    bool releaseParking(const std::string& requestId);
    
    // Rollback operations
    bool undoLastOperation();
    bool undoOperations(int steps);
    
    // Analytics
    void displayAnalytics() const;
    void displayZoneStatus() const;
    void displayAllRequests() const;
    
    // Utility
    int getTotalAvailableSlots() const;
    int getTotalCapacity() const;
    double getOverallUtilization() const;
    
    // Testing
    void runDemo();
};

#endif
