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

export let csvData = [];

export class CsvPlayback {
  constructor(data, onData, intervalMs = 50) {
    this.data = data;
    this.onData = onData;
    this.intervalMs = intervalMs;
    this.index = 0;
    this.timer = null;
  }

  start() {
    console.log('[CSV] ▶️ Starting playback');
    this.index = 0;
    this.timer = setInterval(() => {
      if (this.index >= this.data.length) {
        console.log('[CSV] 🏁 Playback complete');
        this.stop();
        return;
      }

      const row = this.data[this.index];
      this.onData(row);
      this.index++;
    }, this.intervalMs);
  }

  stop() {
    console.log('[CSV] ⏹️ Stopping playback');
    if (this.timer) {
      clearInterval(this.timer);
      this.timer = null;
    }
  }

	pause() {
	  if (this.timer) {
		clearInterval(this.timer);
		this.timer = null;
		console.log('[CSV] ⏸️ Playback paused');
	  }
	}

	resume() {
	  if (!this.timer && this.index < this.data.length) {
		console.log('[CSV] ▶️ Resuming playback');
		this.timer = setInterval(() => {
		  if (this.index >= this.data.length) {
			console.log('[CSV] 🏁 Playback complete');
			this.stop();
			return;
		  }

		  const row = this.data[this.index];
		  this.onData(row);
		  this.index++;
		}, this.intervalMs);
	  }
	}

}

export function initCSVLoader(onLoadCallback) {
  const input = document.getElementById('csvInput');
  if (!input) {
    console.warn('[CSV] ❌ File input element #csvInput not found');
    return;
  }

  input.addEventListener('change', (event) => {
    const file = event.target.files[0];
    if (!file) {
      console.warn('[CSV] ⚠️ No file selected');
      return;
    }

    const reader = new FileReader();
    reader.onload = (e) => {
      csvData = parseCSV(e.target.result);
      console.log(`[CSV] ✅ Loaded ${csvData.length} rows`);

      if (typeof onLoadCallback === 'function') {
        onLoadCallback();
      }
    };
    reader.readAsText(file);
  });
}

function parseCSV(text) {
  const lines = text.trim().split('\n');
  const headers = lines.shift().split(',').map(h => h.trim());

  return lines.map(line => {
    const cols = line.split(',').map(c => c.trim());
    const obj = {};
    headers.forEach((h, i) => {
      const num = parseFloat(cols[i]);
      obj[h] = isNaN(num) ? cols[i] : num;
    });
    return obj;
  });
}
