//
// Created by afu on 06.06.23.
//

#include "CErrDialog.h"
#include "../libs/rang.hpp"
#include "termios.h"

using namespace std;
using namespace rang;

CErrDialog::CErrDialog(CSize size) : CAbsWidnow(size, this) {
    m_Label = "Chyba";
}

void CErrDialog::Print() {
    ClearDialogSpace();
    PrintBorders('*');
    PrintLabel();
    moveto(m_Size.m_PosX + (m_Size.m_Width / 2), m_Size.m_Height - 4 + m_Size.m_PosY);
    cout << bg::blue << "[OK]" << style::reset;
    moveto(m_Size.m_Width - 10 + m_Size.m_PosX, m_Size.m_Height - 4 + m_Size.m_PosY);
    moveto(1, m_Size.m_AbsPosY + 2);
}

void CErrDialog::PrintBorders(const char &c) const {
    moveto(m_Size.m_PosX, m_Size.m_PosY);
    //Upper borders
    for (size_t i = 1; i < m_Size.m_Width; ++i) {
        cout << bg::black << c << style::reset;

    }
    for (size_t i = m_Size.m_PosY; i < m_Size.m_Height + m_Size.m_PosY; ++i) {
        moveto(m_Size.m_PosX, i);
        cout << bg::black << c << style::reset;
        moveto((int) (m_Size.m_Width) + m_Size.m_PosX - 1, i);
        cout << bg::black << c << style::reset;
        moveto(m_Size.m_PosX, i);
    }
    for (size_t i = 1; i < m_Size.m_Width; ++i) {
        cout << bg::black << c << style::reset;

    }
}

void CErrDialog::ClearDialogSpace() const{
    moveto(m_Size.m_PosX, m_Size.m_PosY);
    for (size_t i = m_Size.m_PosY; i < m_Size.m_Height + m_Size.m_PosY; ++i) {
        moveto(m_Size.m_PosX, i);
        for (size_t j = 1; j < m_Size.m_Width; ++j) {
            cout << " ";
        }
    }
}

void CErrDialog::PrintLabel() const {
    moveto(m_Size.m_PosX + (m_Size.m_Width / 2), m_Size.m_PosY + 4);
    cout << m_Label;
}

void CErrDialog::ReadKey() {
    cursorOff();
    char c;
    std::cin >> c;


    switch (c) {
        case 'E':
        case 'e':
            m_LastActive->m_Scene = m_LastActive;
            break;
        case '\x04':
            m_LastActive->mIsrunning = false;
    }

    cursorOn();
}

void CErrDialog::Setup(CAbsWidnow *lastActive, const string &Label) {
    m_LastActive = lastActive;
    m_Label = Label;
}

void CErrDialog::Run() {
    Print();
    ReadKey();

}
