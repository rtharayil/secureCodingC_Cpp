FROM debian:bullseye

RUN apt-get update && \
    apt-get install -y g++ curl make && \
    apt-get clean

COPY . /app
WORKDIR /app

RUN g++ main.cpp -o xssdemo -lpthread

EXPOSE 8080
CMD ["./xssdemo"]
