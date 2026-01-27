#include "include/ParkingSystem.h"
#include <iostream>

int main() {
    std::cout << "Testing ParkingSystem..." << std::endl;
    
    // Create system
    ParkingSystem system;
    
    // Try to run demo (which calls initializeZones)
    system.runDemo();
    
    std::cout << "Test completed!" << std::endl;
    return 0;
}
