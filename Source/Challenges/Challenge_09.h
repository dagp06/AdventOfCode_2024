#pragma once

#include <Challenges/Common/Challenge.h>

using namespace std;

class CChallenge_09 : public CChallenge
{
private:
    struct File
    {
        File() : m_ID(0), m_Size(0), m_FreeSpaceAfter(0) {}
        File(int _id, int _size, int _freeSpaceAfter = 0) : m_ID(_id), m_Size(_size), m_FreeSpaceAfter(_freeSpaceAfter) {}
        int m_ID;
        int m_Size;
        int m_FreeSpaceAfter;

        File* m_PreviousFile = nullptr;
        File* m_NextFile = nullptr;
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

#ifdef _DEBUG
    void PrintChecksum() const;
#endif

    static string const sm_inputFilePath;

    typedef vector<File> DiskMap;
    DiskMap m_DiskMap;
};
