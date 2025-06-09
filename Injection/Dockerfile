FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libsqlite3-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN g++ -std=c++17 main.cpp -lsqlite3 -o server

EXPOSE 8080

CMD ["./server"]
