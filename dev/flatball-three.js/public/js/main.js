import * as THREE from 'three';
import { scene, camera, renderer } from './sceneSetup.js';
import { initCSVLoader, csvData } from './csvLoader.js';
import { addLights } from './lights.js';
import { addGround } from './ground.js';
import { addControls } from './controls.js';
import { loadFrisbee, frisbee } from './frisbeeLoader.js';
import { addCrossHatch, addNSEWMarkers, addFieldLines, addFieldGrid } from './fieldHelpers.js';
import { initHUDs, updatePlaybackState, updateHUDs } from './hudHandler.js';
import { connectTelemetry } from './telemetryHelper.js';

let config;
let controls;
let playStartTime = null;
let playing = false;
let telemetryActive = false;

// === Load config.json ===
async function loadConfig() {
  const res = await fetch('./public/config.json');
  config = await res.json();
  console.group('[Startup] Config loaded');
  console.table(config.scene || {});
  console.table(config.processing?.units || {});
  console.table(config.playback || {});
  console.table(config.orientationOffsets || {});
  console.table(config.model || {});
  console.table(config.hudDebug || {});
  console.table(config.hudState || {});
  console.table(config.dataSource || {});
  console.groupEnd();
}

// === CSV load callback ===
function onCSVLoaded() {
  playStartTime = null;
  playing = config.playback.autoPlayOnLoad;
  console.info(`[CSV] Loaded ${csvData.length} rows — playback ${playing ? 'started' : 'paused'}`);
}

// === Telemetry handler ===
function handleTelemetry(data) {
  telemetryActive = true;

  const scale = config.dataSource.telemetry.scale || 1.0;
  const rollDeg  = (data.roll  || 0) + (config.orientationOffsets.rollOffsetDeg  || 0);
  const pitchDeg = (data.pitch || 0) + (config.orientationOffsets.pitchOffsetDeg || 0);
  const yawDeg   = (data.yaw   || 0) + (config.orientationOffsets.yawOffsetDeg   || 0);

  if (frisbee && data.x !== undefined && data.y !== undefined && data.z !== undefined) {
    frisbee.position.set(data.x * scale, data.y * scale, data.z * scale);
  }

  frisbee.rotation.set(
    THREE.MathUtils.degToRad(pitchDeg),
    THREE.MathUtils.degToRad(yawDeg),
    THREE.MathUtils.degToRad(rollDeg),
    'ZYX'
  );

  updatePlaybackState(data.timestamp ?? 0, rollDeg, pitchDeg, yawDeg, data.gyro_x, data.gyro_y, data.gyro_z);
}

// === Animation loop ===
function animate(timestamp) {
  requestAnimationFrame(animate);

  if (!telemetryActive && playing && csvData.length > 0) {
    if (!playStartTime) playStartTime = timestamp;

    let simTime = ((timestamp - playStartTime) / 1000) * config.playback.timeScale;
    const lastTime = (csvData.length - 1) * config.playback.frameInterval;
    if (simTime >= lastTime) {
      simTime = lastTime;
      playing = false;
      console.info('[Playback] Reached end of CSV');
    }

    const idx = Math.min(Math.floor(simTime / config.playback.frameInterval), csvData.length - 1);
    const row = csvData[idx];
    if (row) {
      handleTelemetry(row); // reuse telemetry handler for consistency
    }
  }

  updateHUDs();
  if (controls) controls.update();
  renderer.render(scene, camera);
}

// === Wait for frisbee before placing markers ===
function waitForFrisbeeThenAddMarkers(scene, config, maxTries = 50, interval = 100) {
  let tries = 0;
  const check = () => {
    if (frisbee && frisbee.position) {
      console.log('Frisbee ready, adding markers');
      addNSEWMarkers(scene, config);
    } else if (tries < maxTries) {
      tries++;
      setTimeout(check, interval);
    } else {
      console.warn('Frisbee not ready after polling — NSEW markers skipped');
    }
  };
  check();
}

// === Toggle button for switching data source ===
function addDataSourceToggle(config, onSwitch) {
  const btn = document.createElement('button');
  btn.textContent = `Switch to ${config.dataSource.mode === 'telemetry' ? 'CSV' : 'Telemetry'}`;
  btn.style.position = 'absolute';
  btn.style.top = '10px';
  btn.style.right = '10px';
  btn.style.zIndex = '1000';
  btn.style.padding = '6px 12px';
  btn.style.fontSize = '14px';

  btn.onclick = () => {
    config.dataSource.mode = config.dataSource.mode === 'telemetry' ? 'csv' : 'telemetry';
    btn.textContent = `Switch to ${config.dataSource.mode === 'telemetry' ? 'CSV' : 'Telemetry'}`;
    telemetryActive = false;
    playStartTime = null;
    playing = config.playback.autoPlayOnLoad;
    onSwitch(config.dataSource.mode);
  };

  document.body.appendChild(btn);
}

// === Start data source based on config ===
function startDataSource(mode) {
  if (mode === 'telemetry') {
    connectTelemetry(config.dataSource.telemetry, handleTelemetry);
  } else if (mode === 'csv') {
    initCSVLoader(onCSVLoaded);
  }
}

// === Init ===
async function init() {
  await loadConfig();

  scene.up.set(0, 1, 0);
  camera.up.set(0, 1, 0);
  scene.background = new THREE.Color(config.scene.skyColor || '#87CEEB');

  const camPos = config.camera?.startPosition || { x: 0, y: 10, z: -30 };
  camera.position.set(camPos.x, camPos.y, camPos.z);

  const camLook = config.camera?.lookAt || { x: 0, y: 0, z: 0 };
  camera.lookAt(camLook.x, camLook.y, camLook.z);
  controls = addControls(camera, renderer);
  controls.target.set(camLook.x, camLook.y, camLook.z);

  loadFrisbee(scene, config);
  addLights(scene, config);
  addCrossHatch(scene, config);
  addFieldLines(scene, config);
  addFieldGrid(scene, config);
  waitForFrisbeeThenAddMarkers(scene, config);
  initHUDs(config);

  addDataSourceToggle(config, startDataSource);
  startDataSource(config.dataSource.mode);

  animate();
}

init();

// === Resize handling ===
window.addEventListener('resize', () => {
  camera.aspect = window.innerWidth / window.innerHeight;
  camera.updateProjectionMatrix();
  renderer.setSize(window.innerWidth, window.innerHeight);
});
