#pragma once

#include <miniaudio.h>

namespace AudioEngineMA
{
    namespace FileIO
    {
        ma_result FileIO_Open(ma_vfs* pVFS, const char* pFilePath, ma_uint32 openMode, ma_vfs_file* pFile);
        ma_result FileIO_Close(ma_vfs* pVFS, ma_vfs_file file);
        ma_result FileIO_Read(ma_vfs* pVFS, ma_vfs_file file, void* pDst, size_t sizeInBytes, size_t* pBytesRead);
        ma_result FileIO_Seek(ma_vfs* pVFS, ma_vfs_file file, ma_int64 offset, ma_seek_origin origin);
        ma_result FileIO_Tell(ma_vfs* pVFS, ma_vfs_file file, ma_int64* pCursor);
        ma_result FileIO_Info(ma_vfs* pVFS, ma_vfs_file file, ma_file_info* pInfo);
    }
}