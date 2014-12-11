/*
 * Device interface to retreive or update settings
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sysctrl.h>

#define INITIALIZE_TAGS

#include "psia.h"
#include "tags.h"
#include "iface.h"

#define ETH_NAME	"eth0"

/*******************************************************************/
/*                        Standard Response                        */
/*******************************************************************/
void add_response_status(request *req, node_t *node)
{
	char scstr[32];

	sprintf(scstr, "%d", req->status_code);
	xadd_stag_attr(req, TAG_ResponseStatus, RootAttrStr);
	xadd_elem(req, TAG_requestURL, req->command);
	xadd_elem(req, TAG_statusCode, scstr);
	xadd_elem(req, TAG_statusString, StatusString[req->status_code]);
	xadd_etag(req, TAG_ResponseStatus);
}

/* return the corresponsing HTTP status code */
int get_http_status(request *req)
{
	int http_status;

	http_status = STATUS_INVALID_OPERATION;
	switch(req->status_code) {
		case STATUS_OK:
			http_status = R_REQUEST_OK;
			break;
		case STATUS_DEVICE_BUSY:
			http_status = R_SERVICE_UNAV;
			break;
		case STATUS_DEVICE_ERROR:
			http_status = R_ERROR;
			break;
		case STATUS_INVALID_OPERATION:
			http_status = R_FORBIDDEN;
			break;
		case STATUS_INVALID_XML_FORMAT:
			http_status = R_BAD_REQUEST;
			break;
		case STATUS_INVALID_XML_CONTENT:
			http_status = R_BAD_REQUEST;
			break;
		case STATUS_REBOOT_REQUIRED:
			http_status = R_REQUEST_OK;
			break;
	}
	req->response_complete = 1;
	//fprintf(stderr,"\r\n After get_http_status with http status:%d\n",http_status);
	return http_status;
}

/*******************************************************************/
/*                          System/Network                         */
/*******************************************************************/
int retrieve_system_network_interfaces(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_NetworkInterfaceList, RootAttrStr);
	for(n = 0; n < get_network_count(); n++)
		retrieve_system_network_interfaces_id(req, node);
	xadd_etag(req, TAG_NetworkInterfaceList);
	return STATUS_OK;
}

int retrieve_system_network_interfaces_id(request *req, node_t *node)
{
	xadd_stag_attr(req, TAG_NetworkInterface, RootAttrStr);

	retrieve_system_network_interfaces_id_ipaddress(req, node);
	retrieve_system_network_interfaces_id_discovery(req, node);
	xadd_etag(req, TAG_NetworkInterface);

	return STATUS_OK;
}

int update_system_network_interfaces_id(request *req, node_t *node)
{
	update_system_network_interfaces_id_ipaddress(req, node);
	update_system_network_interfaces_id_discovery(req, node);	

	return STATUS_OK;
}

int retrieve_system_network_interfaces_id_ipaddress(request *req, node_t *node)
{
	const char *ip_version = "v4";
	const char *addressing_type = "static";
	const char *ip_address = "10.100.49.10";
	const char *subnet_mask = "255.255.0.0";
	const char *gateway = "10.100.1.1";

	xadd_stag_attr(req, TAG_IPAddress, RootAttrStr);
	xadd_elem(req, TAG_ipVersion, ip_version);
	xadd_elem(req, TAG_addressingType, addressing_type);
	xadd_elem(req, TAG_ipAddress, ip_address);
	xadd_elem(req, TAG_subnetMask, subnet_mask);
	xadd_stag(req, TAG_DefaultGateway);
	xadd_elem(req, TAG_ipAddress, gateway);
	xadd_etag(req, TAG_DefaultGateway);
	/* primary dns */
	/* secondary dns */
	xadd_etag(req, TAG_IPAddress);
	return STATUS_OK;
}

int update_system_network_interfaces_id_ipaddress(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;	
	IPAddress_psia IPAddress;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_IPAddress) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_ipVersion) == 0) {
			strcpy(IPAddress.ip_version, tag->value);
			required |= xml_required_field(1);            
		}
		else if(strcmp(tag->name, TAG_addressingType) == 0) {
			strcpy(IPAddress.addressing_type, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_ipAddress) == 0) {
			strcpy(IPAddress.ip_address, tag->value);
			required |= xml_required_field(3);
            ControlSystemData(SFIELD_SET_IP, (void *)&IPAddress.ip_address, sizeof(IPAddress.ip_address));
		}
		else if(strcmp(tag->name, TAG_subnetMask) == 0) {
			strcpy(IPAddress.subnet_mask, tag->value);
			required |= xml_required_field(4);
			ControlSystemData(SFIELD_SET_NETMASK, (void *)&IPAddress.subnet_mask, sizeof(IPAddress.subnet_mask));
		}
		else if(strcmp(tag->name, TAG_ipAddress) == 0) {
			strcpy(IPAddress.gateway, tag->value);
			required |= xml_required_field(5);
			ControlSystemData(SFIELD_SET_GATEWAY, (void *)&IPAddress.gateway, sizeof(IPAddress.gateway));
		}
		else if(strcmp(tag->name, TAG_ipAddress) == 0) {
			strcpy(IPAddress.primaryDNS, tag->value);
			required |= xml_required_field(6);
			ControlSystemData(SFIELD_SET_GATEWAY, (void *)&IPAddress.primaryDNS, sizeof(IPAddress.primaryDNS));
		}		
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fields were present */
	if(required != xml_required_mask(5))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_system_network_interfaces_id_discovery(request *req, node_t *node)
{
	const char *upnp_enabled = "1";
	const char *zconf_enabled = "1";
	const char *mcast_enabled = "1";
	const char *ipAddress = "224.0.0.251";	
	const char *portNo = "5353";			
	const char *ttl = "255";			  

	xadd_stag_attr(req, TAG_Discovery, RootAttrStr);
	xadd_stag(req, TAG_UPnP);
	xadd_elem(req, TAG_enabled, upnp_enabled);
	xadd_etag(req, TAG_UPnP);
	xadd_stag(req, TAG_Zeroconf);
	xadd_elem(req, TAG_enabled, zconf_enabled);
	xadd_etag(req, TAG_Zeroconf);
	xadd_stag(req, TAG_MulticastDiscovery);
	xadd_elem(req, TAG_enabled, mcast_enabled);
	xadd_elem(req, TAG_ipAddress, ipAddress);
	xadd_elem(req, TAG_portNo, portNo);
	xadd_elem(req, TAG_ttl, ttl);
	xadd_etag(req, TAG_MulticastDiscovery);
	xadd_etag(req, TAG_Discovery);

	return STATUS_OK;
}

int update_system_network_interfaces_id_discovery(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;	
	Discovery_psia Discovery;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_Discovery) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_UPnP) == 0) {
			strcpy(Discovery.upnp_enabled, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_Zeroconf) == 0) {
			strcpy(Discovery.zconf_enabled, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_MulticastDiscovery) == 0) {
			strcpy(Discovery.mcast_enabled, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_ipAddress) == 0) {
			strcpy(Discovery.ipAddress, tag->value);
			required |= xml_required_field(4);
		}
		else if(strcmp(tag->name, TAG_portNo) == 0) {
			strcpy(Discovery.portNo, tag->value);
			required |= xml_required_field(5);
			ControlSystemData(SFIELD_SET_HTTPPORT, (void *)&Discovery.portNo, sizeof(Discovery.portNo));
		}
		else if(strcmp(tag->name, TAG_ttl) == 0) {
			strcpy(Discovery.ttl, tag->value);
			required |= xml_required_field(6);
		}	
		/* Protocol */

		tag = tag->next;
	}

	/* check all the required fields were present */
	if(required != xml_required_mask(6))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int get_network_count(void)
{
	return 1;
}

void get_network_url(int id, char *idstr)
{
	/* default ID 0,1,2,3... */
	sprintf(idstr,"/%d", id);
}

int get_network_index(char *idstr)
{
	idstr++;  // skip the '/'
	return(atoi(idstr));
}

/*******************************************************************/
/*                          System/Video                         */
/*******************************************************************/

int retrieve_system_video_inputs(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_VideoInput, RootAttrStr);
	for(n = 0; n < get_channel_count(); n++)
		retrieve_system_video_channels(req, node);
	xadd_etag(req, TAG_VideoInput);

	return STATUS_OK;
}

int retrieve_system_video_channels(request *req, node_t *node)
{
	xadd_stag_attr(req, TAG_VideoInputChannelList, RootAttrStr);
	retrieve_system_video_channels_id(req, node);
	xadd_etag(req, TAG_VideoInputChannelList);

	return STATUS_OK;
}

int retrieve_system_video_channels_id(request *req, node_t *node)
{	
	const char *id = "1";
	const char *input = "1";
	const char *whiteBalanceMode = "Auto";
	const char *whiteBalanceLevel = "1";
	const char *exposureMode = "Auto";
	const char *exposureTarget = "v4";
	const char *exposureAutoMin = "static";
	const char *exposureAutoMax = "1";
	const char *gainLevel = "1";
	const char *brightnessLevel = "1";
	const char *contrastLevel = "1";
	const char *sharpnessLevel = "1";
	const char *saturationLevel = "1";
	const char *dayNightFilterType = "Day";

	xadd_stag_attr(req, TAG_VideoInputChannel, RootAttrStr);
	xadd_elem(req, TAG_id, id);
	xadd_elem(req, TAG_addressingType, input);
	xadd_elem(req, TAG_whiteBalanceMode, whiteBalanceMode);
	xadd_elem(req, TAG_whiteBalanceLevel, whiteBalanceLevel);
	xadd_stag(req, TAG_Exposure);
	xadd_elem(req, TAG_exposureTarget, exposureTarget);
    xadd_elem(req, TAG_exposureAutoMin, exposureAutoMin);
    xadd_elem(req, TAG_exposureAutoMax, exposureAutoMax);    
	xadd_etag(req, TAG_Exposure);
	xadd_elem(req, TAG_gainLevel, gainLevel);
	xadd_elem(req, TAG_brightnessLevel, brightnessLevel);
	xadd_elem(req, TAG_contrastLevel, contrastLevel);
	xadd_elem(req, TAG_sharpnessLevel, sharpnessLevel);
	xadd_elem(req, TAG_saturationLevel, saturationLevel);
	xadd_stag(req, TAG_DayNightFilter);
	xadd_elem(req, TAG_dayNightFilterType, dayNightFilterType);
	xadd_etag(req, TAG_DayNightFilter);	
	xadd_etag(req, TAG_VideoInputChannel);

	return STATUS_OK;
}

