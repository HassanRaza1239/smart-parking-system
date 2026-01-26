#include "ParkingRequest.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// Constructor
ParkingRequest::ParkingRequest(const std::string& reqId, 
                              const std::string& vehicle, 
                              const std::string& zone)
    : requestId(reqId), vehicleId(vehicle), preferredZone(zone),
      allocatedZone(""), slotId(""), currentState(REQUESTED),
      durationHours(0), totalCost(0.0), isCrossZone(false) {
    requestTime = time(nullptr);
    std::cout << "Created parking request " << reqId 
              << " for vehicle " << vehicle << std::endl;
}

// Check if state transition is valid
bool ParkingRequest::isValidTransition(RequestState newState) const {
    switch(currentState) {
        case REQUESTED:
            return (newState == ALLOCATED || newState == CANCELLED);
        case ALLOCATED:
            return (newState == OCCUPIED || newState == CANCELLED);
        case OCCUPIED:
            return (newState == RELEASED);
        case RELEASED:
        case CANCELLED:
            return false; // Terminal states
    }
    return false;
}

// Allocate a slot to this request
bool ParkingRequest::allocate(const std::string& zone, const std::string& slot, 
                            double cost, bool crossZone) {
    if (!isValidTransition(ALLOCATED)) {
        std::cout << "Invalid transition from " << getStateString() 
                  << " to ALLOCATED" << std::endl;
        return false;
    }
    
    allocatedZone = zone;
    slotId = slot;
    totalCost = cost;
    isCrossZone = crossZone;
    currentState = ALLOCATED;
    allocationTime = time(nullptr);
    
    std::cout << "Request " << requestId << " allocated to " 
              << zone << ", slot " << slot << std::endl;
    return true;
}

// Mark as occupied
bool ParkingRequest::occupy() {
    if (!isValidTransition(OCCUPIED)) {
        std::cout << "Invalid transition from " << getStateString() 
                  << " to OCCUPIED" << std::endl;
        return false;
    }
    
    currentState = OCCUPIED;
    std::cout << "Vehicle " << vehicleId << " occupied slot " << slotId << std::endl;
    return true;
}

// Release the parking
bool ParkingRequest::release() {
    if (!isValidTransition(RELEASED)) {
        std::cout << "Invalid transition from " << getStateString() 
                  << " to RELEASED" << std::endl;
        return false;
    }
    
    currentState = RELEASED;
    completionTime = time(nullptr);
    
    // Calculate actual duration
    double duration = difftime(completionTime, allocationTime) / 3600.0;
    durationHours = static_cast<int>(duration + 0.5); // Round to nearest hour
    
    std::cout << "Request " << requestId << " released after " 
              << durationHours << " hours" << std::endl;
    return true;
}

// Cancel the request
bool ParkingRequest::cancel() {
    if (!isValidTransition(CANCELLED)) {
        std::cout << "Invalid transition from " << getStateString() 
                  << " to CANCELLED" << std::endl;
        return false;
    }
    
    currentState = CANCELLED;
    completionTime = time(nullptr);
    
    std::cout << "Request " << requestId << " cancelled" << std::endl;
    return true;
}

// Getters
std::string ParkingRequest::getRequestId() const {
    return requestId;
}

std::string ParkingRequest::getVehicleId() const {
    return vehicleId;
}

std::string ParkingRequest::getPreferredZone() const {
    return preferredZone;
}

std::string ParkingRequest::getAllocatedZone() const {
    return allocatedZone;
}

std::string ParkingRequest::getSlotId() const {
    return slotId;
}

RequestState ParkingRequest::getState() const {
    return currentState;
}

std::string ParkingRequest::getStateString() const {
    switch(currentState) {
        case REQUESTED: return "REQUESTED";
        case ALLOCATED: return "ALLOCATED";
        case OCCUPIED: return "OCCUPIED";
        case RELEASED: return "RELEASED";
        case CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}

time_t ParkingRequest::getRequestTime() const {
    return requestTime;
}

time_t ParkingRequest::getAllocationTime() const {
    return allocationTime;
}

time_t ParkingRequest::getCompletionTime() const {
    return completionTime;
}

double ParkingRequest::getDuration() const {
    if (currentState == RELEASED || currentState == CANCELLED) {
        return durationHours;
    } else if (currentState == OCCUPIED || currentState == ALLOCATED) {
        time_t now = time(nullptr);
        return difftime(now, allocationTime) / 3600.0;
    }
    return 0.0;
}

double ParkingRequest::getTotalCost() const {
    return totalCost;
}

bool ParkingRequest::getIsCrossZone() const {
    return isCrossZone;
}

// Display basic info
std::string ParkingRequest::toString() const {
    std::stringstream ss;
    ss << "Request: " << requestId 
       << " | Vehicle: " << vehicleId
       << " | State: " << getStateString()
       << " | Zone: " << (allocatedZone.empty() ? preferredZone : allocatedZone);
    
    if (!slotId.empty()) {
        ss << " | Slot: " << slotId;
    }
    
    if (totalCost > 0) {
        ss << " | Cost: $" << std::fixed << std::setprecision(2) << totalCost;
    }
    
    return ss.str();
}

// Display detailed info
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
       << "Duration: " << getDuration() << " hours" << std::endl
       << "Total Cost: $" << std::fixed << std::setprecision(2) << totalCost << std::endl;
    
    return ss.str();
}