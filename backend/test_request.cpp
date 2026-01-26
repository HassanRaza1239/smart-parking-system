#include "include/ParkingRequest.h"
#include <iostream>


int main() {
    std::cout << "=== Testing ParkingRequest State Machine ===\n" << std::endl;
    
    // Create a parking request
    std::cout << "Test 1: Creating Parking Request..." << std::endl;
    ParkingRequest request1("REQ001", "CAR123", "ZoneA");
    std::cout << "✅ Created: " << request1.toString() << std::endl;
    
    // Test allocation
    std::cout << "\nTest 2: Allocating Slot..." << std::endl;
    if (request1.allocate("ZoneA", "A-101", 15.0, false)) {
        std::cout << "✅ Allocation successful!" << std::endl;
        std::cout << "Request status: " << request1.toString() << std::endl;
    }
    
    // Test occupy
    std::cout << "\nTest 3: Occupying Slot..." << std::endl;
    if (request1.occupy()) {
        std::cout << "✅ Occupation successful!" << std::endl;
        std::cout << "Request status: " << request1.toString() << std::endl;
    }
    
    // Test invalid transitions
    std::cout << "\nTest 4: Testing Invalid Transitions..." << std::endl;
    std::cout << "Trying to allocate again (should fail):" << std::endl;
    if (!request1.allocate("ZoneB", "B-101", 20.0, true)) {
        std::cout << "✅ Correctly failed - invalid transition!" << std::endl;
    }
    
  
    // Simulate time passing
    std::cout << "\nTest 5: Simulating parking duration..." << std::endl;
    std::cout << "Simulating 2 hours of parking..." << std::endl;
   
    
    // Test release
    std::cout << "\nTest 6: Releasing Parking..." << std::endl;
    if (request1.release()) {
        std::cout << "✅ Release successful!" << std::endl;
        std::cout << "Final duration: " << request1.getDuration() << " hours" << std::endl;
        std::cout << "Total cost: $" << request1.getTotalCost() << std::endl;
    }
    
    // Test detailed info
    std::cout << "\nTest 7: Detailed Information..." << std::endl;
    std::cout << request1.getDetailedInfo() << std::endl;
    
    // Test cancellation flow
    std::cout << "\nTest 8: Testing Cancellation Flow..." << std::endl;
    ParkingRequest request2("REQ002", "BIKE001", "ZoneB");
    std::cout << "Created: " << request2.toString() << std::endl;
    
    std::cout << "Cancelling before allocation..." << std::endl;
    if (request2.cancel()) {
        std::cout << "✅ Cancellation successful!" << std::endl;
        std::cout << "Final status: " << request2.toString() << std::endl;
    }
    
    // Test trying to modify cancelled request
    std::cout << "\nTest 9: Trying to modify cancelled request (should fail)..." << std::endl;
    if (!request2.allocate("ZoneB", "B-102", 10.0, false)) {
        std::cout << "✅ Correctly failed - cannot modify cancelled request!" << std::endl;
    }
    
    std::cout << "\n=== All State Machine Tests Complete! ===" << std::endl;
    return 0;
}