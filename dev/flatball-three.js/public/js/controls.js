import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';

function addControls(camera, renderer) {
  const controls = new OrbitControls(camera, renderer.domElement);
  controls.target.set(0, 0, 0);
  controls.enableDamping = true;
  controls.dampingFactor = 0.05;
  controls.update();
  return controls;
}

export { addControls };
