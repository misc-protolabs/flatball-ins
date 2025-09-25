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

class ChaseCamera {
  constructor(camera, target) {
    this.camera = camera;
    this.target = target;
    this.offset = new THREE.Vector3(3.0, 1.0, 3.0);
  }

  update() {
    if (!this.target) return;

    const targetPosition = this.target.position.clone();
    const cameraPosition = targetPosition.add(this.offset);

    this.camera.position.copy(cameraPosition);
    this.camera.lookAt(this.target.position);
  }
}

export { ChaseCamera };
