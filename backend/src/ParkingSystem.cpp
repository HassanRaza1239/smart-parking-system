#include "ParkingSystem.h"
#include <iostream>
#include <sstream>
#include <ctime>

// Constructor
ParkingSystem::ParkingSystem() : zones(nullptr), zoneCount(0), 
                                 allocator(nullptr), rollbackManager(nullptr), 
                                 analytics(nullptr) {
    std::cout << "\n=========================================" << std::endl;
    std::cout << "   NexusPark Smart Parking System" << std::endl;
    std::cout << "=========================================\n" << std::endl;
    
    // Initialize with 5 zones as specified in requirements
    initializeZones();
    
    // Create allocation engine
    allocator = new AllocationEngine(zones, zoneCount);
    
    // Create rollback manager (10-step undo)
    rollbackManager = new RollbackManager(10);
    
    // Analytics will be created when needed
    std::cout << "✅ System initialized with " << zoneCount << " zones" << std::endl;
}

// Destructor
ParkingSystem::~ParkingSystem() {
    // Cleanup zones
    for (int i = 0; i < zoneCount; i++) {
        delete zones[i];
    }
    delete[] zones;
    
    // Cleanup vehicles
    for (Vehicle* vehicle : vehicles) {
        delete vehicle;
    }
    
    // Cleanup requests
    for (ParkingRequest* request : requests) {
        delete request;
    }
    
    // Cleanup engines
    delete allocator;
    delete rollbackManager;
    delete analytics;
    
    std::cout << "\nParking system shutdown complete." << std::endl;
}

// Initialize with 5 zones as per requirements
void ParkingSystem::initializeZones() {
    zoneCount = 5;
    zones = new Zone*[zoneCount];
    
    // Create 5 zones with different characteristics
    zones[0] = new Zone("ZA", "Zone A - City Center", 10, 5.0);
    zones[1] = new Zone("ZB", "Zone B - Shopping District", 8, 6.0);
    zones[2] = new Zone("ZC", "Zone C - Business Park", 12, 4.5);
    zones[3] = new Zone("ZD", "Zone D - University Area", 15, 3.5);
    zones[4] = new Zone("ZE", "Zone E - Riverside", 6, 7.0);
    
    // Create zone connections (graph)
    zones[0]->addConnection(new ZoneConnection("ZB", 500));
    zones[0]->addConnection(new ZoneConnection("ZC", 800));
    
    zones[1]->addConnection(new ZoneConnection("ZA", 500));
    zones[1]->addConnection(new ZoneConnection("ZC", 300));
    zones[1]->addConnection(new ZoneConnection("ZD", 700));
    
    zones[2]->addConnection(new ZoneConnection("ZA", 800));
    zones[2]->addConnection(new ZoneConnection("ZB", 300));
    zones[2]->addConnection(new ZoneConnection("ZE", 400));
    
    zones[3]->addConnection(new ZoneConnection("ZB", 700));
    zones[3]->addConnection(new ZoneConnection("ZE", 600));
    
    zones[4]->addConnection(new ZoneConnection("ZC", 400));
    zones[4]->addConnection(new ZoneConnection("ZD", 600));
}

// Add a zone
void ParkingSystem::addZone(Zone* zone) {
    // Implementation would resize array
    std::cout << "Zone " << zone->getZoneId() << " added to system" << std::endl;
}

// Add a vehicle
void ParkingSystem::addVehicle(Vehicle* vehicle) {
    vehicles.push_back(vehicle);
    std::cout << "Vehicle " << vehicle->getVehicleId() << " registered" << std::endl;
}

// Find vehicle by ID
Vehicle* ParkingSystem::findVehicle(const std::string& vehicleId) const {
    for (Vehicle* vehicle : vehicles) {
        if (vehicle->getVehicleId() == vehicleId) {
            return vehicle;
        }
    }
    return nullptr;
}

