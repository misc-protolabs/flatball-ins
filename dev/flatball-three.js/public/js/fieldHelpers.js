import * as THREE from 'three';
import { createLetterSprite } from './letterSprite.js'; // assuming you have this helper

export function addCrossHatch(scene, config) {
  // Remove old field if re‑initializing
  const oldField = scene.getObjectByName('ultimateField');
  if (oldField) scene.remove(oldField);

  const group = new THREE.Group();
  group.name = 'ultimateField';

  const fieldLength = config.field.length; // total length incl. end zones
  const fieldWidth = config.field.width;
  const endZoneDepth = (fieldLength - 64) / 2; // regulation: 64m playing + 9m end zones

  // === Field surface ===
  const fieldGeometry = new THREE.PlaneGeometry(fieldWidth, fieldLength);
  const fieldMaterial = new THREE.MeshBasicMaterial({
    color: config.field.color,
    side: THREE.DoubleSide
  });
  const fieldMesh = new THREE.Mesh(fieldGeometry, fieldMaterial);
  fieldMesh.rotation.x = -Math.PI / 2;
  group.add(fieldMesh);

  // === Field lines ===
  const lineMaterial = new THREE.LineBasicMaterial({ color: 0xffffff, linewidth: 2 });

  function addLine(x1, z1, x2, z2) {
    const points = [new THREE.Vector3(x1, 0.01, z1), new THREE.Vector3(x2, 0.01, z2)];
    const geometry = new THREE.BufferGeometry().setFromPoints(points);
    const line = new THREE.Line(geometry, lineMaterial);
    group.add(line);
  }

  const halfW = fieldWidth / 2;
  const halfL = fieldLength / 2;

  // Outer boundary
  addLine(-halfW, -halfL, halfW, -halfL); // South sideline
  addLine(-halfW, halfL, halfW, halfL);   // North sideline
  addLine(-halfW, -halfL, -halfW, halfL); // West end line
  addLine(halfW, -halfL, halfW, halfL);   // East end line

  // End zone lines
  addLine(-halfW, -halfL + endZoneDepth, halfW, -halfL + endZoneDepth); // South EZ front
  addLine(-halfW, halfL - endZoneDepth, halfW, halfL - endZoneDepth);   // North EZ front

  // === Brick mark ===
  const brickMarkRadius = 0.5; // 1m diameter
  const brickMarkSegments = 32;
  const brickMarkGeometry = new THREE.CircleGeometry(brickMarkRadius, brickMarkSegments);
  const brickMarkMaterial = new THREE.MeshBasicMaterial({ color: 0xffffff });
  const brickMarkMesh = new THREE.Mesh(brickMarkGeometry, brickMarkMaterial);
  brickMarkMesh.rotation.x = -Math.PI / 2;
  brickMarkMesh.position.set(
    config.model.startPosition.x,
    0.02,
    config.model.startPosition.z
  );
  group.add(brickMarkMesh);

  scene.add(group);
}

export function addNSEWMarkers(scene, config) {
  // Remove old markers if re‑initializing
  const oldMarkers = scene.getObjectByName('nsewMarkers');
  if (oldMarkers) scene.remove(oldMarkers);

  const group = new THREE.Group();
  group.name = 'nsewMarkers';

  const halfLength = config.field.length / 2;
  const halfWidth = config.field.width / 2;
  const delta = config.scene.nsewDistanceDelta || 0;
  const height = config.scene.nsewHeight || 1.5;
  const color = config.scene.nsewColor || '#ffaa00';

  // ENU: +Z = North, +X = East
  const directions = [
    { label: 'N', x: 0, z:  halfLength + delta },
    { label: 'S', x: 0, z: -halfLength - delta },
    { label: 'E', x:  halfWidth + delta, z: 0 },
    { label: 'W', x: -halfWidth - delta, z: 0 }
  ];

  directions.forEach(dir => {
    const sprite = createLetterSprite(dir.label, color);
    sprite.position.set(dir.x, height, dir.z);
    sprite.scale.set(1, 1, 1);
    group.add(sprite);
  });

  scene.add(group);
}

