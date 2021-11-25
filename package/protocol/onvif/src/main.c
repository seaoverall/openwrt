/*
 * =====================================================================================
 *
 *    Filename:  main.c
 *    Description:  简单例程测试:客户端通过ONVIF协议搜索前端设备
 *    Compiler:  gcc
 *    Author:  miibotree
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>


#include "wsdd.h"
#include "wsseapi.h"
#include "wsaapi.h"

#define ONVIF_USER "admin"
#define ONVIF_PASSWORD "123456"

static struct soap* ONVIF_Initsoap(struct SOAP_ENV__Header *header, const char *was_To, const char *was_Action, int timeout);
int ONVIF_ClientDiscovery( );
int ONVIF_Capabilities(struct __wsdd__ProbeMatches *resp);
void UserGetProfiles(struct soap *soap, struct _tds__GetCapabilitiesResponse *capa_resp);
void UserGetUri(struct soap *soap, struct _trt__GetProfilesResponse *trt__GetProfilesResponse,struct _tds__GetCapabilitiesResponse *capa_resp);


extern void FFmpeg_Open_Uri(char *uri);



int HasDev = 0;//the number of devices

/*********************************************************************
get local ip-addr & mac-addr
**********************************************************************/
static char _IPv4Address[64] = {0};
static char LOCAL_MAC_ADDR[64] = {0};
#define ETH_NAME      "ens33"

void get_local_address(char *ip, int size)
{
    if (ip && (size > 0) && (size <= 64)) {
        memcpy(ip, _IPv4Address, size);
        printf("get local ipaddr = %s \r\n", ip);
    }
}

void get_mac_address(char *mac, int size)
{
    if (mac && size > 0 && size <= 64) {
        memcpy(mac, LOCAL_MAC_ADDR, size);
        printf("get mac ipaddr = %s \r\n", mac);
    }
}

char* GetLocalAddress(void)
{
    int sock;
    char *ip = NULL;
    struct ifreq ifr;
    struct sockaddr_in sin;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        return NULL;
    }

    strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ - 1] = 0;

    if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
        perror("ioctl");
        return NULL;
    }

    memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
    ip = inet_ntoa(sin.sin_addr);
    memcpy(_IPv4Address, ip , sizeof(_IPv4Address));
    snprintf(LOCAL_MAC_ADDR ,64 ,"%02X:%02X:%02X:%02X:%02X:%02X",(unsigned char)ifr.ifr_hwaddr.sa_data[0],
        (unsigned char)ifr.ifr_hwaddr.sa_data[1],
        (unsigned char)ifr.ifr_hwaddr.sa_data[2],
        (unsigned char)ifr.ifr_hwaddr.sa_data[3],
        (unsigned char)ifr.ifr_hwaddr.sa_data[4],
        (unsigned char)ifr.ifr_hwaddr.sa_data[5]);

    return ip;
}

int main(void )
{
	if (ONVIF_ClientDiscovery() != 0 )
	{
		printf("discovery failed!\n");
		return -1;
	}
	return 0;
}

static struct soap* ONVIF_Initsoap(struct SOAP_ENV__Header *header, const char *was_To, const char *was_Action, 
		int timeout)
{
	struct soap *soap = NULL; 
	unsigned char macaddr[6];
	char _HwId[1024];
	unsigned int Flagrand;
	soap = soap_new();
	if(soap == NULL)
	{
		printf("[%d]soap = NULL\n", __LINE__);
		return NULL;
	}
	 soap_set_namespaces( soap, namespaces);
	if (timeout > 0)
	{
		soap->recv_timeout = timeout;
		soap->send_timeout = timeout;
		soap->connect_timeout = timeout;
	}
	else
	{
		soap->recv_timeout    = 10;
		soap->send_timeout    = 10;
		soap->connect_timeout = 10;
	}
    soap->socket_flags = MSG_NOSIGNAL;
    soap_set_mode(soap, SOAP_C_UTFSTRING);
	soap_default_SOAP_ENV__Header(soap, header);

	srand((int)time(0));
	Flagrand = rand()%9000 + 1000;

    GetLocalAddress();
	sprintf(_HwId,"urn:uuid:%ud68a-1dd2-11b2-a105-%s", Flagrand, LOCAL_MAC_ADDR);
	header->wsa__MessageID =(char *)malloc( 100);
	memset(header->wsa__MessageID, 0, 100);
	strncpy(header->wsa__MessageID, _HwId, strlen(_HwId));

	if (was_Action != NULL) {
		header->wsa__Action =(char *)malloc(1024);
		memset(header->wsa__Action, '\0', 1024);
		strncpy(header->wsa__Action, was_Action, 1024);//"http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";
	}

	if (was_To != NULL)	{
		header->wsa__To =(char *)malloc(1024);
		memset(header->wsa__To, '\0', 1024);
		strncpy(header->wsa__To,  was_To, 1024);//"urn:schemas-xmlsoap-org:ws:2005:04:discovery";	
	}

	soap->header = header;
	return soap;
}

