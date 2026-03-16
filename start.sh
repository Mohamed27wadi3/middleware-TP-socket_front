#!/bin/bash
set -e

# Démarrer rpcbind (nécessaire pour exo1, exo2, exo3)
rpcbind -w 2>/dev/null || true
sleep 0.5

# Démarrer le serveur Node.js
cd /app/front_tp_web
exec node server.js
