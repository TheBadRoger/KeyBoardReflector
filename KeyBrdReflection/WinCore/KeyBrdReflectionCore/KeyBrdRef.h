#pragma once

//导出DLL
#ifdef KEYBRDREFLECTIONCORE_EXPORTS
#define KEYBOARD_REFLECTION_API __declspec(dllexport)
#else
#define KEYBOARD_REFLECTION_API __declspec(dllimport)
#endif

#include <string>
//这些是静态变量
//定义三个字符列表，以下的这些字符由于和ASCII码值不一一匹配，故需要单独进行查找。

//前两个字符串列表是同一个按键在shift状态不同时的不同输入

//这些为内部变量，不导出

const static std::string u_shiftspc = "-=[];',./\\`";
const static std::string shiftspc = "_+{}|:\"<>?~";
const static std::string numshiftspc = ")!@#$%^&*(";

static int reflist[256];

//声明函数并导出

extern KEYBOARD_REFLECTION_API void initializer();

extern KEYBOARD_REFLECTION_API bool isCapsLkOn();

extern KEYBOARD_REFLECTION_API void switch_lower();

extern KEYBOARD_REFLECTION_API void switch_upper();

extern KEYBOARD_REFLECTION_API void input_space();

extern KEYBOARD_REFLECTION_API void input_enter();

extern KEYBOARD_REFLECTION_API void input_escape();

extern KEYBOARD_REFLECTION_API void input_special(char kch);

extern KEYBOARD_REFLECTION_API void input_alphakey(char kch);

extern KEYBOARD_REFLECTION_API void input_digit(char kch);