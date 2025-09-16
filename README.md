# Flatball Inertial Navigation System: Advanced Frisbee Flight Dynamics & Aerodynamics Research

## 🌀 Open Call: Help Us Decode Frisbee Flight

We’re building an open-source platform to analyze and visualize frisbee flight — from onboard IMU logging to 3D trajectory playback and CFD-backed airfoil modeling. If you’re into embedded systems, real-time sensor fusion, or just want to help frisbees fly smarter, this is your launchpad.

## What’s in motion:

🛠️ Custom PCB for onboard IMU logging  
🚀 Twin-motor frisbee launcher for repeatable flight studies

	➡️ [`launcher-concept-spin-test.mov`](arxiv/vid/launcher-concept-spin-test.mov)
 
🌐 Three.js-based 3D visualization of flight trajectories  

	➡️ [`prototype-pcb-ahrs-demo.webm`](arxiv/vid/prototype-pcb-ahrs-demo.webm)
 
✈️ Airfoil design iterations using 3D-printed prototypes
📍 Flight path reconstruction from live telemetry and datalogging

## We’re looking for:
- Embedded devs, PCB designers, and sensor wranglers
- Web-based visualization builders (Three.js, D3, etc.)
- CFD modelers and aerodynamic tinkerers
- Outreach-minded collaborators who care about open sports-tech

## Why it matters:
Frisbee flight is beautiful, chaotic, and underexplored. We want to make flight analytics accessible — not just for researchers, but for athletes, makers, and curious minds everywhere.
Join us. Open an issue. Let’s build something that flies.

## 🥏 Ultimate Frisbee & UFA Spotlight

This project is deeply inspired by the athleticism, precision, and innovation seen in professional Ultimate.  
Explore the evolution of the sport and the rise of the Ultimate Frisbee Association (UFA):  
➡️ [`ultimate-history.md`](admin/md/ultimate-history.md)  
➡️ [`UFA.md`](admin/md/UFA.md)

## 🎯 Project Goals

- Develop a custom PCB mounted to a Discraft 175g Ultra-Star that logs:
  - 3DOF accelerometer, gyroscope, magnetometer
  - Barometric pressure, temperature, battery voltage
- Build a launcher capable of repeatable 6DOF launch conditions using twin e-bike hub motors
- Validate aerodynamic models across throw types (forehand, backhand, hammer, scoober)
- Simulate airfoil shape changes using CFD (OpenFOAM) and predict trajectory impacts
- Visualize flight data in 3D using Three.js and web-based tools

## 🧠 Skills We're Looking For

| Domain | Skills |
|--------|--------|
| Embedded Systems | Bluetooth, WiFi, SD/MMC, USB, sensor integration |
| Realtime Programming | Sensor fusion, I2C/SPI, IMU calibration |
| Aerospace Engineering | 6DOF rigid body modeling, aerodynamic coefficient estimation |
| Visualization | Three.js, Node.js, browser-based 3D rendering |
| Hardware Prototyping | PCB design, launcher fabrication, disc integration |
| Math & Physics | Quaternions, rotation matrices, Euler angles |

## ⚖️ Licensing

- All software and hardware files are licensed under the **Apache License 2.0**  
- Scientific data and papers are licensed under **Creative Commons Attribution (CC-BY)**  
- Trademark use is restricted; see `NOTICE` for details

## 🤝 Contributing

We welcome collaborators! Please read `CONTRIBUTING.md` before submitting code, hardware designs, or documentation. By contributing, you agree to license your work under Apache 2.0 and grant the project maintainers the right to relicense or publish derivative works, including scientific papers and datasets.

## 💰 Funding Opportunities

Interested in supporting this project?  
See [funding.md](admin/mike-schaefer/md/funding.md) for details on sponsorship tiers, collaboration channels, and impact goals.

## 📡 GitHub Repository

Explore the code and contribute here:  
👉 [https://github.com/misc-protolabs/flatball-ins](https://github.com/misc-protolabs/flatball-ins)

📘 For detailed repo administration and workflow docs, see [repo-admin.md](admin/mike-schaefer/md/repo-admin.md).

---

Whether you're an engineer, maker, coder, or frisbee fanatic — this is your invitation to help build something novel, rigorous, and open-source.