int update_system_video_channels_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;	
	VideoInputChannel_psia VideoInputChannel;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_VideoInputChannel) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(VideoInputChannel.id, tag->value);
			required |= xml_required_field(1);            
		}
		else if(strcmp(tag->name, TAG_input) == 0) {
			strcpy(VideoInputChannel.input, tag->value);
			required |= xml_required_field(2);
		}	
		else if(strcmp(tag->name, TAG_whiteBalanceMode) == 0) {
			strcpy(VideoInputChannel.whiteBalanceMode, tag->value);
			required |= xml_required_field(3);
			ControlSystemData(SFIELD_SET_WHITE_BALANCE, (void *)&VideoInputChannel.whiteBalanceMode, sizeof(VideoInputChannel.whiteBalanceMode));
		}
		else if(strcmp(tag->name, TAG_whiteBalanceLevel) == 0) {
			strcpy(VideoInputChannel.whiteBalanceLevel, tag->value);
			required |= xml_required_field(4);
		}
		else if(strcmp(tag->name, TAG_exposureMode) == 0) {
			strcpy(VideoInputChannel.exposureMode, tag->value);
			required |= xml_required_field(5);
		}
		else if(strcmp(tag->name, TAG_exposureTarget) == 0) {
			strcpy(VideoInputChannel.exposureTarget, tag->value);
			required |= xml_required_field(6);
		}
		else if(strcmp(tag->name, TAG_exposureAutoMin) == 0) {
			strcpy(VideoInputChannel.exposureAutoMin, tag->value);
			required |= xml_required_field(7);
		}
		else if(strcmp(tag->name, TAG_exposureAutoMax) == 0) {
			strcpy(VideoInputChannel.exposureAutoMax, tag->value);
			required |= xml_required_field(8);
		}
		else if(strcmp(tag->name, TAG_gainLevel) == 0) {
			strcpy(VideoInputChannel.gainLevel, tag->value);
			required |= xml_required_field(9);
		}
		else if(strcmp(tag->name, TAG_brightnessLevel) == 0) {
			strcpy(VideoInputChannel.brightnessLevel, tag->value);
			required |= xml_required_field(10);
			ControlSystemData(SFIELD_SET_BRIGHTNESS, (void *)&VideoInputChannel.brightnessLevel, sizeof(VideoInputChannel.brightnessLevel));
		}
		else if(strcmp(tag->name, TAG_contrastLevel) == 0) {
			strcpy(VideoInputChannel.contrastLevel, tag->value);
			required |= xml_required_field(11);
			ControlSystemData(SFIELD_SET_CONTRAST, (void *)&VideoInputChannel.contrastLevel, sizeof(VideoInputChannel.contrastLevel));
		}
		else if(strcmp(tag->name, TAG_sharpnessLevel) == 0) {
			strcpy(VideoInputChannel.sharpnessLevel, tag->value);
			required |= xml_required_field(12);
			ControlSystemData(SFIELD_SET_SHARPNESS, (void *)&VideoInputChannel.sharpnessLevel, sizeof(VideoInputChannel.sharpnessLevel));
		}
		else if(strcmp(tag->name, TAG_saturationLevel) == 0) {
			strcpy(VideoInputChannel.saturationLevel, tag->value);
			required |= xml_required_field(13);
			ControlSystemData(SFIELD_SET_SATURATION, (void *)&VideoInputChannel.sharpnessLevel, sizeof(VideoInputChannel.sharpnessLevel));
		}
		else if(strcmp(tag->name, TAG_dayNightFilterType) == 0) {
			strcpy(VideoInputChannel.dayNightFilterType, tag->value);
			required |= xml_required_field(14);
			ControlSystemData(SFIELD_SET_DAY_NIGHT, (void *)&VideoInputChannel.dayNightFilterType, sizeof(VideoInputChannel.dayNightFilterType));
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fields were present */
	if(required != xml_required_mask(14))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_system_video_channels_id_overlays(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_VideoOverlay, RootAttrStr);
	for(n = 0; n < get_channel_count(); n++){
		retrieve_system_video_channels_id_overlays_text(req, node);
	    retrieve_system_video_channels_id_overlays_image(req, node);
	}
	xadd_etag(req, TAG_VideoOverlay);

	return STATUS_OK;
}

int update_system_video_channels_id_overlays(request *req, node_t *node)
{
	int n;	

	for(n = 0; n < get_channel_count(); n++){
		update_system_video_channels_id_overlays_text(req, node);
	    update_system_video_channels_id_overlays_image(req, node);
	}	
	return STATUS_OK;
}

int delete_system_video_channels_id_overlays(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_system_video_channels_id_privacymask(request *req, node_t *node)
{
	const char *enabled = "TRUE";
	
	xadd_stag_attr(req, TAG_PrivacyMask, RootAttrStr);
	xadd_elem(req, TAG_enabled, enabled);	
	xadd_etag(req, TAG_PrivacyMask);

	return STATUS_OK;
}

int update_system_video_channels_id_privacymask(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;	
	PrivacyMask_psia PrivacyMask;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_PrivacyMask) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(PrivacyMask.enabled, tag->value);
			required |= xml_required_field(1);            
		}				
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fields were present */
	if(required != xml_required_mask(11))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_system_video_channels_id_overlays_text(request *req, node_t *node)
{
	xadd_stag_attr(req, TAG_TextOverlayList, RootAttrStr);
	retrieve_system_video_channels_id_overlays_text_id(req, node);	
	xadd_etag(req, TAG_TextOverlayList);

	return STATUS_OK;
}

int update_system_video_channels_id_overlays_text(request *req, node_t *node)
{
	update_system_video_channels_id_overlays_text_id(req, node);	

	return STATUS_OK;
}

int create_system_video_channels_id_overlays_text(request *req, node_t *node)
{
	return STATUS_OK;
}