// Find request by ID
ParkingRequest* ParkingSystem::findRequest(const std::string& requestId) const {
    for (ParkingRequest* request : requests) {
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

// Request parking
std::string ParkingSystem::requestParking(const std::string& vehicleId, 
                                         const std::string& preferredZone,
                                         int durationHours) {
    std::cout << "\n=== Processing Parking Request ===" << std::endl;
    
    // Check if vehicle exists
    Vehicle* vehicle = findVehicle(vehicleId);
    if (!vehicle) {
        // Create new vehicle if not found
        vehicle = new Vehicle(vehicleId, preferredZone, CAR);
        vehicles.push_back(vehicle);
        std::cout << "New vehicle registered: " << vehicle->toString() << std::endl;
    }
    
    // Generate request ID
    std::stringstream ss;
    ss << "REQ" << (requests.size() + 1);
    std::string requestId = ss.str();
    
    // Create parking request
    ParkingRequest* request = new ParkingRequest(requestId, vehicleId, preferredZone);
    requests.push_back(request);
    
    // Allocate parking
    std::string allocatedZone, allocatedSlot;
    std::vector<std::string> optimalPath;
    double totalCost;
    
    if (allocator->allocateParking(request, vehicle, durationHours, 
                                   allocatedZone, allocatedSlot, 
                                   optimalPath, totalCost)) {
        // Record operation for rollback
        rollbackManager->pushOperation(new RollbackOperation(
            OP_ALLOCATE, requestId, allocatedZone, allocatedSlot, REQUESTED
        ));
        
        std::cout << "✅ Request " << requestId << " processed successfully!" << std::endl;
        return requestId;
    } else {
        std::cout << "❌ Failed to allocate parking" << std::endl;
        // Remove failed request
        requests.pop_back();
        delete request;
        return "";
    }
}

// Cancel parking
bool ParkingSystem::cancelParking(const std::string& requestId) {
    ParkingRequest* request = findRequest(requestId);
    if (!request) {
        std::cout << "Request " << requestId << " not found!" << std::endl;
        return false;
    }
    
    // Record current state for rollback
    RequestState currentState = request->getState();
    std::string allocatedZone = request->getAllocatedZone();
    std::string slotId = request->getSlotId();
    
    if (request->cancel()) {
        // Record operation for rollback
        rollbackManager->pushOperation(new RollbackOperation(
            OP_CANCEL, requestId, allocatedZone, slotId, currentState
        ));
        
        // Release slot if it was allocated
        if (!allocatedZone.empty()) {
            Zone* zone = findZone(allocatedZone);
            if (zone) {
                zone->releaseSlot();
            }
        }
        
        std::cout << "✅ Request " << requestId << " cancelled" << std::endl;
        return true;
    }
    
    return false;
}

// Occupy parking
bool ParkingSystem::occupyParking(const std::string& requestId) {
    ParkingRequest* request = findRequest(requestId);
    if (!request) {
        std::cout << "Request " << requestId << " not found!" << std::endl;
        return false;
    }
    
    return request->occupy();
}

// Release parking
bool ParkingSystem::releaseParking(const std::string& requestId) {
    ParkingRequest* request = findRequest(requestId);
    if (!request) {
        std::cout << "Request " << requestId << " not found!" << std::endl;
        return false;
    }
    
    // Record current state for rollback
    RequestState currentState = request->getState();
    std::string allocatedZone = request->getAllocatedZone();
    std::string slotId = request->getSlotId();
    
    if (request->release()) {
        // Record operation for rollback
        rollbackManager->pushOperation(new RollbackOperation(
            OP_RELEASE, requestId, allocatedZone, slotId, currentState
        ));
        
        std::cout << "✅ Parking released for request " << requestId << std::endl;
        return true;
    }
    
    return false;
}

// Undo last operation
bool ParkingSystem::undoLastOperation() {
    // Create arrays for rollback manager
    ParkingRequest** requestArray = new ParkingRequest*[requests.size()];
    for (size_t i = 0; i < requests.size(); i++) {
        requestArray[i] = requests[i];
    }
    
    bool result = rollbackManager->undoLastOperation(requestArray, requests.size(), zones, zoneCount);
    delete[] requestArray;
    
    if (result) {
        std::cout << "✅ Last operation undone" << std::endl;
    }
    
    return result;
}

// Undo multiple operations
bool ParkingSystem::undoOperations(int steps) {
    // Create arrays for rollback manager
    ParkingRequest** requestArray = new ParkingRequest*[requests.size()];
    for (size_t i = 0; i < requests.size(); i++) {
        requestArray[i] = requests[i];
    }
    
    bool result = rollbackManager->undoMultipleOperations(steps, requestArray, requests.size(), zones, zoneCount);
    delete[] requestArray;
    
    if (result) {
        std::cout << "✅ " << steps << " operations undone" << std::endl;
    }
    
    return result;
}

// Display analytics
void ParkingSystem::displayAnalytics() const {
    if (requests.empty()) {
        std::cout << "No data for analytics yet." << std::endl;
        return;
    }
    
    // Create analytics engine
    ParkingRequest** requestArray = new ParkingRequest*[requests.size()];
    for (size_t i = 0; i < requests.size(); i++) {
        requestArray[i] = requests[i];
    }
    
    Analytics tempAnalytics(requestArray, requests.size(), zones, zoneCount);
    tempAnalytics.displayDetailedReport();
    
    delete[] requestArray;
}

// Display zone status
void ParkingSystem::displayZoneStatus() const {
    allocator->displayZoneStatus();
}

// Display all requests
void ParkingSystem::displayAllRequests() const {
    std::cout << "\n=== All Parking Requests ===" << std::endl;
    if (requests.empty()) {
        std::cout << "No requests yet." << std::endl;
        return;
    }
    
    for (size_t i = 0; i < requests.size(); i++) {
        std::cout << i + 1 << ". " << requests[i]->toString() << std::endl;
    }
    std::cout << "===========================\n" << std::endl;
}

// Get total available slots
int ParkingSystem::getTotalAvailableSlots() const {
    return allocator->getTotalAvailableSlots();
}

// Get total capacity
int ParkingSystem::getTotalCapacity() const {
    return allocator->getTotalCapacity();
}

// Get overall utilization
double ParkingSystem::getOverallUtilization() const {
    return allocator->getOverallUtilization();
}

// Run demo
void ParkingSystem::runDemo() {
    std::cout << "\n=== Running System Demo ===\n" << std::endl;
    
    // Add some vehicles
    addVehicle(new Vehicle("CAR001", "ZA", CAR));
    addVehicle(new Vehicle("BIKE001", "ZB", BIKE));
    addVehicle(new Vehicle("TRUCK001", "ZC", TRUCK));
    
    // Display initial status
    displayZoneStatus();
    
    // Make some requests
    std::cout << "Making parking requests..." << std::endl;
    std::string req1 = requestParking("CAR001", "ZA", 3);
    std::string req2 = requestParking("BIKE001", "ZB", 2);
    std::string req3 = requestParking("TRUCK001", "ZC", 5);
    
    // Display status
    displayZoneStatus();
    displayAllRequests();
    
    // Occupy and release
    std::cout << "\nSimulating parking usage..." << std::endl;
    occupyParking(req1);
    occupyParking(req2);
    releaseParking(req1);
    releaseParking(req2);
    
    // Test cancellation
    std::cout << "\nTesting cancellation..." << std::endl;
    std::string req4 = requestParking("CAR001", "ZA", 4);
    cancelParking(req4);
    
    // Test cross-zone allocation (fill Zone A)
    std::cout << "\nTesting cross-zone allocation..." << std::endl;
    for (int i = 0; i < 7; i++) {
        std::stringstream ss;
        ss << "CAR" << (100 + i);
        requestParking(ss.str(), "ZA", 1);
    }
    
    // Now Zone A should be full, next request should go to nearest zone
    std::string req5 = requestParking("CAR999", "ZA", 2);
    
    // Display analytics
    std::cout << "\nGenerating analytics report..." << std::endl;
    displayAnalytics();
    
    // Test rollback
    std::cout << "\nTesting rollback system..." << std::endl;
    std::cout << "Before rollback:" << std::endl;
    displayZoneStatus();
    
    undoLastOperation();
    
    std::cout << "After rollback:" << std::endl;
    displayZoneStatus();
    
    std::cout << "\n=== Demo Complete ===\n" << std::endl;
}