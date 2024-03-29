#ifndef UI_CONFIG_TAB_HH
#define UI_CONFIG_TAB_HH

#include <QPushButton>
#include <QWidget>
#include <memory>
#include <thread>

namespace c64 {

namespace hw {
class c64;
} // namespace hw

namespace ui {

class control_tab : public QWidget {
	Q_OBJECT

public:
	explicit control_tab(const std::shared_ptr<hw::c64> &c64, QWidget *parent = nullptr);

private slots:
	void boot();
	void shutdown();
	void dump_state();

private:
	void update_buttons();

	std::shared_ptr<hw::c64> _c64;

	std::unique_ptr<std::thread> _thread{};

	QPushButton *_boot_btn;
	QPushButton *_shutdown_btn;
	QPushButton *_dump_state_btn;
};

} // namespace ui
} // namespace c64

#endif
