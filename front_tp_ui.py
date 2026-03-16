#!/usr/bin/env python3
import curses
import subprocess
import os
import time

ROOT = os.path.dirname(os.path.abspath(__file__))

MENU = [
    ("1", "exo1_local (rpcgen dictionnaire local)", "exo1_local/mmots", [
        ["make"],
        ["sh", "-c", "./mmotsd >/tmp/mmotsd_local.log 2>&1 & echo $!"],
        ["sh", "-c", "sleep 1; printf 'I\\nn\\ni\\nalpha\\nn\\nq\\n' | ./mmots localhost"],
        ["sh", "-c", "pkill mmotsd || true"],
    ]),
    ("2", "exo1_remote (rpcgen dictionnaire distant)", "exo1_remote", [
        ["make"],
        ["sh", "-c", "./server/mmotsd_remote >/tmp/mmotsd_remote.log 2>&1 & echo $!"],
        ["sh", "-c", "sleep 1; printf 'I\\nn\\ni\\ncloud\\nn\\nq\\n' | ./client/mmots_remote localhost"],
        ["sh", "-c", "pkill mmotsd_remote || true"],
    ]),
    ("3", "exo2 (increment RPC)", "exo2", [
        ["make"],
        ["sh", "-c", "./incr_server >/tmp/incr_server.log 2>&1 & echo $!"],
        ["./incr_client", "localhost", "7"],
        ["sh", "-c", "pkill incr_server || true"],
    ]),
    ("4", "exo3 (ops a+b/a/b/a-b/a*b RPC)", "exo3", [
        ["make"],
        ["sh", "-c", "./calc_server >/tmp/calc_server.log 2>&1 & echo $!"],
        ["./calc_client", "localhost", "6", "3"],
        ["./calc_client", "localhost", "5", "0"],
        ["sh", "-c", "pkill calc_server || true"],
    ]),
    ("5", "exo4 (sockets)", "exo4", [
        ["make"],
        ["sh", "-c", "./socket_server >/tmp/socket_server.log 2>&1 & echo $!"],
        ["./socket_client", "127.0.0.1", "8", "4"],
        ["./socket_client", "127.0.0.1", "5", "0"],
        ["sh", "-c", "pkill socket_server || true"],
    ]),
]


def require_rpcbind():
    try:
        subprocess.run(["rpcinfo", "-p"], check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        return True
    except subprocess.CalledProcessError:
        return False


def run_commands(base, commands):
    output_lines = []
    for cmd in commands:
        proc = subprocess.run(cmd, cwd=os.path.join(ROOT, base), capture_output=True, text=True)
        if proc.stdout:
            output_lines.append(proc.stdout)
        if proc.stderr:
            output_lines.append(proc.stderr)
    return "".join(output_lines)


def main(stdscr):
    curses.curs_set(0)
    stdscr.nodelay(False)

    while True:
        stdscr.clear()
        stdscr.addstr(0, 0, "===== FRONT TP UI =====")
        for idx, (key, label, _, _) in enumerate(MENU, start=2):
            stdscr.addstr(idx, 0, f"{key}) {label}")
        stdscr.addstr(len(MENU) + 2, 0, "q) quitter")
        stdscr.addstr(len(MENU) + 4, 0, "Choix: ")
        stdscr.refresh()

        ch = stdscr.getkey()
        if ch == 'q':
            break
        choice = next((m for m in MENU if m[0] == ch), None)
        if not choice:
            continue

        key, label, base, commands = choice
        stdscr.clear()
        stdscr.addstr(0, 0, f"Exécution {label}...")
        stdscr.refresh()

        if key in ('1', '2', '3', '4') and not require_rpcbind():
            stdscr.addstr(2, 0, "rpcbind non lancé. Lancez: sudo rpcbind -w")
            stdscr.addstr(4, 0, "Appuyez sur une touche pour revenir au menu")
            stdscr.getch()
            continue

        log = run_commands(base, commands)
        stdscr.clear()
        stdscr.addstr(0, 0, f"Résultat {label}:")
        lines = log.splitlines() or ["(pas de sortie)"]
        max_y, max_x = stdscr.getmaxyx()
        for i, line in enumerate(lines[:max_y - 4], start=2):
            stdscr.addstr(i, 0, line[:max_x - 1])
        stdscr.addstr(max_y - 1, 0, "Appuyez sur une touche pour revenir")
        stdscr.refresh()
        stdscr.getch()


if __name__ == "__main__":
    curses.wrapper(main)
