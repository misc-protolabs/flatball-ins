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

// public/js/utils/logger.js
const logBuffer = [];
let config = {};

export function setConfig(newConfig) {
  config = newConfig;
}

export function log(...args) {
  const message = args.map(a => (typeof a === 'object' ? JSON.stringify(a) : a)).join(' ');
  logBuffer.push(`[${new Date().toISOString()}] ${message}`);
  if (logBuffer.length > 500) logBuffer.shift(); // keep buffer size manageable
}

export function getLogDump() {
  const logData = logBuffer.join('\n');

  if (config.logging?.dumpMethod === 'file') {
    const blob = new Blob([logData], { type: 'text/markdown;charset=utf-8' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = config.logging.dumpFilename || 'log-dump.md';
    a.click();
    URL.revokeObjectURL(url);
  } else if (config.logging?.dumpMethod === 'clipboard') {
    navigator.clipboard.writeText(logData).then(() => {
      console.log('Logs copied to clipboard');
    }, () => {
      console.error('Failed to copy logs to clipboard');
    });
  } else {
    console.log(logData);
  }
}

window.getLogDump = getLogDump;

export function clearLog() {
  logBuffer.length = 0;
}
