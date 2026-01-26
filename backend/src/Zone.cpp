#include "Zone.h"
#include <iostream>
#include <sstream>

// ZoneConnection constructor
ZoneConnection::ZoneConnection(const std::string& zoneId, int dist, double penalty)
    : connectedZoneId(zoneId), distance(dist), penaltyMultiplier(penalty), next(nullptr) {}

// Zone constructor
Zone::Zone(const std::string& id, const std::string& name, int slots, double rate)
    : zoneId(id), zoneName(name), totalSlots(slots), availableSlots(slots), 
      hourlyRate(rate), areaList(nullptr), areaCount(0), connections(nullptr) {}

// Destructor
Zone::~Zone() {
    // Clean up connections
    ZoneConnection* conn = connections;
    while (conn != nullptr) {
        ZoneConnection* next = conn->next;
        delete conn;
        conn = next;
    }
    
    // Note: ParkingArea cleanup will be handled by ParkingSystem
}

// Add connection to another zone (Graph edge)
void Zone::addConnection(ZoneConnection* connection) {
    if (connections == nullptr) {
        connections = connection;
    } else {
        ZoneConnection* current = connections;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = connection;
    }
    std::cout << "Zone " << zoneId << " connected to " << connection->connectedZoneId 
              << " (distance: " << connection->distance << "m)" << std::endl;
}

// Get connections list
ZoneConnection* Zone::getConnections() const {
    return connections;
}

// Check if connected to specific zone
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

// Add parking area to zone
bool Zone::addArea(ParkingArea* area) {
    // Simple implementation - in real system would use linked list
    areaCount++;
    std::cout << "Added area " << area->getAreaId() << " to zone " << zoneId << std::endl;
    return true;
}

// Find area with available slot
ParkingArea* Zone::findAreaWithAvailableSlot() const {
    // Simplified - returns first area with availability
    // In full system, would iterate through areaList
    return nullptr; // Will be implemented in ParkingSystem
}

// Find available slot in zone
ParkingSlot* Zone::findAvailableSlot() const {
    // Simplified - will be implemented in ParkingSystem
    return nullptr;
}

// Allocate slot in zone
bool Zone::allocateSlot() {
    if (availableSlots > 0) {
        availableSlots--;
        return true;
    }
    return false;
}

// Release slot in zone
bool Zone::releaseSlot() {
    if (availableSlots < totalSlots) {
        availableSlots++;
        return true;
    }
    return false;
}

// Getters
std::string Zone::getZoneId() const {
    return zoneId;
}

std::string Zone::getZoneName() const {
    return zoneName;
}

int Zone::getAvailableSlots() const {
    return availableSlots;
}

int Zone::getTotalSlots() const {
    return totalSlots;
}

double Zone::getHourlyRate() const {
    return hourlyRate;
}

double Zone::getUtilizationRate() const {
    if (totalSlots == 0) return 0.0;
    return ((totalSlots - availableSlots) * 100.0) / totalSlots;
}

// Convert to string
std::string Zone::toString() const {
    std::stringstream ss;
    ss << "Zone: " << zoneName << " (" << zoneId << ")"
       << " | Slots: " << availableSlots << "/" << totalSlots
       << " | Rate: $" << hourlyRate << "/hr"
       << " | Utilization: " << getUtilizationRate() << "%";
    return ss.str();
}

// Get connections as string
std::string Zone::getConnectionsString() const {
    std::stringstream ss;
    ss << "Connections from " << zoneId << ": ";
    
    ZoneConnection* current = connections;
    if (current == nullptr) {
        ss << "None";
    } else {
        while (current != nullptr) {
            ss << current->connectedZoneId << "(" << current->distance << "m)";
            if (current->next != nullptr) {
                ss << ", ";
            }
            current = current->next;
        }
    }
    return ss.str();
}