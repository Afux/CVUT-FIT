

#ifndef SEMESTRALWORK_CWINDOW_H
#define SEMESTRALWORK_CWINDOW_H

#include "iostream"
#include "CSize.h"
#include "CItem.h"
#include "CAbsWidnow.h"
#include "CDir.h"
#include "unordered_map"
/**
* @brief class CAbsWindow
*
* This class represents one window
*
*/
class CWindow : public CAbsWidnow {
public:
    /**
     * @brief CAbsWindow constructor
     * @param size represent size of a window
     * @param path represent path of a starting directory
     *
     */
    CWindow(CSize size, std::string path);

    /**
     * @brief std::map<std::string, std::shared_ptr<CItem>> m_Items
     * pointer to a current directory items
     */
    std::map<std::string, std::shared_ptr<CItem>> *m_Items;

    /**
     * @brief  std::map<std::string, std::shared_ptr<CItem>> m_Selecteditems
     * represents selected items for Concat func.
     */
    std::map<std::string, std::shared_ptr<CItem>> m_Selecteditems;


    CDir m_Dir;

    /**
    * @brief  std::map<std::string, std::shared_ptr<CItem>>::iterator m_FromItem
    * iterator that says from which item should window display
    */
    std::map<std::string, std::shared_ptr<CItem>>::iterator m_FromItem;
    /**
     * @brief   CItem *m_CurrFile
     * Represents current directory
     */
    CItem *m_CurrFile;

    void Print() override;

    void ReadKey() override;

    void Run();

    void Enter();
    /**
    * @brief void Jump(const std::string &to)
    * Func. that opens directory. "Multilevel jump"
    * @param to path of a directory
    */
    void Jump(const std::string &to);

private:
    void PrintBorders();
};


#endif //SEMESTRALWORK_CWINDOW_H
