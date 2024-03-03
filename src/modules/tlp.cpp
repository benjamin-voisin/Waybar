#include "modules/tlp.hpp"
#include "util/command.hpp"

waybar::modules::TLP::TLP(const std::string& id, const Json::Value& config)
    : ALabel(config, "tlp", id, "{status} ({mode})", 2), tlpstat_(false), tlpmode_(false)
{
  tlp_request("tlp-stat --version");

  event_box_.add_events(Gdk::BUTTON_PRESS_MASK);
  event_box_.signal_button_press_event().connect(
      sigc::mem_fun(*this, &TLP::handleToggle));

  thread_ = [this] {
    dp.emit();
    thread_.sleep_for(interval_);
  };
}

waybar::modules::TLP::~TLP() {}

std::string waybar::modules::TLP::tlp_request(std::string command) {
  waybar::util::command::res res = waybar::util::command::exec(command, "TLP");
  if (res.exit_code) {
    throw std::runtime_error(
      fmt::format(fmt::runtime("Unable to use `{cmd}`!"),
                  fmt::arg("cmd", command)
      ));
  }
  return res.out;
}

void waybar::modules::TLP::update_status() {
  std::string status = tlp_request("tlp-stat -m -q");
  if( status == "AC" ) {
    tlpstat_  = true;
    tlpmode_ = false;
  }
  else if( status == "AC (manual)" ) {
    tlpstat_  = true;
    tlpmode_ = true;
  }
  else if( status == "battery" ) {
    tlpstat_  = false;
    tlpmode_ = false;
  }
  else if( status == "battery (manual)" ) {
    tlpstat_  = false;
    tlpmode_ = true;
  }
  else throw std::runtime_error(
      fmt::format(fmt::runtime("Unable to recognize tlp-stat `{status}`!"),
                  fmt::arg("status", status)
  ));
}

void waybar::modules::TLP::update_style() {

  label_.get_style_context()->remove_class(tlpmode_tostring(!tlpmode_));
  label_.get_style_context()->remove_class(tlpstat_tostring(!tlpstat_));
  label_.get_style_context()->add_class(tlpmode_tostring());
  label_.get_style_context()->add_class(tlpstat_tostring());


  label_.set_markup(format(format_));

  if (tooltipEnabled()) {
    auto config = config_["tooltip-format"];
    auto tooltip_format = config.isString() ? config.asString() : format_;
    label_.set_tooltip_markup(format(tooltip_format));
  }

  // Call parent update
  ALabel::update();
}

std::string waybar::modules::TLP::tlpstat_tostring(bool tlpstat) {
  if (tlpstat)
    return "ac";
  else
    return "bat";
}
std::string waybar::modules::TLP::tlpstat_tostring() {
  return tlpstat_tostring(tlpstat_);
}

std::string waybar::modules::TLP::tlpmode_tostring(bool tlpmode) {
  if (tlpmode)
    return "manual";
  else
    return "auto";
}
std::string waybar::modules::TLP::tlpmode_tostring() {
  return tlpmode_tostring(tlpmode_);
}

std::string waybar::modules::TLP::format(std::string format) {
  return fmt::format(fmt::runtime(format),
                     fmt::arg("status", tlpstat_tostring()),
                     fmt::arg("mode",   tlpmode_tostring()),
                     fmt::arg("icon_stat", getIcon(0, tlpstat_tostring())),
                     fmt::arg("icon_mode", getIcon(1, tlpmode_tostring())));
}

auto waybar::modules::TLP::update() -> void {
  update_status();
  update_style();
}

void waybar::modules::TLP::toggleStatus() {
  std::string cmd = "";
  if (config_["sudo"] == "true") {
    cmd = "sudo ";
  } else {
    cmd = "pkexec ";
  }
  if (tlpmode_) {
    // if on MANUAL mode put AUTO
    tlpmode_ = false;
    tlpstat_ = false;
    cmd = cmd + "tlp start";
  } else {
    tlpmode_ = true;
    if (tlpstat_) {
      // if on AUTO AC mode put MANUAL BATTERY mode
      tlpstat_ = false;
      cmd = cmd + "tlp bat";
    } else {
      // if on AUTO BATTERY mode put MANUAL AC mode
      tlpstat_ = true;
      cmd = cmd + "tlp ac";
    }
  }
  update_style();
  tlp_request(cmd);

}

bool waybar::modules::TLP::handleToggle(GdkEventButton* const& e) {
  if (e->button == 1) {
    toggleStatus();
  }

  ALabel::handleToggle(e);
  return true;
}
