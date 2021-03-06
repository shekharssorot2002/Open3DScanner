/**
 * Prior starting a scan an overview of all settings and values are presented to the user which can
 * either confirm or abort the scanning job.
 */

/**
 * Indicator if ok or cancel was selected
 */
bool ok_selected = true;

// Bitmap to display on the lcd. It one line on the screen and shows OK and CANCEL
// OK is selected
const unsigned char OK_SELECTED[84] PROGMEM = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3,
0xBD, 0xBD, 0xBD, 0xC3, 0xFF, 0x81, 0xE7, 0xDB, 0xBD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42,
0x42, 0x42, 0x24, 0x00, 0x7C, 0x0A, 0x0A, 0x0A, 0x7C, 0x00, 0x7E, 0x04, 0x08, 0x10, 0x7E, 0x00,
0x3C, 0x42, 0x42, 0x42, 0x24, 0x00, 0x7E, 0x4A, 0x4A, 0x42, 0x42, 0x00, 0x7E, 0x40, 0x40, 0x40,
0x00, 0x00, 0x00, 0x00,  
};

// Bitmap to display on the lcd. It one line on the screen and shows OK and CANCEL
// CANCEL is selected
const unsigned char CANCEL_SELECTED[84] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C,
0x42, 0x42, 0x42, 0x3C, 0x00, 0x7E, 0x18, 0x24, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0xBD,
0xBD, 0xBD, 0xDB, 0xFF, 0x83, 0xF5, 0xF5, 0xF5, 0x83, 0xFF, 0x81, 0xFB, 0xF7, 0xEF, 0x81, 0xFF,
0xC3, 0xBD, 0xBD, 0xBD, 0xDB, 0xFF, 0x81, 0xB5, 0xB5, 0xBD, 0xBD, 0xFF, 0x81, 0xBF, 0xBF, 0xBF,
0xFF, 0xFF, 0xFF, 0xFF,  
};

void scan_confirm_state_logic() {
  // process inputs
  if (encoder_button_pressed) {
    consume_encoder_button();
    // depending on the seleted item change to another state
    if (ok_selected) {
      current_state = SCAN;
    } else {
      current_state = MENU;
    }
    // reset the ok_selected status for next time the menu is opened
    ok_selected = true;
  } else if ((int32_t)encoder.getCount() != 0) {
    if ((int32_t)encoder.getCount() > 0) {
      ok_selected = false;
    } else {
      ok_selected = true;
    }
    encoder.setCount(0);
  }

  // retrieve/calculate data to display
  uint64_t etr = time_for_scan(*selected_scan);
  
  // show content on lcd
  String photos_per_revolution = "Photos/Rev: " + String(selected_scan->photos_per_revolution);
  String rotation = "Rotation: " + String(selected_scan->rotation);
  String rotation_steps = "#Rot Steps: " + String(selected_scan->rotation_divisions);
  String number_of_photos = "#Pics: " + String(selected_scan->photos_per_revolution * selected_scan->rotation_divisions);
  String etr_str = retrieve_time_string_for_ms(etr);

  lcd.clear(false);
  print_centered(photos_per_revolution, 0);
  print_centered(rotation, 1);
  print_centered(rotation_steps, 2);
  print_centered(number_of_photos, 3);
  print_centered(etr_str, 4);
  lcd.setCursor(0, 5);
  if (ok_selected) {
    lcd.draw(OK_SELECTED, 84, true);
  } else {
    lcd.draw(CANCEL_SELECTED, 84, true);
  }

  // sleep some time for (better) visibility on screen
  delay(TIME_BETWEEN_FRAMES);
}
