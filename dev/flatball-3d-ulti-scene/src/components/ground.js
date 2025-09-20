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

import * as THREE from 'three';

/**
 * Adds a ground plane based on config.
 * @param {THREE.Scene} scene - The scene to add the ground to.
 * @param {Object} config - The loaded config object.
 */
function addGround(scene, config) {
  const groundGeo = new THREE.PlaneGeometry(
    config.scene.gridSize,
    config.scene.gridSize
  );
  const groundMat = new THREE.MeshPhongMaterial({
    color: config.ground.color,
    depthWrite: false
  });
  const ground = new THREE.Mesh(groundGeo, groundMat);
  ground.rotation.x = -Math.PI / 2;
  ground.position.y = 0;
  scene.add(ground);
}

export { addGround };
