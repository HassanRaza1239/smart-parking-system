#include "Vehicle.h"
#include <sstream>

// Constructor
Vehicle::Vehicle(const std::string& id, const std::string& zone, VehicleType vtype)
    : vehicleId(id), preferredZone(zone), type(vtype) {}

// Getters
std::string Vehicle::getVehicleId() const {
    return vehicleId;
}

std::string Vehicle::getPreferredZone() const {
    return preferredZone;
}

VehicleType Vehicle::getVehicleType() const {
    return type;
}

std::string Vehicle::getVehicleTypeString() const {
    switch(type) {
        case CAR: return "Car";
        case BIKE: return "Bike";
        case TRUCK: return "Truck";
        default: return "Unknown";
    }
}

// Vehicle type multiplier for pricing
double Vehicle::getTypeMultiplier() const {
    switch(type) {
        case CAR: return 1.0;    // Base rate
        case BIKE: return 0.7;   // 30% cheaper
        case TRUCK: return 1.5;  // 50% more expensive
        default: return 1.0;
    }
}

// Display
std::string Vehicle::toString() const {
    std::stringstream ss;
    ss << "Vehicle: " << vehicleId 
       << " | Type: " << getVehicleTypeString()
       << " | Preferred Zone: " << preferredZone
       << " | Rate Multiplier: " << getTypeMultiplier() << "x";
    return ss.str();
}