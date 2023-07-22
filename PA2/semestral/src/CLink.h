
#ifndef SEMESTRALWORK_CLINK_H
#define SEMESTRALWORK_CLINK_H

#include "CItem.h"
#include "CDir.h"

class CLink : public CItem {
public:
    CLink(std::string path, CItem *toFile, CItem *parr);

    CItem *m_toFile;

    void Open(std::map<std::string, std::shared_ptr<CItem>> **item, CItem **inFold) override;

    void Print() override;

    void Copy(const std::string &to) override;

    void Delete() override;

    void Move(const std::string &dest) override;

    void Deduplicate(CItem *DeduplicateMe) override;

    void FindText(const std::string &FindThis, std::vector<CItem *> *Found) override;

    void ConCat(const std::string &To) override;


};


#endif //SEMESTRALWORK_CLINK_H
