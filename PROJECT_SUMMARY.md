# NexusPark - Smart Parking System - Complete Project Overview

## PROJECT DESCRIPTION
**Name:** NexusPark - Smart Parking System  
**Type:** DSA Semester Project  
**Language:** C++ (Backend), HTML/JavaScript (Frontend)  
**Purpose:** Smart parking allocation & zone management system with pathfinding algorithms

---

## PROJECT STRUCTURE

```
MyParkingProject/
├── README.md
├── simple_check.cpp
├── test.cpp
├── backend/
│   ├── main.cpp
│   ├── test_allocator.cpp
│   ├── test_analytics.cpp
│   ├── test_parking_area.cpp
│   ├── test_parking_slot.cpp
│   ├── test_pathfinder.cpp
│   ├── test_request.cpp
│   ├── test_rollback.cpp
│   ├── test_vehicle.cpp
│   ├── test_zone.cpp
│   ├── include/
│   │   ├── AllocationEngine.h
│   │   ├── Analytics.h
│   │   ├── ParkingArea.h
│   │   ├── ParkingRequest.h
│   │   ├── ParkingSlot.h
│   │   ├── ParkingSystem.h
│   │   ├── PathFinder.h
│   │   ├── RollbackManager.h
│   │   ├── Vehicle.h
│   │   └── Zone.h
│   ├── src/
│   │   ├── AllocationEngine.cpp
│   │   ├── Analytics.cpp
│   │   ├── ParkingArea.cpp
│   │   ├── ParkingRequest.cpp
│   │   ├── ParkingSlot.cpp
│   │   ├── ParkingSystem.cpp
│   │   ├── PathFinder.cpp
│   │   ├── RollbackManager.cpp
│   │   ├── Vehicle.cpp
│   │   └── Zone.cpp
│   └── frontend/
│       └── index.html
├── frontend/
│   └── index.html
├── build/
└── docs/
```

---

## CORE COMPONENTS

### 1. **ParkingSystem.h** (Main Controller)
Orchestrates the entire parking system.

```cpp
class ParkingSystem {
private:
    Zone** zones;                           // Array of parking zones
    std::vector<Vehicle*> vehicles;         // Registered vehicles
    std::vector<ParkingRequest*> requests;  // All parking requests
    AllocationEngine* allocator;            // Handles slot allocation
    RollbackManager* rollbackManager;       // Manages undo operations
    Analytics* analytics;                   // Tracks statistics

public:
    // Setup
    void initializeZones();
    void addZone(Zone* zone);
    void addVehicle(Vehicle* vehicle);
    
    // Core Operations
    std::string requestParking(const std::string& vehicleId, 
                              const std::string& preferredZone,
                              int durationHours);
    bool cancelParking(const std::string& requestId);
    bool occupyParking(const std::string& requestId);
    bool releaseParking(const std::string& requestId);
    
    // Rollback
    bool undoLastOperation();
    bool undoOperations(int steps);
    
    // Analytics
    void displayAnalytics() const;
    void displayZoneStatus() const;
    void displayAllRequests() const;
    
    // Metrics
    int getTotalAvailableSlots() const;
    int getTotalCapacity() const;
    double getOverallUtilization() const;
    
    // Testing
    void runDemo();
};
```

---

### 2. **Zone.h** (Graph Node - Parking Zone)
Represents a parking zone with connections to other zones (Graph Structure).

```cpp
struct ZoneConnection {
    std::string connectedZoneId;
    int distance;                   // Distance in meters
    double penaltyMultiplier;       // Cost multiplier (typically 1.5 for cross-zone)
    ZoneConnection* next;           // Linked list node
};

class Zone {
private:
    std::string zoneId;             // ID (e.g., "ZA", "ZB")
    std::string zoneName;
    int totalSlots;
    int availableSlots;
    double hourlyRate;
    ParkingArea* areaList;          // Linked list of parking areas
    int areaCount;
    ZoneConnection* connections;    // Adjacency list (graph edges)

public:
    // Graph operations
    void addConnection(ZoneConnection* connection);
    ZoneConnection* getConnections() const;
    bool isConnectedTo(const std::string& zoneId) const;
    int getDistanceTo(const std::string& zoneId) const;
    
    // Area & Slot management
    bool addArea(ParkingArea* area);
    ParkingArea* findAreaWithAvailableSlot() const;
    ParkingSlot* findAvailableSlot() const;
    
    // Slot operations
    bool allocateSlot();
    bool releaseSlot();
    
    // Getters
    std::string getZoneId() const;
    std::string getZoneName() const;
    int getAvailableSlots() const;
    int getTotalSlots() const;
    double getHourlyRate() const;
    double getUtilizationRate() const;
};
```

