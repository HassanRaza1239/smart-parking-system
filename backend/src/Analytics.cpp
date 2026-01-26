#include "Analytics.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>

// Constructor
Analytics::Analytics(ParkingRequest** reqArray, int reqCount, Zone** zoneArray, int zCount)
    : requests(reqArray), requestCount(reqCount), zones(zoneArray), zoneCount(zCount) {
    std::cout << "Analytics engine initialized with " << reqCount 
              << " requests and " << zCount << " zones" << std::endl;
}

// Get average parking duration
double Analytics::getAverageParkingDuration() const {
    if (requestCount == 0) return 0.0;
    
    double totalDuration = 0.0;
    int completedCount = 0;
    
    for (int i = 0; i < requestCount; i++) {
        if (requests[i]->getState() == RELEASED) {
            totalDuration += requests[i]->getDuration();
            completedCount++;
        }
    }
    
    return completedCount > 0 ? totalDuration / completedCount : 0.0;
}

// Get utilization rate for specific zone
double Analytics::getZoneUtilizationRate(const std::string& zoneId) const {
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i]->getZoneId() == zoneId) {
            return zones[i]->getUtilizationRate();
        }
    }
    return 0.0;
}

// Get utilization rates for all zones
std::vector<double> Analytics::getAllZoneUtilizationRates() const {
    std::vector<double> rates;
    for (int i = 0; i < zoneCount; i++) {
        rates.push_back(zones[i]->getUtilizationRate());
    }
    return rates;
}

// Get completed requests count
int Analytics::getCompletedRequests() const {
    int count = 0;
    for (int i = 0; i < requestCount; i++) {
        if (requests[i]->getState() == RELEASED) {
            count++;
        }
    }
    return count;
}

// Get cancelled requests count
int Analytics::getCancelledRequests() const {
    int count = 0;
    for (int i = 0; i < requestCount; i++) {
        if (requests[i]->getState() == CANCELLED) {
            count++;
        }
    }
    return count;
}

// Get completion rate
double Analytics::getCompletionRate() const {
    if (requestCount == 0) return 0.0;
    return (getCompletedRequests() * 100.0) / requestCount;
}

// Get cancellation rate
double Analytics::getCancellationRate() const {
    if (requestCount == 0) return 0.0;
    return (getCancelledRequests() * 100.0) / requestCount;
}

// Get peak usage zone
std::string Analytics::getPeakUsageZone() const {
    if (zoneCount == 0) return "";
    
    Zone* peakZone = zones[0];
    double maxUtilization = peakZone->getUtilizationRate();
    
    for (int i = 1; i < zoneCount; i++) {
        double utilization = zones[i]->getUtilizationRate();
        if (utilization > maxUtilization) {
            maxUtilization = utilization;
            peakZone = zones[i];
        }
    }
    
    return peakZone->getZoneId();
}

// Get top N peak usage zones
std::vector<std::string> Analytics::getPeakUsageZones(int topN) const {
    std::vector<std::pair<double, std::string>> zoneUtilizations;
    
    for (int i = 0; i < zoneCount; i++) {
        zoneUtilizations.push_back(
            std::make_pair(zones[i]->getUtilizationRate(), zones[i]->getZoneId())
        );
    }
    
    // Sort by utilization (descending)
    std::sort(zoneUtilizations.begin(), zoneUtilizations.end(),
              [](const auto& a, const auto& b) {
                  return a.first > b.first;
              });
    
    // Get top N
    std::vector<std::string> result;
    int limit = std::min(topN, (int)zoneUtilizations.size());
    for (int i = 0; i < limit; i++) {
        result.push_back(zoneUtilizations[i].second);
    }
    
    return result;
}

// Get total revenue
double Analytics::getTotalRevenue() const {
    double total = 0.0;
    for (int i = 0; i < requestCount; i++) {
        if (requests[i]->getState() == RELEASED) {
            total += requests[i]->getTotalCost();
        }
    }
    return total;
}

// Get average revenue per hour
double Analytics::getAverageRevenuePerHour() const {
    double totalDuration = 0.0;
    double totalRevenue = getTotalRevenue();
    
    for (int i = 0; i < requestCount; i++) {
        if (requests[i]->getState() == RELEASED) {
            totalDuration += requests[i]->getDuration();
        }
    }
    
    return totalDuration > 0 ? totalRevenue / totalDuration : 0.0;
}

