/*
*                                                                          
* DrawSpace Rendering engine                                               
* Emmanuel Chaumont Copyright (c) 2013-2017                        
*                                                                          
* This file is part of DrawSpace.                                          
*                                                                          
*    DrawSpace is free software: you can redistribute it and/or modify     
*    it under the terms of the GNU General Public License as published by  
*    the Free Software Foundation, either version 3 of the License, or     
*    (at your option) any later version.                                   
*                                                                          
*    DrawSpace is distributed in the hope that it will be useful,          
*    but WITHOUT ANY WARRANTY; without even the implied warranty of        
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
*    GNU General Public License for more details.                          
*                                                                          
*    You should have received a copy of the GNU General Public License     
*    along with DrawSpace.  If not, see <http://www.gnu.org/licenses/>.    
*
*/

#ifndef _MUTEX_H_
#define _MUTEX_H_

#include "drawspace_commons.h"

namespace DrawSpace
{
namespace Utils
{
class Mutex
{
protected:
    HANDLE m_hmutex;

public:
    Mutex( void );
    ~Mutex( void );

    void WaitInfinite( void );    // blocking
    bool Wait( long p_interval );  // blocking with timer or nonblocking if timer = 0
    void Release( void );
};
}
}

#endif
