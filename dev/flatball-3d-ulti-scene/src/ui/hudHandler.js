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

// hudHandler.js
// Handles creation, styling, and updates for Debug HUD and State Vector HUD

//import config from './config.json';

let config;
let lastStateHudUpdateMs = 0;

// Internal state for HUD updates
let t = 0;
let idx = 0;
let ax_raw = 0;
let ay_raw = 0;
let az_raw = 0;
let gx_raw = 0;
let gy_raw = 0;
let gz_raw = 0;
let mx_raw = 0;
let my_raw = 0;
let mz_raw = 0;
let ax = 0;
let ay = 0;
let az = 0;
let gx = 0;
let gy = 0;
let gz = 0;
let mx = 0;
let my = 0;
let mz = 0;
let qx = 0;
let qy = 0;
let qz = 0;
let roll = 0;
let pitch = 0;
let yaw = 0;
let x = 0;
let y = 0;
let z = 0;
let u = 0;
let v = 0;
let w = 0;
let phi = 0;
let theta = 0;
let psi = 0;
let phidt = 0;
let thetadt = 0;
let psidt = 0;
let v_batt = 0;
let p_atm = 0;
let t_atm = 0;
let stat = 0;

// === Utility functions ===
const degToRad = (deg) => deg * Math.PI / 180;

function formatNumber(val, fmt) {
  const match = /^%(\d+)?\.(\d+)f$/.exec(fmt);
  const width = match ? parseInt(match[1] || "0", 10) : 0;
  const precision = match ? parseInt(match[2], 10) : 4;
  const fixed = Number(val).toFixed(precision);
  return width > 0 ? fixed.padStart(width, ' ') : fixed;
}

function setupHudElement(el, settings) {
  el.style.position = 'absolute';
  el.style.fontFamily = 'monospace';
  el.style.fontSize = `${settings.fontSize}px`;
  el.style.color = settings.color;
  el.style.background = settings.background;
  el.style.padding = '6px 10px';
  el.style.lineHeight = '1.35';
  el.style.whiteSpace = 'pre';
  el.style.pointerEvents = 'none';

  el.style.top = ''; el.style.bottom = ''; el.style.left = ''; el.style.right = '';
  const pos = (settings.position || '').toLowerCase();
  switch (pos) {
    case 'top-left':
    case 'upperl': el.style.top = '10px'; el.style.left = '10px'; break;
    case 'top-right':
    case 'upperr': el.style.top = '10px'; el.style.right = '10px'; break;
    case 'bottom-left':
    case 'lowerl': el.style.bottom = '10px'; el.style.left = '10px'; break;
    case 'bottom-right':
    case 'lowerr': el.style.bottom = '10px'; el.style.right = '10px'; break;
    default: el.style.top = '10px'; el.style.left = '10px'; break;
  }
}

// === Public API ===
export function initHUDs(cfg) {
  config = cfg;
  const dbg = document.getElementById('hudDebug');
  const st = document.getElementById('hudState');

  if (config.hudDebug?.enabled) {
    setupHudElement(dbg, config.hudDebug);
    dbg.style.display = 'block';
  } else {
    dbg.style.display = 'none';
  }

  if (config.hudState?.enabled) {
    setupHudElement(st, config.hudState);
    st.style.display = 'block';
  } else {
    st.style.display = 'none';
  }
}

export function updatePlaybackState( data) { //simTime, rollDeg, pitchDeg, yawDeg, rollDegPerSec, pitchDegPerSec, yawDegPerSec) {
	idx = data.idx;
	ax_raw = data.ax_raw;
	ay_raw = data.ay_raw;
	az_raw = data.az_raw;
	gx_raw = data.gx_raw;
	gy_raw = data.gy_raw;
	gz_raw = data.gz_raw;
	mx_raw = data.mx_raw;
	my_raw = data.my_raw;
	mz_raw = data.mz_raw;
	ax = data.ax;
	ay = data.ay;
	az = data.az;
	gx = data.gx;
	gy = data.gy;
	gz = data.gz;
	mx = data.mx;
	my = data.my
	mz = data.mz;
	roll= data.roll;
	pitch = data.pitch
	yaw = data.yaw;
	x = data.x;
	y = data.y;
	z = data.z;
	u = data.u;
	v = data.v;
	w = data.w;
	phi = data.phi;
	theta = data.theta;
	psi = data.psi;
	phidt = data.phidt;
	thetadt = data.thetadt;
	psidt = data.psidt;
}

