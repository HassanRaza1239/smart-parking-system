#include "../include/Zone.h"
#include "../include/ParkingArea.h"
#include <iostream>
#include <sstream>

// ZoneConnection constructor
ZoneConnection::ZoneConnection(const std::string& zoneId, int dist, double penalty) 
    : connectedZoneId(zoneId), distance(dist), penaltyMultiplier(penalty), next(nullptr) {}

// Zone constructor
Zone::Zone(const std::string& id, const std::string& name, int slots, double rate)
    : zoneId(id), zoneName(name), totalSlots(slots), availableSlots(slots), 
      hourlyRate(rate), areaList(nullptr), areaCount(0), connections(nullptr) {}

// Zone destructor
Zone::~Zone() {
    // Delete all connections
    ZoneConnection* current = connections;
    while (current != nullptr) {
        ZoneConnection* next = current->next;
        delete current;
        current = next;
    }
    
    // Delete all areas
    ParkingArea* currentArea = areaList;
    while (currentArea != nullptr) {
        ParkingArea* nextArea = nullptr; // We need to get next before deleting
        delete currentArea;
        currentArea = nextArea;
    }
}

// Add connection to another zone
void Zone::addConnection(ZoneConnection* connection) {
    if (!connection) return;
    
    connection->next = connections;
    connections = connection;
}

// Get connections list
ZoneConnection* Zone::getConnections() const {
    return connections;
}

// Check if connected to a zone
bool Zone::isConnectedTo(const std::string& zoneId) const {
    ZoneConnection* current = connections;
    while (current != nullptr) {
        if (current->connectedZoneId == zoneId) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Get distance to connected zone
int Zone::getDistanceTo(const std::string& zoneId) const {
    ZoneConnection* current = connections;
    while (current != nullptr) {
        if (current->connectedZoneId == zoneId) {
            return current->distance;
        }
        current = current->next;
    }
    return -1; // Not connected
}

// Add parking area
bool Zone::addArea(ParkingArea* area) {
    if (!area) return false;
    
    // Simple implementation - just add to list
    // In real implementation, you might want to manage areas differently
    areaCount++;
    return true;
}

// Find area with available slot
ParkingArea* Zone::findAreaWithAvailableSlot() const {
    // For now, return nullptr - will be implemented when ParkingArea is complete
    return nullptr;
}

// Find available slot in this zone
ParkingSlot* Zone::findAvailableSlot() const {
    // For now, return nullptr - will be implemented when ParkingArea is complete
    return nullptr;
}

// Allocate a slot in this zone
bool Zone::allocateSlot() {
    if (availableSlots > 0) {
        availableSlots--;
        return true;
    }
    return false;
}

// Release a slot in this zone
bool Zone::releaseSlot() {
    if (availableSlots < totalSlots) {
        availableSlots++;
        return true;
    }
    return false;
}

// Get zone ID
std::string Zone::getZoneId() const {
    return zoneId;
}

// Get zone name
std::string Zone::getZoneName() const {
    return zoneName;
}

// Get available slots
int Zone::getAvailableSlots() const {
    return availableSlots;
}

// Get total slots
int Zone::getTotalSlots() const {
    return totalSlots;
}

// Get hourly rate
double Zone::getHourlyRate() const {
    return hourlyRate;
}

// Get utilization rate
double Zone::getUtilizationRate() const {
    if (totalSlots == 0) return 0.0;
    double usedSlots = totalSlots - availableSlots;
    return (usedSlots / totalSlots) * 100.0;
}

// Convert to string
std::string Zone::toString() const {
    std::stringstream ss;
    ss << "Zone " << zoneId << ": " << zoneName 
       << " (" << availableSlots << "/" << totalSlots << " available)"
       << " Rate: $" << hourlyRate << "/hr"
       << " Utilization: " << getUtilizationRate() << "%";
    return ss.str();
}

// Get connections as string
std::string Zone::getConnectionsString() const {
    std::stringstream ss;
    ss << "Connections from " << zoneId << ": ";
    
    ZoneConnection* current = connections;
    bool first = true;
    while (current != nullptr) {
        if (!first) ss << ", ";
        ss << current->connectedZoneId << " (" << current->distance << "m)";
        first = false;
        current = current->next;
    }
    
    if (first) {
        ss << "None";
    }
    
    return ss.str();
}
