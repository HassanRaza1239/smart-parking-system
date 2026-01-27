#include "../include/Analytics.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <vector>
#include <utility>

// Analytics constructor
Analytics::Analytics(ParkingRequest** reqArray, int reqCount, Zone** zoneArray, int zCount)
    : requests(reqArray), requestCount(reqCount), zones(zoneArray), zoneCount(zCount) {}

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
    
    if (completedCount == 0) return 0.0;
    return totalDuration / completedCount;
}

// Get zone utilization rate
double Analytics::getZoneUtilizationRate(const std::string& zoneId) const {
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i]->getZoneId() == zoneId) {
            return zones[i]->getUtilizationRate();
        }
    }
    return 0.0;
}

// Get all zone utilization rates
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
    return (static_cast<double>(getCompletedRequests()) / requestCount) * 100.0;
}

// Get cancellation rate
double Analytics::getCancellationRate() const {
    if (requestCount == 0) return 0.0;
    return (static_cast<double>(getCancelledRequests()) / requestCount) * 100.0;
}

// Get peak usage zone
std::string Analytics::getPeakUsageZone() const {
    if (requestCount == 0) return "No requests";
    
    std::map<std::string, int> zoneCounts;
    
    // Count allocations per zone
    for (int i = 0; i < requestCount; i++) {
        std::string zone = requests[i]->getAllocatedZone();
        if (!zone.empty()) {
            zoneCounts[zone]++;
        }
    }
    
    // Find zone with max count
    std::string peakZone;
    int maxCount = 0;
    
    for (const auto& pair : zoneCounts) {
        if (pair.second > maxCount) {
            maxCount = pair.second;
            peakZone = pair.first;
        }
    }
    
    return peakZone.empty() ? "No allocations" : peakZone;
}

// Get top N peak usage zones
std::vector<std::string> Analytics::getPeakUsageZones(int topN) const {
    std::map<std::string, int> zoneCounts;
    
    // Count allocations per zone
    for (int i = 0; i < requestCount; i++) {
        std::string zone = requests[i]->getAllocatedZone();
        if (!zone.empty()) {
            zoneCounts[zone]++;
        }
    }
    
    // Convert to vector for sorting
    std::vector<std::pair<std::string, int>> zonesVector(zoneCounts.begin(), zoneCounts.end());
    
    // Sort by count (descending)
    std::sort(zonesVector.begin(), zonesVector.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });
    
    // Get top N
    std::vector<std::string> result;
    int count = 0;
    for (const auto& pair : zonesVector) {
        if (count >= topN) break;
        result.push_back(pair.first + " (" + std::to_string(pair.second) + " requests)");
        count++;
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
    int completed = getCompletedRequests();
    if (completed == 0) return 0.0;
    
    double totalDuration = 0.0;
    for (int i = 0; i < requestCount; i++) {
        if (requests[i]->getState() == RELEASED) {
            totalDuration += requests[i]->getDuration();
        }
    }
    
    if (totalDuration == 0.0) return 0.0;
    return getTotalRevenue() / totalDuration;
}

// Get revenue by zone
double Analytics::getRevenueByZone(const std::string& zoneId) const {
    double total = 0.0;
    for (int i = 0; i < requestCount; i++) {
        if (requests[i]->getState() == RELEASED && 
            requests[i]->getAllocatedZone() == zoneId) {
            total += requests[i]->getTotalCost();
        }
    }
    return total;
}

// Get cross-zone allocations count
int Analytics::getCrossZoneAllocations() const {
    int count = 0;
    for (int i = 0; i < requestCount; i++) {
        if (requests[i]->getState() == RELEASED && 
            requests[i]->getIsCrossZone()) {
            count++;
        }
    }
    return count;
}

// Get cross-zone allocation rate
double Analytics::getCrossZoneRate() const {
    int completed = getCompletedRequests();
    if (completed == 0) return 0.0;
    
    return (static_cast<double>(getCrossZoneAllocations()) / completed) * 100.0;
}

// Display summary
void Analytics::displaySummary() const {
    std::cout << "\n=== PARKING ANALYTICS SUMMARY ===" << std::endl;
    std::cout << "Total Requests: " << requestCount << std::endl;
    std::cout << "Completed: " << getCompletedRequests() 
              << " (" << std::fixed << std::setprecision(1) << getCompletionRate() << "%)" << std::endl;
    std::cout << "Cancelled: " << getCancelledRequests() 
              << " (" << std::fixed << std::setprecision(1) << getCancellationRate() << "%)" << std::endl;
    std::cout << "Average Duration: " << std::fixed << std::setprecision(1) 
              << getAverageParkingDuration() << " hours" << std::endl;
    std::cout << "Total Revenue: $" << std::fixed << std::setprecision(2) 
              << getTotalRevenue() << std::endl;
    std::cout << "Average Revenue/Hour: $" << std::fixed << std::setprecision(2) 
              << getAverageRevenuePerHour() << std::endl;
    std::cout << "Cross-Zone Allocations: " << getCrossZoneAllocations() 
              << " (" << std::fixed << std::setprecision(1) << getCrossZoneRate() << "%)" << std::endl;
    std::cout << "Peak Usage Zone: " << getPeakUsageZone() << std::endl;
}

// Display detailed report
void Analytics::displayDetailedReport() const {
    displaySummary();
    
    std::cout << "\n=== ZONE UTILIZATION ===" << std::endl;
    for (int i = 0; i < zoneCount; i++) {
        std::cout << zones[i]->getZoneId() << ": " 
                  << std::fixed << std::setprecision(1) 
                  << zones[i]->getUtilizationRate() << "% | Revenue: $" 
                  << std::fixed << std::setprecision(2) 
                  << getRevenueByZone(zones[i]->getZoneId()) << std::endl;
    }
    
    std::cout << "\n=== TOP ZONES BY USAGE ===" << std::endl;
    std::vector<std::string> topZones = getPeakUsageZones(3);
    for (size_t i = 0; i < topZones.size(); i++) {
        std::cout << i + 1 << ". " << topZones[i] << std::endl;
    }
}

// Generate report string
std::string Analytics::generateReport() const {
    std::stringstream ss;
    ss << "Parking System Analytics Report\n";
    ss << "===============================\n";
    ss << "Total Requests: " << requestCount << "\n";
    ss << "Completed: " << getCompletedRequests() << "\n";
    ss << "Cancelled: " << getCancelledRequests() << "\n";
    ss << "Total Revenue: $" << std::fixed << std::setprecision(2) << getTotalRevenue() << "\n";
    ss << "Cross-Zone Rate: " << std::fixed << std::setprecision(1) << getCrossZoneRate() << "%\n";
    return ss.str();
}
