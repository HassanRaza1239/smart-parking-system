#include "include/RollbackManager.h"
#include "include/Zone.h"
#include "include/ParkingRequest.h"
#include <iostream>

int main() {
    std::cout << "=== Testing Rollback Manager ===\n" << std::endl;
    
    // Create zones and requests for testing
    std::cout << "Test 1: Setting up test environment..." << std::endl;
    Zone* zone1 = new Zone("ZA", "Zone A", 10, 5.0);
    Zone* zone2 = new Zone("ZB", "Zone B", 8, 6.0);
    
    ParkingRequest* request1 = new ParkingRequest("REQ001", "CAR001", "ZA");
    ParkingRequest* request2 = new ParkingRequest("REQ002", "CAR002", "ZA");
    
    Zone* zones[] = {zone1, zone2};
    ParkingRequest* requests[] = {request1, request2};
    int zoneCount = 2;
    int requestCount = 2;
    
    // Create Rollback Manager
    std::cout << "\nTest 2: Creating Rollback Manager..." << std::endl;
    RollbackManager rollbackMgr(5);
    std::cout << "Max undo steps: 5" << std::endl;
    
    // Simulate some operations and record them
    std::cout << "\nTest 3: Recording operations..." << std::endl;
    
    // Record allocation operation 1
    rollbackMgr.pushOperation(new RollbackOperation(
        OP_ALLOCATE, "REQ001", "ZA", "A-101", REQUESTED
    ));
    
    // Simulate allocation
    zone1->allocateSlot();
    request1->allocate("ZA", "A-101", 15.0, false);
    
    // Record allocation operation 2
    rollbackMgr.pushOperation(new RollbackOperation(
        OP_ALLOCATE, "REQ002", "ZA", "A-102", REQUESTED
    ));
    
    // Simulate allocation
    zone1->allocateSlot();
    request2->allocate("ZA", "A-102", 15.0, false);
    
    // Display current state
    std::cout << "\nCurrent zone status:" << std::endl;
    std::cout << "Zone A: " << zone1->getAvailableSlots() << "/" 
              << zone1->getTotalSlots() << " available" << std::endl;
    std::cout << "Zone B: " << zone2->getAvailableSlots() << "/" 
              << zone2->getTotalSlots() << " available" << std::endl;
    
    // Display rollback stack
    rollbackMgr.displayStack();
    
    // Test single undo
    std::cout << "\nTest 4: Undoing last operation..." << std::endl;
    if (rollbackMgr.undoLastOperation(requests, requestCount, zones, zoneCount)) {
        std::cout << "✅ Undo successful!" << std::endl;
        std::cout << "Zone A now: " << zone1->getAvailableSlots() << "/" 
                  << zone1->getTotalSlots() << " available" << std::endl;
    }
    
    rollbackMgr.displayStack();
    
    // Record more operations
    std::cout << "\nTest 5: Recording more operations..." << std::endl;
    
    // Record cancellation
    rollbackMgr.pushOperation(new RollbackOperation(
        OP_CANCEL, "REQ002", "ZA", "A-102", ALLOCATED
    ));
    
    // Simulate cancellation
    request2->cancel();
    
    // Record cross-zone allocation
    rollbackMgr.pushOperation(new RollbackOperation(
        OP_ALLOCATE, "REQ002", "ZB", "B-101", CANCELLED
    ));
    
    // Simulate allocation in different zone
    zone2->allocateSlot();
    request2->allocate("ZB", "B-101", 18.0, true);
    
    rollbackMgr.displayStack();
    
    // Test multiple undo
    std::cout << "\nTest 6: Undoing 2 operations..." << std::endl;
    std::cout << "Before undo:" << std::endl;
    std::cout << "Zone A available: " << zone1->getAvailableSlots() << std::endl;
    std::cout << "Zone B available: " << zone2->getAvailableSlots() << std::endl;
    
    if (rollbackMgr.undoMultipleOperations(2, requests, requestCount, zones, zoneCount)) {
        std::cout << "\n✅ 2 operations undone successfully!" << std::endl;
        std::cout << "After undo:" << std::endl;
        std::cout << "Zone A available: " << zone1->getAvailableSlots() << std::endl;
        std::cout << "Zone B available: " << zone2->getAvailableSlots() << std::endl;
    }
    
    rollbackMgr.displayStack();
    
    // Test undo limits
    std::cout << "\nTest 7: Testing undo limits..." << std::endl;
    std::cout << "Available undo steps: " << rollbackMgr.getAvailableUndoSteps() << std::endl;
    std::cout << "Can undo? " << (rollbackMgr.canUndo() ? "Yes" : "No") << std::endl;
    
    // Try to undo more than available
    std::cout << "Trying to undo 5 steps (only " << rollbackMgr.getAvailableUndoSteps() << " available)..." << std::endl;
    if (rollbackMgr.undoMultipleOperations(5, requests, requestCount, zones, zoneCount)) {
        std::cout << "Undo completed" << std::endl;
    }
    
    std::cout << "Available undo steps now: " << rollbackMgr.getAvailableUndoSteps() << std::endl;
    std::cout << "Can undo? " << (rollbackMgr.canUndo() ? "Yes" : "No") << std::endl;
    
    // Cleanup
    std::cout << "\nTest 8: Cleanup..." << std::endl;
    delete zone1;
    delete zone2;
    delete request1;
    delete request2;
    
    std::cout << "\n=== All Rollback Manager Tests Complete! ===" << std::endl;
    return 0;
}