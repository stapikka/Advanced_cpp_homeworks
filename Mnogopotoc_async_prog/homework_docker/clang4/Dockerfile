FROM ubuntu:latest
RUN apt-get update && apt-get install -y clang
COPY . /usr/src/Dockertest
WORKDIR /usr/src/Dockertest
RUN clang++ -o main main.cpp