const express = require('express');
const { exec } = require('child_process');
const path = require('path');

const ROOT = path.join(__dirname, '..');
const app = express();
app.use(express.json());
app.use(express.static(path.join(__dirname, 'public')));

function run(cmd, cwd) {
  return new Promise((resolve, reject) => {
    exec(cmd, { cwd, shell: '/bin/bash' }, (err, stdout, stderr) => {
      if (err) return reject(stderr || err.message);
      resolve(stdout + stderr);
    });
  });
}

async function ensureRpcbind() {
  try {
    await run('rpcinfo -p >/dev/null');
    return null;
  } catch (_) {
    return '[!] rpcbind non lancé. Lancez: sudo rpcbind -w';
  }
}

const demos = {
  exo1_local: async () => {
    const err = await ensureRpcbind();
    if (err) return err;
    const base = path.join(ROOT, 'exo1_local/mmots');
    let out = '';
    out += await run('make', base);
    out += await run('./mmotsd >/tmp/mmotsd_local.log 2>&1 & echo $!', base);
    await new Promise(r => setTimeout(r, 800));
    out += await run("printf 'I\nn\ni\nalpha\nn\nq\n' | ./mmots localhost", base);
    await run('pkill mmotsd || true', base);
    return out;
  },
  exo1_remote: async () => {
    const err = await ensureRpcbind();
    if (err) return err;
    const base = path.join(ROOT, 'exo1_remote');
    let out = '';
    out += await run('make', base);
    out += await run('./server/mmotsd_remote >/tmp/mmotsd_remote.log 2>&1 & echo $!', base);
    await new Promise(r => setTimeout(r, 800));
    out += await run("printf 'I\nn\ni\ncloud\nn\nq\n' | ./client/mmots_remote localhost", base);
    await run('pkill mmotsd_remote || true', base);
    return out;
  },
  exo2: async () => {
    const err = await ensureRpcbind();
    if (err) return err;
    const base = path.join(ROOT, 'exo2');
    let out = '';
    out += await run('make', base);
    out += await run('./incr_server >/tmp/incr_server.log 2>&1 & echo $!', base);
    await new Promise(r => setTimeout(r, 500));
    out += await run('./incr_client localhost 7', base);
    await run('pkill incr_server || true', base);
    return out;
  },
  exo3: async () => {
    const err = await ensureRpcbind();
    if (err) return err;
    const base = path.join(ROOT, 'exo3');
    let out = '';
    out += await run('make', base);
    out += await run('./calc_server >/tmp/calc_server.log 2>&1 & echo $!', base);
    await new Promise(r => setTimeout(r, 500));
    out += await run('./calc_client localhost 6 3', base);
    out += await run('./calc_client localhost 5 0', base);
    await run('pkill calc_server || true', base);
    return out;
  },
  exo4: async () => {
    const base = path.join(ROOT, 'exo4');
    let out = '';
    out += await run('make', base);
    out += await run('./socket_server >/tmp/socket_server.log 2>&1 & echo $!', base);
    await new Promise(r => setTimeout(r, 300));
    out += await run('./socket_client 127.0.0.1 8 4', base);
    out += await run('./socket_client 127.0.0.1 5 0', base);
    await run('pkill socket_server || true', base);
    return out;
  }
};

app.post('/run', async (req, res) => {
  const { exo } = req.body || {};
  const fn = demos[exo];
  if (!fn) return res.status(400).json({ error: 'exercice inconnu' });
  try {
    const output = await fn();
    res.json({ ok: true, output });
  } catch (e) {
    res.status(500).json({ ok: false, error: String(e) });
  }
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => console.log(`Front TP web prêt sur http://localhost:${PORT}`));
