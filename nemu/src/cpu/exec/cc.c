#include "cpu/rtl.h"

/* Condition Code */

void rtl_setcc(rtlreg_t* dest, uint8_t subcode) {
  bool invert = subcode & 0x1;
  enum {
    CC_O, CC_NO, CC_B,  CC_NB,
    CC_E, CC_NE, CC_BE, CC_NBE,
    CC_S, CC_NS, CC_P,  CC_NP,
    CC_L, CC_NL, CC_LE, CC_NLE
  };
   (*dest) = 0; 
  // TODO: Query EFLAGS to determine whether the condition code is satisfied.
  // dest <- ( cc is satisfied ? 1 : 0)
  switch (subcode & 0xe) {
    case CC_O:if(cpu.OF==1)
	      {
		   (*dest) = 1;
	      }
	      break;
    case CC_B:if(cpu.CF==1)
	      {
		      (*dest) = 1;
	      }
              break;
    case CC_E:if(cpu.ZF==1)
	      {
		      (*dest) = 1;
	      }
	      break;
    case CC_BE:if(cpu.ZF==1||cpu.CF==1)
	       {
		       (*dest) = 1;
	       }
	       break;
    case CC_S:if(cpu.SF==1)
	      {
		      (*dest) = 1;
	      }
	      break;
    case CC_L:if(cpu.SF!=cpu.OF)
	      {
		      (*dest) = 1;
	      }
	      break;
    case CC_LE:if(cpu.SF!=cpu.OF||cpu.ZF==1)
	       {
		       (*dest) = 1;
	       }
	       break;
    default: panic("should not reach here");
    case CC_P: panic("n86 does not have PF");
  }

  if (invert) {
    rtl_xori(dest, dest, 0x1);
  }
}
