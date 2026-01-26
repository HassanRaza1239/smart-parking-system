#include "include/Vehicle.h"
#include <iostream>

int main() {
    std::cout << "=== Testing Vehicle Class ===\n" << std::endl;
    
    // Create different vehicle types
    std::cout << "Test 1: Creating Different Vehicle Types..." << std::endl;
    Vehicle car1("CAR123", "ZoneA", CAR);
    Vehicle bike1("BIKE001", "ZoneB", BIKE);
    Vehicle truck1("TRUCK999", "ZoneC", TRUCK);
    
    std::cout << "✅ Created vehicles:" << std::endl;
    std::cout << "   " << car1.toString() << std::endl;
    std::cout << "   " << bike1.toString() << std::endl;
    std::cout << "   " << truck1.toString() << std::endl;
    
    // Test getters
    std::cout << "\nTest 2: Testing Getters..." << std::endl;
    std::cout << "Car ID: " << car1.getVehicleId() << std::endl;
    std::cout << "Car preferred zone: " << car1.getPreferredZone() << std::endl;
    std::cout << "Car type: " << car1.getVehicleTypeString() << std::endl;
    
    // Test pricing multipliers
    std::cout << "\nTest 3: Testing Pricing Multipliers..." << std::endl;
    std::cout << "Car multiplier: " << car1.getTypeMultiplier() << "x" << std::endl;
    std::cout << "Bike multiplier: " << bike1.getTypeMultiplier() << "x" << std::endl;
    std::cout << "Truck multiplier: " << truck1.getTypeMultiplier() << "x" << std::endl;
    
    // Calculate sample costs
    std::cout << "\nTest 4: Sample Cost Calculation..." << std::endl;
    double baseRate = 5.0; // $5/hour
    int hours = 3;
    
    std::cout << "Base rate: $" << baseRate << "/hour for " << hours << " hours" << std::endl;
    std::cout << "Car cost: $" << (baseRate * hours * car1.getTypeMultiplier()) << std::endl;
    std::cout << "Bike cost: $" << (baseRate * hours * bike1.getTypeMultiplier()) << std::endl;
    std::cout << "Truck cost: $" << (baseRate * hours * truck1.getTypeMultiplier()) << std::endl;
    
    // Test enum values
    std::cout << "\nTest 5: Testing Vehicle Type Enum..." << std::endl;
    VehicleType type = car1.getVehicleType();
    if (type == CAR) {
        std::cout << "✅ Correctly identified as CAR" << std::endl;
    }
    
    std::cout << "\n=== All Vehicle Tests Complete! ===" << std::endl;
    return 0;
}