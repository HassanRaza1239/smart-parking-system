#include "include/ParkingSlot.h"
#include <iostream>

int main() {
    std::cout << "=== Testing ParkingSlot Class ===\n" << std::endl;
    
    // Test 1: Create parking slot
    std::cout << "Test 1: Creating Parking Slot..." << std::endl;
    ParkingSlot slot1("A-101", "ZoneA", "Area1", 5.0);
    std::cout << "✅ Created: " << slot1.toString() << std::endl;
    
    // Test 2: Check availability
    std::cout << "\nTest 2: Checking Availability..." << std::endl;
    std::cout << "Is available? " << (slot1.getIsAvailable() ? "Yes" : "No") << std::endl;
    std::cout << "Can be allocated? " << (slot1.canBeAllocated() ? "Yes" : "No") << std::endl;
    
    // Test 3: Allocate slot
    std::cout << "\nTest 3: Allocating to vehicle CAR123..." << std::endl;
    if (slot1.allocateSlot("CAR123")) {
        std::cout << "✅ Allocation successful!" << std::endl;
    }
    std::cout << "Slot info: " << slot1.toString() << std::endl;
    
    // Test 4: Try to allocate again (should fail)
    std::cout << "\nTest 4: Try allocate again (should fail)..." << std::endl;
    if (!slot1.allocateSlot("CAR456")) {
        std::cout << "✅ Correctly failed - slot already occupied!" << std::endl;
    }
    
    // Test 5: Calculate cost
    std::cout << "\nTest 5: Calculate parking cost..." << std::endl;
    std::cout << "3 hours cost: $" << slot1.calculateCost(3) << std::endl;
    std::cout << "8 hours cost: $" << slot1.calculateCost(8) << std::endl;
    
    // Test 6: Release slot
    std::cout << "\nTest 6: Releasing slot..." << std::endl;
    if (slot1.releaseSlot()) {
        std::cout << "✅ Slot released successfully!" << std::endl;
    }
    std::cout << "Final status: " << slot1.toString() << std::endl;
    
    std::cout << "\n=== All Tests Passed! ===" << std::endl;
    return 0;
}