int delete_system_video_channels_id_overlays_text(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_system_video_channels_id_overlays_image(request *req, node_t *node)
{
	xadd_stag_attr(req, TAG_ImageOverlayList, RootAttrStr);
	retrieve_system_video_channels_id_overlays_image_id(req, node);	
	xadd_etag(req, TAG_ImageOverlayList);

	return STATUS_OK;
}

int update_system_video_channels_id_overlays_image(request *req, node_t *node)
{
	update_system_video_channels_id_overlays_image_id(req, node);
	
	return STATUS_OK;
}

int delete_system_video_channels_id_overlays_image(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_system_video_channels_id_overlays_text_id(request *req, node_t *node)
{
	const char *id = "1";
	const char *enabled = "TRUE";
	const char *videoInputID = "1";
	const char *timeStampEnabled = "TRUE";
	const char *dateTimeFormat = "MM/DD/YYYY";
	const char *backgroundColor = "1";
	const char *fontColor = "1";
	const char *fontSize = "1";
	const char *displayText = "IPNC";
	const char *horizontalAlignType = "left";
	const char *verticalAlignType = "top";	

	xadd_stag_attr(req, TAG_TextOverlay, RootAttrStr);
	xadd_elem(req, TAG_id, id);
	xadd_elem(req, TAG_enabled, enabled);
	xadd_elem(req, TAG_videoInputID, videoInputID);
	xadd_elem(req, TAG_timeStampEnabled, timeStampEnabled);	
	xadd_elem(req, TAG_dateTimeFormat, dateTimeFormat);
    xadd_elem(req, TAG_backgroundColor, backgroundColor);
    xadd_elem(req, TAG_fontColor, fontColor);	
	xadd_elem(req, TAG_fontSize, fontSize);
	xadd_elem(req, TAG_displayText, displayText);
	xadd_elem(req, TAG_horizontalAlignType, horizontalAlignType);
	xadd_elem(req, TAG_verticalAlignType, verticalAlignType);
	xadd_etag(req, TAG_TextOverlay);

	return STATUS_OK;
}

int update_system_video_channels_id_overlays_text_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;	
	TextOverlay_psia TextOverlay;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_TextOverlay) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(TextOverlay.id, tag->value);
			required |= xml_required_field(1);            
		}
		else if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(TextOverlay.enabled, tag->value);
			required |= xml_required_field(2);
		}	
		else if(strcmp(tag->name, TAG_videoInputID) == 0) {
			strcpy(TextOverlay.videoInputID, tag->value);
			required |= xml_required_field(3);			
		}
		else if(strcmp(tag->name, TAG_timeStampEnabled) == 0) {
			strcpy(TextOverlay.timeStampEnabled, tag->value);
			required |= xml_required_field(4);
			ControlSystemData(SFIELD_SET_TSTAMPENABLE, &TextOverlay.timeStampEnabled, sizeof(TextOverlay.timeStampEnabled));
		}
		else if(strcmp(tag->name, TAG_dateTimeFormat) == 0) {
			strcpy(TextOverlay.dateTimeFormat, tag->value);
			required |= xml_required_field(5);
			ControlSystemData(SFIELD_SET_TSTAMPFORMAT, &TextOverlay.dateTimeFormat, sizeof(TextOverlay.dateTimeFormat));
		}
		else if(strcmp(tag->name, TAG_backgroundColor) == 0) {
			strcpy(TextOverlay.backgroundColor, tag->value);
			required |= xml_required_field(6);			
		}
		else if(strcmp(tag->name, TAG_fontColor) == 0) {
			strcpy(TextOverlay.fontColor, tag->value);
			required |= xml_required_field(7);			
		}
		else if(strcmp(tag->name, TAG_fontSize) == 0) {
			strcpy(TextOverlay.fontSize, tag->value);
			required |= xml_required_field(8);			
		}
		else if(strcmp(tag->name, TAG_displayText) == 0) {
			strcpy(TextOverlay.displayText, tag->value);
			required |= xml_required_field(9);	
			ControlSystemData(SFIELD_SET_OSDTEXT, (void *)TextOverlay.displayText, strlen(TextOverlay.displayText));
		}
		else if(strcmp(tag->name, TAG_horizontalAlignType) == 0) {
			strcpy(TextOverlay.horizontalAlignType, tag->value);
			required |= xml_required_field(10);			
		}
		else if(strcmp(tag->name, TAG_verticalAlignType) == 0) {
			strcpy(TextOverlay.verticalAlignType, tag->value);
			required |= xml_required_field(11);			
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fields were present */
	if(required != xml_required_mask(11))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int delete_system_video_channels_id_overlays_text_id(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_system_video_channels_id_overlays_image_id(request *req, node_t *node)
{
	const char *id = "1";
	const char *enabled = "TRUE";
	const char *videoInputID = "Auto";
	const char *imageName = "1";
	const char *positionX = "1";
	const char *positionY = "1";
	const char *transparentColorEnabled = "TRUE";
	const char *transparentColor = "1";	

	xadd_stag_attr(req, TAG_ImageOverlay, RootAttrStr);
	xadd_elem(req, TAG_id, id);
	xadd_elem(req, TAG_enabled, enabled);
	xadd_elem(req, TAG_videoInputID, videoInputID);
	xadd_elem(req, TAG_imageName, imageName);	
	xadd_elem(req, TAG_positionX, positionX);
    xadd_elem(req, TAG_positionY, positionY);
    xadd_elem(req, TAG_transparentColorEnabled, transparentColorEnabled);    
	xadd_elem(req, TAG_transparentColor, transparentColor);	
	xadd_etag(req, TAG_ImageOverlay);

	return STATUS_OK;
}

int update_system_video_channels_id_overlays_image_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;	
	ImageOverlay_psia ImageOverlay;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_ImageOverlay) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(ImageOverlay.id, tag->value);
			required |= xml_required_field(1);            
		}
		else if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(ImageOverlay.enabled, tag->value);
			required |= xml_required_field(2);
		}	
		else if(strcmp(tag->name, TAG_videoInputID) == 0) {
			strcpy(ImageOverlay.videoInputID, tag->value);
			required |= xml_required_field(3);			
		}
		else if(strcmp(tag->name, TAG_imageName) == 0) {
			strcpy(ImageOverlay.imageName, tag->value);
			required |= xml_required_field(4);			
		}
		else if(strcmp(tag->name, TAG_positionX) == 0) {
			strcpy(ImageOverlay.positionX, tag->value);
			required |= xml_required_field(5);			
		}
		else if(strcmp(tag->name, TAG_positionY) == 0) {
			strcpy(ImageOverlay.positionY, tag->value);
			required |= xml_required_field(6);			
		}
		else if(strcmp(tag->name, TAG_transparentColorEnabled) == 0) {
			strcpy(ImageOverlay.transparentColorEnabled, tag->value);
			required |= xml_required_field(7);			
		}
		else if(strcmp(tag->name, TAG_transparentColor) == 0) {
			strcpy(ImageOverlay.transparentColor, tag->value);
			required |= xml_required_field(8);			
		}		
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fields were present */
	if(required != xml_required_mask(8))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int delete_system_video_channels_id_overlays_image_id(request *req, node_t *node)
{
	return STATUS_OK;
}


/*******************************************************************/
/*                          System/Audio                         */
/*******************************************************************/

int retrieve_system_audio_channels(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_AudioChannelList, RootAttrStr);
	for(n = 0; n < get_channel_count(); n++)
		retrieve_system_audio_channels_id(req, node);
	xadd_etag(req, TAG_AudioChannelList);

	return STATUS_OK;
}

int retrieve_system_audio_channels_id(request *req, node_t *node)
{
	const char *id = "1";
	const char *enabled = "TRUE";
	const char *audioMode = "Mic";
	const char *microphoneEnabled = "TRUE";
	const char *microphoneSource = "1";
	const char *microphoneVolume = "1";
	const char *speakerEnabled = "TRUE";
	const char *speakerVolume = "2";

	xadd_stag_attr(req, TAG_AudioChannel, RootAttrStr);
	xadd_elem(req, TAG_id, id);
	xadd_elem(req, TAG_enabled, enabled);
	xadd_elem(req, TAG_audioMode, audioMode);
	xadd_elem(req, TAG_microphoneEnabled, microphoneEnabled);
	xadd_elem(req, TAG_microphoneSource, microphoneSource);
	xadd_elem(req, TAG_microphoneVolume, microphoneVolume);
	xadd_elem(req, TAG_speakerEnabled, speakerEnabled);
	xadd_elem(req, TAG_speakerVolume, speakerVolume);
	xadd_etag(req, TAG_AudioChannel);

	return STATUS_OK;
}

int update_system_audio_channels_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;	
	AudioChannel_psia AudioChannel;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_AudioChannel) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(AudioChannel.id, tag->value);
			required |= xml_required_field(1);            
		}
		else if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(AudioChannel.enabled, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_audioMode) == 0) {
			strcpy(AudioChannel.audioMode, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_microphoneEnabled) == 0) {
			strcpy(AudioChannel.microphoneEnabled, tag->value);
			required |= xml_required_field(4);
		}
		else if(strcmp(tag->name, TAG_microphoneSource) == 0) {
			strcpy(AudioChannel.microphoneSource, tag->value);
			required |= xml_required_field(5);
		}
		else if(strcmp(tag->name, TAG_microphoneVolume) == 0) {
			strcpy(AudioChannel.microphoneVolume, tag->value);
			required |= xml_required_field(6);
		}
		else if(strcmp(tag->name, TAG_speakerEnabled) == 0) {
			strcpy(AudioChannel.speakerEnabled, tag->value);
			required |= xml_required_field(7);
		}
		else if(strcmp(tag->name, TAG_speakerVolume) == 0) {
			strcpy(AudioChannel.speakerVolume, tag->value);
			required |= xml_required_field(8);
			ControlSystemData(SFIELD_SET_AUDIOINVOLUME, &AudioChannel.speakerVolume, sizeof(AudioChannel.speakerVolume));
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fields were present */
	if(required != xml_required_mask(14))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

/*******************************************************************/
/*                              System                             */
/*******************************************************************/
int do_system_reboot(request *req, node_t *node)
{
	return STATUS_REBOOT_REQUIRED;
}

int do_system_updatefirmware(request *req, node_t *node)
{
	/* update firmware of inlen bytes starting at inbuf */
	return STATUS_OK;
}

int retrieve_system_configurationdata(request *req, node_t *node)
{
	return STATUS_OK;
}

int update_system_configurationdata(request *req, node_t *node)
{
	return STATUS_OK;
}

int do_system_factory_reset(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_system_deviceinfo(request *req, node_t *node)
{
	const char *device_name = "DM368IPNC";
	const char *device_id = "DM368IPNC-MT5-A033";
	const char *model = "400S";
	//const char *mac_address = "00:0C:0C:A0:02:40";
	char mac_address[12];
	char mac[6];
	const char *firmware_version = "2.1";
	const char *firmware_released_date = "090630";
	const char *boot_version = "1.2";
	const char *hardware_version = "1.2a";   	
	
	net_get_hwaddr(ETH_NAME, mac);	
	sprintf(mac_address,"%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);	

	xadd_stag_attr(req, TAG_DeviceInfo, RootAttrStr);	
	xadd_elem(req, TAG_deviceName, device_name);
	xadd_elem(req, TAG_deviceID, device_id);
	xadd_elem(req, TAG_model, model);
	xadd_elem(req, TAG_macAddress, mac_address);
	xadd_elem(req, TAG_firmwareVersion, firmware_version);
	xadd_elem(req, TAG_firmwareReleasedDate, firmware_released_date);
	xadd_elem(req, TAG_bootVersion, boot_version);
	xadd_elem(req, TAG_hardwareVersion, hardware_version);
	xadd_etag(req, TAG_DeviceInfo);	

	return STATUS_OK;
}

int update_system_deviceinfo(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;	
	DeviceInfo_psia DeviceInfo;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_DeviceInfo) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_deviceName) == 0) {
			strcpy(DeviceInfo.device_name, tag->value);
			required |= xml_required_field(1);
			ControlSystemData(SFIELD_SET_TITLE, (void *)DeviceInfo.device_name, strlen(DeviceInfo.device_name));
		}
		else if(strcmp(tag->name, TAG_model) == 0) {
			strcpy(DeviceInfo.model, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_macAddress) == 0) {
			strcpy(DeviceInfo.mac_address, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_firmwareVersion) == 0) {
			strcpy(DeviceInfo.firmware_version, tag->value);
			required |= xml_required_field(4);
		}
		else if(strcmp(tag->name, TAG_firmwareReleasedDate) == 0) {
			strcpy(DeviceInfo.firmware_released_date, tag->value);
			required |= xml_required_field(5);
		}
		else if(strcmp(tag->name, TAG_bootVersion) == 0) {
			strcpy(DeviceInfo.boot_version, tag->value);
			required |= xml_required_field(6);
		}
		else if(strcmp(tag->name, TAG_hardwareVersion) == 0) {
			strcpy(DeviceInfo.hardware_version, tag->value);
			required |= xml_required_field(7);
		}		
		/* Protocol */

		tag = tag->next;
	}

	/* check all the required fields were present */
	if(required != xml_required_mask(7))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_system_supportreport(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_system_status(request *req, node_t *node)
{
	const char *current_device_time = "Wed Jun 30 21:49:08 2007";
	const char *device_up_time = "1000";
	const char *cpu_description = "X-Processor 100MHz";
	const char *cpu_utilization = "45";
	const char *memory_description = "System SDRAM";
	const char *memory_usage = "15.5";
	const char *memory_available = "16.5";

	xadd_stag_attr(req, TAG_DeviceStatus, RootAttrStr);
	xadd_elem(req, TAG_currentDeviceTime, current_device_time);
	xadd_elem(req, TAG_deviceUpTime, device_up_time);

	xadd_stag(req, TAG_CPUList);
	xadd_stag(req, TAG_CPU);
	xadd_elem(req, TAG_cpuDescription, cpu_description);
	xadd_elem(req, TAG_cpuUtilization, cpu_utilization);
	xadd_etag(req, TAG_CPU);
	xadd_etag(req, TAG_CPUList);

	xadd_stag(req, TAG_MemoryList);
	xadd_stag(req, TAG_Memory);
	xadd_elem(req, TAG_memoryDescription, memory_description);
	xadd_elem(req, TAG_memoryUsage, memory_usage);
	xadd_elem(req, TAG_memoryAvailable, memory_available);
	xadd_etag(req, TAG_Memory);
	xadd_etag(req, TAG_MemoryList);
	
	xadd_etag(req, TAG_DeviceStatus);
	return STATUS_OK;
}

int retrieve_system_time(request *req, node_t *node)
{
	const char *timemode = "Manual/NTP";
	const char *localtime = "13.00.32 21/07/2009";
	const char *timezone = "GMT+5.30";

	xadd_stag_attr(req, TAG_Time, RootAttrStr);
	xadd_elem(req, TAG_deviceName, timemode);
	xadd_elem(req, TAG_model, localtime);
	xadd_elem(req, TAG_macAddress, timezone);	
	xadd_etag(req, TAG_Time);

	return STATUS_OK;
}

int update_system_time(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;	
	Time_psia Time;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_Time) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_timeMode) == 0) {
			strcpy(Time.timemode, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_localTime) == 0) {
			strcpy(Time.localtime, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_timeZone) == 0) {
			strcpy(Time.timezone, tag->value);
			required |= xml_required_field(3);				
		}		
		/* Protocol */

		tag = tag->next;
	}

	/* check all the required fields were present */
	if(required != xml_required_mask(3))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_system_time_localtime(request *req, node_t *node)
{
	const char *localtime;

	//get_date(request *req, COMMAND_ARGUMENT *argm);
	//get_time(request *req, COMMAND_ARGUMENT *argm);

	return STATUS_OK;
}

