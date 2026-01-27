# TECHNICAL IMPLEMENTATION DETAILS

## CLASS RELATIONSHIPS & DEPENDENCIES

```
ParkingSystem (Main Controller)
    ├── Zone** (Array of zones)
    │   ├── ParkingArea* (Linked list of areas)
    │   │   └── ParkingSlot (Individual slots)
    │   └── ZoneConnection* (Graph edges - adjacency list)
    ├── Vehicle* (Vector)
    ├── ParkingRequest* (Vector)
    │   └── RequestState (Enum - state machine)
    ├── AllocationEngine*
    │   ├── Zone** (Reference)
    │   └── PathFinder*
    │       ├── MinHeap* (Priority queue)
    │       └── PathNode (Struct)
    ├── RollbackManager*
    │   └── RollbackOperation* (Stack)
    └── Analytics*
        ├── ParkingRequest** (Reference)
        └── Zone** (Reference)
```

---

## DETAILED ALGORITHM BREAKDOWN

### 1. SMART PARKING ALLOCATION ALGORITHM

**Location:** `AllocationEngine::allocateParking()`

```
Algorithm: SmartParkingAllocation
Input: 
    - ParkingRequest (contains vehicleId, preferredZone)
    - Vehicle (contains type and preferred zone)
    - Duration in hours

Process:
    1. Try to allocate in preferred zone:
       - Call findAvailableSlotInZone(preferredZone)
       - If slot found:
           ✓ Mark as ALLOCATED
           ✓ Set isCrossZone = false
           ✓ Calculate cost WITHOUT penalty
           ✓ Return success
       
    2. If preferred zone full, try nearest zones:
       - Call PathFinder::findNearestAvailableZone()
       - Get path to nearest zone with availability
       - If zone found:
           ✓ Mark as ALLOCATED
           ✓ Set isCrossZone = true
           ✓ Calculate cost WITH 1.5x penalty multiplier
           ✓ Return success
       
    3. If all zones full:
       ✗ Return failure

Cost Calculation Formula:
    totalCost = baseHourlyRate × duration × vehicleTypeMultiplier × (isCrossZone ? 1.5 : 1.0)
    
    Example: Zone hourly rate = $5, duration = 3 hours, vehicle = CAR
    → Cost = 5 × 3 × 1.0 × 1.0 = $15 (same zone)
    → Cost = 5 × 3 × 1.0 × 1.5 = $22.50 (cross-zone)

Time Complexity: O(Z × S) where Z = zones, S = slots per zone
Space Complexity: O(Z) for path storage
```

---

### 2. DIJKSTRA'S SHORTEST PATH ALGORITHM

**Location:** `PathFinder::findShortestPath()`

```
Algorithm: Dijkstra
Input:
    - Start Zone
    - Target Zone
    - All Zones array
    - Zone connections (with distances)

Data Structure Used: MinHeap (custom implementation)

Process:
    1. Initialize:
       - Create MinHeap with all zones
       - Set distances: startZone = 0, others = INFINITY
       - Create visited array
    
    2. Main Loop:
       while heap not empty:
           currentNode = heap.pop()  // Get minimum distance node
           
           if visited[currentNode]:
               continue
           
           mark currentNode as visited
           
           for each connected zone:
               newDistance = currentNode.distance + edgeWeight
               
               if newDistance < neighbor.distance:
                   neighbor.distance = newDistance
                   neighbor.previousZone = currentNode
                   heap.push(neighbor)  // Update in heap
    
    3. Reconstruct path:
       - Start from targetZone
       - Follow previousZone pointers back to startZone
       - Reverse to get forward path

Time Complexity: O(V + E log V) = O(Z + C log Z)
    where V = vertices (zones), E = edges (connections)
    In typical parking system: ~5-10 zones, O(log Z) is very fast

Space Complexity: O(V + E) = O(Z + C) for heap and graph storage

Example with 5 zones:
    Zones: ZA, ZB, ZC, ZD, ZE
    Connections (bi-directional):
        ZA-ZB: 500m, ZA-ZC: 800m
        ZB-ZC: 300m, ZB-ZD: 700m
        ZC-ZE: 400m, ZD-ZE: 600m
    
    Find shortest path from ZA to ZE:
    Distance: 0
    | Dist | Visit | Process
    |------|-------|------------------
    | ZA:0 | V     | Start
    | ZB:500 | | Process ZB
    | ZC:800 | | Relax ZC via ZB (300+500=800, no change)
    | ZD:1200 | | Process ZC
    | ZE:1200 | V | Relax ZE (800+400=1200)
    
    Path: ZA → ZB → ZC → ZE (1200m)
```

---

### 3. STATE MACHINE FOR PARKING REQUEST

**Location:** `ParkingRequest` class