---

### 3. **ParkingArea.h** (Area within Zone)
Groups parking slots within a zone.

```cpp
struct SlotNode {
    ParkingSlot* slot;
    SlotNode* next;              // Linked list node
};

class ParkingArea {
private:
    std::string areaId;
    std::string zoneId;
    int capacity;
    int availableSlots;
    SlotNode* slotList;          // Linked list of slots

public:
    // Slot management
    bool addSlot(ParkingSlot* slot);
    ParkingSlot* findAvailableSlot() const;
    ParkingSlot* getSlotById(const std::string& slotId) const;
    
    // Metrics
    int getAvailableCount() const;
    int getTotalCapacity() const;
    double getUtilizationRate() const;
    
    // Getters
    std::string getAreaId() const;
    std::string getZoneId() const;
};
```

---

### 4. **ParkingSlot.h** (Individual Parking Spot)
Represents a single parking space.

```cpp
class ParkingSlot {
private:
    std::string slotId;           // e.g., "ZA-1", "ZA-2"
    std::string zoneId;
    std::string areaId;
    bool isAvailable;
    std::string vehicleId;        // Vehicle currently using slot
    double hourlyRate;

public:
    // Slot management
    bool allocateSlot(const std::string& vehicleId);
    bool releaseSlot();
    bool canBeAllocated() const;
    
    // Getters
    std::string getSlotId() const;
    std::string getZoneId() const;
    std::string getAreaId() const;
    bool getIsAvailable() const;
    std::string getVehicleId() const;
    double getHourlyRate() const;
    
    // Pricing
    double calculateCost(int hours) const;
};
```

---

### 5. **Vehicle.h** (User's Vehicle)
Stores vehicle information and determines pricing.

```cpp
enum VehicleType {
    CAR,                          // 1.0x multiplier
    BIKE,                         // 0.5x multiplier
    TRUCK                         // 2.0x multiplier
};

class Vehicle {
private:
    std::string vehicleId;
    std::string preferredZone;
    VehicleType type;

public:
    // Getters
    std::string getVehicleId() const;
    std::string getPreferredZone() const;
    VehicleType getVehicleType() const;
    std::string getVehicleTypeString() const;
    double getTypeMultiplier() const;  // Pricing multiplier
};
```

---

### 6. **ParkingRequest.h** (State Machine)
Manages parking request lifecycle with strict state transitions.

```cpp
enum RequestState {
    REQUESTED,                    // Initial state
    ALLOCATED,                    // Slot assigned
    OCCUPIED,                     // Vehicle occupying slot
    RELEASED,                     // Parking concluded
    CANCELLED                     // Request cancelled
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
    bool isCrossZone;             // True if allocated to different zone

public:
    // State transitions (strict state machine)
    bool allocate(const std::string& zone, const std::string& slot, 
                 double cost, bool crossZone = false);
    bool occupy();
    bool release();
    bool cancel();
    
    // State validation
    bool isValidTransition(RequestState newState) const;
    
    // Getters
    std::string getRequestId() const;
    std::string getVehicleId() const;
    std::string getPreferredZone() const;
    std::string getAllocatedZone() const;
    std::string getSlotId() const;
    RequestState getState() const;
    std::string getStateString() const;
    double getTotalCost() const;
    bool getIsCrossZone() const;
};
```

---

### 7. **AllocationEngine.h** (Smart Allocation Algorithm)
Allocates parking slots intelligently, considers cross-zone allocation.

