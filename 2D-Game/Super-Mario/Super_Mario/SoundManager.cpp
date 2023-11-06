#include "SoundManager.h"
#include <fstream>
#include <iostream> 
using namespace irrklang;

SoundManager::SoundManager() {
	music = NULL;
	sfx = NULL;

	std::ofstream nullStream("/dev/null");
	std::streambuf* originalCoutStreamBuffer = std::cout.rdbuf();
	std::cout.rdbuf(nullStream.rdbuf());

	// start the sound engine with default parameters
	soundEngine = createIrrKlangDevice();

	std::cout.rdbuf(originalCoutStreamBuffer);
}

SoundManager::~SoundManager() {
	if (music != NULL) delete music;
	if (sfx != NULL) delete sfx;
	soundEngine->drop(); // delete engine
}

void SoundManager::playBGM(const char* filename, bool playLooped) {
	music = soundEngine->play2D(filename, playLooped, false, true, ESM_AUTO_DETECT, true);
}

void SoundManager::stopBGM() {
	if (music != NULL) music->stop();
}

void SoundManager::pauseBGM() {
	if (music != NULL) music->setIsPaused(true);
}

void SoundManager::resumeBGM() {
	if (music != NULL) music->setIsPaused(false);
}

void SoundManager::playSFX(const char* filename) {
	sfx = soundEngine->play2D(filename);
}