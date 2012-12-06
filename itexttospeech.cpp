#include "itexttospeech.h"

#ifdef _WIN32
    const DWORD csDefaultSynchronousSpeechOptions = SPF_PURGEBEFORESPEAK;
    const DWORD csDefaultAsynchronousSpeechOptions = SPF_ASYNC | SPF_PURGEBEFORESPEAK;
    const DWORD csAsynchronousNoPurgeOptions = SPF_ASYNC;
    const DWORD csSpeakPunctuationOption = SPF_NLP_SPEAK_PUNC;
//#elif .......
//    const DWORD csSynchronousSpeechOptions = 0;
//    const DWORD csAsynchronousSpeechOptions = 0;
//    const DWORD csAsynchronousNoPurgeOptions = 0;
//    const DWORD csSpeakPunctuationOption = 0;
#endif
