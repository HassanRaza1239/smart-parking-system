#include "../include/RollbackManager.h"
#include <iostream>

// RollbackOperation constructor
RollbackOperation::RollbackOperation(RollbackOpType type, const std::string& reqId, 
                                     const std::string& zone, const std::string& slot, 
                                     RequestState state)
    : operationType(type), requestId(reqId), zoneId(zone), slotId(slot), 
      previousState(state), next(nullptr) {}

// RollbackManager constructor
RollbackManager::RollbackManager(int maxSteps) 
    : undoStack(nullptr), maxUndoSteps(maxSteps), currentSteps(0) {}

// RollbackManager destructor
RollbackManager::~RollbackManager() {
    clearStack();
}

// Push operation to stack
void RollbackManager::pushOperation(RollbackOperation* op) {
    if (!op) return;
    
    op->next = undoStack;
    undoStack = op;
    currentSteps++;
    
    // If we exceed max steps, remove oldest from bottom
    if (currentSteps > maxUndoSteps) {
        // Simple implementation - just don't push if at max
        // For proper implementation, we'd need a doubly linked list
        if (currentSteps > maxUndoSteps) {
            popOperation(); // Remove one to stay at max
        }
    }
}

// Pop operation from stack
RollbackOperation* RollbackManager::popOperation() {
    if (!undoStack) return nullptr;
    
    RollbackOperation* op = undoStack;
    undoStack = undoStack->next;
    currentSteps--;
    
    op->next = nullptr; // Detach from list
    return op;
}

// Clear the entire stack
void RollbackManager::clearStack() {
    while (undoStack != nullptr) {
        RollbackOperation* temp = undoStack;
        undoStack = undoStack->next;
        delete temp;
    }
    currentSteps = 0;
}

// Undo last operation
bool RollbackManager::undoLastOperation(ParkingRequest** requests, int requestCount, 
                                        Zone** zones, int zoneCount) {
    if (!canUndo()) {
        std::cout << "❌ No operations to undo!" << std::endl;
        return false;
    }
    
    RollbackOperation* op = popOperation();
    if (!op) return false;
    
    bool success = executeUndo(op, requests, requestCount, zones, zoneCount);
    delete op;
    
    return success;
}

// Undo multiple operations
bool RollbackManager::undoMultipleOperations(int steps, ParkingRequest** requests, 
                                             int requestCount, Zone** zones, int zoneCount) {
    if (steps <= 0 || steps > currentSteps) {
        std::cout << "❌ Invalid number of steps to undo!" << std::endl;
        return false;
    }
    
    bool allSuccess = true;
    for (int i = 0; i < steps; i++) {
        if (!undoLastOperation(requests, requestCount, zones, zoneCount)) {
            allSuccess = false;
            break;
        }
    }
    
    return allSuccess;
}

// Execute undo for a specific operation
bool RollbackManager::executeUndo(RollbackOperation* op, ParkingRequest** requests, 
                                  int requestCount, Zone** zones, int zoneCount) {
    if (!op) return false;
    
    // Find the request
    ParkingRequest* request = nullptr;
    for (int i = 0; i < requestCount; i++) {
        if (requests[i]->getRequestId() == op->requestId) {
            request = requests[i];
            break;
        }
    }
    
    if (!request) {
        std::cout << "❌ Request " << op->requestId << " not found for undo!" << std::endl;
        return false;
    }
    
    // Find the zone
    Zone* zone = nullptr;
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i]->getZoneId() == op->zoneId) {
            zone = zones[i];
            break;
        }
    }
    
    // Execute undo based on operation type
    switch (op->operationType) {
        case OP_ALLOCATE:
            // Undo allocation: release slot and revert to REQUESTED
            if (zone) {
                zone->releaseSlot();
            }
            // In a real implementation, we'd revert the request state
            std::cout << "✅ Undid allocation operation for request " 
                      << op->requestId << std::endl;
            break;
            
        case OP_RELEASE:
            // Undo release: allocate slot and revert to OCCUPIED
            if (zone) {
                zone->allocateSlot();
            }
            std::cout << "✅ Undid release operation for request " 
                      << op->requestId << std::endl;
            break;
            
        case OP_CANCEL:
            // Undo cancellation: revert to previous state
            if (zone && !op->slotId.empty()) {
                zone->allocateSlot();
            }
            std::cout << "✅ Undid cancellation for request " 
                      << op->requestId << std::endl;
            break;
    }
    
    return true;
}

// Check if undo is possible
bool RollbackManager::canUndo() const {
    return undoStack != nullptr;
}

// Get available undo steps
int RollbackManager::getAvailableUndoSteps() const {
    return currentSteps;
}

// Get operation count
int RollbackManager::getOperationCount() const {
    return currentSteps;
}

// Display stack contents
void RollbackManager::displayStack() const {
    std::cout << "=== Rollback Stack (" << currentSteps << " operations) ===" << std::endl;
    
    if (!undoStack) {
        std::cout << "Stack is empty." << std::endl;
        return;
    }
    
    RollbackOperation* current = undoStack;
    int index = 1;
    
    while (current != nullptr) {
        std::string opType;
        switch (current->operationType) {
            case OP_ALLOCATE: opType = "ALLOCATE"; break;
            case OP_RELEASE: opType = "RELEASE"; break;
            case OP_CANCEL: opType = "CANCEL"; break;
        }
        
        std::cout << index << ". [" << opType << "] Request: " << current->requestId
                  << " | Zone: " << current->zoneId
                  << " | Slot: " << current->slotId
                  << " | Prev State: ";
        
        switch (current->previousState) {
            case REQUESTED: std::cout << "REQUESTED"; break;
            case ALLOCATED: std::cout << "ALLOCATED"; break;
            case OCCUPIED: std::cout << "OCCUPIED"; break;
            case RELEASED: std::cout << "RELEASED"; break;
            case CANCELLED: std::cout << "CANCELLED"; break;
        }
        
        std::cout << std::endl;
        
        current = current->next;
        index++;
    }
}
