#ifndef __RTL_H__
#define __RTL_H__

#include "nemu.h"

extern rtlreg_t t0, t1, t2, t3;
extern const rtlreg_t tzero;

/* RTL basic instructions */

static inline void rtl_li(rtlreg_t* dest, uint32_t imm) {
  *dest = imm;
}

#define c_add(a, b) ((a) + (b))
#define c_sub(a, b) ((a) - (b))
#define c_and(a, b) ((a) & (b))
#define c_or(a, b)  ((a) | (b))
#define c_xor(a, b) ((a) ^ (b))
#define c_shl(a, b) ((a) << (b))
#define c_shr(a, b) ((a) >> (b))
#define c_sar(a, b) ((int32_t)(a) >> (b))
#define c_slt(a, b) ((int32_t)(a) < (int32_t)(b))
#define c_sltu(a, b) ((a) < (b))

#define make_rtl_arith_logic(name) \
  static inline void concat(rtl_, name) (rtlreg_t* dest, const rtlreg_t* src1, const rtlreg_t* src2) { \
    *dest = concat(c_, name) (*src1, *src2); \
  } \
  static inline void concat3(rtl_, name, i) (rtlreg_t* dest, const rtlreg_t* src1, int imm) { \
    *dest = concat(c_, name) (*src1, imm); \
  }


make_rtl_arith_logic(add)
make_rtl_arith_logic(sub)
make_rtl_arith_logic(and)
make_rtl_arith_logic(or)
make_rtl_arith_logic(xor)
make_rtl_arith_logic(shl)
make_rtl_arith_logic(shr)
make_rtl_arith_logic(sar)
make_rtl_arith_logic(slt)
make_rtl_arith_logic(sltu)

static inline void rtl_mul(rtlreg_t* dest_hi, rtlreg_t* dest_lo, const rtlreg_t* src1, const rtlreg_t* src2) {
  asm volatile("mul %3" : "=d"(*dest_hi), "=a"(*dest_lo) : "a"(*src1), "r"(*src2));
}

static inline void rtl_imul(rtlreg_t* dest_hi, rtlreg_t* dest_lo, const rtlreg_t* src1, const rtlreg_t* src2) {
  asm volatile("imul %3" : "=d"(*dest_hi), "=a"(*dest_lo) : "a"(*src1), "r"(*src2));
}

static inline void rtl_div(rtlreg_t* q, rtlreg_t* r, const rtlreg_t* src1_hi, const rtlreg_t* src1_lo, const rtlreg_t* src2) {
  asm volatile("div %4" : "=a"(*q), "=d"(*r) : "d"(*src1_hi), "a"(*src1_lo), "r"(*src2));
}

static inline void rtl_idiv(rtlreg_t* q, rtlreg_t* r, const rtlreg_t* src1_hi, const rtlreg_t* src1_lo, const rtlreg_t* src2) {
  asm volatile("idiv %4" : "=a"(*q), "=d"(*r) : "d"(*src1_hi), "a"(*src1_lo), "r"(*src2));
}
static inline void rtl_sext(rtlreg_t* dest, const rtlreg_t* src1, int width) {
   int len;
   len = 8*(4-width);
   int temp;
   temp = (*src1);
   temp = (temp<<len)>>len;
   (*dest) = temp;
}
static inline void rtl_zext(rtlreg_t* dest, const rtlreg_t* src1, int width) {
   int len;
   len = 8*(4-width);
   uint32_t temp;
   temp = (*src1);
   temp = (temp<<len)>>len;
   (*dest) = temp;
}
static inline void rtl_set_imul_OFCF(rtlreg_t* dest_hi, rtlreg_t* dest_lo, int width) {
  cpu.OF = cpu.CF = 1;
  uint32_t temp1,temp2;
  int temp;
  switch (width)
  {
     case 1: temp1 = (*dest_lo);
             temp2 = temp1;
	     rtl_sext(&temp1,&temp1,1);
	     rtl_sext(&temp2,&temp2,2);
	     if(temp1==temp2)
	     {
                cpu.OF = cpu.CF = 0;
	     }
	     break;
     case 2: temp1 = (*dest_lo);
	     temp2 = temp1;
	     rtl_sext(&temp1,&temp1,2);
	     if(temp1==temp2)
	     {
		cpu.OF = cpu.CF = 0;
	     }
	     break;
     case 4: temp1 = (*dest_lo);
	    temp2 = (*dest_hi);
	    temp = temp1;
	    temp = temp >> 31;
	    temp1 = temp;
	    if(temp1==temp2)
	    {
	        cpu.OF = cpu.CF = 0;
	    }
	    break;
     default: assert(0);
	      break;
  }
}
static inline void rtl_set_mul_OFCF(rtlreg_t* dest_hi, rtlreg_t* dest_lo, int width) {
  uint32_t temp1,temp2;
  uint32_t temp;
  cpu.OF = cpu.CF = 1;
  switch (width)
  {
      case 1: temp1 = (*dest_lo);
	      temp2 = temp1;
	      rtl_zext(&temp1,&temp1,1);
	      rtl_zext(&temp2,&temp2,2);
	      if(temp1==temp2)
	      {
                  cpu.OF = cpu.CF = 0;
	      }
	      break;
      case 2: temp1 = (*dest_lo);
	      temp2 = temp1;
	      rtl_zext(&temp1,&temp1,2);
	      if(temp1==temp2)
	      {
                 cpu.OF = cpu.CF = 0;
	      }
	      break;
     case 4: temp = (*dest_hi);
             if(temp==0)
	     {
                cpu.OF = cpu.CF = 0;
	     }
	     break;
    default: assert(0);
	      break; 
  }
}
static inline void rtl_lm(rtlreg_t *dest, const rtlreg_t* addr, int len) {
  *dest = vaddr_read(*addr, len);
}

