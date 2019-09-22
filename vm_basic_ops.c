#include "vm.h"
#include "vm_basic_ops.h"
#include <assert.h>

void init_operations(_vm* vm)
{
    register_opcode(vm, 0x3fu, movi, 2);
    register_opcode(vm, 0x40u, mov, 2);

    register_opcode(vm, 0x19u, pushi, 1);
    register_opcode(vm, 0x12u, push, 1);
    register_opcode(vm, 0x9du, pop, 1);

    register_opcode(vm, 0xdeu, andi, 2);
    register_opcode(vm, 0xdau, and, 2);
    register_opcode(vm, 0xb7u, ori, 2);
    register_opcode(vm, 0xb2u, or, 2);
    register_opcode(vm, 0xc1u, xori, 2);
    register_opcode(vm, 0xc9u, xor, 2);
    register_opcode(vm, 0x56u, not, 1);

    register_opcode(vm, 0x05u, addi, 2);
    register_opcode(vm, 0x0bu, add, 2);
    register_opcode(vm, 0x2fu, subi, 2);
    register_opcode(vm, 0x2cu, sub, 2);
    register_opcode(vm, 0x3du, muli, 2);
    register_opcode(vm, 0x39u, mul, 2);
    register_opcode(vm, 0x44u, divi, 2);
    register_opcode(vm, 0x47u, div, 2);
    register_opcode(vm, 0x66u, modi, 2);
    register_opcode(vm, 0x60u, mod, 2);

    register_opcode(vm, 0xe9u, cmpi, 2);
    register_opcode(vm, 0xeeu, cmp, 2);

    register_opcode(vm, 0xfe, jmp, 1);
    register_opcode(vm, 0xf0, jz, 1);
    register_opcode(vm, 0xf1, jnz, 1);
    register_opcode(vm, 0xf2, je, 1);
    register_opcode(vm, 0xf3, jne, 1);
    register_opcode(vm, 0xf4, jl, 1);
    register_opcode(vm, 0xf5, jle, 1);
    register_opcode(vm, 0xf6, jg, 1);
    register_opcode(vm, 0xf7, jge, 1);

    register_opcode(vm, 0x99u, call, 1);
}

void movi(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t reg = args->args[0];
    uint64_t im = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    *r1 = im;
}

void mov(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t  reg = args->args[0];
    uint64_t sreg = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    uint64_t* r2 = translate_register(vm, sreg);

    *r1 = *r2;
}

void pushi(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 1u);
    _vm* vm = __vm;
    uint64_t im = args->args[0];

    if(vm->regs->sp < 8)
        assert(0);

    if(vm->regs->sp > (uint64_t)((uint64_t)vm->_base_ptr + vm->_memory))
        assert(0);

    vm->regs->sp -= 8;
    *(uint64_t*)((uint64_t)vm->_base_ptr + vm->regs->sp) = im;
}

void push(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 1u);
    _vm* vm = __vm;
    uint64_t reg = args->args[0];

    uint64_t* r1 = translate_register(vm, reg);

    if(vm->regs->sp < 8)
        assert(0);

    if(vm->regs->sp > (uint64_t)((uint64_t)vm->_base_ptr + vm->_memory))
        assert(0);

    vm->regs->sp -= 8;
    *(uint64_t*)((uint64_t)vm->_base_ptr + vm->regs->sp) = *r1;
}

void pop(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 1u);
    _vm* vm = __vm;
    uint64_t reg = args->args[0];

    uint64_t* r1 = translate_register(vm, reg);

    if((vm->regs->sp + 8) > (uint64_t)((uint64_t)vm->_base_ptr + vm->_memory))
        assert(0);

    if(vm->regs->sp < 0)
        assert(0);

    *r1 = *(uint64_t*)((uint64_t)vm->_base_ptr + vm->regs->sp);
    vm->regs->sp += 8u;
}

void andi(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t reg = args->args[0];
    uint64_t im = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    *r1 &= im;
}

