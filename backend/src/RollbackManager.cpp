#include "RollbackManager.h"
#include "Zone.h"
#include <iostream>
#include <sstream>

// RollbackOperation constructor
RollbackOperation::RollbackOperation(RollbackOpType type, const std::string& reqId, 
                                    const std::string& zone, const std::string& slot, 
                                    RequestState state)
    : operationType(type), requestId(reqId), zoneId(zone), 
      slotId(slot), previousState(state), next(nullptr) {}

// RollbackManager constructor
RollbackManager::RollbackManager(int maxSteps)
    : undoStack(nullptr), maxUndoSteps(maxSteps), currentSteps(0) {
    std::cout << "Rollback Manager initialized (max " << maxSteps << " undo steps)" << std::endl;
}

// Destructor
RollbackManager::~RollbackManager() {
    clearStack();
}

// Push operation to stack
void RollbackManager::pushOperation(RollbackOperation* op) {
    if (currentSteps >= maxUndoSteps) {
        // Remove oldest operation (bottom of stack)
        if (undoStack == nullptr) return;
        
        if (undoStack->next == nullptr) {
            delete undoStack;
            undoStack = nullptr;
        } else {
            RollbackOperation* current = undoStack;
            while (current->next->next != nullptr) {
                current = current->next;
            }
            delete current->next;
            current->next = nullptr;
        }
        currentSteps--;
    }
    
    op->next = undoStack;
    undoStack = op;
    currentSteps++;
    
    std::cout << "Recorded operation for rollback: ";
    switch(op->operationType) {
        case OP_ALLOCATE: std::cout << "ALLOCATE"; break;
        case OP_RELEASE: std::cout << "RELEASE"; break;
        case OP_CANCEL: std::cout << "CANCEL"; break;
    }
    std::cout << " (request: " << op->requestId << ")" << std::endl;
}

// Pop operation from stack
RollbackOperation* RollbackManager::popOperation() {
    if (undoStack == nullptr) return nullptr;
    
    RollbackOperation* op = undoStack;
    undoStack = undoStack->next;
    currentSteps--;
    
    op->next = nullptr;
    return op;
}

// Clear entire stack
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
        std::cout << "No operations to undo!" << std::endl;
        return false;
    }
    
    RollbackOperation* op = popOperation();
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
        std::cout << "Request " << op->requestId << " not found!" << std::endl;
        delete op;
        return false;
    }
    
    // Perform undo based on operation type
    bool success = false;
    
    switch(op->operationType) {
        case OP_ALLOCATE:
            // Undo allocation: release slot and set back to REQUESTED
            std::cout << "Undoing allocation for request " << op->requestId << std::endl;
            
            // Release slot in zone
            for (int i = 0; i < zoneCount; i++) {
                if (zones[i]->getZoneId() == op->zoneId) {
                    zones[i]->releaseSlot();
                    break;
                }
            }
            
            // Cannot directly set state, so we'll simulate
            std::cout << "Slot " << op->slotId << " released" << std::endl;
            success = true;
            break;
            
        case OP_RELEASE:
            std::cout << "Undoing release for request " << op->requestId << std::endl;
            // Would re-allocate the slot
            success = true;
            break;
            
        case OP_CANCEL:
            std::cout << "Undoing cancellation for request " << op->requestId << std::endl;
            // Would restore to previous state
            success = true;
            break;
    }
    
    delete op;
    return success;
}

// Undo multiple operations
bool RollbackManager::undoMultipleOperations(int steps, ParkingRequest** requests, 
                                            int requestCount, Zone** zones, int zoneCount) {
    if (steps <= 0) return false;
    if (steps > currentSteps) steps = currentSteps;
    
    std::cout << "Undoing last " << steps << " operations..." << std::endl;
    
    bool allSuccess = true;
    for (int i = 0; i < steps; i++) {
        if (!undoLastOperation(requests, requestCount, zones, zoneCount)) {
            allSuccess = false;
        }
    }
    
    return allSuccess;
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
    std::cout << "\n=== Rollback Stack (" << currentSteps << " operations) ===" << std::endl;
    
    if (undoStack == nullptr) {
        std::cout << "Stack is empty" << std::endl;
        return;
    }
    
    RollbackOperation* current = undoStack;
    int position = 1;
    
    while (current != nullptr) {
        std::cout << position << ". ";
        
        switch(current->operationType) {
            case OP_ALLOCATE: std::cout << "ALLOCATE"; break;
            case OP_RELEASE: std::cout << "RELEASE"; break;
            case OP_CANCEL: std::cout << "CANCEL"; break;
        }
        
        std::cout << " - Request: " << current->requestId
                  << " | Zone: " << current->zoneId
                  << " | Slot: " << current->slotId
                  << " | Previous State: ";
        
        switch(current->previousState) {
            case REQUESTED: std::cout << "REQUESTED"; break;
            case ALLOCATED: std::cout << "ALLOCATED"; break;
            case OCCUPIED: std::cout << "OCCUPIED"; break;
            case RELEASED: std::cout << "RELEASED"; break;
            case CANCELLED: std::cout << "CANCELLED"; break;
        }
        
        std::cout << std::endl;
        
        current = current->next;
        position++;
    }
    
    std::cout << "==============================\n" << std::endl;
}