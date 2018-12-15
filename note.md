
基本块标签

.BB{基本块bid}


字符串名称
str_{字符串id}

全局变量名称
global_{变量名称name}


编译流程
1. SON 生成
2. 生成 def-use 链
3. clean phi
4. merge linear region

5. SCCP optimization
    1. compute lattice for every node
    2. remove unreachable code
    3. clean phi
    4. merge linear region
    
6. GVN optimization (optional)

7. liveness analysis

8. find variable can be allocated on template register

9. spill

10. another liveness analysis (maybe need)

11. register allocation

12. stack slot allocation

13. code gen


寄存器分配上使用的尝试
1. 我们对临时寄存器进行显式分配, 所有在函数调用之前存活的值将不能分配到临时寄存器上
2. 不对栈槽进行预先分配,在spill时产生Spillslot Node, 将栈槽当作寄存器进行分配

节点语义
1. 算术节点,表示算术运算的结果
2. 比较节点,表示比较结果
3. Print节点,表示Print完成后的World
4. Call节点,产生tuple(ret, world, global...)
5. Read节点,产生tuple(ret, world)
6. Get节点,产生ret
7. Set节点,产生global
8. Region节点,表示控制流图中的一个基本块,产生一个control value
8. if节点,表示控制流图中的一个跳转,产生一个control value的tuple, (true branch, false branch)
8. ProjIf,取出tuple中的bool, field = 1表示为true branch, 0表示是false branch
8. ProjWorld节点,取出tuple中的world, field = -1
9. ProjRet节点,取出tuple中的ret, field = -2
10. GlobalProj节点,取出global, field = varID

tuple produce node
1. call 
2. read
3. if



节点设计

到底那些节点需要使用寄存器？

常数节点的类型
1. label(), 全局变量地址
2. int, 整数字面量
3. sp(), 栈地址

需要注意的是,不会出现两个地址相加的情况

首先
1. 算术节点需要使用寄存器.用来存放计算的结果

2. CmpN节点不需要使用寄存器.

3. 控制节点都不需要寄存器

4. 保存实际值的Phi节点需要寄存器

5. 常数节点不需要寄存器

6. 有返回值的Call需要寄存器

7. Print 不需要寄存器

8. Read 需要寄存器. 用来存放读取到的值

代码生成概述

1. 算术节点生成
    1. add, mul 由于 add 和 mul 的运算是交换的, 所以只需要保证两边都不是立即数就可以生成.
       如果是栈上地址加偏移量,可以使用la $d addr($sp)配合一条add, 如果是全局地址,我们可以使用
       la $d addr(label)配合一条add
       通过SCCP,我们可以完成这个任务, 需要注意的是要将立即数放到后面.
    
    2. sub, div 由于运算不是交换的,所以当出现左立即数的情况,需要额外处理, sub可以通过后加neg来实现.
       div 可能需要通过 li $at 来实现.
    
      
2. 单个基本块的所有Phi节点生成需要统一完成.
必要的时候可以使用swap

3. 控制节点唯一需要生成代码的是If Node, 需要完成.

4. Call的代码生成需要遵循调用规范, 完成所有参数的计算后,将前四个参数拷贝到a族寄存器上,剩余的参数写到栈上

5. Read的代码不需要额外设计, 直接进行系统调用,之后将v0的值拷贝到目标寄存器
6. Print的代码需要进行设计, 将a0的值先保存到v1, 完成后恢复a0

