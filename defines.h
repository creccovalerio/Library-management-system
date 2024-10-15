#pragma once

#include <stdbool.h>
#include <mysql.h>

struct configuration {
	char *host;
	char *db_username;
	char *db_password;
	unsigned int port;
	char *database;

	char username[128];
	char password[128];
};

struct libro{
	int idLibro;
	char nome[46];
	char autore[46];
	char edizione[11];
};

struct copie_disponibili{
	int idCopia;
	int Ripiano;
	int Scaffale;
	char nome[45];
};

struct bibliotecari_disponibili{
	int idBibliotecario;
	char Nome[30];
	char Cognome[30];
};

struct contatti_utente{
	int CF;
	char nome[30];
	char cognome[30];
	char titolo_libro[45];
	char contatto_preferito[45];
};


struct copie_possedute_utente{
	int idCopia;
	char nome[45];
	char data_inizio[15];
	int periodo_cons;
	int idPrestito;
};


struct utente{
	int idUtente;
	char nome[30];
	char cognome[30];
	char data_nascita[12];
	char sesso[8];
	int idUtilizzatoreSist;
};



extern struct copie_disponibili *cp;
extern struct libro *lb;
extern struct bibliotecari_disponibili *bd;
extern struct contatti_utente *cu;
extern struct copie_possedute_utente *cpu;
extern struct configuration conf;
extern struct utente *ub;

extern int parse_config(char *path, struct configuration *conf);
extern char *getInput(unsigned int lung, char *stringa, bool hide);
extern bool yesOrNo(char *domanda, char yes, char no, bool predef, bool insensitive);
extern char multiChoice(char *domanda, char choices[], int num);
extern void print_error (MYSQL *conn, char *message);
extern void print_stmt_error (MYSQL_STMT *stmt, char *message);
extern void finish_with_error(MYSQL *conn, char *message);
extern void finish_with_stmt_error(MYSQL *conn, MYSQL_STMT *stmt, char *message, bool close_stmt);
extern bool setup_prepared_stmt(MYSQL_STMT **stmt, char *statement, MYSQL *conn);
extern void dump_result_set(MYSQL *conn, MYSQL_STMT *stmt, char *title);
extern void run_as_utente(MYSQL *conn, int idUtilizzatore_sistema);
extern void run_as_bibliotecario(MYSQL *conn, int idUtilizzatore_sistema);
extern void run_as_amministratore(MYSQL *conn, int idUtilizzatore_sistema);
