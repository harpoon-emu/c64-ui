#ifndef CPU_INSTRUCTIONS_BCC_HH
#define CPU_INSTRUCTIONS_BCC_HH

#include "mos_6510_instruction.hh"

namespace commodore {
namespace cpu {
namespace instructions {

class bcc : public mos_6510_relative_branch_instruction<0x90, 3> {
public:
	void execute() {
		branch(!get_cpu()->get_registers().P.C());
	}

	void disassemble(std::ostream& stream) const {
		mos_disassemble_relative(stream, "BCC");
	}
};

}
}
}

#endif