int update_system_time_localtime(request *req, node_t *node)
{
	const char *localtime = "13.00.32 21/07/2009";

	//sys_set_date(int year, int month, int day);
	//sys_set_time(int hour, int min, int sec);

	return STATUS_OK;
}

int retrieve_system_time_timezone(request *req, node_t *node)
{
	const char *timezone = "GMT+5.30";

	return STATUS_OK;
}

int update_system_time_timezone(request *req, node_t *node)
{
	const char *timezone = "GMT+5.30";

	ControlSystemData(SFIELD_SET_TIMEZONE, (void *)&timezone, sizeof(timezone));

	return STATUS_OK;
}

int retrieve_system_time_ntpservers(request *req, node_t *node)
{
    int c;
	char idstr[64];

	xadd_stag_attr(req, TAG_NtpServerList, RootAttrStr);
	for(c = 0; c < get_ntpserver_count(); c++) {
		get_ntpserver_url(c, idstr);
		retrieve_system_time_ntpservers_id(req, node);
	}
	xadd_etag(req, TAG_NtpServerList);

	return STATUS_OK;
}

int update_system_time_ntpservers(request *req, node_t *node)
{
	int c;
	char idstr[64];
	
	for(c = 0; c < get_ntpserver_count(); c++) {
		get_ntpserver_url(c, idstr);
		update_system_time_ntpservers_id(req, node);
	}
	return STATUS_OK;
}

int create_system_time_ntpservers(request *req, node_t *node)
{
	return STATUS_OK;
}

int delete_system_time_ntpservers(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_system_time_ntpservers_id(request *req, node_t *node)
{
	int i;
	const char *hostname = "IPNetCam";
	const char *ip_address = "172.24.190.182";
	const char *ipV6_address = "XX.XX";
	const char *portNo = "80";

	xadd_stag_attr(req, TAG_NtpServer, RootAttrStr);	
	xadd_elem(req, TAG_addressingFormatType,"ipAddress/hostName");
	xadd_elem(req, TAG_hostName,hostname);
	xadd_elem(req, TAG_ipAddress, ip_address);	
	xadd_elem(req, TAG_ipv6Address,ipV6_address);
	xadd_elem(req, TAG_portNo, portNo);	
	xadd_etag(req, TAG_NtpServer);	
	
	return STATUS_OK;
}

int update_system_time_ntpservers_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;	
	NtpServer_psia NtpServer;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_NtpServer) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_hostName) == 0) {
			strcpy(NtpServer.hostname, tag->value);
			required |= xml_required_field(1);
			ControlSystemData(SFIELD_SET_SNTP_SERVER, (void *)NtpServer.hostname, strlen(NtpServer.hostname));
		}
		else if(strcmp(tag->name, TAG_ipAddress) == 0) {
			strcpy(NtpServer.ip_address, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_ipv6Address) == 0) {
			strcpy(NtpServer.ipV6_address, tag->value);
			required |= xml_required_field(3);				
		}
		else if(strcmp(tag->name, TAG_portNo) == 0) {
			strcpy(NtpServer.portNo, tag->value);
			required |= xml_required_field(4);				
		}
		/* Protocol */

		tag = tag->next;
	}

	/* check all the required fields were present */
	if(required != xml_required_mask(4))
		return STATUS_INVALID_XML_CONTENT;
	return STATUS_OK;
}

int delete_system_time_ntpservers_id(request *req, node_t *node)
{
	return STATUS_OK;
}

int get_ntpserver_count(void)
{
	return 1;
}

void get_ntpserver_url(int id, char *idstr)
{
	/* default ID 0,1,2,3... */
	sprintf(idstr,"/%d", id);
}

int get_ntpserver_index(char *idstr)
{
	idstr++;  // skip the '/'
	return(atoi(idstr));
}

/*******************************************************************/
/*                             Security                            */
/*******************************************************************/

static User_psia DemoUserList[MAX_USERS] = {
	{"0", 	"John",		"john1234",	"0"},
	{"1",	"Mike",		"mike@home",	"2"},
	{"2",	"Alan",		"aaa",		"3"},
	{"4",	"Bruce",	"bbb",		"1"},
};
static int DemoUserCount = 4;

int get_user_count(void)
{
	return DemoUserCount;
}

/* create the url based on the user index */
void get_user_url(int i, char *idstr)
{
	sprintf(idstr, "/%s", DemoUserList[i].id);
}

int retieve_security_aaa_users(request *req, node_t *node)
{
	int i;
	char idstr[64];

	xadd_stag_attr(req, TAG_UserList, RootAttrStr);
	for(i = 0; i < get_user_count(); i++) {
		req->list_index = i;
		get_user_url(i, idstr);
		retieve_security_aaa_users_id(req, node);
	}
	xadd_etag(req, TAG_UserList);
	return STATUS_OK;
}

int update_security_aaa_users(request *req, node_t *node)
{
	/* update a user list */
	int i;
	char idstr[64];
	
	for(i = 0; i < get_user_count(); i++) {
		req->list_index = i;
		get_user_url(i, idstr);
		update_security_aaa_users_id(req, node);
	}	
	return STATUS_OK;
}

int create_security_aaa_users(request *req, node_t *node)
{
	int ret;
	tag_t	*tag;
	unsigned long required;
	User_psia User;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_User) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(User.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_userName) == 0) {
			strcpy(User.userName, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_password) == 0) {
			strcpy(User.password, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_privilegeLevel) == 0) {
			strcpy(User.privilegeLevel, tag->value);
			required |= xml_required_field(4);
		}
		/* Protocol */

		tag = tag->next;
	}

    ret = ControlSystemData(SFIELD_ADD_USER, (void *)&User, sizeof(User));
	/* check all the required fileds were present */
	if(required != xml_required_mask(4))
		return STATUS_INVALID_XML_CONTENT;

	/* <!--- add new user to list ---> */
	return STATUS_OK;
}

int delete_security_aaa_users(request *req, node_t *node)
{
	/* delete users from the list */
	//DemoUserCount = 0;
	int i;
	char idstr[64];
	
	for(i = 0; i < get_user_count(); i++) {
		req->list_index = i;
		get_user_url(i, idstr);
		delete_security_aaa_users_id(req, node);
	}	
	return STATUS_OK;	
}

int retieve_security_aaa_users_id(request *req, node_t *node)
{
	int i;
	User_psia *user;

	/* either use the node->name, or the list_index */
	if(strcmp("/users", node->name) == 0)
		i = req->list_index;
	/* get the index from the node->name */
	else {
		for(i = 0; i < get_user_count(); i++) {
			user = &DemoUserList[i];
			if(strcmp(user->id, &node->name[1]) == 0)  // skip the '/'
				break;
		}
	}

	if(i >= get_user_count())
		return STATUS_INVALID_OPERATION;  // shouldn't be possible

	user = &DemoUserList[i];
	xadd_stag_attr(req, TAG_User, RootAttrStr);
	xadd_elem(req, TAG_id, user->id);
	xadd_elem(req, TAG_userName, user->userName);
	/* password is read only */
//	xadd_elem(req, TAG_password, user->password);
	xadd_elem(req, TAG_privilegeLevel, user->privilegeLevel);
//	xadd_stag(req, TAG_ProtocolList);
//	xadd_stag(req, TAG_Protocol);
//	xadd_elem(req, TAG_protocol, get_protocol());
//	xadd_stag(req, TAG_Protocol);
//	xadd_stag(req, TAG_ProtocolList);
	xadd_etag(req, TAG_User);

	return STATUS_OK;
}

