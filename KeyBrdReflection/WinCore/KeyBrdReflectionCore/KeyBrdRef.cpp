﻿#include "pch.h"

#include "KeyBrdRef.h"

//初始化键盘码表，强行将ASCII码值映射到虚拟键盘码值
void initializer() {
    reflist['-'] = reflist['_'] = VK_OEM_MINUS;//两个相等的表示二者是同一个键在shift状态不同时的不同输入
    reflist['='] = reflist['+'] = VK_OEM_PLUS;
    reflist['['] = reflist['{'] = VK_OEM_4;
    reflist[']'] = reflist['}'] = VK_OEM_6;
    reflist['\\'] = reflist['|'] = VK_OEM_5;
    reflist[';'] = reflist[':'] = VK_OEM_1;
    reflist['\''] = reflist['\"'] = VK_OEM_7;
    reflist[','] = reflist['<'] = VK_OEM_COMMA;
    reflist['.'] = reflist['>'] = VK_OEM_PERIOD;
    reflist['/'] = reflist['?'] = VK_OEM_2;
    reflist['`'] = reflist['~'] = VK_OEM_3;
    reflist[0] = '0';
    reflist[1] = '1';
    reflist[2] = '2';
    reflist[3] = '3';
    reflist[4] = '4';
    reflist[5] = '5';
    reflist[6] = '6';
    reflist[7] = '7';
    reflist[8] = '8';
    reflist[9] = '9';
}

bool isCapsLkOn() {
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

void switch_lower() {
    if (isCapsLkOn()) {
        keybd_event(VK_CAPITAL, 0, 0, 0);
        keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
    }
}

void switch_upper() {
    if (!isCapsLkOn()) {
        keybd_event(VK_CAPITAL, 0, 0, 0);
        keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
    }
}

void input_space() {
    keybd_event(VK_SPACE, 0, 0, 0);
    keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
}

void input_enter() {
    keybd_event(VK_RETURN, 0, 0, 0);
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
}

void input_escape() {
    keybd_event(VK_ESCAPE, 0, 0, 0);
    keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);
}

//三类特殊字符分情况讨论
void input_special(char kch) {
    if (u_shiftspc.find(kch) != u_shiftspc.npos) {
        keybd_event(reflist[kch], 0, 0, 0);
        keybd_event(reflist[kch], 0, KEYEVENTF_KEYUP, 0);
    }
    else if (shiftspc.find(kch) != shiftspc.npos) {
        keybd_event(VK_SHIFT, 0, 0, 0);
        keybd_event(reflist[kch], 0, 0, 0);
        keybd_event(reflist[kch], 0, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
        if (kch == '>') input_escape();
    }
    else if (numshiftspc.find(kch) != numshiftspc.npos) {
        keybd_event(VK_SHIFT, 0, 0, 0);
        keybd_event(reflist[numshiftspc.find(kch)], 0, 0, 0);
        keybd_event(reflist[numshiftspc.find(kch)], 0, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
    }
}

//键盘码只和大写字母ASCII一一对应，所以要把所有的小写字母转成大写输入，具体字符的大小写可以控制CapsLock
void input_alphakey(char kch) {
    int keyid = toupper(kch);
    keybd_event(keyid, 0, 0, 0);
    keybd_event(keyid, 0, KEYEVENTF_KEYUP, 0);
}

//对数字的处理
void input_digit(char kch) {
    keybd_event(kch, 0, 0, 0);
    keybd_event(kch, 0, KEYEVENTF_KEYUP, 0);
}
