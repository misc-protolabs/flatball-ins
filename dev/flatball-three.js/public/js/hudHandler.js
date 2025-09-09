// hudHandler.js
// Handles creation, styling, and updates for Debug HUD and State Vector HUD

let config;
let lastStateHudUpdateMs = 0;

// Internal state for HUD updates
let lastSimTime = 0;
let lastRollDeg = 0;
let lastPitchDeg = 0;
let lastYawDeg = 0;

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

export function updatePlaybackState(simTime, rollDeg, pitchDeg, yawDeg) {
  lastSimTime = simTime;
  lastRollDeg = rollDeg;
  lastPitchDeg = pitchDeg;
  lastYawDeg = yawDeg;
}

export function updateHUDs() {
  // Debug HUD (every frame)
  if (config.hudDebug?.enabled) {
    const dbg = document.getElementById('hudDebug');
    const anglesUnit = (config.processing?.units?.angles || 'rad').toLowerCase();
    const rollOut  = anglesUnit === 'rad' ? degToRad(lastRollDeg)  : lastRollDeg;
    const pitchOut = anglesUnit === 'rad' ? degToRad(lastPitchDeg) : lastPitchDeg;
    const yawOut   = anglesUnit === 'rad' ? degToRad(lastYawDeg)   : lastYawDeg;
    const unitLabel = anglesUnit === 'rad' ? 'rad' : 'deg';

    dbg.textContent =
      `    t: ${lastSimTime.toFixed(2)}  s\n` +
      ` roll: ${rollOut.toFixed(3)} ${unitLabel}\n` +
      `pitch: ${pitchOut.toFixed(3)} ${unitLabel}\n` +
      `  yaw: ${yawOut.toFixed(3)} ${unitLabel}`;
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

      const phi   = anglesUnit === 'rad' ? degToRad(lastRollDeg)  : lastRollDeg;
      const theta = anglesUnit === 'rad' ? degToRad(lastPitchDeg) : lastPitchDeg;
      const psi   = anglesUnit === 'rad' ? degToRad(lastYawDeg)   : lastYawDeg;

      const x = 0, y = 0, z = 1;
      const u = 0, v = 0, w = 0;
      const phidt = 0, thetadt = 0, psidt = 0;

      const line1 = `    x: ${formatNumber(x, fmt)}        y: ${formatNumber(y, fmt)}      z: ${formatNumber(z, fmt)}`;
      const line2 = `    u: ${formatNumber(u, fmt)}        v: ${formatNumber(v, fmt)}      w: ${formatNumber(w, fmt)}`;
      const line3 = `  phi: ${formatNumber(phi, fmt)}    theta: ${formatNumber(theta, fmt)}    psi: ${formatNumber(psi, fmt)}`;
      const line4 = `phidt: ${formatNumber(phidt, fmt)}  thetadt: ${formatNumber(thetadt, fmt)}  psidt: ${formatNumber(psidt, fmt)}`;

      const unitsFooter = `[units] pos: ${config.processing.units.length}, vel: ${config.processing.units.velocity}, angles: ${config.processing.units.angles}, rates: ${config.processing.units.angleRates}`;

      st.textContent = `${line1}\n${line2}\n${line3}\n${line4}\n${unitsFooter}`;
    }
  }
}
