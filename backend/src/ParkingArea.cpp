#include "../include/ParkingArea.h"
#include <sstream>

// SlotNode constructor
SlotNode::SlotNode(ParkingSlot* s) : slot(s), next(nullptr) {}

// ParkingArea constructor
ParkingArea::ParkingArea(const std::string& id, const std::string& zone, int cap)
    : areaId(id), zoneId(zone), capacity(cap), availableSlots(cap), slotList(nullptr) {}

// ParkingArea destructor
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
    if (!slot) return false;
    
    SlotNode* newNode = new SlotNode(slot);
    newNode->next = slotList;
    slotList = newNode;
    
    return true;
}

// Find available slot in area
ParkingSlot* ParkingArea::findAvailableSlot() const {
    SlotNode* current = slotList;
    while (current != nullptr) {
        if (current->slot->canBeAllocated()) {
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

// Get available count
int ParkingArea::getAvailableCount() const {
    return availableSlots;
}

// Get total capacity
int ParkingArea::getTotalCapacity() const {
    return capacity;
}

// Get utilization rate
double ParkingArea::getUtilizationRate() const {
    if (capacity == 0) return 0.0;
    double usedSlots = capacity - availableSlots;
    return (usedSlots / capacity) * 100.0;
}

// Get area ID
std::string ParkingArea::getAreaId() const {
    return areaId;
}

// Get zone ID
std::string ParkingArea::getZoneId() const {
    return zoneId;
}

// Convert to string
std::string ParkingArea::toString() const {
    std::stringstream ss;
    ss << "Area " << areaId << " in Zone " << zoneId
       << " (" << availableSlots << "/" << capacity << " available)"
       << " Utilization: " << getUtilizationRate() << "%";
    return ss.str();
}
