/* Copyright 2019 Thomas Baart
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// Defines the keycodes used by our macros in process_record_user
// enum custom_keycodes { QMKBEST = SAFE_RANGE, QMKURL };

enum custom_keycodes {
    HS_PCB = SAFE_RANGE,
    HS_CASE,
    ITERM_NEXT_TAB,
    ITERM_PREV_TAB,
};

#define _BASE 0
#define _TRANSITION 1
#define _NUMPAD 2
#define _NUMPAD_MOD 3

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(/* Base */
            KC__MUTE,  TO(_TRANSITION),  XXXXXXX,
            XXXXXXX,  XXXXXXX,  XXXXXXX,
            XXXXXXX,  XXXXXXX,  XXXXXXX,
            ITERM_PREV_TAB,  ITERM_NEXT_TAB, XXXXXXX
    ),
    [_TRANSITION] = LAYOUT(/* Transition */
            XXXXXXX,  TO(_BASE),  TO(_NUMPAD),
            XXXXXXX,  XXXXXXX,  XXXXXXX,
            XXXXXXX,  XXXXXXX,  XXXXXXX,
            XXXXXXX,  XXXXXXX,  XXXXXXX
    ),
    [_NUMPAD] = LAYOUT(/* Numpad */
            TO(_TRANSITION),  MO(_NUMPAD_MOD),  KC_0,
            KC_1,  KC_2,  KC_3,
            KC_4,  KC_5,  KC_6,
            KC_7,  KC_8,  KC_9 
    ),
    [_NUMPAD_MOD] = LAYOUT(/* Numpad Mod*/
            XXXXXXX,  KC_TRANSPARENT,  KC_PPLS,
            KC_LPRN,  XXXXXXX,         KC_PMNS,
            KC_RPRN,  XXXXXXX,         KC_PAST,
            KC_BSPC,  KC_PENT,         KC_PSLS
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HS_PCB:
            if (record->event.pressed) {
                SEND_STRING("853400");
            } else {
            }
            break;
        case HS_CASE:
            if (record->event.pressed) {
                SEND_STRING("847160");
            } else {
            }
            break;
        case ITERM_PREV_TAB:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LGUI)SS_DOWN(X_LSHIFT)SS_TAP(X_LBRACKET)SS_UP(X_LSHIFT)SS_UP(X_LGUI));
            } else {
            }
            break;
        case ITERM_NEXT_TAB:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LGUI)SS_DOWN(X_LSHIFT)SS_TAP(X_RBRACKET)SS_UP(X_LSHIFT)SS_UP(X_LGUI));
            } else {
            }
            break;
    }
    return true;
}

#ifdef RGBLIGHT_ENABLE
void keyboard_post_init_user(void) {
  rgblight_enable_noeeprom(); // Enables RGB, without saving settings
  rgblight_sethsv_noeeprom(100, 200, 150);
  rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_SWIRL);
}

uint32_t layer_state_set_user(uint32_t state) {
  switch (biton32(state)) {
    case _BASE:
      rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_SWIRL);
      rgblight_sethsv_noeeprom(100, 200, 150);
      break;
    case _TRANSITION:
      rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING + 3);
      rgblight_sethsv_noeeprom(100, 0, 150);
      break;
    case _NUMPAD:
    case _NUMPAD_MOD:
      rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
      rgblight_sethsv_noeeprom(HSV_ORANGE);
      break;
  }
  return state;
}
#endif

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	return OLED_ROTATION_180;
}

void oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_P(PSTR("Base\n"), false);
            break;
        case _TRANSITION:
            oled_write_P(PSTR("Transition\n"), false);
            break;
        case _NUMPAD:
        case _NUMPAD_MOD:
            oled_write_P(PSTR("Numpad\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }
}
#endif

#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        switch (get_highest_layer(layer_state)) {
            case _NUMPAD:
            case _NUMPAD_MOD:
                if (clockwise) {
                    tap_code(KC_RIGHT);
                } else {
                    tap_code(KC_LEFT);
                }
                break;
            default:
                if (clockwise) {
                    tap_code(KC__VOLUP);
                } else {
                    tap_code(KC__VOLDOWN);
                }
        }
    } else {
        rgblight_disable_noeeprom(); // Enables RGB, without saving settings
    }
}
#endif

void matrix_init_user(void) {}

void matrix_scan_user(void) {}

void led_set_user(uint8_t usb_led) {}