export function addFieldLines(scene, config) {
  const group = new THREE.Group();
  group.name = 'fieldLines';

  const field = config.field;
  const lines = field.lines;
  if (!lines?.enabled) return;

  const halfLength = field.length / 2;
  const halfWidth = field.width / 2;
  const endZoneDepth = field.endZoneDepth;
  const brickOffset = field.brickMarkDistance;
  const lineColor = lines.color || '#ffffff';
  const lineWidth = lines.width || 0.05;

  const lineMaterial = new THREE.LineBasicMaterial({ color: lineColor });

  function addLine(x1, z1, x2, z2) {
    const points = [new THREE.Vector3(x1, 0.01, z1), new THREE.Vector3(x2, 0.01, z2)];
    const geometry = new THREE.BufferGeometry().setFromPoints(points);
    const line = new THREE.Line(geometry, lineMaterial);
    group.add(line);
  }

  // Sidelines
  if (lines.drawSidelines) {
    addLine(-halfWidth, -halfLength, halfWidth, -halfLength); // South sideline
    addLine(-halfWidth, halfLength, halfWidth, halfLength);   // North sideline
    addLine(-halfWidth, -halfLength, -halfWidth, halfLength); // West end line
    addLine(halfWidth, -halfLength, halfWidth, halfLength);   // East end line
  }

  // End zone front lines
  if (lines.drawEndZoneLines) {
    addLine(-halfWidth, -halfLength + endZoneDepth, halfWidth, -halfLength + endZoneDepth); // South EZ front
    addLine(-halfWidth, halfLength - endZoneDepth, halfWidth, halfLength - endZoneDepth);   // North EZ front
  }

  // Midline
  if (lines.drawMidline) {
    addLine(-halfWidth, 0, halfWidth, 0);
  }

  // Brick marks
  if (lines.drawBrickMarks) {
    const brickRadius = lines.brickMarkRadius || 0.5;
    const brickSegments = 32;
    const brickMaterial = new THREE.MeshBasicMaterial({ color: lineColor });
    const brickGeometry = new THREE.CircleGeometry(brickRadius, brickSegments);
    brickGeometry.rotateX(-Math.PI / 2);

    const southBrick = new THREE.Mesh(brickGeometry, brickMaterial);
    southBrick.position.set(0, 0.02, -halfLength + endZoneDepth + brickOffset);
    group.add(southBrick);

    const northBrick = new THREE.Mesh(brickGeometry, brickMaterial);
    northBrick.position.set(0, 0.02, halfLength - endZoneDepth - brickOffset);
    group.add(northBrick);
  }

  scene.add(group);
}

export function addFieldGrid(scene, config) {
  const gridCfg = config.field?.grid;
  if (!gridCfg?.enabled) return;

  const group = new THREE.Group();
  group.name = 'fieldGrid';

  const fieldLength = config.field.length;
  const fieldWidth = config.field.width;
  const spacing = gridCfg.spacing || 5;
  const color = new THREE.Color(gridCfg.color || '#cccccc');
  const lineWidth = gridCfg.width || 0.01;
  const opacity = gridCfg.opacity ?? 0.3;

  const halfL = fieldLength / 2;
  const halfW = fieldWidth / 2;

  const material = new THREE.LineBasicMaterial({
    color: color,
    linewidth: lineWidth,
    transparent: true,
    opacity: opacity
  });

  function addGridLine(x1, z1, x2, z2) {
    const points = [new THREE.Vector3(x1, 0.005, z1), new THREE.Vector3(x2, 0.005, z2)];
    const geometry = new THREE.BufferGeometry().setFromPoints(points);
    const line = new THREE.Line(geometry, material);
    group.add(line);
  }

  // Vertical lines (parallel to Z)
  for (let x = -halfW; x <= halfW; x += spacing) {
    const xPos = x;
    const zStart = gridCfg.drawBeyondField ? -halfL * 1.5 : -halfL;
    const zEnd   = gridCfg.drawBeyondField ?  halfL * 1.5 :  halfL;
    addGridLine(xPos, zStart, xPos, zEnd);
  }

  // Horizontal lines (parallel to X)
  for (let z = -halfL; z <= halfL; z += spacing) {
    const zPos = z;
    const xStart = gridCfg.drawBeyondField ? -halfW * 1.5 : -halfW;
    const xEnd   = gridCfg.drawBeyondField ?  halfW * 1.5 :  halfW;
    addGridLine(xStart, zPos, xEnd, zPos);
  }

  scene.add(group);
}
