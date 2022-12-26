#pragma once

#define AUDIO_GAME_START "Pacman-Clone-master/Resources/Sound/game_start.wav"
#define AUDIO_EAT_GHOST "Pacman-Clone-master/Resources/Sound/eat_ghost.wav"
#define AUDIO_DEATH_1 "Pacman-Clone-master/Resources/Sound/death_1.wav"
#define AUDIO_POWER_SNACK "Pacman-Clone-master/Resources/Sound/power_pellet.wav"
#define AUDIO_MUNCH "Pacman-Clone-master/Resources/Sound/munch.wav"
#define AUDIO_SIREN "Pacman-Clone-master/Resources/Sound/siren_1.wav"
#define AUDIO_RETREATING "Pacman-Clone-master/Resources/Sound/retreating.wav"

#define VOLUME 50
#define VOLUME_MUNCH 10
#define VOLUME_SIREN 60

enum class Sounds {
	None,
	PowerSnack,
	EatGhost,
	Death,
	Munch,
	GameStart,
	Siren,
	Retreating
};