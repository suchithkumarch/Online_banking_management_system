#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "pds.h"
#include "user.h"

int main(int argc, char *argv[])
{
    char db_name[30];
    char txt_name[30];
    if( argc != 3)
    {
        fprintf(stderr, "Usage: %s  <db-name> <text-file-name>\n", argv[0]);
        exit(1);
    }
    strcpy(db_name,argv[1]);
    strcpy(txt_name,argv[2]);
    printf("normal size-%d\n",sizeof(normal_user));
    printf("joint size-%d\n",sizeof(joint_user));
    printf("admin size-%d\n",sizeof(admin));
    int status = pds_create_schema(txt_name);
    pds_db_open( db_name );
    //testing admin and respective functions
    admin admn={0,"admin","pwd"};
    admin test_admn;
    put_rec_by_key( "admin",0, &admn);
    search_admin( 0, &test_admn );
    print_admin( &admn );
    print_admin( &test_admn ); 
    //testing normal_user and respective functions - print, add, update, delete
    normal_user t2;
    normal_user t={1,"normal_user","pwd",5};
    print_normal_user(&t);
    add_normal_user(&t);
    search_normal_user(1,&t2);
    print_normal_user(&t2);
    t2.balance=100;
    update_normal_user(&t2);
    print_normal_user(&t2);
    delete_normal_user(1);
    normal_user t3;
    search_normal_user(1,&t3);
    print_normal_user(&t3);
    add_normal_user(&t);
    normal_user t4;
    search_normal_user(1,&t4);
    print_normal_user(&t4);
    normal_user t5={0,"five","pwd",10};
    print_normal_user(&t5);
    add_normal_user(&t5);
    normal_user t6;
    search_normal_user(0,&t6);
    print_normal_user(&t6);
    //testing joint_user and respective functions - print, add, update, delete
    joint_user j2;
    joint_user j={2,"joint_user1","joint_user2","pwd",20};
    print_joint_user(&j);
    add_joint_user(&j);
    search_joint_user(2,&j2);
    print_joint_user(&j2);
    j2.balance=100;
    update_joint_user(&j2);
    print_joint_user(&j2);
    delete_joint_user(2);
    joint_user j3;
    search_joint_user(2,&j3);
    print_joint_user(&j3);
    add_joint_user(&j);
    joint_user j4;
    search_joint_user(2,&j4);
    print_joint_user(&j4);
    //This code created the schema and made 3 accounts - an admin, a normal user and a joint user for starters.
    pds_db_close( db_name );
    printf("Done\n");
    return 0;
}