```cpp
class AllocationEngine {
private:
    Zone** zones;
    int zoneCount;
    PathFinder* pathFinder;       // Uses pathfinding for optimization
    
    std::string findAvailableSlotInZone(const std::string& zoneId);
    double calculateCost(const std::string& zoneId, int hours, 
                        bool crossZone, Vehicle* vehicle) const;

public:
    // Main allocation
    bool allocateParking(
        ParkingRequest* request,
        Vehicle* vehicle,
        int durationHours,
        std::string& allocatedZone,
        std::string& allocatedSlot,
        std::vector<std::string>& optimalPath,
        double& totalCost
    );
    
    // Find nearest zone with available slots
    std::string findNearestAvailableZone(
        const std::string& preferredZone,
        std::vector<std::string>& path
    );
    
    // Metrics
    int getTotalAvailableSlots() const;
    int getTotalCapacity() const;
    double getOverallUtilization() const;
    void displayZoneStatus() const;
};
```

---

### 8. **PathFinder.h** (Dijkstra's Algorithm)
Implements Dijkstra's shortest path algorithm for zone pathfinding.

```cpp
struct PathNode {
    std::string zoneId;
    int distance;
    std::string previousZone;
};

class MinHeap {
    // Custom min-heap implementation for Dijkstra's algorithm
    PathNode** heap;
    int heapSize;
    int capacity;
    
    void heapifyUp(int index);
    void heapifyDown(int index);
    
public:
    void push(PathNode* node);
    PathNode* pop();
    bool isEmpty() const;
    void updateDistance(const std::string& zoneId, int newDistance, 
                       const std::string& previous);
};

class PathFinder {
private:
    MinHeap* priorityQueue;

public:
    // Dijkstra's algorithm - finds shortest path between zones
    std::vector<std::string> findShortestPath(
        Zone* startZone,
        Zone* targetZone,
        Zone** allZones,
        int zoneCount
    );
    
    int calculateDistance(const std::vector<std::string>& path, 
                         Zone** allZones, int zoneCount);
    
    std::string findNearestAvailableZone(
        const std::string& startZoneId,
        Zone** allZones,
        int zoneCount,
        std::vector<std::string>& path
    );
};
```

---

### 9. **RollbackManager.h** (Undo Stack)
Implements custom stack for undoing operations.

```cpp
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
    RollbackOperation* next;      // Stack node
};

class RollbackManager {
private:
    RollbackOperation* undoStack;
    int maxUndoSteps;
    int currentSteps;

public:
    // Stack operations
    void pushOperation(RollbackOperation* op);
    RollbackOperation* popOperation();
    void clearStack();
    
    // Rollback execution
    bool undoLastOperation(ParkingRequest** requests, int requestCount, 
                          Zone** zones, int zoneCount);
    bool undoMultipleOperations(int steps, ParkingRequest** requests, 
                               int requestCount, Zone** zones, int zoneCount);
    
    // Utility
    bool canUndo() const;
    int getAvailableUndoSteps() const;
    int getOperationCount() const;
};
```

---

### 10. **Analytics.h** (Statistics & Reporting)
Provides comprehensive analytics about the parking system.

```cpp
class Analytics {
private:
    ParkingRequest** requests;
    int requestCount;
    Zone** zones;
    int zoneCount;

public:
    // Request statistics
    double getAverageParkingDuration() const;
    int getCompletedRequests() const;
    int getCancelledRequests() const;
    double getCompletionRate() const;
    double getCancellationRate() const;
    
    // Zone utilization
    double getZoneUtilizationRate(const std::string& zoneId) const;
    std::vector<double> getAllZoneUtilizationRates() const;
    
    // Peak usage
    std::string getPeakUsageZone() const;
    std::vector<std::string> getPeakUsageZones(int topN = 3) const;
    
    // Revenue
    double getTotalRevenue() const;
    double getAverageRevenuePerHour() const;
    double getRevenueByZone(const std::string& zoneId) const;
    
    // Cross-zone
    int getCrossZoneAllocations() const;
    double getCrossZoneRate() const;
    
    // Display & Export
    void displaySummary() const;
    void displayDetailedReport() const;
    std::string generateReport() const;
};
```

---

## DATA STRUCTURES USED

