FROM debian:bullseye

RUN apt-get update && \
    apt-get install -y g++ make curl iputils-ping && \
    apt-get clean

COPY . /app
WORKDIR /app

RUN g++ main.cpp -o pingserver -pthread

EXPOSE 8080
CMD ["./pingserver"]
