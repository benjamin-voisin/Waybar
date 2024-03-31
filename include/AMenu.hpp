#pragma once

#include <glibmm/markup.h>
#include <gtkmm/label.h>
#include <json/json.h>

#include "AModule.hpp"

namespace waybar {

class AMenu : public AModule {
 public:
  AMenu(const Json::Value &, const std::string &, const std::string &, const std::string &format,
         uint16_t interval = 0, bool ellipsize = false, bool enable_click = false,
         bool enable_scroll = false);
  virtual ~AMenu() = default;
  auto update() -> void override;
  virtual std::string getIcon(uint16_t, const std::string &alt = "", uint16_t max = 0);
  virtual std::string getIcon(uint16_t, const std::vector<std::string> &alts, uint16_t max = 0);

 protected:
  Gtk::Label label_;
  std::string format_;
  const std::chrono::seconds interval_;
  bool alt_ = false;
  std::string default_format_;

  bool handleToggle(GdkEventButton *const &e) override;
  virtual std::string getState(uint8_t value, bool lesser = false);

  GtkMenu* menu_ = GTK_MENU(gtk_menu_new());
  Gtk::EventBox event_box;
};


    /* GtkWidget* feur1 = gtk_menu_item_new_with_label("feur"); */
    /* gtk_menu_attach(menu, GTK_WIDGET(feur1), 0,1000,0,10); */

    /* GtkWidget* feur2 = gtk_menu_item_new_with_label("feur2"); */
    /* gtk_menu_attach(menu, GTK_WIDGET(feur2), 0,1000,10,20); */

    /* GtkWidget* feur = gtk_text_view_new(); */
    /* GtkTextBuffer* buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (feur)); */
    /* gtk_text_buffer_set_text (buffer, "Hello, this is some text", -1); */
    /* gtk_menu_attach(menu, GTK_WIDGET(feur), 0,1000,0,1); */


    /* gtk_menu_popup_at_pointer (menu, reinterpret_cast<GdkEvent*>(e)); */

}  // namespace waybar