void and(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t  reg = args->args[0];
    uint64_t sreg = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    uint64_t* r2 = translate_register(vm, sreg);

    *r1 &= *r2;
}

void ori(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t reg = args->args[0];
    uint64_t im = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    *r1 |= im;
}

void or(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t  reg = args->args[0];
    uint64_t sreg = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    uint64_t* r2 = translate_register(vm, sreg);

    *r1 |= *r2;
}

void xori(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t reg = args->args[0];
    uint64_t im = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    *r1 ^= im;
}

void xor(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t  reg = args->args[0];
    uint64_t sreg = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    uint64_t* r2 = translate_register(vm, sreg);

    *r1 ^= *r2;
}

void not(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 1u);
    _vm* vm = __vm;
    uint64_t reg = args->args[0];

    uint64_t* r1 = translate_register(vm, reg);
    *r1 = ~(*r1);
}

void addi(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t reg = args->args[0];
    uint64_t im = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    *r1 += im;
}

void add(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t  reg = args->args[0];
    uint64_t sreg = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    uint64_t* r2 = translate_register(vm, sreg);

    *r1 += *r2;
}

void subi(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t reg = args->args[0];
    uint64_t im = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    *r1 -= im;
}

void sub(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t  reg = args->args[0];
    uint64_t sreg = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    uint64_t* r2 = translate_register(vm, sreg);

    *r1 -= *r2;
}

void muli(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t reg = args->args[0];
    uint64_t im = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    *r1 *= im;
}

void mul(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t  reg = args->args[0];
    uint64_t sreg = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    uint64_t* r2 = translate_register(vm, sreg);

    *r1 *= *r2;
}

void divi(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t reg = args->args[0];
    uint64_t im = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    *r1 /= im;
}

void div(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t  reg = args->args[0];
    uint64_t sreg = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    uint64_t* r2 = translate_register(vm, sreg);

    *r1 /= *r2;
}

void modi(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t reg = args->args[0];
    uint64_t im = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    *r1 %= im;
}

void mod(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t  reg = args->args[0];
    uint64_t sreg = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    uint64_t* r2 = translate_register(vm, sreg);

    *r1 %= *r2;
}

void cmpi(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t reg = args->args[0];
    uint64_t im = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    vm->regs->zf = *r1 - im;
}

void cmp(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 2u);
    _vm* vm = __vm;
    uint64_t  reg = args->args[0];
    uint64_t sreg = args->args[1];

    uint64_t* r1 = translate_register(vm, reg);
    uint64_t* r2 = translate_register(vm, sreg);

    vm->regs->zf = *r1 - *r2;
}

void jmp(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 1u);
    _vm* vm = __vm;
    uint64_t  addr = args->args[0] + (uint64_t)vm->_base_ptr;

    if((addr < (uint64_t)vm->_base_ptr) || (addr >= (uint64_t)((uint64_t)vm->_base_ptr + vm->_memory)))
        assert(0);

    vm->regs->ip = addr - (uint64_t)vm->_base_ptr;
}

void jz(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 1u);
    _vm* vm = __vm;
    uint64_t  addr = args->args[0] + (uint64_t)vm->_base_ptr;

    if((addr < (uint64_t)vm->_base_ptr) || (addr >= (uint64_t)((uint64_t)vm->_base_ptr + vm->_memory)))
        assert(0);

    if(vm->regs->zf == 0u)
        vm->regs->ip = addr - (uint64_t)vm->_base_ptr;
}

void jnz(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 1u);
    _vm* vm = __vm;
    uint64_t  addr = args->args[0] + (uint64_t)vm->_base_ptr;

    if((addr < (uint64_t)vm->_base_ptr) || (addr >= (uint64_t)((uint64_t)vm->_base_ptr + vm->_memory)))
        assert(0);

    if(vm->regs->zf != 0u)
        vm->regs->ip = addr - (uint64_t)vm->_base_ptr;
}

