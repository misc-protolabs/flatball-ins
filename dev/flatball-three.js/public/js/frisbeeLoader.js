import * as THREE from 'three';
import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader.js';

let frisbee = null;

function loadFrisbee(scene, config) {
  const loader = new GLTFLoader().setPath('./public/models/');
  loader.load(
    'flatball-usau-frisbee.gltf',
    (gltf) => {
      frisbee = gltf.scene;

      // Rotate to match Y-up
      frisbee.rotation.x = Math.PI / 2;

      // Scale to target diameter from config
      const box = new THREE.Box3().setFromObject(frisbee);
      const size = new THREE.Vector3();
      box.getSize(size);
      const maxDim = Math.max(size.x, size.z) || 1;
      const scale = config.model.frisbeeDiameter / maxDim;
      frisbee.scale.setScalar(scale);

      // Position above ground from config
      frisbee.position.set(0, config.model.frisbeeStartHeight, 0);

// Apply frame correction from config
const [pitchDeg, yawDeg, rollDeg] = config.model.frameCorrectionDeg;
frisbee.rotation.x += THREE.MathUtils.degToRad(pitchDeg);
frisbee.rotation.y += THREE.MathUtils.degToRad(yawDeg);
frisbee.rotation.z += THREE.MathUtils.degToRad(rollDeg);

      // Optional AxesHelper
      if (config.scene.showAxesHelper) {
        const axesHelper = new THREE.AxesHelper(config.scene.axesHelperLength);
        frisbee.add(axesHelper);
      }

      scene.add(frisbee);
      console.log(
        `[GLTF] Frisbee loaded, scale factor: ${scale.toFixed(3)}, diameter: ${config.model.frisbeeDiameter}m`
      );
    },
    (xhr) => {
      if (xhr.total) {
        console.log(`[GLTF] Loading ${(xhr.loaded / xhr.total) * 100}%`);
      }
    },
    (err) => console.error('[GLTF] Load error:', err)
  );
}

export { loadFrisbee, frisbee };
