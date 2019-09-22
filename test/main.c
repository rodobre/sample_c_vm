#include "../vm.h"
#include "../vm_basic_ops.h"
#include <assert.h>

int main()
{
    _vm* vm = init_vm();
    init_operations(vm);
    eval(vm, "\x3f\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x13\x37\x40\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x01"
         "\x19\x00\x00\x00\x00\x13\x33\x33\x37\x9d\x00\x00\x00\x00\x00\x00\x00\x03\x47\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x01"
         "\xfe\x00\x00\x00\x00\x00\x00\x00\x4e\x66\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x02", 95u);
    dump_state(vm, stdout);
    cleanup(vm);
}
