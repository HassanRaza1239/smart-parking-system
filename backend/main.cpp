#include "include/ParkingSystem.h"
#include <iostream>
#include <cstdlib>

using namespace std;

void displayMenu() {
    cout << "\n🚀 NEXUSPARK SMART PARKING SYSTEM 🚀" << endl;
    cout << "======================================" << endl;
    cout << "1. Run Complete Demo" << endl;
    cout << "2. Interactive Mode" << endl;
    cout << "3. Show Zone Status" << endl;
    cout << "4. Show Analytics" << endl;
    cout << "5. Test Allocation" << endl;
    cout << "6. Exit" << endl;
    cout << "======================================" << endl;
    cout << "Enter your choice (1-6): ";
}

void interactiveMode(ParkingSystem& system) {
    cout << "\n=== INTERACTIVE MODE ===" << endl;
    cout << "Available vehicles: CAR001, BIKE001, TRUCK001, CAR002, BIKE002" << endl;
    cout << "Available zones: ZA, ZB, ZC, ZD, ZE" << endl;
    
    string vehicleId, zoneId;
    int duration;
    
    cout << "Enter vehicle ID: ";
    cin >> vehicleId;
    
    cout << "Enter preferred zone: ";
    cin >> zoneId;
    
    cout << "Enter duration (hours): ";
    cin >> duration;
    
    string reqId = system.requestParking(vehicleId, zoneId, duration);
    if (!reqId.empty()) {
        cout << "✅ Request created: " << reqId << endl;
    }
}

void testAllocation(ParkingSystem& system) {
    cout << "\n=== TESTING ALLOCATION ===" << endl;
    
    // Test 1: Car in preferred zone
    cout << "\nTest 1: Car in preferred zone (ZA)" << endl;
    string req1 = system.requestParking("CAR001", "ZA", 3);
    
    // Test 2: Bike in preferred zone
    cout << "\nTest 2: Bike in preferred zone (ZB)" << endl;
    string req2 = system.requestParking("BIKE001", "ZB", 2);
    
    // Test 3: Occupy a slot
    if (!req1.empty()) {
        cout << "\nTest 3: Occupying slot for request " << req1 << endl;
        system.occupyParking(req1);
    }
    
    // Test 4: Release a slot
    if (!req1.empty()) {
        cout << "\nTest 4: Releasing slot for request " << req1 << endl;
        system.releaseParking(req1);
    }
    
    // Test 5: Undo last operation
    cout << "\nTest 5: Undoing last operation" << endl;
    system.undoLastOperation();
}

int main() {
    ParkingSystem system;
    
    int choice;
    bool running = true;
    
    while (running) {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "\n=== RUNNING COMPLETE DEMO ===" << endl;
                system.runDemo();
                break;
                
            case 2:
                interactiveMode(system);
                break;
                
            case 3:
                system.displayZoneStatus();
                break;
                
            case 4:
                system.displayAnalytics();
                break;
                
            case 5:
                testAllocation(system);
                break;
                
            case 6:
                cout << "\nThank you for using NexusPark!" << endl;
                running = false;
                break;
                
            default:
                cout << "Invalid choice! Please try again." << endl;
                break;
        }
    }
    
    return 0;
}
