/*
 * Header file to generate xml tags 
 *
 */

#ifndef __PSIA_TAGS_H__
#define __PSIA_TAGS_H__

#ifdef INITIALIZE_TAGS
#define CREATE_TAG(tag) const char* TAG_##tag=#tag
#else
#define CREATE_TAG(tag) extern const char* TAG_##tag
#endif

/* root tags */
CREATE_TAG(ResourceList);
CREATE_TAG(Resource);
CREATE_TAG(name);
CREATE_TAG(type);
CREATE_TAG(ResourceDescription);
CREATE_TAG(version);
CREATE_TAG(get);
CREATE_TAG(put);
CREATE_TAG(post);
CREATE_TAG(delete);
CREATE_TAG(queryStringParameterList);
CREATE_TAG(inboundXML);
CREATE_TAG(function);
CREATE_TAG(returnResult);
CREATE_TAG(notes);

/* Response tag */
CREATE_TAG(ResponseStatus);

/* System/Network tags */
CREATE_TAG(NetworkInterfaceList);
CREATE_TAG(NetworkInterface);
CREATE_TAG(Discovery);
CREATE_TAG(UPnP);
CREATE_TAG(Zeroconf);
CREATE_TAG(MulticastDiscovery);
CREATE_TAG(portNo);
CREATE_TAG(ttl);

/* System/Video tags */
CREATE_TAG(VideoInput);
CREATE_TAG(VideoInputChannelList);
CREATE_TAG(VideoInputChannel);
CREATE_TAG(input);
CREATE_TAG(whiteBalanceMode);
CREATE_TAG(whiteBalanceLevel);
CREATE_TAG(exposureMode);
CREATE_TAG(Exposure);
CREATE_TAG(exposureTarget);
CREATE_TAG(exposureAutoMin);
CREATE_TAG(exposureAutoMax);
CREATE_TAG(gainLevel);
CREATE_TAG(brightnessLevel);
CREATE_TAG(contrastLevel);
CREATE_TAG(sharpnessLevel);
CREATE_TAG(saturationLevel);
CREATE_TAG(DayNightFilter);
CREATE_TAG(dayNightFilterType);
CREATE_TAG(VideoOverlay);
CREATE_TAG(TextOverlayList);
CREATE_TAG(TextOverlay);
CREATE_TAG(ImageOverlayList);
CREATE_TAG(ImageOverlay);
CREATE_TAG(PrivacyMask);
CREATE_TAG(videoInputID);
CREATE_TAG(timeStampEnabled);
CREATE_TAG(dateTimeFormat);
CREATE_TAG(backgroundColor);
CREATE_TAG(fontColor);
CREATE_TAG(fontSize);
CREATE_TAG(displayText);
CREATE_TAG(horizontalAlignType);
CREATE_TAG(verticalAlignType);
CREATE_TAG(imageName);
CREATE_TAG(positionX);
CREATE_TAG(positionY);
CREATE_TAG(transparentColorEnabled);
CREATE_TAG(transparentColor);

/* System/Audio tags */
CREATE_TAG(AudioChannelList);
CREATE_TAG(AudioChannel);
CREATE_TAG(audioMode);
CREATE_TAG(microphoneEnabled);
CREATE_TAG(microphoneSource);
CREATE_TAG(microphoneVolume);
CREATE_TAG(speakerEnabled);
CREATE_TAG(speakerVolume);

/* System tags */
CREATE_TAG(Time);
CREATE_TAG(timeMode);
CREATE_TAG(localTime);
CREATE_TAG(timeZone);
CREATE_TAG(NtpServerList);
CREATE_TAG(NtpServer);
CREATE_TAG(addressingFormatType);
CREATE_TAG(hostName);
CREATE_TAG(ipv6Address);
CREATE_TAG(requestURL);
CREATE_TAG(statusCode);
CREATE_TAG(statusString);
CREATE_TAG(DeviceInfo);
CREATE_TAG(deviceName);
CREATE_TAG(deviceID);
CREATE_TAG(model);
CREATE_TAG(macAddress);
CREATE_TAG(firmwareVersion);
CREATE_TAG(firmwareReleasedDate);
CREATE_TAG(bootVersion);
CREATE_TAG(hardwareVersion);

CREATE_TAG(DeviceStatus);
CREATE_TAG(currentDeviceTime);
CREATE_TAG(deviceUpTime);
CREATE_TAG(CPUList);
CREATE_TAG(CPU);
CREATE_TAG(cpuDescription);
CREATE_TAG(cpuUtilization);
CREATE_TAG(MemoryList);
CREATE_TAG(Memory);
CREATE_TAG(memoryDescription);
CREATE_TAG(memoryUsage);
CREATE_TAG(memoryAvailable);

