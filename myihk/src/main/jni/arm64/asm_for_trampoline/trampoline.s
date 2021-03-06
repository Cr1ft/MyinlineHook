.section ".my_sec", "awx"

.global _myopen_start

.text

_myopen_start:
    ldr     x0, [sp, #-0x8];            //恢复x0
    b       my_open;                    //未改变任何寄存器，执行hook函数。

.end

//用于手动生成蹦床代码，因为hook代码和这个蹦床一起编译的，所以基本上不会超出加减64m，那么就可以使用b跳转
//到偏移，就不用占用一个寄存器了。需要每增加一个hook函数，就加一个蹦床，相应的生成shellcode中跳转到hook
//函数的地方都要改成这个蹦床的地址。难就难在这个不好通过内嵌汇编实现，因为b跟的是个偏移值，在汇编中可以使用
//函数名称，编译器替换，但是内嵌汇编中不行。而自己计算