int ONVIF_ClientDiscovery( )
{
	int retval = SOAP_OK;
	wsdd__ProbeType req;       
	struct __wsdd__ProbeMatches resp;
	wsdd__ScopesType sScope;
	struct soap *soap = NULL; 
	struct SOAP_ENV__Header header;	

	const char *was_To = "urn:schemas-xmlsoap-org:ws:2005:04:discovery";
	const char *was_Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";
	const char *soap_endpoint = "soap.udp://239.255.255.250:3702/";


    while (1) {
    	soap = ONVIF_Initsoap(&header, was_To, was_Action, 5);
    	
    	soap_default_wsdd__ScopesType(soap, &sScope);
    	sScope.__item = NULL;
    	soap_default_wsdd__ProbeType(soap, &req);
    	req.Scopes = &sScope;
    	req.Types = "tdn:NetworkVideoTransmitter";
        soap_register_plugin(soap, soap_wsa);

        printf("plugins_id = %s \r\n", soap->plugins->id);
	
        retval = soap_send___wsdd__Probe(soap, soap_endpoint, NULL, &req);
		retval = soap_recv___wsdd__ProbeMatches(soap, &resp);
        if (retval == SOAP_OK) {
            if (soap->error) {
                printf("[%d]: recv soap error :%d, %s, %s\n", __LINE__, soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
			    retval = soap->error;
            } else {
				HasDev ++;
				if (resp.wsdd__ProbeMatches->ProbeMatch != NULL && resp.wsdd__ProbeMatches->ProbeMatch->XAddrs != NULL)
				{
					printf(" ################  recv  %d devices info #### \n", HasDev );
					
					printf("Target Service Address  : %s\n", resp.wsdd__ProbeMatches->ProbeMatch->XAddrs);	
					printf("Target EP Address       : %s\n", resp.wsdd__ProbeMatches->ProbeMatch->wsa__EndpointReference.Address);  
					printf("Target Type             : %s\n", resp.wsdd__ProbeMatches->ProbeMatch->Types);  
					printf("Target Metadata Version : %d\n", resp.wsdd__ProbeMatches->ProbeMatch->MetadataVersion); 
					ONVIF_Capabilities(&resp);
					sleep(1);
				}
			}
		} else if (soap->error) {  
			if (HasDev == 0) {
				printf("[%s][%s][Line:%d] Thers Device discovery or soap error: %d, %s, %s \n",__FILE__, __func__, __LINE__, soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
				retval = soap->error;  
			} else {
				printf(" [%s]-[%d] Search end! It has Searched %d devices! soap->error = %d \r\n", __func__, __LINE__, HasDev, soap->error);
				retval = 0;
			}
            soap_destroy(soap); 
            soap_end(soap); 
            soap_free(soap);
		}
    }
	
	return retval;
}

int ONVIF_Capabilities(struct __wsdd__ProbeMatches *resp)
{    
    int retval = 0;
    struct soap *soap = NULL; 
    struct SOAP_ENV__Header header = {0};
    struct _tds__GetCapabilities capa_req = {0};
    struct _tds__GetCapabilitiesResponse capa_resp = {0};

    soap = ONVIF_Initsoap(&header, NULL, NULL, 5);
    char *soap_endpoint = (char *)malloc(256);
    if (!soap_endpoint) {
        printf("soap_endpoint malloc failed \r\n");
        return -1;
    }

    memset(soap_endpoint, '\0', 256);
    sprintf(soap_endpoint, "%s", resp->wsdd__ProbeMatches->ProbeMatch->XAddrs);
    capa_req.Category = (enum tt__CapabilityCategory *)soap_malloc(soap, sizeof(int));

    capa_req.__sizeCategory = 1;
    *(capa_req.Category) = (enum tt__CapabilityCategory)0;  
    const char *soap_action = "http://www.onvif.org/ver10/device/wsdl/GetCapabilities";  
    capa_resp.Capabilities = (struct tt__Capabilities*)soap_malloc(soap,sizeof(struct tt__Capabilities)) ;

    //soap_wsse_add_UsernameTokenDigest(soap, "user", ONVIF_USER, ONVIF_PASSWORD);
    printf("soap_endpoint = %s \r\n", soap_endpoint);
    do  
    {
        int result = soap_call___tds__GetCapabilities(soap, soap_endpoint, soap_action, &capa_req, &capa_resp);  
        if (soap->error) {  
            printf("[%s][%d]--->>> soap error: %d, %s, %s\n", __func__, __LINE__, soap->error, *soap_faultcode(soap), *soap_faultstring(soap));  
            retval = soap->error;  
            break;
        } else {
            printf("[%s][%d] Get capabilities success !\n", __func__, __LINE__);  
            if(capa_resp.Capabilities == NULL) {
                printf("GetCapabilities failed! result = %d\n", result);
            } else {
                printf(" Media->XAddr=%s \n", capa_resp.Capabilities->Media->XAddr);
                UserGetProfiles(soap, &capa_resp);//
            }
        }
    } while(0);
  
    free(soap_endpoint);  
    soap_endpoint = NULL;  
    soap_destroy(soap);  
    return retval;  
}  

void UserGetProfiles(struct soap *soap, struct _tds__GetCapabilitiesResponse *capa_resp)  
{  
    struct _trt__GetProfiles trt__GetProfiles;
    struct _trt__GetProfilesResponse trt__GetProfilesResponse;
    int result= SOAP_OK ;  
    printf("\n-------------------Getting Onvif Devices Profiles--------------\n\n");  
    //soap_wsse_add_UsernameTokenDigest(soap,"user", ONVIF_USER, ONVIF_PASSWORD);  

    result = soap_call___trt__GetProfiles(soap, capa_resp->Capabilities->Media->XAddr, NULL, &trt__GetProfiles, &trt__GetProfilesResponse);  
    if (result==-1) {
         //NOTE: it may be regular if result isn't SOAP_OK.Because some attributes aren't supported by server.  
      	 printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap));  
       	 result = soap->error;
       	 return;
    } else {  
   		 printf("\n-------------------Profiles Get OK--------------\n\n");  
     	 if(trt__GetProfilesResponse.Profiles!=NULL)  
     	 {  
            int profile_cnt = trt__GetProfilesResponse.__sizeProfiles;
            printf("the number of profiles are: %d\n", profile_cnt);
       	    if(trt__GetProfilesResponse.Profiles->Name!=NULL) {
       	        printf("Profiles Name:%s  \n",trt__GetProfilesResponse.Profiles->Name);
            }
            
            if(trt__GetProfilesResponse.Profiles->token!=NULL) {
                printf("Profiles Taken:%s\n",trt__GetProfilesResponse.Profiles->token);
            }
	        UserGetUri(soap, &trt__GetProfilesResponse, capa_resp);
       	 } else {
    	    printf("Profiles Get inner Error\n");
         }
    } 
    printf("Profiles Get Procedure over\n");
}  

