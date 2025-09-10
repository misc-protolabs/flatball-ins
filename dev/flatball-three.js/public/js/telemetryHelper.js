export function connectTelemetry(config, onData) {
  if (!config || !config.url) {
    console.warn('[Telemetry] Missing config or URL');
    return;
  }

  console.log(`[Telemetry] Attempting to connect to ${config.url}...`);

  const socket = new WebSocket(config.url);

  socket.onopen = () => {
    console.log(`[Telemetry] ✅ Connected to ${config.url}`);
  };

  socket.onmessage = (event) => {
    if (!event.data) {
      console.warn('[Telemetry] Received empty message');
      return;
    }

    try {
      const data = JSON.parse(event.data);

      if (typeof data.roll === 'number' &&
          typeof data.pitch === 'number' &&
          typeof data.yaw === 'number') {
        console.log(`📦 Orientation: roll=${data.roll}, pitch=${data.pitch}, yaw=${data.yaw}`);
      } else {
        console.warn('[Telemetry] Orientation fields missing or invalid:', data);
      }

      onData(data); // Pass to your scene logic
    } catch (err) {
      console.warn('[Telemetry] Malformed packet:', event.data);
      console.error(err);
    }
  };

  socket.onerror = (err) => {
    console.error('[Telemetry] ❌ WebSocket error:', err);
  };

  socket.onclose = (event) => {
    console.warn(`[Telemetry] 🔌 Connection closed (code ${event.code})`);
  };
}
