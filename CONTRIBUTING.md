# Contributing to Flatball INS

Thanks for your interest in contributing to Flatball INS — a multi-year, open-source effort to explore frisbee flight dynamics through embedded sensors, repeatable launch systems, and 3D visualization.

We welcome collaborators from all backgrounds, whether you're into embedded systems, aerospace modeling, visualization, or just love frisbee and physics.

## 🧠 Contribution Philosophy

This project values:

- **Empirical rigor** — contributions should be grounded in testable logic and reproducible workflows  
- **Readable outputs** — code, documentation, and data must be copy/paste-proof and batch-friendly  
- **Transparent reasoning** — calibration, axis conventions, and parametric relationships must be clearly documented  
- **Rapid iteration** — we expect fast feedback loops and error correction without drama  
- **Scalable design** — everything should be deployable across multiple discs, launchers, and environments

## 📜 Licensing Terms

By submitting a contribution, you agree to:

- License your work under the **Apache License 2.0**  
- Grant the project maintainers the right to relicense or publish derivative works, including scientific papers and datasets  
- Retain attribution and include a notice of modification in any changed files  
- Avoid submitting code or designs that you do not have the rights to share

## 📂 What You Can Contribute

- Firmware for sensor logging and calibration  
- Launcher control code and mechanical design improvements  
- Visualization tools (e.g., Three.js modules, trajectory playback)  
- Documentation, diagrams, and calibration workflows  
- Flight data logs and annotated test results  
- CFD simulations and aerodynamic modeling

## 🛠️ Getting Started

### System Requirements

Before you can contribute, you will need to install several system-level tools. This project involves multiple disciplines, including hardware design, firmware development, and data analysis.

Please install the following software using your operating system's recommended package manager (e.g., Homebrew on macOS, Chocolatey/Winget on Windows, or `apt`/`dnf` on Linux).

**Core Tools:**
*   **Git:** For version control.
*   **Node.js:** LTS version is recommended. Used for the 3D visualization components.
*   **Python:** Version 3.11 or newer. Used for data analysis and utility scripts.
*   **Visual Studio Code:** The recommended code editor for this project.

**Hardware & Firmware:**
*   **KiCad:** For PCB design.
*   **PlatformIO CLI:** For building and uploading the firmware. This can be installed as a VSCode extension or via `pip`.

**Mechanical Design & Documentation:**
*   **FreeCAD:** For mechanical design of brackets and enclosures.
*   **LibreOffice:** For viewing and editing project documentation in `.odt` format.
*   **Ultimaker Cura:** For slicing 3D models for printing.

### Installation

Once you have the system requirements installed, you can set up the project for development.

1.  **(Optional) Verify Your Environment:**
    After installing the tools listed above, you can run this command to verify they are all accessible from your terminal.
    ```bash
    npm run check:env
    ```

2.  **Clone the Repository:**
    ```bash
    git clone https://github.com/misc-protolabs/flatball-ins.git
    ```

3.  **Navigate into the Project Directory:**
    ```bash
    cd flatball-ins
    ```

4.  **Install Dependencies:**
    This command installs dependencies for the root project and also automatically triggers the installation for the 3D visualization project.
    ```bash
    npm install
    ```

5.  **Run the Development Server:**
    This command starts the development server for the 3D visualization scene.
    ```bash
    npm start
    ```
    You should now be able to access the application in your web browser at the address provided (usually `http://localhost:5173`).

### Contribution Workflow

1. Fork the repository and clone your fork  
2. Create a feature branch (`git checkout -b feature/your-feature-name`)  
3. Make your changes and commit with clear messages  
4. Submit a pull request with a description of your contribution  
5. Engage in review — we value clarity, reproducibility, and technical depth

## 📣 Communication

We use GitHub Issues and Discussions for technical dialogue. Please tag issues with `help wanted`, `good first issue`, or `calibration` as appropriate.

---

We’re excited to build something novel, rigorous, and open-source — and we’re glad you’re here to help make it fly.
