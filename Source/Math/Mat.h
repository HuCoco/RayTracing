#pragma once
#include <Platform/Platform.h>

template<typename _T, uint32_t nRow, uint32_t nCol>
class Mat
{


private:
    _T m_Data[nRow][nCol];
};