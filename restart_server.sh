# PORT=2029 ./restart_server.sh
./compile_server.sh
brew services stop nginx
cgi-fcgi -connect "127.0.0.1:$PORT" server.fcgi
echo "events{} http { server {listen 9999; location / { fastcgi_pass 127.0.0.1:$PORT; include fastcgi_params; } } }" > /opt/homebrew/etc/nginx/nginx.conf
brew services run nginx
