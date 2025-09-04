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
const axesHelper = new THREE.AxesHelper(5); 
scene.add(axesHelper);

// Optional: Add a test cube to confirm rendering is working
const geometry = new THREE.BoxGeometry(1, 1, 1);
const material = new THREE.MeshBasicMaterial({ color: 0x00ff00 });
const cube = new THREE.Mesh(geometry, material);
scene.add(cube);

// --- LIGHTING ---
const ambientLight = new THREE.AmbientLight(0xffffff, 0.8);
scene.add(ambientLight);
const directionalLight = new THREE.DirectionalLight(0xffffff, 1);
directionalLight.position.set(0, 10, 5);
scene.add(directionalLight);

// --- FIELD & MARKER LOGIC ---
const markerGroup = new THREE.Group();
scene.add(markerGroup);

function createMarkers(csvData) {
  markerGroup.clear(); // Clear any existing markers

  const lines = csvData.split('\n');
  const markerMaterial = new THREE.MeshBasicMaterial({ color: 0xff0000 });
  const markerGeometry = new THREE.SphereGeometry(0.1, 16, 16);

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

// Replace './public/models/my_model.gltf' with your model's correct path and name
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
    camera.position.set(center.x, center.y, center.z + cameraZ);

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