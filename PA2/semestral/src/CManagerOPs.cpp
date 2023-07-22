//
// Created by afu on 05.06.23.
//

#include "CManagerOPs.h"
#include "CItem.h"
#include "CDir.h"
#include "CFile.h"
#include "CLink.h"
#include "filesystem"
#include "regex"
#include "fstream"

using namespace std;
namespace fs = std::filesystem;

void CManagerOPs::Copy(CItem *item, const std::string &to) {
    if (fs::exists(to))
        item->Copy(to);

}

void CManagerOPs::Copy(const std::string &reg, const std::string &to, CItem *CurrDir) {
    std::map<std::string, std::shared_ptr<CItem>> items;
    std::regex r(reg);
    if (!CurrDir->m_Items.empty()) {
        for (auto it = CurrDir->m_Items.begin(); it != CurrDir->m_Items.end(); ++it) {
            if (regex_match(it->second->m_Name, r)) {
                items[it->second->m_Path] = it->second;
            }
        }
        for (auto i = items.begin(); i != items.end(); ++i) {
            i->second->Copy(to);
        }
    }
}

void CManagerOPs::Delete(CItem *item) {
    item->Delete();
}

void CManagerOPs::Delete(const string &reg, CItem *CurrDir) {
    std::map<std::string, std::shared_ptr<CItem>> items;
    regex r(reg);
    if (!CurrDir->m_Items.empty()) {
        for (auto it = CurrDir->m_Items.begin(); it != CurrDir->m_Items.end(); ++it) {
            if (regex_match(it->second->m_Name, r)) {
                items[it->second->m_Path] = it->second;
            }
        }
        for (auto i = items.begin(); i != items.end(); ++i) {
            i->second->Delete();
        }
    }
}

void CManagerOPs::Move(CItem *item, const string &dest) {
    item->Move(dest);

}

void CManagerOPs::Move(const string &reg, const std::string &to, CItem *CurrDir) {
    std::map<std::string, std::shared_ptr<CItem>> items;
    regex r(reg);
    if (!CurrDir->m_Items.empty()) {
        for (auto it = CurrDir->m_Items.begin(); it != CurrDir->m_Items.end(); ++it) {
            if (regex_match(it->second->m_Name, r)) {
                items[it->second->m_Path] = it->second;
            }
        }
        for (auto i = items.begin(); i != items.end(); ++i) {
            i->second->Move(to);
        }

    } else {
    }
}

void CManagerOPs::CreateFolder(const string &name, CItem *CurrDir) {
    if (CurrDir->IsWriteable() && CurrDir->IsReadable()) {
        shared_ptr<CItem> tmp = shared_ptr<CItem>(new CDir(CurrDir->m_Path + "/" + name, CurrDir));
        CurrDir->m_Items.insert({tmp->m_Path, tmp});
    } else
        throw logic_error("Can't create folder");
}

void CManagerOPs::CreateFile(const string &name, CItem *CurrDir) {
    if (CurrDir->IsWriteable() && CurrDir->IsReadable()) {
        shared_ptr<CItem> tmp = shared_ptr<CItem>(new CFile(CurrDir->m_Path + "/" + name, CurrDir));
        CurrDir->m_Items.insert({tmp->m_Path, tmp});
    } else
        throw logic_error("Can't create file");
}

void CManagerOPs::CreateLink(const string &name, CItem *to, CItem *CurrDir) {
    if (CurrDir->IsWriteable() && CurrDir->IsReadable()) {
        shared_ptr<CItem> tmp = shared_ptr<CItem>(new CLink(CurrDir->m_Path + "/" + name, to, CurrDir));
        CurrDir->m_Items.insert({tmp->m_Path, tmp});
    } else
        throw logic_error("Can't create link");

}


void CManagerOPs::FindByText(const std::string &text, CItem *CurrDir) {

    if (!CurrDir->m_Items.empty()) {

        vector<CItem *> found;
        CurrDir->FindText(text, &found);
        ofstream MyFile(CurrDir->m_Path+"/foundItems.txt");
        for (size_t i = 0; i < found.size(); ++i) {
            MyFile << found[i]->m_Path << endl;
        }
        MyFile.close();
    }

}

void CManagerOPs::Deduplicate(CItem *item, CItem *CurrDir) {
    if (!CurrDir->m_Items.empty()) {
        CurrDir->Deduplicate(item);
    }

}

void CManagerOPs::ConcatFiles(std::map<std::string, std::shared_ptr<CItem>> *Items, const string &to) {
    if (Items->size() > 1) {
        auto itr = Items->begin();
        CItem *item = itr->second->m_InFolder;
        CreateFolder(item->m_Path + "/" + to,item);

        for (auto it = Items->begin(); it != Items->end(); ++it) {
            it->second->ConCat(item->m_Path + "/" + to);
        }


    } else
        throw logic_error("Choose at least 2 files");
}