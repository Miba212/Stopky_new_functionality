#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <esp_now.h>
#include <WiFi.h>

// Message types
enum MessageType {
    MSG_STATE_UPDATE = 1,
    MSG_TRIGGER_EVENT = 2,
    MSG_TIME_SYNC = 3,
    MSG_ACK = 4,
    MSG_CONFIG = 5
};

// System states
enum SystemState {
    STATE_RESET = 0,
    STATE_RUNNING = 1,
    STATE_STOPPED = 2
};

// Trigger roles
enum TriggerRole {
    ROLE_START = 0,
    ROLE_STOP = 1,
    ROLE_START_STOP = 2,
    ROLE_SPLIT = 3,
    ROLE_DISPLAY = 4
};

// Trigger event types
enum TriggerEventType {
    TRIGGER_START = 0,
    TRIGGER_STOP = 1,
    TRIGGER_SPLIT = 2
};

// Message structures
struct StateUpdateMessage {
    MessageType type = MSG_STATE_UPDATE;
    SystemState state;
    unsigned long timestamp;
    uint32_t messageId;
};

struct TriggerEventMessage {
    MessageType type = MSG_TRIGGER_EVENT;
    TriggerEventType triggerType;
    unsigned long timestamp;
    uint8_t unitId;
    uint32_t messageId;
};

struct TimeSyncMessage {
    MessageType type = MSG_TIME_SYNC;
    unsigned long masterTime;
    uint32_t messageId;
};

struct AckMessage {
    MessageType type = MSG_ACK;
    uint32_t ackForMessageId;
    uint8_t unitId;
};

struct ConfigMessage {
    MessageType type = MSG_CONFIG;
    uint8_t unitId;
    TriggerRole role;
    char unitName[32];
    uint32_t messageId;
};

// Union for all message types
union Message {
    MessageType type;
    StateUpdateMessage stateUpdate;
    TriggerEventMessage triggerEvent;
    TimeSyncMessage timeSync;
    AckMessage ack;
    ConfigMessage config;
};

// Communication configuration
#define MAX_UNITS 10
#define ACK_TIMEOUT_MS 500
#define MAX_RETRIES 3
#define TIME_SYNC_INTERVAL_MS 300000  // 5 minutes

// Function declarations
bool initESPNow();
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void onDataReceived(const uint8_t * mac, const uint8_t *incomingData, int len);
bool sendMessage(const uint8_t* mac_addr, const Message& msg);
bool sendMessageWithRetry(const uint8_t* mac_addr, const Message& msg);
uint32_t generateMessageId();

#endif
