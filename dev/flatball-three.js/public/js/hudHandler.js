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

let config;
let lastStateHudUpdateMs = 0;

// Internal state for HUD updates
let t = 0;
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
	t = data.idx * 0.01;
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
}

export function updateHUDs() {
  // Debug HUD (every frame)
  if (config.hudDebug?.enabled) {
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

	dbg.textContent = `${line1}\n${line2}\n${line3}\n\n${line4}\n${line5}\n${line6}\n\n${line7}`;

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

      const x = 0, y = 0, z = 1;
      const u = 0, v = 0, w = 0;

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
