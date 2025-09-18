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

/**
 * Adds scene lighting based on config.
 * @param {THREE.Scene} scene - The scene to add lights to.
 * @param {Object} config - The loaded config object.
 */
function addLights(scene, config) {
  const skyColor = config.lights?.skyColor || 0xffffff;
  const groundColor = config.lights?.groundColor || 0x444444;

  const hemiLight = new THREE.HemisphereLight(skyColor, groundColor, 0.6);
  scene.add(hemiLight);

  const ambient = new THREE.AmbientLight(0xffffff, config.lights?.ambient?.intensity ?? 0.5);
  scene.add(ambient);

  const dirLight = new THREE.DirectionalLight(0xffffff, config.lights?.directional?.intensity ?? 0.8);
  dirLight.position.set(5, 10, 7.5);
  scene.add(dirLight);
}

export { addLights };
