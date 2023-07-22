//
// Created by afu on 05.06.23.
//

#ifndef SEMESTRALWORK_CMANAGEROPS_H
#define SEMESTRALWORK_CMANAGEROPS_H

#include "iostream"
#include "map"
#include "memory"
#include "CItem.h"


class CManagerOPs {
public:
    void Copy(CItem *item, const std::string &to);

    void Copy(const std::string &reg, const std::string &to, CItem *CurrDir);

    void Delete(CItem *item);

    void Delete(const std::string &reg, CItem *CurrDir);

    void Move(CItem *item, const std::string &dest);

    void Move(const std::string &reg, const std::string &to, CItem *CurrDir);

    void CreateFolder(const std::string &name, CItem *CurrDir);

    void CreateFile(const std::string &name, CItem *CurrDir);

    void CreateLink(const std::string &name, CItem *to, CItem *CurrDir);

    void FindByText(const std::string &text, CItem *CurrDir);

    void Deduplicate(CItem *item, CItem *CurrDir);

    void ConcatFiles(std::map<std::string, std::shared_ptr<CItem>> *Items, const std::string &to);
};


#endif //SEMESTRALWORK_CMANAGEROPS_H
