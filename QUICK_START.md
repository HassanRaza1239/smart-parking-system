# QUICK START GUIDE FOR AI ASSISTANTS

## What is This Project?

**NexusPark** - A smart parking allocation system that:
- Manages multiple parking zones with interconnected network
- Intelligently allocates parking slots based on availability
- Finds optimal routes between zones using Dijkstra's algorithm
- Calculates dynamic pricing based on vehicle type and cross-zone allocation
- Provides rollback/undo functionality
- Tracks comprehensive analytics

---

## Project Status

### ✅ COMPLETED
- All 10 header files with complete class definitions
- Frontend UI with Leaflet.js map interface
- System architecture and design
- Data structure specifications
- Algorithm specifications

### ❌ NOT COMPLETED
- C++ implementation files (src/ folder)
- HTTP server integration
- Frontend-backend API communication
- Database persistence
- Testing and deployment

---

## File Organization

```
MyParkingProject/
├── Documentation (NEW - for AI assistants):
│   ├── PROJECT_SUMMARY.md          ← Start here
│   ├── TECHNICAL_SPECIFICATION.md   ← Implementation details
│   └── SOURCE_CODE_REFERENCE.md     ← All header files
│
├── Backend C++ Code:
│   ├── backend/include/*.h          ← All class definitions (DONE)
│   ├── backend/src/*.cpp            ← Implementation (NEEDED)
│   ├── backend/main.cpp             ← Menu-driven interface
│   └── backend/test_*.cpp           ← Test files
│
├── Frontend:
│   ├── frontend/index.html          ← Interactive UI (DONE, needs API)
│   └── backend/frontend/index.html  ← Alternative UI
│
└── Build/Docs:
    ├── build/                       ← Compiled executables
    └── docs/                        ← Documentation
```

---

## What Each Document Contains

### 1. PROJECT_SUMMARY.md
**For:** Quick overview and understanding
**Contains:**
- Project description and purpose
- Complete component documentation (10 classes)
- Data structures used
- Algorithms implemented
- Frontend status
- Architecture diagram
- Features checklist

### 2. TECHNICAL_SPECIFICATION.md
**For:** Implementation details and algorithms
**Contains:**
- Class relationships and dependencies
- Detailed algorithm breakdowns:
  - Smart Parking Allocation
  - Dijkstra's Shortest Path
  - State Machine
  - Rollback Stack
- Custom data structure implementations
- Pricing model
- Analytics metrics
- Memory management
- REST API specifications
- Testing strategy
- Performance characteristics

### 3. SOURCE_CODE_REFERENCE.md
**For:** All header files and code reference
**Contains:**
- Complete code for all 10 header files
- Frontend file overview
- Build instructions
- Files that need implementation
- Project statistics

---

## Key Concepts

### 1. Zones and Graph
- 5 zones: ZA, ZB, ZC, ZD, ZE
- Zones connected as a graph with distances
- Adjacency list implementation
- Example: ZA ↔ ZB (500m), ZA ↔ ZC (800m)

### 2. Parking Allocation
- User requests parking in preferred zone
- If available → allocate with no penalty
- If full → find nearest zone using Dijkstra's algorithm
- Apply 1.5x cost multiplier for cross-zone allocation

### 3. Vehicle Types
- **CAR:** 1.0x price multiplier
- **BIKE:** 0.5x price multiplier (cheaper)
- **TRUCK:** 2.0x price multiplier (expensive)

### 4. State Machine
```
REQUESTED → ALLOCATED → OCCUPIED → RELEASED
    ↓_______________↓_____________↓
             CANCELLED
```

### 5. Cost Formula
```
Total Cost = HourlyRate × Duration × VehicleTypeMultiplier × CrossZoneMultiplier
Example: $5/hr × 3 hours × 1.0 (car) × 1.5 (cross-zone) = $22.50
```

---

## Instructions for AI Assistant

### To Complete the Project:

```
You have a parking system with complete architecture and headers.
Here's what needs to be implemented:

1. **Implement all C++ files in backend/src/:**
   - Refer to PROJECT_SUMMARY.md for class documentation
   - Refer to TECHNICAL_SPECIFICATION.md for algorithm details
   - Refer to SOURCE_CODE_REFERENCE.md for header files

2. **Implement Dijkstra's Algorithm:**
   - File: PathFinder.cpp
   - Implements MinHeap class
   - Finds shortest path between zones
   - Time complexity: O(V + E log V)

3. **Implement Smart Allocation:**
   - File: AllocationEngine.cpp
   - Primary: allocate in preferred zone
   - Secondary: find nearest zone using PathFinder
   - Apply cost multipliers

4. **Implement Rollback Stack:**
   - File: RollbackManager.cpp
   - LIFO structure for undo operations
   - Restore zone slots and request states

5. **Add HTTP Server:**
   - Create REST API endpoints (see TECHNICAL_SPECIFICATION.md)
   - Connect C++ backend to frontend
   - Support these endpoints:
     - GET /api/zones
     - POST /api/request-parking
     - POST /api/cancel-parking
     - GET /api/analytics
     - POST /api/undo

6. **Update Frontend:**
   - Replace mock data with real API calls
   - Connect to backend HTTP server
   - Update visualization for real data
```

