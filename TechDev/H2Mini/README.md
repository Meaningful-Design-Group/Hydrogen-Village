# H2MINI: Hydrogen-Powered DC/AC Charging Station (1 kW)

## 1. Background & Project Purpose

To bring green hydrogen technology to a scale usable in daily life across the communities of Pulau Serangan, this research stream develops compact, practical prototypes that respond to how residents move, cook, and work. The approach prioritises durability, simple maintenance, and locally achievable manufacturing — designing devices that function in harmony with the rhythms of island life. The aim is to turn prototypes into deployable solutions that improve mobility and everyday energy access in Pulau Serangan.

A central part of this work is the use of fuel cells as the primary method for converting hydrogen into usable energy. Fuel cells allow hydrogen to power devices quietly and efficiently, without combustion. The process can be summarised simply as:

1. Hydrogen enters the system.
2. Hydrogen meets oxygen extracted from the air.
3. The fuel cell converts both gases into electricity.
4. Water vapour is exhausted as the only by-product.

There is no combustion and no moving parts inside the fuel cell, making the system quiet, safe, and easy to maintain. This "hydrogen-in, electricity-out" process allows the design of tools that fit into everyday island life. By grounding the prototype in fuel cell technology, this research stream bridges hydrogen from an industrial concept into a usable technology for local communities.

The R&D project for H2MINI aims to design a self-sufficient (off-grid) charging unit based on clean energy, to support the local community's electric vehicle ecosystem — particularly e-bikes — in Pulau Serangan, Bali. Using a frugal engineering approach, the system uses a 1 kW Proton Exchange Membrane Fuel Cell (PEMFC) from Horizon. The design prioritises a product form familiar to the community, physical durability suited to a coastal environment, and radical efficiency in hydrogen gas consumption through an automated sequential control system based on a microcontroller.

### H2MINI — Hydrogen-Powered DC Charging Station

