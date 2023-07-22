
#ifndef SEMESTRALWORK_CDIR_H
#define SEMESTRALWORK_CDIR_H

#include "CItem.h"

class CDir : public CItem {
public:
    CDir(std::string path, CItem *inFolder);


    void Open(std::map<std::string, std::shared_ptr<CItem>> **item, CItem **inFold) override;

    void Copy(const std::string &to) override;

    void Delete() override;

    void Move(const std::string &dest) override;

    void Deduplicate(CItem *DeduplicateMe) override;

    void Print() override;

    void FindText(const std::string &FindThis, std::vector<CItem *> *Found) override;

    void ConCat(const std::string &To) override;
    /**
     * @brief void FillItems()
     *  Insert items in this folder to map
     */
    void FillItems();



private:
    /**
    * @brief void Refresh()
    *  Update map with items
    */
    void Refresh();

};


#endif //SEMESTRALWORK_CDIR_H
