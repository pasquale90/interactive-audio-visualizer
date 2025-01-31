#include "settingsTest.cpp"
#include "configTest.cpp"
#include "triggerTest.cpp"
#include "timerTest.cpp"
#include "sineTest.cpp"
#include "audiolizerTest.cpp"
#include "waveformTest.cpp"
#include "spectrogramTest.cpp"

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