int update_security_aaa_users_id(request *req, node_t *node)
{
	return STATUS_OK;
}

int delete_security_aaa_users_id(request *req, node_t *node)
{
	return STATUS_OK;
}

/*******************************************************************/
/*                            Streaming                            */
/*******************************************************************/

int get_channel_count(void)
{
	return 1;
}

void get_channel_url(int id, char *idstr)
{
	/* default ID 0,1,2,3... */
	sprintf(idstr,"/%d", id);
}

int get_channel_index(char *idstr)
{
	idstr++;  // skip the '/'
	return(atoi(idstr));
}

int retrieve_streaming_status(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_streaming_channels(request *req, node_t *node)
{
	int c;
	char idstr[64];

	xadd_stag_attr(req, TAG_StreamingChannelList, RootAttrStr);
	for(c = 0; c < get_channel_count(); c++) {
		get_channel_url(c, idstr);
		retrieve_streaming_channels_id(req, node);
	}
	xadd_etag(req, TAG_StreamingChannelList);

	return STATUS_OK;
}

int retrieve_streaming_channels_id(request *req, node_t *node)
{	
	const char *channelName = "MJPEG Stream";	
	const char *rtspPortNo = "8555";	
	const char *maxPacketSize = "1490";		
	const char *enabled = "1";
	const char *disabled = "0";
	const char *videoCodecType = "MJPEG";	
	const char *videoScanType = "interlaced";
	const char *fixedQuality = "High";
	const char *videoResolutionWidth = "720";
	const char *videoResolutionHeight = "480";
	const char *maxFrameRate100 = "3000";		
    const char *id = "0";
	const char *rotationDegree = "0";
	const char *mirrorEnabled = "TRUE";
	const char *audioInputChannelID = "1";
	const char *audioCompressionType = "G.711ulaw";
	const char *audioBitRate = "4000";
	const char *audioSamplingRate = "2";

	xadd_stag_attr(req, TAG_StreamingChannel, RootAttrStr);
	xadd_elem(req, TAG_id, id);
	xadd_elem(req, TAG_channelName, channelName);
	xadd_elem(req, TAG_enabled, enabled);
	xadd_stag(req, TAG_Transport);
	xadd_elem(req, TAG_rtspPortNo,rtspPortNo);
	xadd_elem(req, TAG_maxPacketSize, maxPacketSize);	
	xadd_stag(req, TAG_ControlProtocolList);
	xadd_stag(req, TAG_ControlProtocol);
	xadd_elem(req, TAG_streamingTransport, "http");
	xadd_etag(req, TAG_ControlProtocol);
	xadd_etag(req, TAG_ControlProtocolList);
	xadd_stag(req, TAG_Unicast);
	xadd_elem(req, TAG_enabled, enabled);
	xadd_elem(req, TAG_interfaceID, "0");
	xadd_elem(req, TAG_rtpTransportType, "RTP/TCP");
	xadd_etag(req, TAG_Unicast);
	xadd_stag(req, TAG_Multicast);
	xadd_elem(req, TAG_enabled, enabled);
	xadd_etag(req, TAG_Multicast);
	xadd_stag(req, TAG_Security);
	xadd_elem(req, TAG_enabled, disabled);
	xadd_etag(req, TAG_Security);
	xadd_etag(req, TAG_Transport);
	xadd_stag(req, TAG_Video);
	xadd_elem(req, TAG_enabled, enabled);
	xadd_elem(req, TAG_videoCodecType, videoCodecType);
	xadd_elem(req, TAG_videoScanType, videoScanType);
	xadd_elem(req, TAG_videoResolutionWidth, videoResolutionWidth);
	xadd_elem(req, TAG_videoResolutionHeight, videoResolutionHeight);
	xadd_elem(req, TAG_fixedQuality, fixedQuality);
	xadd_elem(req, TAG_maxFrameRate, maxFrameRate100);
	xadd_elem(req, TAG_rotationDegree, rotationDegree);
	xadd_elem(req, TAG_mirrorEnabled, mirrorEnabled);
	xadd_etag(req, TAG_Video);
	xadd_stag(req, TAG_Audio);
    xadd_elem(req, TAG_enabled, enabled);
	xadd_elem(req, TAG_audioInputChannelID, audioInputChannelID);
	xadd_elem(req, TAG_audioCompressionType, audioCompressionType);
    xadd_elem(req, TAG_audioBitRate, audioBitRate);
	xadd_elem(req, TAG_audioSamplingRate, audioSamplingRate);
	xadd_etag(req, TAG_Audio);	
	xadd_etag(req, TAG_StreamingChannel);
	return STATUS_OK;
}

int update_streaming_channels(request *req, node_t *node)
{
	int c;
	char idstr[64];

	xadd_stag_attr(req, TAG_StreamingChannelList, RootAttrStr);
	for(c = 0; c < get_channel_count(); c++) {
		get_channel_url(c, idstr);
		update_streaming_channels_id(req, node);
	}
	xadd_etag(req, TAG_StreamingChannelList);	
	return STATUS_OK;
}

int update_streaming_channels_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;	
	StreamingChannel_psia StreamingChannel;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_StreamingChannel) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_channelName) == 0) {
			strcpy(StreamingChannel.channelName, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_rtspPortNo) == 0) {
			strcpy(StreamingChannel.rtspPortNo, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_maxPacketSize) == 0) {
			strcpy(StreamingChannel.maxPacketSize, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(StreamingChannel.enabled, tag->value);
			required |= xml_required_field(4);
		}
		else if(strcmp(tag->name, TAG_disabled) == 0) {
			strcpy(StreamingChannel.disabled, tag->value);
			required |= xml_required_field(5);
		}
		else if(strcmp(tag->name, TAG_videoInputChannelID) == 0) {
			strcpy(StreamingChannel.videoInputChannelID, tag->value);
			required |= xml_required_field(6);
            ControlSystemData(SFIELD_SET_VIDEO_MODE, (void *)&StreamingChannel.videoInputChannelID, sizeof(StreamingChannel.videoInputChannelID));
		}
		else if(strcmp(tag->name, TAG_videoCodecType) == 0) {
			strcpy(StreamingChannel.videoCodecType, tag->value);
			required |= xml_required_field(7);
			ControlSystemData(SFIELD_SET_VIDEOCODECMODE, (void *)&StreamingChannel.videoCodecType, sizeof(StreamingChannel.videoCodecType));
		}
		else if(strcmp(tag->name, TAG_videoScanType) == 0) {
			strcpy(StreamingChannel.videoScanType, tag->value);
			required |= xml_required_field(8);
		}
		else if(strcmp(tag->name, TAG_fixedQuality) == 0) {
			strcpy(StreamingChannel.fixedQuality, tag->value);
			required |= xml_required_field(9);
			ControlSystemData(SFIELD_SET_JQUALITY, (void *)&StreamingChannel.fixedQuality, sizeof(StreamingChannel.fixedQuality));
		}
		else if(strcmp(tag->name, TAG_videoResolutionWidth) == 0) {
			strcpy(StreamingChannel.videoResolutionWidth, tag->value);
			required |= xml_required_field(10);
			ControlSystemData(SFIELD_SET_VIDEOCODECRES, (void *)&StreamingChannel.videoResolutionWidth, sizeof(StreamingChannel.videoResolutionWidth));
		}
		else if(strcmp(tag->name, TAG_videoResolutionHeight) == 0) {
			strcpy(StreamingChannel.videoResolutionHeight, tag->value);
			required |= xml_required_field(11);
			ControlSystemData(SFIELD_SET_VIDEOCODECRES, (void *)&StreamingChannel.videoResolutionHeight, sizeof(StreamingChannel.videoResolutionHeight));
		}
		else if(strcmp(tag->name, TAG_videoQualityControlType) == 0) {
			strcpy(StreamingChannel.videoQualityControlType, tag->value);
			required |= xml_required_field(12);
			ControlSystemData(SFIELD_SET_VIDEOCODECRES, (void *)&StreamingChannel.videoQualityControlType, sizeof(StreamingChannel.videoQualityControlType));
		}
		else if(strcmp(tag->name, TAG_constantBitRate) == 0) {
			strcpy(StreamingChannel.constantBitRate, tag->value);
			required |= xml_required_field(13);
			ControlSystemData(SFIELD_SET_MPEG41_BITRATE, (void *)&StreamingChannel.constantBitRate, sizeof(StreamingChannel.constantBitRate));
		}		
		else if(strcmp(tag->name, TAG_maxFrameRate) == 0) {
			strcpy(StreamingChannel.maxFrameRate, tag->value);
			required |= xml_required_field(14);
			ControlSystemData(SFIELD_SET_MPEG41_FRAMERATE, (void *)&StreamingChannel.maxFrameRate, sizeof(StreamingChannel.maxFrameRate));
		}
		else if(strcmp(tag->name, TAG_rotationDegree) == 0) {
			strcpy(StreamingChannel.rotationDegree, tag->value);
			required |= xml_required_field(15);
			ControlSystemData(SFIELD_SET_ROTCTRL, (void *)&StreamingChannel.rotationDegree, sizeof(StreamingChannel.rotationDegree));
		}
		else if(strcmp(tag->name, TAG_mirrorEnabled) == 0) {
			strcpy(StreamingChannel.mirrorEnabled, tag->value);
			required |= xml_required_field(16);
			ControlSystemData(SFIELD_SET_ROTCTRL, (void *)&StreamingChannel.mirrorEnabled, sizeof(StreamingChannel.mirrorEnabled));
		}
		else if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(StreamingChannel.audioEnabled, tag->value);
			required |= xml_required_field(17);
			ControlSystemData(SFIELD_SET_AUDIOENABLE, (void *)&StreamingChannel.audioEnabled, sizeof(StreamingChannel.audioEnabled));
		}
		else if(strcmp(tag->name, TAG_audioInputChannelID) == 0) {
			strcpy(StreamingChannel.audioInputChannelID, tag->value);
			required |= xml_required_field(18);			
		}
		else if(strcmp(tag->name, TAG_audioCompressionType) == 0) {
			strcpy(StreamingChannel.audioCompressionType, tag->value);
			required |= xml_required_field(19);			
		}
		else if(strcmp(tag->name, TAG_audioBitRate) == 0) {
			strcpy(StreamingChannel.audioBitRate, tag->value);
			required |= xml_required_field(20);			
		}
		else if(strcmp(tag->name, TAG_audioSamplingRate) == 0) {
			strcpy(StreamingChannel.audioSamplingRate, tag->value);
			required |= xml_required_field(21);			
		}
		/* Protocol */
		tag = tag->next;
	}

	/* check all the required fileds were present */
	if(required != xml_required_mask(11))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int create_streaming_channels(request *req, node_t *node)
{
	return STATUS_OK;
}

