#include "crow.h"
#include "../include/ParkingSystem.h"
#include <iostream>
#include <memory>
#include <string>
#include <sstream>

using namespace std;

// Global parking system
unique_ptr<ParkingSystem> parkingSystem;

// Helper to create JSON response
string jsonResponse(bool success, const string& message, const string& data = "{}") {
    stringstream ss;
    ss << R"({"success":)" << (success ? "true" : "false") 
       << R"(,"message":")" << message << R"(")";
    if (!data.empty() && data != "{}") {
        ss << R"(,"data":)" << data;
    }
    ss << "}";
    return ss.str();
}

int main() {
    cout << "🚀 Starting NexusPark HTTP Server..." << endl;
    cout << "Frontend will be available at: http://localhost:8080" << endl;
    cout << "API endpoints:" << endl;
    cout << "  GET  /api/zones" << endl;
    cout << "  POST /api/request-parking" << endl;
    cout << "  GET  /api/analytics" << endl;
    
    // Initialize parking system
    parkingSystem = make_unique<ParkingSystem>();
    parkingSystem->initializeZones();
    
    // Create Crow app
    crow::SimpleApp app;
    
    // CORS middleware
    CROW_ROUTE(app, "/")
    ([]() {
        return "NexusPark HTTP Server is running!";
    });
    
    // Serve frontend files
    CROW_ROUTE(app, "/<path>")
    ([](const crow::request& req, crow::response& res, string path) {
        // Serve index.html for frontend
        if (path.empty() || path == "index.html") {
            res.set_static_file_info("../frontend/index.html");
        } else {
            res.set_static_file_info("../frontend/" + path);
        }
        res.end();
    });
    
    // API: Get all zones
    CROW_ROUTE(app, "/api/zones")
    ([]() {
        // Mock response matching your frontend format
        return R"([
            {
                "zoneId": "ZA",
                "zoneName": "Downtown Parking A", 
                "totalSlots": 20,
                "availableSlots": 12,
                "hourlyRate": 5.0,
                "utilization": 40.0,
                "connections": [
                    {"zoneId": "ZB", "distance": 500},
                    {"zoneId": "ZC", "distance": 800}
                ]
            },
            {
                "zoneId": "ZB",
                "zoneName": "Mall Parking B",
                "totalSlots": 15,
                "availableSlots": 8,
                "hourlyRate": 4.0,
                "utilization": 46.7,
                "connections": [
                    {"zoneId": "ZA", "distance": 500},
                    {"zoneId": "ZC", "distance": 300},
                    {"zoneId": "ZD", "distance": 700}
                ]
            },
            {
                "zoneId": "ZC",
                "zoneName": "Office Parking C",
                "totalSlots": 25,
                "availableSlots": 18,
                "hourlyRate": 6.0,
                "utilization": 28.0,
                "connections": [
                    {"zoneId": "ZA", "distance": 800},
                    {"zoneId": "ZB", "distance": 300},
                    {"zoneId": "ZE", "distance": 400}
                ]
            },
            {
                "zoneId": "ZD", 
                "zoneName": "Residential Parking D",
                "totalSlots": 18,
                "availableSlots": 10,
                "hourlyRate": 3.5,
                "utilization": 44.4,
                "connections": [
                    {"zoneId": "ZB", "distance": 700},
                    {"zoneId": "ZE", "distance": 600}
                ]
            },
            {
                "zoneId": "ZE",
                "zoneName": "Airport Parking E",
                "totalSlots": 30,
                "availableSlots": 22,
                "hourlyRate": 8.0,
                "utilization": 26.7,
                "connections": [
                    {"zoneId": "ZC", "distance": 400},
                    {"zoneId": "ZD", "distance": 600}
                ]
            }
        ])";
    });
    
    // API: Request parking
    CROW_ROUTE(app, "/api/request-parking").methods("POST"_method)
    ([](const crow::request& req) {
        // Parse JSON request
        auto body = crow::json::load(req.body);
        if (!body) {
            return jsonResponse(false, "Invalid JSON request");
        }
        
        string vehicleId = body["vehicleId"].s();
        string preferredZone = body["preferredZone"].s();
        int duration = body["durationHours"].i();
        
        // Call actual parking system
        string requestId = parkingSystem->requestParking(vehicleId, preferredZone, duration);
        
        if (!requestId.empty()) {
            stringstream data;
            data << R"({"requestId":")" << requestId 
                 << R"(","message":"Parking allocated successfully"})";
            return jsonResponse(true, "Parking allocated", data.str());
        } else {
            return jsonResponse(false, "No parking available");
        }
    });
    
    // API: Get analytics
    CROW_ROUTE(app, "/api/analytics")
    ([]() {
        return R"({
            "totalRequests": 50,
            "completedRequests": 40,
            "cancelledRequests": 10,
            "completionRate": 80.0,
            "averageDuration": 2.5,
            "totalRevenue": 450.0,
            "peakZone": "ZA",
            "crossZoneRate": 15.0
        })";
    });
    
    // Start server on port 8080
    cout << "✅ Server starting on http://localhost:8080" << endl;
    app.port(8080).multithreaded().run();
    
    return 0;
}
