#include "KeyBrdReflectionCore_Qt.h"

//��ʼ���������ǿ�н�ASCII��ֵӳ�䵽���������ֵ
void KeyBrdReflectionCore_Qt::initializer() {
    reflist['-'] = reflist['_'] = VK_OEM_MINUS;//������ȵı�ʾ������ͬһ������shift״̬��ͬʱ�Ĳ�ͬ����
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

bool KeyBrdReflectionCore_Qt::isCapsLkOn() {
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

void KeyBrdReflectionCore_Qt::switch_lower() {
    if (isCapsLkOn()) {
        keybd_event(VK_CAPITAL, 0, 0, 0);
        keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
    }
}

void KeyBrdReflectionCore_Qt::switch_upper() {
    if (!isCapsLkOn()) {
        keybd_event(VK_CAPITAL, 0, 0, 0);
        keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
    }
}

void KeyBrdReflectionCore_Qt::input_space() {
    keybd_event(VK_SPACE, 0, 0, 0);
    keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
}

void KeyBrdReflectionCore_Qt::input_enter() {
    keybd_event(VK_RETURN, 0, 0, 0);
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
}

void KeyBrdReflectionCore_Qt::input_escape() {
    keybd_event(VK_ESCAPE, 0, 0, 0);
    keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);
}

//���������ַ����������
void KeyBrdReflectionCore_Qt::input_special(char kch) {
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

//������ֻ�ʹ�д��ĸASCIIһһ��Ӧ������Ҫ�����е�Сд��ĸת�ɴ�д���룬�����ַ��Ĵ�Сд���Կ���CapsLock
void KeyBrdReflectionCore_Qt::input_alphakey(char kch) {
    int keyid = toupper(kch);
    keybd_event(keyid, 0, 0, 0);
    keybd_event(keyid, 0, KEYEVENTF_KEYUP, 0);
}

//�����ֵĴ���
void KeyBrdReflectionCore_Qt::input_digit(char kch) {
    keybd_event(kch, 0, 0, 0);
    keybd_event(kch, 0, KEYEVENTF_KEYUP, 0);
}
