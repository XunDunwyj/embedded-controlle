# embedded-controlle
ubuntu下进行GPIO读写EC
使用C++进行编译
例如:C++ io.c -o io
执行:
./io -r 0x62
-r为读取,0x62是操作的地址.
./io -w 0x62 0x40
-w为写入,062是操作的地址,0x40是操作的命令.