export function updateHUDs() {
  // Debug HUD (every frame)
  if (config.hudDebug?.enabled) {
	const dt = 0.010;
    const dbg = document.getElementById('hudDebug');
	const fmt = config.hudState.numericFormat || '%10.4f';
    const anglesUnit = (config.processing?.units?.angles || 'rad').toLowerCase();
    const unitLabel = anglesUnit === 'rad' ? 'rad' : 'deg';

	const line1 = `ax_raw: ${formatNumber(ax_raw, fmt)} ay_raw: ${formatNumber(ay_raw, fmt)} az_raw: ${formatNumber(az_raw, fmt)}`;
	const line2 = `gx_raw: ${formatNumber(gx_raw, fmt)} gy_raw: ${formatNumber(gy_raw, fmt)} gz_raw: ${formatNumber(gz_raw, fmt)}`;
	const line3 = `mx_raw: ${formatNumber(mx_raw, fmt)} my_raw: ${formatNumber(my_raw, fmt)} mz_raw: ${formatNumber(mz_raw, fmt)}`;

	const line4 = `    ax: ${formatNumber(    ax, fmt)}     ay: ${formatNumber(    ay, fmt)}     az: ${formatNumber(    az, fmt)}`;
	const line5 = `    gx: ${formatNumber(    gx, fmt)}     gy: ${formatNumber(    gy, fmt)}     gz: ${formatNumber(    gz, fmt)}`;
	const line6 = `    mx: ${formatNumber(    mx, fmt)}     my: ${formatNumber(    my, fmt)}     mz: ${formatNumber(    mz, fmt)}`;

	const line7 = `  roll: ${formatNumber(  roll, fmt)}  pitch: ${formatNumber( pitch, fmt)}    yaw: ${formatNumber(   yaw, fmt)}`;

	const line8 = `  time: ${formatNumber(  idx * dt, fmt)}`;

	dbg.textContent = `${line1}\n${line2}\n${line3}\n\n${line4}\n${line5}\n${line6}\n\n${line7}\n${line8}`;

	//const unitsFooter = `[units] pos: ${config.processing.units.length}, vel: ${config.processing.units.velocity}, angles: ${config.processing.units.angles}, rates: ${config.processing.units.angleRates}`;
  }

  // State HUD (throttled)
  if (config.hudState?.enabled) {
    const nowMs = performance.now();
    const hz = config.hudState.updateRateHz || 20;
    const periodMs = 1000 / hz;

    if (nowMs - lastStateHudUpdateMs >= periodMs) {
      lastStateHudUpdateMs = nowMs;
      const st = document.getElementById('hudState');
      const fmt = config.hudState.numericFormat || '%10.4f';
      const anglesUnit = (config.processing?.units?.angles || 'rad').toLowerCase();

      const phi   = anglesUnit === 'rad' ? degToRad(roll)  : roll;
      const theta = anglesUnit === 'rad' ? degToRad(pitch) : pitch;
      const psi   = anglesUnit === 'rad' ? degToRad(yaw)   : yaw;

      const phidt   = anglesUnit === 'rad' ? degToRad(gx) : gx;
      const thetadt = anglesUnit === 'rad' ? degToRad(gy) : gy;
      const psidt   = anglesUnit === 'rad' ? degToRad(gz) : gz;

      const line1 = `    x: ${formatNumber(x, fmt)}        y: ${formatNumber(y, fmt)}      z: ${formatNumber(z, fmt)}`;
      const line2 = `    u: ${formatNumber(u, fmt)}        v: ${formatNumber(v, fmt)}      w: ${formatNumber(w, fmt)}`;
      const line3 = `  phi: ${formatNumber(phi, fmt)}    theta: ${formatNumber(theta, fmt)}    psi: ${formatNumber(psi, fmt)}`;
      const line4 = `phidt: ${formatNumber(phidt, fmt)}  thetadt: ${formatNumber(thetadt, fmt)}  psidt: ${formatNumber(psidt, fmt)}`;

      const unitsFooter = `[units] pos: ${config.processing.units.length}, vel: ${config.processing.units.velocity}, angles: ${config.processing.units.angles}, rates: ${config.processing.units.angleRates}`;

      st.textContent = `${line1}\n${line2}\n${line3}\n${line4}\n${unitsFooter}`;
    }
  }
}

