let csvData = [];

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

function initCSVLoader(onLoadCallback) {
  const input = document.getElementById('csvInput');
  if (!input) return;

  input.addEventListener('change', (event) => {
    const file = event.target.files[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onload = (e) => {
      csvData = parseCSV(e.target.result);
      console.log(`[CSV] Loaded ${csvData.length} rows`);
      if (typeof onLoadCallback === 'function') {
        onLoadCallback();
      }
    };
    reader.readAsText(file);
  });
}

export { initCSVLoader, csvData };
