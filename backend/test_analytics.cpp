#include "include/Analytics.h"
#include "include/ParkingRequest.h"
#include "include/Zone.h"
#include <iostream>

int main() {
    std::cout << "=== Testing Analytics Engine ===\n" << std::endl;
    
    // Create test data
    std::cout << "Test 1: Setting up test data..." << std::endl;
    
    // Create zones
    Zone* zoneA = new Zone("ZA", "Zone A", 10, 5.0);
    Zone* zoneB = new Zone("ZB", "Zone B", 8, 6.0);
    Zone* zoneC = new Zone("ZC", "Zone C", 12, 4.5);
    
    // Create parking requests with different states
    ParkingRequest* requests[10];
    
    // Completed requests
    requests[0] = new ParkingRequest("REQ001", "CAR001", "ZA");
    requests[0]->allocate("ZA", "A-101", 15.0, false);
    requests[0]->occupy();
    requests[0]->release();
    
    requests[1] = new ParkingRequest("REQ002", "CAR002", "ZA");
    requests[1]->allocate("ZA", "A-102", 12.0, false);
    requests[1]->occupy();
    requests[1]->release();
    
    requests[2] = new ParkingRequest("REQ003", "BIKE001", "ZB");
    requests[2]->allocate("ZB", "B-101", 8.4, false); // Bike rate
    requests[2]->occupy();
    requests[2]->release();
    
    // Cross-zone allocation
    requests[3] = new ParkingRequest("REQ004", "CAR003", "ZA");
    requests[3]->allocate("ZB", "B-102", 27.0, true); // Cross-zone with penalty
    requests[3]->occupy();
    requests[3]->release();
    
    // Cancelled requests
    requests[4] = new ParkingRequest("REQ005", "CAR004", "ZA");
    requests[4]->cancel();
    
    requests[5] = new ParkingRequest("REQ006", "CAR005", "ZC");
    requests[5]->allocate("ZC", "C-101", 13.5, false);
    requests[5]->cancel();
    
    // Active/allocated requests
    requests[6] = new ParkingRequest("REQ007", "TRUCK001", "ZB");
    requests[6]->allocate("ZB", "B-103", 45.0, false); // Truck rate
    
    requests[7] = new ParkingRequest("REQ008", "CAR006", "ZC");
    requests[7]->allocate("ZC", "C-102", 9.0, false);
    
    // Just requested
    requests[8] = new ParkingRequest("REQ009", "CAR007", "ZA");
    requests[9] = new ParkingRequest("REQ010", "BIKE002", "ZB");
    
    int requestCount = 10;
    
    // Fill some slots in zones
    for (int i = 0; i < 7; i++) zoneA->allocateSlot(); // 7/10 occupied
    for (int i = 0; i < 5; i++) zoneB->allocateSlot(); // 5/8 occupied
    for (int i = 0; i < 3; i++) zoneC->allocateSlot(); // 3/12 occupied
    
    Zone* zones[] = {zoneA, zoneB, zoneC};
    int zoneCount = 3;
    
    // Create Analytics engine
    std::cout << "\nTest 2: Creating Analytics Engine..." << std::endl;
    Analytics analytics(requests, requestCount, zones, zoneCount);
    
    // Test summary
    std::cout << "\nTest 3: Displaying Summary..." << std::endl;
    analytics.displaySummary();
    
    // Test detailed report
    std::cout << "\nTest 4: Detailed Report..." << std::endl;
    analytics.displayDetailedReport();
    
    // Test individual metrics
    std::cout << "\nTest 5: Individual Metrics..." << std::endl;
    std::cout << "Average parking duration: " 
              << analytics.getAverageParkingDuration() << " hours" << std::endl;
    
    std::cout << "Completed requests: " << analytics.getCompletedRequests() << std::endl;
    std::cout << "Cancelled requests: " << analytics.getCancelledRequests() << std::endl;
    std::cout << "Completion rate: " << analytics.getCompletionRate() << "%" << std::endl;
    std::cout << "Cancellation rate: " << analytics.getCancellationRate() << "%" << std::endl;
    
    std::cout << "Total revenue: $" << analytics.getTotalRevenue() << std::endl;
    std::cout << "Average revenue/hour: $" << analytics.getAverageRevenuePerHour() << std::endl;
    
    std::cout << "Cross-zone allocations: " << analytics.getCrossZoneAllocations() << std::endl;
    std::cout << "Cross-zone rate: " << analytics.getCrossZoneRate() << "%" << std::endl;
    
    // Test zone-specific metrics
    std::cout << "\nTest 6: Zone-specific Metrics..." << std::endl;
    std::cout << "Zone A utilization: " << analytics.getZoneUtilizationRate("ZA") << "%" << std::endl;
    std::cout << "Zone B utilization: " << analytics.getZoneUtilizationRate("ZB") << "%" << std::endl;
    std::cout << "Zone C utilization: " << analytics.getZoneUtilizationRate("ZC") << "%" << std::endl;
    
    std::cout << "Zone A revenue: $" << analytics.getRevenueByZone("ZA") << std::endl;
    std::cout << "Zone B revenue: $" << analytics.getRevenueByZone("ZB") << std::endl;
    std::cout << "Zone C revenue: $" << analytics.getRevenueByZone("ZC") << std::endl;
    
    // Test peak usage
    std::cout << "\nTest 7: Peak Usage Analysis..." << std::endl;
    std::cout << "Peak usage zone: " << analytics.getPeakUsageZone() << std::endl;
    
    std::vector<std::string> peakZones = analytics.getPeakUsageZones(3);
    std::cout << "Top 3 busiest zones: ";
    for (size_t i = 0; i < peakZones.size(); i++) {
        std::cout << peakZones[i];
        if (i < peakZones.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    
    // Test report generation
    std::cout << "\nTest 8: Report Generation..." << std::endl;
    std::string report = analytics.generateReport();
    std::cout << report << std::endl;
    
    // Cleanup
    std::cout << "\nTest 9: Cleanup..." << std::endl;
    delete zoneA;
    delete zoneB;
    delete zoneC;
    
    for (int i = 0; i < requestCount; i++) {
        delete requests[i];
    }
    
    std::cout << "\n=== All Analytics Tests Complete! ===" << std::endl;
    return 0;
}