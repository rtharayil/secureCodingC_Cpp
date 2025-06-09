FROM debian:bullseye

RUN apt-get update && apt-get install -y g++ make curl

COPY . /app
WORKDIR /app

RUN g++ main.cpp -o csrf_demo -std=c++17 -lpthread

EXPOSE 8080
CMD ["./csrf_demo"]
