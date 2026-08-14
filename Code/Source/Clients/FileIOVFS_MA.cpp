#include "FileIOVFS_MA.h"

#include <AzCore/IO/FileIO.h>
#include <AzCore/Interface/Interface.h>

#include "Config_MA.h"

namespace AudioEngineMA::FileIO
{

struct AZIO_VFSData {
    AZ::IO::HandleType handle;
};

ma_result FileIO_Open(ma_vfs *pVFS, const char *pFilePath, ma_uint32 openMode, ma_vfs_file *pFile)
{
    if(!pFile)
    {
        return MA_INVALID_ARGS;
    }

    AZ_UNUSED(pVFS);
    AZ_UNUSED(openMode); //We don't support writing here.

    auto fileIO = AZ::IO::FileIOBase::GetInstance();
    const AZ::IO::FixedMaxPath audioAssetsPath = Config::AudioFilesPath;
    auto fullFilePath = AZ::IO::FixedMaxPath { audioAssetsPath / pFilePath }.Native();

    if(AZ::u64 fileSize = 0; fileIO->Size(fullFilePath.data(), fileSize) && fileSize != 0)
    {
        AZ::IO::HandleType fH = AZ::IO::InvalidHandle;
        if(fileIO->Open(fullFilePath.data(), AZ::IO::OpenMode::ModeRead | AZ::IO::OpenMode::ModeBinary, fH))
        {
            auto IOData = azcreate(AZIO_VFSData);
            IOData->handle = fH;
            *pFile = IOData;
            return MA_SUCCESS;
        }
    }

    return MA_INVALID_FILE;
}

ma_result FileIO_Close(ma_vfs *pVFS, ma_vfs_file file)
{
    AZ_UNUSED(pVFS);
    auto fileIO = AZ::IO::FileIOBase::GetInstance();
    auto iodata = reinterpret_cast<AZIO_VFSData*>(file);

    if(!iodata)
    {
        return MA_INVALID_FILE;
    }

    if(!fileIO->Close(iodata->handle))
    {
        return MA_INVALID_FILE;
    }

    return MA_SUCCESS;
}

ma_result FileIO_Read(ma_vfs *pVFS, ma_vfs_file file, void *pDst, size_t sizeInBytes, size_t *pBytesRead)
{
    AZ_UNUSED(pVFS);
    auto fileIO = AZ::IO::FileIOBase::GetInstance();
    auto iodata = reinterpret_cast<AZIO_VFSData*>(file);

    AZ::u64 bytesRead = 0;
    fileIO->Read(iodata->handle, pDst, aznumeric_cast<AZ::u64>(sizeInBytes), false, &bytesRead);

    if(bytesRead <= 0)
    {
        return MA_IO_ERROR;
    }
    *pBytesRead = aznumeric_cast<size_t>(bytesRead);
    return MA_SUCCESS;
}

ma_result FileIO_Seek(ma_vfs *pVFS, ma_vfs_file file, ma_int64 offset, ma_seek_origin origin)
{
    AZ_UNUSED(pVFS);
    auto fileIO = AZ::IO::FileIOBase::GetInstance();
    auto iodata = reinterpret_cast<AZIO_VFSData*>(file);

    AZ::IO::SeekType seekType;

    switch(origin)
    {
    case ma_seek_origin_current:
        seekType = AZ::IO::SeekType::SeekFromCurrent;
        break;
    case ma_seek_origin_start:
        seekType = AZ::IO::SeekType::SeekFromStart;
        break;
    case ma_seek_origin_end:
        seekType = AZ::IO::SeekType::SeekFromEnd;
        break;
    default:
        return MA_INVALID_ARGS;
    }

    if(!fileIO->Seek(iodata->handle, offset, seekType))
    {
        return MA_BAD_SEEK;
    }

    return MA_SUCCESS;
}

ma_result FileIO_Tell(ma_vfs *pVFS, ma_vfs_file file, ma_int64 *pCursor)
{
    AZ_UNUSED(pVFS);
    auto fileIO = AZ::IO::FileIOBase::GetInstance();
    auto iodata = reinterpret_cast<AZIO_VFSData*>(file);

    AZ::u64 offset = 0;
    if(!fileIO->Tell(iodata->handle, offset))
    {
        return MA_IO_ERROR;
    }

    *pCursor = aznumeric_cast<ma_int64>(offset);
    return MA_SUCCESS;
}

ma_result FileIO_Info(ma_vfs *pVFS, ma_vfs_file file, ma_file_info *pInfo)
{
    AZ_UNUSED(pVFS);
    auto fileIO = AZ::IO::FileIOBase::GetInstance();
    auto iodata = reinterpret_cast<AZIO_VFSData*>(file);

    AZ::u64 fileSize = 0;
    fileIO->Size(iodata->handle, fileSize);
    pInfo->sizeInBytes = aznumeric_cast<ma_uint64>(fileSize);
    return MA_SUCCESS;
}


}