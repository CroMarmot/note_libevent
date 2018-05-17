# note_libevent

 ./configure --enable-debug-mode --enable-static --enable-thread-support --prefix=/home/hcl0589/libevent

gcc -g -o hw.o -I ~/libevent/include/ -L ~/libevent/lib/ hello-world.c -levent

## epoll 两种触发模式 水平触发和边缘触发
