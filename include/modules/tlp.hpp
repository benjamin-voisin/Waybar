#pragma once

#include "ALabel.hpp"


namespace waybar::modules {

class TLP : public ALabel {
  public:
    TLP(const std::string&, const Json::Value&);
    virtual ~TLP();
    auto update() -> void override;

  private:
    bool tlpstat_;
    bool tlpmode_;
    
    const std::string DEFAULT_FORMAT = "{status} ({mode})";
    const std::string DEFAULT_FORMAT_ALT = "{status} ({mode})";

    std::string tlp_request(std::string); // static
    void update_status(); // static
    std::string tlpstat_tostring(bool tlpstat); // static
    std::string tlpstat_tostring(); // static
    std::string tlpmode_tostring(bool tlpmode); // static
    std::string tlpmode_tostring(); // static

    std::string format(std::string format);

    void toggleStatus(); // static
    bool handleToggle(GdkEventButton* const& e) override;

};

}  // namespace waybar::modules

