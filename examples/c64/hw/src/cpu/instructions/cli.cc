#include "cli.hh"

#include "../mos_6510.hh"
#include "instruction_step.hh"

namespace c64 {
namespace hw {
namespace cpu {
namespace instructions {
namespace cli {

harpoon::execution::instruction implied::factory(harpoon::execution::processing_unit *cpu) {
	return set_flag_factory<&mos_6510::set_flag_I, false>(cpu, MNEMONIC);
}

} // namespace cli
} // namespace instructions
} // namespace cpu
} // namespace hw
} // namespace c64