int delete_streaming_channels(request *req, node_t *node)
{
	return STATUS_OK;
}

int delete_streaming_channels_id(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_streaming_channels_id_status(request *req, node_t *node)
{
	const char *ipAddress;
	const char *clientUserName;
	const char *startDateTime;		
	const char *elapsedTime;
	const char *bandwidth;

	xadd_stag_attr(req, TAG_StreamingSessionStatusList, RootAttrStr);
	xadd_stag_attr(req, TAG_StreamingSessionStatus, RootAttrStr);		
	xadd_stag(req, TAG_clientAddress);
	xadd_elem(req, TAG_ipAddress,ipAddress);	
	xadd_etag(req, TAG_clientAddress);	
	xadd_elem(req, TAG_clientUserName, clientUserName);
	xadd_elem(req, TAG_startDateTime, startDateTime);
	xadd_elem(req, TAG_elapsedTime, elapsedTime);	
	xadd_elem(req, TAG_bandwidth, bandwidth);	
	xadd_etag(req, TAG_StreamingSessionStatus);	
	xadd_etag(req, TAG_StreamingSessionStatusList);

	return STATUS_OK;
}

int retrieve_streaming_channels_id_http(request *req, node_t *node)
{
#if 0
	StreamingChannel_psia StreamingChannel;

	if(strcmp(StreamingChannel.videoCodecType,"MJPEG")==0)
	    uri_mjpeg(req);
	else if(strcmp(StreamingChannel.videoCodecType,"H.264")==0)
		uri_avc(req);
	else if(strcmp(StreamingChannel.videoCodecType,"MPEG4")==0)
		uri_mpeg4(req);
#endif
    uri_mjpeg(req);
	return STATUS_OK;
}

int retrieve_streaming_channels_id_picture(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_streaming_channels_id_http_capabilities(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_streaming_channels_id_picture_capabilities(request *req, node_t *node)
{
	return STATUS_OK;
}

/*******************************************************************/
/*                            Custom/Event                         */
/*******************************************************************/
int get_event_count(void)
{
	return 1;
}

int retrieve_custom_event(request *req, node_t *node)
{
	xadd_stag_attr(req, TAG_EventNotification, RootAttrStr);

	retrieve_custom_event_triggers(req, node);
	retrieve_custom_event_schedule(req, node);
	retrieve_custom_event_notification(req, node);

	xadd_etag(req, TAG_EventNotification);

	return STATUS_OK;
}

int update_custom_event(request *req, node_t *node)
{
	update_custom_event_triggers(req, node);
	update_custom_event_schedule(req, node);
	update_custom_event_notification(req, node);

	return STATUS_OK;
}

int retrieve_custom_event_triggers(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_EventTriggerList, RootAttrStr);
	for(n = 0; n < get_event_count(); n++)
		retrieve_custom_event_triggers_id(req, node);
	xadd_etag(req, TAG_EventTriggerList);

	return STATUS_OK;
}

int update_custom_event_triggers(request *req, node_t *node)
{
	int n;
	
	for(n = 0; n < get_event_count(); n++)
		update_custom_event_triggers_id(req, node);
	
	return STATUS_OK;
}

int create_custom_event_triggers(request *req, node_t *node)
{
	return STATUS_OK;
}

int delete_custom_event_triggers(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_custom_event_triggers_id(request *req, node_t *node)
{
	const char *id = "1";
	const char *eventType = "IO";
	const char *eventDescription = "Event";		
	const char *inputIOPortID = "1";
	const char *intervalBetweenEvents = "10";	

	xadd_stag_attr(req, TAG_EventTrigger, RootAttrStr);
	xadd_elem(req, TAG_id, id);
	xadd_elem(req, TAG_eventType, eventType);
	xadd_elem(req, TAG_eventDescription, eventDescription);
	xadd_elem(req, TAG_inputIOPortID, inputIOPortID);
	xadd_elem(req, TAG_intervalBetweenEvents, intervalBetweenEvents);
	retrieve_custom_event_triggers_id_notifications(req,node);		
	xadd_etag(req, TAG_EventTrigger);

	return STATUS_OK;
}

int update_custom_event_triggers_id(request *req, node_t *node)
{
	int ret;
	tag_t	*tag;
	unsigned long required;
	EventTrigger_psia EventTrigger;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_EventTrigger) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(EventTrigger.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_eventType) == 0) {
			strcpy(EventTrigger.eventType, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_eventDescription) == 0) {
			strcpy(EventTrigger.eventDescription, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_inputIOPortID) == 0) {
			strcpy(EventTrigger.inputIOPortID, tag->value);
			required |= xml_required_field(4);
		}
		else if(strcmp(tag->name, TAG_intervalBetweenEvents) == 0) {
			strcpy(EventTrigger.intervalBetweenEvents, tag->value);
			required |= xml_required_field(5);
			ControlSystemData(SFIELD_SET_ALARMDURATION, &EventTrigger.intervalBetweenEvents, sizeof(EventTrigger.intervalBetweenEvents));
		}
		else if(strcmp(tag->name, TAG_EventTriggerNotificationList) == 0) {
			update_custom_event_triggers_id_notifications(req,node);
			required |= xml_required_field(6);
		}
		/* Protocol */
		tag = tag->next;
	}
	
	/* check all the required fileds were present */
	if(required != xml_required_mask(6))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int delete_custom_event_triggers_id(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_custom_event_triggers_id_notifications(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_EventTriggerNotificationList, RootAttrStr);
	for(n = 0; n < get_event_count(); n++)
		retrieve_custom_event_triggers_id(req, node);
	xadd_etag(req, TAG_EventTriggerNotificationList);

	return STATUS_OK;
}

int update_custom_event_triggers_id_notifications(request *req, node_t *node)
{
	int n;
	
	for(n = 0; n < get_event_count(); n++)
		update_custom_event_triggers_id_notifications_id(req, node);
	
	return STATUS_OK;
}

int create_custom_event_triggers_id_notifications(request *req, node_t *node)
{
	return STATUS_OK;
}

int delete_custom_event_triggers_id_notifications(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_custom_event_triggers_id_notifications_id(request *req, node_t *node)
{
	const char *id = "1";
	const char *notificationMethod = "email";
	const char *notificationRecurrence = "recurring";		
	const char *notificationInterval = "1";
	
	xadd_stag_attr(req, TAG_EventTriggerNotification, RootAttrStr);
	xadd_elem(req, TAG_id, id);
	xadd_elem(req, TAG_notificationMethod, notificationMethod);
	xadd_elem(req, TAG_notificationRecurrence, notificationRecurrence);
	xadd_elem(req, TAG_notificationInterval, notificationInterval);
	xadd_etag(req, TAG_EventTriggerNotification);

	return STATUS_OK;
}

int update_custom_event_triggers_id_notifications_id(request *req, node_t *node)
{
	int ret;
	tag_t	*tag;
	unsigned long required;
	EventTriggerNotification_psia EventTriggerNotification;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_EventTrigger) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(EventTriggerNotification.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_notificationMethod) == 0) {
			strcpy(EventTriggerNotification.notificationMethod, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_notificationRecurrence) == 0) {
			strcpy(EventTriggerNotification.notificationRecurrence, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_notificationInterval) == 0) {
			strcpy(EventTriggerNotification.notificationInterval, tag->value);
			required |= xml_required_field(4);
			ControlSystemData(SFIELD_SET_AVIDURATION, (void *)&EventTriggerNotification.notificationInterval, sizeof(EventTriggerNotification.notificationInterval));
		}		
		/* Protocol */
		tag = tag->next;
	}
	
	/* check all the required fileds were present */
	if(required != xml_required_mask(4))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int delete_custom_event_triggers_id_notifications_id(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_custom_event_schedule(request *req, node_t *node)
{
	const char *beginDateTime = "1";
	const char *endDateTime = "2";
		
	xadd_stag_attr(req, TAG_EventSchedule,RootAttrStr);
	xadd_stag(req, TAG_DateTimeRange);
	xadd_elem(req, TAG_beginDateTime, beginDateTime);
	xadd_elem(req, TAG_endDateTime, endDateTime);
	xadd_etag(req, TAG_DateTimeRange);
	/*TimeBlockList*/
	xadd_etag(req, TAG_EventSchedule);

	return STATUS_OK;
}

int update_custom_event_schedule(request *req, node_t *node)
{	
	tag_t	*tag;
	unsigned long required;
	EventSchedule_psia EventSchedule;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_EventTrigger) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_beginDateTime) == 0) {
			strcpy(EventSchedule.beginDateTime, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_endDateTime) == 0) {
			strcpy(EventSchedule.endDateTime, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_TimeBlockList) == 0) {
			/*TimeBlockList*/
			required |= xml_required_field(3);
		}
		/* Protocol */
		tag = tag->next;
	}
	ControlSystemData(SFIELD_SET_SCHEDULE, &EventSchedule, sizeof(EventSchedule));
	
	/* check all the required fileds were present */
	if(required != xml_required_mask(3))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_custom_event_notification(request *req, node_t *node)
{
	const char *uploadVideoClipEnabled = "TRUE";
	const char *senderEmailAddress = "email1";
	const char *receiverEmailAddress = "email2";		
	const char *videoClipFormatType = "AVI";
	
	xadd_stag_attr(req, TAG_EvenNotificationMethods, RootAttrStr);
	xadd_stag(req, TAG_FTPFormat);	
	xadd_elem(req, TAG_uploadVideoClipEnabled, uploadVideoClipEnabled);
	xadd_etag(req, TAG_FTPFormat);
	xadd_stag(req, TAG_EmailFormat);
	xadd_elem(req, TAG_senderEmailAddress, senderEmailAddress);
	xadd_elem(req, TAG_receiverEmailAddress, receiverEmailAddress);
	xadd_etag(req, TAG_EmailFormat);
	xadd_stag(req, TAG_MediaFormat);
	xadd_elem(req, TAG_videoClipFormatType, videoClipFormatType);	
	xadd_etag(req, TAG_MediaFormat);
	xadd_etag(req, TAG_EvenNotificationMethods);

	return STATUS_OK;
}

