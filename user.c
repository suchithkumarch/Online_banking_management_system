#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pds.h"
#include "user.h"

void print_normal_user( normal_user *c )
{
	printf("Acc no:%d,user name:%s,balance:%f,pwd:%s\n", c->account_num,c->username,c->balance,c->password);
}

// Use get_rec_by_key function to retrieve USER
int search_normal_user( int account_num, normal_user *c )
{
	int status;
	//printf("in\n");

	status = get_rec_by_ndx_key( "normal_user", account_num, c );
//printf("in2\n");
	if( status == PDS_SUCCESS )
		status = USER_SUCCESS;
	else
		status = USER_FAILURE;

	return status;
}

int update_normal_user( normal_user *c )
{
	int status;

	status = update_by_key( "normal_user",c->account_num, c );

	if( status != PDS_SUCCESS ){
		fprintf(stderr, "Unable to add student with key %d. Error %d", c->account_num, status );
		status=USER_FAILURE;
	}
	else
		status=USER_SUCCESS;

	return status;
}
// Add the given USER into the repository by calling put_rec_by_key
int add_normal_user( normal_user *c )
{
	int status;
	int key=c->account_num;
	status = put_rec_by_key( "normal_user",key, c );

	if( status != PDS_SUCCESS ){
		fprintf(stderr, "Unable to add student with key %d. Error %d", c->account_num, status );
		status=USER_FAILURE;
	}
	else
		status=USER_SUCCESS;

	return status;
}
int search_normal_user_by_username( char *usr_name, normal_user *c, int *io_count )
{
//	printf("hell\n");
		return get_rec_by_non_ndx_key( "normal_user",usr_name, c , match_normal_user_and_username,io_count);
}

/* Return 0 if phone of the contact matches with phone parameter */
/* Return 1 if phone of the contact does NOT match */
/* Return > 1 in case of any other error */
int match_normal_user_and_username( normal_user *c, char *usr_name )
{
//	printf(" %d - %s - %s \n",c->contact_id,c->phone,phone );
	if( !strcmp(c->username,usr_name) )
		return 0;
	else
		return 1;
	return 2;
}

//joint
void print_joint_user( joint_user *c )
{
	printf("Acc no:%d,user name1:%s,user name2:%s,balance:%f,pwd:%s,\n", c->account_num,c->username1,c->username2,c->balance,c->password);
}

// Use get_rec_by_key function to retrieve USER
int search_joint_user( int account_num, joint_user *c )
{
	int status;
	//printf("in\n");

	status = get_rec_by_ndx_key( "joint_user", account_num, c );
//printf("in2\n");
	if( status == PDS_SUCCESS )
		status = USER_SUCCESS;
	else
		status = USER_FAILURE;

	return status;
}

int update_joint_user( joint_user *c )
{
	int status;

	status = update_by_key( "joint_user",c->account_num, c );

	if( status != PDS_SUCCESS ){
		fprintf(stderr, "Unable to add student with key %d. Error %d", c->account_num, status );
		status=USER_FAILURE;
	}
	else
		status=USER_SUCCESS;

	return status;
}
// Add the given USER into the repository by calling put_rec_by_key
int add_joint_user( joint_user *c )
{
	int status;

	status = put_rec_by_key( "joint_user",c->account_num, c );

	if( status != PDS_SUCCESS ){
		fprintf(stderr, "Unable to add student with key %d. Error %d", c->account_num, status );
		status=USER_FAILURE;
	}
	else
		status=USER_SUCCESS;

	return status;
}
int search_joint_user_by_username( char *usr_name, joint_user *c, int *io_count )
{
//	printf("hell\n");
		return get_rec_by_non_ndx_key( "joint_user",usr_name, c , match_joint_user_and_username,io_count);
}

/* Return 0 if phone of the contact matches with phone parameter */
/* Return 1 if phone of the contact does NOT match */
/* Return > 1 in case of any other error */
int match_joint_user_and_username( joint_user *c, char *usr_name )
{
//	printf(" %d - %s - %s \n",c->contact_id,c->phone,phone );
	if( !strcmp(c->username1,usr_name) || !strcmp(c->username2,usr_name))
		return 0;
	else
		return 1;
	return 2;
}
int delete_joint_user( int key )
{
 return delete_by_key("joint_user",key);
}
int delete_normal_user( int key )
{
 return delete_by_key("normal_user",key);
}
int search_admin( int account_num, admin *c )
{
	int status;
	//printf("in\n");

	status = get_rec_by_ndx_key( "admin", account_num, c );
//printf("in2\n");
	if( status == PDS_SUCCESS )
		status = USER_SUCCESS;
	else
		status = USER_FAILURE;

	return status;
}
void print_admin( admin *c )
{
	printf("Acc no:%d,user name:%s,pwd:%s,\n", c->account_num,c->username,c->password);
}




