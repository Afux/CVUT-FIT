

#ifndef SEMESTRALWORK_CABSWIDNOW_H
#define SEMESTRALWORK_CABSWIDNOW_H

#include "iostream"
#include "CSize.h"
#include "CItem.h"
#include "CDir.h"
#include "CFile.h"
#include "CLink.h"
#include <memory>
#include "CManagerOPs.h"
#include "termios.h"
/**
 * @brief class CAbsWindow
 *
 * This class is parent of all UI classes int his project
 *
 */
class CAbsWidnow {
public:

    CAbsWidnow(CSize size, CAbsWidnow *Scene);

    virtual ~CAbsWidnow();

    /**
     * @brief Print func. Prints the object
     */
    virtual void Print() = 0;

    /**
     * @brief ReadKey func. Reads user input
     */
    virtual void ReadKey() = 0;

    CSize m_Size;
    /**
     * @brief Var. m_Selected, represents selected index of a class container
     */
    unsigned int m_Selected;
    /**
     * @brief Var. m_Scene, represents scene to display
     */
    CAbsWidnow *m_Scene;
    /**
    * @brief Var. m_OperMan, represents class CManagerOPs,to call operation on a item
    */
    CManagerOPs m_OperMan;
    /**
     * @brief Var. m_Iter, represents iterator for container of items
     */
    std::map<std::string, std::shared_ptr<CItem>>::iterator m_Iter;
    /**
     * @brief Var. m_Iter, represents iterator for container of items
     */
    bool mIsrunning;




protected:
    /**
     * @brief Func. cursorOff, makes cursor invisible
     */
    void cursorOff();
    /**
     * @brief Func. cursorOn, makes cursor visible
     */
    void cursorOn();
    /**
     * @brief Func. clear, call system func "clear"
     */
    int clear() const;
    /**
     * @brief Func. moveto, sets cursor to coordinates (x,y)
     * @param x sets x coordinate for a cursor
     * @param y sets y coordinate for a cursor
     */
    void moveto(int x = 1, int y = 1) const;

private:
    termios term;

};


#endif //SEMESTRALWORK_CABSWIDNOW_H
