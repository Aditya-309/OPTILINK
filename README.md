# OPTILINK
OPTILINK enables secure, RF-free communication using modulated infrared (IR) light. It provides line-of-sight, interference-free data transfer with high privacy and low detectability, integrating seamlessly with IoT networks via ESP32 nodes and centralized coordination.
🧩 Overview

OPTILINK is a secure, RF-free communication system that uses infrared (IR) transmitters and receivers to establish line-of-sight digital links. It enables reliable data transmission in environments where traditional radio frequency (RF) communication is blocked, jammed, or restricted — such as defense operations, industrial plants, and secure facilities.

By modulating IR light for data transfer, OPTILINK ensures:
1.Low detectability
2.Interference-free transmission
3.Directional privacy
4.IoT integration using ESP32 microcontrollers

🚀 Objectives

Develop a multi-node IR-based Li-Fi communication network using ESP32 microcontrollers.
Establish a secure optical link capable of transmitting digital data via modulated IR signals.
Implement redundant communication via Bluetooth/Wi-Fi fallback when IR transmission is obstructed.
Measure system range, latency, bit-error rate (BER), and packet reliability.
Prototype a scalable and eco-friendly communication framework for secure IoT and defense systems.

⚙️ System Architecture
🧱 Core Components
Component	Function
*ESP32 Microcontroller	Handles IR communication, data encoding/decoding, and fallback connections (Wi-Fi/Bluetooth).
*IR Transmitter (LED)	Emits modulated IR light (typically 38 kHz) for directional, interference-resistant communication.
*HW-477 IR Receiver	Detects and demodulates incoming IR signals for ESP32 processing.
🔁 Communication Flow

1.Data Encoding: Message is converted into binary and modulated as a 38 kHz IR signal.
2.Transmission: IR LED sends the encoded light pulses in a line-of-sight direction.
3.Reception: The IR receiver decodes the modulated light into electrical signals.
4.Processing: ESP32 interprets the data, displays it, or routes it to another node.
5.Fallback: If IR communication fails, ESP32 switches to Bluetooth/Wi-Fi for redundancy.

📊 Expected Outcomes

Functional multi-node IR communication prototype
Successful text message transfer using modulated IR signals
Measurement of communication range, latency, and BER
Verified Bluetooth/Wi-Fi fallback operation
Demonstration of secure, RF-free communication in real-world conditions

🌱 Sustainability & Efficiency

Low-power IR LEDs and ESP32 deep sleep for energy efficiency
Modular and recyclable hardware design
No RF emissions, reducing electromagnetic pollution
Optimized power usage for longer endurance

🔐 Future Enhancements

End-to-end encryption modules for higher data security
AI-based routing and adaptive IR beam focusing
Integration with IoT dashboards for centralized monitoring
Development of mesh IR communication networks

🧠 Key Features

✅ RF-free, interference-proof communication
✅ Secure and directional IR data transfer
✅ IoT-ready via ESP32 nodes
✅ Smart fallback system (Wi-Fi/Bluetooth)
✅ Low power and eco-friendly operation

📷 Block Diagram (Conceptual)
[ Sender ESP32 ] → [ IR LED (38kHz) ] ⇢⇢⇢ [ IR Receiver (HW-477) ] → [ Receiver ESP32 ]
                                 ↓
                            Wi-Fi / Bluetooth Fallback

🧩 Applications

Defense & Tactical Communication
Industrial Automation
Secure IoT Infrastructure
RF-restricted or jammed environments

🧾 License

This project is developed for academic and research purposes.
For commercial or defense-grade use, please contact the authors for permissions and collaboration.
