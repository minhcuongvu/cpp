# cpp on linux

## setup gdb, cmake

```shell
sudo apt update -y &&\
sudo apt install -y cmake gdb &&\
sudo add-apt-repository ppa:ubuntu-toolchain-r/test &&\
sudo apt upgrade -y libstdc++6
```

## setup GCC

download from [Index of /gnu/gcc](https://ftp.gnu.org/gnu/gcc/)

using `gcc-14`

```shell
wget https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.gz
tar -xvf gcc-14.2.0.tar.gz
cd gcc-14.2.0
```

download prerequisites

```shell
./contrib/download_prerequisites
```

create a build dir

```shell
mkdir build
cd build
```

Run the Configure Script

```shell
../configure --prefix=/usr/local/gcc-14 --enable-languages=c,c++
```

- `--prefix=/usr/local/gcc-14`: Specifies the installation directory.
- `--enable-languages=c,c++`: Builds GCC with support for C and C++.
- `--disable-multilib`: Disables support for multiple library architectures (e.g., 32-bit and 64-bit).

Build GCC, use parallel jobs

```shell
make -j$(nproc)
```

Install GCC

```shell
sudo make install
```

This will install GCC to `/usr/local/gcc-14` or the directory specified in the `--prefix` option.

Update Alternatives (Optional)

Add GCC 14 to Alternatives

```shell
sudo update-alternatives --install /usr/bin/gcc gcc /usr/local/gcc-14/bin/gcc 60
sudo update-alternatives --install /usr/bin/g++ g++ /usr/local/gcc-14/bin/g++ 60
```

Select default version

```shell
sudo update-alternatives --config gcc
sudo update-alternatives --config g++
```
