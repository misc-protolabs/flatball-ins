import * as THREE from 'three';

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
