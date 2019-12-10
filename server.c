#include "header.h"
int count=0,count1=0;
int createSocket(int port)
{
	int sock, err;
	struct sockaddr_in server;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Socket creation failed\n");
		exit(1);
	}
	printf("Socket created\n");

	bzero((char *) &server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
	if (bind(sock, (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		printf("Bind failed\n");
		exit(1);
	}
	listen(sock , 10);
	return sock;
}

void closeSocket(int sock)
{
	close(sock);
	return;
}

void sendMsg(int sock, void* msg, uint32_t msgsize)
{
	if (write(sock, msg, msgsize) < 0)
	{
		printf("Can't send message.\n");
		closeSocket(sock);
		exit(1);
	}
	return;
}

int main()
{
	int PORT = 3000;
	int BUFFSIZE = 2000;
	char buff[BUFFSIZE],buff1[]="Received",str[BUFFSIZE];
	int ssock, csock;
	int nread;
	int i;
	struct sockaddr_in client;
	int clilen = sizeof(client);

	ssock = createSocket(PORT);

	struct in_addr DFIP;

	while (1)
	{
		csock = accept(ssock, (struct sockaddr *)&client, &clilen);
		if (csock < 0)
		{
			printf("Error: accept() failed\n");
			continue;
		}
		bzero(buff, BUFFSIZE);
		while ((nread=read(csock, buff, BUFFSIZE)) > 0)
		{
			struct  x1_targets_t *p = (struct  x1_targets_t*) buff;
			printf("\nMessage = %s\n",p->Msg_type_o.Messagetype);
			if(strcmp(p->Msg_type_o.Messagetype,"AddInterception")==0)
			{

				strcpy(x1_targets[count].Msg_type_o.Messagetype,p->Msg_type_o.Messagetype);
				printf("\nMessage = %s\n",x1_targets[count].Msg_type_o.Messagetype);
				x1_targets[count].liid_o.liid=p->liid_o.liid;
				printf("\nLIID = %d\n",x1_targets[count].liid_o.liid);
				strcpy(x1_targets[count].target_type,p->target_type);
				printf("\nTarget type = %s\n",p->target_type);
				x1_targets[count].imsi=p->imsi;
				printf("\nIMSI = %ld\n",p->imsi);
				strcpy(x1_targets[count].df2.df_name,p->df2.df_name);
				x1_targets[count].df2.dfid_o.df_id=p->df2.dfid_o.df_id;
				printf("\nDF ID = %d\n",p->df2.dfid_o.df_id);
				printf("\nDF name = %s\n",p->df2.df_name);
				strcpy(x1_targets[count].df2.df_ip,p->df2.df_ip);
				inet_pton(AF_INET, x1_targets[count].df2.df_ip, &(DFIP.s_addr));
				printf("\nDF IP = %s\n",p->df2.df_ip);
				printf("\nDFI = %d\n",DFIP.s_addr);
				inet_ntop(AF_INET, &(DFIP.s_addr), str, BUFFSIZE);
				printf("\nDF IP = %s\n",str);
				x1_targets[count].df2.df_port=p->df2.df_port;
				printf("\nDF port = %d\n",p->df2.df_port);
				x1_targets[count].df3.dfid_o.df_id=p->df3.dfid_o.df_id;
				printf("\nDF ID = %d\n",p->df3.dfid_o.df_id);
				strcpy(x1_targets[count].df3.df_name,p->df3.df_name);
				printf("\nDF name = %s\n",p->df3.df_name);
				strcpy(x1_targets[count].df3.df_ip,p->df3.df_ip);
				inet_pton(AF_INET, x1_targets[count].df3.df_ip, &(DFIP.s_addr));
				printf("\nDF IP = %s\n",p->df3.df_ip);
				printf("\nDFI = %d\n",DFIP.s_addr);
				inet_ntop(AF_INET, &(DFIP.s_addr), str, BUFFSIZE);
				printf("\nDF IP = %s\n",str);
				x1_targets[count].df3.df_port=p->df3.df_port;
				printf("\nDF port = %d\n",p->df3.df_port);
				count++;
				struct response *r;
				response_o.response_code=500;
				r=&response_o;
				sendMsg(csock, r, sizeof(struct response));
			}
			else if(strcmp(p->Msg_type_o.Messagetype,"ModifyInterception")==0)
			{
				struct Modify_interception *m = (struct  Modify_interception*) buff;
				for(i=0;i<=count;i++)
				{
				if(m->liid_o.liid==x1_targets[i].liid_o.liid)
				{
				x1_targets[i].liid_o.liid=m->liid_o.liid;
				printf("\nLIID = %d\n",x1_targets[i].liid_o.liid);
				x1_targets[i].df2.dfid_o.df_id=m->df2.dfid_o.df_id;
				printf("\nDF ID = %d\n",x1_targets[i].df2.dfid_o.df_id);
				strcpy(x1_targets[i].df2.df_name,m->df2.df_name);
				strcpy(x1_targets[i].df2.df_ip,m->df2.df_ip);
				x1_targets[i].df2.df_port=m->df2.df_port;
				x1_targets[i].df3.dfid_o.df_id=m->df3.dfid_o.df_id;
				printf("\nDF ID = %d\n",x1_targets[i].df3.dfid_o.df_id);
				strcpy(x1_targets[i].df3.df_name,m->df3.df_name);
				strcpy(x1_targets[i].df3.df_ip,m->df3.df_ip);
				x1_targets[i].df3.df_port=m->df3.df_port;
				struct response *r;
				response_o.response_code=500;
				r=&response_o;
				sendMsg(csock, r, sizeof(struct response));
				}
				}
			}
			else if(strcmp(p->Msg_type_o.Messagetype,"DeleteInterception")==0)
			{
				struct ReqInterception *m = (struct ReqInterception*) buff;
				for(i=0;i<=count;i++)
				{
				if(m->liid_o.liid==x1_targets[i].liid_o.liid)
				{
				x1_targets[i].liid_o.liid=0;
				memset(x1_targets[i].target_type,0,strlen(x1_targets[i].target_type));
				printf("\nTarget type = %s\n",p->target_type);
				x1_targets[i].imsi=0;
				x1_targets[i].df2.dfid_o.df_id=0;
				x1_targets[i].df3.dfid_o.df_id=0;
				memset(x1_targets[i].df2.df_name,0,strlen(x1_targets[i].df2.df_name));
				memset(x1_targets[i].df3.df_name,0,strlen(x1_targets[i].df3.df_name));
				memset(x1_targets[i].df2.df_ip,0,strlen(x1_targets[i].df2.df_ip));
				memset(x1_targets[i].df3.df_ip,0,strlen(x1_targets[i].df3.df_ip));
				x1_targets[i].df2.df_port=0;
				x1_targets[i].df3.df_port=0;
				printf("\nLIID = %d\n",x1_targets[i].liid_o.liid);
				struct response *r;
				response_o.response_code=500;
				r=&response_o;
				sendMsg(csock, r, sizeof(struct response));
				}
				}
			}

			else if(strcmp(p->Msg_type_o.Messagetype,"ListOneInterception")==0)
			{
				struct ReqInterception *m;
				printf("\nLIID = %d\n",p->liid_o.liid);
				strcpy(reqinter.Msg_type_o.Messagetype,x1_targets[count].Msg_type_o.Messagetype);
				reqinter.liid_o.liid=x1_targets[count].liid_o.liid;
				m=&reqinter;
				sendMsg(csock, m, sizeof(struct ReqInterception));
			}
			else if(strcmp(p->Msg_type_o.Messagetype,"ListAllInterception")==0)
			{
				for(i=0;i<count;i++)
				{
					struct ReqInterception *m;
					strcpy(reqinter.Msg_type_o.Messagetype,x1_targets[i].Msg_type_o.Messagetype);
					reqinter.liid_o.liid=x1_targets[i].liid_o.liid;
					m=&reqinter;
					sendMsg(csock, m, sizeof(struct ReqInterception));
				}
			}
			else if(strcmp(p->Msg_type_o.Messagetype,"AddDF")==0)
			{
				struct Add_DF *m = (struct Add_DF*) buff;
				addDf_o[count1].df2.dfid_o.df_id=m->df2.dfid_o.df_id;
				strcpy(addDf_o[count1].df2.df_name,m->df2.df_name);
				strcpy(addDf_o[count1].df2.df_ip,m->df2.df_ip);
				addDf_o[count1].df2.df_port=m->df2.df_port;
				printf("\nDF ID = %d\n",addDf_o[count1].df2.dfid_o.df_id);
				printf("\nDF name = %s\n",addDf_o[count1].df2.df_name);
				addDf_o[count1].df3.dfid_o.df_id=m->df3.dfid_o.df_id;
				strcpy(addDf_o[count1].df3.df_name,m->df3.df_name);
				strcpy(addDf_o[count1].df3.df_ip,m->df3.df_ip);
				addDf_o[count1].df3.df_port=m->df3.df_port;
				printf("\nDF ID = %d\n",addDf_o[count1].df3.dfid_o.df_id);
				printf("\nDF name = %s\n",addDf_o[count1].df3.df_name);
				struct response *r;
				response_o.response_code=500;
				r=&response_o;
				count1++;
				sendMsg(csock, r, sizeof(struct response));
			}
			else if(strcmp(p->Msg_type_o.Messagetype,"ModifyDF")==0)
			{
				struct Add_DF *m = (struct Add_DF*) buff;
				for(i=0;i<=count1;i++)
				{
				if((m->df2.dfid_o.df_id==addDf_o[i].df2.dfid_o.df_id)&&(m->df3.dfid_o.df_id==addDf_o[i].df3.dfid_o.df_id))
				{
				addDf_o[i].df2.dfid_o.df_id=m->df2.dfid_o.df_id;
				strcpy(addDf_o[i].df2.df_name,m->df2.df_name);
				strcpy(addDf_o[i].df2.df_ip,m->df2.df_ip);
				addDf_o[i].df2.df_port=m->df2.df_port;
				printf("\nDF ID = %d\n",addDf_o[i].df2.dfid_o.df_id);
				printf("\nDF name = %s\n",addDf_o[i].df2.df_name);
				addDf_o[i].df3.dfid_o.df_id=m->df3.dfid_o.df_id;
				strcpy(addDf_o[i].df3.df_name,m->df3.df_name);
				strcpy(addDf_o[i].df3.df_ip,m->df3.df_ip);
				addDf_o[i].df3.df_port=m->df3.df_port;
				printf("\nDF ID = %d\n",addDf_o[i].df3.dfid_o.df_id);
				printf("\nDF name = %s\n",addDf_o[i].df3.df_name);
				struct response *r;
				response_o.response_code=500;
				r=&response_o;
				sendMsg(csock, r, sizeof(struct response));
				}
				}
			}
			else if(strcmp(p->Msg_type_o.Messagetype,"DeleteDF")==0)
			{
				struct ReqDF *m = (struct ReqDF*) buff;
				for(i=0;i<=count1;i++)
				{
				if((m->df2id.df_id==addDf_o[i].df2.dfid_o.df_id)&&(m->df3id.df_id==addDf_o[i].df3.dfid_o.df_id))
				{
				addDf_o[i].df2.dfid_o.df_id=0;
				addDf_o[i].df3.dfid_o.df_id=0;
				memset(addDf_o[i].df2.df_name,0,strlen(addDf_o[i].df2.df_name));
				memset(addDf_o[i].df3.df_name,0,strlen(addDf_o[i].df3.df_name));
				memset(addDf_o[i].df2.df_ip,0,strlen(addDf_o[i].df2.df_ip));
				memset(addDf_o[i].df3.df_ip,0,strlen(addDf_o[i].df3.df_ip));
				addDf_o[i].df2.df_port=0;
				addDf_o[i].df3.df_port=0;
				printf("\nDF ID = %d\n",addDf_o[i].df2.dfid_o.df_id);
				printf("\nDF ID = %d\n",addDf_o[i].df3.dfid_o.df_id);
				printf("\nDF name = %s\n",addDf_o[i].df2.df_name);
				printf("\nDF name = %s\n",addDf_o[i].df3.df_name);
				struct response *r;
				response_o.response_code=500;
				r=&response_o;
				sendMsg(csock, r, sizeof(struct response));
				}
				}
			}
			else if(strcmp(p->Msg_type_o.Messagetype,"ListOneDF")==0)
			{
				struct ReqDF *m;
				strcpy(reqdf.Msg_type_o.Messagetype,x1_targets[count].Msg_type_o.Messagetype);
				reqdf.df2id.df_id=x1_targets[count].df2.dfid_o.df_id;
				reqdf.df3id.df_id=x1_targets[count].df3.dfid_o.df_id;
				m=&reqdf;
				sendMsg(csock,m , sizeof(struct ReqDF));
			}
			else if(strcmp(p->Msg_type_o.Messagetype,"ListAllDf")==0)
			{
				for(i=0;i<count;i++)
				{
					struct ReqDF *m;
					strcpy(reqdf.Msg_type_o.Messagetype,x1_targets[i].Msg_type_o.Messagetype);
					reqdf.df2id.df_id=x1_targets[count].df2.dfid_o.df_id;
					reqdf.df3id.df_id=x1_targets[count].df3.dfid_o.df_id;
					m=&reqdf;
					sendMsg(csock,m, sizeof(struct ReqDF));
				}
			}
			else
				printf("\nInvalid\n");
		}
		closeSocket(csock);
		bzero(buff, BUFFSIZE);
	}

	closeSocket(ssock);
	return 0;
}
