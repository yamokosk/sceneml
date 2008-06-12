/*************************************************************************
 * SceneML, Copyright (C) 2007, 2008  J.D. Yamokoski
 * All rights reserved.
 * Email: yamokosk at gmail dot com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the License, 
 * or (at your option) any later version. The text of the GNU Lesser General 
 * Public License is included with this library in the file LICENSE.TXT.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the file LICENSE.TXT for 
 * more details.
 *
 *************************************************************************/

//
#ifndef _SML_ERRORCODES_H_
#define _SML_ERRORCODES_H_

#define SMLERR_OK                       0
#define SMLERR_GENERIC                  1 // unspecified error
#define SMLERR_NoDevice                 2
#define SMLERR_SetPropertyFailed        3
#define SMLERR_LibaryFunctionNotFound   4
#define SMLERR_ModuleVersionMismatch    5
#define SMLERR_DeviceVersionMismatch    6
#define SMLERR_UnknownModule            7
#define SMLERR_LoadLibraryFailed        8
#define SMLERR_CreateFailed             9
#define SMLERR_CreateNotFound           10
#define SMLERR_DeleteNotFound           11
#define SMLERR_DeleteFailed             12
#define SMLERR_UnexpectedDevice         13
#define SMLERR_DeviceUnloadFailed       14
#define SMLERR_CameraNotAvailable       15
#define SMLERR_DuplicateLabel           16
#define SMLERR_InvalidLabel             17
#define SMLERR_InvalidStateDevice       19
#define SMLERR_NoConfiguration          20
#define SMLERR_InvalidConfigurationIndex 21
#define SMLERR_DEVICE_GENERIC           22
#define SMLERR_InvalidPropertyBlock     23
#define SMLERR_UnhandledException       24
#define SMLERR_DevicePollingTimeout     25
#define SMLERR_InvalidShutterDevice     26
#define SMLERR_InvalidSerialDevice      27
#define SMLERR_InvalidStageDevice       28
#define SMLERR_InvalidSpecificDevice    29
#define SMLERR_InvalidXYStageDevice     30
#define SMLERR_FileOpenFailed           31
#define SMLERR_InvalidCFGEntry          32
#define SMLERR_InvalidContents          33
#define SMLERR_InvalidCoreProperty      34
#define SMLERR_InvalidCoreValue         35
#define SMLERR_NoConfigGroup            36
#define SMLERR_CameraBufferReadFailed   37
#define SMLERR_DuplicateConfigGroup     38
#define SMLERR_InvalidConfigurationFile 39
#define SMLERR_CircularBufferFailedToInitialize 40
#define SMLERR_CircularBufferEmpty      41
#define SMLERR_ContFocusNotAvailable    42
#endif //_ERRORCODES_H_