```
State Diagram:

                    ┌─────────────────┐
                    │   REQUESTED     │ (Initial State)
                    └────────┬────────┘
                             │
                    allocate()│
                             ↓
                    ┌─────────────────┐       cancel()
    cancel() ─────→ │   ALLOCATED     │ ←─────────────┐
    from here       └────────┬────────┘               │
                             │                       │
                    occupy() │                       │
                             ↓                       │
                    ┌─────────────────┐              │
                    │    OCCUPIED     │              │
                    └────────┬────────┘              │
                             │                       │
                    release()│                       │
                             ↓                       │
                    ┌─────────────────┐      ┌──────────────┐
                    │   RELEASED      │      │  CANCELLED   │
                    │  (Final State)  │      │ (Final State)│
                    └─────────────────┘      └──────────────┘

Valid Transitions:
    REQUESTED → ALLOCATED     (Can be allocated in preferred zone)
    REQUESTED → CANCELLED     (User cancels before allocation)
    ALLOCATED → OCCUPIED      (User arrives and occupies spot)
    ALLOCATED → CANCELLED     (User cancels after allocation)
    OCCUPIED → RELEASED       (User leaves the spot)
    
Invalid Transitions (Prevented by isValidTransition()):
    RELEASED → OCCUPIED       (Can't re-occupy released spot)
    CANCELLED → *             (Can't transition from cancelled)
    OCCUPIED → ALLOCATED      (Can't go back to allocated)
    etc.

Time tracking:
    - requestTime: When request created
    - allocationTime: When slot assigned
    - completionTime: When released or cancelled
```

---

### 4. CUSTOM STACK FOR ROLLBACK

**Location:** `RollbackManager` class

```
Data Structure: Singly-linked list (Stack)

Node Structure:
    struct RollbackOperation {
        RollbackOpType operationType;    // OP_ALLOCATE, OP_RELEASE, OP_CANCEL
        std::string requestId;           // Which request
        std::string zoneId;              // Which zone affected
        std::string slotId;              // Which slot affected
        RequestState previousState;      // State before operation
        RollbackOperation* next;         // Pointer to next (below) in stack
    }

Stack Operations:
    push(operation)          // Add to top (O(1))
    pop()                    // Remove from top (O(1))
    isEmpty()                // Check if empty (O(1))
    getOperationCount()      // Count operations (O(1))

Example Rollback Sequence:

    1. User requests parking ZA
       → Stack: [ALLOCATE|REQ001|ZA|ZA-1] (top)
    
    2. User occupies
       → Stack: [OCCUPY|REQ001|ZA|ZA-1]
               [ALLOCATE|REQ001|ZA|ZA-1]
    
    3. User cancels
       → Stack: [CANCEL|REQ001|ZA|ZA-1]
               [OCCUPY|REQ001|ZA|ZA-1]
               [ALLOCATE|REQ001|ZA|ZA-1]
    
    4. Undo last operation (cancel)
       → pop() retrieves [CANCEL|REQ001|ZA|ZA-1]
       → Restore: req state = OCCUPIED, zone slot available--
       → Stack: [OCCUPY|REQ001|ZA|ZA-1]
               [ALLOCATE|REQ001|ZA|ZA-1]
    
    5. Undo 2 more operations
       → pop() retrieves [OCCUPY...]
       → pop() retrieves [ALLOCATE...]
       → Restore both, return to initial state

Operations Supported:
    OP_ALLOCATE: Reverse → release slot, mark request as REQUESTED
    OP_RELEASE:  Reverse → allocate slot, mark request as OCCUPIED
    OP_CANCEL:   Reverse → restore slot, mark request to pre-cancel state

Time Complexity: All operations O(1)
Space Complexity: O(N) where N = number of operations
```

---

## CUSTOM DATA STRUCTURES

### 1. ADJACENCY LIST (Graph - Zone Connections)

```cpp
// Zone.h
struct ZoneConnection {
    std::string connectedZoneId;     // ID of connected zone
    int distance;                    // Edge weight in meters
    double penaltyMultiplier;        // Cross-zone cost multiplier
    ZoneConnection* next;            // Next connection in list
};

// In Zone class:
ZoneConnection* connections;         // Head of linked list

// Creates graph where:
// ZA.connections → ZB (500m) → ZC (800m) → nullptr
// ZB.connections → ZA (500m) → ZC (300m) → ZD (700m) → nullptr
// etc.

Graph Representation:
    ZA ─→ [ZB:500m] ─→ [ZC:800m] ─→ NULL
    ZB ─→ [ZA:500m] ─→ [ZC:300m] ─→ [ZD:700m] ─→ NULL
    ZC ─→ [ZA:800m] ─→ [ZB:300m] ─→ [ZE:400m] ─→ NULL
    ZD ─→ [ZB:700m] ─→ [ZE:600m] ─→ NULL
    ZE ─→ [ZC:400m] ─→ [ZD:600m] ─→ NULL

Traversal: Loop through connections using next pointer
Time to find connection: O(degree of node), typically O(5)
```