| Component | Data Structure | Purpose |
|-----------|---|---|
| **Zones** | Dynamic Array (Zone**) | Store all parking zones |
| **Zone Connections** | Adjacency List (ZoneConnection*) | Graph representation of zone network |
| **Parking Areas** | Linked List (SlotNode*) | Link parking slots within a zone |
| **Parking Slots** | Linked List (SlotNode*) | Chain slots in parking area |
| **Vehicles** | Vector<Vehicle*> | Store registered vehicles |
| **Requests** | Vector<ParkingRequest*> | Track all parking requests |
| **Rollback Stack** | Custom Stack (RollbackOperation*) | Undo operations |
| **Dijkstra's Algorithm** | Min-Heap (MinHeap) | Find shortest path between zones |

---

## ALGORITHMS IMPLEMENTED

### 1. **Dijkstra's Shortest Path Algorithm**
- Used in `PathFinder` class
- Finds optimal route between parking zones
- Considers distance weights between zones
- Time Complexity: O(V + E log V) using min-heap

### 2. **Smart Parking Allocation**
- In `AllocationEngine` class
- Primary allocation: Preferred zone
- Secondary allocation: Cross-zone with penalty multiplier (1.5x)
- Cost calculation factors:
  - Base hourly rate of zone
  - Duration in hours
  - Vehicle type multiplier (Car: 1.0x, Bike: 0.5x, Truck: 2.0x)
  - Cross-zone penalty (1.5x if different zone)

### 3. **State Machine (Strict State Transitions)**
- In `ParkingRequest` class
- Valid transitions: REQUESTED → ALLOCATED → OCCUPIED → RELEASED
- Allows cancellation from REQUESTED or ALLOCATED states
- Prevents invalid state transitions

### 4. **Custom Stack for Rollback**
- In `RollbackManager` class
- LIFO (Last In First Out) structure
- Supports unlimited undo depth (configurable max steps)
- Restores zone slots and request states

---

## FRONTEND STATUS (NOT INTEGRATED)

### Current Frontend Features
- Interactive map using Leaflet.js
- Search parking functionality
- Request parking form
- Mock pathfinding visualization
- Zone statistics dashboard
- Progress tracking

### Missing Integration
❌ **NO BACKEND API CALLS** - Uses mock data only  
❌ **NO HTTP SERVER** - C++ backend has no web server  
❌ **NO DATABASE** - All data is in-memory  
❌ **NO REAL-TIME SYNC** - Frontend and backend not connected  

### What Needs to Be Done
1. Add HTTP Server to C++ backend (Crow, Beast, or similar)
2. Create REST API endpoints for:
   - `/api/zones` - Get all zones and status
   - `/api/request-parking` - Submit parking request
   - `/api/cancel-parking` - Cancel request
   - `/api/analytics` - Get system statistics
   - `/api/available-slots` - Get available parking spots
3. Update frontend to call actual API instead of mock data
4. Add WebSocket for real-time updates (optional)

---

## TEST FILES AVAILABLE

| File | Purpose |
|------|---------|
| `test_allocator.cpp` | Test allocation engine |
| `test_analytics.cpp` | Test analytics calculations |
| `test_parking_area.cpp` | Test parking area functionality |
| `test_parking_slot.cpp` | Test individual slot operations |
| `test_pathfinder.cpp` | Test Dijkstra's algorithm |
| `test_request.cpp` | Test state machine |
| `test_rollback.cpp` | Test undo functionality |
| `test_vehicle.cpp` | Test vehicle type pricing |
| `test_zone.cpp` | Test zone management |

---

## MAIN FEATURES

✅ **Smart Parking Allocation** - Intelligent slot assignment with cross-zone support  
✅ **Dijkstra's Algorithm** - Optimal pathfinding between zones  
✅ **Dynamic Pricing** - Based on zone, vehicle type, and cross-zone allocation  
✅ **State Machine** - Strict parking request lifecycle management  
✅ **Rollback System** - Undo operations with custom stack  
✅ **Analytics** - Comprehensive system statistics and reporting  
✅ **Graph Structure** - Zone network with distance-weighted edges  
✅ **Custom Data Structures** - Linked lists, stacks, heaps, graphs  
✅ **Interactive UI** - Map-based parking interface (frontend)  

---

