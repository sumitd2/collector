FROM registry.access.redhat.com/ubi8/ubi:8.6 AS rhel-8-base

RUN dnf config-manager \
		--enable rhel-8-for-x86_64-baseos-rpms \
		--enable rhel-8-for-x86_64-appstream-rpms && \
	dnf -y update && \
	dnf -y install \
		make \
		cmake \
		gcc-c++ \
		llvm-7.0.1 \
		clang-7.0.1 \
		elfutils-libelf \
		elfutils-libelf-devel \
		kmod
