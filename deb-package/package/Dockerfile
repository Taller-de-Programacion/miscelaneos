FROM ubuntu

COPY test-client_0.0.1.deb /tmp

RUN apt update && \
apt install -y cowsay && \
dpkg -i /tmp/test-client_0.0.1.deb

CMD test-client
