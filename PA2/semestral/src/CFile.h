
#ifndef SEMESTRALWORK_CFILE_H
#define SEMESTRALWORK_CFILE_H

#include "CItem.h"
#include "CDir.h"

class CFile : public CItem {
public:
    CFile(std::string path, CItem *parr);

    void Print() override;

    void Copy(const std::string &to) override;

    void Delete() override;

    void Move(const std::string &dest) override;

    void FindText(const std::string &FindThis, std::vector<CItem *> *Found) override;

    void Deduplicate(CItem *DeduplicateMe) override;


    void Open(std::map<std::string, std::shared_ptr<CItem>> **item, CItem **inFold) override;

    void ConCat(const std::string &To) override;

private:
    bool identicalFiles(const std::string &file1, const std::string &file2);

};


#endif //SEMESTRALWORK_CFILE_H
