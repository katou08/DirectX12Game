#include "System/AudioSystem.h"

AudioSystem::AudioSystem(Game* game) : m_pGame(game)
{
}

AudioSystem::~AudioSystem()
{
}

bool AudioSystem::Initialize()
{
	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;
#ifdef _DEBUG
	eflags |= DirectX::AudioEngine_Debug;
#endif
	m_pAudioEngine = std::make_unique<DirectX::AudioEngine>(eflags);
	if (!m_pAudioEngine)
		return false;

	return true;
}

void AudioSystem::Update()
{
	if (!m_pAudioEngine->Update())
	{
		if (m_pAudioEngine->IsCriticalError())
			ThrowFailed("オーディオエンジンのエラー");
	}
}

bool AudioSystem::SetAudioFile(const std::wstring& audioPath)
{
	uint32_t index = std::hash<std::wstring>()(audioPath);
	m_pSoundEffects.emplace(index, std::make_unique<DirectX::SoundEffect>(m_pAudioEngine.get(), audioPath.c_str()));
	if (!m_pSoundEffects.at(index))
		return false;

	return true;
}

void AudioSystem::PlaySE(const std::wstring& audioPath, float volume, float pitch, float pan, bool loop)
{
	uint32_t index = std::hash<std::wstring>()(audioPath);
	m_pSEEffectInsts.push_back(m_pSoundEffects.at(index)->CreateInstance());
	
	m_pSEEffectInsts.back()->SetVolume(volume);
	m_pSEEffectInsts.back()->SetPitch(volume);
	m_pSEEffectInsts.back()->SetPan(pan);

	if (m_pSEEffectInsts.back())
	{
		m_pSEEffectInsts.back()->Play(loop);
	}
}

void AudioSystem::PlayBGM(const std::wstring& audioPath, float volume, float pitch, float pan, bool loop)
{
	uint32_t index = std::hash<std::wstring>()(audioPath);
	m_pBGMEffectInsts.emplace(index, m_pSoundEffects.at(index)->CreateInstance());

	m_pBGMEffectInsts.at(index)->SetVolume(volume);
	m_pBGMEffectInsts.at(index)->SetPitch(volume);
	m_pBGMEffectInsts.at(index)->SetPan(pan);

	if (m_pBGMEffectInsts.at(index))
	{
		m_pBGMEffectInsts.at(index)->Play(loop);
	}
}

void AudioSystem::StopBGM(const std::wstring& audioPath)
{
	uint32_t index = std::hash<std::wstring>()(audioPath);
	if (m_pBGMEffectInsts.at(index))
	{
		m_pBGMEffectInsts.at(index)->Stop();
	}
}

void AudioSystem::ResumeBGM(const std::wstring& audioPath)
{
	uint32_t index = std::hash<std::wstring>()(audioPath);
	if (m_pBGMEffectInsts.at(index))
	{
		m_pBGMEffectInsts.at(index)->Play(true);
	}
}
