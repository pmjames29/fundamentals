# linux version:
FROM rockylinux:9

# udpate and get dev tools:
RUN dnf update -y
RUN dnf clean all
RUN dnf makecache --refresh
RUN dnf group install -y "Development Tools"
RUN dnf group install -y "RPM Development Tools"
RUN dnf install -y epel-release
RUN dnf install -y sudo
RUN dnf install -y htop
RUN dnf install -y bash-completion
RUN dnf install -y man man-pages

# setup .bashrc:
RUN echo "alias ll='ls -al'" >> ~/.bashrc
# RUN echo "export LD_PRELOAD=libXext.so.6" >> ~/.bashrc

# Display for local apps:
# ENV DISPLAY=host.docker.internal:0.0

# root is working directory:
WORKDIR /root/

# calls .bashrc at startup (aliases, etc.):
ENTRYPOINT ["/bin/bash", "-it"]
