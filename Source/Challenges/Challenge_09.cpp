#include "Challenge_09.h"
#include "../Helpers/FileHelpers.h"

#include <list>

////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_09::sm_inputFilePath = "Inputs/Input_Challenge_09.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_09::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    const string& line = lines[0];
    int id = 0;
    for (int i = 0; i < line.size(); i += 2)
    {
        m_DiskMap.push_back(File(id++, line[i] - '0', (i + 1 < line.size()) ? line[i + 1] - '0' : 0));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_09::Run_FirstPart()
{
    vector<File> compressedDiskMap;

    for (int i = 0; i < m_DiskMap.size(); ++i)
    {
        File& file = m_DiskMap[i];
        compressedDiskMap.push_back(File(file.m_ID, file.m_Size));

        if (i == m_DiskMap.size() - 1)
            break;

        int remainingSpace = file.m_FreeSpaceAfter;
        while (remainingSpace > 0)
        {
            File& lastFile = m_DiskMap.back();

            if (lastFile.m_Size < remainingSpace)
            {
                compressedDiskMap.push_back(File(lastFile.m_ID, lastFile.m_Size));
                remainingSpace -= lastFile.m_Size;
                m_DiskMap.resize(m_DiskMap.size() - 1);
            }
            else
            {
                compressedDiskMap.push_back(File(lastFile.m_ID, remainingSpace));

                if (lastFile.m_Size == remainingSpace)
                    m_DiskMap.resize(m_DiskMap.size() - 1);
                else
                    lastFile.m_Size -= remainingSpace;

                remainingSpace = 0;
            }
        }
    }

    // Merge last 2 files if necessary
    if (compressedDiskMap[compressedDiskMap.size() - 2].m_ID == compressedDiskMap[compressedDiskMap.size() - 1].m_ID)
    {
        compressedDiskMap[compressedDiskMap.size() - 2].m_Size += compressedDiskMap[compressedDiskMap.size() - 1].m_Size;
        compressedDiskMap.resize(compressedDiskMap.size() - 1);
    }

    long long checksum = 0ll;
    int blockPos = 0;
    for (int i = 0; i < compressedDiskMap.size(); ++i)
    {
        const File& file = compressedDiskMap[i];
        for (int j = 0; j < file.m_Size; ++j)
        {
            checksum += blockPos++ * file.m_ID;
        }
    }

    cout << "the resulting filesystem checksum is " << checksum << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_09::CleanUp_FirstPart()
{
    m_DiskMap.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_09::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    const string& line = lines[0];
    int id = 0;
    for (int i = 0; i < line.size(); i += 2)
    {
        m_DiskMap.push_back(File(id++, line[i] - '0', (i + 1 < line.size()) ? line[i + 1] - '0' : 0));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_09::Run_SecondPart()
{
    for (int i = 0; i < m_DiskMap.size() - 1; ++i)
    {
        m_DiskMap[i].m_NextFile = &m_DiskMap[i + 1];
        m_DiskMap[i + 1].m_PreviousFile = &m_DiskMap[i];
    }

#ifdef _DEBUG
    PrintChecksum();
    cout << "STARTING" << endl;
#endif

    for (DiskMap::reverse_iterator rIt = m_DiskMap.rbegin(); rIt != m_DiskMap.rend(); ++rIt)
    {
        File& fileToMove = *rIt;

        File* currentFile = &m_DiskMap[0];
        while (currentFile != nullptr)
        {
            if (currentFile->m_ID == fileToMove.m_ID)
                break;

            if (currentFile->m_FreeSpaceAfter >= fileToMove.m_Size)
            {
                if (fileToMove.m_PreviousFile == currentFile)
                {
                    // Simply move fileToMove right after its left neighboor (currentFile), remove the free space between them
                    fileToMove.m_FreeSpaceAfter += currentFile->m_FreeSpaceAfter;
                    currentFile->m_FreeSpaceAfter = 0;
                }
                else
                {
                    // Move fileToMove after currentFile
                    fileToMove.m_PreviousFile->m_FreeSpaceAfter += fileToMove.m_Size + fileToMove.m_FreeSpaceAfter;
                    fileToMove.m_FreeSpaceAfter = currentFile->m_FreeSpaceAfter - fileToMove.m_Size;
                    currentFile->m_FreeSpaceAfter = 0;

                    // Update links between elements of the list
                    File* newNextFile = currentFile->m_NextFile;
                    currentFile->m_NextFile = &fileToMove;
                    newNextFile->m_PreviousFile = &fileToMove;

                    if (fileToMove.m_NextFile)
                        fileToMove.m_NextFile->m_PreviousFile = fileToMove.m_PreviousFile;
                    fileToMove.m_PreviousFile->m_NextFile = fileToMove.m_NextFile;

                    fileToMove.m_PreviousFile = currentFile;
                    fileToMove.m_NextFile = newNextFile;
                }
#ifdef _DEBUG
                PrintChecksum();
#endif
                break;
            }

            currentFile = currentFile->m_NextFile;
        }
    }

    // Compute checksum
    long long checksum = 0ll;
    int blockPos = 0;
    File* currentFile = &m_DiskMap[0];
    while (currentFile != nullptr)
    {
        for (int i = 0; i < currentFile->m_Size; ++i)
        {
            checksum += blockPos++ * currentFile->m_ID;
        }

        blockPos += currentFile->m_FreeSpaceAfter;
        currentFile = currentFile->m_NextFile;
    }

    cout << "The resulting filesystem checksum is " << checksum << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_09::CleanUp_SecondPart()
{
    m_DiskMap.clear();
    return EErrorCode::Success;
}

#ifdef _DEBUG
void CChallenge_09::PrintChecksum() const
{
    const File* currentFile = &m_DiskMap[0];
    while (currentFile != nullptr)
    {
        for (int i = 0; i < currentFile->m_Size; ++i)
        {
            cout << currentFile->m_ID;
        }

        for (int i = 0; i < currentFile->m_FreeSpaceAfter; ++i)
        {
            cout << '.';
        }

        currentFile = currentFile->m_NextFile;
    }

    cout << endl << endl;
}
#endif