int update_custom_event_notification(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	EvenNotificationMethods_psia EvenNotificationMethods;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_EvenNotificationMethods) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_uploadVideoClipEnabled) == 0) {
			strcpy(EvenNotificationMethods.uploadVideoClipEnabled, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_senderEmailAddress) == 0) {
			strcpy(EvenNotificationMethods.senderEmailAddress, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_receiverEmailAddress) == 0) {
			strcpy(EvenNotificationMethods.receiverEmailAddress, tag->value);
			required |= xml_required_field(3);
			ControlSystemData(SFIELD_SET_SMTP_RECEIVER_EMAIL_ADDRESS, (void *)EvenNotificationMethods.receiverEmailAddress, strlen(EvenNotificationMethods.receiverEmailAddress));
		}
		else if(strcmp(tag->name, TAG_videoClipFormatType) == 0) {
			strcpy(EvenNotificationMethods.videoClipFormatType, tag->value);
			required |= xml_required_field(4);
		}
		/* Protocol */
		tag = tag->next;
	}
	
	/* check all the required fileds were present */
	if(required != xml_required_mask(4))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_custom_event_notification_mailing(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_MailingNotificationList, RootAttrStr);
	for(n = 0; n < get_event_count(); n++)
		retrieve_custom_event_notification_mailing_id(req, node);
	xadd_etag(req, TAG_MailingNotificationList);

	return STATUS_OK;
}

int update_custom_event_notification_mailing(request *req, node_t *node)
{
	int n;
	
	for(n = 0; n < get_event_count(); n++)
		update_custom_event_notification_mailing_id(req, node);
	
	return STATUS_OK;
}

int create_custom_event_notification_mailing(request *req, node_t *node)
{
	return STATUS_OK;
}

int delete_custom_event_notification_mailing(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_custom_event_notification_mailing_id(request *req, node_t *node)
{
	const char *id = "1";
	const char *authenticationMode = "1";
	const char *addressingFormatType = "ipaddress";		
	const char *hostName = "IPNetCam";
	const char *ipAddress = "172.24.190.119";
	const char *accountName = "1234";
	const char *password = "123";	
	
	xadd_stag_attr(req, TAG_MailingNotification, RootAttrStr);
	xadd_elem(req, TAG_id, id);
	xadd_elem(req, TAG_authenticationMode, authenticationMode);
	xadd_elem(req, TAG_addressingFormatType, addressingFormatType);
	xadd_elem(req, TAG_hostName, hostName);
	xadd_elem(req, TAG_ipAddress, ipAddress);
	xadd_elem(req, TAG_accountName, accountName);
	xadd_elem(req, TAG_password, password);
	xadd_etag(req, TAG_MailingNotification);

	return STATUS_OK;
}

int update_custom_event_notification_mailing_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	MailingNotification_psia MailingNotification;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_MailingNotification) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(MailingNotification.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_authenticationMode) == 0) {
			strcpy(MailingNotification.authenticationMode, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_addressingFormatType) == 0) {
			strcpy(MailingNotification.addressingFormatType, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_hostName) == 0) {
			strcpy(MailingNotification.hostName, tag->value);
			required |= xml_required_field(4);
			ControlSystemData(SFIELD_SET_SMTP_SENDER_EMAIL_ADDRESS, (void *)MailingNotification.hostName, strlen(MailingNotification.hostName));
		}
		else if(strcmp(tag->name, TAG_ipAddress) == 0) {
			strcpy(MailingNotification.ipAddress, tag->value);
			required |= xml_required_field(5);
			ControlSystemData(SFIELD_SET_SMTP_SERVER_IP, (void *)MailingNotification.ipAddress, strlen(MailingNotification.ipAddress));
		}
		else if(strcmp(tag->name, TAG_accountName) == 0) {
			strcpy(MailingNotification.accountName, tag->value);
			required |= xml_required_field(6);
			ControlSystemData(SFIELD_SET_SMTP_USERNAME, (void *)MailingNotification.accountName, strlen(MailingNotification.accountName));
		}
		else if(strcmp(tag->name, TAG_password) == 0) {
			strcpy(MailingNotification.password, tag->value);
			required |= xml_required_field(7);
			ControlSystemData(SFIELD_SET_SMTP_PASSWORD, (void *)MailingNotification.password, strlen(MailingNotification.password));
		}
		/* Protocol */
		tag = tag->next;
	}
	
	/* check all the required fileds were present */
	if(required != xml_required_mask(7))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int delete_custom_event_notification_mailing_id(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_custom_event_notification_ftp(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_FTPNotificationList, RootAttrStr);
	for(n = 0; n < get_event_count(); n++)
		retrieve_custom_event_notification_ftp_id(req, node);
	xadd_etag(req, TAG_FTPNotificationList);

	return STATUS_OK;
}

int update_custom_event_notification_ftp(request *req, node_t *node)
{
	int n;
	
	for(n = 0; n < get_event_count(); n++)
		update_custom_event_notification_ftp_id(req, node);
	
	return STATUS_OK;
}

int create_custom_event_notification_ftp(request *req, node_t *node)
{
	return STATUS_OK;
}

int delete_custom_event_notification_ftp(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_custom_event_notification_ftp_id(request *req, node_t *node)
{
	const char *id = "1";
	const char *addressingFormatType = "ipaddress";	
	const char *ipAddress = "172.24.190.119";
	const char *portNo = "80";
	const char *userName = "1234";
	const char *password = "123";
	const char *uploadPath = "D:/temp";
	
	xadd_stag_attr(req, TAG_FTPNotification, RootAttrStr);
	xadd_elem(req, TAG_id, id);
	xadd_elem(req, TAG_addressingFormatType, addressingFormatType);
	xadd_elem(req, TAG_ipAddress, ipAddress);
    xadd_elem(req, TAG_portNo, portNo);
	xadd_elem(req, TAG_userName, userName);
	xadd_elem(req, TAG_password, password);
	xadd_elem(req, TAG_uploadPath, uploadPath);
	xadd_etag(req, TAG_FTPNotification);

	return STATUS_OK;
}

int update_custom_event_notification_ftp_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	FTPNotification_psia FTPNotification;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_FTPNotification) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(FTPNotification.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_addressingFormatType) == 0) {
			strcpy(FTPNotification.addressingFormatType, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_ipAddress) == 0) {
			strcpy(FTPNotification.ipAddress, tag->value);
			required |= xml_required_field(3);
			ControlSystemData(SFIELD_SET_FTP_SERVER_IP, (void *)FTPNotification.ipAddress, strlen(FTPNotification.ipAddress));
		}
		else if(strcmp(tag->name, TAG_portNo) == 0) {
			strcpy(FTPNotification.portNo, tag->value);
			required |= xml_required_field(4);
			ControlSystemData(SFIELD_SET_FTP_PORT, (void *)&FTPNotification.portNo, sizeof(FTPNotification.portNo));
		}
		else if(strcmp(tag->name, TAG_userName) == 0) {
			strcpy(FTPNotification.userName, tag->value);
			required |= xml_required_field(5);
			ControlSystemData(SFIELD_SET_FTP_USERNAME, (void *)FTPNotification.userName, strlen(FTPNotification.userName));
		}
		else if(strcmp(tag->name, TAG_password) == 0) {
			strcpy(FTPNotification.password, tag->value);
			required |= xml_required_field(6);
			ControlSystemData(SFIELD_SET_FTP_PASSWORD, (void *)FTPNotification.password, strlen(FTPNotification.password));
		}
		else if(strcmp(tag->name, TAG_uploadPath) == 0) {
			strcpy(FTPNotification.uploadPath, tag->value);
			required |= xml_required_field(7);
			ControlSystemData(SFIELD_SET_FTP_FOLDNAME, (void *)FTPNotification.uploadPath, strlen(FTPNotification.uploadPath));
		}
		/* Protocol */
		tag = tag->next;
	}
	
	/* check all the required fileds were present */
	if(required != xml_required_mask(7))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int delete_custom_event_notification_ftp_id(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_custom_event_notification_httpHost(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_HttpHostNotificationList, RootAttrStr);
	for(n = 0; n < get_event_count(); n++)
		retrieve_custom_event_notification_httpHost_id(req, node);
	xadd_etag(req, TAG_HttpHostNotificationList);

	return STATUS_OK;
}

int update_custom_event_notification_httpHost(request *req, node_t *node)
{
	int n;
	
	for(n = 0; n < get_event_count(); n++)
		update_custom_event_notification_httpHost_id(req, node);

	return STATUS_OK;
}

int create_custom_event_notification_httpHost(request *req, node_t *node)
{
	return STATUS_OK;
}

