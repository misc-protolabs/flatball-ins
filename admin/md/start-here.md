# START-HERE.md

## Quick onboarding (2 minutes)
1. Read CONTRIBUTING.md and admin/mike-schaefer/md/repo-admin.md.  
2. Find a starter issue labeled `good first issue`, `onboarding`, or `help wanted`.  
3. Clone the repo and check out the sandbox branch (commands below).  
4. Create a small topic branch, implement, open a draft PR, link the issue, request a reviewer.

---

## Clone and open the sandbox branch

Option A — clone then checkout (recommended)
```bash
git clone --branch sandbox --single-branch git@github.com:misc-protolabs/flatball-ins.git
cd flatball-ins
```

---

## Running the 3D Visualization (`flatball-3d-ulti-scene`)

This section guides you through setting up and running the primary 3D visualization application, which displays frisbee flight data.

### 1. Navigate to the Project Directory

Open your terminal and change into the `flatball-3d-ulti-scene` directory:

```bash
cd dev/flatball-3d-ulti-scene
```

### 2. Install Dependencies

This project requires both Node.js (for the frontend) and Python (for the telemetry backend).

**Node.js Dependencies:**

Install the JavaScript dependencies using npm:

```bash
npm install
```

**Python Dependencies:**

Install the necessary Python packages (`websockets` and `pyserial`):

```bash
pip install websockets pyserial
```

### 3. Run the Application

Start the development server. This command will concurrently launch:
*   The **Vite frontend server** (serving the web application).
*   The **Python telemetry server** (providing live data via WebSocket).

```bash
npm run dev
```

Upon execution:
*   Your default web browser should automatically open to `http://localhost:5173/`.
*   Closing the browser tab will gracefully shut down both the Vite and Python servers.

### 4. Using the 3D Visualizer

Once the application is running in your browser:

*   **Data Sources:** The application supports two primary data sources:
    *   **Live Telemetry:** Connects to the Python backend, which reads from a serial port (default `COM3`). If no device is connected, the telemetry stream will be active but silent.
    *   **CSV Playback:** Allows you to load a `.csv` file containing flight data.

*   **Switching Modes:** Use the **"Switch to CSV/Telemetry" button** in the UI to toggle between these data sources.

*   **CSV Loading:** To load a CSV file, click the file input button and select your `.csv` data. Remember to click the **Play button** to start playback.

### 5. Logging and Debugging

*   **Client-Side (Browser) Logs:**
    *   Open your browser's developer console (usually F12).
    *   Type `getLogDump()` and press Enter to download a Markdown file (`.md`) containing the application's internal JavaScript logs.

*   **Server-Side (Python) Logs:**
    *   The Python telemetry server's output is redirected to `admin/logs/telemetry-server.log`.
    *   Inspect this file for any errors or status messages from the Python backend.

---