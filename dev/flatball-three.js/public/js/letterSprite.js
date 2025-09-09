import * as THREE from 'three';

export function createLetterSprite(letter, color = '#ffffff') {
  const fontSize = 64;
  const canvas = document.createElement('canvas');
  const ctx = canvas.getContext('2d');
  canvas.width = fontSize;
  canvas.height = fontSize;

  ctx.font = `${fontSize * 0.8}px Arial`;
  ctx.fillStyle = color;
  ctx.textAlign = 'center';
  ctx.textBaseline = 'middle';
  ctx.fillText(letter, fontSize / 2, fontSize / 2);

  const texture = new THREE.CanvasTexture(canvas);
  texture.minFilter = THREE.LinearFilter;
  texture.magFilter = THREE.LinearFilter;

  const material = new THREE.SpriteMaterial({ map: texture, transparent: true });
  const sprite = new THREE.Sprite(material);

  return sprite;
}
