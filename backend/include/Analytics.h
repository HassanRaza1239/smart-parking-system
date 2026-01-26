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
    
    // Core analytics
    double getAverageParkingDuration() const;
    double getZoneUtilizationRate(const std::string& zoneId) const;
    std::vector<double> getAllZoneUtilizationRates() const;
    
    // Request statistics
    int getCompletedRequests() const;
    int getCancelledRequests() const;
    double getCompletionRate() const;
    double getCancellationRate() const;
    
    // Peak usage
    std::string getPeakUsageZone() const;
    std::vector<std::string> getPeakUsageZones(int topN = 3) const;
    
    // Revenue analytics
    double getTotalRevenue() const;
    double getAverageRevenuePerHour() const;
    double getRevenueByZone(const std::string& zoneId) const;
    
    // Cross-zone statistics
    int getCrossZoneAllocations() const;
    double getCrossZoneRate() const;
    
    // Display
    void displaySummary() const;
    void displayDetailedReport() const;
    
    // Export
    std::string generateReport() const;
};

#endif