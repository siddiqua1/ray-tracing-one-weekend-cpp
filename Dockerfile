FROM ubuntu:24.04 

WORKDIR /app

RUN apt-get update && apt-get install -y 
RUN apt-get install -y build-essential cmake
RUN apt-get install -y ninja-build
RUN apt-get install -y imagemagick
RUN apt-get install -y python3

COPY CMakeLists.txt .
COPY /include ./include 
COPY /src ./src
COPY /apps ./apps

RUN cmake -S . -B build -GNinja -DBUILD_TESTING=OFF
RUN cmake --build build --config release
RUN mkdir -p ./output

COPY bulk_convert.py .
CMD [ "python3", "bulk_convert.py"]