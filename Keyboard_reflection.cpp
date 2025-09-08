#include <fstream>
#include <iostream>
#include <Windows.h>
using namespace std;

//定义三个字符列表，以下的这些字符由于和ASCII码值不一一匹配，故需要单独进行查找。

//以下的两个字符串列表是同一个按键在shift状态不同时的不同输入
string u_shiftspc = "-=[];',./\\`";
string shiftspc = "_+{}|:\"<>?~";

//这个字符串列表是数字大键盘在shift状态时的输入
string numshiftspc = ")!@#$%^&*(";

int reflist[256];
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

int main(int argc,char* argv[])
{
    if (argc == 2) {
        initializer();
        cout << "使用前提醒：\n由于该软件的工作原理是将文本内容1：1复制，故建议关闭工作软件所有的智能提示、自动补全和自动缩进" << endl;
        cout << "复制进行期间最好不要随意使用键盘或者鼠标，否则可能会出现预料之外的结果\n" << endl;
        cout << "确认后按任意键继续..." << endl;
        getchar();

        for (int i = 3; i >= 1; i--) {
            cout << i << " 秒后开始复制。" << endl;
            Sleep(1000);
        }
        
        fstream targetfile(argv[1], ios::in);
        targetfile >> noskipws;//不要跳过空白字符
        char tkey;
        while (targetfile >> tkey) {
            if (isalpha(tkey)) {
                if (islower(tkey)) {
                    switch_lower();
                    input_alphakey(tkey);
                }
                else if (isupper(tkey)) {
                    switch_upper();
                    input_alphakey(tkey);
                }
            }
            else if (isdigit(tkey)) {
                input_digit(tkey);
            }
            else if (tkey == ' ') input_space();
            else if (tkey == '\n') input_enter();
            else input_special(tkey);
        }
    }

    input_enter();
    cout << "复制内容不能保证在工作区上百分百一致，但仅需稍作修改即可\n" << endl;
    cout << "按任意键继续..." << endl;
    getchar();
    return 0;
}