void UserGetUri(struct soap *soap, struct _trt__GetProfilesResponse *trt__GetProfilesResponse,struct _tds__GetCapabilitiesResponse *capa_resp)  
{  
    struct _trt__GetStreamUri *trt__GetStreamUri = (struct _trt__GetStreamUri *)malloc(sizeof(struct _trt__GetStreamUri));
    struct _trt__GetStreamUriResponse *trt__GetStreamUriResponse = (struct _trt__GetStreamUriResponse *)malloc(sizeof(struct _trt__GetStreamUriResponse));
    int result=0 ;  
    trt__GetStreamUri->StreamSetup = (struct tt__StreamSetup*)soap_malloc(soap,sizeof(struct tt__StreamSetup));//初始化，分配空间  
    trt__GetStreamUri->StreamSetup->Stream = 0;//stream type  
    trt__GetStreamUri->StreamSetup->Transport = (struct tt__Transport *)soap_malloc(soap, sizeof(struct tt__Transport));//初始化，分配空间  
    trt__GetStreamUri->StreamSetup->Transport->Protocol = 0;  
    trt__GetStreamUri->StreamSetup->Transport->Tunnel = 0;  
    trt__GetStreamUri->StreamSetup->__size = 1;  
    trt__GetStreamUri->StreamSetup->__any = NULL;  
    trt__GetStreamUri->StreamSetup->__anyAttribute =NULL;  
  
    trt__GetStreamUri->ProfileToken = trt__GetProfilesResponse->Profiles->token ;  

    printf("\n\n---------------Getting Uri----------------\n\n");  
    //soap_wsse_add_UsernameTokenDigest(soap,"user", ONVIF_USER, ONVIF_PASSWORD);  
    soap_call___trt__GetStreamUri(soap, capa_resp->Capabilities->Media->XAddr, NULL, trt__GetStreamUri, trt__GetStreamUriResponse);  

    if (soap->error) {  
        printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap));  
        result = soap->error;  
    } else {
        printf("!!!!NOTE: RTSP Addr Get Done is :%s \n", trt__GetStreamUriResponse->MediaUri->Uri);
        //FFmpeg_Open_Uri(trt__GetStreamUriResponse->MediaUri->Uri);
        pthread_t rtsp_id = -1;
        static char uri_buff[256] = {0};
        memcpy(uri_buff, trt__GetStreamUriResponse->MediaUri->Uri, 256);
        extern void rtsp_client(void* arg);
        pthread_create(&rtsp_id, NULL, rtsp_client, uri_buff);
    }
}
