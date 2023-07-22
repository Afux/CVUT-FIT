
#include "CSize.h"

CSize::CSize(unsigned int Width, unsigned int Height, int PosX, int PosY, int AbsPosY) {
    m_Width = Width;
    m_Height = Height;
    m_PosX = PosX;
    m_PosY = PosY;
    m_AbsPosY = AbsPosY;
}

CSize::CSize(CSize &rhs) {
    m_Width = rhs.m_Width;
    m_Height = rhs.m_Height;
    m_PosX = rhs.m_PosX;
    m_PosY = rhs.m_PosY;
    m_AbsPosY = rhs.m_AbsPosY;
}
