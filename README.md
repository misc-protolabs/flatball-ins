# Flatball Inertial Navigation System
Advanced frisbee flight dynamics, sensor fusion, and aerodynamics research — open-source platform for precise onboard IMU logging, 3D trajectory playback, and CFD-backed airfoil modeling.

[![License: Apache-2.0](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/build-none-lightgrey)]()
[![Contributors](https://img.shields.io/github/contributors/misc-protolabs/flatball-ins)](https://github.com/misc-protolabs/flatball-ins/graphs/contributors)
[![Issues](https://img.shields.io/github/issues/misc-protolabs/flatball-ins)](https://github.com/misc-protolabs/flatball-ins/issues)

---

## Elevator pitch
Precise, repeatable frisbee flight datasets and open tooling for 3D visualization and aerodynamic validation. We provide hardware, firmware, simulation, and web tools so researchers, athletes, and makers can analyze and reproduce frisbee flight with scientific rigor.

---

## Featured media
- Launcher concept: `arxiv/vid/launcher-concept-spin-test.mov`  
- AHRS demo: `arxiv/vid/prototype-pcb-ahrs-demo.webm`

(Consider replacing these links with embedded thumbnails or a single highlight video for the project landing page.)

---

## Open call — how to help
We welcome contributions across hardware, firmware, simulation, and visualization. Typical ways to join:
- Open an issue to propose work or report data
- Pick a "Start here" onboarding issue from the project board
- Submit a focused PR (see CONTRIBUTING.md)

Who we need
- Embedded engineers and PCB designers
- Sensor fusion and realtime programmers
- CFD and aerodynamics modelers
- Web visualization engineers (Three.js, D3)
- Outreach and documentation contributors

Start here:
- Hardware onboarding: admin/mike-schaefer/md/repo-admin.md#hardware
- Firmware onboarding: admin/mike-schaefer/md/repo-admin.md#firmware
- Visualization onboarding: admin/mike-schaefer/md/repo-admin.md#visualization
- CFD onboarding: admin/mike-schaefer/md/repo-admin.md#cfd

---

## What we build
- Custom PCB mounted to a Discraft 175g Ultra-Star for onboard logging:
  - 3-axis accel, gyro, magnetometer
  - Barometer, temperature, battery voltage
- Twin-motor launcher for reproducible 6DOF launch conditions
- Flight path reconstruction from onboard telemetry and ground tracking
- CFD (OpenFOAM) airfoil simulations and 3D-printed prototype testing
- Web-based 3D visualization and playback using Three.js

---

## Project goals (short list)
- Deliver a sharable dataset of repeatable throws with synchronized IMU and telemetry
- Provide an open PCB and firmware reference for instrumented frisbees
- Validate aerodynamic and rigid-body models across common throw types
- Ship a web-based replay tool for interactive analysis and publication-ready figures

---

## Quick status (example)
- Labels and milestones are config-driven in `admin/json/labels.json` and `admin/json/milestones.json`
- Sync tooling: `admin/mike-schaefer/py/sync-github-config.py`
- Repo admin docs: `admin/mike-schaefer/md/repo-admin.md`

---

## Roadmap (high level)
- onboarding/v1.0 — contributor docs, labels, and starter issues
- infra/sync-scripts — reliable config-driven label/milestone sync + audit logs
- hardware/pcb-v1.0 — design freeze and PCBA V&V
- math/flight-prototype — quaternion-based pose and trajectory pipeline
- visualization/v1.0 — Three.js playback and publication export

Acceptance criteria: one reproducible experiment with full data, one validated PCB V&V run, and a public Three.js replay.

---

## Contributing & governance
- Read `CONTRIBUTING.md` before submitting contributions.
- Contributions are licensed under **Apache License 2.0**. Scientific data and papers are CC-BY unless stated otherwise. See `NOTICE` for trademark rules.
- Maintainability rules: config-driven labels/milestones, descriptive PRs, and automated sync logs in `admin/logs-and-reports/`.

---

## Code of conduct
Be respectful. Inclusive collaboration is required. See `CODE_OF_CONDUCT.md` for details.

---

## Funding & sponsorship
Interested in sponsoring the project? See `admin/mike-schaefer/md/funding.md` for tiers and contact channels.

---

## Links
- Repository: https://github.com/misc-protolabs/flatball-ins  
- Admin docs: `admin/mike-schaefer/md/repo-admin.md`  
- Ultimate history and context: `admin/md/ultimate-history.md`, `admin/md/UFA.md`

---

## Contact
Open an issue, or reach out via the repository issue tracker to start collaborating.

---

Thank you for helping build reproducible, rigorous, and open frisbee flight science.
