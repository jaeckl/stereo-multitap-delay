/*
Copyright (C) 2024  Ludwig Jäck

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
namespace Constants {
// window bounds
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 300;

// main theme colours
const juce::Colour WHITE_COLOUR = juce::Colour(255, 255, 255);
const juce::Colour BLUE_COLOUR = juce::Colour(76, 144, 255);
const juce::Colour ORGANGE_COLOUR = juce::Colour(255, 144, 76);
const juce::Colour GREEN_COLOUR = juce::Colour(144, 255, 76);

// component xml names
const juce::String COMPONENT_IMAGE_BACKGROUND = "image-background";
const juce::String COMPONENT_SLIDER_WETDRY = "slider-wetdry";
const juce::String COMPONENT_SLIDER_GAIN = "slider-gain";
const juce::String COMPONENT_SLIDER_FILTER = "slider-filter";
const juce::String COMPONENT_BUTTON_CHANNEL_SPLIT = "button-splitChannels";
const juce::String COMPONENT_BUTTON_CHANNEL_LEFT = "button-leftChannel";
const juce::String COMPONENT_BUTTON_CHANNEL_RIGHT = "button-rightChannel";

const juce::String COMPONENT_CANVAS = "component-canvas";
const juce::String COMPONENT_RULER_HORIZONTAL = "ruler-horizontal";
const juce::String COMPONENT_RULER_VERTICAL = "ruler-vertical";
const juce::String COMPONENT_COMBOBOX_GRID_RESOLUTION = "comboBox-gridRes";

const juce::String COMPONENT_BUTTON_BYPASS = "button-bypass";
const juce::String COMPONENT_BUTTON_HIGHPASS = "button-highpass";
const juce::String COMPONENT_BUTTON_LOWPASS = "button-lowpass";
const juce::String COMPONENT_COMBOBOX_PRESETS = "comboBox-preset";
const juce::String COMPONENT_BUTTON_GRID_LEFT = "button-gridLeft";
const juce::String COMPONENT_BUTTON_GRID_RIGHT = "button-gridRight";
const juce::String COMPONENT_LABEL_GRID_RES = "label-gridRes";

const juce::String COMPONENT_BUTTON_FILESYTEM = "button-filesystem";
const juce::String COMPONENT_BREADCRUMB_DIRS = "breadcrumb-dirs";
const juce::String COMPONENT_FILELIST = "filelist";
const juce::String COMPONENT_BUTTON_BACK = "button-back";
const juce::String COMPONENT_TEXT_LABEL_PATH = "label-folderpath";
const juce::String COMPONENT_LABEL_NO_PRESET = "label-nopreset";
const juce::String TEXT_PRESET_FILE_WILDCARD = "*.xml";
const juce::String TEXT_PRESET_FOLDER_WILDCARD = "*";
const juce::String TEXT_PRESETS_NOT_FOUND = "No Presets Found";

const juce::String COMPONENT_VIEW_XYEDITOR = "view-xyEditor";
const juce::String COMPONENT_VIEW_FILEBROWSER = "view-fileBrowser";
// component labels
const juce::String TEXT_BUTTON_CHANNEL_SPLIT = "X";
const juce::String TEXT_BUTTON_CHANNEL_LEFT = "L";
const juce::String TEXT_BUTTON_CHANNEL_RIGHT = "R";
const juce::String TEXT_BUTTON_GRID_LEFT = "<";
const juce::String TEXT_BUTTON_GRID_RIGHT = ">";

const juce::String TEXT_UNIT_SLIDER_WETDRY = "mix";
const juce::String TEXT_UNIT_SLIDER_GAIN = "db";
const juce::String TEXT_UNIT_SLIDER_HZ = "Hz";
const juce::String TEXT_UNIT_SLIDER_KHZ = "kHz";

const juce::String TEXT_TITLE_WETDRY = "Dry/Wet";
const juce::String TEXT_TITLE_GAIN = "Gain";
const juce::String TEXT_TITLE_FILTER = "Filter";

const juce::String TEXT_RULER_HORIZONTAL_TICKS_END = "1 bar";

const juce::String TEXT_CRUMB_SYMBOL = " > ";

// button groups
enum RadioGroups { LeftRightChannel = 1001, HighLowPass = 1002 };

// rotary button values
const float WETDRY_MIN = 0.0f;
const float WETDRY_MAX = 1.0f;
const float WETDRY_STEP = 0.01f;
const float WETDRY_VALUE = 0.5f;

const float GAIN_MIN = -9.0f;
const float GAIN_MAX = 9.0f;
const float GAIN_STEP = 1.0f;
const float GAIN_VALUE = 0.0f;

const float FILTER_MIN = std::log10(20.0f);
const float FILTER_MAX = std::log10(20000.0f);
const float FILTER_STEP = 0.01f;
const float FILTER_VALUE = (FILTER_MIN + FILTER_MAX) / 2.0f;
const float FILTER_VALUES_WITHOUT_PRECISION = 100.0f;
const float FILTER_VALUES_IN_KHZ = 1000.0f;

constexpr char FORMAT_STRING_WETDRY[] = "{}:{}";
constexpr char FORMAT_STRING_FLOAT_ZERO_PREC_POS[] = "+{:.0f}";
constexpr char FORMAT_STRING_FLOAT_ZERO_PREC[] = "{:.0f}";
constexpr char FORMAT_STRING_FLOAT_SINGLE_PREC[] = "{:.1f}";
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
