#include <stingray/toolkit/Factory.h>

#include <stingray/toolkit/any.h>

#include <stingray/app/RecordErrors.h>
#include <stingray/app/activation_manager/ActivationIntent.h>
#include <stingray/app/application_context/AppChannel.h>
#include <stingray/app/application_context/ChannelList.h>
#include <stingray/app/scheduler/ScheduledEvents.h>
#include <stingray/ca/BasicSubscriptionLease.h>
#include <stingray/ca/BissConditionalAccess.h>
#include <stingray/ca/DreSubscription.h>
#include <stingray/ca/SubscriptionBundle.h>
#include <stingray/crypto/PlainCipherKey.h>
#include <stingray/details/IReceiverTrait.h>
#include <stingray/hdmi/IHDMI.h>
#include <stingray/media/ImageFileMediaData.h>
#include <stingray/media/MediaInfoBase.h>
#include <stingray/media/formats/flv/MediaInfo.h>
#include <stingray/media/formats/mp3/Mp3MediaInfo.h>
#include <stingray/media/formats/mp4/MediaInfo.h>
#include <stingray/media/formats/mp4/Stream.h>
#include <stingray/media/formats/mp4/SubstreamDescriptors.h>
#include <stingray/mpeg/ContentDescription.h>
#include <stingray/mpeg/PvrDescription.h>
#include <stingray/mpeg/Stream.h>
#include <stingray/net/DHCPInterfaceConfiguration.h>
#include <stingray/net/IgnoredInterfaceConfiguration.h>
#include <stingray/net/LinkLocalInterfaceConfiguration.h>
#include <stingray/net/ManualInterfaceConfiguration.h>
#include <stingray/net/NetworkConfiguration.h>
#include <stingray/parentalcontrol/AgeRating.h>
#ifdef PLATFORM_EMU
#	include <stingray/platform/emu/scanner/Channel.h>
#endif
#ifdef PLATFORM_EMU
#	include <stingray/platform/emu/scanner/EmuScanPerformerFactory.h>
#endif
#ifdef PLATFORM_MSTAR
#	include <stingray/platform/mstar/crypto/HardwareCipherKey.h>
#endif
#ifdef PLATFORM_OPENSSL
#	include <stingray/platform/openssl/crypto/Certificate.h>
#endif
#ifdef PLATFORM_OPENSSL
#	include <stingray/platform/openssl/crypto/CertificateRevocationList.h>
#endif
#ifdef PLATFORM_OPENSSL
#	include <stingray/platform/openssl/crypto/EvpKey.h>
#endif
#ifdef PLATFORM_STAPI
#	include <stingray/platform/stapi/crypto/HardwareCipherKey.h>
#endif
#include <stingray/pushvod/pushvod_emu/PushVODEmulationMovie.h>
#include <stingray/records/FileSystemRecord.h>
#include <stingray/rpc/UrlObjectId.h>
#include <stingray/scanner/DVBServiceId.h>
#include <stingray/scanner/DefaultDVBTBandInfo.h>
#include <stingray/scanner/DefaultMpegService.h>
#include <stingray/scanner/DefaultMpegSubstreamDescriptor.h>
#include <stingray/scanner/DefaultScanParams.h>
#include <stingray/scanner/DefaultScanPerformerFactory.h>
#include <stingray/scanner/DreCasGeographicRegion.h>
#include <stingray/scanner/LybidScanParams.h>
#include <stingray/scanner/LybidScanPerformerFactory.h>
#include <stingray/scanner/TerrestrialScanParams.h>
#include <stingray/scanner/TricolorGeographicRegion.h>
#include <stingray/scanner/TricolorScanParams.h>
#include <stingray/scanner/TricolorScanPerformerFactory.h>
#include <stingray/scanner/lybid/LybidServiceMetaInfo.h>
#include <stingray/scanner/terrestrial/TerrestrialServiceMetaInfo.h>
#include <stingray/scanner/tricolor/AutoCreatedListMetaInfo.h>
#include <stingray/scanner/tricolor/TricolorServiceMetaInfo.h>
#include <stingray/stats/StatisticChannelInfo.h>
#include <stingray/streams/PlaybackStreamContent.h>
#include <stingray/streams/RecordStreamContent.h>
#include <stingray/streams/RecordStreamMetaInfo.h>
#include <stingray/time/BuiltinTimeSourceId.h>
#include <stingray/time/TDTTimeSourceId.h>
#include <stingray/time/TransportsTimeSourceObtainer.h>
#include <stingray/tuners/TunerState.h>
#include <stingray/tuners/dvbs/Antenna.h>
#include <stingray/tuners/dvbs/DefaultDVBSTransport.h>
#include <stingray/tuners/dvbs/Satellite.h>
#include <stingray/tuners/dvbt/DVBTTransport.h>
#include <stingray/tuners/ip/TsOverIpTransport.h>
#include <stingray/update/VersionRequirement.h>
#include <stingray/update/system/CopyFile.h>
#include <stingray/update/system/EraseFlashPartition.h>
#include <stingray/update/system/MountFilesystem.h>
#include <stingray/update/system/MoveFile.h>
#include <stingray/update/system/RemoveFile.h>
#include <stingray/update/system/UnmountFilesystem.h>
#include <stingray/update/system/WriteFlashPartition.h>

/* WARNING! This is autogenerated file, DO NOT EDIT! */

