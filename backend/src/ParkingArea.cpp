#include "ParkingArea.h"
#include <iostream>
#include <sstream>

// SlotNode constructor
SlotNode::SlotNode(ParkingSlot* s) : slot(s), next(nullptr) {}

// ParkingArea constructor
ParkingArea::ParkingArea(const std::string& id, const std::string& zone, int cap)
    : areaId(id), zoneId(zone), capacity(cap), availableSlots(0), slotList(nullptr) {}

// Destructor
ParkingArea::~ParkingArea() {
    SlotNode* current = slotList;
    while (current != nullptr) {
        SlotNode* next = current->next;
        delete current->slot;
        delete current;
        current = next;
    }
}

// Add slot to area
bool ParkingArea::addSlot(ParkingSlot* slot) {
    SlotNode* newNode = new SlotNode(slot);
    
    if (slotList == nullptr) {
        slotList = newNode;
    } else {
        SlotNode* current = slotList;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
    
    availableSlots++;
    std::cout << "Added slot " << slot->getSlotId() << " to area " << areaId << std::endl;
    return true;
}

// Find first available slot
ParkingSlot* ParkingArea::findAvailableSlot() const {
    SlotNode* current = slotList;
    while (current != nullptr) {
        if (current->slot->getIsAvailable()) {
            return current->slot;
        }
        current = current->next;
    }
    return nullptr;
}

// Get slot by ID
ParkingSlot* ParkingArea::getSlotById(const std::string& slotId) const {
    SlotNode* current = slotList;
    while (current != nullptr) {
        if (current->slot->getSlotId() == slotId) {
            return current->slot;
        }
        current = current->next;
    }
    return nullptr;
}

// Get available slots count
int ParkingArea::getAvailableCount() const {
    int count = 0;
    SlotNode* current = slotList;
    while (current != nullptr) {
        if (current->slot->getIsAvailable()) {
            count++;
        }
        current = current->next;
    }
    return count;
}

// Get total capacity
int ParkingArea::getTotalCapacity() const {
    int count = 0;
    SlotNode* current = slotList;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

// Get utilization rate
double ParkingArea::getUtilizationRate() const {
    int total = getTotalCapacity();
    if (total == 0) return 0.0;
    return ((total - getAvailableCount()) * 100.0) / total;
}

// Getters
std::string ParkingArea::getAreaId() const {
    return areaId;
}

std::string ParkingArea::getZoneId() const {
    return zoneId;
}

// Convert to string
std::string ParkingArea::toString() const {
    std::stringstream ss;
    ss << "Area: " << areaId 
       << " | Zone: " << zoneId
       << " | Capacity: " << getTotalCapacity()
       << " | Available: " << getAvailableCount()
       << " | Utilization: " << getUtilizationRate() << "%";
    return ss.str();
}