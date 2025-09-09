import * as THREE from 'three';
import { scene, camera, renderer } from './sceneSetup.js';
import { initCSVLoader, csvData } from './csvLoader.js';
import { addLights } from './lights.js';
import { addGround } from './ground.js';
import { addControls } from './controls.js';
import { loadFrisbee, frisbee } from './frisbeeLoader.js';
import { addCrossHatch, addNSEWMarkers, addFieldLines, addFieldGrid } from './fieldHelpers.js';
import { initHUDs, updatePlaybackState, updateHUDs } from './hudHandler.js';

let config;
let controls;
let playStartTime = null;
let playing = false;

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
  console.groupEnd();
}

// === CSV load callback ===
function onCSVLoaded() {
  playStartTime = null;
  playing = config.playback.autoPlayOnLoad;
  console.info(`[CSV] Loaded ${csvData.length} rows — playback ${playing ? 'started' : 'paused'}`);
}

// === Animation loop ===
function animate(timestamp) {
  requestAnimationFrame(animate);

  if (playing && csvData.length > 0) {
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
      const rollDeg  = (row.roll  || 0) + (config.orientationOffsets.rollOffsetDeg  || 0);
      const pitchDeg = (row.pitch || 0) + (config.orientationOffsets.pitchOffsetDeg || 0);
      const yawDeg   = (row.yaw   || 0) + (config.orientationOffsets.yawOffsetDeg   || 0);

      frisbee.rotation.set(
        THREE.MathUtils.degToRad(pitchDeg),
        THREE.MathUtils.degToRad(yawDeg),
        THREE.MathUtils.degToRad(rollDeg),
        'ZYX'
      );

      updatePlaybackState(simTime, rollDeg, pitchDeg, yawDeg);
    }
  }

  updateHUDs();
  if (controls) controls.update();
  renderer.render(scene, camera);
}

// === Init ===
async function init() {
  await loadConfig();

  scene.up.set(0, 1, 0);
  camera.up.set(0, 1, 0);
  scene.background = new THREE.Color(config.scene.skyColor || '#87CEEB');

  addLights(scene, config);

  // Field surface
  addCrossHatch(scene, config);

  // Field lines
  addFieldLines(scene, config);

  // Grid overlay
  addFieldGrid(scene, config);

  // NESW markers
  addNSEWMarkers(scene, config);

  // Camera setup
  const camPos = config.camera?.startPosition || { x: 0, y: 10, z: -30 };
  camera.position.set(camPos.x, camPos.y, camPos.z);

  const camLook = config.camera?.lookAt || { x: 0, y: 0, z: 0 };
  camera.lookAt(camLook.x, camLook.y, camLook.z);
  controls = addControls(camera, renderer);
  controls.target.set(camLook.x, camLook.y, camLook.z);

  // Frisbee
  loadFrisbee(scene, config);

  // HUDs
  initHUDs(config);

  // CSV loader
  initCSVLoader(onCSVLoaded);

  animate();
}

init();

// === Resize handling ===
window.addEventListener('resize', () => {
  camera.aspect = window.innerWidth / window.innerHeight;
  camera.updateProjectionMatrix();
  renderer.setSize(window.innerWidth, window.innerHeight);
});
