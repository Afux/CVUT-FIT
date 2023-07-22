
#ifndef SEMESTRALWORK_CMENU_H
#define SEMESTRALWORK_CMENU_H

#include "iostream"
#include "CSize.h"
#include "CItem.h"
#include "CConfirmDialog.h"
#include "CInputDialog.h"
#include "CAbsWidnow.h"
#include "CErrDialog.h"

class CMenu : public CAbsWidnow {
public:

    CMenu(CSize size, CAbsWidnow *lastActive);

    CItem *CurrDir;
    CAbsWidnow *m_lastActive;
    CAbsWidnow *m_winActive;
    CInputDialog m_inputDialog;
    CErrDialog m_errorDialog;
    CItem *m_selectedItem;
    std::map<std::string, std::shared_ptr<CItem>> *m_SelectedItems;

    void Print() override;

    void ReadKey() override;

    void Enter();

    void Setup(CItem *Curr, CAbsWidnow *LastActive, CAbsWidnow *WinActive,
               std::map<std::string, std::shared_ptr<CItem>> *SelectedItems);

private:
    std::vector<std::string> m_Content;
    void PrintBorders(const char &c) const;
    void ClearDialogSpace();
};


#endif //SEMESTRALWORK_CMENU_H
