#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#define PORT 8000

typedef struct normal_user
{
    int account_num;
    char username[20];
    char password[20];
    float balance;
}normal_user;

typedef struct joint_user
{
    int account_num;
    char username1[20];
    char username2[20];
    char password[20];
    float balance;
}joint_user;

typedef struct admin
{
    int account_num;
    char username[20];
    char password[20];
}admin;

void print_normal_user(  normal_user *c );
int search_normal_user( int account_num,  normal_user *c );
int update_normal_user( normal_user *c );
int add_normal_user(  normal_user *c );
int search_normal_user_by_username( char *usr_name, normal_user *c, int *io_count );
int match_normal_user_and_username( normal_user *c, char *usr_name );

void print_joint_user(  joint_user *c );
int search_joint_user( int account_num,  joint_user *c );
int update_joint_user( joint_user *c );
int add_joint_user(  joint_user *c );
int search_joint_user_by_username( char *usr_name, joint_user *c, int *io_count );
int match_joint_user_and_username( joint_user *c, char *usr_name );

int delete_joint_user( int key );
int delete_normal_user( int key );

int search_admin( int account_num, admin *c );
void print_admin( admin *c );
#define USER_SUCCESS 0
#define USER_FAILURE 1

#endif
