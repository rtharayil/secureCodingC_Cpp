CID=$(sudo docker ps -q --filter ancestor=cpp-csrf-session)
if [ -n "$CID" ]; then
  sudo docker kill $CID
else
  echo "No running container found for image: cpp-csrf-session"
fi
