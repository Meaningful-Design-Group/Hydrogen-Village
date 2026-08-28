#include <WiFi.h>
#include <WiFiManager.h>
#include <FirebaseESP32.h>
#include <HX711.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Preferences.h>
#include <LittleFS.h>

// --- PIN CONFIGURATION ---
#define HX711_DAT 2   
#define HX711_CLK 3   
#define ENC_CLK   4   
#define ENC_DT    5   
#define ENC_SW    21  
#define BTN_STOP  20  

// --- OBJECTS ---
HX711 scale;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Preferences prefs;

// --- SYSTEM VARIABLES ---
enum State { IDLE, SELECT_MAT, CONFIRM, RECORDING };
State currentState = IDLE;

const char* materials[] = {"PET", "HDPE", "LDPE", "PP", "MIX"};
int currentMat = 0;
const int totalMats = 5;

int lastClk;
float weight = 0;
int sessionCount = 0;
float calibration_factor = 1.0; 
String fullBagID = "";    
bool is_busy = false; 

unsigned long lastSend = 0;
unsigned long stopTimer = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(6, 7); 
  lcd.init(); lcd.backlight();
  if(!LittleFS.begin(true)) Serial.println("LittleFS Error");

  pinMode(ENC_CLK, INPUT); pinMode(ENC_DT, INPUT);
  pinMode(ENC_SW, INPUT_PULLUP); pinMode(BTN_STOP, INPUT_PULLUP);

  lcd.print("WEEDO SYSTEM");
  lcd.setCursor(0,1); lcd.print("CONNECTING...");

  WiFiManager wm;
  wm.autoConnect("Weedo-Unit01", "admin123");

  prefs.begin("timbangan", true); 
  calibration_factor = prefs.getFloat("factor", 2280.0); 
  sessionCount = prefs.getInt("session", 0); 
  prefs.end();

  config.host = "weedo-749ac-default-rtdb.asia-southeast1.firebasedatabase.app";
  config.signer.tokens.legacy_token = "CqUAXSTnuayu2Kcf7juyVoeBwMNtPPt6ukkPPB27";
  Firebase.begin(&config, &auth);

  scale.begin(HX711_DAT, HX711_CLK);
  scale.set_scale(calibration_factor);
  updateDisplay();
}

void loop() {
  if (digitalRead(BTN_STOP) == LOW) {
    if (stopTimer == 0) stopTimer = millis();
    if (millis() - stopTimer > 10000) {
      WiFiManager wm; wm.resetSettings(); ESP.restart();
    }
  } else { stopTimer = 0; }

  int clk = digitalRead(ENC_CLK);
  bool clicked = (digitalRead(ENC_SW) == LOW);

  switch (currentState) {
    case IDLE:
      // PERUBAHAN: Reset fullBagID dipindah ke sini, HANYA JIKA user klik mulai sesi baru
      if (clicked) { 
        delay(200); 
        fullBagID = ""; // Dihapus hanya saat mau mulai sesi baru
        currentState = SELECT_MAT; 
        updateDisplay(); 
      }
      break;

    case SELECT_MAT:
      if (clk != lastClk && clk == LOW) {
        if (digitalRead(ENC_DT) != clk) currentMat = (currentMat + 1) % totalMats;
        else currentMat = (currentMat - 1 + totalMats) % totalMats;
        updateDisplay();
      }
      if (clicked) { delay(200); generateFullBagID(); currentState = CONFIRM; updateDisplay(); }
      break;

    case CONFIRM:
      if (clicked) {
        delay(200);
        is_busy = true; 
        sessionCount++;
        prefs.begin("timbangan", false); 
        prefs.putInt("session", sessionCount); 
        prefs.end();
        scale.tare(); 
        currentState = RECORDING; 
        
        // Kirim ID aktif
        sendHistoryEvent("Session start", String(fullBagID)); 
        
        is_busy = false; 
        lcd.clear();
      }
      break;

    case RECORDING:
      if (digitalRead(BTN_STOP) == LOW) {
        delay(200);
        is_busy = true; 
        
        // LOCK DATA AGAR TIDAK BERUBAH
        String ID_SIMPAN = String(fullBagID); 
        float BERAT_SIMPAN = weight;
        String MAT_SIMPAN = String(materials[currentMat]);

        lcd.clear();
        lcd.print("SAVING DATA...");
        lcd.setCursor(0,1);
        lcd.print("ID: " + ID_SIMPAN);

        // 1. Kirim Log Produksi
        FirebaseJson prod;
        prod.set("bag_id", ID_SIMPAN);
        prod.set("weight", BERAT_SIMPAN);
        prod.set("material", MAT_SIMPAN);
        prod.set("session", sessionCount);
        prod.set("ts/.sv", "timestamp");
        Firebase.pushJSON(fbdo, "/units/unit_01/production_logs", prod);

        // 2. Kirim Event History
        FirebaseJson hist;
        hist.set("Activity", "Session ended");
        hist.set("Bag ID", ID_SIMPAN); 
        hist.set("Material", MAT_SIMPAN);
        hist.set("Time/.sv", "timestamp");
        hist.set("Value", String(BERAT_SIMPAN, 2));
        hist.set("Bucket", "unit_01");
        Firebase.pushJSON(fbdo, "/units/unit_01/history", hist);

        // Jeda agar Firebase tuntas
        delay(3000); 

        // KUNCI UTAMA: Pindah state tapi JANGAN hapus fullBagID dulu
        currentState = IDLE;
        is_busy = false; 
        updateDisplay();
      }
      break;
  }
  lastClk = clk;

  if (scale.is_ready()) {
    weight = scale.get_units(1);
    if (weight < 0.02) weight = 0;
  }

  if (millis() - lastSend > 2000 && !is_busy) {
    syncLiveStatus();
    lastSend = millis();
    if (currentState == RECORDING) updateDisplay();
  }
}

