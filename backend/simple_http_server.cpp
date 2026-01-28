#include "../include/ParkingSystem.h"
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

unique_ptr<ParkingSystem> parkingSystem;

// Helper to create JSON
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

// HTTP response builder
string buildHttpResponse(int status, const string& content, const string& contentType = "application/json") {
    stringstream response;
    response << "HTTP/1.1 " << status << " OK\r\n"
             << "Content-Type: " << contentType << "\r\n"
             << "Access-Control-Allow-Origin: *\r\n"
             << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
             << "Access-Control-Allow-Headers: Content-Type\r\n"
             << "Content-Length: " << content.length() << "\r\n"
             << "Connection: close\r\n\r\n"
             << content;
    return response.str();
}

// Serve frontend files
string serveFrontend() {
    // Read index.html
    FILE* file = fopen("../frontend/index.html", "rb");
    if (!file) {
        return buildHttpResponse(404, "Frontend not found", "text/plain");
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    vector<char> buffer(size + 1);
    fread(buffer.data(), 1, size, file);
    fclose(file);
    buffer[size] = 0;
    
    return buildHttpResponse(200, string(buffer.data()), "text/html");
}

// Handle API requests
string handleApiRequest(const string& method, const string& path, const string& body) {
    cout << "API Request: " << method << " " << path << endl;
    
    // GET /api/zones
    if (method == "GET" && path == "/api/zones") {
        // Simulate zone data - YOU'LL CONNECT THIS TO REAL ParkingSystem
        string zonesJson = R"([
            {"zoneId":"ZA","totalSlots":20,"availableSlots":12,"hourlyRate":10},
            {"zoneId":"ZB","totalSlots":15,"availableSlots":8,"hourlyRate":12},
            {"zoneId":"ZC","totalSlots":25,"availableSlots":5,"hourlyRate":8},
            {"zoneId":"ZD","totalSlots":30,"availableSlots":20,"hourlyRate":15},
            {"zoneId":"ZE","totalSlots":18,"availableSlots":10,"hourlyRate":11}
        ])";
        
        return buildHttpResponse(200, jsonResponse(true, "Zones retrieved", zonesJson));
    }
    
    // POST /api/request-parking
    if (method == "POST" && path == "/api/request-parking") {
        // TODO: Parse JSON body and call parkingSystem->requestParking()
        string response = R"({
            "requestId": "REQ001",
            "allocatedZone": "ZA",
            "slotNumber": 5,
            "totalCost": 30.0,
            "estimatedPath": ["Entrance", "ZA"]
        })";
        
        return buildHttpResponse(200, jsonResponse(true, "Parking allocated", response));
    }
    
    // GET /api/analytics
    if (method == "GET" && path == "/api/analytics") {
        // TODO: Call parkingSystem->getAnalytics()
        string analytics = R"({
            "totalRequests": 50,
            "successfulRequests": 40,
            "completionRate": 80.0,
            "totalRevenue": 450.0,
            "mostOccupiedZone": "ZC",
            "leastOccupiedZone": "ZE"
        })";
        
        return buildHttpResponse(200, jsonResponse(true, "Analytics data", analytics));
    }
    
    return buildHttpResponse(404, jsonResponse(false, "Endpoint not found"));
}

int main() {
    cout << "🚀 Starting NexusPark HTTP Server (Simple Version)..." << endl;
    cout << "Frontend: http://localhost:8080" << endl;
    cout << "APIs: /api/zones, /api/request-parking, /api/analytics" << endl;
    
    // Initialize parking system
    parkingSystem = make_unique<ParkingSystem>();
    parkingSystem->initializeZones();
    
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Failed to initialize Winsock" << endl;
        return 1;
    }
    
    // Create socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Failed to create socket" << endl;
        WSACleanup();
        return 1;
    }
    
    // Bind to port 8080
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);
    
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Failed to bind to port 8080" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    // Listen for connections
    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        cerr << "Failed to listen" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    cout << "✅ Server running on http://localhost:8080" << endl;
    cout << "Press Ctrl+C to stop" << endl;
    
    // Main server loop
    while (true) {
        sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
        
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Failed to accept connection" << endl;
            continue;
        }
        
        // Read request
        char buffer[4096];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            string request(buffer);
            
            // Parse request
            string method, path;
            stringstream requestStream(request);
            requestStream >> method >> path;
            
            // Read body for POST requests
            string body;
            if (method == "POST") {
                // Find body (after empty line)
                size_t bodyPos = request.find("\r\n\r\n");
                if (bodyPos != string::npos) {
                    body = request.substr(bodyPos + 4);
                }
            }
            
            string response;
            
            // Route requests
            if (path == "/" || path == "/index.html") {
                response = serveFrontend();
            }
            else if (path.find("/api/") == 0) {
                response = handleApiRequest(method, path, body);
            }
            else {
                response = buildHttpResponse(404, "Not Found", "text/plain");
            }
            
            // Send response
            send(clientSocket, response.c_str(), response.length(), 0);
        }
        
        closesocket(clientSocket);
    }
    
    // Cleanup
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
