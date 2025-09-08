import * as THREE from 'three';

function addCrossHatch(scene, config) {
  const grid = new THREE.GridHelper(
    config.scene.gridSize,
    config.scene.gridSize / config.scene.gridStep,
    0xcccccc,
    0xcccccc
  );
  grid.position.y = 0.01; // avoid z-fighting
  scene.add(grid);
}

function createLetterSprite(letter, color = '#ffaa00', fontSize = 64) {
  const canvas = document.createElement('canvas');
  const ctx = canvas.getContext('2d');
  const size = 128;
  canvas.width = size;
  canvas.height = size;

  ctx.clearRect(0, 0, size, size);
  ctx.font = `bold ${fontSize}px sans-serif`;
  ctx.fillStyle = color;
  ctx.textAlign = 'center';
  ctx.textBaseline = 'middle';
  ctx.fillText(letter, size / 2, size / 2);

  const texture = new THREE.CanvasTexture(canvas);
  texture.needsUpdate = true;

  const material = new THREE.SpriteMaterial({ map: texture, transparent: true });
  return new THREE.Sprite(material);
}

function addNSEWMarkers(scene, config) {
  let directions;

  if (config.scene.coordinateSystem === 'ENU') {
    // Camera-relative tweak: rotate marker layout so N→E→S→W matches view
directions = [
  { label: 'S', x: 0, z:  config.scene.nsewDistance },  // +Z
  { label: 'N', x: 0, z: -config.scene.nsewDistance },  // -Z
  { label: 'E', x:  config.scene.nsewDistance, z: 0 },  // +X
  { label: 'W', x: -config.scene.nsewDistance, z: 0 }   // -X
];
  } else {
    // Legacy layout
    directions = [
      { label: 'N', x: 0, z: -config.scene.nsewDistance },
      { label: 'E', x:  config.scene.nsewDistance, z: 0 },
      { label: 'S', x: 0, z:  config.scene.nsewDistance },
      { label: 'W', x: -config.scene.nsewDistance, z: 0 }
    ];
  }

  directions.forEach(dir => {
    const sprite = createLetterSprite(dir.label, config.scene.nsewColor);
    sprite.position.set(dir.x, config.scene.nsewHeight, dir.z);
    sprite.scale.set(1, 1, 1);
    scene.add(sprite);
  });
}

export { addCrossHatch, addNSEWMarkers };
