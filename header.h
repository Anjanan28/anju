#include<unistd.h>
#include<stdio.h>
#include<stddef.h>
#include<stdint.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<getopt.h>
struct method{

	char Messagetype[255];
};
struct Liid_s{

	uint32_t liid;
};
struct DFID{
	uint32_t df_id;
};
struct Df{
	struct DFID dfid_o;
	char df_name[255];
	char df_ip[255];
	uint32_t df_port;
};

struct x1_targets_t {
	struct method Msg_type_o;
	struct Liid_s liid_o;
	char target_type[255];
	uint64_t imsi;
	struct Df df2;
	struct Df df3;
	/*union target_id_t {
	  uint64_t imsi;
	  uint64_t mei;
	  uint8_t msisdn[16];
	  }target_id;*/
}x1_targets[100];


struct Modify_interception {
	struct method Msg_type_o;
	struct Liid_s liid_o;
	struct Df df2;
	struct Df df3;
}modify_o;

struct Add_DF
{
	struct method Msg_type_o;
	struct Df df2;
	struct Df df3;
}addDf_o[100];

struct ReqDF
{
	struct method Msg_type_o;
	struct DFID df2id;
	struct DFID df3id;
}reqdf;

struct ReqInterception
{
	struct method Msg_type_o;
	struct Liid_s liid_o;
}reqinter;

struct response
{
	uint32_t response_code;
}response_o;
