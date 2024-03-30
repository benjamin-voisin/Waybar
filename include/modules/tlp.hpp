#pragma once

#include "ALabel.hpp"
#include "util/sleeper_thread.hpp"

namespace waybar::modules {

class TLP : public ALabel {
  public:
    TLP(const std::string&, const Json::Value&);
    virtual ~TLP();
    auto update() -> void override;

  private:
    bool tlpstat_;
    bool tlpmode_;
    util::SleeperThread thread_;

    std::string tlp_request(std::string); // static
    void update_status(); // static
    void update_style(); // static
    std::string tlpstat_tostring(bool tlpstat); // static
    std::string tlpstat_tostring(); // static
    std::string tlpmode_tostring(bool tlpmode); // static
    std::string tlpmode_tostring(); // static

    std::string format(std::string format);

    void toggleStatus(); // static
    bool handleClick(GdkEventButton* const& e);

    void showMenu(GdkEventButton* const& e);
    void quit();

};

}  // namespace waybar::modules

