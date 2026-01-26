#include "include/ParkingArea.h"
#include <iostream>

int main() {
    std::cout << "=== Testing ParkingArea Class ===\n" << std::endl;
    
    // Create parking area
    std::cout << "Test 1: Creating Parking Area..." << std::endl;
    ParkingArea area1("Area1", "ZoneA", 10);
    std::cout << "✅ Created: " << area1.toString() << std::endl;
    
    // Add slots
    std::cout << "\nTest 2: Adding Parking Slots..." << std::endl;
    area1.addSlot(new ParkingSlot("A-101", "ZoneA", "Area1", 5.0));
    area1.addSlot(new ParkingSlot("A-102", "ZoneA", "Area1", 5.0));
    area1.addSlot(new ParkingSlot("A-103", "ZoneA", "Area1", 5.0));
    std::cout << "✅ Added 3 slots" << std::endl;
    std::cout << "Area status: " << area1.toString() << std::endl;
    
    // Test availability
    std::cout << "\nTest 3: Testing Availability..." << std::endl;
    std::cout << "Total capacity: " << area1.getTotalCapacity() << std::endl;
    std::cout << "Available slots: " << area1.getAvailableCount() << std::endl;
    std::cout << "Utilization rate: " << area1.getUtilizationRate() << "%" << std::endl;
    
    // Find available slot
    std::cout << "\nTest 4: Finding Available Slot..." << std::endl;
    ParkingSlot* availableSlot = area1.findAvailableSlot();
    if (availableSlot) {
        std::cout << "✅ Found available slot: " << availableSlot->getSlotId() << std::endl;
        
        // Allocate it
        availableSlot->allocateSlot("CAR123");
        std::cout << "Allocated to CAR123" << std::endl;
    }
    
    // Check updated status
    std::cout << "\nTest 5: Check Updated Status..." << std::endl;
    std::cout << "Available slots now: " << area1.getAvailableCount() << std::endl;
    std::cout << "Utilization rate now: " << area1.getUtilizationRate() << "%" << std::endl;
    
    // Get slot by ID
    std::cout << "\nTest 6: Get Slot by ID..." << std::endl;
    ParkingSlot* foundSlot = area1.getSlotById("A-102");
    if (foundSlot) {
        std::cout << "✅ Found slot: " << foundSlot->toString() << std::endl;
    }
    
    std::cout << "\n=== All ParkingArea Tests Complete! ===" << std::endl;
    return 0;
}