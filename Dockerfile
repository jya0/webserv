# Use Debian as the base image
FROM debian:bullseye

# Install required dependencies
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    valgrind

# Set the working directory
WORKDIR /usr/src/app

# Copy the web server files to the container
COPY . .

# Expose the port on which the web server will run
EXPOSE 8001

# Command to run the web server with Valgrind
CMD ["./webserv", "conf.d/startup.conf"]

