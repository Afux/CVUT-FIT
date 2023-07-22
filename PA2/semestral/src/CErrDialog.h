//
// Created by afu on 06.06.23.
//

#ifndef SEMESTRALWORK_CERRDIALOG_H
#define SEMESTRALWORK_CERRDIALOG_H

#include "iostream"
#include "CAbsWidnow.h"

class CErrDialog : CAbsWidnow {
public:
    CErrDialog(CSize size);

    std::string m_Label;
    CAbsWidnow *m_LastActive;

    void Print() override;

    void Setup(CAbsWidnow *lastActive, const std::string &Label);

    void ReadKey() override;

    void Run();

private:
    void PrintBorders(const char &c) const;

    void ClearDialogSpace() const;

    void PrintLabel() const;

};


#endif //SEMESTRALWORK_CERRDIALOG_H
