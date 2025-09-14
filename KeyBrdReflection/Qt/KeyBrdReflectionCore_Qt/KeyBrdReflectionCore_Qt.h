#pragma once

#include "keybrdreflectioncore_qt_global.h"

class KEYBRDREFLECTIONCORE_QT_EXPORT KeyBrdReflectionCore_Qt
{
private:
    int reflist[256];
    const std::string u_shiftspc = "-=[];',./\\`";
    const std::string shiftspc = "_+{}|:\"<>?~";
    const std::string numshiftspc = ")!@#$%^&*(";

public:
    void initializer();
    bool isCapsLkOn();
    void switch_lower();
    void switch_upper();
    void input_space();
    void input_enter();
    void input_escape();
    void input_special(char kch);
    void input_alphakey(char kch);
    void input_digit(char kch);
};
