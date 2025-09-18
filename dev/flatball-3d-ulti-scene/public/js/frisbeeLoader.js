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
import { GLTFLoader } from './libs/GLTFLoader.js';

let frisbee = null;

function loadFrisbee(scene, config) {
  const loader = new GLTFLoader();
  loader.load(
	'./public/models/flatball-usau-frisbee.gltf', (gltf) => {
    frisbee = gltf.scene;

    // Start position
    if (config.model?.startPosition) {
      frisbee.position.set(
        config.model.startPosition.x,
        config.model.startPosition.y,
        config.model.startPosition.z
      );
    }

    // Frame correction
    if (config.model?.frameCorrectionDeg) {
      frisbee.rotation.set(
        THREE.MathUtils.degToRad(config.model.frameCorrectionDeg[0] || 0),
        THREE.MathUtils.degToRad(config.model.frameCorrectionDeg[1] || 0),
        THREE.MathUtils.degToRad(config.model.frameCorrectionDeg[2] || 0)
      );
    }

	  // Optional AxesHelper
	  if (config.scene.showAxesHelper) {
		const axesHelper = new THREE.AxesHelper(config.scene.axesHelperLength);
		frisbee.add(axesHelper);
	  }
	  scene.add(frisbee);
  });
}

export { loadFrisbee, frisbee };
