#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address
{
	int id;
	int set;
	char *name;
	char *email;
};

struct Database
{
	int max_data;
    int max_rows;
	struct Address *rows;
};

struct Connection
{
	FILE *file;
	struct Database *db;
};

struct Connection *conn;

void Database_close()
{
	if (conn) {
		if (conn->db->rows) {
			int i = 0;
			for (i = 0; i < conn->db->max_rows; i++) {
		        free(conn->db->rows[i].name);
		        free(conn->db->rows[i].email);
	    	}
	    	free(conn->db->rows);
		}
		if (conn->file) fclose(conn->file);
		if (conn->db) free(conn->db);
		free(conn);
	}
}

void die(const char *message)
{
	if(errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}

	if(conn) Database_close(conn);
	exit(1);
}

void Address_print(struct Address *addr)
{
	printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load()
{
	int rc = 1;

	rc = fread(&conn->db->max_data, sizeof(int), 1, conn->file);
	if(rc != 1) die("Failed to load database");
	rc = fread(&conn->db->max_rows, sizeof(int), 1, conn->file);
	if(rc != 1) die("Failed to load database");

	conn->db->rows = malloc(conn->db->max_rows * sizeof(struct Address));
	int i =0;
	for(i = 0; i < conn->db->max_rows; i++){
		struct Address *addr = &conn->db->rows[i];
		addr->name = malloc(conn->db->max_data);
		if (!addr->name) die("Memory error.");
		addr->email = malloc(conn->db->max_data);
		if (!addr->email) die("Memory error.");
		rc = fread(&addr->id, sizeof(int), 1, conn->file);
		if (rc != 1) die("Failed to write to database");
      	rc = fread(&addr->set, sizeof(int), 1, conn->file);
      	if (rc != 1) die("Failed to write to database");
      	rc = fread(addr->name, conn->db->max_data, 1, conn->file);
      	if (rc != 1) die("Failed to write to database");
      	rc = fread(addr->email, conn->db->max_data, 1, conn->file);
      	if (rc != 1) die("Failed to write to database");
	}
}

void Database_open(const char *filename, char mode)
{
	conn = malloc(sizeof(struct Connection));
	if(!conn) die("Memory error");

	conn->db = malloc(sizeof(struct Database));
	if(!conn->db) die("Memory error");

	if (mode == 'c') {
		conn->file = fopen(filename, "w");
	} else {
		conn->file = fopen(filename, "r+");
		if (conn->file) {
			Database_load();
		}
	}

	if(!conn->file) die("Failed to open database file");
}

void Database_write()
{
	rewind(conn->file);
	int rc;

	rc = fwrite(&conn->db->max_data, sizeof(int), 1, conn->file);
	if (rc != 1) die("Failed to write to database");

	rc = fwrite(&conn->db->max_rows, sizeof(int), 1, conn->file);
	if (rc != 1) die("Failed to write to database");

	int i =0;
	for(i = 0; i < conn->db->max_rows; i++){
		struct Address *addr = &conn->db->rows[i];
		rc = fwrite(&addr->id, sizeof(int), 1, conn->file);
		if (rc != 1) die("Failed to write to database");
      	rc = fwrite(&addr->set, sizeof(int), 1, conn->file);
      	if (rc != 1) die("Failed to write to database");
      	rc = fwrite(addr->name, conn->db->max_data, 1, conn->file);
      	if (rc != 1) die("Failed to write to database");
      	rc = fwrite(addr->email, conn->db->max_data, 1, conn->file);
      	if (rc != 1) die("Failed to write to database");
	}

	rc = fflush(conn->file);
	if (rc == 1) die("Cannot flush database");
}

void Database_create()
{
	conn->db->rows = malloc(conn->db->max_rows * sizeof(struct Address));

	int i = 0;
	for(i = 0; i < conn->db->max_rows; i++){
		//initialize our prototype
		struct Address addr = {.id = i, .set = 0};
		addr.name = malloc(conn->db->max_data);
		addr.email = malloc(conn->db->max_data);
		//then assign it to the row
		conn->db->rows[i] = addr;
	}
}

void Database_set(int id, const char *name, const char *email)
{
	struct Address *addr = &conn->db->rows[id];
	if(addr->set) die("Already set, delete it first");

	addr->set = 1;
	char *res = strncpy(addr->name, name, conn->db->max_data);
	// Fix strncpy: terminate the string ALWAYS
	addr->name[conn->db->max_data - 1] = '\0';
	if (!res) die("Name copy failed");

	res = strncpy(addr->email, email, conn->db->max_data);
	addr->email[conn->db->max_data - 1] = '\0';
	if (!res) die("Email copy failed");
}

void Database_get(int id)
{
	struct Address *addr = &conn->db->rows[id];

	if (addr->set){
		Address_print(addr);
	} else {
		die ("ID is not set");
	}
}

void Database_delete(int id)
{
	struct Address addr = {.id = id, .set = 0};
	addr.name = malloc(conn->db->max_data);
	addr.email = malloc(conn->db->max_data);
	conn->db->rows[id] = addr;
}

void Database_list()
{
	int i = 0;
	struct Database *db = conn->db;

	for(i = 0; i < conn->db->max_rows; i++){
		struct Address *cur = &db->rows[i];

		if (cur->set) {
			Address_print(cur);
		}
	}
}

void Database_find(const char *term)
{
	int i =0;
	struct Database *db = conn->db;

	for(i = 0; i < conn->db->max_rows; i++){
		struct Address *cur = &db->rows[i];
		int result;
		result = strncmp(cur->name, term, conn->db->max_data);
		if (cur->set && result == 0) {
			Address_print(cur);
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]");

	char *filename = argv[1];
	char action = argv[2][0];
	Database_open(filename, action);
	int id = 0;

	if(argc > 3 && action != 'f' && action != 'c') {
		id = atoi(argv[3]);
		if(id >= conn->db->max_rows) die("There's not that many records.");
	}

	switch(action){
		case 'c':
			conn->db->max_data = (argc >= 3) ? atoi(argv[3]): 512;
      		conn->db->max_rows = (argc >= 4) ? atoi(argv[4]): 512;
			Database_create();
			Database_write();
			break;
		case 'g':
			if(argc != 4) die("Need an id to get a record");
			Database_get(id);
			break;
		case 's':
			if(argc != 6) die("Need an id, name, and email to set");
			Database_set(id, argv[4], argv[5]);
			Database_write();
			break;
		case 'd':
			if(argc != 4) die("Need an id to delete");
			Database_delete(id);
			Database_write();
			break;
		case 'l':
			Database_list();
			break;
		case 'f':
			Database_find(argv[3]);
			break;
		default:
			die("Invalid action, only: c=create, g=get, s=set, d=del, l=list, f=find");
	}

	Database_close();

	return 0;
}
