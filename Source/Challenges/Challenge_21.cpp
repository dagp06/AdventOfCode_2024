#include <Challenges/Challenge_21.h>
#include <Helpers/FileHelpers.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_21::sm_inputFilePath = "Inputs/Input_Challenge_21.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_21::SetUp_FirstPart()
{
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, m_Codes);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_21::Run_FirstPart()
{
    InitializeKeyPads();
    InitializeCharNodes();

    int robotCount = 3;
    int complexitySum = ComputeComplexitySum(robotCount);

    cout << "The sum of the complexities of all codes is " << complexitySum << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_21::CleanUp_FirstPart()
{
    m_Codes.clear();
    m_DirectionalKeypad.Clear();
    m_NumericKeypad.Clear();
    m_AllCharNodes.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_21::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }
    
    return EErrorCode::NotImplemented;
}

EErrorCode CChallenge_21::Run_SecondPart()
{
    return EErrorCode::NotImplemented;
}

EErrorCode CChallenge_21::CleanUp_SecondPart()
{
    return EErrorCode::NotImplemented;
}

void CChallenge_21::InitializeKeyPads()
{
    m_DirectionalKeypad.AddKey('^', { 1, 0 });
    m_DirectionalKeypad.AddKey('A', { 2, 0 });
    m_DirectionalKeypad.AddKey('<', { 0, 1 });
    m_DirectionalKeypad.AddKey('v', { 1, 1 });
    m_DirectionalKeypad.AddKey('>', { 2, 1 });

    m_NumericKeypad.AddKey('7', { 0, 0 });
    m_NumericKeypad.AddKey('8', { 1, 0 });
    m_NumericKeypad.AddKey('9', { 2, 0 });
    m_NumericKeypad.AddKey('4', { 0, 1 });
    m_NumericKeypad.AddKey('5', { 1, 1 });
    m_NumericKeypad.AddKey('6', { 2, 1 });
    m_NumericKeypad.AddKey('1', { 0, 2 });
    m_NumericKeypad.AddKey('2', { 1, 2 });
    m_NumericKeypad.AddKey('3', { 2, 2 });
    m_NumericKeypad.AddKey('0', { 1, 3 });
    m_NumericKeypad.AddKey('A', { 2, 3 });
}

void CChallenge_21::InitializeCharNodes()
{
    vector<Keypad*> keypads;
    keypads.push_back(&m_DirectionalKeypad);
    keypads.push_back(&m_NumericKeypad);

    for (Keypad* keypad : keypads)
    {
        set<CharNode>& charNodes = m_AllCharNodes[keypad];

        for (const pair<Vector2, char>& posKeyStart : keypad->m_PosToKey)
        {
            for (const pair<Vector2, char>& posKeyEnd : keypad->m_PosToKey)
            {
                CharNode newCharNode;
                newCharNode.start = posKeyStart.second;
                newCharNode.end = posKeyEnd.second;
                keypad->ExtractAllPossibleSequences(posKeyStart.first, posKeyEnd.first, "", newCharNode.possibleSequences);
                charNodes.insert(move(newCharNode));
            }
        }
    }
}

int CChallenge_21::ComputeComplexitySum(const int& _robotCount)
{
    int _complexitySum = 0;

    for (const string& code : m_Codes)
    {
#ifdef _DEBUG
        cout << "Code: " << code << endl;
#endif // _DEBUG

        Keypad* currentKeypad = &m_NumericKeypad;

        CodeNode initialCodeNode(code, nullptr);
        vector<CodeNode*> nextCodeNodes;
        nextCodeNodes.push_back(&initialCodeNode);

        vector<CodeNode*> currentCodeNodes;
        for (int i = 0; i < _robotCount; i++)
        {
            currentCodeNodes.clear();
            swap(nextCodeNodes, currentCodeNodes);

#ifdef _DEBUG
            cout << "AGENT " << i << " - " << currentCodeNodes.size() << " codes" << endl;
#endif // _DEBUG

            for (CodeNode* currentCodeNodePtr : currentCodeNodes)
            {
                CodeNode& currentCodeNode = *currentCodeNodePtr;
                const string& currentCode = currentCodeNode.code;

#ifdef _DEBUG
                cout << "\tCode " << currentCode << endl;
#endif // _DEBUG

                currentCodeNode.childrenCodeNodes.resize(currentCode.size());
                for (int codeIdx = 0; codeIdx < currentCode.size(); ++codeIdx)
                {
                    // Find sequences from previous key to current key
                    char start = codeIdx == 0 ? 'A' : currentCode[codeIdx - 1];
                    char end = currentCode[codeIdx];
                    CharNode charNode;
                    FindCharNode(currentKeypad, start, end, charNode);

                    // Create new code nodes from all possible sequences
                    vector<CodeNode*>& childrenCodeNodes = currentCodeNode.childrenCodeNodes[codeIdx];
                    for (string& sequence : charNode.possibleSequences)
                    {
                        childrenCodeNodes.push_back(new CodeNode(move(sequence), currentCodeNodePtr));
                        nextCodeNodes.push_back(childrenCodeNodes.back());
                    }

#ifdef _DEBUG
                    cout << "\t[" << codeIdx << "] - [" << start << end << "] : ";
                    for (const string& sequence : charNode.possibleSequences)
                    {
                        cout << "[" << sequence << "] ";
                    }
                    cout << endl;
#endif // _DEBUG
                }
            }

            currentKeypad = &m_DirectionalKeypad;
        }

        string shortestSequence;
        FindShortestSequence(initialCodeNode, shortestSequence);

        int numericCode = stoi(code.substr(0, code.size() - 1));
        _complexitySum += (int)shortestSequence.size() * numericCode;

#ifdef _DEBUG
        cout << "Complexity = " << shortestSequence.size() << " * " << numericCode << endl;

        cout << endl;
        string currentSequence = shortestSequence;
        cout << "\tValidating end sequence " << currentSequence << endl;
        for (int i = 0; i < _robotCount; i++)
        {
            string outSequence;
            currentKeypad = (i == _robotCount - 1) ? &m_NumericKeypad : &m_DirectionalKeypad;
            ExecuteSequence(*currentKeypad, currentSequence, outSequence);
            cout << "\t  " << i << " - " << outSequence << endl;

            currentSequence = move(outSequence);
        }
        cout << endl;
#endif // _DEBUG
    }

    return _complexitySum;
}

