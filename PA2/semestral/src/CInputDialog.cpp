

#include "CInputDialog.h"
#include "../libs/rang.hpp"
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include "filesystem"

using namespace rang;
using namespace std;
namespace fs = std::filesystem;

CInputDialog::CInputDialog(CSize size) : CAbsWidnow(size, this), m_ErrDialog(CErrDialog(size)) {
    m_Op = 0;
    m_Label = "Enter input";
}

void CInputDialog::Print() {


    ClearDialogSpace();
    PrintBorders();
    PrintLabel();

    moveto(1, m_Size.m_AbsPosY + 2);

}

void CInputDialog::ReadKey() {
    cursorOn();
    cout << bg::yellow << m_Label << ": " << bg::reset;
    std::getline(std::cin, m_Input);
    if (cin.eof()) {
        m_LastActive->mIsrunning = false;
    }
    if (m_Input.empty()) {
        m_Op = 0;
        m_LastActive->m_Scene = m_LastActive;
    }

    parseString(m_Input, ':');
    Enter();
    m_Op = 0;
    m_LastActive->m_Scene = m_LastActive;
}

void CInputDialog::Enter() {
    try {
        switch (m_Op) {
            case 1:
                m_OperMan.Delete(m_Input, m_CurrDir);
                m_WinActive->m_Selected = 0;
                break;
            case 2:
                m_OperMan.Copy(m_Reg, m_Path, m_CurrDir);
                break;
            case 3:
                m_OperMan.Move(m_Reg, m_Path, m_CurrDir);
                m_WinActive->m_Selected = 0;
                break;
            case 5:
                m_OperMan.FindByText(m_Input, m_CurrDir);
                break;
            case 6:
                m_OperMan.ConcatFiles(m_SelectedItems, m_Input);
                break;
            case 7:
                m_OperMan.Deduplicate(m_SelectedItem, m_CurrDir);
                break;
            case 8:
                m_OperMan.Copy(m_SelectedItem, m_Input);
                break;
            case 9:
                m_OperMan.Move(m_SelectedItem, m_Input);
                m_WinActive->m_Selected = 0;
                break;
            case 10:
                m_OperMan.CreateFile(m_Input, m_CurrDir);
                break;
            case 12:
                m_OperMan.CreateFolder(m_Input, m_CurrDir);
                break;
            case 13:
                m_OperMan.CreateLink(m_Input, m_SelectedItem, m_CurrDir);
                break;
            default:
                m_Op = 0;
                m_LastActive->m_Scene = m_LastActive;
                break;

        }
    }
    catch (logic_error &e) {
        m_ErrDialog.Setup(m_LastActive, e.what());
        m_ErrDialog.Run();
    }
    catch (const fs::filesystem_error &e) {
        m_ErrDialog.Setup(m_LastActive, e.code().message());
        m_ErrDialog.Run();
    }

    m_Op = 0;
    m_LastActive->m_Scene = m_LastActive;


}

void CInputDialog::Run() {
    Print();
    ReadKey();
}

//...
void CInputDialog::parseString(const string &input, char delimiter) {
    std::string::size_type start = 0;
    std::string::size_type end = input.find(delimiter);

    while (end != std::string::npos) {
        m_Reg = input.substr(start, end - start);
        start = end + 1;
        end = input.find(delimiter, start);
    }


    m_Path = input.substr(start);

}

void CInputDialog::Setup(CItem *Curr, CAbsWidnow *LastActive, const int &Op, const string &Label, CItem *SelectedItem,
                         CAbsWidnow *LastWin, std::map<std::string, std::shared_ptr<CItem>> *Selecteditems) {
    m_CurrDir = Curr;
    m_LastActive = LastActive;
    m_Op = Op;
    m_Label = Label;
    m_SelectedItem = SelectedItem;
    m_WinActive = LastWin;
    m_SelectedItems = Selecteditems;
}

void CInputDialog::PrintBorders() {
    moveto(m_Size.m_PosX, m_Size.m_PosY);
    for (size_t i = 1; i < m_Size.m_Width; ++i) {
        cout << bg::black << "*" << style::reset;
    }
    for (size_t i = m_Size.m_PosY; i < m_Size.m_Height + m_Size.m_PosY; ++i) {
        moveto(m_Size.m_PosX, i);
        cout << bg::black << "*" << style::reset;
        moveto((int) (m_Size.m_Width) + m_Size.m_PosX - 1, i);
        cout << bg::black << "*" << style::reset;
        moveto(m_Size.m_PosX, i);
    }
    //Dolni

    for (size_t i = 1; i < m_Size.m_Width; ++i) {
        cout << bg::black << "*" << style::reset;

    }

}

void CInputDialog::ClearDialogSpace() {
    moveto(m_Size.m_PosX, m_Size.m_PosY);
    for (size_t i = m_Size.m_PosY; i < m_Size.m_Height + m_Size.m_PosY; ++i) {
        moveto(m_Size.m_PosX, i);
        for (size_t j = 1; j < m_Size.m_Width; ++j) {
            cout << " ";
        }
    }
}

void CInputDialog::PrintLabel() {
    moveto(m_Size.m_PosX + 3, m_Size.m_PosY + 4);
    cout << m_Label;
}


