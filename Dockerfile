FROM node:14.5.0

# Installing node-gyp dependencies and git
RUN apt-get -y update
RUN apt-get -y install build-essential
RUN apt-get install -y git

# Installing golang
RUN curl -O https://dl.google.com/go/go1.14.6.linux-amd64.tar.gz
RUN sha256sum go1.14.6.linux-amd64.tar.gz
RUN tar xvf go1.14.6.linux-amd64.tar.gz
RUN chown -R root:root ./go
RUN mv go /usr/local
ENV PATH=$PATH:/usr/local/go/bin

# Installing node-gyp
RUN npm install -g node-gyp
RUN apt update
RUN apt -y install build-essential zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libreadline-dev libffi-dev libsqlite3-dev wget libbz2-dev
RUN wget https://www.python.org/ftp/python/3.9.1/Python-3.9.1.tgz
RUN tar -xf Python-3.9.1.tgz
RUN cd Python-3.9.1 && ./configure --enable-optimizations && make -j 12 && make altinstall