### Key Requirements:

- **Language:** C++17
- **Build Tool:** CMake or Makefile
- **HTTP Library:** Crow, Beast, or similar
- **No External Database Needed** (in-memory is fine for now)
- **Must Implement:** All algorithms exactly as specified

---

## Testing the System

After implementation, you can test:

```cpp
// Create parking system
ParkingSystem system;

// Request parking
std::string reqId = system.requestParking("CAR001", "ZA", 3);

// Get zone status
system.displayZoneStatus();

// Display analytics
system.displayAnalytics();

// Undo operation
system.undoLastOperation();
```

---

## Most Important Algorithm

### Dijkstra's Shortest Path Algorithm

**Purpose:** Find the nearest parking zone when preferred zone is full

**Input:** Start zone, all zones with connections

**Output:** Shortest path with total distance

**Time Complexity:** O(V + E log V) - Very efficient

**Implementation:**
1. Use MinHeap with zones sorted by distance
2. Mark visited zones
3. Relax edges (update distances)
4. Reconstruct path from target back to start

**Example:**
```
Zones: ZA, ZB, ZC, ZD, ZE
Find nearest to ZA: 
  ZA (0m) → ZB (500m) → ZC (800m) → ZE (1200m)
```

---

## Code Quality Guidelines

- Follow C++17 standards
- Use proper memory management (no leaks)
- Implement all methods from headers
- Add error handling for edge cases
- Write clear comments for complex logic
- Test against provided test files

---

## Common Pitfalls to Avoid

❌ **Memory Leaks**
- Don't forget to delete dynamically allocated objects
- Use destructors properly

❌ **State Machine Violations**
- Only allow valid state transitions
- Check isValidTransition() before state changes

❌ **Graph Traversal**
- Handle all edge cases in Dijkstra's algorithm
- Properly handle disconnected zones

❌ **Pricing Calculation**
- Apply all multipliers correctly
- Track cross-zone status

❌ **Stack Implementation**
- Ensure proper LIFO order for rollback
- Don't lose operation history

---

## Performance Targets

| Operation | Target | Note |
|-----------|--------|------|
| Allocate Parking | < 100ms | With 5 zones |
| Find Path | < 10ms | Dijkstra's algorithm |
| Cancel Parking | < 1ms | Direct state change |
| Undo | < 1ms | Pop from stack |
| Get Analytics | < 50ms | For ~100 requests |

---

## Files to Provide to AI Assistant

1. ✅ PROJECT_SUMMARY.md
2. ✅ TECHNICAL_SPECIFICATION.md
3. ✅ SOURCE_CODE_REFERENCE.md
4. ✅ This file (QUICK_START.md)
5. ✅ All header files from backend/include/
6. ✅ frontend/index.html

---

## Expected Deliverables

After completion, you should have:

1. **Compiled executable** that runs without errors
2. **All 10 C++ implementation files** in src/
3. **HTTP server** running on localhost:8080 (or configurable port)
4. **Working API endpoints** for all operations
5. **Frontend connected to backend** with real data
6. **All tests passing** from test_*.cpp files
7. **Documentation** updated with any changes

---

## Quick Command Reference

```bash
# Build the project
g++ -std=c++17 -o main backend/main.cpp backend/src/*.cpp -I backend

# Run with full demo
./main
# Select option 1

# Run tests
./main
# Select option 2

# Interactive mode
./main
# Select option 3
```

---

## Support Information

**If implementing with ChatGPT/DeepSeek:**

1. Share all 4 documentation files
2. Request implementation section by section
3. Start with data structures (Zone.cpp, Vehicle.cpp)
4. Then algorithms (PathFinder.cpp, AllocationEngine.cpp)
5. Finally integration (HTTP server)
6. Test at each stage

**Debugging Tips:**
- Use displayZoneStatus() frequently
- Check logs from Analytics
- Verify state transitions
- Test with small datasets first

---

## Next Steps

1. **Immediate:** Share these 4 files with AI assistant
2. **Then:** Request implementation of backend/src/ files
3. **After:** Request HTTP server integration
4. **Finally:** Test complete system end-to-end

---

**You now have everything needed to complete this project with any AI assistant!**

---

**Version:** 1.0  
**Last Updated:** January 27, 2026  
**Project:** NexusPark - Smart Parking System  
**Status:** Awaiting Backend Implementation
