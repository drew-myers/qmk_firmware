/*
Copyright 2022 <hello@keebfront.com>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "analog.h"
uint8_t last_val = 0;
extern MidiDevice midi_device;

enum layers {
    _LAYER0,
    _LAYER1,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_LAYER0] = LAYOUT_all(
        KC_MUTE, QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,  KC_HOME,
                 KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_PGUP,
                 KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGDN,
                 KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,
                 KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                    KC_RALT, MO(1),            KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [_LAYER1] = LAYOUT_all(
        _______, QK_GESC, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, KC_END,
                 _______, UG_TOGG, UG_NEXT, UG_HUEU, UG_HUED, UG_SATU, UG_SATD, UG_VALU, UG_VALD, _______, KC_PSCR, KC_SCRL, KC_PAUS, QK_BOOT,          _______,
                 _______, UG_SPDU, UG_SPDD, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
                 _______, _______, _______, _______, _______, _______, _______, NK_TOGG, _______, _______, _______, _______, _______, KC_VOLU,
                 _______, _______, _______,                            _______,                   _______, _______,          KC_MPRV, KC_VOLD, KC_MNXT
    )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_LAYER0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_LAYER1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
};
#endif

void slider(void) {
    uint8_t current_val = analogReadPin(SLIDER_PINA) >>3;

    if ( last_val - current_val < -1 || last_val - current_val > 1 ) { 
        midi_send_cc(&midi_device, 0, 90, current_val );
    }

    last_val = current_val;
}

void housekeeping_task_user(void) {
    slider();
}
