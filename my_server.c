#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdbool.h>
#define PORT 8000

#include "pds.h"
#include "user.h"

void* connection_handler(void* new_skt);
int main(int argc, char *argv[])
{
 struct sockaddr_in saddr, caddr;
 int new_skt, len = sizeof(caddr);
 int skt = socket(AF_INET, SOCK_STREAM, 0);
 saddr.sin_family = AF_INET;
 saddr.sin_addr.s_addr = INADDR_ANY;
 saddr.sin_port = htons(PORT);
 bind(skt, (struct sockaddr *) &saddr, sizeof(saddr));
 listen(skt, 5);
 printf("Waiting for client...\n");
 while(1)
 {
  new_skt = accept(skt, (struct sockaddr *) &caddr, &len);
  printf("Connected to client\n");
  pthread_t thrid;
  pthread_create(&thrid, NULL, connection_handler, (void *) &new_skt);
 }
}
void* connection_handler(void* new_skt)
{
 int skt = *(int *) new_skt;
 int login_mode,account_number,action;
 char password[20];
 read(skt, &login_mode, sizeof(int));
 char db_name[]="user";
 pds_db_open(db_name);
 if(login_mode==1)
 {
  printf("Request received for logging in as normal user...\n");
  read(skt, &account_number, sizeof(int));
  printf("Searcing %d\n",account_number);
  normal_user test;
  search_normal_user(account_number, &test);
  print_normal_user(&test);
  char password1[20];
  read(skt, password1, sizeof(char)*20);
  int q;
  if(!(strcmp(test.password, password1)))
  {
   q=0;
   write(skt, &q, sizeof(int));
   printf("Login successfull\n");
  }
  else
  {
   q=1;
   pds_db_close(db_name);
   write(skt, &q, sizeof(int));
   printf("Login failed\n");
   return 0;
  }
  read(skt, &action, sizeof(int));
  if(action==1)
  {
   float dep;
   read(skt, &dep, sizeof(float));
   test.balance+=dep;
   update_normal_user(&test );
   write(skt, &test.balance, sizeof(float));
  }
  else if(action==2)
  {
   float dep;
   read(skt, &dep, sizeof(float));
   if(test.balance>=dep)
   test.balance-=dep;
   else
   printf("Error, Can't withdraw due to insufficient balance\n");
   update_normal_user(&test );
   write(skt, &test.balance, sizeof(float));
  }
  else if(action==3)
  {
   write(skt, &test.balance, sizeof(float));
  }
  else if(action==4)
  {
   read(skt, &test.password, 20*sizeof(char));
   int sts= update_normal_user(&test);
   write(skt, &sts, sizeof(int));
  }  
  else if(action==5)
  {
   write(skt, &test.account_num, sizeof(int));
   write(skt, &test.balance, sizeof(float));
   write(skt, &test.username, 20*sizeof(char));
   printf("Acc no:%d,user name:%s,balance:%f,pwd:%s,\n", test.account_num,test.username,test.balance,test.password);
  }  
  else if(action==6)
  {
   //printf("Exiting..\n");
  }   
 }
 
 else if(login_mode==2)
 {
  printf("Request received for logging in as joint user...\n");
  read(skt, &account_number, sizeof(int));
  //verify
  printf("Searcing %d\n",account_number);
  joint_user test;
  search_joint_user(account_number, &test);
  print_joint_user(&test);
  char password1[20];
  read(skt, password1, sizeof(char)*20);
  int q;
  if(!(strcmp(test.password, password1)))
  {
   q=0;
   printf("Login successfull\n");
  }
  else
  {
   q=1;
   pds_db_close(db_name);
   printf("Login failed\n");
   return 0;
  }
  write(skt, &q, sizeof(int));
  read(skt, &action, sizeof(int));
  //perform corrosponding action
  if(action==1)
  {
   float dep;
   read(skt, &dep, sizeof(float));
   test.balance+=dep;
   update_joint_user(&test);
   write(skt, &test.balance, sizeof(float));
  }
  else if(action==2)
  {
   float dep;
   read(skt, &dep, sizeof(float));
   if(test.balance>=dep)
   test.balance-=dep;
   else
   printf("Error, Can't withdraw due to insufficient balance\n");
   update_joint_user(&test);
   write(skt, &test.balance, sizeof(float));
  }
  else if(action==3)
  {
   write(skt, &test.balance, sizeof(float));
  }
  else if(action==4)
  {
   read(skt, &test.password, 20*sizeof(char));
   int sts= update_joint_user(&test);
   write(skt, &sts, sizeof(int));
  }  
  else if(action==5)
  {
    write(skt, &test.account_num, sizeof(int));
    write(skt, &test.username1, 20*sizeof(char));
    write(skt, &test.username2, 20*sizeof(char));
    write(skt, &test.balance, sizeof(float));
  }  
  else if(action==6)
  {
   //printf("Exiting..\n");
  }   
 }
 
 else if(login_mode==3)
 {
  printf("Requested for logging in as the admin...\n");
  read(skt,&account_number,sizeof(int));
  //verify
  admin test;
  printf("Searching for %d\n",account_number);
  search_admin(account_number, &test);
  print_admin(&test);
  char password1[20];
  read(skt,&password1,20*sizeof(char));
  int log_sts;
  if(!(strcmp(test.password, password1)))
  {
   log_sts=0;
   printf("Login successfull\n");
  }
  else
  {
   log_sts=1;
   pds_db_close(db_name);
   printf("Login failed\n");
   return 0;
  }  
  write(skt,&log_sts,sizeof(int));
  read(skt,&action,sizeof(int));
  int acc;
  read(skt,&acc,sizeof(int));
  if(action==1)
  {
   printf("Enter new account details\n");
   if(acc==0)
   {
    normal_user new_user;
    read(skt,&new_user.username,sizeof(char)*20);
    read(skt,&new_user.balance,sizeof(float));  
    read(skt,&new_user.password,sizeof(char)*20);   
    read(skt,&new_user.account_num,sizeof(int));
    print_normal_user(&new_user);    
    int sts=add_normal_user(&new_user);
    normal_user test_user;
    search_normal_user(new_user.account_num,&test_user);
    print_normal_user(&test_user);
    write(skt,&sts,sizeof(int));
   }
   else if(acc==1)
   {
    joint_user new_user;
    read(skt,&new_user.username1,sizeof(char)*20);
    read(skt,&new_user.username2,sizeof(char)*20);
    read(skt,&new_user.balance,sizeof(float)); 
    read(skt,&new_user.password,sizeof(char)*20);  
    read(skt,&new_user.account_num,sizeof(int));
    print_joint_user(&new_user);    
    int sts=add_joint_user(&new_user);
    joint_user test_user;
    search_joint_user(new_user.account_num,&test_user);
    print_joint_user(&test_user);
    write(skt,&sts,sizeof(int));
   }   
  }
  else if(action==2)
  {
   int ac_n;
   read(skt,&ac_n,sizeof(int));
   int sts=1;
   if(acc==0)
   {
    sts=delete_normal_user(ac_n);    
   }
   else if(acc==1)
   {
    sts=delete_joint_user(ac_n);  
   }
   write(skt,&sts,sizeof(int));
  }  
  else if(action==3)
  {
   printf("Enter updated account details\n");
   if(acc==0)
   {
    normal_user new_user;
    read(skt, &new_user.account_num, sizeof(int));
    read(skt, &new_user.username, 20*sizeof(char));
    read(skt, &new_user.password, 20*sizeof(char));
    read(skt, &new_user.balance, sizeof(float));
    int sts=update_normal_user(&new_user);
    write(skt,&sts,sizeof(int));
   }
   else if(acc==1)
   {
    joint_user new_user;
    read(skt, &new_user.account_num, sizeof(int));
    read(skt, &new_user.username1, 20*sizeof(char));
    read(skt, &new_user.username2, 20*sizeof(char));
    read(skt, &new_user.password, 20*sizeof(char));
    read(skt, &new_user.balance, sizeof(float));
    int sts=update_joint_user(&new_user);
    write(skt,&sts,sizeof(int));
   }   
  }   
  else if(action==4)
  {
   int ac_num;
   read(skt,&ac_num,sizeof(int));
   printf("Searching for %d...\n",ac_num);
   if(acc==0)
   {
    normal_user c;
    search_normal_user(ac_num,&c);
    print_normal_user(&c); 
    write(skt, &c.account_num, sizeof(int));
    write(skt, &c.balance, sizeof(float));
    write(skt, &c.username, 20*sizeof(char));
   }
   else if(acc==1)
   {
    joint_user c;
    search_joint_user(ac_num,&c);
    print_joint_user(&c);     
    write(skt, &c.account_num, sizeof(int));
    write(skt, &c.balance, sizeof(float));
    write(skt, &c.username1, 20*sizeof(char));
    write(skt, &c.username2, 20*sizeof(char));
   }   
  }   
 }
 else
 {
  printf("Invalid input\n");
 }
 //pds_db_close(db_name);
 close(skt);
 printf("Closed connection\n\n");
 return 0;
}
