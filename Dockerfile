FROM ubuntu:latest 

WORKDIR /app

RUN apt-get update && apt-get install -y 
RUN apt-get install -y build-essential cmake
RUN apt-get install -y ninja-build
RUN apt-get install -y ffmpeg

COPY CMakeLists.txt .
COPY /include ./include 
COPY /src ./src
COPY /apps ./apps
COPY bulk_convert.sh .
RUN chmod +x bulk_convert.sh

RUN cmake -S . -B build -GNinja -DBUILD_TESTING=OFF
RUN cmake --build build --config release
RUN mkdir -p /output
CMD [ "./bulk_convert.sh"]