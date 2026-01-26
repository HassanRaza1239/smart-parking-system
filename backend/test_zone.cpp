#include "include/Zone.h"
#include <iostream>

int main() {
    std::cout << "=== Testing Zone Class with Graph Connections ===\n" << std::endl;
    
    // Create zones
    std::cout << "Test 1: Creating Zones..." << std::endl;
    Zone zoneA("ZA", "Zone A - City Center", 30, 5.0);
    Zone zoneB("ZB", "Zone B - Shopping District", 25, 6.0);
    Zone zoneC("ZC", "Zone C - Business Park", 40, 4.5);
    
    std::cout << "✅ Created zones:" << std::endl;
    std::cout << "   " << zoneA.toString() << std::endl;
    std::cout << "   " << zoneB.toString() << std::endl;
    std::cout << "   " << zoneC.toString() << std::endl;
    
    // Create zone connections (Graph edges)
    std::cout << "\nTest 2: Creating Zone Connections (Graph)..." << std::endl;
    
    // Zone A connects to B and C
    zoneA.addConnection(new ZoneConnection("ZB", 500));  // 500 meters
    zoneA.addConnection(new ZoneConnection("ZC", 800));  // 800 meters
    
    // Zone B connects to A and C
    zoneB.addConnection(new ZoneConnection("ZA", 500));
    zoneB.addConnection(new ZoneConnection("ZC", 300));
    
    // Zone C connects to A and B
    zoneC.addConnection(new ZoneConnection("ZA", 800));
    zoneC.addConnection(new ZoneConnection("ZB", 300));
    
    std::cout << "\n✅ Connection Graph Created:" << std::endl;
    std::cout << "   " << zoneA.getConnectionsString() << std::endl;
    std::cout << "   " << zoneB.getConnectionsString() << std::endl;
    std::cout << "   " << zoneC.getConnectionsString() << std::endl;
    
    // Test connection checks
    std::cout << "\nTest 3: Testing Connection Checks..." << std::endl;
    std::cout << "Is Zone A connected to Zone B? " 
              << (zoneA.isConnectedTo("ZB") ? "Yes" : "No") << std::endl;
    std::cout << "Is Zone A connected to Zone C? " 
              << (zoneA.isConnectedTo("ZC") ? "Yes" : "No") << std::endl;
    std::cout << "Is Zone B connected to Zone A? " 
              << (zoneB.isConnectedTo("ZA") ? "Yes" : "No") << std::endl;
    
    // Test distance queries
    std::cout << "\nTest 4: Testing Distance Queries..." << std::endl;
    std::cout << "Distance from A to B: " << zoneA.getDistanceTo("ZB") << "m" << std::endl;
    std::cout << "Distance from B to C: " << zoneB.getDistanceTo("ZC") << "m" << std::endl;
    std::cout << "Distance from A to non-existent: " << zoneA.getDistanceTo("ZD") << "m (should be -1)" << std::endl;
    
    // Test slot allocation
    std::cout << "\nTest 5: Testing Slot Allocation..." << std::endl;
    std::cout << "Available slots in Zone A: " << zoneA.getAvailableSlots() << std::endl;
    
    if (zoneA.allocateSlot()) {
        std::cout << "✅ Allocated slot in Zone A" << std::endl;
    }
    
    std::cout << "Available slots now: " << zoneA.getAvailableSlots() << std::endl;
    std::cout << "Utilization rate: " << zoneA.getUtilizationRate() << "%" << std::endl;
    
    // Test slot release
    std::cout << "\nTest 6: Testing Slot Release..." << std::endl;
    if (zoneA.releaseSlot()) {
        std::cout << "✅ Released slot in Zone A" << std::endl;
    }
    std::cout << "Available slots after release: " << zoneA.getAvailableSlots() << std::endl;
    
    std::cout << "\n=== All Zone Tests Complete! ===" << std::endl;
    return 0;
}