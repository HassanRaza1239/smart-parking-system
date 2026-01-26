#ifndef ZONE_H
#define ZONE_H

#include "ParkingArea.h"
#include <string>

// Custom adjacency list node for zone connections (Graph structure)
struct ZoneConnection {
    std::string connectedZoneId;
    int distance;  // in meters
    double penaltyMultiplier; // Cost multiplier for cross-zone
    ZoneConnection* next;
    
    ZoneConnection(const std::string& zoneId, int dist, double penalty = 1.5);
};

class Zone {
private:
    std::string zoneId;
    std::string zoneName;
    int totalSlots;
    int availableSlots;
    double hourlyRate;
    
    // Custom linked list for parking areas
    ParkingArea* areaList;
    int areaCount;
    
    // Custom adjacency list for zone connections (Graph)
    ZoneConnection* connections;
    
public:
    Zone(const std::string& id, const std::string& name, int slots, double rate);
    ~Zone();
    
    // Graph operations
    void addConnection(ZoneConnection* connection);
    ZoneConnection* getConnections() const;
    bool isConnectedTo(const std::string& zoneId) const;
    int getDistanceTo(const std::string& zoneId) const;
    
    // Area management
    bool addArea(ParkingArea* area);
    ParkingArea* findAreaWithAvailableSlot() const;
    ParkingSlot* findAvailableSlot() const;
    
    // Slot management
    bool allocateSlot();
    bool releaseSlot();
    
    // Getters
    std::string getZoneId() const;
    std::string getZoneName() const;
    int getAvailableSlots() const;
    int getTotalSlots() const;
    double getHourlyRate() const;
    double getUtilizationRate() const;
    
    // Display
    std::string toString() const;
    std::string getConnectionsString() const;
};

#endif