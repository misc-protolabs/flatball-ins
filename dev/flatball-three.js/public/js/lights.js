import * as THREE from 'three';

/**
 * Adds scene lighting based on config.
 * @param {THREE.Scene} scene - The scene to add lights to.
 * @param {Object} config - The loaded config object.
 */
function addLights(scene, config) {
  // Hemisphere light for ambient fill
  const hemiLight = new THREE.HemisphereLight(
    config.lights.hemisphere.skyColor,
    config.lights.hemisphere.groundColor,
    config.lights.hemisphere.intensity
  );
  scene.add(hemiLight);

  // Directional light for shadows and highlights
  const dirLight = new THREE.DirectionalLight(
    config.lights.directional.color,
    config.lights.directional.intensity
  );
  dirLight.position.set(
    config.lights.directional.position[0],
    config.lights.directional.position[1],
    config.lights.directional.position[2]
  );
  scene.add(dirLight);
}

export { addLights };
