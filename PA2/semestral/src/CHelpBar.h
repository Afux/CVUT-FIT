
#ifndef SEMESTRALWORK_CHELPBAR_H
#define SEMESTRALWORK_CHELPBAR_H

#include "CSize.h"
#include "iostream"
#include "vector"
#include "CAbsWidnow.h"

class CHelpBar : public CAbsWidnow {
public:
    CHelpBar(CSize size);

    std::vector<std::string> m_Content;

    void Print() override;

    void ReadKey() override;
};


#endif //SEMESTRALWORK_CHELPBAR_H