int delete_custom_event_notification_httpHost(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_custom_event_notification_httpHost_id(request *req, node_t *node)
{
	const char *id = "1";
	const char *url = "URL";	
	const char *protocolType = "http";
	const char *portNo = "80";	
	
	xadd_stag_attr(req, TAG_HttpHostNotification, RootAttrStr);
	xadd_elem(req, TAG_id, id);
	xadd_elem(req, TAG_url, url);
	xadd_elem(req, TAG_protocolType, protocolType);
    xadd_elem(req, TAG_portNo, portNo);	
	xadd_etag(req, TAG_HttpHostNotification);

	return STATUS_OK;
}

int update_custom_event_notification_httpHost_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	HttpHostNotification_psia HttpHostNotification;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_HttpHostNotification) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(HttpHostNotification.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_url) == 0) {
			strcpy(HttpHostNotification.url, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_protocolType) == 0) {
			strcpy(HttpHostNotification.protocolType, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_portNo) == 0) {
			strcpy(HttpHostNotification.portNo, tag->value);
			required |= xml_required_field(4);
			ControlSystemData(SFIELD_GET_HTTPPORT, (void *)&HttpHostNotification.portNo, sizeof(HttpHostNotification.portNo));
		}		
		/* Protocol */
		tag = tag->next;
	}
	
	/* check all the required fileds were present */
	if(required != xml_required_mask(4))
		return STATUS_INVALID_XML_CONTENT;


	return STATUS_OK;
}

int delete_custom_event_notification_httpHost_id(request *req, node_t *node)
{
	return STATUS_OK;
}

/*******************************************************************/
/*                            Custom/Analytics                     */
/*******************************************************************/
int get_region_count(void)
{
	return 1;
}

int retrieve_custom_analytics_motiondetection(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_MotionDetectionList, RootAttrStr);
	for(n = 0; n < get_channel_count(); n++)
		retrieve_custom_analytics_motiondetection_id(req, node);
	xadd_etag(req, TAG_MotionDetectionList);

	return STATUS_OK;
}

int retrieve_custom_analytics_motiondetection_id(request *req, node_t *node)
{
	const char *id = "1";
	const char *enabled = "TRUE";	
	const char *samplingInterval = "1000";
	const char *startTriggerTime = "80";
	const char *endTriggerTime = "1";
	const char *directionSensitivity = "left-right";	
	const char *regionType = "grid";	
	const char *rowGranularity = "200";
	const char *columnGranularity = "200";
	
	xadd_stag_attr(req, TAG_MotionDetection, RootAttrStr);
	xadd_elem(req, TAG_id, id);
	xadd_elem(req, TAG_enabled, enabled);
	xadd_elem(req, TAG_samplingInterval, samplingInterval);
    xadd_elem(req, TAG_startTriggerTime, startTriggerTime);	
	xadd_elem(req, TAG_endTriggerTime, endTriggerTime);
	xadd_elem(req, TAG_directionSensitivity, directionSensitivity);
	xadd_elem(req, TAG_regionType, regionType);
	xadd_stag(req, TAG_Grid);
	xadd_elem(req, TAG_rowGranularity, rowGranularity);
	xadd_elem(req, TAG_columnGranularity, columnGranularity);
	xadd_etag(req, TAG_Grid);
	/* MotionDetectionRegionList */
	retrieve_custom_analytics_motiondetection_id_regions(req,node);
	xadd_etag(req, TAG_MotionDetection);

	return STATUS_OK;
}

int update_custom_analytics_motiondetection_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	MotionDetection_psia MotionDetection;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_MotionDetection) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(MotionDetection.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(MotionDetection.enabled, tag->value);
			required |= xml_required_field(2);
			ControlSystemData(SFIELD_SET_MOTIONENABLE, (void *)MotionDetection.enabled, strlen(MotionDetection.enabled));
		}
		else if(strcmp(tag->name, TAG_samplingInterval) == 0) {
			strcpy(MotionDetection.samplingInterval, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_startTriggerTime) == 0) {
			strcpy(MotionDetection.startTriggerTime, tag->value);
			required |= xml_required_field(4);			
		}
		else if(strcmp(tag->name, TAG_endTriggerTime) == 0) {
			strcpy(MotionDetection.endTriggerTime, tag->value);
			required |= xml_required_field(5);			
		}	
		else if(strcmp(tag->name, TAG_directionSensitivity) == 0) {
			strcpy(MotionDetection.directionSensitivity, tag->value);
			required |= xml_required_field(6);			
		}	
		else if(strcmp(tag->name, TAG_regionType) == 0) {
			strcpy(MotionDetection.regionType, tag->value);
			required |= xml_required_field(7);			
		}	
		else if(strcmp(tag->name, TAG_rowGranularity) == 0) {
			strcpy(MotionDetection.rowGranularity, tag->value);
			required |= xml_required_field(8);			
		}	
		else if(strcmp(tag->name, TAG_columnGranularity) == 0) {
			strcpy(MotionDetection.columnGranularity, tag->value);
			required |= xml_required_field(9);			
		}
		else if(strcmp(tag->name, TAG_MotionDetectionRegionList) == 0) {
			update_custom_analytics_motiondetection_id_regions(req,node);
			required |= xml_required_field(10);			
		}
		/* Protocol */
		tag = tag->next;
	}
	
	/* check all the required fileds were present */
	if(required != xml_required_mask(10))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int retrieve_custom_analytics_motiondetection_id_regions(request *req, node_t *node)
{
	int n;

	xadd_stag_attr(req, TAG_MotionDetectionRegionList, RootAttrStr);
	for(n = 0; n < get_region_count(); n++)
		retrieve_custom_analytics_motiondetection_id_regions_id(req, node);
	xadd_etag(req, TAG_MotionDetectionRegionList);

	return STATUS_OK;
}

int update_custom_analytics_motiondetection_id_regions(request *req, node_t *node)
{
	int n;
	
	for(n = 0; n < get_region_count(); n++)
		update_custom_analytics_motiondetection_id_regions_id(req, node);
	
	return STATUS_OK;
}

int create_custom_analytics_motiondetection_id_regions(request *req, node_t *node)
{
	return STATUS_OK;
}

int delete_custom_analytics_motiondetection_id_regions(request *req, node_t *node)
{
	return STATUS_OK;
}

int retrieve_custom_analytics_motiondetection_id_regions_id(request *req, node_t *node)
{
	const char *id = "1";
	const char *enabled = "TRUE";	
	const char *maskEnabled = "TRUE";
	const char *sensitivityLevel = "80";
	const char *detectionThreshold = "80";
	const char *positionX = "0";	
	const char *positionY = "0";	
	
	xadd_stag_attr(req, TAG_MotionDetectionRegion, RootAttrStr);
	xadd_elem(req, TAG_id, id);
	xadd_elem(req, TAG_enabled, enabled);
	xadd_elem(req, TAG_maskEnabled, maskEnabled);
    xadd_elem(req, TAG_sensitivityLevel, sensitivityLevel);	
	xadd_elem(req, TAG_detectionThreshold, detectionThreshold);	
	xadd_stag(req, TAG_RegionCoordinatesList);
	xadd_stag(req, TAG_RegionCoordinates);
	xadd_elem(req, TAG_positionX, positionX);
	xadd_elem(req, TAG_positionY, positionY);
	xadd_etag(req, TAG_RegionCoordinates);	
	xadd_etag(req, TAG_RegionCoordinatesList);	
	xadd_etag(req, TAG_MotionDetectionRegion);

	return STATUS_OK;
}

int update_custom_analytics_motiondetection_id_regions_id(request *req, node_t *node)
{
	tag_t	*tag;
	unsigned long required;
	MotionDetectionRegion_psia MotionDetectionRegion;

	// returns bytes processed or -1
	if(xml_validate(req, req->inbuf, req->inlen) < 0)
		return STATUS_INVALID_XML_FORMAT;

	tag = req->root_tag;
	if(!((tag) && (strcmp(tag->name, TAG_MotionDetectionRegion) == 0)))
		return STATUS_INVALID_XML_CONTENT;

	tag = tag->first_child;
	required = 0;
	while(tag) {
		if(strcmp(tag->name, TAG_id) == 0) {
			strcpy(MotionDetectionRegion.id, tag->value);
			required |= xml_required_field(1);
		}
		else if(strcmp(tag->name, TAG_enabled) == 0) {
			strcpy(MotionDetectionRegion.enabled, tag->value);
			required |= xml_required_field(2);
		}
		else if(strcmp(tag->name, TAG_maskEnabled) == 0) {
			strcpy(MotionDetectionRegion.maskEnabled, tag->value);
			required |= xml_required_field(3);
		}
		else if(strcmp(tag->name, TAG_sensitivityLevel) == 0) {
			strcpy(MotionDetectionRegion.sensitivityLevel, tag->value);
			required |= xml_required_field(4);
			ControlSystemData(SFIELD_SET_MOTIONLEVEL, &MotionDetectionRegion.sensitivityLevel, sizeof(MotionDetectionRegion.sensitivityLevel));
		}
		else if(strcmp(tag->name, TAG_detectionThreshold) == 0) {
			strcpy(MotionDetectionRegion.detectionThreshold, tag->value);
			required |= xml_required_field(5);	
			ControlSystemData(SFIELD_SET_MOTIONCVALUE, &MotionDetectionRegion.detectionThreshold, sizeof(MotionDetectionRegion.detectionThreshold));
		}	
		else if(strcmp(tag->name, TAG_positionX) == 0) {
			strcpy(MotionDetectionRegion.positionX, tag->value);
			required |= xml_required_field(6);			
		}	
		else if(strcmp(tag->name, TAG_positionY) == 0) {
			strcpy(MotionDetectionRegion.positionY, tag->value);
			required |= xml_required_field(7);			
		}			
		/* Protocol */
		tag = tag->next;
	}
	
	/* check all the required fileds were present */
	if(required != xml_required_mask(7))
		return STATUS_INVALID_XML_CONTENT;

	return STATUS_OK;
}

int delete_custom_analytics_motiondetection_id_regions_id(request *req, node_t *node)
{
	return STATUS_OK;
}



/* called just before the request in going to to be processed */
int iface_init(request *req)
{
	return R_REQUEST_OK;
}

/* called after the request has completed */
void iface_cleanup(request *req)
{

}


