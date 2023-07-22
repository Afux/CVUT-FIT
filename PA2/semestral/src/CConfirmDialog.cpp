

#include "CConfirmDialog.h"
#include <unistd.h>
#include <iostream>
#include "../libs/rang.hpp"

using namespace rang;
using namespace std;

CConfirmDialog::CConfirmDialog(CSize size) : CAbsWidnow(size, this), m_ErrDialog(size) {
    m_Op = 0;
    m_Content.emplace_back("[YES]");
    m_Content.emplace_back("[No]");
    m_Label = "ARE YOU SURE?";
}

void CConfirmDialog::Print() {
    if (m_Selected >= m_Content.size()) {
        m_Selected = 0;
    }

    ClearDialogSpace();
    PrintBorders('*');
    PrintLabel();


    moveto(m_Size.m_PosX + 2, m_Size.m_Height - 4 + m_Size.m_PosY);
    for (size_t i = 0; i < m_Content.size(); ++i) {
        if (i == m_Selected) {
            cout << bg::blue << m_Content[i] << style::reset;
        } else {
            cout << m_Content[i];
        }
        moveto(m_Size.m_Width - 10 + m_Size.m_PosX, m_Size.m_Height - 4 + m_Size.m_PosY);
    }

    moveto(1, m_Size.m_AbsPosY + 2);

}

void CConfirmDialog::ReadKey() {
    cursorOff();

    char c;
    std::cin >> c;


    switch (c) {
        case 'a':
        case 'A':
            m_Selected--;
            break;
        case 'd':
        case 'D':
            m_Selected++;
            break;
        case '\x04':
            m_LastActive->mIsrunning = false;
            break;
        case 'E':
        case 'e':
            Enter();
            break;


    }

    cursorOn();
}

void CConfirmDialog::Enter() {
    if (m_Selected == 0) {
        switch (m_Op) {
            case 1:
                m_Win->m_Selected--;
                if (m_Win->m_Iter != m_Items->begin()) {
                    m_Win->m_Iter--;
                }
                try {
                    m_OperMan.Delete(m_SelectedItem);
                }
                catch (logic_error &e) {
                    m_ErrDialog.Setup(m_LastActive, e.what());
                }
                break;
        }
        m_Op = 0;
        m_Selected = 0;
        m_LastActive->m_Scene = m_LastActive;


    } else {
        m_Selected = 0;
        m_LastActive->m_Scene = m_LastActive;
    }


}

void CConfirmDialog::Run() {
    while (1) {
        Print();
        ReadKey();
        clear();
    }

}

void CConfirmDialog::PrintBorders(const char &c) {
    moveto(m_Size.m_PosX, m_Size.m_PosY);
    //Upper borders
    for (size_t i = 1; i < m_Size.m_Width; ++i) {
        cout << bg::black << c << style::reset;

    }
    //SIDE BORDES
    for (size_t i = m_Size.m_PosY; i < m_Size.m_Height + m_Size.m_PosY; ++i) {
        moveto(m_Size.m_PosX, i);
        cout << bg::black << c << style::reset;
        moveto((int) (m_Size.m_Width) + m_Size.m_PosX - 1, i);
        cout << bg::black << c << style::reset;
        moveto(m_Size.m_PosX, i);
    }
    //Bottom bordesr
    for (size_t i = 1; i < m_Size.m_Width; ++i) {
        cout << bg::black << c << style::reset;

    }
}

void CConfirmDialog::ClearDialogSpace() {
    //CLEAR DIALOG
    moveto(m_Size.m_PosX, m_Size.m_PosY);
    for (size_t i = m_Size.m_PosY; i < m_Size.m_Height + m_Size.m_PosY; ++i) {
        moveto(m_Size.m_PosX, i);
        for (size_t j = 1; j < m_Size.m_Width; ++j) {
            cout << " ";
        }
    }
}

void CConfirmDialog::PrintLabel() {
    //LABEL
    moveto(m_Size.m_PosX + 3, m_Size.m_PosY + 4);
    cout << m_Label;
}

void CConfirmDialog::Setup(CAbsWidnow *LastActive, CItem *SelectedItem, std::map<std::string, std::shared_ptr<CItem>> *Items,
                           CAbsWidnow *Win, const int &Op) {
    m_LastActive = LastActive;
    m_SelectedItem = SelectedItem;
    m_Items = Items;
    m_Win = Win;
    m_Op = Op;
}
