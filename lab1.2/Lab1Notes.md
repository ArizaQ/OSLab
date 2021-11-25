# Lab1Notes

## Note

### NASM

- Section
  - text
  - bss
  - data
- System Call
  - exit
  - read
  - write
  - 预处理
- 函数

### nasm常用指令

- 算数与逻辑
  - mov
  - movzx
  - add
  - sub
  - mul
  - div

- 条件与分支
  - jmp
  - cmp
- 位运算
  - and
  - not
  - test
- 栈操作
- 

## 问题清单

1. 请简述 80x86发展历史

   8086： 16bit

   80286

   80386: 32bit

   80486

   80585: 奔腾

2. 说明小端和大端的区别，并说明 80x86系列采用了哪种方式？

   ![image-20211020190721985](D:\pictureBed\image-20211020190721985.png)

   小端

3. 8086 有哪五类寄存器，请分别举例说明其作用？

   ![image-20211004185503074](D:\pictureBed\image-20211004185503074.png)

   - 数据寄存器

     AH&AL=AX(accumulator)：累加寄存器，常用于运算;在乘除等指令中指定用来存放操作数,另外,所有的I/O指令都使用这一寄存器与外界设备传送数据.
     BH&BL=BX(base)：基址寄存器，常用于地址索引；
     CH&CL=CX(count)：计数寄存器，常用于计数；常用于保存计算值,如在[移位指令](https://www.cnblogs.com/view/1365206.htm),循环(loop)和串处理指令中用作隐含的计数器.
     DH&DL=DX(data)：数据寄存器，常用于数据传递。

     他们的特点是,这4个16位的寄存器可以分为高8位: AH, BH, CH, DH.以及低八位：AL,BL,CL,DL。这2组8位寄存器可以分别寻址，并单独使用。

   - 控制寄存器

     IP指向的是指令地址的段内地址[偏移量](https://www.cnblogs.com/view/1254177.htm)，又称偏移地址(Offset Address)或有效地址(EA，Effective Address)。

   - 标志寄存器FLAG

     8086有一个16位的标志性寄存器FR，在FR中有意义的有9位，其中6位是状态位，3位是控制位。

     **OF溢出状态标志位：**

     OF用于反映有[符号](https://www.cnblogs.com/view/115742.htm)数加减运算所得[结果](https://www.cnblogs.com/view/275137.htm)是否溢出。如果运算结果超过当前运算位数所能表示的范围，则称为溢出，OF的值被置为1，否则，OF的值被清为0。

     **DF方向控制标志位：**

     DF位用来决定在串操作指令执行时有关指针寄存器发生调整的方向。

     **IF中断允许控制标志位：**

     IF位用来决定CPU是否响应CPU外部的可屏蔽中断发出的中断请求。但不管该标志为何值，CPU都必须响应CPU外部的不可屏蔽中断所发出的中断请求，以及CPU内部产生的中断请求。具体规定如下：

     (1) 当IF=1时，CPU可以响应CPU外部的可屏蔽中断发出的中断请求；

     (2) 当IF=0时，CPU不响应CPU外部的可屏蔽中断发出的中断请求。

     **TF跟踪控制标志位：**

     TF该标志可用于程序调试。TF标志没有专门的指令来设置或清除。

     (1) 如果TF=1，则CPU处于单步执行指令的工作方式，此时每执行完一条指令，就显示CPU内各个寄存器的当前值及CPU将要执行的下一条指令。

     (2) 如果TF=0，则处于连续工作模式。

     **SF符号状态标志位：**

     SF用来反映运算结果的符号位，它与运算结果的最高位相同。在[微机系统](https://www.cnblogs.com/view/2955084.htm)中，有符号数采用补码表示法，所以，SF也就反映运算结果的正负号。运算结果为正数时，SF的值为0，否则其值为1。当运算结果没有产生溢出时，运算结果等于逻辑结果（即应该得到的正确的结果），此时SF表示的是逻辑结果的正负，当运算结果产生溢出时，运算结果不等于逻辑结果，此时的SF值所表示的正负情况与逻辑结果相反，即：SF=0时，逻辑结果为负，SF=1时，逻辑结果为正。

     **ZF零状态标志位：**

     ZF用来反映运算结果是否为0。如果运算结果为0，则其值为1，否则其值为0。在判断运算结果是否为0时，可使用此标志位。

     **AF辅助进位状态标志位：**

     下列情况下，辅助进位标志AF的值被置为1，否则其值为0：

     (1)在字操作时，发生低字节向高字节进位或借位时；

     (2)在字节操作时，发生低4位向高4位进位或借位时。

     **PF奇偶状态标志：**

     PF用于反映运算结果中“1”的个数的奇偶性。如果“1”的个数为偶数，则PF的值为1，否则其值为0。

     **CF进位状态标志：**

     CF主要用来反映无符号数运算是否产生进位或借位。如果运算结果的最高位产生了一个进位或借位，那么，其值为1，否则其值为0。

4. 什么是寻址？立即寻址和直接寻址的区别是什么？

   寻址：找到操作数的地址(从⽽能够取出操作数)

   立即

   直接

   寄存器

   寄存器间接: 寄存器保存地址 MOV AX [BX]

   寄存器相对：寄存器中的地址加一个偏移量。MOV AX [SI+3]

   基址加变址：基址寄存器(BX、BP) + 变址寄存器(SI、DI) MOV AX [BX+DI]

   相对基址加变址：MOV AX [BX+DI+3]

5. 请举例说明寄存器间接寻址、寄存器相对寻址、基址加变址寻址、相对基址加变址寻址四种方式的 区别

   

6. . 请分别简述 MOV 指令和 LEA 指令的用法和作用？

   - mov 目的操作数 源操作数

     对寄存器值或变量值操作

     ```c
     mov eax ,userMsg ; 加载userMsg地址
     mov eax ,[userMsg] ; 加载userMsg内容
     ```

   - lea 目的操作数 源操作数

     取操作数偏移地址

     `lea eax ,userMsg` 非法

       `lea eax ,[*userMsg*]` 加载userMsg地址

7. 请说出主程序与子程序之间至少三种参数传递方式

   - 寄存器
   - 约定地址
   - 堆栈

8. 如何处理输入和输出，代码中哪里体现出来？ 

   中断系统调用

9. . 有哪些段寄存器 

   - CS
   - DS
   - SS
   - ES :附加数据段用来存放字符串操作时的目的字符串。

10. 通过什么寄存器保存前一次的运算结果，在代码中哪里体现出来。

    通用寄存器的数据寄存器

11. 解释 boot.asm 文件中， org 0700h 的作用 

    org伪指令，只对汇编器使用。

    BIOS将软盘内容放在07c00h位置。告诉编译器程序加载到这个位置，即告诉编译器从相对地址07c00h处开始编译第一条指令，用于之后的寻址。

12. boot.bin 应该放在软盘的哪一个扇区？为什么？

    第一个扇区。

    开机，从ROM运⾏BIOS程序，BIOS是⼚家写好的。BIOS程序检查软盘0⾯0磁道1扇区，如果扇区以0xaa55结束， 则认定为引导扇区，将其512字节的数据加载到内存的07c00处， 然后设置PC，跳到内存07c00处开始执⾏代码。以上的0xaa55以及07c00都是⼀种约定，BIOS程序就是这样做 的，所以我们就需要把我们的OS放在软盘的第⼀个扇区，填充， 并在最末尾写⼊0xaa55。

13. loader 的作用有哪些？ 

    引导扇区 只负责把loader加载⼊内存并把控制权交给他。

    loader将内核加载至内存：

    - 跳入保护模式。
    - 启动内存分页。
    - 从kernel.bin读取内核并放入内存。跳转到内核首地址开始执行。

14. 解释 NASM 语言中 [ ] 的作用

    访问变量，解引用。

15. 解释语句 `times 510-($-$$) db 0` ，为什么是 510? $ 和 $$ 分别表示什么？ 

    $: 当前航汇编后地址

    $$: 一个section开始处被汇编后的地址。

    将0字节填充510-($-$$)次。

    加上结束标志0xAA55正好512byte。

16. 解释配置文件 bochsrc 文件中如下参数的含义

    配置⽂件保存为bochsrc，和a.img以及boot.bin放在同⼀⽬录下

    • display_library: bochs使⽤的GUI库，在Ubuntu下⾯是sdl 

    • megs：虚拟机内存⼤⼩ 

    • floppya：虚拟机外设，使用这个盘，软盘为a.img⽂件 

    • boot：虚拟机启动⽅式，从软盘启动