void je(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 1u);
    _vm* vm = __vm;
    uint64_t  addr = args->args[0] + (uint64_t)vm->_base_ptr;

    if((addr < (uint64_t)vm->_base_ptr) || (addr >= (uint64_t)((uint64_t)vm->_base_ptr + vm->_memory)))
        assert(0);

    if(vm->regs->zf == 0u)
        vm->regs->ip = addr - (uint64_t)vm->_base_ptr;
}

void jne(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 1u);
    _vm* vm = __vm;
    uint64_t  addr = args->args[0] + (uint64_t)vm->_base_ptr;

    if((addr < (uint64_t)vm->_base_ptr) || (addr >= (uint64_t)((uint64_t)vm->_base_ptr + vm->_memory)))
        assert(0);

    if(vm->regs->zf != 0u)
        vm->regs->ip = addr - (uint64_t)vm->_base_ptr;
}

void jl(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 1u);
    _vm* vm = __vm;
    uint64_t  addr = args->args[0] + (uint64_t)vm->_base_ptr;

    if((addr < (uint64_t)vm->_base_ptr) || (addr >= (uint64_t)((uint64_t)vm->_base_ptr + vm->_memory)))
        assert(0);

    if((int64_t)vm->regs->zf < 0)
        vm->regs->ip = addr - (uint64_t)vm->_base_ptr;
}

void jle(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 1u);
    _vm* vm = __vm;
    uint64_t  addr = args->args[0] + (uint64_t)vm->_base_ptr;

    if((addr < (uint64_t)vm->_base_ptr) || (addr >= (uint64_t)((uint64_t)vm->_base_ptr + vm->_memory)))
        assert(0);

    if((int64_t)vm->regs->zf <= 0)
        vm->regs->ip = addr - (uint64_t)vm->_base_ptr;
}

void jg(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 1u);
    _vm* vm = __vm;
    uint64_t  addr = args->args[0] + (uint64_t)vm->_base_ptr;

    if((addr < (uint64_t)vm->_base_ptr) || (addr >= (uint64_t)((uint64_t)vm->_base_ptr + vm->_memory)))
        assert(0);

    if((int64_t)vm->regs->zf > 0)
        vm->regs->ip = addr - (uint64_t)vm->_base_ptr;
}

void jge(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 1u);
    _vm* vm = __vm;
    uint64_t  addr = args->args[0] + (uint64_t)vm->_base_ptr;

    if((addr < (uint64_t)vm->_base_ptr) || (addr >= (uint64_t)((uint64_t)vm->_base_ptr + vm->_memory)))
        assert(0);

    if((int64_t)vm->regs->zf >= 0)
        vm->regs->ip = addr - (uint64_t)vm->_base_ptr;
}

void call(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 1u);
    _vm* vm = __vm;
    uint64_t  addr = args->args[0] + (uint64_t)vm->_base_ptr;

    if((addr < (uint64_t)vm->_base_ptr) || (addr >= (uint64_t)((uint64_t)vm->_base_ptr + vm->_memory)))
        assert(0);

    if(vm->regs->sp < 8)
        assert(0);

    if(vm->regs->sp > (uint64_t)(vm->_base_ptr + vm->_memory))
        assert(0);

    if((vm->regs->ip + 9u) > (uint64_t)(vm->regs + sizeof(vm_registers)))
        assert(0);

    if(vm->regs->ip < (uint64_t)vm->regs)
        assert(0);

    vm->regs->sp -= 8u;
    *(uint64_t*)(vm->_base_ptr + vm->regs->sp) = vm->regs->ip + 9u;
    vm->regs->ip = addr - (uint64_t)vm->_base_ptr;
}

void ret(void* __vm, vm_arg* args)
{
    assert(args->arg_count == 1u);
    _vm* vm = __vm;

    if((vm->regs->sp + 8u) > (uint64_t)(vm->_base_ptr + vm->_memory))
        assert(0);

    uint64_t addr = *(uint64_t*)(vm->_base_ptr + vm->regs->sp);
    vm->regs->sp += 8u;
    vm->regs->ip = addr - (uint64_t)vm->_base_ptr;
}
