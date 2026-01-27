# COMPLETE SOURCE CODE REFERENCE

## HEADER FILES

### 1. ParkingSystem.h

```cpp
#ifndef PARKINGSYSTEM_H
#define PARKINGSYSTEM_H

#include "Zone.h"
#include "Vehicle.h"
#include "ParkingRequest.h"
#include "AllocationEngine.h"
#include "RollbackManager.h"
#include "Analytics.h"
#include <vector>
#include <string>

class ParkingSystem {
private:
    Zone** zones;
    int zoneCount;
    std::vector<Vehicle*> vehicles;
    std::vector<ParkingRequest*> requests;
    
    AllocationEngine* allocator;
    RollbackManager* rollbackManager;
    Analytics* analytics;
    
    Vehicle* findVehicle(const std::string& vehicleId) const;
    ParkingRequest* findRequest(const std::string& requestId) const;
    Zone* findZone(const std::string& zoneId) const;
    
public:
    ParkingSystem();
    ~ParkingSystem();
    
    void initializeZones();
    void addZone(Zone* zone);
    void addVehicle(Vehicle* vehicle);
    
    std::string requestParking(const std::string& vehicleId, 
                              const std::string& preferredZone,
                              int durationHours);
    
    bool cancelParking(const std::string& requestId);
    bool occupyParking(const std::string& requestId);
    bool releaseParking(const std::string& requestId);
    
    bool undoLastOperation();
    bool undoOperations(int steps);
    
    void displayAnalytics() const;
    void displayZoneStatus() const;
    void displayAllRequests() const;
    
    int getTotalAvailableSlots() const;
    int getTotalCapacity() const;
    double getOverallUtilization() const;
    
    void runDemo();
};

#endif
```

### 2. Zone.h

```cpp
#ifndef ZONE_H
#define ZONE_H

#include "ParkingArea.h"
#include <string>

struct ZoneConnection {
    std::string connectedZoneId;
    int distance;
    double penaltyMultiplier;
    ZoneConnection* next;
    
    ZoneConnection(const std::string& zoneId, int dist, double penalty = 1.5);
};

class Zone {
private:
    std::string zoneId;
    std::string zoneName;
    int totalSlots;
    int availableSlots;
    double hourlyRate;
    
    ParkingArea* areaList;
    int areaCount;
    
    ZoneConnection* connections;
    
public:
    Zone(const std::string& id, const std::string& name, int slots, double rate);
    ~Zone();
    
    void addConnection(ZoneConnection* connection);
    ZoneConnection* getConnections() const;
    bool isConnectedTo(const std::string& zoneId) const;
    int getDistanceTo(const std::string& zoneId) const;
    
    bool addArea(ParkingArea* area);
    ParkingArea* findAreaWithAvailableSlot() const;
    ParkingSlot* findAvailableSlot() const;
    
    bool allocateSlot();
    bool releaseSlot();
    
    std::string getZoneId() const;
    std::string getZoneName() const;
    int getAvailableSlots() const;
    int getTotalSlots() const;
    double getHourlyRate() const;
    double getUtilizationRate() const;
    
    std::string toString() const;
    std::string getConnectionsString() const;
};

#endif
```

### 3. Vehicle.h

```cpp
#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

enum VehicleType {
    CAR,
    BIKE,
    TRUCK
};

class Vehicle {
private:
    std::string vehicleId;
    std::string preferredZone;
    VehicleType type;
    
public:
    Vehicle(const std::string& id, const std::string& zone, VehicleType vtype = CAR);
    
    std::string getVehicleId() const;
    std::string getPreferredZone() const;
    VehicleType getVehicleType() const;
    std::string getVehicleTypeString() const;
    
    double getTypeMultiplier() const;
    
    std::string toString() const;
};

#endif
```

### 4. ParkingRequest.h

```cpp
#ifndef PARKINGREQUEST_H
#define PARKINGREQUEST_H

#include <string>
#include <ctime>

enum RequestState {
    REQUESTED,
    ALLOCATED,
    OCCUPIED,
    RELEASED,
    CANCELLED
};

class ParkingRequest {
private:
    std::string requestId;
    std::string vehicleId;
    std::string preferredZone;
    std::string allocatedZone;
    std::string slotId;
    RequestState currentState;
    time_t requestTime;
    time_t allocationTime;
    time_t completionTime;
    int durationHours;
    double totalCost;
    bool isCrossZone;
    
public:
    ParkingRequest(const std::string& reqId, 
                  const std::string& vehicle, 
                  const std::string& zone);
    
    bool allocate(const std::string& zone, const std::string& slot, 
                 double cost, bool crossZone = false);
    bool occupy();
    bool release();
    bool cancel();
    
    bool isValidTransition(RequestState newState) const;
    
    std::string getRequestId() const;
    std::string getVehicleId() const;
    std::string getPreferredZone() const;
    std::string getAllocatedZone() const;
    std::string getSlotId() const;
    RequestState getState() const;
    std::string getStateString() const;
    time_t getRequestTime() const;
    time_t getAllocationTime() const;
    time_t getCompletionTime() const;
    double getDuration() const;
    double getTotalCost() const;
    bool getIsCrossZone() const;
    
    std::string toString() const;
    std::string getDetailedInfo() const;
};

#endif
```

