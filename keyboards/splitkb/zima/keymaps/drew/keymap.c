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

enum layers { _BASE = 0, _TRANSITION, _FIGHTSTICK };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {[_BASE] = LAYOUT_ortho_4x3(KC_MUTE, TO(_TRANSITION), KC_ENTER, C(KC_LEFT), KC_P8, C(KC_RGHT), KC_MPRV, KC_MPLY, KC_MNXT, G(S(KC_LBRC)), KC_MPLY, G(S(KC_RBRC))), [_TRANSITION] = LAYOUT_ortho_4x3(XXXXXXX, TO(_BASE), TO(_FIGHTSTICK), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX), [_FIGHTSTICK] = LAYOUT_ortho_4x3(TO(_TRANSITION), KC_P0, KC_PDOT, KC_P7, KC_P8, KC_P9, KC_P4, KC_P5, KC_P6, KC_P1, KC_P2, KC_P3)};

void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom(); // Enables RGB, without saving settings
    rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_SWIRL);
    rgblight_sethsv_noeeprom(100, 240, 150);
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (get_highest_layer(layer_state | default_layer_state)) {
        default:
            if (clockwise) {
                tap_code(KC_PGDN);
            } else {
                tap_code(KC_PGUP);
            }
            break;
    }
    return false;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case _BASE:
            rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_SWIRL);
            rgblight_sethsv_noeeprom(100, 240, 150);
            break;
        case _TRANSITION:
            rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING + 3);
            rgblight_sethsv_noeeprom(100, 0, 150);
            break;
        case _FIGHTSTICK:
            rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
            rgblight_sethsv_noeeprom(21, 240, 150);
            break;
    }
    return state;
}

bool oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_P(PSTR("Base\n"), false);
            break;
        case _TRANSITION:
            oled_write_P(PSTR("Transition\n"), false);
            break;
        case _FIGHTSTICK:
            oled_write_P(PSTR("Fightstick\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
            break;
    }
    return false;
}
