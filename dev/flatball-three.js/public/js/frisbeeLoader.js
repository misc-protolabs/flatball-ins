import * as THREE from 'three';
import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader.js';

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

    scene.add(frisbee);
  });
}

export { loadFrisbee, frisbee };
