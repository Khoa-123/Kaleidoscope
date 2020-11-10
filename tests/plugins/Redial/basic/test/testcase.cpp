/* -*- mode: c++ -*-
 * Copyright (C) 2020  Keyboard.io, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "testing/setup-googletest.h"

SETUP_GOOGLETEST();

namespace kaleidoscope {
namespace testing {
namespace {

constexpr KeyAddr key_addr_Redial{0, 9};
constexpr KeyAddr key_addr_A{2, 1};
constexpr KeyAddr key_addr_X{3, 2};

class RedialBasic : public VirtualDeviceTest {
};

TEST_F(RedialBasic, RedialWithoutPriorKey) {

  PressKey(Millis{10}, key_addr_Redial);
  ReleaseKey(Millis{25}, key_addr_Redial);

  sim_.RunForMillis(10);
  LoadState();

  ASSERT_EQ(HIDReports()->Keyboard().size(), 0)
      << "There should be no HID report without a prior keypress";
}

TEST_F(RedialBasic, RedialFirstKey) {

  PressKey(Millis{10}, key_addr_A);
  ExpectReport(Cycles{1}, AddKeycodes{Key_A}, "Report should contain only `A`");
  ReleaseKey(Millis{25}, key_addr_A);
  ExpectReport(Cycles{1}, RemoveKeycodes{Key_A}, "Report should be empty");

  PressKey(Millis{10}, key_addr_Redial);
  ExpectReport(Cycles{1}, AddKeycodes{Key_A}, "Report should contain only `A`");
  ReleaseKey(Millis{25}, key_addr_Redial);
  ExpectReport(Cycles{1}, RemoveKeycodes{Key_A}, "Report should be empty");

  sim_.RunForMillis(10);

  CHECK_EXPECTED_REPORTS();
}

TEST_F(RedialBasic, RedialNextKey) {

  PressKey(Millis{10}, key_addr_X);
  ExpectReport(Cycles{1}, AddKeycodes{Key_X}, "Report should contain only `X`");
  ReleaseKey(Millis{25}, key_addr_X);
  ExpectReport(Cycles{1}, RemoveKeycodes{Key_X}, "Report should be empty");

  PressKey(Millis{10}, key_addr_Redial);
  ExpectReport(Cycles{1}, AddKeycodes{Key_X}, "Report should contain only `X`");
  ReleaseKey(Millis{25}, key_addr_Redial);
  ExpectReport(Cycles{1}, RemoveKeycodes{Key_X}, "Report should be empty");

  sim_.RunForMillis(10);

  CHECK_EXPECTED_REPORTS();
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
