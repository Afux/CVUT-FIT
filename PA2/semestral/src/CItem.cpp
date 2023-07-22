
#include "CItem.h"
#include "filesystem"

namespace fs = std::filesystem;

CItem::CItem(std::string path, CItem *inFolder) : m_Path(path), m_InFolder(inFolder) {
    m_Name = fs::path(m_Path).filename();
    m_isSelected = false;
}

CItem::~CItem() {

}

void CItem::Select() {
    if (m_isSelected)
        m_isSelected = false;
    else
        m_isSelected = true;
}

bool CItem::IsReadable(const fs::path &p) {

    try {

        auto perms = fs::status(p).permissions();
        if ((perms & fs::perms::owner_read) != fs::perms::none && (perms & fs::perms::group_read) != fs::perms::none &&
            (fs::exists(p))) {
            return true;
        }
        return false;
    }
    catch (fs::filesystem_error &e) {
        return false;
    }

}

bool CItem::IsWriteable(const std::filesystem::path &p) {
    try {

        auto perms = fs::status(p).permissions();
        if ((perms & fs::perms::owner_write) != fs::perms::none &&
            (perms & fs::perms::group_write) != fs::perms::none && (fs::exists(p))) {
            return true;
        }
        return false;

    }
    catch (fs::filesystem_error &e) {
        return false;
    }
}

bool CItem::IsReadable() {
    return IsReadable(m_Path);
}

bool CItem::IsWriteable() {
    return IsWriteable(m_Path);
}





