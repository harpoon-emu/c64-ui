#include "c64.hh"
#include "console_log.hh"
#include "counted_breakpoint_argument.hh"
#include "cpu/mos_6510.hh"
#include "integer.hh"
#include "load_argument.hh"

#include "harpoon/memory/deserializer/binary_file.hh"
#include "harpoon/memory/serializer/binary_file.hh"

#include <boost/program_options.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

static void set_options_description(boost::program_options::options_description &desc) {
	desc.add_options()("help,h", "produce help message");
	desc.add_options()("disassemble,d", boost::program_options::bool_switch()->default_value(false),
	                   "disassemble executed instructions");
	desc.add_options()("flat-memory,f", boost::program_options::bool_switch()->default_value(false),
	                   "use flat memory (no ROM, no I/O, don't load KERNAL and ROM)");
	desc.add_options()(
	    "init-pc,p",
	    boost::program_options::value<c64::cli::integer<std::uint16_t>>()->default_value(0xffff),
	    "initial value of PC register ($FFFF - read from RESET_VECTOR)");
	desc.add_options()(
	    "load,l",
	    boost::program_options::value<std::vector<c64::cli::load_argument>>()->composing(),
	    "load data from file into memory (ADDRESS:FILE)");
	desc.add_options()(
	    "kernal,K", boost::program_options::value<std::string>()->default_value("kernal.rom"),
	    "load KERNAL from file into $E000-$FFFF (same effect as --load FILE:0xE000)");
	desc.add_options()("basic,B",
	                   boost::program_options::value<std::string>()->default_value("basic.rom"),
	                   "load BASIC from file into $A000-$BFFF (same effect as --load FILE:0xA000)");
	desc.add_options()(
	    "no-kernal,k", boost::program_options::bool_switch()->default_value(false),
	    "do not load KERNAL (has no effect if KERNAL image is loaded using --load option)");
	desc.add_options()("no-basic,b", boost::program_options::bool_switch()->default_value(false),
	                   "do not load BASIC (has no effect if BASIC image is loaded using --load "
	                   "option)");
	desc.add_options()(
	    "terminate-pc,T",
	    boost::program_options::value<std::vector<c64::cli::integer<std::uint16_t>>>()->composing(),
	    "terminate and dump MOS 6510 state when PC equal to address (same as --terminate-at-count "
	    "ADDRESS:1");
	desc.add_options()(
	    "terminate-pc-count",
	    boost::program_options::value<std::vector<c64::cli::counted_breakpoint_argument>>()
	        ->composing(),
	    "terminate and dump MOS 6510 state when PC reaches address specified number of times "
	    "(ADDRESS:COUNT)");
	desc.add_options()("terminate-after,A",
	                   boost::program_options::value<c64::cli::integer<std::uint32_t>>(),
	                   "terminate after executing specified number of instructions");
	desc.add_options()(
	    "dump-state-pc,D",
	    boost::program_options::value<std::vector<c64::cli::integer<std::uint16_t>>>()->composing(),
	    "dump MOS 6510 state when PC equal to address");
	desc.add_options()("dump-memory,m", boost::program_options::value<std::string>(),
	                   "dump content of memory to file on exit");
	desc.add_options()("statistics,s",
	                   boost::program_options::value<std::uint64_t>()->default_value(0),
	                   "dump statistics every X executed instructions (0 = disable)");
}

