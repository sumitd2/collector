# Declare TARGETARCH to make it available
ARG TARGETARCH=${TARGETARCH}

FROM --platform=linux/amd64 phoronix/pts as stage-amd64
RUN cat /root/.profile

FROM --platform=linux/amd64 phoronix/pts as stage-x86_64

FROM --platform=linux/ppc64le sumitdubey/phoronix-pts-ppc64le as stage-ppc64le