void CChallenge_21::FindCharNode(const Keypad* _keypad, const char& _start, const char& _end, CharNode& _outCharNode) const
{
    const set<CharNode>& charNodes = m_AllCharNodes.at(_keypad);

    CharNode searchNode;
    searchNode.start = _start;
    searchNode.end = _end;
    set<CharNode>::const_iterator it = charNodes.find(searchNode);
    if (it != charNodes.end())
    {
        _outCharNode = *it;
    }
}

void CChallenge_21::FindShortestSequence(const CodeNode& _codeNode, string& _outShortestSequence) const
{
    if (_codeNode.childrenCodeNodes.empty())
    {
        _outShortestSequence = _codeNode.code;
        return;
    }

    for (const vector<CodeNode*>& childrenCodeNodes : _codeNode.childrenCodeNodes)
    {
        string childNodeShortestSequence;
        for (const CodeNode* codeNode : childrenCodeNodes)
        {
            string sequence;
            FindShortestSequence(*codeNode, sequence);
            if (childNodeShortestSequence.empty() || sequence.size() < childNodeShortestSequence.size())
            {
                childNodeShortestSequence = sequence;
            }
        }
        _outShortestSequence.append(childNodeShortestSequence);
    }
}

void CChallenge_21::ExecuteSequence(const Keypad& _keyPad, const string& _sequence, string& _outSequence) const
{
    Vector2 currentPos = _keyPad.GetKeyPos('A');

    for (const char& targetKey : _sequence)
    {
        switch (targetKey)
        {
            case '<':
                --currentPos.x;
                break;
            case '>':
                ++currentPos.x;
                break;
            case '^':
                --currentPos.y;
                break;
            case 'v':
                ++currentPos.y;
                break;
            case 'A':
                _outSequence += _keyPad.GetPosKey(currentPos);
                break;
            default:
                break;
        }
    }
}

void CChallenge_21::Keypad::ExtractAllPossibleSequences(const Vector2& _start, const Vector2& _end, string currentSequence, vector<string>& _outSequences) const
{
    if (_start == _end)
    {
        currentSequence += 'A';
        _outSequences.push_back(currentSequence);
        return;
    }

    Vector2 toKey = _end - _start;

    if (toKey.x > 0)
    {
        Vector2 newKeyPos = _start + Vector2(1, 0);
        if (IsPosValid(newKeyPos))
        {
            string newSequence = currentSequence;
            newSequence.push_back('>');
            ExtractAllPossibleSequences(newKeyPos, _end, newSequence, _outSequences);
        }
    }
    else if (toKey.x < 0)
    {
        Vector2 newKeyPos = _start - Vector2(1, 0);
        if (IsPosValid(newKeyPos))
        {
            string newSequence = currentSequence;
            newSequence.push_back('<');
            ExtractAllPossibleSequences(newKeyPos, _end, newSequence, _outSequences);
        }
    }

    if (toKey.y > 0)
    {
        Vector2 newKeyPos = _start + Vector2(0, 1);
        if (IsPosValid(newKeyPos))
        {
            string newSequence = currentSequence;
            newSequence.push_back('v');
            ExtractAllPossibleSequences(newKeyPos, _end, newSequence, _outSequences);
        }
    }
    else if (toKey.y < 0)
    {
        Vector2 newKeyPos = _start - Vector2(0, 1);
        if (IsPosValid(newKeyPos))
        {
            string newSequence = currentSequence;
            newSequence.push_back('^');
            ExtractAllPossibleSequences(newKeyPos, _end, newSequence, _outSequences);
        }
    }
}
