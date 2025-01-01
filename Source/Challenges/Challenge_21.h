#pragma once

#include <Challenges/Common/Challenge.h>
#include <Helpers/Vector2.h>

#include <set>
#include <map>

using namespace std;

class CChallenge_21 : public CChallenge
{
private:
    struct Keypad
    {
        void AddKey(const char& _key, const Vector2& _pos)
        {
            m_KeyToPos[_key] = _pos;
            m_PosToKey[_pos] = _key;
        }

        void Clear()
        {
            m_KeyToPos.clear();
            m_PosToKey.clear();
        }

        Vector2 GetKeyPos(const char& _key) const
        {
            return m_KeyToPos.at(_key);
        }

        char GetPosKey(const Vector2& _pos) const
        {
            return m_PosToKey.at(_pos);
        }

        bool GetPosKeyIfExists(const Vector2& _pos, char& _outKey) const
        {
            map<Vector2, char>::const_iterator it = m_PosToKey.find(_pos);
            if (it == m_PosToKey.end())
                return false;

            _outKey = it->second;
            return true;
        }

        bool IsPosValid(const Vector2& _pos) const
        {
            return m_PosToKey.find(_pos) != m_PosToKey.end();
        }

        Vector2 GetKeyPosDiff(const char& _sourceKey, const char& _targetKey) const
        {
            return GetKeyPos(_targetKey) - GetKeyPos(_sourceKey);
        }

        void ExtractAllPossibleSequences(const Vector2& _start, const Vector2& _end, string _currentSequence, vector<string>& _outSequences) const;

        map<char, Vector2> m_KeyToPos;
        map<Vector2, char> m_PosToKey;
    };

    struct CharNode
    {
        bool operator<(const CharNode& _other) const
        {
            if (start == _other.start)
            {
                return end < _other.end;
            }

            return start < _other.start;
        }

        char start;
        char end;
        vector<string> possibleSequences;
    };

    struct CodeNode
    {
        CodeNode(const string& _code, const CodeNode* _parentNode) : code(_code), parentNode(_parentNode) {}

        ~CodeNode()
        {
            for (vector<CodeNode*>& codeNodes : childrenCodeNodes)
            {
                for (CodeNode* codeNodePtr : codeNodes)
                {
                    delete codeNodePtr;
                }
            }
        }

        string code;

        const CodeNode* parentNode;
        vector<vector<CodeNode*>> childrenCodeNodes;
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    void InitializeKeyPads();
    void InitializeCharNodes();
    int ComputeComplexitySum(const int& robotCount);
    void FindCharNode(const Keypad* _keypad, const char& _start, const char& _end, CharNode& _outCharNode) const;
    void FindShortestSequence(const CodeNode& _initialCodeNode, string& _outShortestSequence) const;
    void ExecuteSequence(const Keypad& _keyPad, const string& _sequence, string& _outSequence) const;

    static string const sm_inputFilePath;

    vector<string> m_Codes;
    Keypad m_DirectionalKeypad;
    Keypad m_NumericKeypad;
    map<const Keypad*, set<CharNode>> m_AllCharNodes;
};
