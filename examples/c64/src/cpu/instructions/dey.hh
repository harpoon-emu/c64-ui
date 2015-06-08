#ifndef CPU_INSTRUCTIONS_DEY_HH
#define CPU_INSTRUCTIONS_DEY_HH

#include "mos_6510_instruction.hh"

namespace commodore {
namespace cpu {
namespace instructions {

class dey : public mos_6510_y_nullary_instruction<0x88, 2> {
public:
	void execute() {
		get_cpu()->get_registers().Y--;
		update_flags_ZN(get_cpu());
	}

	void disassemble(std::ostream& stream) const {
		mos_disassemble(stream, "DEY");
	}
};

}
}
}

#endif

