# MiniAudio implementation for O3DE's ATL/AudioSystem

Basic implementation for O3DE's AudioSystem Gem using MiniAudio, aims to implement a basic AudioEngine implementation to be usable in smaller projects that don't need full blown solutions, or licensing burdens such as FMOD Studio and Wwise. Note this project do not intend to become an full FMOD or Wwise replacement

It reuses the same MiniAudio and audio decoder libraries uses in O3DE, so no external dependencies will need to be set to use this gem.

## Early release current features:

* Standard audio playback + Spatialization supported
* Listeners up to 4 (max supported by miniaudio)
* Audio controls with tweakable parameters for Audio Files, Audio File RTPCs, and Global RTPCs (⚠️ RTPC untested!)
* Basic RTPCs: Volume (Per object, per audio file instance or global), and Pitch (Per Object, audio file instance).

## Current bugs:
* .ogg files are not supported in audio controls due to libvorbis miniaudio wrapper vtable don't implement in-memory decoding, .mp3, .wav and .flac works fine.

## Future Plans:
* Sound Groups: Management and basic RTPCs
* AudioSources support: Microphone Input and large file streaming. or from other sources.

## License

MIT license.
