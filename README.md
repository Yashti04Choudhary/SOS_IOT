# 🚨 IoT-Based SOS Emergency Safety Device

An IoT-based emergency safety system developed using **ESP32**, **GSM**, and **GPS** modules to provide real-time emergency alerts and live location tracking during panic situations.

The device is capable of:

* Sending SOS SMS alerts
* Sharing live GPS coordinates
* Initiating emergency phone calls
* Activating panic mode alerts in real time

This project was developed as an exploratory embedded systems and IoT project to understand real-world integration of communication modules, emergency response mechanisms, and hardware-software interaction. While the project was primarily built for learning and experimentation, the core functionalities were successfully implemented and tested.

---

# 📌 Features

* 📍 Real-time GPS location tracking
* 📩 GSM-based SOS SMS alerts
* 📞 Automatic emergency calling
* 🚨 Panic mode activation
* 🔔 Buzzer alert system
* ⚡ ESP32-based embedded control
* 🌐 IoT-oriented emergency communication
* 🔋 Low-cost portable prototype

---

# 🛠️ Technologies & Components Used

## Hardware

* ESP32 Microcontroller
* GSM Module (SIM800L / SIM900A)
* GPS Module (NEO-6M)
* Push Button / Panic Switch
* Buzzer
* Breadboard & Jumper Wires
* Power Supply Module

## Software

* Arduino IDE
* Embedded C / Arduino Programming
* UART Serial Communication
* AT Commands for GSM Communication

---

# ⚙️ System Workflow

1. Device initializes ESP32, GSM, and GPS modules
2. User activates panic button
3. ESP32 enters emergency mode
4. GPS module fetches current coordinates
5. GSM module sends SOS SMS alerts
6. Emergency call is initiated automatically
7. Buzzer activates for local alert indication
8. System continues emergency monitoring

---

# 📂 Repository Structure

```plaintext
SOS-IOT-Safety-Device/
│
├── circuit/
│   ├── real_circuit.jpg
│   └── circuit_design.jpg
│
├── SOS_Device_code/
│   └── SOS_Device.ino
│
├── UI_images/
│   ├── ui_3.jpg
│   ├── ui_4.jpg
│   ├── ui_5.jpg
│   └── ui_6.jpg
│
├── SOS_report.docx
│
├── README.md
│
└── .gitignore
```

---

# 🚀 Applications

* Women Safety Systems
* Elderly Emergency Assistance
* Child Safety Monitoring
* Personal Security Devices
* Smart Wearable Safety Systems
* Emergency Response Solutions

---

# 🔮 Future Improvements

* Mobile app integration
* Cloud-based live tracking
* Rechargeable battery support
* AI-based danger detection
* Voice-triggered SOS activation
* Compact wearable implementation

---

# 📖 Learning Outcomes

This project helped explore:

* Embedded systems development
* GSM and GPS communication
* Serial/UART interfacing
* IoT device integration
* Real-time emergency systems
* Hardware-software debugging

---

# ⚠️ Project Note

This project was created mainly as an exploratory and educational implementation to understand IoT-based emergency response systems and hardware integration concepts. Although it is a prototype-level system, the core emergency functionalities were implemented successfully and tested under controlled conditions.

---

# 👨‍💻 Authors

Developed as a mini-project for academic and learning purposes.

---

# 📜 License

This project is intended for educational and non-commercial use.
