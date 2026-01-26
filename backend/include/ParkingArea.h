#ifndef PARKINGAREA_H
#define PARKINGAREA_H

#include "ParkingSlot.h"
#include <string>

// Custom linked list node for parking slots
struct SlotNode {
    ParkingSlot* slot;
    SlotNode* next;
    
    SlotNode(ParkingSlot* s);
};

class ParkingArea {
private:
    std::string areaId;
    std::string zoneId;
    int capacity;
    int availableSlots;
    
    // Custom linked list for parking slots
    SlotNode* slotList;
    
public:
    ParkingArea(const std::string& id, const std::string& zone, int cap);
    ~ParkingArea();
    
    // Slot management
    bool addSlot(ParkingSlot* slot);
    ParkingSlot* findAvailableSlot() const;
    ParkingSlot* getSlotById(const std::string& slotId) const;
    
    // Availability
    int getAvailableCount() const;
    int getTotalCapacity() const;
    double getUtilizationRate() const;
    
    // Getters
    std::string getAreaId() const;
    std::string getZoneId() const;
    
    // Display
    std::string toString() const;
};

#endif