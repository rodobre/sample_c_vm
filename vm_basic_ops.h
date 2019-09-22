#ifndef _VM_BASIC_OPS_H
#define _VM_BASIC_OPS_H
#include "vm.h"

void init_operations(_vm* vm);

// MOV operations
void movi(void* __vm, vm_arg* args);
void mov(void* __vm, vm_arg* args);

// Stack operations
void pushi(void* __vm, vm_arg* args);
void push(void* __vm, vm_arg* args);
void pop(void* __vm, vm_arg* args);

// Logical operations
void andi(void* __vm, vm_arg* args);
void and(void* __vm, vm_arg* args);

void ori(void* __vm, vm_arg* args);
void or(void* __vm, vm_arg* args);

void xori(void* __vm, vm_arg* args);
void xor(void* __vm, vm_arg* args);

void not(void* __vm, vm_arg* args);

// Arithmetic operations
void addi(void* __vm, vm_arg* args);
void add(void* __vm, vm_arg* args);

void subi(void* __vm, vm_arg* args);
void sub(void* __vm, vm_arg* args);

void muli(void* __vm, vm_arg* args);
void mul(void* __vm, vm_arg* args);

void divi(void* __vm, vm_arg* args);
void div(void* __vm, vm_arg* args);

void modi(void* __vm, vm_arg* args);
void mod(void* __vm, vm_arg* args);

// Relational operator
void cmpi(void* __vm, vm_arg* args);
void cmp(void* __vm, vm_arg* args);

// Jump
void jmp(void* __vm, vm_arg* args);
void jz(void* __vm, vm_arg* args);
void jnz(void* __vm, vm_arg* args);
void je(void* __vm, vm_arg* args);
void jne(void* __vm, vm_arg* args);
void jl(void* __vm, vm_arg* args);
void jle(void* __vm, vm_arg* args);
void jg(void* __vm, vm_arg* args);
void jge(void* __vm, vm_arg* args);

// Function calls
void call(void* __vm, vm_arg* args);
void ret(void* __vm, vm_arg* args);
#endif // _VM_BASIC_OPS_H
