# Build docker image
docker build -t cpp-sqli-demo .

# Run container
docker run -p 8080:8080 cpp-sqli-demo
