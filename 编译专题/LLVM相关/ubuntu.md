# Ubuntu上升级Clang-LLVM(Tested on Ubuntu 20.04)
https://packages.ubuntu.com/search?keywords=llvm-11
```
- wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
- sudo apt-add-repository "deb http://apt.llvm.org/bionic/ - - llvm-toolchain-bionic-11 main"
- sudo apt-get update
- sudo apt-get install -y llvm-11 llvm-11-dev clang-11 llvm-11-tools
安装多个版本
- sudo add-apt-repository ppa:kxstudio-team/builds
- sudo apt-get update
- sudo apt install build-essential
- sudo apt-get install clang-10 clang-11
- sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-10 100 --slave /usr/bin/clang++ clang++ /usr/bin/clang++-10
- sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-11 110 --slave /usr/bin/clang++ clang++ /usr/bin/clang++-11
- sudo update-alternatives --config clang
安装LLVM开发板
- sudo apt-get install llvm-10-dev llvm-11-dev
```
# 查看Clang版本
https://releases.llvm.org/

# 查看Clang状态
https://clang.llvm.org/cxx_status.html

# 查看Clang开发时间线
https://llvm.org/docs/HowToReleaseLLVM.html
https://clang.llvm.org/docs/ReleaseNotes.html