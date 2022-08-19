# Declare TARGETARCH to make it available
ARG TARGETARCH=${TARGETARCH}

FROM --platform=linux/amd64 phoronix/pts as stage-amd64
FROM --platform=linux/amd64 phoronix/pts as stage-x86_64
FROM --platform=linux/ppc64le sumitdubey/phoronix-pts-ppc64le as stage-ppc64le

# Select final stage based on TARGETARCH ARG
FROM stage-${TARGETARCH} as final

# Commands run at final stage for all target platforms
ADD phoronix-test-suite.xml /etc/phoronix-test-suite.xml
ADD suite-definition.xml/ /var/lib/phoronix-test-suite/test-suites/local/collector/

ENTRYPOINT ["/phoronix-test-suite/phoronix-test-suite"]
