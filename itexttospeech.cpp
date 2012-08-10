#include "itexttospeech.h"

#ifdef _WIN32
    const DWORD csSynchronousSpeechOptions = SPF_PURGEBEFORESPEAK;
    const DWORD csAsynchronousSpeechOptions = SPF_ASYNC | SPF_PURGEBEFORESPEAK;
//#elif
//    const DWORD csSynchronousSpeechOptions = 0;
//    const DWORD csAsynchronousSpeechOptions = 0;
#endif
