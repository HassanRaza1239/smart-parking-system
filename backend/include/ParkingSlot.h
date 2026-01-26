#ifndef PARKINGSLOT_H
#define PARKINGSLOT_H

#include <string>

class ParkingSlot {
private:
    std::string slotId;
    std::string zoneId;
    std::string areaId;
    bool isAvailable;
    std::string vehicleId;
    double hourlyRate;
    
public:
    // Constructor
    ParkingSlot(const std::string& id, 
               const std::string& zone, 
               const std::string& area, 
               double rate);
    
    // Getters
    std::string getSlotId() const;
    std::string getZoneId() const;
    std::string getAreaId() const;
    bool getIsAvailable() const;
    std::string getVehicleId() const;
    double getHourlyRate() const;
    
    // State management
    bool allocateSlot(const std::string& vehicleId);
    bool releaseSlot();
    bool canBeAllocated() const;
    
    // Cost calculation
    double calculateCost(int hours) const;
    
    // Display
    std::string toString() const;
};

#endif