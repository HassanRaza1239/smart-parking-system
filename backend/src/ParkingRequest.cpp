#include "../include/ParkingRequest.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// ParkingRequest constructor
ParkingRequest::ParkingRequest(const std::string& reqId, 
                               const std::string& vehicle, 
                               const std::string& zone)
    : requestId(reqId), vehicleId(vehicle), preferredZone(zone),
      allocatedZone(""), slotId(""), currentState(REQUESTED),
      durationHours(0), totalCost(0.0), isCrossZone(false) {
    requestTime = time(nullptr);
    allocationTime = 0;
    completionTime = 0;
}

// Allocate a parking slot
bool ParkingRequest::allocate(const std::string& zone, const std::string& slot, 
                              double cost, bool crossZone) {
    if (!isValidTransition(ALLOCATED)) {
        std::cout << "❌ Invalid transition to ALLOCATED from " 
                  << getStateString() << std::endl;
        return false;
    }
    
    allocatedZone = zone;
    slotId = slot;
    totalCost = cost;
    isCrossZone = crossZone;
    currentState = ALLOCATED;
    allocationTime = time(nullptr);
    
    return true;
}

// Occupy the parking slot
bool ParkingRequest::occupy() {
    if (!isValidTransition(OCCUPIED)) {
        std::cout << "❌ Invalid transition to OCCUPIED from " 
                  << getStateString() << std::endl;
        return false;
    }
    
    currentState = OCCUPIED;
    return true;
}

// Release the parking slot
bool ParkingRequest::release() {
    if (!isValidTransition(RELEASED)) {
        std::cout << "❌ Invalid transition to RELEASED from " 
                  << getStateString() << std::endl;
        return false;
    }
    
    currentState = RELEASED;
    completionTime = time(nullptr);
    return true;
}

// Cancel the request
bool ParkingRequest::cancel() {
    if (!isValidTransition(CANCELLED)) {
        std::cout << "❌ Invalid transition to CANCELLED from " 
                  << getStateString() << std::endl;
        return false;
    }
    
    currentState = CANCELLED;
    completionTime = time(nullptr);
    return true;
}

// Check if transition is valid
bool ParkingRequest::isValidTransition(RequestState newState) const {
    switch (currentState) {
        case REQUESTED:
            return newState == ALLOCATED || newState == CANCELLED;
            
        case ALLOCATED:
            return newState == OCCUPIED || newState == CANCELLED;
            
        case OCCUPIED:
            return newState == RELEASED;
            
        case RELEASED:
        case CANCELLED:
            return false; // Final states
            
        default:
            return false;
    }
}

// Get request ID
std::string ParkingRequest::getRequestId() const {
    return requestId;
}

// Get vehicle ID
std::string ParkingRequest::getVehicleId() const {
    return vehicleId;
}

// Get preferred zone
std::string ParkingRequest::getPreferredZone() const {
    return preferredZone;
}

// Get allocated zone
std::string ParkingRequest::getAllocatedZone() const {
    return allocatedZone;
}

// Get slot ID
std::string ParkingRequest::getSlotId() const {
    return slotId;
}

// Get current state
RequestState ParkingRequest::getState() const {
    return currentState;
}

// Get state as string
std::string ParkingRequest::getStateString() const {
    switch (currentState) {
        case REQUESTED: return "REQUESTED";
        case ALLOCATED: return "ALLOCATED";
        case OCCUPIED: return "OCCUPIED";
        case RELEASED: return "RELEASED";
        case CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}

// Get request time
time_t ParkingRequest::getRequestTime() const {
    return requestTime;
}

// Get allocation time
time_t ParkingRequest::getAllocationTime() const {
    return allocationTime;
}

// Get completion time
time_t ParkingRequest::getCompletionTime() const {
    return completionTime;
}

// Get duration in hours
double ParkingRequest::getDuration() const {
    return durationHours;
}

// Get total cost
double ParkingRequest::getTotalCost() const {
    return totalCost;
}

// Check if cross-zone allocation
bool ParkingRequest::getIsCrossZone() const {
    return isCrossZone;
}

// Convert to string
std::string ParkingRequest::toString() const {
    std::stringstream ss;
    ss << "Request " << requestId 
       << " - Vehicle: " << vehicleId
       << " | State: " << getStateString()
       << " | Zone: " << (allocatedZone.empty() ? "None" : allocatedZone)
       << " | Cost: $" << std::fixed << std::setprecision(2) << totalCost;
    return ss.str();
}

// Get detailed info
std::string ParkingRequest::getDetailedInfo() const {
    std::stringstream ss;
    ss << "=== Parking Request Details ===" << std::endl
       << "Request ID: " << requestId << std::endl
       << "Vehicle ID: " << vehicleId << std::endl
       << "Preferred Zone: " << preferredZone << std::endl
       << "Allocated Zone: " << (allocatedZone.empty() ? "None" : allocatedZone) << std::endl
       << "Slot ID: " << (slotId.empty() ? "None" : slotId) << std::endl
       << "State: " << getStateString() << std::endl
       << "Cross Zone: " << (isCrossZone ? "Yes" : "No") << std::endl
       << "Duration: " << durationHours << " hours" << std::endl
       << "Total Cost: $" << std::fixed << std::setprecision(2) << totalCost << std::endl;
    
    return ss.str();
}