export function createCompositeStream(videoEl, options = {}) {
  const {
    canvasSelector = 'canvas',
    pipSize = { width: 160, height: 90 },
    pipLocation = 'lowerR',
    frameRate = 30,
    resolution = { width: 1280, height: 720 }
  } = options;

  const sceneCanvas = document.querySelector(canvasSelector);
  if (!sceneCanvas) {
    console.warn('[CompositeStream] Scene canvas not found.');
    return null;
  }

  const compositeCanvas = document.createElement('canvas');
  compositeCanvas.width = resolution.width;
  compositeCanvas.height = resolution.height;
  const ctx = compositeCanvas.getContext('2d');

  const getPipPosition = () => {
    const { width, height } = pipSize;
    switch (pipLocation) {
      case 'upperL': return { x: 10, y: 10 };
      case 'upperR': return { x: compositeCanvas.width - width - 10, y: 10 };
      case 'lowerL': return { x: 10, y: compositeCanvas.height - height - 10 };
      case 'lowerR': return { x: compositeCanvas.width - width - 10, y: compositeCanvas.height - height - 10 };
      case 'center': return {
        x: (compositeCanvas.width - width) / 2,
        y: (compositeCanvas.height - height) / 2
      };
      default: return { x: 10, y: 10 };
    }
  };

  const drawFrame = () => {
    ctx.clearRect(0, 0, compositeCanvas.width, compositeCanvas.height);
    ctx.drawImage(sceneCanvas, 0, 0, compositeCanvas.width, compositeCanvas.height);

    if (videoEl?.readyState >= 2) {
      const { width, height } = pipSize;
      const { x, y } = getPipPosition();
      ctx.drawImage(videoEl, x, y, width, height);
    }

    requestAnimationFrame(drawFrame);
  };

  drawFrame();
  console.log('[CompositeStream] Composite canvas stream initialized.');
  return compositeCanvas.captureStream(frameRate);
}

function shouldShowPipHUD() {
  return config.hudPiP?.enabled && config.hudPiP?.show;
}

