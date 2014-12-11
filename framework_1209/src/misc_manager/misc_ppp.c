#include "misc_private.h"

succeed_type readpppfile(char *strname,char *strpasswd)
{
	char rdbuf[500];
	char wrbufhead[250];
	char wrbuftail[250];
	char a_char_name[30];
	char a_char_passwd[30];
	char *ptname,*ptpasswd,*ptlcp;
	memset(rdbuf,0,sizeof(rdbuf));
	memset(wrbufhead,0,sizeof(wrbufhead));	
	memset(wrbuftail,0,sizeof(wrbuftail));
	memset(a_char_name,0,sizeof(a_char_name));
	memset(a_char_passwd,0,sizeof(a_char_passwd));
	ptname=ptpasswd=ptlcp=NULL;
	
	int fd = open("/etc/ppp/peers/ppp",O_RDONLY);
    if(fd==-1)
    {
   		 SYS_ERROR("PPP Open Failed.\r\n");
    	return succeed_type_failed;
	}
    else
    {
         lseek(fd,0,SEEK_SET);
     	 int length = read(fd,rdbuf,400);
         ptname = strstr(rdbuf,"user");
         ptpasswd = strstr(rdbuf,"password");
         ptlcp = strstr(rdbuf,"lcp-echo-interval");
         memcpy(wrbufhead,rdbuf,ptname-rdbuf);
         memcpy(wrbuftail,rdbuf+(ptlcp-rdbuf),length-(ptlcp-rdbuf));
         close(fd);
     }

	 fd = open("/etc/ppp/peers/ppp0",O_WRONLY,O_TRUNC);
   	 if(fd==-1)
     {
         SYS_ERROR("PPP Open Failed.\r\n");
    	 return succeed_type_failed;
	 }
     else
     {
         lseek(fd,0,SEEK_SET);
         write(fd,wrbufhead,strlen(wrbufhead));
         write(fd,strname,strlen(strname));
         write(fd,strpasswd,strlen(strpasswd));
         write(fd,wrbuftail,strlen(wrbuftail));
         close(fd);
     }
	return succeed_type_succeed;
}
