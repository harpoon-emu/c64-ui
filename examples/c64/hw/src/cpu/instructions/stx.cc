#include "stx.hh"

#include "../mos_6510.hh"
#include "write_instruction.hh"

namespace c64 {
namespace hw {
namespace cpu {
namespace instructions {
namespace stx {

template<void (instruction_step::*store)(std::uint8_t)>
using store_x = store_register<std::uint8_t, &mos_6510::get_X, store>;

harpoon::execution::instruction zero_page::factory(harpoon::execution::processing_unit *cpu) {
	return zero_page_write_factory<store_x>(cpu, MNEMONIC);
}

harpoon::execution::instruction zero_page_y::factory(harpoon::execution::processing_unit *cpu) {
	return zero_page_y_write_factory<store_x>(cpu, MNEMONIC);
}

harpoon::execution::instruction absolute::factory(harpoon::execution::processing_unit *cpu) {
	return absolute_write_factory<store_x>(cpu, MNEMONIC);
}

} // namespace stx
} // namespace instructions
} // namespace cpu
} // namespace hw
} // namespace c64
