#pragma once
#include "pch.h"

class Game;

class AudioSystem
{
public:
	AudioSystem(Game* game);
	~AudioSystem();
	bool Initialize();
	void Update();
	bool SetAudioFile(const std::wstring& audioPath);
	void PlaySE(const std::wstring& audioPath, float volume, float pitch = 0.0f, float pan = 0.0f, bool loop = false);
	void PlayBGM(const std::wstring& audioPath, float volume, float pitch = 0.0f, float pan = 0.0f, bool loop = false);
	void StopBGM(const std::wstring& audioPath);
	void ResumeBGM(const std::wstring& audioPath);
	
	DirectX::AudioEngine* GetAudioEngine() const { return m_pAudioEngine.get(); }
private:
	Game* m_pGame = nullptr;
	std::unique_ptr<DirectX::AudioEngine> m_pAudioEngine = nullptr;
	std::unordered_map<uint32_t, std::unique_ptr<DirectX::SoundEffect>> m_pSoundEffects;
	std::vector<std::shared_ptr<DirectX::SoundEffectInstance>> m_pSEEffectInsts;
	std::unordered_map<uint32_t, std::shared_ptr<DirectX::SoundEffectInstance>> m_pBGMEffectInsts;
};