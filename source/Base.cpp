//*************************************************************************************************
//
// PROJECT:             Storage Class Library (SCL)
// FILE:                Base
// SUBSYSTEM:           Base Class for Storage Class Library
// TARGET OS:           All
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2010, 2013-2015 Gavin Blakeman.
//                      This file is part of the Storage Class Library (SCL)
//
//                      SCL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
//                      License as published by the Free Software Foundation, either version 2 of the License, or (at your option)
//                      any later version.
//
//                      SCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
//                      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//                      more details.
//
//                      You should have received a copy of the GNU General Public License along with SCL.  If not, see
//                      <http://www.gnu.org/licenses/>.
//
// OVERVIEW:            Provides a group of storage class implementations.
//
// HISTORY:             2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-20 GGB - AIRDAS 0000.00 release.
//                      2010-04-28 GGB - Unscrabble Application Release 1.00
//
//*************************************************************************************************

#include "../Include/Base.h"

namespace SCL
{

	// Default Constructor for the ContainerBase class.
	//
  // 2010-04-20/GGB - Function created.

  CContainer::CContainer()
	{
		lPackageCount = 0;
		bOwnsPackages = true;
  }



}
