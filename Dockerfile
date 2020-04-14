FROM mysql:5.7.28

RUN set -x \
        && apt-get update && apt-get install -y --no-install-recommends \
	qt5-default \
	build-essential \
	cmake \
	libssl-dev \
	libncurses5-dev \
	pkg-config \
	libmysqlclient-dev \
	zlib1g-dev \
	curl \
	ca-certificates

RUN	mkdir /source/ && \
	curl -o /source/mysql-5.7.28.tar.gz https://cdn.mysql.com/archives/mysql-5.7/mysql-5.7.28.tar.gz && \
	mkdir -p /source/mysql-5.7.28 && \
	tar -xzf /source/mysql-5.7.28.tar.gz -C /source && \
	rm /source/mysql-5.7.28.tar.gz

WORKDIR /source/mysql-5.7.28
RUN cmake . -DDOWNLOAD_BOOST=1 -DWITH_BOOST=/source/boost && \
    make -j$((`nproc`+1))

COPY ./src /source/dimaqs/src
COPY ./lib /source/dimaqs/lib
COPY ./Dimaqs.pro /source/dimaqs/Dimaqs.pro
COPY ./.qmake.conf /source/dimaqs/.qmake.conf

WORKDIR /source/dimaqs
RUN qmake && \
    make -j$((`nproc`+1))