void sendHistoryEvent(String type, String idToUse) {
  if(WiFi.status() != WL_CONNECTED) return;
  FirebaseJson json;
  json.set("Activity", type);
  json.set("Bag ID", idToUse); 
  json.set("Material", materials[currentMat]);
  json.set("Time/.sv", "timestamp");
  json.set("Value", "-");
  json.set("Bucket", "unit_01");
  Firebase.pushJSON(fbdo, "/units/unit_01/history", json);
}

void generateFullBagID() {
  String rnd = String(random(0x100, 0xFFF), HEX); 
  rnd.toUpperCase();
  String prefix = "";
  String m = materials[currentMat];
  if (m == "PET") prefix = "PT";
  else if (m == "HDPE") prefix = "HD";
  else if (m == "LDPE") prefix = "LD";
  else if (m == "PP") prefix = "PP";
  else prefix = "MX";
  
  fullBagID = "B1-" + String(sessionCount + 1) + "-" + prefix + "-" + rnd;
}

void updateDisplay() {
  lcd.clear(); lcd.setCursor(0,0);
  switch (currentState) {
    case IDLE: 
      lcd.print("SYSTEM READY"); 
      lcd.setCursor(0,1); 
      // TAMPILKAN LAST BAG ID DI BARIS KEDUA LCD [KONSEP USER]
      if (fullBagID != "") lcd.print("LST:" + fullBagID);
      else lcd.print("> CLICK START"); 
      break;
    case SELECT_MAT: 
      lcd.print("SELECT MATERIAL:"); 
      lcd.setCursor(0,1); lcd.print("> " + String(materials[currentMat])); 
      break;
    case CONFIRM: 
      lcd.print("WRITE ON BAG:"); 
      lcd.setCursor(0,1); lcd.print(fullBagID); 
      break;
    case RECORDING: 
      lcd.print(String(materials[currentMat]) + " " + String(weight, 2) + "kg"); 
      lcd.setCursor(0,1); lcd.print("ID: " + fullBagID); 
      break;
  }
}

void syncLiveStatus() {
  if(WiFi.status() != WL_CONNECTED) return;
  FirebaseJson json;
  // Live Dashboard tetap rapi, ID hanya tampil saat recording
  json.set("bag_id", (currentState == IDLE) ? "-" : fullBagID);
  json.set("is_recording", (currentState == RECORDING));
  json.set("material", materials[currentMat]);
  json.set("session", sessionCount);
  json.set("status", "online");
  json.set("ts/.sv", "timestamp");
  json.set("weight", weight);
  Firebase.updateNode(fbdo, "/units/unit_01/live", json);
}