CREATE_TAG(IPAddress);
CREATE_TAG(ipVersion);
CREATE_TAG(addressingType);
CREATE_TAG(ipAddress);
CREATE_TAG(subnetMask);
CREATE_TAG(DefaultGateway);

/* Streaming tags */
CREATE_TAG(StreamingStatus);
CREATE_TAG(StreamingChannelList);
CREATE_TAG(StreamingChannel);
CREATE_TAG(StreamingSessionStatusList);
CREATE_TAG(StreamingSessionStatus);
CREATE_TAG(clientAddress);
CREATE_TAG(clientUserName);
CREATE_TAG(startDateTime);
CREATE_TAG(elapsedTime);
CREATE_TAG(bandwidth);
CREATE_TAG(id);
CREATE_TAG(channelName);
CREATE_TAG(enabled);
CREATE_TAG(disabled);
CREATE_TAG(Transport);
CREATE_TAG(rtspPortNo);
CREATE_TAG(maxPacketSize);
CREATE_TAG(ControlProtocolList);
CREATE_TAG(ControlProtocol);
CREATE_TAG(streamingTransport);
CREATE_TAG(Unicast);
CREATE_TAG(interfaceID);
CREATE_TAG(rtpTransportType);
CREATE_TAG(Multicast);
CREATE_TAG(Security);
CREATE_TAG(Video);
CREATE_TAG(videoInputChannelID);
CREATE_TAG(videoCodecType);
CREATE_TAG(videoScanType);
CREATE_TAG(videoResolutionWidth);
CREATE_TAG(videoResolutionHeight);
CREATE_TAG(videoQualityControlType);
CREATE_TAG(constantBitRate);
CREATE_TAG(fixedQuality);
CREATE_TAG(maxFrameRate);
CREATE_TAG(rotationDegree);
CREATE_TAG(mirrorEnabled);
CREATE_TAG(Audio);
CREATE_TAG(audioInputChannelID);
CREATE_TAG(audioCompressionType);
CREATE_TAG(audioBitRate);
CREATE_TAG(audioSamplingRate);

/* security tags*/
CREATE_TAG(UserList);
CREATE_TAG(User);
CREATE_TAG(userName);
CREATE_TAG(password);
CREATE_TAG(privilegeLevel);
CREATE_TAG(ProtocolList);
CREATE_TAG(Protocol);
CREATE_TAG(protocol);

/* Custom/Event tags */
CREATE_TAG(EventNotification);
CREATE_TAG(EventTriggerList);
CREATE_TAG(EventTrigger);
CREATE_TAG(eventType);
CREATE_TAG(eventDescription);
CREATE_TAG(inputIOPortID);
CREATE_TAG(intervalBetweenEvents);
CREATE_TAG(EventTriggerNotificationList);
CREATE_TAG(EventTriggerNotification);
CREATE_TAG(notificationMethod);
CREATE_TAG(notificationRecurrence);
CREATE_TAG(notificationInterval);
CREATE_TAG(EventSchedule);
CREATE_TAG(DateTimeRange);
CREATE_TAG(beginDateTime);
CREATE_TAG(endDateTime);
CREATE_TAG(TimeBlockList);
CREATE_TAG(EvenNotificationMethods);
CREATE_TAG(FTPFormat);
CREATE_TAG(uploadVideoClipEnabled);
CREATE_TAG(EmailFormat);
CREATE_TAG(senderEmailAddress);
CREATE_TAG(receiverEmailAddress);
CREATE_TAG(MediaFormat);
CREATE_TAG(videoClipFormatType);
CREATE_TAG(MailingNotificationList);
CREATE_TAG(MailingNotification);
CREATE_TAG(authenticationMode);
CREATE_TAG(accountName);
CREATE_TAG(FTPNotificationList);
CREATE_TAG(FTPNotification);
CREATE_TAG(uploadPath);
CREATE_TAG(HttpHostNotificationList);
CREATE_TAG(HttpHostNotification);
CREATE_TAG(url);
CREATE_TAG(protocolType);

/* Custom/Analytics tags */
CREATE_TAG(MotionDetectionList);
CREATE_TAG(MotionDetection);
CREATE_TAG(MotionDetectionRegionList);
CREATE_TAG(MotionDetectionRegion);
CREATE_TAG(samplingInterval);
CREATE_TAG(startTriggerTime);
CREATE_TAG(endTriggerTime);
CREATE_TAG(directionSensitivity);
CREATE_TAG(regionType);
CREATE_TAG(Grid);
CREATE_TAG(rowGranularity);
CREATE_TAG(columnGranularity);
CREATE_TAG(maskEnabled);
CREATE_TAG(sensitivityLevel);
CREATE_TAG(detectionThreshold);
CREATE_TAG(RegionCoordinatesList);
CREATE_TAG(RegionCoordinates);

#endif /* __PSIA_TAGS_H__ */