### 2. LINKED LIST (Parking Slots in Area)

```cpp
// ParkingArea.h
struct SlotNode {
    ParkingSlot* slot;
    SlotNode* next;
};

// In ParkingArea class:
SlotNode* slotList;      // Head of linked list

// Example structure for Zone A, Area 1:
slotList → [ZA-1] → [ZA-2] → [ZA-3] → [ZA-4] → ... → NULL

Operations:
    addSlot(slot)          // Insert at head - O(1)
    findAvailableSlot()    // Traverse list - O(S) where S = slots
    getSlotById(id)        // Search - O(S)
    getAvailableCount()    // Count - O(S)

Space Efficiency: Each node = pointer + 1 slot object
```

### 3. CUSTOM MIN-HEAP (Dijkstra's Priority Queue)

```cpp
// PathFinder.h
class MinHeap {
private:
    PathNode** heap;       // Dynamic array of pointers
    int heapSize;
    int capacity;
    
    void heapifyUp(int index)      // Bubble up from leaf
    void heapifyDown(int index)    // Bubble down from root
};

Heap Property: heap[i].distance ≤ heap[2i+1].distance
                                 ≤ heap[2i+2].distance

Example Heap:
            ┌───[0:0]───┐
           /             \
        [1:500]        [2:800]
       /      \        /
    [3:1200] [4:700] [5:1200]

Operations:
    push(node)        // Insert at end, heapifyUp - O(log N)
    pop()             // Remove root, move last to root, heapifyDown - O(log N)
    updateDistance()  // Update node distance, heapifyUp - O(log N)

Total Dijkstra Time: O(E log V) = O(connections × log zones)
```

### 4. DYNAMIC ARRAY (Zones)

```cpp
// ParkingSystem.h
Zone** zones;           // Pointer to pointer (dynamic array)
int zoneCount;

// Allocated in constructor:
zones = new Zone*[MAX_ZONES];

// Access:
zones[0] → ZA
zones[1] → ZB
zones[2] → ZC
etc.

Direct access: O(1)
Space: Fixed at initialization (no resizing)
```

---

## PRICING MODEL

### Cost Calculation Formula

```
Total Cost = Hourly Rate × Duration × Vehicle Type Multiplier × Cross-Zone Multiplier

Vehicle Type Multiplier:
    CAR:   1.0x  (standard parking)
    BIKE:  0.5x  (smaller vehicle, cheaper)
    TRUCK: 2.0x  (larger vehicle, more expensive)

Cross-Zone Multiplier:
    Same Zone: 1.0x
    Different Zone: 1.5x (penalty for longer walk)

Example Calculations:
    Scenario 1: Car, Zone A (preferred), 3 hours, rate=$5/hr
        Cost = 5 × 3 × 1.0 × 1.0 = $15
    
    Scenario 2: Truck, Zone B (cross-zone), 2 hours, rate=$4/hr
        Cost = 4 × 2 × 2.0 × 1.5 = $24
    
    Scenario 3: Bike, Zone C (cross-zone), 4 hours, rate=$5/hr
        Cost = 5 × 4 × 0.5 × 1.5 = $15
```

---

## ANALYTICS METRICS

```
1. Request Statistics:
    - Total Requests: All created requests
    - Completed Requests: State = RELEASED
    - Cancelled Requests: State = CANCELLED
    - Completion Rate = Completed / Total × 100%
    - Cancellation Rate = Cancelled / Total × 100%
    - Average Duration = Sum(duration) / Completed requests

2. Zone Utilization:
    - Zone Utilization = (TotalSlots - AvailableSlots) / TotalSlots × 100%
    - Example: 15 total slots, 5 available
      Utilization = (15 - 5) / 15 = 66.67%

3. Peak Usage:
    - Count requests per zone
    - Sort by request count
    - Return top N zones

4. Revenue:
    - Total Revenue = Sum(totalCost) for all requests
    - Revenue by Zone = Sum(totalCost) where allocatedZone = zone
    - Average Revenue per Hour = Total Revenue / Total Hours Used
    
5. Cross-Zone Statistics:
    - Cross-Zone Allocations = Count where isCrossZone = true
    - Cross-Zone Rate = Cross-Zone Allocations / Total Allocations × 100%
```

---

## MEMORY MANAGEMENT

### Allocation Responsibility Matrix

