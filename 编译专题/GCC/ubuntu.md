# Ubuntu上升级GCC(Tested on Ubuntu 20.04)
https://launchpad.net/~ubuntu-toolchain-r/+archive/ubuntu/test
```
安装多个版本
- sudo add-apt-repository ppa:ubuntu-toolchain-r/ppa
- sudo apt update
- sudo apt install build-essential
- sudo apt install gcc-8 g++-8 gcc-9 g++-9 gcc-10 g++-10
- sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 100 --slave /usr/bin/g++ g++ /usr/bin/g++-10 --slave /usr/bin/gcov gcov /usr/bin/gcov-10
- sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 --slave /usr/bin/g++ g++ /usr/bin/g++-9 --slave /usr/bin/gcov gcov /usr/bin/gcov-9
- sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 80 --slave /usr/bin/g++ g++ /usr/bin/g++-8 --slave /usr/bin/gcov gcov /usr/bin/gcov-8
- sudo update-alternatives --config gcc

安装使用GNU/Linux进行开发的手册页
- sudo apt-get install manpages-dev
- sudo apt-get install manpages-dev man-db manpages-posix-dev
```
# 查看GCC版本
https://gcc.gnu.org/releases.html

# 查看GCC状态
https://gcc.gnu.org/projects/cxx-status.html

# 查看GCC开发时间线
https://gcc.gnu.org/develop.html#timeline