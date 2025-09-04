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

import * as THREE from 'https://cdn.jsdelivr.net/npm/three@0.160.0/build/three.module.js';
import Papa from 'https://cdn.jsdelivr.net/npm/papaparse@5.4.1/+esm';

// --- Scene ---
const scene = new THREE.Scene();
scene.background = new THREE.Color(0x87ceeb);

const camera = new THREE.PerspectiveCamera(60, window.innerWidth / window.innerHeight, 0.1, 1000);
camera.position.set(0, 60, 100);
camera.lookAt(0, 0, 0);

const renderer = new THREE.WebGLRenderer({ antialias: true });
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

// --- Field ---
const fieldLength = 105;
const fieldWidth = 68;
const fieldGeometry = new THREE.PlaneGeometry(fieldWidth, fieldLength);
const fieldMaterial = new THREE.MeshBasicMaterial({ color: 0x228B22, side: THREE.DoubleSide });
const field = new THREE.Mesh(fieldGeometry, fieldMaterial);
field.rotation.x = -Math.PI / 2;
scene.add(field);

// --- Grid ---
const gridHelper = new THREE.GridHelper(
  Math.max(fieldLength, fieldWidth),
  Math.max(fieldLength, fieldWidth) / 5,
  0xcccccc,
  0xcccccc
);
gridHelper.position.y = 0.01;
scene.add(gridHelper);

// --- Animation loop (rotating camera) ---
let angle = 0;
function animate() {
  requestAnimationFrame(animate);
  angle += 0.002;
  camera.position.x = 120 * Math.sin(angle);
  camera.position.z = 120 * Math.cos(angle);
  camera.lookAt(0, 0, 0);
  renderer.render(scene, camera);
}
animate();

// --- CSV Loader ---
const uploader = document.createElement('input');
uploader.type = 'file';
uploader.accept = '.csv';
uploader.style.position = 'absolute';
uploader.style.top = '10px';
uploader.style.left = '10px';
uploader.style.zIndex = '10';
document.body.appendChild(uploader);

uploader.addEventListener('change', event => {
  const file = event.target.files[0];
  if (!file) return;

  Papa.parse(file, {
    header: true,
    dynamicTyping: true,
    skipEmptyLines: true,
    complete: results => {
      console.log(`Loaded ${results.data.length} rows from CSV`);
      // Show first 5 rows with computed time
      results.data.slice(0, 5).forEach(row => {
        const time = row.idx * 0.01;
        console.log(`t=${time.toFixed(2)}s | qw=${row.qw}, qx=${row.qx}, qy=${row.qy}, qz=${row.qz}`);
      });
    },
    error: err => {
      console.error("CSV parse error:", err);
    }
  });
});
