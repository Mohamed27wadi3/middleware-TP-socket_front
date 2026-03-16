FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install Node.js 22, gcc, make, rpcgen, libtirpc, rpcbind
RUN apt-get update && apt-get install -y \
        curl \
        gcc \
        make \
        rpcbind \
        libtirpc-dev \
        rpcsvc-proto \
        pkg-config \
    && curl -fsSL https://deb.nodesource.com/setup_22.x | bash - \
    && apt-get install -y nodejs \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

# Install Node.js dependencies for the web frontend
RUN cd front_tp_web && npm install

EXPOSE 3000

COPY start.sh /start.sh
RUN chmod +x /start.sh

CMD ["/start.sh"]
