About how gdb works from this [blog](http://blog.csdn.net/edonlii/article/details/8717029).

* Use `ptrace` to send `SIGSTOP` to debugged process;
* Replace breakpoint code with `int 3` interuption (0xcc); restore code when continue;
