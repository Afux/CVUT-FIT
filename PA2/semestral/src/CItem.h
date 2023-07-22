
#ifndef SEMESTRALWORK_CITEM_H
#define SEMESTRALWORK_CITEM_H

#include "iostream"
#include "vector"
#include "map"
#include <memory>
#include <iostream>
#include <filesystem>

class CItem {


public:
    CItem(std::string path, CItem *inFolder);

    virtual ~CItem();

    std::string m_Name;
    std::string m_Path;
    bool m_isSelected;

    void Select();

    CItem *m_InFolder;
    std::map<std::string, std::shared_ptr<CItem>> m_Items;

    virtual void Print() = 0;

    virtual void Open(std::map<std::string, std::shared_ptr<CItem>> **item, CItem **inFold) = 0;

    virtual void Delete() = 0;

    virtual void Move(const std::string &dest) = 0;

    virtual void Copy(const std::string &dest) = 0;

    virtual void FindText(const std::string &FindThis, std::vector<CItem *> *Found) = 0;

    virtual void ConCat(const std::string &To) = 0;

    virtual void Deduplicate(CItem *DeduplicateMe) = 0;

    bool IsReadable();

    bool IsWriteable();

    bool IsReadable(const std::filesystem::path &p);

protected:

    bool IsWriteable(const std::filesystem::path &p);


};


#endif //SEMESTRALWORK_CITEM_H
