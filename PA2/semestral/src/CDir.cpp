
#include "CDir.h"
#include "CFile.h"
#include "CLink.h"
#include "set"
#include "filesystem"
#include "iostream"

using directory_iterator = std::filesystem::directory_iterator;
namespace fs = std::filesystem;
using namespace std;

CDir::CDir(std::string path, CItem *inFolder) : CItem(path, inFolder) {
    if (IsReadable(fs::path(path).parent_path()) && IsWriteable(fs::path(path).parent_path()) && !fs::exists(path)) {
        try {
            fs::create_directory(path);
        }
        catch (const fs::filesystem_error &e) {
            throw logic_error(e.code().message());
        }
    }
}

void CDir::Copy(const string &to) {

    try {
        if (IsWriteable(to))
            fs::create_directory(to + "/" + m_Name);
    }
    catch (const fs::filesystem_error &e) {
        throw logic_error(e.code().message());
    }
    try {
        fs::copy(m_Path, to + "/" + m_Name, std::filesystem::copy_options::recursive);
    }
    catch (const fs::filesystem_error &e) {
        fs::remove(to + "/" + m_Name);
        throw logic_error(e.code().message());

    }
}


void CDir::Delete() {
    try {
        fs::remove_all(m_Path);
    }
    catch (const fs::filesystem_error &e) {
        throw logic_error(e.code().message());
    }
    if (m_InFolder != NULL) {
        if (m_InFolder->m_Items.count(m_Path))
            m_InFolder->m_Items.erase(m_Path);
    }
}

void CDir::Move(const string &dest) {


    try {
        Copy(dest);
    }
    catch (const logic_error &e) {
        throw logic_error(e.what());
    }
    try {
        Delete();
    }
    catch (const logic_error &e) {
        throw logic_error(e.what());
    }

}


void CDir::Print() {
    cout << "/" << m_Name;

}


void CDir::Open(std::map<std::string, std::shared_ptr<CItem>> **items, CItem **inFold) {
    FillItems();
    Refresh();
    *items = &m_Items;
    *inFold = this;

}


void CDir::FindText(const string &FindThis, std::vector<CItem *> *Found) {
    if (m_Items.empty()) {
        FillItems();
    }
    for (auto it = m_Items.begin(); it != m_Items.end(); ++it) {
        it->second->FindText(FindThis, Found);
    }
}

void CDir::Deduplicate(CItem *DeduplicateMe) {
    if (m_Items.empty()) {
        FillItems();
    }
    for (auto it = m_Items.begin(); it != m_Items.end(); ++it) {
        if (it->second.get() != DeduplicateMe)
            it->second->Deduplicate(DeduplicateMe);
    }
    Refresh();

}

void CDir::ConCat(const string &To) {}

void CDir::Refresh() {
    std::set<string> tmp;
    if (IsReadable(m_Path) && fs::exists(m_Path)) {
        for (const auto &dirEntry: filesystem::directory_iterator(m_Path,
                                                                  std::filesystem::directory_options::skip_permission_denied)) {
            string s = dirEntry.path();
            tmp.insert(s);
            if (IsReadable(s) && !m_Items.count(s)) {
                if (dirEntry.is_symlink()) {

                    shared_ptr<CItem> temp = shared_ptr<CItem>(new CLink(s, NULL, this));
                    m_Items[temp->m_Path] = temp;

                } else if (dirEntry.is_directory()) {

                    shared_ptr<CItem> temp = shared_ptr<CItem>(new CDir(s, this));
                    m_Items[temp->m_Path] = temp;

                } else if (dirEntry.is_regular_file()) {

                    shared_ptr<CItem> temp = shared_ptr<CItem>(new CFile(CFile(s,this)));
                    m_Items[temp->m_Path] = temp;

                }
            }
        }

        for (auto it = m_Items.begin(); it != m_Items.end(); it++) {
            it->second->m_isSelected = false;
            if (!tmp.count(it->first) || !IsReadable(it->first)) {
                m_Items.erase(it->first);
            }

        }
    }

}

void CDir::FillItems() {
    if (m_Items.empty() && IsReadable(m_Path)) {
        for (const auto &dirEntry: filesystem::directory_iterator(m_Path,
                                                                  std::filesystem::directory_options::skip_permission_denied)) {
            string s = dirEntry.path();
            if (!m_Items.count(s)) {


                if (IsReadable(s)) {
                    if (dirEntry.is_symlink()) {
                        shared_ptr<CItem> tmp = shared_ptr<CItem>(new CLink(s, NULL, this));
                        m_Items[tmp->m_Path] = tmp;

                    } else if (dirEntry.is_directory()) {

                        shared_ptr<CItem> tmp = shared_ptr<CItem>(new CDir(s, this));
                        m_Items[tmp->m_Path] = tmp;

                    } else if (dirEntry.is_regular_file()) {

                        shared_ptr<CItem> tmp = shared_ptr<CItem>(new CFile(CFile(s, this)));
                        m_Items[tmp->m_Path] = tmp;

                    }
                }
            }
        }
    }

}


