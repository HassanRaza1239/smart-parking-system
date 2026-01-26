#include "include/AllocationEngine.h"
#include "include/Vehicle.h"
#include "include/ParkingRequest.h"
#include <iostream>

int main() {
    std::cout << "=== Testing Allocation Engine ===\n" << std::endl;
    
    // Create zones
    std::cout << "Test 1: Setting up city zones..." << std::endl;
    Zone* zones[5];
    zones[0] = new Zone("ZA", "Zone A - City Center", 10, 5.0);
    zones[1] = new Zone("ZB", "Zone B - Shopping", 8, 6.0);
    zones[2] = new Zone("ZC", "Zone C - Business", 12, 4.5);
    zones[3] = new Zone("ZD", "Zone D - University", 15, 3.5);
    zones[4] = new Zone("ZE", "Zone E - Riverside", 6, 7.0);
    
    // Create connections
    zones[0]->addConnection(new ZoneConnection("ZB", 500));
    zones[0]->addConnection(new ZoneConnection("ZC", 800));
    zones[1]->addConnection(new ZoneConnection("ZA", 500));
    zones[1]->addConnection(new ZoneConnection("ZC", 300));
    zones[1]->addConnection(new ZoneConnection("ZD", 700));
    zones[2]->addConnection(new ZoneConnection("ZA", 800));
    zones[2]->addConnection(new ZoneConnection("ZB", 300));
    zones[2]->addConnection(new ZoneConnection("ZE", 400));
    
    // Create Allocation Engine
    AllocationEngine allocator(zones, 5);
    allocator.displayZoneStatus();
    
    // Test 1: Simple allocation in preferred zone
    std::cout << "\nTest 2: Simple Allocation (Car in preferred zone)..." << std::endl;
    Vehicle car1("CAR001", "ZA", CAR);
    ParkingRequest req1("REQ001", "CAR001", "ZA");
    
    std::string allocatedZone1, allocatedSlot1;
    std::vector<std::string> path1;
    double cost1;
    
    if (allocator.allocateParking(&req1, &car1, 3, allocatedZone1, allocatedSlot1, path1, cost1)) {
        std::cout << "✅ Allocation successful!" << std::endl;
        std::cout << "Request details: " << req1.toString() << std::endl;
    }
    
    allocator.displayZoneStatus();
    
    // Test 2: Another allocation in same zone
    std::cout << "\nTest 3: Second Allocation (Bike in same zone)..." << std::endl;
    Vehicle bike1("BIKE001", "ZA", BIKE);
    ParkingRequest req2("REQ002", "BIKE001", "ZA");
    
    std::string allocatedZone2, allocatedSlot2;
    std::vector<std::string> path2;
    double cost2;
    
    if (allocator.allocateParking(&req2, &bike1, 2, allocatedZone2, allocatedSlot2, path2, cost2)) {
        std::cout << "✅ Bike allocation successful!" << std::endl;
        std::cout << "Bike pays less: $" << cost2 << " (Car would pay: $" << (5.0 * 2) << ")" << std::endl;
    }
    
    allocator.displayZoneStatus();
    
    // Fill up Zone A
    std::cout << "\nTest 4: Filling up Zone A..." << std::endl;
    for (int i = 0; i < 8; i++) { // Fill remaining 8 slots
        zones[0]->allocateSlot();
    }
    std::cout << "Zone A is now full!" << std::endl;
    
    // Test 3: Cross-zone allocation (Zone A full)
    std::cout << "\nTest 5: Cross-Zone Allocation (Zone A full)..." << std::endl;
    Vehicle car2("CAR002", "ZA", CAR);
    ParkingRequest req3("REQ003", "CAR002", "ZA");
    
    std::string allocatedZone3, allocatedSlot3;
    std::vector<std::string> path3;
    double cost3;
    
    if (allocator.allocateParking(&req3, &car2, 4, allocatedZone3, allocatedSlot3, path3, cost3)) {
        std::cout << "✅ Cross-zone allocation successful!" << std::endl;
        std::cout << "Path taken: ";
        for (size_t i = 0; i < path3.size(); i++) {
            std::cout << path3[i];
            if (i < path3.size() - 1) std::cout << " -> ";
        }
        std::cout << std::endl;
        std::cout << "Cross-zone penalty applied: $" << cost3 << std::endl;
    }
    
    allocator.displayZoneStatus();
    
    // Test 4: Truck with different pricing
    std::cout << "\nTest 6: Truck Allocation (Higher rate)..." << std::endl;
    Vehicle truck1("TRUCK001", "ZB", TRUCK);
    ParkingRequest req4("REQ004", "TRUCK001", "ZB");
    
    std::string allocatedZone4, allocatedSlot4;
    std::vector<std::string> path4;
    double cost4;
    
    if (allocator.allocateParking(&req4, &truck1, 5, allocatedZone4, allocatedSlot4, path4, cost4)) {
        std::cout << "✅ Truck allocation successful!" << std::endl;
        std::cout << "Truck pays more: $" << cost4 << " (Car would pay: $" << (6.0 * 5) << ")" << std::endl;
    }
    
    // Test statistics
    std::cout << "\nTest 7: System Statistics..." << std::endl;
    std::cout << "Total capacity: " << allocator.getTotalCapacity() << " slots" << std::endl;
    std::cout << "Available slots: " << allocator.getTotalAvailableSlots() << std::endl;
    std::cout << "Overall utilization: " << allocator.getOverallUtilization() << "%" << std::endl;
    
    // Cleanup
    std::cout << "\nTest 8: Cleanup..." << std::endl;
    for (int i = 0; i < 5; i++) {
        delete zones[i];
    }
    
    std::cout << "\n=== All Allocation Engine Tests Complete! ===" << std::endl;
    return 0;
}