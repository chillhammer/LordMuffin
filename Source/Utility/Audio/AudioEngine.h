#pragma once
#include <irrKlang.h>
#include <Camera/CameraComponent.h>

namespace Skel
{
	#define Audio AudioEngine::Instance()
	class AudioEngine
	{
	public:
		static AudioEngine& AudioEngine::Instance();
		AudioEngine();
		~AudioEngine();

		void PlaySound(std::string file);
		void PlaySound3D(std::string file, Vector3 position);
		void PlayMusic(std::string file);

		void SetAudioEnabled(bool enabled);
		bool GetAudioEnabled() const;
	private:
		CameraComponent* m_Camera;
		const std::string m_Directory = "../Assets/Sounds/";
		irrklang::ISoundEngine* m_Engine;
		irrklang::ISound* m_Music;
		bool m_Enabled;
	};
}