export function renderHUD(state, payload) {
  const hudElement = document.getElementById('hudPip');
  if (!hudElement) {
    console.warn('[HUD] Element not found.');
    return;
  }

  hudElement.innerHTML = '';

  const {
    location = 'upperL',
    offset = { x: 10, y: 10 },
    units = 'px',
    size = { width: '300px', height: '200px' },
    style = 'minimal',
    startCamera = false,
    recordingEnabled = true,
    resolution = { width: 1280, height: 720 },
    frameRate = 30,
    mimeType = 'video/webm',
    recordSource = 'camera',
    composite = {
      pipSize: { width: 160, height: 90 },
      pipLocation: 'lowerR'
    }
  } = payload;

  console.log('[HUD] Config:', {
    location, startCamera, recordingEnabled, resolution, frameRate, mimeType, recordSource
  });

  // Positioning
  hudElement.style.display = 'block';
  hudElement.style.position = 'absolute';
  hudElement.style.width = size.width;
  hudElement.style.height = size.height;
  hudElement.style.overflow = 'hidden';
  hudElement.style.top = '';
  hudElement.style.bottom = '';
  hudElement.style.left = '';
  hudElement.style.right = '';
  hudElement.style.transform = '';

  switch (location) {
    case 'upperL': hudElement.style.top = `${offset.y}${units}`; hudElement.style.left = `${offset.x}${units}`; break;
    case 'upperR': hudElement.style.top = `${offset.y}${units}`; hudElement.style.right = `${offset.x}${units}`; break;
    case 'lowerL': hudElement.style.bottom = `${offset.y}${units}`; hudElement.style.left = `${offset.x}${units}`; break;
    case 'lowerR': hudElement.style.bottom = `${offset.y}${units}`; hudElement.style.right = `${offset.x}${units}`; break;
    case 'center': hudElement.style.top = '50%'; hudElement.style.left = '50%'; hudElement.style.transform = 'translate(-50%, -50%)'; break;
    default: hudElement.style.top = `${offset.y}${units}`; hudElement.style.left = `${offset.x}${units}`; break;
  }

  if (style === 'minimal') {
    hudElement.style.backgroundColor = '#000';
    hudElement.style.border = '2px solid #444';
    hudElement.style.borderRadius = '8px';
  }

  // Video preview
  const video = document.createElement('video');
  video.setAttribute('autoplay', true);
  video.setAttribute('playsinline', true);
  video.style.width = '100%';
  video.style.height = '100%';
  video.style.objectFit = 'cover';
  hudElement.appendChild(video);

  // Toggle button
  const toggleBtn = document.createElement('button');
  toggleBtn.innerText = '▶️';
  toggleBtn.title = 'Toggle Camera';
  Object.assign(toggleBtn.style, {
    position: 'absolute', top: '5px', left: '5px', zIndex: '10',
    padding: '4px 8px', fontSize: '14px', border: 'none',
    borderRadius: '4px', backgroundColor: '#444', color: '#fff', cursor: 'pointer'
  });
  hudElement.appendChild(toggleBtn);

  // Record button
  let recordBtn = null;
  if (recordingEnabled) {
    recordBtn = document.createElement('button');
    recordBtn.innerText = '📹';
    recordBtn.title = 'Start Recording';
    Object.assign(recordBtn.style, {
      position: 'absolute', top: '5px', right: '5px', zIndex: '10',
      padding: '4px 8px', fontSize: '14px', border: 'none',
      borderRadius: '4px', backgroundColor: '#444', color: '#fff', cursor: 'pointer'
    });
    hudElement.appendChild(recordBtn);
  }

  let cameraStream = null;
  let mediaRecorder = null;
  let recordedChunks = [];

  function activateCameraStream() {
    const constraints = {
      video: {
        width: resolution.width,
        height: resolution.height,
        frameRate: frameRate
      }
    };
    console.log('[HUD] Requesting camera stream:', constraints);

    navigator.mediaDevices.getUserMedia(constraints)
      .then(s => {
        cameraStream = s;
        video.srcObject = s;
        toggleBtn.innerText = '⏹️';
        console.log('[HUD] Camera stream activated.');
      })
      .catch(err => {
        console.error('[HUD] Camera access failed:', err);
        hudElement.innerHTML = `<div style="color: red; padding: 10px;">Camera access denied</div>`;
      });
  }

  function deactivateCameraStream() {
    if (cameraStream) {
      cameraStream.getTracks().forEach(track => track.stop());
      cameraStream = null;
      video.srcObject = null;
      toggleBtn.innerText = '▶️';
      console.log('[HUD] Camera stream deactivated.');
    }
  }

  function getRecordingStream() {
    if (recordSource === 'scene') {
      const canvas = document.querySelector('canvas');
      if (!canvas) {
        console.warn('[HUD] Canvas not found.');
        return null;
      }
      console.log('[HUD] Capturing canvas stream.');
      return canvas.captureStream(frameRate);
    }

    if (recordSource === 'composite') {
      const compositeStream = createCompositeStream(video, {
        pipSize: composite.pipSize,
        pipLocation: composite.pipLocation,
        frameRate,
        resolution
      });
      console.log('[HUD] Capturing composite stream.');
      return compositeStream;
    }

    if (recordSource === 'camera') {
      if (!cameraStream) {
        console.warn('[HUD] Camera stream not active.');
        return null;
      }
      console.log('[HUD] Using camera stream.');
      return cameraStream;
    }

    console.warn(`[HUD] Unknown recordSource: ${recordSource}`);
    return null;
  }

  toggleBtn.addEventListener('click', () => {
    if (cameraStream) {
      deactivateCameraStream();
    } else {
      activateCameraStream();
    }
  });

  if (recordBtn) {
    recordBtn.addEventListener('click', () => {
      const sourceStream = getRecordingStream();
      if (!sourceStream) {
        console.warn('[HUD] No stream available for recording.');
        return;
      }

      if (!mediaRecorder || mediaRecorder.state === 'inactive') {
        recordedChunks = [];
        try {
          mediaRecorder = new MediaRecorder(sourceStream, { mimeType });
          console.log('[HUD] MediaRecorder initialized.');
        } catch (err) {
          console.error('[HUD] MediaRecorder failed:', err);
          return;
        }

        mediaRecorder.ondataavailable = e => {
          if (e.data.size > 0) recordedChunks.push(e.data);
        };

        mediaRecorder.onstop = () => {
          const blob = new Blob(recordedChunks, { type: mimeType });
          const url = URL.createObjectURL(blob);
          const a = document.createElement('a');
          a.href = url;
          a.download = `demo-recording-${Date.now()}.webm`;
          a.click();
          console.log('[HUD] Recording saved.');
        };

        mediaRecorder.start();
        recordBtn.innerText = '⏹️';
        recordBtn.title = 'Stop Recording';
        console.log('[HUD] Recording started.');
      } else {
        mediaRecorder.stop();
        recordBtn.innerText = '📹';
        recordBtn.title = 'Start Recording';
        console.log('[HUD] Recording stopped.');
      }
    });
  }

  // Initial stream setup
  if (startCamera) {
    activateCameraStream();
    toggleBtn.style.display = 'block';
  } else {
    toggleBtn.innerText = '▶️';
    toggleBtn.style.display = 'block';
  }

  if (recordSource === 'scene' || recordSource === 'composite') {
    const previewStream = getRecordingStream();
    if (previewStream) {
      video.srcObject = previewStream;
      console.log('[HUD] Previewing stream for recordSource:', recordSource);
    } else {
      video.style.display = 'none';
      console.warn('[HUD] Preview stream not available.');
    }
  }
}
