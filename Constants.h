#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
namespace Constants {
// window bounds
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 300;

// main theme colours
const juce::Colour WHITE_COLOUR = juce::Colour(100, 100, 100);
const juce::Colour BLUE_COLOUR = juce::Colour(76, 144, 255);
const juce::Colour ORGANGE_COLOUR = juce::Colour(255, 144, 76);
const juce::Colour GREEN_COLOUR = juce::Colour(144, 255, 76);

// component xml names
const juce::String COMPONENT_IMAGE_BACKGROUND = "image-background";
const juce::String COMPONENT_SLIDER_WETDRY = "slider-wetdry";
const juce::String COMPONENT_SLIDER_GAIN = "slider-gain";
const juce::String COMPONENT_BUTTON_CHANNEL_SPLIT = "button-splitChannels";
const juce::String COMPONENT_BUTTON_CHANNEL_LEFT = "button-leftChannel";
const juce::String COMPONENT_BUTTON_CHANNEL_RIGHT = "button-rightChannel";

const juce::String COMPONENT_CANVAS = "component-canvas";
const juce::String COMPONENT_RULER_HORIZONTAL = "ruler-horizontal";
const juce::String COMPONENT_RULER_VERTICAL = "ruler-vertical";
const juce::String COMPONENT_COMBOBOX_GRID_RESOLUTION = "comboBox-gridRes";

const juce::String COMPONENT_BUTTON_BYPASS = "button-bypass";
const juce::String COMPONENT_COMBOBOX_PRESETS = "comboBox-preset";
const juce::String COMPONENT_BUTTON_GRID_LEFT = "button-gridLeft";
const juce::String COMPONENT_BUTTON_GRID_RIGHT = "button-gridRight";
const juce::String COMPONENT_LABEL_GRID_RES = "label-gridRes";
// component labels
const juce::String TEXT_BUTTON_CHANNEL_SPLIT = "X";
const juce::String TEXT_BUTTON_CHANNEL_LEFT = "L";
const juce::String TEXT_BUTTON_CHANNEL_RIGHT = "R";
const juce::String TEXT_BUTTON_GRID_LEFT = "<";
const juce::String TEXT_BUTTON_GRID_RIGHT = ">";

const juce::String TEXT_UNIT_SLIDER_WETDRY = "mix";
const juce::String TEXT_UNIT_SLIDER_GAIN = "db";

const juce::String TEXT_RULER_HORIZONTAL_TICKS_END = "1 bar";

// button groups
enum RadioGroups { LeftRightChannel = 1001 };

// rotary button values
const float WETDRY_MIN = 0.0f;
const float WETDRY_MAX = 1.0f;
const float WETDRY_STEP = 0.01f;
const float WETDRY_VALUE = 0.5f;

const float GAIN_MIN = -9.0f;
const float GAIN_MAX = 9.0f;
const float GAIN_STEP = 1.0f;
const float GAIN_VALUE = 0.0f;

constexpr char FORMAT_STRING_WETDRY[] = "{}:{}";
constexpr char FORMAT_STRING_GAIN_POS[] = "+{:.0}";
constexpr char FORMAT_STRING_GAIN_NEG[] = "{:.0}";
constexpr char FORMAT_STRING_BEATS[] = "{}/{}";

// ruler
const int TICK_MARKER_LENGTH = 8;
const int TICK_MARKER_WIDTH = 2;

const int RULER_VERTICAL_NUM_TICKS = 2;

// grid resolution
const int GRID_RES_INITIAL_INDEX = 1;

const juce::String GRID_LABEL = "Grid: ";
const juce::String GRID_HALF = "1/2";
const juce::String GRID_QUATER = "1/4";
const juce::String GRID_EIGHTH = "1/8";
const juce::String GRID_SIXTEENTH = "1/16";
} // namespace Constants
