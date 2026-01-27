#include "../include/ParkingSystem.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <random>

using namespace std;

// Constructor
ParkingSystem::ParkingSystem() : zoneCount(0), zones(nullptr) {
    allocator = nullptr;
    rollbackManager = new RollbackManager(10);
    analytics = nullptr;
    
    // Initialize random number generator for demo
    srand(time(nullptr));
}

// Destructor
ParkingSystem::~ParkingSystem() {
    // Delete all zones
    for (int i = 0; i < zoneCount; i++) {
        delete zones[i];
    }
    delete[] zones;
    
    // Delete all vehicles
    for (auto vehicle : vehicles) {
        delete vehicle;
    }
    
    // Delete all requests
    for (auto request : requests) {
        delete request;
    }
    
    delete allocator;
    delete rollbackManager;
    delete analytics;
}

// Initialize zones with sample data
void ParkingSystem::initializeZones() {
    // Create 5 zones as per specification
    zoneCount = 5;
    zones = new Zone*[zoneCount];
    
    // Zone A - Downtown Parking
    zones[0] = new Zone("ZA", "Downtown Parking A", 20, 5.0);
    
    // Zone B - Mall Parking
    zones[1] = new Zone("ZB", "Mall Parking B", 15, 4.0);
    
    // Zone C - Office Parking
    zones[2] = new Zone("ZC", "Office Parking C", 25, 6.0);
    
    // Zone D - Residential Parking
    zones[3] = new Zone("ZD", "Residential Parking D", 18, 3.5);
    
    // Zone E - Airport Parking
    zones[4] = new Zone("ZE", "Airport Parking E", 30, 8.0);
    
    // Initialize allocation engine with zones
    allocator = new AllocationEngine(zones, zoneCount);
    
    // Create sample vehicles for demo
    addVehicle(new Vehicle("CAR001", "ZA", CAR));
    addVehicle(new Vehicle("BIKE001", "ZB", BIKE));
    addVehicle(new Vehicle("TRUCK001", "ZC", TRUCK));
    addVehicle(new Vehicle("CAR002", "ZA", CAR));
    addVehicle(new Vehicle("BIKE002", "ZD", BIKE));
    
    // Initialize analytics
    updateAnalytics();
    
    cout << "✅ Parking system initialized with " << zoneCount << " zones." << endl;
}

// Add a zone to the system
void ParkingSystem::addZone(Zone* zone) {
    // Implementation will be added
}

// Add a vehicle to the system
void ParkingSystem::addVehicle(Vehicle* vehicle) {
    vehicles.push_back(vehicle);
}

// Find vehicle by ID
Vehicle* ParkingSystem::findVehicle(const std::string& vehicleId) const {
    for (auto vehicle : vehicles) {
        if (vehicle->getVehicleId() == vehicleId) {
            return vehicle;
        }
    }
    return nullptr;
}

// Find request by ID
ParkingRequest* ParkingSystem::findRequest(const std::string& requestId) const {
    for (auto request : requests) {
        if (request->getRequestId() == requestId) {
            return request;
        }
    }
    return nullptr;
}

// Find zone by ID
Zone* ParkingSystem::findZone(const std::string& zoneId) const {
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i]->getZoneId() == zoneId) {
            return zones[i];
        }
    }
    return nullptr;
}

// Update analytics with current data
void ParkingSystem::updateAnalytics() {
    // Stub implementation - will be completed when Analytics.cpp is created
}

// Main function to request parking
std::string ParkingSystem::requestParking(const std::string& vehicleId, 
                                         const std::string& preferredZone,
                                         int durationHours) {
    // Stub implementation
    return "";
}

// Cancel a parking request
bool ParkingSystem::cancelParking(const std::string& requestId) {
    // Stub implementation
    return false;
}

// Occupy a parking slot (user arrives)
bool ParkingSystem::occupyParking(const std::string& requestId) {
    // Stub implementation
    return false;
}

// Release a parking slot (user leaves)
bool ParkingSystem::releaseParking(const std::string& requestId) {
    // Stub implementation
    return false;
}

// Undo last operation
bool ParkingSystem::undoLastOperation() {
    // Stub implementation
    return false;
}

// Undo multiple operations
bool ParkingSystem::undoOperations(int steps) {
    // Stub implementation
    return false;
}

// Display analytics
void ParkingSystem::displayAnalytics() const {
    cout << "Analytics display - to be implemented" << endl;
}

// Display zone status
void ParkingSystem::displayZoneStatus() const {
    cout << "Zone status display - to be implemented" << endl;
}

// Display all requests
void ParkingSystem::displayAllRequests() const {
    cout << "All requests display - to be implemented" << endl;
}

// Get total available slots
int ParkingSystem::getTotalAvailableSlots() const {
    return 0;
}

// Get total capacity
int ParkingSystem::getTotalCapacity() const {
    return 0;
}

// Get overall utilization
double ParkingSystem::getOverallUtilization() const {
    return 0.0;
}

// Run a complete demo
void ParkingSystem::runDemo() {
    cout << "Demo mode - to be implemented" << endl;
}
