.pos 0x4
test:
    halt
    nop
    rrmovq %rax, %rcx
    cmovle %rdx, %rbx
    cmovl %rsp, %rbp 
    cmove %rsi, %rdi
    cmovne %r8, %r9
    cmovge %r10, %r11
    cmovg %r12, %r13
    irmovq $0x100, %r14
    mrmovq 0x8(%rax), %rax
    rmmovq %rax 0x16(%rax)
    addq %rax, %rax
    subq %rax, %rax
    xorq %rax, %rax
    andq %rax, %rax
    jmp test2
    jle test2
    jl test2
    je test2
    jne test2
    jge test2
    jg test2
    call test2
    ret 
    pushq %rcx
    popq %rcx
test2:
    .align 0x8
    .quad 0x8