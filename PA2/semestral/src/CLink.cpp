
#include "CLink.h"
#include "filesystem"

namespace fs = std::filesystem;
using namespace std;

CLink::CLink(std::string path, CItem *toFile, CItem *parr) : CItem(path, parr),
                                                                                m_toFile(toFile) {

    if (toFile != NULL) {
        if (IsReadable(toFile->m_Path) && IsWriteable(toFile->m_Path) && !fs::exists(path)) {
            if (fs::is_directory(toFile->m_Path))
                fs::create_directory_symlink(toFile->m_Path, path);
            else
                fs::create_symlink(toFile->m_Path, path);
        }
    }
}


void CLink::Print() {
    cout << "*" << m_Name;
}


void CLink::Copy(const string &to) {
    try {
        fs::copy_symlink(m_Path, to + "/" + m_Name);
    }
    catch (const fs::filesystem_error &e) {
        throw logic_error(e.code().message());
    }

}

void CLink::Delete() {
    try {
        fs::remove_all(m_Path);
    }
    catch (const fs::filesystem_error &e) {
        throw logic_error(e.code().message());
    }
    if (m_InFolder != NULL && m_InFolder->m_Items.count(m_Path)) {
        m_InFolder->m_Items.erase(m_Path);
    }
}


void CLink::Move(const string &dest) {
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
        fs::remove_all(dest + "/" + m_Name);
        throw logic_error(e.what());
    }
}

void CLink::Open(std::map<std::string, std::shared_ptr<CItem>> **item, CItem **inFold) {}

void CLink::FindText(const string &FindThis, std::vector<CItem *> *Found) {}

void CLink::Deduplicate(CItem *DeduplicateMe) {}

void CLink::ConCat(const string &To) {}