int main(int argc, char **argv) {

	try {
		boost::program_options::options_description desc("Allowed options");
		set_options_description(desc);

		boost::program_options::variables_map vm;
		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc),
		                              vm);
		boost::program_options::notify(vm);

		if (vm.count("help")) {
			std::cout << desc << std::endl;
			return 0;
		}

		std::shared_ptr<c64::cli::console_log> log;
		std::shared_ptr<c64::hw::c64> c64;

		log = std::make_shared<c64::cli::console_log>();
		c64 = std::make_shared<c64::hw::c64>(log);

		if (vm["flat-memory"].as<bool>()) {
			c64->enable_flat_memory();
		}

		c64->create();
		auto mos_6510 = c64->get_mos_6510();

		mos_6510->set_stats_interval(vm["statistics"].as<std::uint64_t>());

		if (vm["disassemble"].as<bool>()) {
			c64->get_main_execution_unit()->enable_disassemble();
		}
		mos_6510->set_init_PC(vm["init-pc"].as<c64::cli::integer<std::uint16_t>>());

		c64->prepare();

		std::vector<c64::cli::load_argument> loads;
		if (!vm["load"].empty()) {
			loads = vm["load"].as<std::vector<c64::cli::load_argument>>();
		}
		if (!vm["no-kernal"].as<bool>()) {
			loads.push_back({0xE000, vm["kernal"].as<std::string>()});
		}
		if (!vm["no-basic"].as<bool>()) {
			loads.push_back({0xA000, vm["basic"].as<std::string>()});
		}
		for (auto &l : loads) {
			harpoon::memory::deserializer::binary_file d(l.address, l.file);
			c64->get_main_memory()->deserialize(d);
		}

		auto b_test = [](c64::cli::integer<std::uint16_t> b, std::uint32_t &count,
		                 harpoon::execution::processing_unit *processing_unit) -> bool {
			c64::hw::cpu::mos_6510 *mos = static_cast<c64::hw::cpu::mos_6510 *>(processing_unit);
			if (mos->get_PC() - 1 != b) {
				return false;
			}
			if (count > 0) {
				count--;
			}
			return count == 0;
		};

		auto b_action = [&c64](bool shutdown,
		                       harpoon::execution::processing_unit *processing_unit) {
			c64::hw::cpu::mos_6510 *mos = static_cast<c64::hw::cpu::mos_6510 *>(processing_unit);
			mos->log_state();
			if (shutdown) {
				c64->shutdown();
			}
		};

		std::vector<c64::cli::counted_breakpoint_argument> c_terms;
		if (!vm["terminate-pc-count"].empty()) {
			c_terms
			    = vm["terminate-pc-count"].as<std::vector<c64::cli::counted_breakpoint_argument>>();
		}
		if (!vm["terminate-pc"].empty()) {
			for (auto b : vm["terminate-pc"].as<std::vector<c64::cli::integer<std::uint16_t>>>()) {
				c_terms.push_back({b, 1});
			}
		}
		for (auto &tb : c_terms) {
			std::uint32_t count = tb.count;
			mos_6510->add_breakpoint({std::bind(b_test, tb.address, count, std::placeholders::_1),
			                          std::bind(b_action, true, std::placeholders::_1)});
		}

		if (!vm["terminate-after"].empty()) {
			std::uint32_t count = vm["terminate-after"].as<c64::cli::integer<std::uint32_t>>();
			mos_6510->add_breakpoint(
			    {[count](harpoon::execution::processing_unit *processing_unit) -> bool {
				     return processing_unit->get_executed_instructions() == count;
			     },
			     std::bind(b_action, true, std::placeholders::_1)});
		}

		if (!vm["dump-state-pc"].empty()) {
			for (auto b : vm["dump-state-pc"].as<std::vector<c64::cli::integer<std::uint16_t>>>()) {
				std::uint32_t count = 1;
				mos_6510->add_breakpoint({std::bind(b_test, b, count, std::placeholders::_1),
				                          std::bind(b_action, false, std::placeholders::_1)});
			}
		}

		c64->boot();
		c64->run();

		if (!vm["dump-memory"].empty()) {
			harpoon::memory::serializer::binary_file ser(
			    c64->get_main_memory()->get_address_range(), vm["dump-memory"].as<std::string>());
			c64->get_main_memory()->serialize(ser);
		}
	} catch (std::exception &e) {
		std::cerr << "error: " << e.what() << std::endl;
	} catch (...) {
		std::cerr << "unknown exception" << std::endl;
	}
}
