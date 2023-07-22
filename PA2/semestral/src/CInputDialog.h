

#ifndef SEMESTRALWORK_CINPUTDIALOG_H
#define SEMESTRALWORK_CINPUTDIALOG_H

#include "CItem.h"
#include "CSize.h"
#include "iostream"
#include "CAbsWidnow.h"
#include "CErrDialog.h"

class CManager;

class CInputDialog : public CAbsWidnow {
public:

    CInputDialog(CSize size);

    CAbsWidnow *m_LastActive;

    void Print() override;

    void ReadKey() override;

    void Run();

    void Setup(CItem *Curr, CAbsWidnow *LastActive, const int &Op, const std::string &Label, CItem *SelectedItem,
               CAbsWidnow *LastWin, std::map<std::string, std::shared_ptr<CItem>> *SelectedItems);

private:

    std::string m_Label;
    CItem *m_SelectedItem;
    CItem *m_CurrDir;
    int m_Op;
    CAbsWidnow *m_WinActive;
    std::map<std::string, std::shared_ptr<CItem>> *m_SelectedItems;

    std::string m_Input;
    std::string m_Reg;
    std::string m_Path;
    CErrDialog m_ErrDialog;

    void parseString(const std::string &input, char delimiter);

    void PrintBorders();

    void ClearDialogSpace();

    void PrintLabel();

    void Enter();
};


#endif //SEMESTRALWORK_CINPUTDIALOG_H
