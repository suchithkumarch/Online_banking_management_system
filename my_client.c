#include<stdio.h>
#include<stdlib.h>
//#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 8000

int main(int argc, char *argv[])
{
 struct sockaddr_in saddr;
 int skt = socket(AF_INET, SOCK_STREAM, 0);
 saddr.sin_family = AF_INET;
 saddr.sin_addr.s_addr = INADDR_ANY;
 saddr.sin_port = htons(PORT);
 connect(skt, (struct sockaddr *) &saddr, sizeof(saddr));
 printf("Welcome to the online banking management system\n");
 printf("Press 1 to login as a normal user\n");
 printf("Press 2 to login as a joint user\n");
 printf("Press 3 to login as the admin\n");
 int login_mode,account_number,action;
 char password[20];
 scanf("%d",&login_mode);
 write(skt, &login_mode, sizeof(int));
 //char db_name[]="user";
 
 if(login_mode==1)
 {
  printf("For logging in as normal user...\n");
  printf("Enter Account Number:\n");
  scanf("%d",&account_number);
  write(skt, &account_number, sizeof(int));
  printf("Enter Account Password:\n");
  char password1[20];
  scanf("%s",password1);
  write(skt, password1, sizeof(char)*20);
  int q;
  read(skt, &q, sizeof(int));
  if(q==0)
  {
   printf("Login successfull\n");
  }
  else
  {
   printf("Login failed\n");
   return 0;
  }
  printf("Press 1 to deposit\n");
  printf("Press 2 to withdraw\n");
  printf("Press 3 to enquire balance\n");
  printf("Press 4 to change password\n");
  printf("Press 5 to view details\n");
  printf("Press 6 to exit\n");
  scanf("%d",&action);
  write(skt, &action, sizeof(int));
  //perform corrosponding action
 
  if(action==1)
  {
   printf("Enter deposit amount\n");
   float dep;
   scanf("%f",&dep);
   write(skt, &dep, sizeof(float));
   float bal;
   read(skt, &bal, sizeof(float));
   printf("%f is the updated balance after depositing\n",bal);
  }
  else if(action==2)
  {
   printf("Enter withdrawl amount\n");
   float dep;
   scanf("%f",&dep);
   write(skt, &dep, sizeof(float));
   float bal;
   read(skt, &bal, sizeof(float));
   printf("%f is the latest balance after withdrawl\n",bal);   
  }
  else if(action==3)
  {
   float bal;
   read(skt, &bal, sizeof(float));
   printf("Balance amount\n:%f",bal);
  }
  else if(action==4)
  {
   printf("Enter new password\n");
   char pswd[20];
   scanf("%s",pswd);
   write(skt, &pswd, 20*sizeof(char));
   int sts;
   read(skt, &sts, sizeof(int));
   if(sts==0)
   printf("Password updated successfully\n");
   else
   printf("Password update failed\n");
  }  
  else if(action==5)
  {
   printf("View details:\n");
   int account_num=32;
   char username[20];
   float balance;
   int r=read(skt, &account_num, sizeof(int));
   printf("Account number: %d,",account_num);  
   read(skt, &balance, sizeof(float));  
   printf("balance: %f,",balance);  
   read(skt, &username, 20*sizeof(char));
   printf("username: %s\n",username);  
  }  
  else if(action==6)
  {
   printf("Exiting..\n");
  }   
 }
 
 else if(login_mode==2)
 {
  printf("For logging in as joint user...\n");
  printf("Enter Account Number:\n");
  scanf("%d",&account_number);
  write(skt, &account_number, sizeof(int));
  printf("Enter Account Password:\n");
  char password1[20];
  scanf("%s",password1);
  write(skt, password1, sizeof(char)*20);
  int q;
  read(skt, &q, sizeof(int));
  if(q==0)
  {
   printf("Login successfull\n");
  }
  else
  {
   printf("Login failed\n");
   return 0;
  }
  printf("Press 1 to deposit\n");
  printf("Press 2 to withdraw\n");
  printf("Press 3 to enquire balance\n");
  printf("Press 4 to change password\n");
  printf("Press 5 to view details\n");
  printf("Press 6 to exit\n");
  scanf("%d",&action);
  write(skt, &action, sizeof(int));
  //perform corrosponding action
 
  if(action==1)
  {
   printf("Enter deposit amount\n");
   float dep;
   scanf("%f",&dep);
   write(skt, &dep, sizeof(float));
   float bal;
   read(skt, &bal, sizeof(float));
   printf("%f is the updated balance after depositing\n",bal);
  }
  else if(action==2)
  {
   printf("Enter withdrawl amount\n");
   float dep;
   scanf("%f",&dep);
   write(skt, &dep, sizeof(float));
   float bal;
   read(skt, &bal, sizeof(float));
   printf("%f is the latest balance after withdrawl\n",bal);   
  }
  else if(action==3)
  {
   float bal;
   read(skt, &bal, sizeof(float));
   printf("Balance amount\n:%f",bal);
  }
  else if(action==4)
  {
   printf("Enter new password\n");
   char pswd[20];
   scanf("%s",pswd);
   write(skt, &pswd, 20*sizeof(char));
   int sts;
   read(skt, &sts, sizeof(int));
   if(sts==0)
   printf("Password updated successfully\n");
   else
   printf("Password update failed\n");
  }  
  else if(action==5)
  {
   printf("View details:\n");
   int account_num;
   char username[20],username2[20];
   float balance;
   read(skt, &account_num, sizeof(int));
   read(skt, &username, 20*sizeof(char));
   read(skt, &username2, 20*sizeof(char));
   read(skt, &balance, sizeof(float));  
   printf("Account number: %d, username1: %s,username2: %s, balance: %f\n",account_num,username,username2,balance);  
  }  
  else if(action==6)
  {
   printf("Exiting..\n");
  }   
 }

 else if(login_mode==3)
 {
  printf("For logging in as the admin...\n");
  printf("Enter Account Number:\n");
  scanf("%d",&account_number);
  write(skt,&account_number,sizeof(int));
  printf("Enter Account Password:\n");
  char password1[20];
  scanf("%s",password1);
  write(skt,&password1,20*sizeof(char));
  int log_sts;
  read(skt,&log_sts,sizeof(int));
  if(log_sts==0)
  {
   printf("Login successfull\n");
  }
  else
  {
   printf("Login failed\n");
   return 0;
  }  
  printf("Press 1 to add a new account\n");
  printf("Press 2 to delete an account\n");
  printf("Press 3 to modify an account details\n");
  printf("Press 4 to search for an account\n");
  scanf("%d",&action);
  write(skt,&action,sizeof(int));
  //perform corrosponding action
  printf("Normal account(0) or Joint account(1)?\n");
  int acc;
  scanf("%d",&acc);
  write(skt,&acc,sizeof(int));
  if(action==1)
  {
   printf("Enter new account details\n");
   if(acc==0)
   {
    char username[20];
    float balance;
    char password[20];
    int account_num;
    printf("Username: ");
    scanf("%s",username);
    write(skt,&username,20*sizeof(char));
    printf("Balance: ");
    scanf("%f",&balance); 
    write(skt,&balance,sizeof(float));
    printf("Password: ");
    scanf("%s",password);   
    write(skt,&password,20*sizeof(char));
    printf("Account number: ");
    scanf("%d",&account_num);   
    write(skt,&account_num,sizeof(int));  
    int sts;
    read(skt,&sts,sizeof(int));  
    if(sts==0)
    printf("New account added successfully\n");
    else
    printf("Error, new account not added\n");
   }
   else if(acc==1)
   {
    char username[20],username2[20];
    float balance;
    char password[20];
    int account_num;
    printf("Username1: ");
    scanf("%s",username);
    write(skt,&username,20*sizeof(char));
    printf("Username2: ");
    scanf("%s",username2);
    write(skt,&username2,20*sizeof(char));    
    printf("Balance: ");
    scanf("%f",&balance); 
    write(skt,&balance,sizeof(float));
    printf("Password: ");
    scanf("%s",password);   
    write(skt,&password,20*sizeof(char));
    printf("Account number: ");
    scanf("%d",&account_num);   
    write(skt,&account_num,sizeof(int));  
    int sts;
    read(skt,&sts,sizeof(int));  
    if(sts==0)
    printf("New account added successfully\n");
    else
    printf("Error, new account not added\n");
   }
  }
  else if(action==2)
  {
   printf("Enter the account number to delete\n");
   int ac_n,sts;
   scanf("%d",&ac_n);
   write(skt,&ac_n,sizeof(int));
   read(skt,&sts,sizeof(int));
   if(sts==0)
   printf("Account deletion done\n");
   else
   printf("Error, could not delete the account\n");
  }  
  else if(action==3)
  {
   printf("Enter updated account details\n");
   if(acc==0)
   {
    char username[20];
    float balance;
    char password[20];
    int account_num;
    printf("Username: ");
    scanf("%s",username);
    printf("Balance: ");
    scanf("%f",&balance); 
    printf("Password: ");
    scanf("%s",password);   
    printf("Account number: ");
    scanf("%d",&account_num);
    write(skt, &account_num, sizeof(int));
    write(skt, &username, 20*sizeof(char));
    write(skt, &password, 20*sizeof(char));
    write(skt, &balance, sizeof(float));
    int sts;
    read(skt,&sts,sizeof(int));
    if(sts==0)
    printf("Update done\n");
    else
    printf("Error, couldn't update\n");
   }
   else if(acc==1)
   {
    char username1[20],username2[20];
    float balance;
    char password[20];
    int account_num;
    printf("Username1: ");
    scanf("%s",username1);
    printf("Username2: ");
    scanf("%s",username2);    
    printf("Balance: ");
    scanf("%f",&balance); 
    printf("Password: ");
    scanf("%s",password);   
    printf("Account number: ");
    scanf("%d",&account_num);
    write(skt, &account_num, sizeof(int));
    write(skt, &username1, 20*sizeof(char));
    write(skt, &username2, 20*sizeof(char));    
    write(skt, &password, 20*sizeof(char));
    write(skt, &balance, sizeof(float));
    int sts;
    read(skt,&sts,sizeof(int));
    if(sts==0)
    printf("Update done\n");
    else
    printf("Error, couldn't update\n");    
   }   
  }   
  else if(action==4)
  {
   int ac_num;
   printf("Enter the account number to search\n");
   scanf("%d",&ac_num);
   write(skt,&ac_num,sizeof(int));
   printf("Searching for %d...\n",ac_num);
   if(acc==0)
   {
    int account_num;
    float balance;
    char username[20];
    read(skt, &account_num, sizeof(int));
    read(skt, &balance, sizeof(float));
    read(skt, &username, 20*sizeof(char));
    printf("Account number: %d, username: %s, balance: %f\n",account_num,username,balance);  
   }
   else if(acc==1)
   {
    int account_num;
    float balance;
    char username[20],username2[20];
    read(skt, &account_num, sizeof(int));
    read(skt, &balance, sizeof(float));
    read(skt, &username, 20*sizeof(char));    
    read(skt, &username2, 20*sizeof(char)); 
    printf("Account number: %d, username1: %s,username2: %s, balance: %f\n",account_num,username,username2,balance);          
   }   
  }   
 }
 else
 {
  printf("Invalid input\n");
 }
 printf("\nThank you for visiting....Bye\n");
 close(skt);
 return 0;
}
