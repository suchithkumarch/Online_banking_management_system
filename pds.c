#include <stdio.h> 
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>


#include "pds.h"
#include"bst.h"
struct PDS_DB_Master db_handle;
void processLine(char *line,struct PDS_DBInfo *dummy){
    char command[50], param1[50], param2[50], param3[50];
    sscanf(line, "%s%s%s%s", command, param1, param2, param3);
    if(!strcmp(command,"entity")){
        
        dummy->entities[dummy->num_entities].entity_size=atoi( param2);
        strcpy(dummy->entities[dummy->num_entities].entity_name,param1);
        dummy->num_entities+=1;
    }
    else if(!strcmp(command,"relationship")){
        strcpy(dummy->links[dummy->num_relationships].link_name,param1);
        strcpy(dummy->links[dummy->num_relationships].pds_name1,param2);
        strcpy(dummy->links[dummy->num_relationships].pds_name2,param3);
        dummy->num_relationships+=1;

    }
}
int pds_create_schema (char *schema_file_name){
    char schema_file[30];
    char line[50];
    char name[50];
    struct PDS_DBInfo *dummy=(struct PDS_DBInfo *)malloc(sizeof(struct PDS_DBInfo));
    
    strcpy(schema_file,schema_file_name);
    
    
    FILE *fp=(FILE*)fopen(schema_file,"r");
    fscanf(fp,"%s\n",name);
    strcpy(dummy->db_name,name);
    while(fgets(line,sizeof(line)-1,fp)){
        processLine(line,dummy);
    }
    strcat(name,".db");
    
    FILE *db=(FILE *)fopen(name,"wb");
    fwrite(dummy,sizeof(struct PDS_DBInfo),1,db);
    // fseek(db,0,SEEK_SET);
    // struct PDS_DBInfo *dummy1=(struct PDS_DBInfo *)malloc(sizeof(struct PDS_DBInfo));
    // fread(dummy1,sizeof(struct PDS_DBInfo),1,db);
    // printf("%d\n",dummy1->num_entities);
    fclose(fp);
    fclose(db);
    return PDS_SUCCESS;

}
struct PDS_LinkFileInfo *get_link(char *e1,char *e2){
    int length=db_handle.db_info.num_relationships;
    int i=0;
    
    for (i=0;i<length;i++){
        if (!strcmp(db_handle.db_info.links[i].pds_name1,e1) && !strcmp(db_handle.db_info.links[i].pds_name2,e2)){
           
            return &db_handle.rel_info[i];
        }  
    }
}
void link_open(char *entity1,char *entity2){
    char file_name[50];
    strcpy(file_name,entity1);
    strcat(file_name,"_");
    strcat(file_name,entity2);
    strcat(file_name,".lnk");
    struct PDS_LinkFileInfo *link_handle=get_link(entity1,entity2);
    // printf("%s",file_name);
    link_handle->pds_link_fp=(FILE *)fopen(file_name,"rb+");
    link_handle->link_status=1;
    


}
int pds_db_open( char *db_name ){
    char name[50];
    strcpy(name,db_name);
    
    strcat(name,".db");
    FILE *fp=(FILE *)fopen(name,"rb");
    struct PDS_DBInfo dummy;
    //  struct PDS_DBInfo *dummy=(struct PDS_DBInfo *)malloc(sizeof(struct PDS_DBInfo));
    fread(&dummy,sizeof(struct PDS_DBInfo),1,fp);
    db_handle.db_info=dummy;
    db_handle.db_status=PDS_DB_OPEN;
    for(int i=0;i<dummy.num_entities;i++){
        struct PDS_RepoInfo a;
        strcpy(a.entity_name,dummy.entities[i].entity_name);
        a.entity_size=dummy.entities[i].entity_size;
        db_handle.entity_info[i]=a;
        pds_open(a.entity_name,a.entity_size);
        // db_handle.entity_info[i]=
    }
    for(int i=0;i<dummy.num_relationships;i++){
        struct PDS_LinkFileInfo a;
        strcpy(a.link_name,dummy.links[i].link_name);
        db_handle.rel_info[i]=a;
        link_open(dummy.links[i].pds_name1,dummy.links[i].pds_name2);

    }
return 0;
}


