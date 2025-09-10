export class TelemetryHelper {
  constructor(config, onData) {
    if (!config || !config.url) {
      console.warn('[Telemetry] Missing config or URL');
      return;
    }

    this.url = config.url;
    this.onData = onData;
    this.socket = null;

    this.messageCount = 0;
    this.lastSecond = Date.now();
    this.mps = 0;
    this.lastReportedMPS = null;
    this.lastMessageTime = Date.now();

    this.stats = {
      totalMessages: 0,
      malformedPackets: 0,
      emptyMessages: 0,
      incompletePackets: 0,
    };

    this.connect();
    this.startMonitoring();
  }

  connect() {
    console.log(`[Telemetry] Attempting to connect to ${this.url}...`);
    this.socket = new WebSocket(this.url);

    this.socket.onopen = () => {
      console.log(`[Telemetry] ✅ Connected to ${this.url}`);
    };

    this.socket.onmessage = (event) => {
      this.lastMessageTime = Date.now();
      this.handleMessage(event);
    };

    this.socket.onerror = (err) => {
      console.error('[Telemetry] ❌ WebSocket error:', err);
    };

    this.socket.onclose = (event) => {
      console.warn(`[Telemetry] 🔌 Connection closed (code ${event.code})`);
    };
  }

  handleMessage(event) {
    this.stats.totalMessages++;

    if (!event.data) {
      this.stats.emptyMessages++;
      console.warn('[Telemetry] Received empty message');
      return;
    }

    this.messageCount++;

    try {
      const data = JSON.parse(event.data);

      const hasOrientation =
        typeof data.roll === 'number' &&
        typeof data.pitch === 'number' &&
        typeof data.yaw === 'number';

      if (!hasOrientation) {
        this.stats.incompletePackets++;
        console.warn('[Telemetry] Orientation fields missing or invalid:', data);
      }

      this.onData(data);
    } catch (err) {
      this.stats.malformedPackets++;
      console.warn('[Telemetry] Malformed packet:', event.data);
      console.error(err);
    }
  }

	startMonitoring() {
	  this.monitorIntervals = [];

	  this.monitorIntervals.push(setInterval(() => {
		const now = Date.now();
		const elapsed = now - this.lastSecond;
		const currentMPS = Math.round((this.messageCount / elapsed) * 1000);

		if (currentMPS !== this.lastReportedMPS) {
		  console.log(`[Telemetry] 📊 Messages per second: ${currentMPS}`);
		  this.lastReportedMPS = currentMPS;
		}

		this.mps = currentMPS;
		this.messageCount = 0;
		this.lastSecond = now;
	  }, 1000));

	  this.monitorIntervals.push(setInterval(() => {
		const silence = Date.now() - this.lastMessageTime;
		if (silence > 3000) {
		  console.warn(`[Telemetry] ⚠️ No messages for ${silence}ms`);
		}
	  }, 1000));

	  this.monitorIntervals.push(setInterval(() => {
		if (this.socket) {
		  const states = ['CONNECTING', 'OPEN', 'CLOSING', 'CLOSED'];
		  console.log(`[Telemetry] 🔄 Socket state: ${states[this.socket.readyState]}`);
		}
	  }, 5000));
	}

	stop() {
	  console.log('[Telemetry] 🛑 Stopping telemetry stream');

	  if (this.socket) {
		this.socket.close();
		this.socket = null;
	  }

	  // Clear all intervals
	  if (this.monitorIntervals) {
		this.monitorIntervals.forEach(clearInterval);
		this.monitorIntervals = [];
	  }
	}

  getStats() {
    return {
      mps: this.mps,
      ...this.stats,
    };
  }
}