// Get revenue by zone
double Analytics::getRevenueByZone(const std::string& zoneId) const {
    double revenue = 0.0;
    for (int i = 0; i < requestCount; i++) {
        if (requests[i]->getState() == RELEASED && 
            requests[i]->getAllocatedZone() == zoneId) {
            revenue += requests[i]->getTotalCost();
        }
    }
    return revenue;
}

// Get cross-zone allocations count
int Analytics::getCrossZoneAllocations() const {
    int count = 0;
    for (int i = 0; i < requestCount; i++) {
        if (requests[i]->getState() != REQUESTED && 
            requests[i]->getState() != CANCELLED &&
            requests[i]->getIsCrossZone()) {
            count++;
        }
    }
    return count;
}

// Get cross-zone rate
double Analytics::getCrossZoneRate() const {
    int allocatedCount = 0;
    for (int i = 0; i < requestCount; i++) {
        if (requests[i]->getState() != REQUESTED && 
            requests[i]->getState() != CANCELLED) {
            allocatedCount++;
        }
    }
    
    return allocatedCount > 0 ? 
           (getCrossZoneAllocations() * 100.0) / allocatedCount : 0.0;
}

// Display summary
void Analytics::displaySummary() const {
    std::cout << "\n=== Parking System Analytics Summary ===" << std::endl;
    std::cout << "Total Requests: " << requestCount << std::endl;
    std::cout << "Completed: " << getCompletedRequests() 
              << " (" << std::fixed << std::setprecision(1) 
              << getCompletionRate() << "%)" << std::endl;
    std::cout << "Cancelled: " << getCancelledRequests() 
              << " (" << std::fixed << std::setprecision(1) 
              << getCancellationRate() << "%)" << std::endl;
    std::cout << "Average Duration: " << std::fixed << std::setprecision(1) 
              << getAverageParkingDuration() << " hours" << std::endl;
    std::cout << "Total Revenue: $" << std::fixed << std::setprecision(2) 
              << getTotalRevenue() << std::endl;
    std::cout << "Cross-Zone Allocations: " << getCrossZoneAllocations() 
              << " (" << std::fixed << std::setprecision(1) 
              << getCrossZoneRate() << "%)" << std::endl;
    std::cout << "Peak Usage Zone: " << getPeakUsageZone() << std::endl;
    std::cout << "=====================================\n" << std::endl;
}

// Display detailed report
void Analytics::displayDetailedReport() const {
    displaySummary();
    
    std::cout << "=== Zone-wise Details ===" << std::endl;
    for (int i = 0; i < zoneCount; i++) {
        std::cout << zones[i]->getZoneId() << ": " 
                  << std::fixed << std::setprecision(1)
                  << zones[i]->getUtilizationRate() << "% utilized | "
                  << "Revenue: $" << std::fixed << std::setprecision(2)
                  << getRevenueByZone(zones[i]->getZoneId()) << std::endl;
    }
    
    std::cout << "\n=== Top 3 Busiest Zones ===" << std::endl;
    std::vector<std::string> peakZones = getPeakUsageZones(3);
    for (size_t i = 0; i < peakZones.size(); i++) {
        std::cout << i + 1 << ". " << peakZones[i] << " (" 
                  << std::fixed << std::setprecision(1)
                  << getZoneUtilizationRate(peakZones[i]) << "%)" << std::endl;
    }
    
    std::cout << "\n=== Revenue Analysis ===" << std::endl;
    std::cout << "Average Revenue/Hour: $" << std::fixed << std::setprecision(2)
              << getAverageRevenuePerHour() << std::endl;
              
    std::cout << "\n=====================================\n" << std::endl;
}

// Generate report string
std::string Analytics::generateReport() const {
    std::stringstream ss;
    ss << "Parking System Analytics Report\n";
    ss << "===============================\n";
    ss << "Total Requests: " << requestCount << "\n";
    ss << "Completed: " << getCompletedRequests() << "\n";
    ss << "Cancelled: " << getCancelledRequests() << "\n";
    ss << "Average Duration: " << std::fixed << std::setprecision(1) 
       << getAverageParkingDuration() << " hours\n";
    ss << "Total Revenue: $" << std::fixed << std::setprecision(2) 
       << getTotalRevenue() << "\n";
    
    return ss.str();
}