#ifndef ROLLBACKMANAGER_H
#define ROLLBACKMANAGER_H

#include "Zone.h"
#include "ParkingRequest.h"
#include <string>

// Operation types for rollback
enum RollbackOpType {
    OP_ALLOCATE,
    OP_RELEASE,
    OP_CANCEL
};

// Custom stack node for rollback operations
struct RollbackOperation {
    RollbackOpType operationType;
    std::string requestId;
    std::string zoneId;
    std::string slotId;
    RequestState previousState;
    RollbackOperation* next;
    
    RollbackOperation(RollbackOpType type, const std::string& reqId, 
                     const std::string& zone, const std::string& slot, 
                     RequestState state);
};

// Rollback Manager with custom stack
class RollbackManager {
private:
    RollbackOperation* undoStack;
    int maxUndoSteps;
    int currentSteps;
    
public:
    RollbackManager(int maxSteps = 10);
    ~RollbackManager();
    
    // Stack operations
    void pushOperation(RollbackOperation* op);
    RollbackOperation* popOperation();
    void clearStack();
    
    // Rollback execution
    bool undoLastOperation(ParkingRequest** requests, int requestCount, Zone** zones, int zoneCount);
    bool undoMultipleOperations(int steps, ParkingRequest** requests, int requestCount, Zone** zones, int zoneCount);
    
    // Utility
    bool canUndo() const;
    int getAvailableUndoSteps() const;
    int getOperationCount() const;
    
    // Display
    void displayStack() const;
};

#endif