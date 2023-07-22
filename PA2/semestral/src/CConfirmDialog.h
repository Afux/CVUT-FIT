

#ifndef SEMESTRALWORK_CCONFIRMDIALOG_H
#define SEMESTRALWORK_CCONFIRMDIALOG_H

#include "iostream"
#include "CSize.h"
#include "CItem.h"
#include "CAbsWidnow.h"
#include "CErrDialog.h"

class CConfirmDialog : public CAbsWidnow {
public:
    CConfirmDialog(CSize size);

    void Print() override;
    /**
       * @brief void Setup(CAbsWidnow *LastActive, CItem *SelectedItem, std::map<std::string, std::shared_ptr<CItem>> *Items,
               CAbsWidnow *Win, const int &Op);
       * Func. that set up variable for op's
       * @param LastActive represents Manager window
       * @param SelectedItem represents selected item
       * @param Items represent items in directory
       * @param Win represent current active window
       * @param Op represent key for operation
       */
    void Setup(CAbsWidnow *LastActive, CItem *SelectedItem, std::map<std::string, std::shared_ptr<CItem>> *Items,
               CAbsWidnow *Win, const int &Op);

    void ReadKey() override;

    void Enter();

    void Run();


private:
    std::string m_Label;
    CItem *m_SelectedItem;
    std::vector<std::string> m_Content;
    CAbsWidnow *m_LastActive;
    CAbsWidnow *m_Win;
    std::map<std::string, std::shared_ptr<CItem>> *m_Items;
    CErrDialog m_ErrDialog;
    int m_Op;

    void PrintBorders(const char &c);

    void ClearDialogSpace();

    void PrintLabel();

};


#endif //SEMESTRALWORK_CCONFIRMDIALOG_H
