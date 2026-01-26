#ifndef PARKINGREQUEST_H
#define PARKINGREQUEST_H

#include <string>
#include <ctime>

enum RequestState {
    REQUESTED,
    ALLOCATED,
    OCCUPIED,
    RELEASED,
    CANCELLED
};

class ParkingRequest {
private:
    std::string requestId;
    std::string vehicleId;
    std::string preferredZone;
    std::string allocatedZone;
    std::string slotId;
    RequestState currentState;
    time_t requestTime;
    time_t allocationTime;
    time_t completionTime;
    int durationHours;
    double totalCost;
    bool isCrossZone;
    
public:
    ParkingRequest(const std::string& reqId, 
                  const std::string& vehicle, 
                  const std::string& zone);
    
    // State transitions (STRICT state machine)
    bool allocate(const std::string& zone, const std::string& slot, 
                 double cost, bool crossZone = false);
    bool occupy();
    bool release();
    bool cancel();
    
    // State validation
    bool isValidTransition(RequestState newState) const;
    
    // Getters
    std::string getRequestId() const;
    std::string getVehicleId() const;
    std::string getPreferredZone() const;
    std::string getAllocatedZone() const;
    std::string getSlotId() const;
    RequestState getState() const;
    std::string getStateString() const;
    time_t getRequestTime() const;
    time_t getAllocationTime() const;
    time_t getCompletionTime() const;
    double getDuration() const;  // in hours
    double getTotalCost() const;
    bool getIsCrossZone() const;
    
    // Display
    std::string toString() const;
    std::string getDetailedInfo() const;
};

#endif