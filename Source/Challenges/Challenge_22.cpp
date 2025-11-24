#include <Challenges/Challenge_22.h>
#include <Helpers/FileHelpers.h>

#include <set>

////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_22::sm_inputFilePath = "Inputs/Input_Challenge_22.txt";




////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_22::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        m_SecretNumbers.push_back(stoll(line));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_22::Run_FirstPart()
{
    long long secretNumberSum = 0ll;

    for (const long long& secretNumber : m_SecretNumbers)
    {
        secretNumberSum += ComputNextSecretNumber(secretNumber, 2000);
    }

    cout << "The sum of the secret numbers after 2000 iterations is " << secretNumberSum << "." << endl;
    return EErrorCode::Success;
}

EErrorCode CChallenge_22::CleanUp_FirstPart()
{
    return EErrorCode::Success;
}

long long CChallenge_22::ComputNextSecretNumber(const long long secretNumber, const int count /* = 1 */)
{
    long long nextSecretNumber = secretNumber;
    for (int i = 0; i < count; ++i)
    {
        long long result = nextSecretNumber << 6; // Multiply by 64
        nextSecretNumber ^= result; // Bit-wise XOR
        nextSecretNumber &= 0xFFFFFF; // Keep only the 24 least significant bits (2^24 - 1)

        result = nextSecretNumber >> 5; // Divide by 32
        nextSecretNumber ^= result; // Bit-wise XOR
        nextSecretNumber &= 0xFFFFFF; // Keep only the 24 least significant bits (2^24 - 1)


        result = nextSecretNumber << 11; // Multiply by 2048
        nextSecretNumber ^= result; // Bit-wise XOR
        nextSecretNumber &= 0xFFFFFF; // Keep only the 24 least significant bits (2^24 - 1)
    }

    return nextSecretNumber;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_22::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        m_SecretNumbers.push_back(stoll(line));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_22::Run_SecondPart()
{
    vector<int> bestSequence{ 0, 0, 0, 0 };
    int bestSequenceTotal = 0;

    for (const long long& secretNumber : m_SecretNumbers)
    {
        long long previousNumber = secretNumber;
        vector<int> currentSequence { 0, 0, 0, 0 };

        set<vector<int>> m_SequencesSeenByCurrentBuyer;

        for (int i = 0; i < 2000; ++i)
        {
            long long currentNumber = ComputNextSecretNumber(previousNumber);
            int currentPrice = currentNumber % 10;
            int variation = currentPrice - (previousNumber % 10);

            UpdateSequenceWithVariation(currentSequence, variation);

            if (i > 2 && m_SequencesSeenByCurrentBuyer.find(currentSequence) == m_SequencesSeenByCurrentBuyer.end())
            {
                m_SequencesSeenByCurrentBuyer.insert(currentSequence);

                int& currentSequenceTotal = m_SequenceTotals[currentSequence];
                currentSequenceTotal += currentPrice;
                if (currentSequenceTotal > bestSequenceTotal)
                {
                    bestSequenceTotal = currentSequenceTotal;
                    bestSequence = currentSequence;
                }
            }

            previousNumber = currentNumber;
        }
        continue;
    }

    cout << "The best sequence is {" << bestSequence[0] << ", " << bestSequence[1] << ", " << bestSequence[2] << ", " << bestSequence[3] << "} with a total of " << bestSequenceTotal << " bananas." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_22::CleanUp_SecondPart()
{
    return EErrorCode::Success;
}

void CChallenge_22::UpdateSequenceWithVariation(vector<int>& currentSequence, int variation)
{
    currentSequence[0] = currentSequence[1];
    currentSequence[1] = currentSequence[2];
    currentSequence[2] = currentSequence[3];
    currentSequence[3] = variation;
}