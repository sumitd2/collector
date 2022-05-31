FROM registry.fedoraproject.org/fedora:36 AS builder

RUN dnf -y install \
	make \
	cmake \
	gcc-c++ \
	llvm \
	clang \
	patch \
	elfutils-libelf \
	elfutils-libelf-devel \
	git \
	python3 \
	kmod
