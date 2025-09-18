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

export function log(...args) {
  const message = args.map(a => (typeof a === 'object' ? JSON.stringify(a) : a)).join(' ');
  logBuffer.push(`[${new Date().toISOString()}] ${message}`);
  if (logBuffer.length > 500) logBuffer.shift(); // keep buffer size manageable
}

export function getLogDump() {
  return logBuffer.join('\n');
}

export function clearLog() {
  logBuffer.length = 0;
}
