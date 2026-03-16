#!/usr/bin/env python3
import json
import os
import subprocess
import time
import atexit
import pty
import select
from http.server import SimpleHTTPRequestHandler, HTTPServer
from urllib.parse import urlparse

ROOT = os.path.join(os.path.dirname(__file__), '..')
WEB = os.path.join(os.path.dirname(__file__), 'public')

EXO1_STATE = {
    'local': {'server': None, 'client_pid': None, 'fd': None, 'host': 'localhost'},
    'remote': {'server': None, 'client_pid': None, 'fd': None, 'host': 'localhost'},
}


def run(cmd, cwd):
    proc = subprocess.run(cmd, cwd=cwd, shell=True, text=True,
                          stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if proc.returncode != 0:
        raise RuntimeError(proc.stdout.strip())
    return proc.stdout


def require_rpcbind():
    try:
        run('rpcinfo -p localhost >/dev/null', ROOT)
        return None
    except Exception:
        return "RPCBIND_OFF"


def _read_until(fd, token, timeout=2.0):
    buf = ''
    end = time.time() + timeout
    while time.time() < end:
        r, _, _ = select.select([fd], [], [], 0.1)
        if fd in r:
            try:
                chunk = os.read(fd, 4096).decode(errors='ignore')
            except OSError:
                break
            if not chunk:
                break
            buf += chunk
            if token in buf:
                break
    return buf


def _read_available(fd, timeout=1.0):
    buf = ''
    end = time.time() + timeout
    while time.time() < end:
        r, _, _ = select.select([fd], [], [], 0.1)
        if fd in r:
            try:
                chunk = os.read(fd, 4096).decode(errors='ignore')
            except OSError:
                break
            if not chunk:
                break
            buf += chunk
        else:
            break
    return buf


def _start_exo1_server(mode):
    state = EXO1_STATE[mode]
    proc = state['server']
    if proc and proc.poll() is None:
        return
    if mode == 'local':
        base = os.path.join(ROOT, 'exo1_local', 'mmots')
        cmd = './mmotsd'
    else:
        base = os.path.join(ROOT, 'exo1_remote', 'server')
        cmd = './mmotsd_remote'
    try:
        run('make', base)
    except Exception:
        pass
    proc = subprocess.Popen(cmd, cwd=base, shell=True,
                            stdout=subprocess.DEVNULL,
                            stderr=subprocess.DEVNULL)
    state['server'] = proc
    time.sleep(0.3)


def _start_exo1_client(mode, host):
    state = EXO1_STATE[mode]
    if state['client_pid'] is not None:
        return ''

    if mode == 'local':
        base = os.path.join(ROOT, 'exo1_local', 'mmots')
        argv = ['./mmots', host]
    else:
        base = os.path.join(ROOT, 'exo1_remote')
        argv = ['./client/mmots_remote', host]

    pid, fd = pty.fork()
    if pid == 0:
        os.chdir(base)
        os.execv(argv[0], argv)

    state['client_pid'] = pid
    state['fd'] = fd
    state['host'] = host
    out = _read_until(fd, 'Commande:', timeout=2.0)
    return out


def _stop_exo1(mode):
    state = EXO1_STATE[mode]
    fd = state['fd']
    pid = state['client_pid']
    if fd is not None:
        try:
            os.close(fd)
        except Exception:
            pass
    if pid is not None:
        try:
            os.waitpid(pid, 0)
        except Exception:
            pass
    state['fd'] = None
    state['client_pid'] = None

    proc = state['server']
    if proc and proc.poll() is None:
        proc.terminate()
        try:
            proc.wait(timeout=1.0)
        except Exception:
            proc.kill()
    state['server'] = None


def _run_exo1_cmd(mode, host, cmd, word=None):
    err = require_rpcbind()
    if err:
        return err

    _start_exo1_server(mode)
    output = ''
    output += _start_exo1_client(mode, host)

    state = EXO1_STATE[mode]
    fd = state['fd']
    if fd is None:
        return output

    cmd = (cmd or '').strip()
    if cmd == '':
        return output
    if cmd.upper() == 'START':
        return output

    # send command
    os.write(fd, (cmd + '\n').encode())

    if cmd.lower() in ['i', 's', 'c']:
        output += _read_until(fd, 'Mot:', timeout=2.0)
        if word is not None:
            os.write(fd, (word + '\n').encode())
            output += _read_until(fd, 'Commande:', timeout=2.0)
        return output

    if cmd.lower() == 'q':
        output += _read_available(fd, timeout=1.0)
        _stop_exo1(mode)
        return output

    output += _read_until(fd, 'Commande:', timeout=2.0)
    return output


def demo_exo2(val=7, host="localhost"):
    err = require_rpcbind()
    if err:
        return err
    base = os.path.join(ROOT, 'exo2')
    run('make -s', base)
    proc = subprocess.Popen('./increment_server', cwd=base, shell=True,
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(0.5)
    out = run(f'./increment_client {host} {val}', base)
    proc.terminate()
    try:
        proc.wait(timeout=1.0)
    except Exception:
        proc.kill()
    return out


def demo_exo3(a=6, b=3, host="localhost"):
    err = require_rpcbind()
    if err:
        return err
    base = os.path.join(ROOT, 'exo3')
    run('make -s', base)
    proc = subprocess.Popen('./calc_server', cwd=base, shell=True,
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(0.5)
    out = run(f'./calc_client {host} {a} {b}', base)
    proc.terminate()
    try:
        proc.wait(timeout=1.0)
    except Exception:
        proc.kill()
    return out


def demo_exo4(host='127.0.0.1', a=8, b=4):
    base = os.path.join(ROOT, 'exo4')
    run('make -s', base)
    proc = subprocess.Popen('./socket_server', cwd=base, shell=True,
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(0.3)
    out = run(f'./socket_client {host} {a} {b}', base)
    proc.terminate()
    try:
        proc.wait(timeout=1.0)
    except Exception:
        proc.kill()
    return out


@atexit.register
def _cleanup():
    _stop_exo1('local')
    _stop_exo1('remote')


class Handler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=WEB, **kwargs)

    def do_POST(self):
        if urlparse(self.path).path != '/run':
            self.send_error(404)
            return
        length = int(self.headers.get('content-length', 0))
        body = self.rfile.read(length) if length else b''
        try:
            payload = json.loads(body or '{}')
        except Exception:
            payload = {}
        exo = payload.get('exo')
        try:
            val = int(payload.get('val', 7)) if str(payload.get('val', '')).strip() else 7
            a = int(payload.get('a', 6)) if str(payload.get('a', '')).strip() else 6
            b = int(payload.get('b', 3)) if str(payload.get('b', '')).strip() else 3
            host_rpc = payload.get('host_rpc', 'localhost') or 'localhost'
            host_sock = payload.get('host_sock', '127.0.0.1') or '127.0.0.1'
            cmd = payload.get('cmd')
            word = payload.get('word')

            if exo == 'exo1_local':
                output = _run_exo1_cmd('local', host_rpc, cmd, word)
            elif exo == 'exo1_remote':
                output = _run_exo1_cmd('remote', host_rpc, cmd, word)
            elif exo == 'exo2':
                output = demo_exo2(val, host_rpc)
            elif exo == 'exo3':
                output = demo_exo3(a, b, host_rpc)
            elif exo == 'exo4':
                output = demo_exo4(host_sock, a, b)
            else:
                self.send_error(400, 'exo inconnu')
                return

            resp = { 'ok': True, 'output': output }
            data = json.dumps(resp).encode()
            self.send_response(200)
        except Exception as e:
            resp = { 'ok': False, 'error': str(e) }
            data = json.dumps(resp).encode()
            self.send_response(500)
        self.send_header('Content-Type', 'application/json')
        self.send_header('Content-Length', str(len(data)))
        self.end_headers()
        self.wfile.write(data)


if __name__ == '__main__':
    port = int(os.environ.get('PORT', 3000))
    server = HTTPServer(('0.0.0.0', port), Handler)
    print(f"Front TP web prêt sur http://localhost:{port}")
    server.serve_forever()
