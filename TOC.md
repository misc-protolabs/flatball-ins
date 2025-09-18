# Repository Table of Contents

Welcome to the Flatball INS project! This document is a map to help you navigate the repository.

## High-Level Structure

*   [**`README.md`**](./README.md): The main project README. Start here for a general overview.
*   [**`admin/`**](./admin/): Contains project administration, high-level documentation, reports, and demos.
    *   [**`admin/md/start-here.md`**](./admin/md/start-here.md): **Excellent starting point for new contributors.**
    *   [**`admin/demos/`**](./admin/demos/): Videos and screenshots of simulations and hardware in action.
*   [**`arxiv/`**](./arxiv/): An archive of external resources.
    *   [**`arxiv/doc/`**](./arxiv/doc/): Datasheets for all electronic components (IMUs, barometers, etc.).
    *   [**`arxiv/research-papers/`**](./arxiv/research-papers/): Key academic papers on flight dynamics and simulation.
    *   [**`arxiv/github/`**](./arxiv/github/): Zipped archives of third-party libraries and reference code.
*   [**`dev/`**](./dev/): Core development happens here! This directory contains all the engineering sub-projects.

---

## `dev/` Sub-Project Breakdown

This is where the different parts of the Flatball INS system are designed and built.

### Simulaton & Visualization

*   [**`dev/flatball-6dof/`**](./dev/flatball-6dof/): A 6-Degrees-of-Freedom (6DoF) flight simulation built in **MATLAB**. Used for algorithm development and verification.
*   [**`dev/flatball-three.js/`**](./dev/flatball-three.js/): A **Three.js** web-based 3D visualizer to play back flight data and view the disc's orientation in real-time.

### Hardware & Firmware

*   [**`dev/flatball-ahrs/`**](./dev/flatball-ahrs/): The embedded firmware for the Attitude and Heading Reference System (AHRS). This is a **PlatformIO** project written in C/C++ for the microcontroller.
*   [**`dev/flatball-imu-pcb/`**](./dev/flatball-imu-pcb/): The **KiCad** project for the Inertial Measurement Unit (IMU) Printed Circuit Board (PCB).
*   [**`dev/flatball-ins-pcb/`**](./dev/flatball-ins-pcb/): The **KiCad** project for the full Inertial Navigation System (INS) PCB, which integrates the IMU and other components.

### Mechanical Design

*   [**`dev/flatball-usau/`**](./dev/flatball-usau/): **FreeCAD** models and Python scripts for designing and generating USA Ultimate (USAU) compliant discs.
*   [**`dev/flatball-ins-brkt/`**](./dev/flatball-ins-brkt/): The **FreeCAD** design for the bracket that holds the INS electronics onto the disc.
*   [**`dev/flatball-misc/`**](./dev/flatball-misc/): Miscellaneous mechanical designs and helper scripts.

---
*This file was generated to help with project onboarding. Feel free to update it!*