## HOW TO BUILD & RUN

### Build
```bash
cd backend
g++ -std=c++17 -o main main.cpp src/*.cpp -I.
```

### Run
```bash
./main
```

### Menu Options
1. Run Full Demo (shows complete system operation)
2. Run Test Cases (runs all test scenarios)
3. Interactive Mode (manual input)
4. Run Integration Test
5. Exit

---

## NEXT STEPS FOR COMPLETION

### Priority 1: Backend-Frontend Integration
- [ ] Add HTTP server to C++ backend
- [ ] Create REST API endpoints
- [ ] Update frontend to use real API

### Priority 2: Database
- [ ] Add database (SQLite, PostgreSQL)
- [ ] Persist parking requests and zones
- [ ] User authentication

### Priority 3: Enhancements
- [ ] Real-time slot availability updates
- [ ] Email/SMS notifications
- [ ] Payment integration
- [ ] Mobile app version

---

## ARCHITECTURE DIAGRAM

```
┌─────────────────────────────────────────────────────────────┐
│                    NEXUSPARK SYSTEM                          │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌──────────────────────────────────────────────────────┐   │
│  │          FRONTEND (HTML/JavaScript)                  │   │
│  │  - Interactive Map (Leaflet.js)                     │   │
│  │  - Parking Request Form                            │   │
│  │  - Analytics Dashboard                             │   │
│  │  - Zone Status Display                             │   │
│  └──────────────────────────────────────────────────────┘   │
│                        ↓ (API Calls)                         │
│  ┌──────────────────────────────────────────────────────┐   │
│  │      C++ BACKEND (HTTP Server needed)               │   │
│  └──────────────────────────────────────────────────────┘   │
│                        ↓                                      │
│  ┌──────────────────────────────────────────────────────┐   │
│  │            CORE LOGIC LAYER                         │   │
│  │  ┌────────────────────────────────────────────────┐ │   │
│  │  │      ParkingSystem (Main Controller)          │ │   │
│  │  └────────────────────────────────────────────────┘ │   │
│  └──────────────────────────────────────────────────────┘   │
│                        ↓                                      │
│  ┌──────────────────────────────────────────────────────┐   │
│  │        ALGORITHM & MANAGER LAYER                   │   │
│  │  ┌────────────────┬──────────────┬──────────────┐  │   │
│  │  │ AllocationEng. │ PathFinder   │ RollbackMgr  │  │   │
│  │  │ (Smart Alloc)  │ (Dijkstra)   │ (Undo Stack) │  │   │
│  │  └────────────────┴──────────────┴──────────────┘  │   │
│  └──────────────────────────────────────────────────────┘   │
│                        ↓                                      │
│  ┌──────────────────────────────────────────────────────┐   │
│  │         DATA MODEL LAYER                            │   │
│  │  ┌─────────────────────────────────────────────────┐ │   │
│  │  │ Zone (Graph) → ParkingArea → ParkingSlot       │ │   │
│  │  │ Vehicle → ParkingRequest (State Machine)       │ │   │
│  │  │ Analytics (Statistics)                         │ │   │
│  │  └─────────────────────────────────────────────────┘ │   │
│  └──────────────────────────────────────────────────────┘   │
│                        ↓                                      │
│  ┌──────────────────────────────────────────────────────┐   │
│  │         IN-MEMORY STORAGE (No DB yet)               │   │
│  │  - Zone Array                                       │   │
│  │  - Vehicle Vector                                  │   │
│  │  - Request Vector                                  │   │
│  └──────────────────────────────────────────────────────┘   │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

---

## EXAMPLE USAGE FLOW

```
1. User opens frontend → Sees interactive map
2. User searches parking → Frontend makes API call (MISSING)
3. Backend receives request → AllocationEngine processes
4. PathFinder calculates optimal zone using Dijkstra
5. AllocationEngine assigns slot with pricing
6. System creates ParkingRequest with state ALLOCATED
7. Frontend shows results and path on map
8. User can cancel (updates state to CANCELLED)
9. System can undo with RollbackManager
10. Analytics tracks all metrics
```

---

**This comprehensive project needs frontend-backend integration via HTTP API to be fully functional.**
