import * as THREE from 'three';
import { scene, camera, renderer } from './sceneSetup.js';
import { initCSVLoader, csvData } from './csvLoader.js';
import { addLights } from './lights.js';
import { addGround } from './ground.js';
import { addControls } from './controls.js';
import { loadFrisbee, frisbee } from './frisbeeLoader.js';
import { addCrossHatch, addNSEWMarkers } from './fieldHelpers.js';

let config;
let controls;

// Playback state
let playStartTime = null;
let playing = false;

// HUD state
let lastSimTime = 0;
let lastRoll = 0;
let lastPitch = 0;
let lastYaw = 0;

// === Load config.json ===
async function loadConfig() {
  const res = await fetch('./public/config.json');
  config = await res.json();
  console.group('[Startup] Config loaded');
  console.table(config.playback);
  console.table(config.orientationOffsets);
  console.table(config.scene);
  console.table(config.model);
  console.table(config.lights);
  console.table(config.ground);
  console.groupEnd();
}

// === CSV load callback ===
function onCSVLoaded() {
  playStartTime = null;
  playing = config.playback.autoPlayOnLoad;
  lastSimTime = 0;
  lastRoll = 0;
  lastPitch = 0;
  lastYaw = 0;
  console.info(`[CSV] Loaded ${csvData.length} rows — playback ${playing ? 'started' : 'paused'}`);
}

// === HUD setup ===
function setupHUD() {
  const hudEl = document.getElementById('hud');
  hudEl.style.position = 'absolute';
  hudEl.style.fontFamily = 'monospace';
  hudEl.style.fontSize = `${config.hud.fontSize}px`;
  hudEl.style.color = config.hud.color;
  hudEl.style.background = config.hud.background;
  hudEl.style.padding = '6px 10px';
  hudEl.style.lineHeight = '1.4';
  hudEl.style.whiteSpace = 'pre';
  hudEl.style.pointerEvents = 'none';

  hudEl.style.top = '';
  hudEl.style.bottom = '';
  hudEl.style.left = '';
  hudEl.style.right = '';
  switch (config.hud.position) {
    case 'top-left': hudEl.style.top = '10px'; hudEl.style.left = '10px'; break;
    case 'top-right': hudEl.style.top = '10px'; hudEl.style.right = '10px'; break;
    case 'bottom-left': hudEl.style.bottom = '10px'; hudEl.style.left = '10px'; break;
    case 'bottom-right': hudEl.style.bottom = '10px'; hudEl.style.right = '10px'; break;
  }
}

// === Animation loop ===
function animate(timestamp) {
  requestAnimationFrame(animate);

  const hudEl = document.getElementById('hud');

  if (playing && csvData.length > 0) {
    if (!playStartTime) {
      playStartTime = timestamp;
      console.debug('[Playback] Started at t=0');
    }
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
      const roll  = (row.roll  || 0) + config.orientationOffsets.rollOffsetDeg;
      const pitch = (row.pitch || 0) + config.orientationOffsets.pitchOffsetDeg;
      const yaw   = (row.yaw   || 0) + config.orientationOffsets.yawOffsetDeg;

      frisbee.rotation.set(
        THREE.MathUtils.degToRad(pitch),
        THREE.MathUtils.degToRad(yaw),
        THREE.MathUtils.degToRad(roll),
        'ZYX'
      );

      lastSimTime = simTime;
      lastRoll = roll;
      lastPitch = pitch;
      lastYaw = yaw;
    }
  }

  hudEl.textContent =
    `Time:  ${lastSimTime.toFixed(2)} s\n` +
    `Roll:  ${lastRoll.toFixed(1)}°\n` +
    `Pitch: ${lastPitch.toFixed(1)}°\n` +
    `Yaw:   ${lastYaw.toFixed(1)}°`;

  if (controls) controls.update();
  renderer.render(scene, camera);
}

// === Init ===
async function init() {
  await loadConfig();

  // Lock camera to ENU top-down
  camera.position.set(0, 50, 0);   // directly above origin
  camera.up.set(0, 0, -1);         // +Z (North) is top of view
  camera.lookAt(0, 0, 0);

  // Scene setup
  addLights(scene, config);
  addGround(scene, config);
  addCrossHatch(scene, config);
  addNSEWMarkers(scene, config);

  // World frame helper at origin
  scene.add(new THREE.AxesHelper(1.0));

  // Controls
  controls = addControls(camera, renderer);

// === Camera: start above field, ENU-aligned ===
camera.position.set(-5, 5, 5);   // elevated and offset so you see depth
camera.up.set(0, 1, 0);            // standard Three.js up (Y-up)
camera.lookAt(0, 0, 0);            // look toward origin

// === Controls: full freedom ===
controls = addControls(camera, renderer);
controls.target.set(0, 0, 0);      // orbit around field center
controls.enableRotate = true;      // allow rotation
controls.enableZoom = true;        // allow zoom
controls.enablePan = true;         // allow panning
controls.minDistance = 5;          // optional: prevent zooming inside model
controls.maxDistance = 200;        // optional: prevent zooming too far
controls.minPolarAngle = 0;        // allow tilt from horizon
controls.maxPolarAngle = Math.PI;  // to straight down

  // Frisbee
  loadFrisbee(scene, config);

  // HUD
  setupHUD();

  // CSV loader
  initCSVLoader(onCSVLoaded);

  // Start loop
  animate();
}

init();

// Resize handling
window.addEventListener('resize', () => {
  camera.aspect = window.innerWidth / window.innerHeight;
  camera.updateProjectionMatrix();
  renderer.setSize(window.innerWidth, window.innerHeight);
});
