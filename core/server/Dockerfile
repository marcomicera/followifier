FROM zouzias/boost:1.67.0

RUN apt-get -y update \
    && apt-get -y upgrade

RUN apt-get install -y \
    openssh-server \
    g++ \
    cmake \
    git

#installing the mongoc dependencies and driver
RUN apt-get install -y \
    pkg-config \
    libssl-dev \
    libsasl2-dev
RUN cd ~ \
    && wget https://github.com/mongodb/mongo-c-driver/releases/download/1.7.0/mongo-c-driver-1.7.0.tar.gz \
    && tar xzf mongo-c-driver-1.7.0.tar.gz \
    && cd mongo-c-driver-1.7.0 \
    && ./configure --disable-automatic-init-and-cleanup \
    && make \
    && make install \
    && cd ~ \
    && rm mongo-c-driver-1.7.0.tar.gz \
    && rm -rf mongo-c-driver-1.4.2

#installing mongocxx driver - connects c++ to mongo
RUN cd ~ \
    && wget https://github.com/mongodb/mongo-cxx-driver/archive/r3.1.4.tar.gz \
    && tar -xzf r3.1.4.tar.gz \
    && cd mongo-cxx-driver-r3.1.4/build \
    && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local .. \
    && make EP_mnmlstc_core \
    && make \
    && make install \
    && cd ~ \
    && rm r3.1.4.tar.gz \
    && rm -rf mongo-cxx-driver-r3.1.4

# install additional boost
RUN cd /usr/include/boost \
    && ./bootstrap.sh \
    && ./bjam install --with-filesystem --with-thread --with-system --with-date_time --with-random link=static runtime-link=shared threading=multi

# install protobuf
RUN cd ~ \
    && apt-get install -y autoconf automake libtool curl make g++ unzip \
    && wget https://github.com/protocolbuffers/protobuf/releases/download/v3.6.1/protobuf-cpp-3.6.1.tar.gz \ 
    && cd protobuf-3.6.1 \
    && ./autogen.sh \
    && ./configure \
    && make \
    && make check \
    && make install \ 
    && ldconfig

# Copy files inside workdir
WORKDIR /server

COPY . /server

RUN rm /server/CMakeCache.txt

# Build and run server
CMD ./docker_entry.sh
