/******************************************************
 * SMART FOOD BAG PROTOTYPE FIRMWARE
 * For Patent Demonstration - v1.0
 * Components: ESP32, HX711, MPU6050, Reed Switch, SIM7600
 ******************************************************/

 #include <Wire.h>
 #include "HX711.h"
 #include <MPU6050.h>
 #include <SoftwareSerial.h>
 
 // Pins
 #define LOADCELL_DT_PIN  32
 #define LOADCELL_SCK_PIN 33
 #define REED_PIN         34
 #define SIM_RX           16
 #define SIM_TX           17
 
 // Thresholds
 #define WEIGHT_THRESHOLD    100   // grams below expected to trigger alert
 #define TILT_THRESHOLD      15000 // MPU sensitivity
 #define DEBOUNCE_DELAY      200   // ms for reed switch
 
 // Objects
 HX711 scale;
 MPU6050 mpu;
 SoftwareSerial sim7600(SIM_RX, SIM_TX); // RX, TX
 
 // Variables
 float calibration_factor = -7050; // Calibrate with known weight
 float expected_weight = 0;
 unsigned long last_reed_trigger = 0;
 bool tamper_alert_sent = false;
 
 void setup() {
   Serial.begin(115200);
   
   // Initialize sensors
   initLoadCell();
   initMPU();
   initReedSwitch();
   initSIM7600();
 
   Serial.println("System Ready");
 }
 
 void loop() {
   // 1. Weight Monitoring
   monitorWeight();
 
   // 2. Motion Detection
   detectRoughHandling();
 
   // 3. Tamper Detection
   checkTamper();
 
   // 4. GPS Reporting (every 30s)
   static unsigned long last_gps = 0;
   if (millis() - last_gps > 30000) {
     reportGPS();
     last_gps = millis();
   }
 
   delay(100);
 }
 
 /**********************
  * SENSOR INITIALIZATION
  **********************/
 void initLoadCell() {
   scale.begin(LOADCELL_DT_PIN, LOADCELL_SCK_PIN);
   scale.set_scale(calibration_factor);
   scale.tare(); // Reset to zero
   expected_weight = scale.get_units(10); // Get initial weight
   Serial.print("Expected Weight Set: ");
   Serial.println(expected_weight);
 }
 
 void initMPU() {
   Wire.begin();
   mpu.initialize();
   if (mpu.testConnection()) {
     Serial.println("MPU6050 Connected");
   } else {
     Serial.println("MPU6050 Connection Failed");
   }
 }
 
 void initReedSwitch() {
   pinMode(REED_PIN, INPUT_PULLUP);
 }
 
 void initSIM7600() {
   sim7600.begin(9600);
   delay(1000);
   sim7600.println("AT+CGPS=1"); // Enable GPS
   delay(500);
 }
 
 /**********************
  * SENSOR FUNCTIONS
  **********************/
 void monitorWeight() {
   float current_weight = scale.get_units(5);
   
   if (current_weight < (expected_weight - WEIGHT_THRESHOLD)) {
     sendAlert("ALERT! Item missing from bag");
     expected_weight = current_weight; // Update expected weight
   }
 }
 
 void detectRoughHandling() {
   int16_t ax, ay, az;
   mpu.getAcceleration(&ax, &ay, &az);
   
   if (abs(ay) > TILT_THRESHOLD || abs(ax) > TILT_THRESHOLD) {
     sendAlert("WARNING! Rough handling detected");
   }
 }
 
 void checkTamper() {
   if (digitalRead(REED_PIN) == LOW) { // Magnet detached
     if (millis() - last_reed_trigger > DEBOUNCE_DELAY && !tamper_alert_sent) {
       sendAlert("TAMPER ALERT! Bag opened unexpectedly");
       tamper_alert_sent = true;
     }
     last_reed_trigger = millis();
   } else {
     tamper_alert_sent = false;
   }
 }
 
 void reportGPS() {
   sim7600.println("AT+CGPSINFO");
   delay(500);
   while (sim7600.available()) {
     Serial.write(sim7600.read());
   }
 }
 
 /**********************
  * ALERT SYSTEM
  **********************/
 void sendAlert(const char* message) {
   // 1. Serial output for debugging
   Serial.println(message);
   
   // 2. Send SMS alert
   sim7600.println("AT+CMGF=1"); // Text mode
   delay(500);
   sim7600.println("AT+CMGS=\"+91XXXXXXXXXX\""); // Replace with your number
   delay(500);
   sim7600.print(message);
   delay(500);
   sim7600.write(26); // CTRL+Z to send
 }
 
 /**********************
  * CALIBRATION UTILITY
  **********************/
 void calibrateScale() {
   Serial.println("Remove all weight from scale");
   delay(5000);
   scale.tare();
   Serial.println("Tare complete");
   
   Serial.println("Place known weight on scale");
   delay(5000);
   float known_weight = 500.0; // Change to your calibration weight
   calibration_factor = scale.get_units(10) / known_weight;
   Serial.print("Calibration Factor: ");
   Serial.println(calibration_factor);
 }