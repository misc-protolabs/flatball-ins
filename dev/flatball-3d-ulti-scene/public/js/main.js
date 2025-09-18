// Copyright 2025 Michael V. Schaefer
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

import * as THREE from './libs/three.module.js';
import { scene, camera, renderer } from './sceneSetup.js';
import { addLights } from './lights.js';
import { addGround } from './ground.js';
import { addControls } from './controls.js';
import { loadFrisbee, frisbee } from './frisbeeLoader.js';
import { addCrossHatch, addNSEWMarkers, addFieldLines, addFieldGrid } from './fieldHelpers.js';
import { initHUDs, updatePlaybackState, updateHUDs } from './hudHandler.js';
import { TelemetryHelper } from './telemetryHelper.js';
import { CsvPlayback, csvData, initCSVLoader } from './csvLoader.js';
import { renderHUD } from './hudHandler.js';
import { initPipFlow } from './pipHelper.js';
import { log, setConfig } from './utils/logger.js';

let config;
let controls;
let telemetryInstance = null;
let playStartTime = null;
let playing = false;
let telemetryActive = false;

// === Load config.json ===
async function loadConfig() {
  const res = await fetch('./public/config.json');
  config = await res.json();

  log('[Startup] Config loaded', config);
}

// === CSV load callback ===
function onCSVLoaded() {
  playStartTime = null;
  playing = false;

  log(`[CSV] Loaded ${csvData.length} rows — ready for playback`);

  if (config.dataSource.mode === 'csv') {
    if (currentDataSource && typeof currentDataSource.stop === 'function') {
      currentDataSource.stop();
    }

    currentDataSource = new CsvPlayback(csvData, handleTelemetry, 50);
    log('[CSV] ✅ CsvPlayback instance created');
  }
}

// === Telemetry packet handler ===
function handleTelemetry(data) {
  telemetryActive = true;

  updatePlaybackState( data); //data.idx*0.01 ?? 0, rollDeg, pitchDeg, yawDeg, data.gx, data.gy, data.gz);

  const scale = config.dataSource.telemetry.scale ?? 1.0;
  const rollDeg  = (data.roll  ?? 0) + (config.orientationOffsets.rollOffsetDeg  ?? 0);
  const pitchDeg = (data.pitch ?? 0) + (config.orientationOffsets.pitchOffsetDeg ?? 0);
  const yawDeg   = (data.yaw   ?? 0) + (config.orientationOffsets.yawOffsetDeg   ?? 0);

  if (frisbee && data.x !== undefined && data.y !== undefined && data.z !== undefined) {
    frisbee.position.set(data.x * scale, data.y * scale, data.z * scale);
  }

  frisbee.rotation.set(
    THREE.MathUtils.degToRad(pitchDeg),
    THREE.MathUtils.degToRad(yawDeg),
    THREE.MathUtils.degToRad(rollDeg),
    'ZYX'
  );
}

// === Playback loop (CSV fallback) ===
function animate(timestamp) {
  requestAnimationFrame(animate);

  if (!telemetryActive && playing && csvData.length > 0) {
    if (!playStartTime) playStartTime = timestamp;

    const simTime = ((timestamp - playStartTime) / 1000) * config.playback.timeScale;
    const lastTime = (csvData.length - 1) * config.playback.frameInterval;

    const clampedTime = Math.min(simTime, lastTime);
    if (clampedTime >= lastTime) {
      playing = false;
      log('[Playback] Reached end of CSV');
    }

    const idx = Math.floor(clampedTime / config.playback.frameInterval);
    const row = csvData[idx];
    if (row) handleTelemetry(row);
  }

  updateHUDs();
  controls?.update();
  renderer.render(scene, camera);
}

// === Wait for frisbee before placing NSEW markers ===
function waitForFrisbeeThenAddMarkers(maxTries = 50, interval = 100) {
  let tries = 0;
  const poll = () => {
    if (frisbee?.position) {
      log('[Startup] Frisbee ready — adding NSEW markers');
      addNSEWMarkers(scene, config);
    } else if (tries++ < maxTries) {
      setTimeout(poll, interval);
    } else {
      log('[Startup] Frisbee not ready — NSEW markers skipped');
    }
  };
  poll();
}

