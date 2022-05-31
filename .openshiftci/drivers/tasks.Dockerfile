FROM module-srcs:latest AS sources

FROM registry.fedoraproject.org/fedora:36

ARG USE_KERNELS_FILE=false
ENV USE_KERNELS_FILE=$USE_KERNELS_FILE

COPY /kernel-modules/dockerized/scripts/ /scripts/
COPY /kernel-modules/build/apply-blocklist.py /scripts/
COPY /kernel-modules/BLOCKLIST /scripts/
COPY /kernel-modules/dockerized/BLOCKLIST /scripts/dockerized/
COPY /kernel-modules/KERNEL_VERSIONS /KERNEL_VERSIONS
COPY --from=sources /kobuild-tmp/versions-src /kobuild-tmp/versions-src

RUN /scripts/get-build-tasks.sh; rm -rf /bundles/ /kobuild-tmp/ /kernel-modules/
