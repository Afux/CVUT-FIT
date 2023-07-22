
#include "CFile.h"
#include "filesystem"
#include <fstream>
#include "CLink.h"

namespace fs = std::filesystem;
using namespace std;

CFile::CFile(std::string path, CItem *parr) : CItem(path, parr) {

    try {
        if (!fs::exists(path)) {
            try {
                std::ofstream{path};
            }
            catch (...) {
                throw logic_error("Cant create file");
            }
        }
    }
    catch (...) {
        throw logic_error("Cant create file");
    }

}

void CFile::Print() {
    cout << m_Name;

}

void CFile::Copy(const string &to) {
    try {
        std::filesystem::copy(m_Path, to, std::filesystem::copy_options::recursive);
    }
    catch (const fs::filesystem_error &e) {
        throw logic_error(e.code().message());
    }

}

void CFile::Delete() {
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


void CFile::Move(const string &dest) {

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



void CFile::Open(std::map<std::string, std::shared_ptr<CItem>> **item, CItem **inFold) {}

void CFile::FindText(const string &FindThis, std::vector<CItem *> *Found) {

    ifstream inFile(m_Path, ios::in);
    if (inFile.good()) {

        string temp;
        size_t pos;

        while (inFile.good()) {
            getline(inFile, temp);
            pos = temp.find(FindThis);
            if (pos != string::npos) {
                Found->push_back(this);
                break;
            }
        }
        inFile.close();
    }

}

void CFile::Deduplicate(CItem *DeduplicateMe) {
    if (identicalFiles(m_Path, DeduplicateMe->m_Path)) {

        try {
            if (fs::exists(m_Path)) {
                if (IsReadable(m_Path) && IsWriteable(m_Path)) {
                    std::filesystem::remove_all(m_Path);
                }
            }
        }
        catch (const fs::filesystem_error &e) {
            throw logic_error(e.code().message());
        }

        shared_ptr<CItem> tmp = shared_ptr<CItem>(new CLink(m_Path, DeduplicateMe, m_InFolder));
        m_InFolder->m_Items[m_Path] = tmp;

    }
}

bool CFile::identicalFiles(const string &file1, const string &file2) {
    bool flag = false;
    ifstream stream1(file1, ios::in | ios::binary);
    ifstream stream2(file2, ios::in | ios::binary);
    if (!stream1.good()) {
        return false;
    }
    if (!stream2.good()) {
        return false;
    }
    stream1.seekg(0, ios::end);
    stream2.seekg(0, ios::end);
    long int file_size1 = stream1.tellg();
    long int file_size2 = stream2.tellg();
    if (file_size1 != file_size2) {
        stream1.close();
        if (!stream1.good()) {
            return false;
        }
        stream2.close();
        if (!stream2.good()) {
            return false;
        }
        return false;
    }

    for (int i = 0; i < file_size1; i++) {
        stream1.seekg(i);
        unsigned char x;
        stream1.read((char *) &x, 1);
        stream2.seekg(i);
        unsigned char y;
        stream2.read((char *) &y, 1);
        if (x != y) {
            flag = true;
            break;
        }


    }

    stream1.close();
    if (!stream1.good()) {

        return false;
    }
    stream2.close();
    if (!stream2.good()) {
        return false;
    }
    if (!flag)
        return true;
    else
        return false;
}

void CFile::ConCat(const string &To) {
    ifstream stream1(m_Path, ios::in);
    fstream stream2(To, ios::out | ios::app);
    string temp;
    if (stream1.good() && stream2.good()) {
        while (getline(stream1, temp)) {
            stream2 << temp;
            stream2 << endl;
        }
    }
    m_isSelected = false;

}


