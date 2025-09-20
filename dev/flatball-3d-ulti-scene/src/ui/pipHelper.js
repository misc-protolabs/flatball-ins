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

// piphelper.js

import { renderHUD } from './hudHandler.js';

export function testPipHUD(config) {
  const hudConfig = config.hudPiP;
  if (hudConfig?.enabled && hudConfig?.show) {
    console.log('[testPipHUD] Rendering HUD with full config:', hudConfig);
    renderHUD('pip_test', { ...hudConfig });
  } else {
    console.log('[testPipHUD] Pip HUD is disabled.');
  }
}

export function runPipInstall(config) {
  const pipConfig = config.pip;
  if (!pipConfig?.enabled) {
    console.log('Pip install skipped: disabled in config.');
    return;
  }

  renderHUD('installing', {
    message: 'Starting pip install...',
    ...config.hudPiP
  });

  // TODO: implement actual pip install logic
  // - use pipConfig.requirements_file
  // - respect pipConfig.extra_index_urls, trusted_hosts
  // - apply timeout, cache, upgrade flags

  // Simulate success for now
  setTimeout(() => {
    renderHUD('success', {
      message: 'Pip install completed.',
      ...config.hudPiP
    });

    // Run post-install hooks
    pipConfig.post_install_hooks?.forEach(hook => {
      try {
        hook(); // or invoke by name if string
      } catch (err) {
        console.error('Post-install hook failed:', err);
      }
    });
  }, 1000);
}

export function initPipFlow(config) {
  if (!config?.hudPiP?.enabled || !config?.hudPiP?.show) {
    console.log('HUD PiP is disabled. Skipping HUD render.');
  } else {
    testPipHUD(config);
  }

  if (config?.pip?.auto_run === true) {
    console.log('Auto-run enabled. Starting pip install...');
    runPipInstall(config);
  } else {
    console.log('Auto-run disabled. Pip install skipped.');
  }
}
