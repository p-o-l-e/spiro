#include "Menu.hpp"
#include "PluginEditor.h"
#include "core/grid.hpp"
#include "core/modules/cso.hpp"
#include "core/modules/interface/cso_interface.hpp"
#include "core/modules/interface/vco_interface.hpp"
#include "descriptor.hxx"
#include "node.hpp"
#include <sys/types.h>

void mainMenu(Editor* editor)
{
    editor->stopTimer();
    editor->display->MainMenu();
};

void croMenu(Editor* editor, const int& i)
{
    editor->startTimerHz(core::settings::scope_fps);
    editor->display->page = Display::Page::CroA;
    editor->display->layerOn = false;
};

void vcoMenu(Editor * editor, const int& i)
{
   editor->stopTimer();
   editor->display->page = static_cast<Display::Page>(i);
   editor->display->VCOMenu(editor->processor.spiro.rack.at(i), i);
}

void csoMenu(Editor* editor, const int& i)
{
    editor->stopTimer();
    editor->display->page = static_cast<Display::Page>(i);
    editor->display->CSOMenu(editor->processor.spiro.rack.at(i), i);
}

void lfoMenu(Editor* editor, const int& i)
{
    editor->stopTimer();
    editor->display->page = static_cast<Display::Page>(i);
    editor->display->LFOMenu(editor->processor.spiro.rack.at(i), i);
}

void Editor::setOption(const core::uid_t& uid, const float& delta, const float& max)
{
    auto index = core::grid.getIndex(uid);

    processor.parameters[index] = processor.tree.getParameter(core::grid.name(uid, true));
    float value = processor.parameters[index]->convertFrom0to1(processor.parameters[index]->getValue());
    value += delta;
    if      (value > max) value = 0.0f;
    else if (value < 0.0f) value = max;
    processor.parameters[index]->beginChangeGesture();
    processor.parameters[index]->setValueNotifyingHost(processor.parameters[index]->convertTo0to1(value));
    processor.parameters[index]->endChangeGesture();
}
