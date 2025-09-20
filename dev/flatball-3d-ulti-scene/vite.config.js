
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
