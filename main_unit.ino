#include <WiFi.h>
#include <esp_now.h>
#include <WebServer.h>

// Define global variables
unsigned long startTime = 0;
bool isRunning = false;
WebServer server;

// Function declarations
void setupWiFi();
void setupESPNow();
void startStopwatch();
void stopStopwatch();
void sendTime();
void onReceiveData(const uint8_t * mac, const uint8_t *incomingData, int len);
void handleRoot();
void handleStart();
void handleStop();
void handleTimeSync();

void setup() {
    Serial.begin(115200);
    setupWiFi();
    setupESPNow();

    // Setup web server routes
    server.on("/", handleRoot);
    server.on("/start", handleStart);
    server.on("/stop", handleStop);
    server.on("/sync", handleTimeSync);
    server.begin();
}

void loop() {
    server.handleClient();

    // Stopwatch logic
    if (isRunning) {
        unsigned long currentTime = millis();
        Serial.print("Elapsed Time: ");
        Serial.println(currentTime - startTime);
    }
}

void setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    WiFi.begin("your_SSID", "your_PASSWORD"); // Replace with your WiFi credentials
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
}

void setupESPNow() {
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_recv_cb(onReceiveData);
}

void startStopwatch() {
    startTime = millis();
    isRunning = true;
    Serial.println("Stopwatch started");
    sendTime();
}

void stopStopwatch() {
    isRunning = false;
    Serial.println("Stopwatch stopped");
}

void sendTime() {
    // Function to send the current stopwatch time via ESP-NOW
    // Implement the ESP-NOW sending logic here
}

void onReceiveData(const uint8_t * mac, const uint8_t *incomingData, int len) {
    // Handle incoming ESP-NOW data
}

void handleRoot() {
    server.send(200, "text/plain", "ESP32 Stopwatch System");
}

void handleStart() {
    startStopwatch();
    server.send(200, "text/plain", "Stopwatch Started");
}

void handleStop() {
    stopStopwatch();
    server.send(200, "text/plain", "Stopwatch Stopped");
}

void handleTimeSync() {
    // Implement time synchronization logic here
    server.send(200, "text/plain", "Time Synchronized");
}