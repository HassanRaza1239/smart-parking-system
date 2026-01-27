#include "../include/ParkingSlot.h"
#include <sstream>

// ParkingSlot constructor
ParkingSlot::ParkingSlot(const std::string& id, 
                         const std::string& zone, 
                         const std::string& area, 
                         double rate)
    : slotId(id), zoneId(zone), areaId(area), isAvailable(true), 
      vehicleId(""), hourlyRate(rate) {}

// Get slot ID
std::string ParkingSlot::getSlotId() const {
    return slotId;
}

// Get zone ID
std::string ParkingSlot::getZoneId() const {
    return zoneId;
}

// Get area ID
std::string ParkingSlot::getAreaId() const {
    return areaId;
}

// Check if available
bool ParkingSlot::getIsAvailable() const {
    return isAvailable;
}

// Get vehicle ID currently using slot
std::string ParkingSlot::getVehicleId() const {
    return vehicleId;
}

// Get hourly rate
double ParkingSlot::getHourlyRate() const {
    return hourlyRate;
}

// Allocate slot to a vehicle
bool ParkingSlot::allocateSlot(const std::string& vid) {
    if (!isAvailable) return false;
    
    isAvailable = false;
    vehicleId = vid;
    return true;
}

// Release slot
bool ParkingSlot::releaseSlot() {
    if (isAvailable) return false;
    
    isAvailable = true;
    vehicleId = "";
    return true;
}

// Check if slot can be allocated
bool ParkingSlot::canBeAllocated() const {
    return isAvailable;
}

// Calculate cost for parking duration
double ParkingSlot::calculateCost(int hours) const {
    if (hours <= 0) return 0.0;
    return hourlyRate * hours;
}

// Convert to string
std::string ParkingSlot::toString() const {
    std::stringstream ss;
    ss << "Slot " << slotId 
       << " [Zone: " << zoneId 
       << ", Area: " << areaId 
       << "] - Status: " << (isAvailable ? "Available" : "Occupied by " + vehicleId)
       << " | Rate: $" << hourlyRate << "/hr";
    return ss.str();
}
