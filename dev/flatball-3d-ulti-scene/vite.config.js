// Copyright 2025 Michael V. Schaefer
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


// vite.config.js
import { defineConfig } from 'vite';

export default defineConfig({
  server: {
    open: true,
  },
  plugins: [
    {
      name: 'shutdown-on-close',
      configureServer(server) {
        // The 'sendBeacon' in index.html sends a request on page close.
        server.middlewares.use((req, res, next) => {
          // The beacon sends a POST request with an empty body.
          if (req.url === '/shutdown' && req.method === 'POST') {
            console.log('Shutdown signal received from browser. Terminating Vite server.');
            // End the response before exiting
            res.end('Server is shutting down.');
            // Close the server and exit the process
            server.close().then(() => {
              process.exit(0);
            });
          } else {
            next();
          }
        });
      }
    }
  ]
});
