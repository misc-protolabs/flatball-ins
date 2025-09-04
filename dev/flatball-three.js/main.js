// The importmap now handles the paths for us
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

// --- GLTF LOADER ---
const loader = new GLTFLoader();

// Replace './public/models/my_model.gltf' with your model's correct path and name
loader.load(
	'./public/models/flatball-usau-frisbee.gltf',
  function (gltf) {
    scene.add(gltf.scene);
    console.log('Model loaded successfully!');

    // Get the model's bounding box to determine its size and position
    const box = new Box3().setFromObject(gltf.scene);
    const center = new Vector3();
    box.getCenter(center);

    // Position the camera to fit the model in view
    const size = new Vector3();
    box.getSize(size);
    const maxDim = Math.max(size.x, size.y, size.z);
    const fov = camera.fov * (Math.PI / 180);
    let cameraZ = Math.abs(maxDim / 2 / Math.tan(fov / 2));
    camera.position.set(center.x, center.y, center.z + cameraZ);

    // Add OrbitControls for easy camera movement
    const controls = new OrbitControls(camera, renderer.domElement);
    controls.target.copy(center); // Set the point of orbit to the model's center
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