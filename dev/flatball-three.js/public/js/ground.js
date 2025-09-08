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