### 5. AllocationEngine.h

```cpp
#ifndef ALLOCATIONENGINE_H
#define ALLOCATIONENGINE_H

#include "Zone.h"
#include "ParkingRequest.h"
#include "PathFinder.h"
#include "Vehicle.h"
#include <string>
#include <vector>

class AllocationEngine {
private:
    Zone** zones;
    int zoneCount;
    PathFinder* pathFinder;
    
    std::string findAvailableSlotInZone(const std::string& zoneId);
    
    double calculateCost(
        const std::string& zoneId,
        int hours,
        bool crossZone,
        Vehicle* vehicle
    ) const;
    
public:
    AllocationEngine(Zone** zoneArray, int count);
    ~AllocationEngine();
    
    bool allocateParking(
        ParkingRequest* request,
        Vehicle* vehicle,
        int durationHours,
        std::string& allocatedZone,
        std::string& allocatedSlot,
        std::vector<std::string>& optimalPath,
        double& totalCost
    );
    
    std::string findNearestAvailableZone(
        const std::string& preferredZone,
        std::vector<std::string>& path
    );
    
    int getTotalAvailableSlots() const;
    int getTotalCapacity() const;
    double getOverallUtilization() const;
    
    void displayZoneStatus() const;
};

#endif
```

### 6. PathFinder.h

```cpp
#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Zone.h"
#include <string>
#include <vector>

struct PathNode {
    std::string zoneId;
    int distance;
    std::string previousZone;
    
    PathNode(const std::string& id, int dist, const std::string& prev = "");
    bool operator>(const PathNode& other) const;
};

class MinHeap {
private:
    PathNode** heap;
    int heapSize;
    int capacity;
    
    void heapifyUp(int index);
    void heapifyDown(int index);
    
public:
    MinHeap(int maxSize);
    ~MinHeap();
    
    void push(PathNode* node);
    PathNode* pop();
    bool isEmpty() const;
    void updateDistance(const std::string& zoneId, int newDistance, const std::string& previous);
};

class PathFinder {
private:
    MinHeap* priorityQueue;
    
public:
    PathFinder(int maxZones);
    ~PathFinder();
    
    std::vector<std::string> findShortestPath(
        Zone* startZone,
        Zone* targetZone,
        Zone** allZones,
        int zoneCount
    );
    
    int calculateDistance(const std::vector<std::string>& path, Zone** allZones, int zoneCount);
    
    std::string findNearestAvailableZone(
        const std::string& startZoneId,
        Zone** allZones,
        int zoneCount,
        std::vector<std::string>& path
    );
};

#endif
```

### 7. RollbackManager.h

```cpp
#ifndef ROLLBACKMANAGER_H
#define ROLLBACKMANAGER_H

#include "Zone.h"
#include "ParkingRequest.h"
#include <string>

enum RollbackOpType {
    OP_ALLOCATE,
    OP_RELEASE,
    OP_CANCEL
};

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

class RollbackManager {
private:
    RollbackOperation* undoStack;
    int maxUndoSteps;
    int currentSteps;
    
public:
    RollbackManager(int maxSteps = 10);
    ~RollbackManager();
    
    void pushOperation(RollbackOperation* op);
    RollbackOperation* popOperation();
    void clearStack();
    
    bool undoLastOperation(ParkingRequest** requests, int requestCount, Zone** zones, int zoneCount);
    bool undoMultipleOperations(int steps, ParkingRequest** requests, int requestCount, Zone** zones, int zoneCount);
    
    bool canUndo() const;
    int getAvailableUndoSteps() const;
    int getOperationCount() const;
    
    void displayStack() const;
};

#endif
```

### 8. Analytics.h

```cpp
#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "ParkingRequest.h"
#include "Zone.h"
#include <vector>
#include <string>

class Analytics {
private:
    ParkingRequest** requests;
    int requestCount;
    Zone** zones;
    int zoneCount;
    
public:
    Analytics(ParkingRequest** reqArray, int reqCount, Zone** zoneArray, int zCount);
    
    double getAverageParkingDuration() const;
    double getZoneUtilizationRate(const std::string& zoneId) const;
    std::vector<double> getAllZoneUtilizationRates() const;
    
    int getCompletedRequests() const;
    int getCancelledRequests() const;
    double getCompletionRate() const;
    double getCancellationRate() const;
    
    std::string getPeakUsageZone() const;
    std::vector<std::string> getPeakUsageZones(int topN = 3) const;
    
    double getTotalRevenue() const;
    double getAverageRevenuePerHour() const;
    double getRevenueByZone(const std::string& zoneId) const;
    
    int getCrossZoneAllocations() const;
    double getCrossZoneRate() const;
    
    void displaySummary() const;
    void displayDetailedReport() const;
    
    std::string generateReport() const;
};

#endif
```

