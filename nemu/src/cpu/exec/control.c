#include "cpu/exec.h"

make_EHelper(jmp) {
  // the target address is calculated at the decode stage
  decoding.is_jmp = 1;

  print_asm("jmp %x", decoding.jmp_eip);
}

make_EHelper(jcc) {
  // the target address is calculated at the decode stage
  uint8_t subcode = decoding.opcode & 0xf;
  rtl_setcc(&t2, subcode);
  decoding.is_jmp = t2;
  print_asm("j%s %x", get_cc_name(subcode), decoding.jmp_eip);
}

make_EHelper(jmp_rm) {
  decoding.jmp_eip = id_dest->val;
  decoding.is_jmp = 1;

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  // the target address is calculated at the decode stage
  uint32_t temp;
  uint32_t *p;
  temp = (*eip);
  p = &temp;
  rtl_push(p);
  decoding.is_jmp = 1;
  decoding.jmp_eip = id_dest->val + (*eip);
  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
  uint32_t temp;
  uint32_t *p;
  p = &temp;
  rtl_pop(p);
  decoding.is_jmp = 1;
  decoding.jmp_eip = temp;
  print_asm("ret");
}

make_EHelper(call_rm) {
  t0 = (*eip);
  rtl_push(&t0);
  decoding.is_jmp = 1;
  decoding.jmp_eip = id_dest->val;
  print_asm("call *%s", id_dest->str);
}
