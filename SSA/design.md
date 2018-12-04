# SSA 总体设计

# Node design

Node represent basic element of SSA graph

Node is divided to two part, control node and value node

control node represent control flow of program

value node represent value of program


# control node design

most common control node is region node, which represent
a basic block of program

second is the start node, represent start of program generate initial
control.

third is the end node, represent end of program, end of all control flow
and is user of all return value

if node use a control edge and a value edge as predict

ifproj node use a if node to project true and false control of if


# value node design

all value belong to a basic block, so, every value node use a region node.

some value node represent actual value, a register can be allocate to these value.
but some value don't need a register, such as a const int.

other value node represent virtual value, so these value don't need a register.


## actual value

1. const int value don't need a register
2. arith value, need a register. such as AddV, SubV, MulV, DivV
    register represent register of result value
    
3. Sp value, don't need register, represent value of **$sp**

4. StackSlot value, don't need register, represent a pointer point to stack, 

5. GlobalAddr value, don't need register, represent a pointer point a stack.

5. call value, may be need a register for result, represent register for result value

6. Get value, need a register to store value get from memory

    + as a user, Get value use to two other value
        1. pointer, pointer used to dereference, 
           can be a StackSlot, GlobalAddr or calculated address
           
        2. virtual value of array or global value
    + semantic, Get value represent a pointer dereference
    + code generation, generate **lw** or **lb** instruction,
        + if pointer is StackSlot, generate **lw/b $reg offset($sp)**
        + if pointer is GlobalSlot, generate **lw/b $reg label**
        + if pointer is calculated, generate **lw/b $reg $pointer**
     

7. Set value, don't need a register

    + as a user Set value use three other value
        1. pointer, pointer used to dereference
           can be a StackSlot, GlobalAddr or calculated address
        2. value used to set, const scalar or calculated scalar
    
    + semantic, set value of global variable or array
    + code generation, generate **sw**, **sb** or extended **si**
        1. if value is const scalar, generate **si const $pointer**
        
        2. if value is not const, has a register, generate **sw** or **sb**
       

## virtual value
1. world value, use to keep order of side effect such as read and write.
2. array value, use to represent value of array, to keep dependency of array.
3. global value, use to represent value of global scalar and array, to keep dependency.


# extension to instruction system
add new pseudo instruction **si**

store a immediate to memory
format 
si 1000, $reg
si 1000, 100($reg)
si 1000, label
si 1000, 100 + label









