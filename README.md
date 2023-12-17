# hw75-tools
瀚文75助手   
支持平台: Windows, Linux, MacOS

# 环境配置

### Windows
> windows推荐使用MSYS2下的MINGW64

```shell
# 进入MSYS2的MINGW64环境
pacman -Syy mingw64/mingw-w64-x86_64-toolchain
pacman -Syy mingw64/mingw-w64-x86_64-protobuf
pacman -Syy mingw64/mingw-w64-x86_64-hidapi
# qt6的扩展看情况安装
pacman -S mingw64/mingw-w64-x86_64-qt6
# 或者可以使用我配置环境使用的命令
pacman -Syy mingw64/mingw-w64-x86_64-openssl mingw64/mingw-w64-x86_64-qt6 mingw64/mingw-w64-x86_64-qt6-tools mingw64/mingw-w64-x86_64-qt6-websockets mingw64/mingw-w64-x86_64-qt6-websockets-debug mingw64/mingw-w64-x86_64-qt6-doc mingw64/mingw-w64-x86_64-qt6-declarative mingw64/mingw-w64-x86_64-qt6-declarative-debug mingw64/mingw-w64-x86_64-qt6-base-debug mingw64/mingw-w64-x86_64-qt6-base mingw64/mingw-w64-x86_64-qt-creator mingw64/mingw-w64-x86_64-qt-creator-devel mingw64/mingw-w64-x86_64-editorconfig-qtcreator


```

### MacOS
> 你已经预先安装了Homebrew

```shell

brew install qt6
brew install qt-creator
brew install hidapi
brew install protobuf

```
> 当你安装完成这些库之后直接编译可能会出现找不到库的情况,这时候需要手动配置一下库的路径

```shell
# 编辑你当前的bash配置文件
vim ~/.zprofile

# 以下皆为brew安装对应库之后需要添加的路径,如果你的库安装路径不同,请自行修改(大部分时候版本号都不一样)
export C_INCLUDE_PATH=$C_INCLUDE_PATH:/usr/local/Cellar/hidapi/0.14.0/include
export C_INCLUDE_PATH=$C_INCLUDE_PATH:/usr/local/Cellar/protobuf/21.12/include
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/usr/local/Cellar/hidapi/0.14.0/include
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/usr/local/Cellar/protobuf/21.12/include
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/Cellar/hidapi/0.14.0/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/Cellar/protobuf/21.12/lib
export LIBRARY_PATH=$LIBRARY_PATH:/usr/local/Cellar/hidapi/0.14.0/lib
export LIBRARY_PATH=$LIBRARY_PATH:/usr/local/Cellar/protobuf/21.12/lib

```
