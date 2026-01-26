#include "ParkingSlot.h"
#include <sstream>

ParkingSlot::ParkingSlot(const std::string& id, 
                        const std::string& zone, 
                        const std::string& area, 
                        double rate)
    : slotId(id), zoneId(zone), areaId(area), 
      hourlyRate(rate), isAvailable(true), vehicleId("") {}

std::string ParkingSlot::getSlotId() const {
    return slotId;
}

std::string ParkingSlot::getZoneId() const {
    return zoneId;
}

std::string ParkingSlot::getAreaId() const {
    return areaId;
}

bool ParkingSlot::getIsAvailable() const {
    return isAvailable;
}

std::string ParkingSlot::getVehicleId() const {
    return vehicleId;
}

double ParkingSlot::getHourlyRate() const {
    return hourlyRate;
}

bool ParkingSlot::allocateSlot(const std::string& vehicleId) {
    if (!isAvailable) {
        return false;
    }
    this->vehicleId = vehicleId;
    isAvailable = false;
    return true;
}

bool ParkingSlot::releaseSlot() {
    if (isAvailable) {
        return false;
    }
    vehicleId = "";
    isAvailable = true;
    return true;
}

bool ParkingSlot::canBeAllocated() const {
    return isAvailable;
}

double ParkingSlot::calculateCost(int hours) const {
    return hourlyRate * hours;
}

std::string ParkingSlot::toString() const {
    std::stringstream ss;
    ss << "Slot: " << slotId 
       << " | Zone: " << zoneId
       << " | Area: " << areaId
       << " | Available: " << (isAvailable ? "Yes" : "No")
       << " | Rate: $" << hourlyRate << "/hr";
    if (!isAvailable) {
        ss << " | Vehicle: " << vehicleId;
    }
    return ss.str();
}