// === Toggle button for switching data source ===
function addDataSourceToggle(startDataSource) {
  const btn = document.createElement('button');
  btn.textContent = `Switch to ${config.dataSource.mode === 'telemetry' ? 'CSV' : 'Telemetry'}`;
  Object.assign(btn.style, {
    position: 'absolute',
    top: '10px',
    right: '10px',
    zIndex: '1000',
    padding: '6px 12px',
    fontSize: '14px',
    backgroundColor: '#222',
    color: '#fff',
    border: '1px solid #555',
    cursor: 'pointer',
  });

  btn.onclick = () => {
    const newMode = config.dataSource.mode === 'telemetry' ? 'csv' : 'telemetry';
    config.dataSource.mode = newMode;

    btn.textContent = `Switch to ${newMode === 'telemetry' ? 'CSV' : 'Telemetry'}`;

    // Reset playback state if needed
    telemetryActive = false;
    playStartTime = null;
    playing = config.playback.autoPlayOnLoad;

    // Start the new data source
    startDataSource(newMode);
  };

  document.body.appendChild(btn);
}

// === Start data source based on config ===
let currentDataSource = null;

function startDataSource(mode) {
  // Gracefully stop any existing stream
  if (currentDataSource && typeof currentDataSource.stop === 'function') {
    currentDataSource.stop();
    log(`[DataSource] 🔄 Switched from ${config.dataSource.mode}`);
  }

  // Update config
  config.dataSource.mode = mode;

  if (mode === 'telemetry') {
    currentDataSource = new TelemetryHelper(config.dataSource.telemetry, handleTelemetry);
  } else {
    if (csvData.length === 0) {
      log('[CSV] ⚠️ No data loaded—prompting user to select a file');

      // Trigger file input click
      const input = document.getElementById('csvInput');
      if (input) {
        input.click();
      } else {
        alert('CSV input element not found.');
      }

      return;
    }

    currentDataSource = new CsvPlayback(csvData, handleTelemetry, 50);
    currentDataSource.start();
  }

  log(`[DataSource] ✅ Started ${mode} mode`);
}

function addPlayPauseButton(onToggle) {
  const btn = document.createElement('button');
  btn.textContent = '▶️ Play';
  Object.assign(btn.style, {
    position: 'absolute',
    top: '50px',
    right: '10px',
    zIndex: '1000',
    padding: '6px 12px',
    fontSize: '14px',
    backgroundColor: '#222',
    color: '#fff',
    border: '1px solid #555',
    cursor: 'pointer',
  });

  let isPlaying = false;

  btn.onclick = () => {
    isPlaying = !isPlaying;
    btn.textContent = isPlaying ? '⏸️ Pause' : '▶️ Play';
    onToggle(isPlaying);
  };

  document.body.appendChild(btn);
}

// === Initialization ===
async function init() {
  log('Initializing application');
  await loadConfig();
  initCSVLoader(onCSVLoaded);

  // Scene setup
  scene.up.set(0, 1, 0);
  camera.up.set(0, 1, 0);
  scene.background = new THREE.Color(config.scene.skyColor ?? '#87CEEB');

  const camPos = config.camera?.startPosition ?? { x: 0, y: 10, z: -30 };
  camera.position.set(camPos.x, camPos.y, camPos.z);

  const camLook = config.camera?.lookAt ?? { x: 0, y: 0, z: 0 };
  camera.lookAt(camLook.x, camLook.y, camLook.z);

  controls = addControls(camera, renderer);
  controls.target.set(camLook.x, camLook.y, camLook.z);

  // Scene elements
  loadFrisbee(scene, config);
  addLights(scene, config);
  addCrossHatch(scene, config);
  addFieldLines(scene, config);
  addFieldGrid(scene, config);
  waitForFrisbeeThenAddMarkers();
  initHUDs(config);

  // Data source + loop
  addDataSourceToggle(startDataSource);
  // 🎮 Add play/pause button for CSV control
  addPlayPauseButton((isPlaying) => {
    if (config.dataSource.mode !== 'csv') return;

    if (isPlaying) {
      currentDataSource?.resume?.();
    } else {
      currentDataSource?.pause?.();
    }
  });
  startDataSource(config.dataSource.mode);

  // PiP
  initPipFlow(config);

  animate();
}

init();

// === Responsive resize ===
window.addEventListener('resize', () => {
  camera.aspect = window.innerWidth / window.innerHeight;
  camera.updateProjectionMatrix();
  renderer.setSize(window.innerWidth, window.innerHeight);
});
