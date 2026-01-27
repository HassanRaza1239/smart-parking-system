#include "../include/Vehicle.h"
#include <sstream>

// Vehicle constructor
Vehicle::Vehicle(const std::string& id, const std::string& zone, VehicleType vtype)
    : vehicleId(id), preferredZone(zone), type(vtype) {}

// Get vehicle ID
std::string Vehicle::getVehicleId() const {
    return vehicleId;
}

// Get preferred zone
std::string Vehicle::getPreferredZone() const {
    return preferredZone;
}

// Get vehicle type
VehicleType Vehicle::getVehicleType() const {
    return type;
}

// Get vehicle type as string
std::string Vehicle::getVehicleTypeString() const {
    switch (type) {
        case CAR: return "CAR";
        case BIKE: return "BIKE";
        case TRUCK: return "TRUCK";
        default: return "UNKNOWN";
    }
}

// Get type multiplier for pricing
double Vehicle::getTypeMultiplier() const {
    switch (type) {
        case CAR: return 1.0;
        case BIKE: return 0.5;
        case TRUCK: return 2.0;
        default: return 1.0;
    }
}

// Convert to string
std::string Vehicle::toString() const {
    std::stringstream ss;
    ss << "Vehicle " << vehicleId 
       << " (" << getVehicleTypeString() 
       << ") - Preferred Zone: " << preferredZone
       << " (Multiplier: " << getTypeMultiplier() << "x)";
    return ss.str();
}
