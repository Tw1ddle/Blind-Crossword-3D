#ifdef NO_TTS

#include "ttsstub.h"

const DWORD SPEECH_OPTIONS::csDefaultSynchronousSpeechOptions = 0;
const DWORD SPEECH_OPTIONS::csDefaultAsynchronousSpeechOptions = 1;
const DWORD SPEECH_OPTIONS::csSpeakPunctuationOption = 2;

TTSStub instance;

TTSStub::TTSStub()
{

}

TTSStub::~TTSStub()
{

}

#endif
