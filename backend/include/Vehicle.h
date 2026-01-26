#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

enum VehicleType {
    CAR,
    BIKE,
    TRUCK
};

class Vehicle {
private:
    std::string vehicleId;
    std::string preferredZone;
    VehicleType type;
    
public:
    Vehicle(const std::string& id, const std::string& zone, VehicleType vtype = CAR);
    
    // Getters
    std::string getVehicleId() const;
    std::string getPreferredZone() const;
    VehicleType getVehicleType() const;
    std::string getVehicleTypeString() const;
    
    // Vehicle type multiplier for pricing
    double getTypeMultiplier() const;
    
    // Display
    std::string toString() const;
};

#endif