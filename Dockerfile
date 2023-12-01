FROM debian:12 as build

RUN apt-get update && \
    apt-get install -y \
        python3-pip \
        python3.11-venv \
        cmake \
        ca-certificates


RUN python3 -m venv /app/venv

RUN /bin/bash -c "source /app/venv/bin/activate && pip3 install --no-cache-dir conan==1.59"

COPY conanfile.txt /app/

RUN mkdir /app/build && cd /app/build && \
    /bin/bash -c "source /app/venv/bin/activate && conan install .. --build=missing -s build_type=Release -s compiler.libcxx=libstdc++11"

COPY ./src /app/src
COPY CMakeLists.txt /app/

RUN cd /app/build && \
    /bin/bash -c "source /app/venv/bin/activate && cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build ."

FROM debian:12 as run

RUN apt-get update && \
    apt-get install -y \
        ca-certificates

COPY --from=build /app/build/bin/gp_time_sender /app/

RUN chmod +x /app/gp_time_sender

ENTRYPOINT ["/app/gp_time_sender"]

