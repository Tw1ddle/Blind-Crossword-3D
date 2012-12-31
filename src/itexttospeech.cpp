#include "itexttospeech.h"

#ifdef _WIN32
    const DWORD SPEECH_MODES::csDefaultSynchronousSpeechOptions = SPF_PURGEBEFORESPEAK;
    const DWORD SPEECH_MODES::csDefaultAsynchronousSpeechOptions = SPF_ASYNC | SPF_PURGEBEFORESPEAK;
    const DWORD SPEECH_MODES::csAsynchronousNoPurgeOptions = SPF_ASYNC;
    const DWORD SPEECH_MODES::csSpeakPunctuationOption = SPF_NLP_SPEAK_PUNC;
//#elif .......
//    const DWORD SPEECH_MODES::csSynchronousSpeechOptions = 0;
//    const DWORD SPEECH_MODES::csAsynchronousSpeechOptions = 0;
//    const DWORD SPEECH_MODES::csAsynchronousNoPurgeOptions = 0;
//    const DWORD SPEECH_MODES::csSpeakPunctuationOption = 0;
#endif
