import * as THREE from 'three';
import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader.js';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';
import { Box3, Vector3 } from 'three';

// --- SCENE SETUP ---
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
const renderer = new THREE.WebGLRenderer({ antialias: true });

renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

// --- HELPER TOOLS (for debugging) ---
// Add axes to visualize the scene's origin (0,0,0)
const axesHelper = new THREE.AxesHelper(10); 
scene.add(axesHelper);

// --- FIELD & NSEW MARKERS ---
// Create a grid to represent the field
const fieldSize = 20; 
const divisions = 20;
const gridHelper = new THREE.GridHelper(fieldSize, divisions, 0x0000ff, 0x888888);
scene.add(gridHelper);

// Function to create NSEW markers for the field
function createNSEWMarkers() {
  const markerMaterial = new THREE.MeshBasicMaterial({ color: 0x0000ff });
  const markerGeometry = new THREE.BoxGeometry(0.5, 0.5, 0.5);

  const nMarker = new THREE.Mesh(markerGeometry, markerMaterial);
  nMarker.position.set(0, 0, -fieldSize / 2);
  scene.add(nMarker);

  const sMarker = new THREE.Mesh(markerGeometry, markerMaterial);
  sMarker.position.set(0, 0, fieldSize / 2);
  scene.add(sMarker);
  
  const eMarker = new THREE.Mesh(markerGeometry, markerMaterial);
  eMarker.position.set(fieldSize / 2, 0, 0);
  scene.add(eMarker);
  
  const wMarker = new THREE.Mesh(markerGeometry, markerMaterial);
  wMarker.position.set(-fieldSize / 2, 0, 0);
  scene.add(wMarker);
}
createNSEWMarkers();

// --- CSV MARKER LOGIC ---
const markerGroup = new THREE.Group();
scene.add(markerGroup);

function createMarkers(csvData) {
  markerGroup.clear(); // Clear any existing markers

  const lines = csvData.split('\n');
  const markerMaterial = new THREE.MeshBasicMaterial({ color: 0xffff00 }); // Changed to yellow for better visibility
  const markerGeometry = new THREE.SphereGeometry(0.2, 16, 16); // Adjusted size to be more visible

  lines.forEach((line) => {
    const values = line.split(',');
    if (values.length >= 3) {
      const x = parseFloat(values[0]);
      const y = parseFloat(values[1]);
      const z = parseFloat(values[2]);

      if (!isNaN(x) && !isNaN(y) && !isNaN(z)) {
        const marker = new THREE.Mesh(markerGeometry, markerMaterial);
        marker.position.set(x, y, z);
        markerGroup.add(marker);
      }
    }
  });
  console.log(`Created ${markerGroup.children.length} markers.`);
}

// Set up the file uploader
const uploader = document.getElementById('uploader');
const uploaderButton = document.getElementById('uploader-button');

if (uploaderButton) {
  uploaderButton.addEventListener('click', () => {
    uploader.click(); // Trigger the hidden file input
  });
}

if (uploader) {
  uploader.addEventListener('change', (event) => {
    const file = event.target.files[0];
    if (file) {
      const reader = new FileReader();
      reader.onload = (e) => {
        const csvContent = e.target.result;
        createMarkers(csvContent);
      };
      reader.readAsText(file);
    }
  });
}

// --- GLTF LOADER ---
const loader = new GLTFLoader();

// Updated to your correct model path
loader.load(
  './public/models/flatball-usau-frisbee.gltf', 
  function (gltf) {
    scene.add(gltf.scene);
    console.log('Model loaded successfully!');

    const box = new Box3().setFromObject(gltf.scene);
    const center = new Vector3();
    box.getCenter(center);

    const size = new Vector3();
    box.getSize(size);
    const maxDim = Math.max(size.x, size.y, size.z);
    const fov = camera.fov * (Math.PI / 180);
    let cameraZ = Math.abs(maxDim / 2 / Math.tan(fov / 2));
    
    // Position the camera to a better angle for a flat object
    camera.position.set(center.x, center.y + maxDim / 2, center.z + cameraZ);
    camera.lookAt(center);

    const controls = new OrbitControls(camera, renderer.domElement);
    controls.target.copy(center);
    controls.update();
  },
  undefined,
  function (error) {
    console.error('An error happened while loading the model:', error);
  }
);

// --- RENDERER AND ANIMATION LOOP ---
function animate() {
  requestAnimationFrame(animate);
  renderer.render(scene, camera);
}

window.addEventListener('resize', () => {
  camera.aspect = window.innerWidth / window.innerHeight;
  camera.updateProjectionMatrix();
  renderer.setSize(window.innerWidth, window.innerHeight);
});

animate();