### 9. ParkingArea.h

```cpp
#ifndef PARKINGAREA_H
#define PARKINGAREA_H

#include "ParkingSlot.h"
#include <string>

struct SlotNode {
    ParkingSlot* slot;
    SlotNode* next;
    
    SlotNode(ParkingSlot* s);
};

class ParkingArea {
private:
    std::string areaId;
    std::string zoneId;
    int capacity;
    int availableSlots;
    
    SlotNode* slotList;
    
public:
    ParkingArea(const std::string& id, const std::string& zone, int cap);
    ~ParkingArea();
    
    bool addSlot(ParkingSlot* slot);
    ParkingSlot* findAvailableSlot() const;
    ParkingSlot* getSlotById(const std::string& slotId) const;
    
    int getAvailableCount() const;
    int getTotalCapacity() const;
    double getUtilizationRate() const;
    
    std::string getAreaId() const;
    std::string getZoneId() const;
    
    std::string toString() const;
};

#endif
```

### 10. ParkingSlot.h

```cpp
#ifndef PARKINGSLOT_H
#define PARKINGSLOT_H

#include <string>

class ParkingSlot {
private:
    std::string slotId;
    std::string zoneId;
    std::string areaId;
    bool isAvailable;
    std::string vehicleId;
    double hourlyRate;
    
public:
    ParkingSlot(const std::string& id, 
               const std::string& zone, 
               const std::string& area, 
               double rate);
    
    std::string getSlotId() const;
    std::string getZoneId() const;
    std::string getAreaId() const;
    bool getIsAvailable() const;
    std::string getVehicleId() const;
    double getHourlyRate() const;
    
    bool allocateSlot(const std::string& vehicleId);
    bool releaseSlot();
    bool canBeAllocated() const;
    
    double calculateCost(int hours) const;
    
    std::string toString() const;
};

#endif
```

---

## FRONTEND FILES

### frontend/index.html

The frontend is a complete single-page application (SPA) with:
- Interactive Leaflet.js map
- Parking request form
- Zone management
- Analytics dashboard
- Real-time visualization
- Mock pathfinding algorithm

**Key Features:**
- Responsive design with grid layout
- Modal dialogs for user interactions
- Toast notifications
- Location services integration
- Dijkstra's algorithm visualization
- Progress tracking system

**Current State:** Uses mock data, needs HTTP API integration

---

## MAIN EXECUTABLE

### backend/main.cpp

Features menu-driven interface:
1. **Full Demo** - Complete system demonstration
2. **Test Cases** - Runs predefined test scenarios
3. **Interactive Mode** - User can manually input parking requests
4. **Integration Test** - Tests system components together
5. **Exit** - Clean shutdown

---

## BUILD INSTRUCTIONS FOR CHATGPT/DEEPSEEK

To complete your project, provide them with:

1. **PROJECT_SUMMARY.md** - Overview and architecture
2. **TECHNICAL_SPECIFICATION.md** - Detailed algorithms and data structures
3. **SOURCE_CODE_REFERENCE.md** (this file) - All header files
4. **Request:** "Implement the missing C++ implementation files in src/ folder, specifically:
   - ParkingSystem.cpp
   - AllocationEngine.cpp
   - PathFinder.cpp (with Dijkstra's algorithm)
   - RollbackManager.cpp
   - And other implementation files
   
   Also, add an HTTP server (using Crow or similar) that exposes the REST API endpoints specified in TECHNICAL_SPECIFICATION.md"

---

## FILES TO COMPLETE

Missing implementations that need to be done:

### In backend/src/:
- [ ] ParkingSystem.cpp - Main system controller
- [ ] AllocationEngine.cpp - Smart allocation with pathfinding
- [ ] PathFinder.cpp - Dijkstra's shortest path
- [ ] RollbackManager.cpp - Undo stack implementation
- [ ] Zone.cpp - Zone with graph connections
- [ ] Vehicle.cpp - Vehicle type and pricing
- [ ] ParkingRequest.cpp - State machine
- [ ] ParkingArea.cpp - Area management
- [ ] ParkingSlot.cpp - Individual slot operations
- [ ] Analytics.cpp - Statistics and reporting

### Additional Files Needed:
- [ ] HTTP Server integration (main_server.cpp or similar)
- [ ] REST API handlers
- [ ] CMakeLists.txt or Makefile for build

---

## PROJECT STATISTICS

- **Total Classes:** 10 core classes + helper structs
- **Total Header Files:** 10
- **Implementation Files Needed:** 10
- **Frontend Pages:** 1 (index.html with 1909 lines)
- **Algorithms:** Dijkstra's shortest path, smart allocation, state machine
- **Custom Data Structures:** Linked lists, stack, min-heap, adjacency list
- **Test Files:** 9 test suites

---

This document provides complete reference material for continuing development with any AI assistant.
