const out = document.getElementById('term-output');
const input = document.getElementById('term-input');
const buttons = document.querySelectorAll('button[data-exo]');

let mode = null;
let step = 0;
let params = {};
let rpcbindWarned = false;

function println(text = '') {
  out.textContent += text + '\n';
  out.scrollTop = out.scrollHeight;
}

function clearOutput() {
  out.textContent = '';
}

function resetState() {
  mode = null;
  step = 0;
  params = {};
}

function focusInput() {
  setTimeout(() => input.focus(), 0);
}

async function run(exo, payload, keep = false) {
  try {
    const res = await fetch('/run', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ exo, ...payload })
    });
    const data = await res.json();
    if (!res.ok || !data.ok) {
      const err = String(data.error || res.statusText || '');
      if (err.includes('RPCBIND_OFF')) {
        if (!rpcbindWarned) {
          println('rpcbind OFF. Lance: sudo rpcbind -w');
          rpcbindWarned = true;
        }
      } else {
        println(`Erreur: ${err}`);
      }
    } else {
      const outText = String(data.output || '(pas de sortie)');
      if (outText.includes('RPCBIND_OFF')) {
        if (!rpcbindWarned) {
          println('rpcbind OFF. Lance: sudo rpcbind -w');
          rpcbindWarned = true;
        }
      } else {
        println(outText);
      }
    }
  } catch (e) {
    println(`Erreur réseau: ${e}`);
  }
  if (!keep) {
    resetState();
    println('--- fin ---');
  }
  if (keep && mode === 'exo2') {
    println('Entrer un nombre :');
  }
  if (keep && mode === 'exo3') {
    println('Entrez a et b :');
  }
  if (keep && mode === 'exo4') {
    println('Entrez a et b :');
  }
}

function startMode(exo, label) {
  resetState();
  mode = exo;
  clearOutput();
  rpcbindWarned = false;
  println(`=== ${label} ===`);

  if (exo === 'exo1_local' || exo === 'exo1_remote') {
    params.host_rpc = 'localhost';
    run(exo, { cmd: '', host_rpc: params.host_rpc }, true);
  } else if (exo === 'exo2') {
    params.host_rpc = 'localhost';
    println('Entrer un nombre :');
  } else if (exo === 'exo3') {
    params.host_rpc = 'localhost';
    println('Entrez a et b :');
  } else if (exo === 'exo4') {
    params.host_sock = '127.0.0.1';
    println('Entrez a et b :');
  }

  focusInput();
}

function handleLine(line) {
  if (!mode) {
    println('Choisissez un exercice avec les boutons.');
    return;
  }

  if (mode === 'exo1_local' || mode === 'exo1_remote') {
    const raw = line.trim();
    if (raw === '') return;

    if (step === 1 && params.pending) {
      const word = raw;
      const cmd = params.pending;
      params.pending = null;
      step = 0;
      if (!word) { println('Mot manquant.'); return; }
      return run(mode, { cmd, word, host_rpc: params.host_rpc }, true);
    }

    const cmd = raw.toLowerCase();
    if (['i', 's', 'c'].includes(cmd)) {
      params.pending = cmd;
      println('Mot:');
      step = 1;
      return;
    }

    if (raw === 'I' || ['n', 'q'].includes(cmd)) {
      const send = (raw === 'I') ? 'I' : cmd;
      return run(mode, { cmd: send, host_rpc: params.host_rpc }, true);
    }

    println('Commande inconnue.');
    return;
  }

  if (mode === 'exo2') {
    const val = line.trim();
    if (!val) { println('Valeur manquante.'); return; }
    return run(mode, { host_rpc: params.host_rpc || 'localhost', val }, true);
  }

  if (mode === 'exo3') {
    const parts = line.trim().split(/\s+/);
    if (parts.length < 2) { println('Entrez deux nombres.'); return; }
    params.a = parts[0];
    params.b = parts[1];
    return run(mode, { host_rpc: params.host_rpc || 'localhost', a: params.a, b: params.b }, true);
  }

  if (mode === 'exo4') {
    const parts = line.trim().split(/\s+/);
    if (parts.length < 2) { println('Entrez deux nombres.'); return; }
    params.a = parts[0];
    params.b = parts[1];
    return run(mode, { host_sock: params.host_sock || '127.0.0.1', a: params.a, b: params.b, a2: params.a, b2: params.b }, true);
  }

  if (step === 1 && params.pending) {
    const word = line.trim();
    const cmd = params.pending;
    params.pending = null;
    step = 0;
    if (!word) { println('Mot manquant.'); return; }
    return run(mode, { cmd, word, host_rpc: params.host_rpc }, true);
  }
}

input.addEventListener('keydown', (e) => {
  if (e.key !== 'Enter') return;
  const line = input.value;
  input.value = '';
  println(`$ ${line}`);
  handleLine(line);
});

buttons.forEach(btn => {
  btn.addEventListener('click', () => startMode(btn.dataset.exo, btn.textContent.trim()));
});

println('Front prêt. Clique un exercice pour commencer.');
focusInput();
