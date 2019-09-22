#ifndef _VM_H
#define _VM_H
#include <stdint.h>
#include <stdio.h>

#define ADMITTED_REGISTERS 7u

enum Registers
{
    RA = 1,
    RB,
    RC,
    RD,
    CT,
    BP,
    SP,
    IP,
    ZF,
    CF
};

typedef struct vm_registers
{
    uint64_t ra;
    uint64_t rb;
    uint64_t rc;
    uint64_t rd;
    uint64_t ct;
    uint64_t bp;
    uint64_t sp;
    uint64_t ip;
    uint64_t zf;
    uint64_t cf;
} vm_registers;

typedef struct vm_arg
{
    size_t arg_count;
    uint64_t* args;
} vm_arg;

typedef void(*vm_func_ptr)(void* vm, vm_arg* arg);

typedef struct vm_func
{
    size_t arg_count;
    vm_func_ptr func;
} vm_func;

typedef uint8_t vm_opcode;

typedef struct _vm
{
    void* _base_ptr;
    size_t _memory;
    vm_registers* regs;

    size_t opcode_count;
    vm_opcode* opcodes;
    vm_opcode* opcode_map;
    vm_func* func_table;
} _vm;

void* alloc_mem(size_t sz);
void cleanup(void* mem);

_vm* init_vm();
uint64_t* translate_register(_vm* vm, uint64_t register_value);
void register_opcode(_vm* vm, vm_opcode op, vm_func_ptr func, size_t args);
vm_opcode translate_opcode(_vm* vm, vm_opcode op);
void eval(_vm* vm, char* code, size_t size);
void dump_state(_vm* vm, FILE* stream);
#endif