int pds_load_ndx(struct PDS_RepoInfo * repo_handle){
    rewind(repo_handle->pds_ndx_fp);
    if (fread(repo_handle->free_list,sizeof(int),MAX_FREE,repo_handle->pds_ndx_fp)==100){
        // printf("");
    
    // else{
    //     for(int i=0;i<100;i++){
    //         repo_handle.free_list[i]=-1;
    //     }
    // }
    int status;
    
        struct PDS_NdxInfo index_entry;
        while(fread(&index_entry,sizeof(index_entry),1,repo_handle->pds_ndx_fp)==1){
       
        struct PDS_NdxInfo *jai=(struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
        jai->key=index_entry.key;
        jai->offset=index_entry.offset;
        // if (count==1){
         status=bst_add_node(&repo_handle->pds_bst,index_entry.key,jai);
        if(status!=BST_SUCCESS){
           
            return status;
            
        }
        else{
            status=PDS_SUCCESS;
        }

}
return PDS_SUCCESS;
    }
    else{
        for(int i=0;i<100;i++){
            repo_handle->free_list[i]=-1;
        }
        return PDS_SUCCESS;
    }
}
struct PDS_RepoInfo * get_entity(char *name){
    int length=db_handle.db_info.num_entities;
    for(int i=0;i<length;i++){
        if (!strcmp(db_handle.entity_info[i].entity_name,name)){
            // printf("%s",db_handle.entity_info[i].entity_name);
            return &db_handle.entity_info[i];
        }
    }

}
int pds_open( char *repo_name, int rec_size ){
    char repo_file[30];
    char index_file[30];
    struct PDS_RepoInfo *repo_handle=get_entity(repo_name);
    if(repo_handle->repo_status==PDS_DB_OPEN){
        return PDS_DB_ALREADY_OPEN;
    }
    strcpy(repo_file,repo_name);
    
    strcat(repo_file,".dat");
    strcpy(index_file,repo_name);
   
    strcat(index_file,".ndx");
    
    
    strcpy(repo_handle->entity_name,repo_name);
    // printf("%s %s\n",repo_file,index_file);
    repo_handle->pds_data_fp=(FILE *)fopen(repo_file,"rb+");
    repo_handle->pds_ndx_fp=(FILE *)fopen(index_file,"rb+");
    if(repo_handle->pds_data_fp==NULL){
        
        perror(repo_file);
    }
    if(repo_handle->pds_ndx_fp==NULL){
        perror(index_file);
    }
    repo_handle->repo_status=PDS_DB_OPEN;
    repo_handle->entity_size=rec_size;
    repo_handle->pds_bst=NULL;
    char string[256];
    int status;
    
        

    status=pds_load_ndx(repo_handle);
    fclose(repo_handle->pds_ndx_fp);
    // bst_print(repo_handle.pds_bst);
    return status;

}



int put_rec_by_key( char *entity_name,int key, void *rec ){
    struct PDS_RepoInfo *repo_handle=get_entity(entity_name);
    // printf("%s",repo_handle->entity_name);
    fseek(repo_handle->pds_data_fp, 0, SEEK_END);
   int offset,status,writesize;
   struct PDS_NdxInfo *ndx_entry;
   int flag=0;
   for(int i=0;i<100;i++){
       if(repo_handle->free_list[i]!=-1){
           offset=repo_handle->free_list[i];
        //    flag=1;
        repo_handle->free_list[i]=-1;
           break;
       }
       else{
            offset=ftell(repo_handle->pds_data_fp);

          
       }
   }
// offset=ftell(repo_handle.pds_data_fp);
    fseek(repo_handle->pds_data_fp,offset,SEEK_SET);
   
   ndx_entry=(struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
   ndx_entry->key=key;
   ndx_entry->offset=offset;
   status=bst_add_node(&repo_handle->pds_bst,key,ndx_entry);
//    printf("\nstatus jai :%d\n",status);
   if(status!=BST_SUCCESS){
    //    fprintf(stderr,"unable to add index entry for key %d- status value %d\n",key,status);
       free(ndx_entry);
       fseek(repo_handle->pds_data_fp,offset,SEEK_SET);
    //    status=PDS_ADD_FAILED;
   }
   else{
       fwrite(&key,sizeof(int),1,repo_handle->pds_data_fp);
   writesize=fwrite(rec,repo_handle->entity_size,1,repo_handle->pds_data_fp);
       status=PDS_SUCCESS;
   }
   return status;

}

// update
// Search for index entry in BST
// Seek to the file location based on offset in index entry
// Overwrite the existing record with the given record
// In case of any error, return PDS_MODIFY_FAILED
int update_by_key( char *entity_name,int key, void *newrec ){
    struct PDS_RepoInfo *repo_handle=get_entity(entity_name);
    struct BST_Node *bst_node;
    int offset;
    struct PDS_NdxInfo *ndx_entry;
    bst_node=bst_search(repo_handle->pds_bst,key);
    if(bst_node==NULL){
        return PDS_MODIFY_FAILED;
    }
    ndx_entry=(struct PDS_NdxInfo*)bst_node->data;
    offset=ndx_entry->offset;
    fseek(repo_handle->pds_data_fp,offset,SEEK_SET);
    
    struct flock l;
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_CUR;
        l.l_start = 0;
        l.l_len = repo_handle->entity_size;
        l.l_pid = getpid();
        fcntl(repo_handle->pds_data_fp, F_SETLKW, &l);
    
    fwrite(&key,sizeof(int),1,repo_handle->pds_data_fp);
    // struct Contact *c=(struct Contact *)newrec;
    // printf("%d %s",c->contact_id,c->contact_name);
    fwrite(newrec,repo_handle->entity_size,1,repo_handle->pds_data_fp);
    l.l_type = F_UNLCK;
    fcntl(repo_handle->pds_data_fp, F_SETLK, &l);
    return PDS_SUCCESS;
}
// pds_delete
// Search for index entry in BST
// store the offset value in to free list

int delete_by_key( char *entity_name,int key ){
    struct PDS_RepoInfo *repo_handle=get_entity(entity_name);
    struct BST_Node *bst_node;
    int offset;
    struct PDS_NdxInfo *ndx_entry;
    bst_node=bst_search(repo_handle->pds_bst,key);
    if(bst_node==NULL){
        return PDS_DELETE_FAILED;
    }
    
    int length=db_handle.db_info.num_relationships;
    int i=0;
    for (i=0;i<length;i++){
        if (!strcmp(db_handle.db_info.links[i].pds_name1,entity_name)){
                struct PDS_Link a;
                
             rewind(db_handle.rel_info[i].pds_link_fp);
         while(fread(&a,sizeof(a),1,db_handle.rel_info[i].pds_link_fp)==1){
             if (a.key==key || a.linked_key==key){
                 return PDS_DELETE_FAILED;
                 
             }
         }
            // return &db_handle.rel_info[i];
        }  
    }
    ndx_entry=(struct PDS_NdxInfo*)bst_node->data;
    offset=ndx_entry->offset;
    for(int i=0;i<100;i++){
        if(repo_handle->free_list[i]==-1){
            repo_handle->free_list[i]=offset;
            break;
        }
    }
    int k=bst_del_node(&repo_handle->pds_bst,key);
    // bst_print(repo_handle->pds_bst);
    return k;
}
int get_rec_by_ndx_key(char *entity_name, int key, void *rec ){
    struct PDS_RepoInfo *repo_handle=get_entity(entity_name);
    struct PDS_NdxInfo *ndx_entry;
    struct BST_Node *bst_node;
    int offset,status,readsize;
    // bst_print(repo_handle->pds_bst);
    bst_node=bst_search(repo_handle->pds_bst,key);
    // print(repo_handle.pds_bst);
    if(bst_node==NULL){
        status=PDS_REC_NOT_FOUND;
    }
    else{
        ndx_entry=(struct PDS_NdxInfo*)bst_node->data;
        offset=ndx_entry->offset;
        fseek(repo_handle->pds_data_fp,offset,SEEK_SET);
        fread(&key,sizeof(int),1,repo_handle->pds_data_fp);
        fread(rec,repo_handle->entity_size,1,repo_handle->pds_data_fp);
    //     struct Contact *c=(struct Contact *)rec;
    // printf("%d %s",c->contact_id,c->contact_name);
        status=PDS_SUCCESS;

    }
    return status;

}
int get_rec_by_non_ndx_key( char *entity_name,
void *key,  			/* The search key */
void *rec,  			/* The output record */
int (*matcher)(void *, void *), /*Function pointer for matching*/
int *count  		/* Count of the number of records read */
){
    
  
    *count=0;
    
   struct PDS_RepoInfo *repo_handle=get_entity(entity_name);
    
    int dummy_key;
  
    rewind(repo_handle->pds_data_fp);

        while(fread(&dummy_key,sizeof(dummy_key),1,repo_handle->pds_data_fp)==1 && fread(rec,repo_handle->entity_size,1,repo_handle->pds_data_fp)==1){
     
      
        int ans=matcher(rec,key);
    
        if (ans==1){
            if(bst_search(repo_handle->pds_bst,dummy_key)!=NULL){
            int waste=*count+1;
        *count=waste;
            }
        }
        else if(ans==2){
            return 100;
        }
        else{
            // return PDS_SUCCESS;
            (*count)++;
            printf("%d\n",*count);
            // *((struct Contact *)rec)=*jai;
            if(bst_search(repo_handle->pds_bst,dummy_key)==NULL){
                return PDS_REC_NOT_FOUND;
            }
     
            break;
        }

        }
        
            return PDS_SUCCESS;
      

}

void preorder(struct BST_Node *root,FILE *fptr){
        //  printf("%d ",root->key);
        if (root==NULL){
            return;
        }
        else{
            
            
            if(fptr==NULL){
                printf("2");
            }
            // struct PDS_NdxInfo *ndx=(struct PDS_NdxInfo *)root->data;
            
            // ndx->key=root->key;
            // // printf("%d %d",ndx->key,ndx->offset);
            // fwrite(ndx,sizeof(ndx),1,fptr);
            fwrite(root->data, sizeof(struct PDS_NdxInfo),1,fptr);
            // fwrite("\n", sizeof(char), 1, fptr);
            preorder(root->left_child,fptr);
            preorder(root->right_child,fptr);
        }

}
int pds_close(char *entity_name){
    char index_file[30];
    // printf("%s",repo_handle.pds_name);
    struct PDS_RepoInfo *repo_handle=get_entity(entity_name);
      strncpy(index_file,repo_handle->entity_name,30);
      
    // strcpy(repo_handle.pds_name,repo_handle.pds_name);
    strcat(index_file,".ndx");
    // printf("%s",index_file);
    
    FILE *index_pointer;
    index_pointer=(FILE *)fopen(index_file,"wb");
    fwrite(repo_handle->free_list,sizeof(repo_handle->free_list),1,index_pointer);
    if(index_pointer==NULL){
        printf("yup");
        perror(index_file);
    }
    // for(int i=0;i<100;i++){
    //     printf("%d",repo_handle.free_list[i]);
    // }
    // bst_print(repo_handle.pds_bst);
    preorder(repo_handle->pds_bst,index_pointer);
    bst_destroy(repo_handle->pds_bst);
    repo_handle->pds_bst=NULL;
    repo_handle->repo_status=PDS_DB_CLOSED;
    strcpy(repo_handle->entity_name,"");
    fclose(repo_handle->pds_data_fp);
    // fclose(repo_handle.pds_ndx_fp);
    fclose(index_pointer);
    return PDS_SUCCESS;
    
    
}
int pds_db_close(){
    for(int i=0;i<db_handle.db_info.num_entities;i++){
        pds_close(db_handle.entity_info[i].entity_name);

    } 
    int length=db_handle.db_info.num_relationships;
    int i=0;
    for (i=0;i<length;i++){
        if (db_handle.rel_info[i].pds_link_fp!=NULL){
        fclose(db_handle.rel_info[i].pds_link_fp);
        }
    }
    return 0;
}

// P09) link_objects
// Retrieve the link_info from PDS_DB_Master corresponding to given link_name
// Adds a link entry to the link file establishing link between two given keys
// Example: link_data("student_course", 12, 15);
// Give error key1 does not exist or key2 does not exist or <key, linke_key> combination already exists
int link_data(char *link_name, int key, int linked_key){
    char e1[30];
   char e2[30];
   
   char * token = strtok(link_name, "_");
   int flag=0;
   // loop through the string to extract all other tokens
   while( token != NULL ) {
      if (flag==0){
         strcpy(e1,token);
         flag=1;

      }
      else{
         strcpy(e2,token);

      }
       token = strtok(NULL, " ");
   }
    struct PDS_LinkFileInfo *link_handle=get_link(e1,e2);
    struct PDS_RepoInfo *entity1=get_entity(e1);
    struct PDS_RepoInfo *entity2=get_entity(e2);
    rewind(link_handle->pds_link_fp);
    if (bst_search(entity1->pds_bst,key)!=NULL && bst_search(entity2->pds_bst,linked_key)!=NULL){
         struct PDS_Link a;
         while(fread(&a,sizeof(a),1,link_handle->pds_link_fp)==1){
             if (a.key==key && a.linked_key==linked_key){
                 return PDS_LINK_FAILED;
             }
         }
    a.key=key;
    a.linked_key=linked_key;
    fwrite(&a,sizeof(struct PDS_Link ),1,link_handle->pds_link_fp);
    }
    else{
        return PDS_LINK_FAILED;
    }
    
 return 0;  
}
// P10) getLinkedData
// Returns the linked data for a given key
// Example: Retrieve all courses (linked_data) done by a given student (data_key)
int get_linked_data( char *link_name, int data_key, struct PDS_LinkedKeySet *linked_data ){
     char e1[30];
   char e2[30];
   
   char * token = strtok(link_name, "_");
   int flag=0;
   // loop through the string to extract all other tokens
   while( token != NULL ) {
      if (flag==0){
         strcpy(e1,token);
         flag=1;

      }
      else{
         strcpy(e2,token);

      }
       token = strtok(NULL, " ");
   }
    struct PDS_LinkFileInfo *link_handle=get_link(e1,e2);
    struct PDS_Link a;
    int counter=0;
    rewind(link_handle->pds_link_fp);
         while(fread(&a,sizeof(a),1,link_handle->pds_link_fp)==1){
             if (a.key==data_key){
                 linked_data->key=data_key;
                //  printf("%d",a.linked_key);
                 linked_data->linked_keys[counter]=a.linked_key;
                 
                 linked_data->link_count+=1;
                 counter++;
                //  linked_data->linked_keys[]
                 
             }
         }
return counter;
}
