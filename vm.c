#include "vm.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define DEFAULT_VM_SIZE         8192u
#define DEFAULT_OPCODE_SIZE     256u
#define LITTLE_ENDIAN

#define __bswap_constant_64(x)			    \
  ((((x) & 0xff00000000000000ull) >> 56)	\
   | (((x) & 0x00ff000000000000ull) >> 40)	\
   | (((x) & 0x0000ff0000000000ull) >> 24)	\
   | (((x) & 0x000000ff00000000ull) >> 8)	\
   | (((x) & 0x00000000ff000000ull) << 8)	\
   | (((x) & 0x0000000000ff0000ull) << 24)	\
   | (((x) & 0x000000000000ff00ull) << 40)	\
   | (((x) & 0x00000000000000ffull) << 56))

void* alloc_mem(size_t sz)
{
    void* mem = malloc(sz);
    assert(mem != NULL);
    return mem;
}

void cleanup(void* mem)
{
    assert(mem != NULL);
    return free(mem);
}

_vm* init_vm()
{
    // Default VM size = 8192 bytes
    // Default supported opcodes = 256

    _vm* _vm_mem = (_vm*)alloc_mem(DEFAULT_VM_SIZE);

    _vm_mem->_base_ptr = (void*)((char*)_vm_mem + sizeof(_vm));
    _vm_mem->_memory = DEFAULT_VM_SIZE;
    _vm_mem->regs = (vm_registers*)alloc_mem(sizeof(vm_registers));

    _vm_mem->regs->ra = 0u;
    _vm_mem->regs->rb = 0u;
    _vm_mem->regs->rc = 0u;
    _vm_mem->regs->rd = 0u;
    _vm_mem->regs->ct = 0u;
    _vm_mem->regs->bp = _vm_mem->_memory - sizeof(_vm);
    _vm_mem->regs->sp = _vm_mem->regs->bp;
    _vm_mem->regs->ip = 0x0u;
    _vm_mem->regs->zf = 0x0u;
    _vm_mem->regs->cf = 0x0u;

    _vm_mem->opcode_count = 0u;
    _vm_mem->opcodes = (vm_opcode*)alloc_mem(sizeof(vm_opcode) * DEFAULT_OPCODE_SIZE);
    _vm_mem->opcode_map = (vm_opcode*)alloc_mem(sizeof(vm_opcode) * DEFAULT_OPCODE_SIZE);
    _vm_mem->func_table = (vm_func*)alloc_mem(sizeof(vm_func) * DEFAULT_OPCODE_SIZE);

    memset(_vm_mem->opcodes, 0, sizeof(vm_opcode) * DEFAULT_OPCODE_SIZE);
    memset(_vm_mem->opcode_map, 0, sizeof(vm_opcode) * DEFAULT_OPCODE_SIZE);
    memset(_vm_mem->func_table, 0, sizeof(vm_func) * DEFAULT_OPCODE_SIZE);

    return _vm_mem;
}

uint64_t* translate_register(_vm* vm, uint64_t register_value)
{
    assert(register_value < ADMITTED_REGISTERS);
    switch(register_value)
    {
    case RA:
        return &vm->regs->ra;
    case RB:
        return &vm->regs->rb;
    case RC:
        return &vm->regs->rc;
    case RD:
        return &vm->regs->rd;
    case CT:
        return &vm->regs->ct;
    case BP:
        return &vm->regs->bp;
    case SP:
        return &vm->regs->sp;
    default:
        assert(0);
        return NULL;
    }
}

void register_opcode(_vm* vm, vm_opcode op, vm_func_ptr func, size_t args)
{
    assert(vm != NULL);
    assert(op != 0);

    vm->opcodes[++vm->opcode_count] = op;
    vm->opcode_map[op] = vm->opcode_count;

    vm_func tmp;
    tmp.arg_count = args;
    tmp.func = func;

    vm->func_table[vm->opcode_count] = tmp;
}

vm_opcode translate_opcode(_vm* vm, vm_opcode op)
{
    vm_opcode map_opcode = vm->opcode_map[op];
    assert(map_opcode != 0);
    return map_opcode;
}

void eval(_vm* vm, char* shellcode, size_t size)
{
    char* pt = shellcode;
    size_t i = 0;

    size_t last_exec_ip = vm->regs->ip;
    int8_t execution_branched = 0;

    while(i < size)
    {
        if(execution_branched)
        {
            pt = shellcode + vm->regs->ip;
        }
        execution_branched = 0;

        vm_opcode opcode = translate_opcode(vm, (vm_opcode)*pt);
        size_t opcode_args = vm->func_table[opcode].arg_count;
        vm_func_ptr func = vm->func_table[opcode].func;

        assert((i + opcode_args * sizeof(uint64_t)) < size);

        ++ pt;
        ++ i;

        vm_arg* args = (vm_arg*)alloc_mem(sizeof(vm_arg));
        args->arg_count = opcode_args;
        uint64_t* arg_ptrs = (uint64_t*)alloc_mem(opcode_args * sizeof(uint64_t));
        args->args = arg_ptrs;

        #ifndef LITTLE_ENDIAN
        memcpy(arg_ptrs, pt, opcode_args * sizeof(uint64_t));
        #else
        for(size_t arg_ct = 0; arg_ct < opcode_args; ++arg_ct)
        {
            char* tmp_pt = pt + (arg_ct * sizeof(uint64_t));
            uint64_t arg = *(uint64_t*)tmp_pt;
            arg_ptrs[arg_ct] = __bswap_constant_64(arg);
        }
        #endif // LITTLE_ENDIAN

        pt += opcode_args * sizeof(uint64_t);
        i += opcode_args * sizeof(uint64_t);

        func(vm, args);
        if(vm->regs->ip == last_exec_ip)
            vm->regs->ip += opcode_args * sizeof(uint64_t);
        else
        {
            execution_branched = 1;
            i = vm->regs->ip;
        }

        cleanup(arg_ptrs);
        cleanup(args);

        last_exec_ip = vm->regs->ip;
    }
}

void dump_state(_vm* vm, FILE* stream)
{
    fprintf(stream,
        "RA: %I64X\n"
        "RB: %I64X\n"
        "RC: %I64X\n"
        "RD: %I64X\n"
        "CT: %I64X\n"
        "BP: %I64X\n"
        "SP: %I64X\n"
        "IP: %I64X\n"
        "ZF: %I64X\n"
        "CF: %I64X\n",
    vm->regs->ra, vm->regs->rb, vm->regs->rc, vm->regs->rd,
    vm->regs->ct, vm->regs->bp, vm->regs->sp, vm->regs->ip,
    vm->regs->zf, vm->regs->cf);
}
