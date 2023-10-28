#ifndef _SOUNDMANAGER_INCLUDE
#define _SOUNDMANAGER_INCLUDE

#include <irrKlang.h>
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

class SoundManager {
public:
	SoundManager();
	~SoundManager();

	void playBGM(const char* filename, bool playLooped);
	void stopBGM();
	void pauseBGM();
	void resumeBGM();
	void playSFX(const char* filename);


private:
	irrklang::ISoundEngine* soundEngine;
	irrklang::ISound* music;
	irrklang::ISound* sfx;
};

#endif // _SOUNDMANAGER_INCLUDE
