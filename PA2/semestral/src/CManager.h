
#ifndef SEMESTRALWORK_CMANAGER_H
#define SEMESTRALWORK_CMANAGER_H

#include "CSize.h"
#include "CWindow.h"
#include "CHelpBar.h"
#include "CAbsWidnow.h"
#include "CInputDialog.h"
#include "CErrDialog.h"
#include "CMenu.h"
#include "CDir.h"


class CManager : public CAbsWidnow {
public:
    CManager(CSize size, std::string Path);

    CHelpBar m_HelpBar;
    CWindow m_LeftPanel;
    CWindow m_RightPanel;
    CMenu m_Menu;
    CInputDialog m_Input;
    CConfirmDialog m_Confirm;
    CWindow *m_ActiveWindow;
    CErrDialog m_ErrorDialog;
    CAbsWidnow *m_ActivePanel;
    void Run();
    void ReadKey() override;

private:
    void Print() override;
    CWindow *NextWind();

};


#endif //SEMESTRALWORK_CMANAGER_H