namespace stingray { namespace Detail
{
	void Factory::RegisterTypes()
	{
#ifdef BUILD_SHARED_LIB
		/*nothing*/
#else
		TOOLKIT_REGISTER_CLASS_EXPLICIT(app::NoSubscriptionRecordError);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(app::SimpleRecordError);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(app::ActivationKeyIntent);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(app::PersonalCodeIntent);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(app::PlatformNameIntent);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(app::ReceiverTraitIntent);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(app::AppChannel);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(app::ChannelList);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(app::CinemaHallScheduledViewing);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(app::ContinuousScheduledEvent);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(app::DeferredStandby);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(app::InfiniteScheduledEvent);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(app::ScheduledEvent);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(app::ScheduledRecord);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(app::ScheduledViewing);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(BasicSubscriptionLease);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(BasicSubscriptionProvider);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(BissSubscription);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(Dre4Provider);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(Dre4Subscription);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(SubscriptionBundle);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(PlainCipherKey);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(DVBCReceiverTrait);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(DVBSReceiverTrait);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(DVBTReceiverTrait);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(HouseholdClientTrait);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(HouseholdServerTrait);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(LybidOperatorTrait);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(SatIpClientTrait);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(TricolorOperatorTrait);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(HDMIConfig);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(ImageFileMediaInfo);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(ImageFileMediaPreview);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(InMemoryImageMediaPreview);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(MediaInfoBase);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(flv::MediaInfo);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(Mp3MediaInfo);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(mp4::MediaInfo);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(mp4::Stream);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(mp4::Mp4MediaAudioSubstreamDescriptor);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(mp4::Mp4MediaVideoSubstreamDescriptor);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(mpeg::ContentDescription);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(mpeg::PvrDescription);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(mpeg::Stream);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(DHCPInterfaceConfiguration);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(IgnoredInterfaceConfiguration);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(LinkLocalInterfaceConfiguration);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(ManualInterfaceConfiguration);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(Detail::any::ObjectHolder<NetworkConfigurationStatus>);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(AgeRating);
#ifdef PLATFORM_EMU
		TOOLKIT_REGISTER_CLASS_EXPLICIT(emu::EmuServiceId);
#endif
#ifdef PLATFORM_EMU
		TOOLKIT_REGISTER_CLASS_EXPLICIT(emu::RadioChannel);
#endif
#ifdef PLATFORM_EMU
		TOOLKIT_REGISTER_CLASS_EXPLICIT(emu::SubstreamDescriptor);
#endif
#ifdef PLATFORM_EMU
		TOOLKIT_REGISTER_CLASS_EXPLICIT(emu::TVChannel);
#endif
#ifdef PLATFORM_EMU
		TOOLKIT_REGISTER_CLASS_EXPLICIT(emu::EmuScanPerformerFactory);
#endif
#ifdef PLATFORM_MSTAR
		TOOLKIT_REGISTER_CLASS_EXPLICIT(mstar::HardwareCipherKey);
#endif
#ifdef PLATFORM_OPENSSL
		TOOLKIT_REGISTER_CLASS_EXPLICIT(openssl::Certificate);
#endif
#ifdef PLATFORM_OPENSSL
		TOOLKIT_REGISTER_CLASS_EXPLICIT(openssl::CertificateRevocationList);
#endif
#ifdef PLATFORM_OPENSSL
		TOOLKIT_REGISTER_CLASS_EXPLICIT(openssl::EvpKey);
#endif
#ifdef PLATFORM_STAPI
		TOOLKIT_REGISTER_CLASS_EXPLICIT(stapi::HardwareCipherKey);
#endif
		TOOLKIT_REGISTER_CLASS_EXPLICIT(PushVODEmulationMovie);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(FileSystemRecord);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(rpc::UrlObjectId);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(DVBServiceId);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(DefaultDVBTBandInfo);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(DefaultMpegRadioChannel);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(DefaultMpegService);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(DefaultMpegTVChannel);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(MpegAudioSubstreamDescriptor);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(MpegDataCarouselSubstreamDescriptor);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(MpegPcrSubstreamDescriptor);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(MpegSubtitlesSubstreamDescriptor);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(MpegTeletextBasedSubtitlesSubstreamDescriptor);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(MpegTeletextSubstreamDescriptor);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(MpegVideoSubstreamDescriptor);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(DefaultScanParams);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(DefaultScanPerformerFactory);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(DreCasGeographicRegion);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(LybidScanParams);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(LybidScanPerformerFactory);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(TerrestrialScanParams);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(TricolorGeographicRegion);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(TricolorScanParams);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(TricolorScanPerformerFactory);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(LybidServiceMetaInfo);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(TerrestrialServiceMetaInfo);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(AutoCreatedListMetaInfo);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(TricolorServiceMetaInfo);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(Detail::any::ObjectHolder<stingray::StatisticChannelInfo>);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(PlaybackStreamContent);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(RecordStreamContent);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(RecordStreamMetaInfo);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(BuiltinTimeSourceId);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(TDTTimeSourceId);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(TransportTimeOffset);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(CircuitedTunerState);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(LockedTunerState);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(UnlockedTunerState);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(Antenna);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(DefaultDVBSTransport);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(Satellite);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(DVBTTransport);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(TsOverIpTransport);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(VersionRequirement);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(CopyFile);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(EraseFlashPartition);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(MountFilesystem);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(MoveFile);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(RemoveFile);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(UnmountFilesystem);
		TOOLKIT_REGISTER_CLASS_EXPLICIT(WriteFlashPartition);
#endif
	}
}}
