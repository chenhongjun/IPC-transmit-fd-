# IPC-transmit-fd-
相当于跨进程dup()
众所周知：
1：进程可以在内部复制文件描述符。
2：父进程可以将打开的文件描述符复制给子进程。

但是，该栗子使用UNIX域协议在无亲缘关系的进程间复制打开的文件描述符。
3：跨进程传输文件描述符（万能）。

sendfd and writefd(UNIX net)
通过sendmsg, recvmsg函数使用UNIX域协议在进程间传输打开的文件描述符。666