static inline void rtl_sm(rtlreg_t* addr, int len, const rtlreg_t* src1) {
  vaddr_write(*addr, len, *src1);
}

static inline void rtl_lr_b(rtlreg_t* dest, int r) {
  *dest = reg_b(r);
}

static inline void rtl_lr_w(rtlreg_t* dest, int r) {
  *dest = reg_w(r);
}

static inline void rtl_lr_l(rtlreg_t* dest, int r) {
  *dest = reg_l(r);
}

static inline void rtl_sr_b(int r, const rtlreg_t* src1) {
  reg_b(r) = *src1;
}

static inline void rtl_sr_w(int r, const rtlreg_t* src1) {
  reg_w(r) = *src1;
}

static inline void rtl_sr_l(int r, const rtlreg_t* src1) {
  reg_l(r) = *src1;
}

/* RTL psuedo instructions */

static inline void rtl_lr(rtlreg_t* dest, int r, int width) {
  switch (width) {
    case 4: rtl_lr_l(dest, r); return;
    case 1: rtl_lr_b(dest, r); return;
    case 2: rtl_lr_w(dest, r); return;
    default: assert(0);
  }
}

static inline void rtl_sr(int r, int width, const rtlreg_t* src1) {
  switch (width) {
    case 4: rtl_sr_l(r, src1); return;
    case 1: rtl_sr_b(r, src1); return;
    case 2: rtl_sr_w(r, src1); return;
    default: assert(0);
  }
}

#define make_rtl_setget_eflags(f) \
  static inline void concat(rtl_set_, f) (const rtlreg_t* src) { \
    rtlreg_t temp;\
    temp = (*src);\
    temp = temp&0x00000001;\
   cpu.f = temp;	  \
  } \
  static inline void concat(rtl_get_, f) (rtlreg_t* dest) { \
    uint32_t temp;\
    temp = cpu.f;\
   (*dest) = temp ;	\
     \
  }

make_rtl_setget_eflags(CF)
make_rtl_setget_eflags(OF)
make_rtl_setget_eflags(ZF)
make_rtl_setget_eflags(SF)

static inline void rtl_mv(rtlreg_t* dest, const rtlreg_t *src1) {
  // dest <- src1
  (*dest) = (*src1);
}

static inline void rtl_not(rtlreg_t* dest) {
  // dest <- ~dest
  (*dest) = ~(*dest);
}

static inline void rtl_push(const rtlreg_t* src1) {
  // esp <- esp - 4
  // M[esp] <- src1
  uint32_t datat;
          cpu.esp = cpu.esp -4;
          datat  = (*src1);
          vaddr_write(cpu.esp,4,datat);
}
static inline void rtl_push_16(const rtlreg_t* src1) {
  uint32_t datat;
          cpu.esp = cpu.esp -2;
	  datat  = (*src1);
	  vaddr_write(cpu.esp,2,datat);
}
static inline void rtl_pop(rtlreg_t* dest) {
  // dest <- M[esp]
  // esp <- esp + 4
  uint32_t datat;
	  datat = vaddr_read(cpu.esp,4);
	  (*dest) = datat;
	  cpu.esp = cpu.esp +4;
}
static inline void rtl_pop_16(rtlreg_t* dest) {
  uint32_t datat;
         datat = vaddr_read(cpu.esp,2);
	 (*dest) = datat;
	 cpu.esp = cpu.esp +2;
         
}
static inline void rtl_eq0(rtlreg_t* dest, const rtlreg_t* src1) {
  // dest <- (src1 == 0 ? 1 : 0)
  (*dest) = ((*src1) == 0 ? 1 : 0);
}

static inline void rtl_eqi(rtlreg_t* dest, const rtlreg_t* src1, int imm) {
  // dest <- (src1 == imm ? 1 : 0)
  (*dest) = ((*src1) == imm ? 1 : 0);
}

static inline void rtl_neq0(rtlreg_t* dest, const rtlreg_t* src1) {
  // dest <- (src1 != 0 ? 1 : 0)
  (*dest) = ((*src1) != 0 ? 1 : 0);
}

static inline void rtl_msb(rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- src1[width * 8 - 1]
  uint32_t tempu;
  uint32_t *p;
  p = &tempu;
  rtl_sext(p,src1,width);
  tempu = tempu >> 31;
  (*dest) = tempu;
}

static inline void rtl_update_ZF(const rtlreg_t* result, int width) {
  // eflags.ZF <- is_zero(result[width * 8 - 1 .. 0])
  int temp;
  rtlreg_t temp2;
  temp2 = (*result);
  temp = temp2;
  temp = temp<<(8*(4-width ) );
  if(temp==0)
  {
	  cpu.ZF = 1;
  }
  else
  {
	  cpu.ZF = 0;
  }

}

static inline void rtl_update_SF(const rtlreg_t* result, int width) {
  // eflags.SF <- is_sign(result[width * 8 - 1 .. 0])
  int temp;
  rtlreg_t temp2;
  temp2 = (*result);
  temp = temp2;
  temp = temp<<(8*(4-width ) );
  temp = temp>>31;
  if(temp==0)
  {
      cpu.SF = 0;
  }
  else
  {
	   cpu.SF = 1;
  }

}

static inline void rtl_update_ZFSF(const rtlreg_t* result, int width) {
  rtl_update_ZF(result, width);
  rtl_update_SF(result, width);
}

#endif
