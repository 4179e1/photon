
# CPU 体系结构  （X86-64)

## CPU 寄存器

- 通用寄存器如rax
- 特殊寄存器如rip,rsp
- 标志位

## 使用惯例

![](./img/registers.png)

- rax：返回值
- rsp：栈指针
- rbp：基址指针（非必须）
  
- 调用者保存：
  - r10
  - r11
- 被调者保存：
  - rbx
  - ebp
  - r12
  - r13
  - r14
  - r16

> 假如函数f调用函数g，那么函数f需要先把r10和r11的值压栈保存起来（如果它用到这个两个寄存器的话），因为函数g会直接覆盖里面的内容（如果它需要的话）。

参数传递：
- rdi 第1个参数
- rsi 第2个参数
- rdx 第3个参数
- rcx 第4个参数
- r8  第5个参数
- r9  第6个参数
- TODO： 其他参数


## 操作数

| 类型   | 形式                                 | 操作数的值                                       |
| ------ | ------------------------------------ | ------------------------------------------------ |
| 立即数 | $Imm                                 | Imm                                              |
| 寄存器 | r<sub>a</sub>                        | R[ r<sub>a</sub> ]                               |
| 内存   | Imm(r<sub>b</sub>, r<sub>i</sub>, s) | M[ Imm + R[r<sub>b</sub>] + R[r<sub>i</sub>]*s ] |

> s必须是1，2，4或者8

## 数据传送指令

| 指令             | 效果   | 描述 |
| ---------------- | ------ | ---- |
| `MOV S,D`        | D <- S |      |
| &emsp;`movb S,D` |        |      |
| &emsp;`movw S,D` |        |      |
| &emsp;`movl S,D` |        |      |
| &emsp;`movq S,D` |        |      |
| `movabsq I, R`   | R <- I |      |

x86-64不允许两个操作数都是内存位置，因此把一个内存的值传送到另一个内存的值需要两条指令。

大多数mov指令只会更新它对应长度位置的寄存器或者内存，唯一的例外是movl当它的目标是一个寄存器时，它同时会把高32位设置位0。

5种可能的组合：

| 指令                   | 效果               |
| ---------------------- | ------------------ |
| `movl $0x4050,%eax`    | Immediae--Register |
| `movw %bp,%sp`         | Register--Register |
| `movb (%rdi,%rcx),%al` | Memory--Register   |
| `movb $-17,(%esp)`     | Immediate--Memory  |
| `movq %rax,-12(%rbp)`  | Register--Memory   |

## 栈

| 指令      | 效果                    |
| --------- | ----------------------- |
| `pushq S` | R[%rsp] <- R[%rsp] - 8; |
|           | M[R[%rsp]] <- S         |
| `popq D`  | D <- M[R[%rsp]];        |
|           | R[%rsp] <- $[%rsp] + 8  |

## 算术和逻辑运算

| 指令        | 效果                    | 说明            |
| ----------- | ----------------------- | --------------- |
| `leaq S, D` | D <- &S                 | 加载有效地址    |
|             |                         |                 |
| `INC D`     | D <- D + 1              |                 |
| `DEC D`     | D <- D - 1              |                 |
| `NEG D`     | D <- -D                 | 取反            |
| `NOT D`     | D <- ~D                 | 取补            |
|             |                         |                 |
| `ADD S, D`  | D <- D + S              |                 |
| `SUB S, D`  | D <- D - S              |                 |
| `IMUL S, D` | D <- D * S              |                 |
| `XOR S,D`   | D <- D ^ S              | 异或            |
| `OR S,D`    | D <- D \| S             | 逻辑或          |
| `AND S,D `  | D <- D & S              | 逻辑与          |
|             |                         |                 |
| `SAL k, D`  | D <- D << k             | 左移            |
| `SHL k, D ` | D <- D << k             | 左移，跟SAL相同 |
| `SAR k, D`  | D <- D >> <sub>A</sub>k | 算术右移        |
| `SHR k, D`  | D <- D >> <sub>L</sub>k | 逻辑右移        |

### Load Effective Address

`mov`和`lea`的区别:
- `mov (%rax,%rdx,4), %rbx`计算出源操作数的地址，从这个地址取值赋给`%rbx` ，类似`i = a[4]`
- `lea  (%rax,%rdx,4), %rbx`计算出源操作数的地址，直接把这个地址赋给`%rbx`，类似`a = &a[4]`

编译器常常会用`lea`来做算术运算，比如`%rdx`的值为`x`，`leaq 7(%rdx,%rdx,4),%rax`会把`%rax`的值设置为`5x + 7`

## 算术右移和逻辑右移动

- 算术右移会扩展符号位
- 逻辑右移会补0

## Control

### Condition Codes

- CF: `Carry flag`. The most recent operation generated a carry out of the most significant bit. Used to detect overflow for unsigned operations.
- ZF: `Zero flag`. The most recent operation yielded zero.
- SF: `Sign flag`. The most recent operation yielded a negative value.
- OF: `Overflow flag`. The most recent operation caused a two’s-complement overflow—either negative or positive.

 假如在C中进行整数计算`t = a + b`，那么处理器会这样设置标志位

|     |                                      |              |
| --- | ------------------------------------ | ------------ |
| CF  | (unsigned) t < (unsigned) a          | 无符号数溢出 |
| ZF  | (t == 0)                             | 结果为0      |
| SF  | (t < 0)                              | 结果为负数   |
| OF  | (a < 0 == b < 0) && (t < 0 != a < 0) | 有符号数溢出 |


| 指令                              | 基于                          | 描述                 |
| --------------------------------- | ----------------------------- | -------------------- |
| CMP  S<sub>1</sub>, S<sub>2</sub> | S<sub>2</sub> - S<sub>1</sub> | 比较                 |
| &emsp;cmpb                        |                               |                      |
| &emsp;cmpw                        |                               |                      |
| &emsp;cmpl                        |                               |                      |
| &emsp;cmpq                        |                               |                      |
| CMP  S<sub>1</sub>, S<sub>2</sub> | S<sub>1</sub> & S<sub>2</sub> | 测试 （logical AND） |
| &emsp;testb                       |                               |                      |
| &emsp;testw                       |                               |                      |
| &emsp;testl                       |                               |                      |
| &emsp;testq                       |                               |                      |

### 读取状态位

1. we can set a single byte to 0 or 1 depending on some combination of the condition codes
2.  we can conditionally jump to some other part of the program, or 
3.  we can conditionally transfer data. 

对于第1种情况，可以使用以下指令

| 指令    | 同义词 | 效果             | 设置的条件                   |
| ------- | ------ | ---------------- | ---------------------------- |
| sete D  | setz   | D ← ZF           | Equal / zero                 |
| setne D | setnz  | D ← ~ZF          | Not equal / not zero         |
| sets D  |        | D ← SF           | Negative                     |
| setns D |        | D ← ~SF          | Nonnegative                  |
| setg D  | setnle | D ← ~(SF^OF)&~ZF | Greater (signed >)           |
| setge D | setnl  | D ← ~(SF^OF)     | Greater or equal (signed >=) |
| setl D  | setnge | D ← SF^OF        | Less (signed <)              |
| setle D | setng  | D ← (SF^OF)      | ZF                           | Less or equal (signed <=)    |
| seta D  | setnbe | D ← ~CF&~ZF      | Above (unsigned >)           |
| setae D | setnb  | D ← ~CF          | Above or equal (unsigned >=) |
| setb    | setnae | D ← CF           | Below (unsigned <)           |
| setbe D | setna  | D ← CF           | ZF                           | Below or equal (unsigned <=) |
