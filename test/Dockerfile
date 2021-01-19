# docker build -f Dockerfile -t pgbackrest-test/main .
# docker run -it -h pgbackrest-test -v /var/run/docker.sock:/var/run/docker.sock -v ~/Documents/Code/pgbackrest:/home/docker/pgbackrest pgbackrest-test/main bash

FROM ubuntu:focal

# Suppress "dpkg-reconfigure: unable to re-open stdin: No file or directory" warning
RUN export DEBCONF_NONINTERACTIVE_SEEN=true DEBIAN_FRONTEND=noninteractive

# Assign a host name
# RUN sed -i 's/^127\.0\.0\.1\t.*/127\.0\.0\.1\tlocalhost pgbackrest-test/' /etc/hosts
# RUN hostnamectl set-hostname pgbackrest-test

RUN apt-get update
RUN apt-get install -y sudo

# Create /tmp/pgbackrest and give ownership to root so we know unit tests are not writing there
RUN sudo mkdir -p /tmp/pgbackrest
RUN sudo chown root:root /tmp/pgbackrest
RUN sudo chmod 700 /tmp/pgbackrest

# Install Perl modules
RUN apt-get install -y libdbd-pg-perl libxml-checker-perl libyaml-libyaml-perl

# Install Build Tools
RUN export DEBCONF_NONINTERACTIVE_SEEN=true DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y devscripts build-essential lintian git cloc txt2man \
      debhelper libssl-dev zlib1g-dev libperl-dev libxml2-dev liblz4-dev \
      liblz4-tool libpq-dev valgrind lcov autoconf-archive zstd libzstd-dev \
      bzip2 libbz2-dev

# Install Dev Utilities
RUN apt-get install -y vim htop jq rsync pkg-config

# Install Docker
RUN curl -fsSL https://get.docker.com | sh

# Create docker user
RUN adduser --ingroup=docker --disabled-password --gecos \"\" docker

# Configure sudo
RUN echo '%docker ALL=(ALL) NOPASSWD: ALL' >> /etc/sudoers

# Mount tmpfs at /home/docker/test for faster testing
RUN sudo -u docker mkdir -m 770 /home/docker/test
RUN echo 'tmpfs /home/docker/test tmpfs size=4096M 0 1' >> /etc/fstab

USER docker
WORKDIR /home/docker