![h2mini](https://drive.google.com/file/d/1pqDcvSiBaJG_SEbkY2bgmf-fyzJeARlt/view?usp=sharing)

Hydrogen can be used to create green hydrogen, an energy source capable of producing energy without emitting smoke, fumes, or carbon emissions. Unlike petrol, which can only be bought at conventional petrol stations, green hydrogen can be generated from renewable sources like sunlight, water, and organic waste.

H2MINI is a project aiming to create a compact charging station to power e-bikes, electronic devices, and small DC appliances for island and coastal communities. The idea came from observing children in Pulau Serangan who rely on e-bikes for daily mobility. Seeing this, H2MINI asked a simple question: what if daily household DC appliances and devices could be charged with hydrogen?

In a place where small petrol kiosks shape everyday mobility, H2MINI imagines an alternative: charging points replacing the kiosks, providing clean hydrogen energy to the community. A society where energy is generated and used locally, reducing the need for external purchases of fossil fuels.

The Hydrogen Village project explores this shift — moving petrol sold in glass bottles to green energy produced on-site. Replacing combustion engines with clean electrochemical conversion, and moving from carbon dependence to community-scale sustainability.

---

## 2. Technical Specification

| Technical Parameter | Specification / Nominal Value |
|---|---|
| Fuel cell type | Horizon PEM Fuel Cell H-1000W |
| Rated power | 1000 W (1 kW) continuous output |
| Stack electrical performance | 28.8 V @ 35 A |
| H2 input pressure | 0.55–0.65 bar |
| Hydrogen purity | ≥99.995% dry H2 |
| Buffer battery system | 50 Ah capacity battery buffer |
| Inverter output | DC to AC pure sine wave, 1000 W (220 V AC, 50 Hz) |
| Main auto cut-off control system | ESP32 with Finite State Machine (FSM) architecture |

---

## 3. Mechanical Design & Component Layout

To bridge community acceptance of this advanced technology, the unit reuses the chassis of a conventional roadside petrol kiosk (Pertamini) — an upcycling approach. Components are arranged strategically by function:

### 3.1 Gas Fluid Architecture & External Hydrogen Supply Integration

The high-pressure pure hydrogen gas cylinder is placed outside the main chassis (external) for air circulation safety and ease of cylinder replacement (regasification).

The hydrogen inlet piping extends from the lower side of the body with a sturdy mount visually resembling a side mirror. At the end of this "mirror" structure, a pressure gauge is fitted, allowing the operator to monitor remaining cylinder pressure directly from outside without opening the body.

All internal hosing toward the purging valve (<20 cm), the purge valve outlet line (<30 cm), and the minimum internal hose diameter (>2 mm) strictly follow Horizon's manual specifications, ensuring reliable release of accumulated water vapour (H2O).

### 3.2 Rear Control & Power Distribution Panel (External Rear)

To maximise internal space and ease of maintenance access, protection components, indicators, and power cut-offs are mounted on a vertical panel on the rear exterior of the body:

- **MPPT Charge Controller (60A, 24–48V)** — positioned centrally on the rear panel as the hub for hybrid charging control to the buffer battery.
- **Main Switch & Protection Box (cream box)** — the cream-coloured external panel housing AC & DC MCBs (Miniature Circuit Breakers), serving as manual power interruption and overcurrent protection.
- **Auto Cut-Off Control System (black box)** — a black projecting enclosure containing the ESP32 microcontroller, voltage divider sensor circuitry, and relay interaction module, placed adjacent to the cream MCB box to simplify sensing cable integration.

### 3.3 Internal Body Compartment

The interior of the Pertamini chassis is fully utilised as a protected space for core components:

- The Horizon 1 kW Fuel Cell Stack unit, with its factory-supplied controller module.
- The 50 Ah Battery Buffer circuit.
- The 1000 W Pure Sine Wave Inverter unit and DC-DC step-up/step-down converters.

### 3.4 Active Thermal Management System (Dual-Fan)

The 1 kW PEMFC's operational characteristics generate significant heat dissipation inside an enclosed compartment. An active thermal management system using a dual-fan configuration with cross airflow is applied:

- **Exhaust fan** — mounted on the left side of the body (adjacent to the fuel cell's internal inlet area). This fan draws and expels hot air from inside the compartment to the outside.
- **Intake fan** — mounted at the bottom of the rear panel (directly below the cream MCB box). This fan draws fresh, cool air from the outside environment and channels it directly to the fuel cell stack's lower vents.

### 3.5 Electrical Architecture Engineering & Modification

The largest engineering focus in this research was redesigning the secondary electrical architecture around the fuel cell. Because the fuel cell stack's baseline characteristics are not responsive to the transient load spikes typically caused by e-bike chargers, the R&D team added a series of power management, energy balancing, and smart automation components not present in the standard factory package.

#### Modified Wiring System Analysis

Based on the developed electrical circuit diagram (Electrical Circuit — Fuel Cell Charging Station), the modified interconnection paths are divided into several main flow blocks:

**1. Main generation path (standard factory link)**

The positive (FC+) and negative (FC-) terminals of the Fuel Cell Stack connect directly via high-current cabling to the power terminal on Horizon's factory-supplied Fuel Cell Controller. All temperature sensors, internal blower, H2 supply valve, and purging valve connect to this main controller to preserve the integrity of the manufacturer's chemical system.

**2. Voltage regulation & current conditioning path (modified)**

The fluctuating DC current from the Load+ and Load- terminals on the Fuel Cell Controller is modified and split into two paths:

- *Control system supply path:* Current from Load+ branches into a **Step-Down DC to DC 13V** module to produce a stable 13V supply that powers the Automatic Cut-Off System (black ESP32 box).
- *Main charging path:* Main power flows through a **Step-Down DC to DC 24V 30A** module to reduce voltage, then is re-stabilised through a **Step-Up DC to DC 24–60V 40A** module. The conditioned voltage passes through an **MCB DC** overcurrent protector before entering the input terminal (Solar/PV Input) of the **MPPT Charge Controller 60A**.

**3. Energy buffering & charging management path (modified)**

The battery output terminal on the MPPT Charge Controller connects to the **Battery 50Ah (Buffer)** terminal through a **Relay Battery (RELAY_BAT)** switch interruption, controlled by the ESP32. This battery acts as an energy reservoir to balance transient load spikes.

**4. AC load distribution path (modified)**

The DC input terminal on the **1000W Pure Sine Wave Inverter** connects in parallel directly to the **Battery 50Ah (Buffer)** terminal using heavy-gauge wiring. The 220V AC output voltage from the inverter passes through the **MCB AC** on the rear panel before reaching the external socket (Load) to supply community e-bike chargers.

**5. Automatic integration path (modified)**

The black ESP32 control box connects in parallel to the physical on/off button on the Fuel Cell Controller via the RELAY_OUT pin (Pin 19). Meanwhile, the TRIGGER_PIN (Pin 14) connects to a voltage divider circuit at the battery terminal to read power capacity status.

---

## 4. Electrical Engineering & Intelligent Auto Cut-Off Logic

### 4.1 Role of the Battery Buffer & Inverter

Hydrogen fuel cells respond slowly to the transient load spikes typically caused by e-bike chargers. This issue is resolved by integrating a 50 Ah Battery Buffer as a short-term energy reservoir. The stable DC output from the battery is then converted by the 1000W Pure Sine Wave Inverter into 220V AC, allowing safe direct use with community e-bike chargers without risking membrane degradation in the fuel cell.

### 4.2 Standby Gas Efficiency Problem

In a standard installation, even once the buffer battery is fully charged, the fuel cell stack remains in idle standby mode. In this mode, the purging valve continues to periodically vent gas, resulting in massive wastage of pure hydrogen gas without producing any useful work.

### 4.3 Finite State Machine (FSM) Algorithm Analysis

To address this, the R&D team designed a smart ESP32-based control system using a sequential Finite State Machine (FSM) method. The system reads a voltage trigger signal (via TRIGGER_PIN) and controls two relay modules: RELAY_OUT (simulating a button press on the Horizon controller) and RELAY_BAT (connecting/disconnecting the physical battery charging path).

The FSM executes two main sequences automatically and safely:

**1. Charging sequence (LOW signal)**

- As battery voltage drops, the trigger pin switches to LOW.
- *Step 1 (STEP_RELAY_OUT_ON):* The ESP32 immediately activates RELAY_OUT for 2000 ms (2 seconds) to simulate pressing the startup button on the Horizon controller, then deactivates it (STEP_RELAY_OUT_OFF).
- *Step 2 (chemical stabilisation):* The system does not immediately connect the fuel cell to the battery. The program forces a wait of 120,000 ms (2 minutes) (FUELCELL_STABILIZE_MS). This delay is critical, giving the fuel cell membrane time to warm up, stabilise internal hydrogen pressure, and reach nominal electrochemical efficiency.
- *Step 3 (STEP_FINAL):* After the 2-minute period, RELAY_BAT is activated (HIGH) to open the energy charging path from the fuel cell to the buffer battery.

**2. Battery full sequence (HIGH signal)**

- When the sensor detects the battery is fully charged, the trigger pin switches to HIGH.
- *Step 1 (STEP_RELAY_OUT_ON):* The ESP32 activates RELAY_OUT for 4000 ms (4 seconds). This longer press duration is universally read by the Horizon controller as a definitive instruction to begin a safe shutdown sequence (powering down the system and fully closing the hydrogen supply valve).
- *Step 2 (STEP_FINAL):* Once the button is released after 4 seconds, RELAY_BAT immediately deactivates (LOW) to isolate the buffer battery, halt total hydrogen gas consumption, and achieve 100% gas efficiency when the charging station is not in use.

### 4.4 Safety Features & Code Protection Systems

- **Anti-debounce system (DEBOUNCE_MS = 50UL):** Prevents false triggering caused by minor voltage fluctuations or electrical noise at the battery terminal. The trigger signal must remain stable for at least 50 milliseconds before being treated as valid data.
- **Runtime protection interrupt:** If a sequence is mid-run (running = true) and the trigger pin's logic state changes suddenly, the ESP32 immediately cancels the sequence for electrical safety and executes resetSystem().
- **Global safety timeout (SAFETY_TIMEOUT_MS = 5 minutes):** If the factory controller fails or a sequence becomes stuck, leaving the process incomplete after 5 minutes, the algorithm automatically disconnects all relays and returns the system to a safe state (STEP_IDLE).

---

## 5. Main Program Implementation (ESP32 FSM)

### FSM State Diagram

```
                    STEP_IDLE
                   /          \
          TRIGGER_LOW      TRIGGER_HIGH
              |                  |
   CHARGING SEQUENCE      BATTERY FULL SEQUENCE
   1. RELAY_OUT ON         1. RELAY_OUT ON
      (2 sec) →               (4 sec) →
      FC Startup Sequence     FC Shutdown Sequence
   2. RELAY_OUT OFF         2. RELAY_OUT OFF
   3. Wait 2 minutes →      3. RELAY_BATTERY OFF
      Chemical                 (instant) →
      stabilisation            Isolate battery load
   4. RELAY_BATTERY ON →    4. Return to STEP_IDLE
      Begin charging
```

### Firmware (initial setup and FSM definitions)

```cpp
#include <FluxGarage_RoboEyes.h>

// ESP32 microcontroller pin mapping
#define TRIGGER_PIN 14   // Buffer battery voltage sensor input
#define RELAY_OUT   19   // Relay output (parallel to Horizon controller button)
#define RELAY_BAT   5    // Main relay for battery path isolation/connection

// System timing constants (in milliseconds)
const unsigned long DEBOUNCE_MS = 50UL;                    // Signal noise filter, 50ms
const unsigned long RELAY_OUT_ON_CHARGE_MS = 2000UL;       // FC startup pulse (2 seconds)
const unsigned long FUELCELL_STABILIZE_MS = 120000UL;      // Stack chemical stabilisation time (2 minutes)
const unsigned long RELAY_OUT_ON_FULL_MS = 4000UL;         // FC shutdown pulse (4 seconds)
const unsigned long SAFETY_TIMEOUT_MS = 5UL * 60UL * 1000UL; // Global emergency timeout (5 minutes)

unsigned long prevMillis = 0;
unsigned long lastDebounceMillis = 0;
unsigned long runningStartMillis = 0;

int lastReadState = HIGH;   // Raw pin reading memory
int stableState = HIGH;     // Pin status after debounce filtering
int startState = HIGH;      // Initial status when FSM sequence executes
bool running = false;

// FSM architecture stage definitions
enum Step {
  STEP_IDLE = 0,
  STEP_RELAY_OUT_ON,
  STEP_RELAY_OUT_OFF,
  STEP_FINAL
};

Step step = STEP_IDLE;
```

*[Remaining firmware logic — main loop, debounce handling, and the two sequence branches — to be added as the implementation is finalised.]*

---

## 6. System Diagram

```
[Hydrogen Tank] → [Charging Station]
                      ├── Power Outlet
                      └── Appliances / Devices
```

*[Diagram: "Desa Hidrogen Hijau" charging station illustration — hydrogen tank feeding into charging station with power outlet for e-bike/device charging. See attached photos for full visual.]*

### Electrical Circuit — Fuel Cell Charging Station

```
LOAD → MCB AC → Inverter (DC to AC, 1000W)
                    ↑
              Battery 50Ah (Buffer)
                    ↑
        MPPT 60A (24–48V) ← Step-Up DC-DC (24–60V, 40A) ← Step-Down DC-DC (24V, 30A)
                                                                    ↑
                                                              MCB DC
                                                                    ↑
                                                          Fuel Cell Controller ← Load+ / Load- / FC+ / FC-
                                                                    ↑
                                                              Stack (Fuel Cell)
                                                                    ↑
                                        H2 Supply Valve / Blower / T Sensor / Purging Valve
                                                                    ↑
                                                          Step-Down DC-DC (13V) → SCU (ON/OFF) → Automatic Cut-Off System
```

*[See attached photos for full circuit diagram.]*

---

## 7. Engineering Conclusion

Through the synchronisation of external physical layout (Pertamini kiosk body, external cylinder placement, mirror-style pressure indicator) and the sophisticated Finite State Machine (FSM) algorithm design in the rear panel, the H2MINI unit successfully transforms into a highly safe, self-sufficient charging station.

The addition of a 2-minute membrane stabilisation delay ensures the longevity of the Horizon 1 kW core component, protecting it from degradation risk due to transient load spikes, while the automatic 4-second shutdown pulse successfully eliminates total wastage of pure hydrogen gas during the standby phase. This project is ready to be reliably implemented in the coastal environment of Desa Hidrogen Hijau, Pulau Serangan, Bali.

---

## 8. Local Modification Summary

To transform the fuel-cell stack into a practical off-grid charging solution, the team developed an additional electrical architecture around the manufacturer's system. This adaptation follows the spirit of frugal engineering — allowing the fuel cell to operate reliably in real-world conditions and deliver stable, safe energy to residents. The result is a hybrid hydrogen charging unit capable of powering e-bikes, phones, small appliances, and other everyday devices.

| # | Modification | Purpose |
|---|---|---|
| 1 | Battery Buffer (50Ah) | Fuel cells do not respond well to sudden or high loads. The battery buffer creates a stable energy reservoir that absorbs sudden spikes from e-bike chargers or devices, smooths the load so the fuel cell can operate within a healthy range, and keeps the system running briefly even when the fuel cell ramps down. |
| 2 | MPPT Charge Controller (60A, 24–48V) | Increases flexibility — the MPPT controller acts mainly as a controller to charge the battery when needed from the fuel cell, but also allows the system to accept solar input in future. This hybrid configuration reduces hydrogen consumption and increases operational resilience. |
| 3 | DC-DC Converters | Fuel cell voltage rises and falls depending on demand. To protect appliances and ensure consistent performance, a step-up converter stabilises higher-voltage lines, and a step-down converter (including a dedicated 13V module) serves sensitive equipment. These converters keep power output predictable and safe. |
| 4 | Inverter (DC → AC, 1000W) | Most community devices operate on 220V AC, not DC. The inverter transforms the battery's DC output into AC electricity so the station can function like a standard outlet, powering appliances and e-bike chargers. This is the bridge that makes hydrogen energy accessible at consumer scale. |
| 5 | Safety & protection layers | Because the system is intended for public use, critical safety mechanisms were added: DC and AC MCBs to prevent overcurrent and short circuits, an automatic cut-off system that protects the fuel cell from voltage drops or overloads, and simplified wiring pathways to minimise risk during operation. These measures ensure the station remains robust under unpredictable community use. |

With these modifications, the fuel cell system was retrofitted into a wagon previously used to sell petrol stored in glass bottles, where it becomes a practical energy source for the community. The station can now charge e-bikes and phones, power small appliances in businesses, and run cleanly and quietly — continuing to work even when power loads change suddenly.