| Component | Allocated By | Deallocated By | Notes |
|-----------|---|---|---|
| Zone* | ParkingSystem | ~ParkingSystem | Array of pointers |
| ParkingArea* | Zone | ~Zone | Linked list |
| ParkingSlot | ParkingArea | ~ParkingArea | In linked list |
| Vehicle* | ParkingSystem | ~ParkingSystem | Vector auto-deletes |
| ParkingRequest* | ParkingSystem | ~ParkingSystem | Vector auto-deletes |
| AllocationEngine* | ParkingSystem | ~ParkingSystem | Manages PathFinder |
| PathFinder* | AllocationEngine | ~AllocationEngine | Manages MinHeap |
| MinHeap* | PathFinder | ~PathFinder | Manages heap nodes |
| RollbackManager* | ParkingSystem | ~ParkingSystem | Manages operation stack |
| Analytics* | ParkingSystem | ~ParkingSystem | References data |

### Potential Memory Leaks to Avoid

1. Zone connections not deleted
2. Parking area slots not freed
3. Request history not cleaned on exit
4. PathFinder heap nodes not properly freed
5. Rollback stack not cleared

---

## INTEGRATION WITH FRONTEND

### Required REST API Endpoints

```
1. GET /api/zones
   Response: Array of zones with availability
   [
     {
       "zoneId": "ZA",
       "zoneName": "Downtown Parking A",
       "totalSlots": 50,
       "availableSlots": 12,
       "hourlyRate": 5.0,
       "utilization": 76.0,
       "connections": [
         {"zoneId": "ZB", "distance": 500}
       ]
     },
     ...
   ]

2. POST /api/request-parking
   Body: {
     "vehicleId": "CAR001",
     "vehicleType": "CAR",
     "preferredZone": "ZA",
     "durationHours": 3
   }
   Response: {
     "requestId": "REQ001",
     "allocatedZone": "ZA",
     "allocatedSlot": "ZA-1",
     "totalCost": 15.0,
     "isCrossZone": false,
     "path": ["ZA"]
   }

3. POST /api/cancel-parking
   Body: {"requestId": "REQ001"}
   Response: {"success": true, "message": "Parking cancelled"}

4. GET /api/analytics
   Response: {
     "totalRequests": 50,
     "completedRequests": 40,
     "cancelledRequests": 10,
     "completionRate": 80.0,
     "averageDuration": 2.5,
     "totalRevenue": 450.0,
     "peakZone": "ZA",
     "crossZoneRate": 15.0
   }

5. GET /api/request/{requestId}
   Response: Detailed request information including state

6. POST /api/undo
   Body: {"steps": 1}
   Response: {"success": true, "undoneOperations": 1}
```

---

## TESTING STRATEGY

### Test Cases Coverage

```
1. Allocation Engine Tests:
   ✓ Allocate in preferred zone
   ✓ Allocate in nearest zone (cross-zone)
   ✓ Handle full system (no slots available)
   ✓ Pricing calculation for all vehicle types

2. PathFinder Tests:
   ✓ Dijkstra algorithm correctness
   ✓ Shortest path with multiple routes
   ✓ Nearest available zone finding
   ✓ Disconnected zones handling

3. State Machine Tests:
   ✓ Valid state transitions
   ✓ Invalid transition prevention
   ✓ State persistence
   ✓ Time tracking accuracy

4. Rollback Tests:
   ✓ Single undo operation
   ✓ Multiple undo operations
   ✓ Undo limits (max steps)
   ✓ State restoration correctness

5. Analytics Tests:
   ✓ Utilization calculation
   ✓ Revenue calculation
   ✓ Peak zone detection
   ✓ Cross-zone statistics

6. Integration Tests:
   ✓ Full workflow: request → allocate → occupy → release
   ✓ Cancellation handling
   ✓ Multiple simultaneous requests
   ✓ System recovery from errors
```

---

## PERFORMANCE CHARACTERISTICS

| Operation | Time Complexity | Space Complexity | Notes |
|-----------|---|---|---|
| Allocate Parking | O(Z × S) | O(Z) | Z zones, S slots/zone |
| Find Nearest Zone | O(Z + C log Z) | O(Z) | Dijkstra with Z zones, C connections |
| Cancel Parking | O(1) | O(1) | Direct state change |
| Undo Operation | O(1) | O(1) | Pop from stack |
| Get Analytics | O(R) | O(R) | R = total requests |
| Find Zone | O(1) | O(1) | Array direct access |
| Find Slot | O(S) | O(S) | Linear search in linked list |

---

## SCALABILITY CONSIDERATIONS

Current Limitations:
- Fixed zone count (statically allocated)
- In-memory storage (no persistence)
- Linear slot search per zone
- No connection pooling for HTTP

Potential Improvements:
- Dynamic zone allocation
- Database for persistent storage
- Hash table for faster slot lookup
- Caching for frequently accessed zones
- Load balancing for multiple backends

---

This comprehensive technical document should provide everything needed to complete, maintain, or improve the parking system.
