
#ifndef SEMESTRALWORK_CSIZE_H
#define SEMESTRALWORK_CSIZE_H


class CSize {
public:
    CSize(unsigned int Width, unsigned int Height, int PosX, int PosY, int AbsPosY);

    CSize(CSize &rhs);

    unsigned int m_Width;
    unsigned int m_Height;
    int m_PosX;
    int m_PosY;
    int m_AbsPosY;
};


#endif //SEMESTRALWORK_CSIZE_H
