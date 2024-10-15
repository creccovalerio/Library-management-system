#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mysql.h>
#include "defines.h"

#define flush(stdin) while(getchar() != '\n')

struct copie_in_prestito{
	char data_inizio[11];
	int periodo_consultazione;
	int idCopia;
};

char buffer[64];
int idUtente_gl, idCopia_gl, Terminato_gl, Periodo_consultazione_gl;
int idBiblioteca, idBibliotecario, idBibliotecario_rp, idUtente_rp;
int id_utilizzatore_sistema;


int id_new_util, id_new_user;
struct copie_disponibili *cd;
struct contatti_utente *cu;
struct utente *ub;

static void reopen_connection(MYSQL *conn){

	mysql_close(conn);

	//Reopen connection
	if(!parse_config("users/login.json", &conf)){
		fprintf(stderr, "Unable to load login configuration\n");
		exit(EXIT_FAILURE);
	}

	conn = mysql_init (NULL);
	if(conn == NULL){
		fprintf(stderr, "mysql_init() failed (probably out of memory)\n");
		exit(EXIT_FAILURE);
	}

	if(mysql_real_connect(conn, conf.host, conf.db_username, conf.db_password, conf.database, conf.port, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS) == NULL) {
		fprintf(stderr, "mysql_real_connect() failed\n");
		mysql_close(conn);
		exit(EXIT_FAILURE);
	}


}



static void richiesta_di_malattia(MYSQL *conn){

	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[8];

	int numero, anno, ret, ret_val = 0;
	char giorno[15], mese[15], ora_inizio[10], ora_fine[10];

rescan_all1:

	printf("Inserisci infomazioni del turno da ricoprire:\n");

	printf("Inserisci ora inizio del turno: (formato: HH:MM:SS)\n");
	memset(ora_inizio, 0, 10);
	getInput(11, ora_inizio, false);

    printf("Inserisci ora fine del turno: (formato: HH:MM:SS)\n");
    memset(ora_fine, 0, 10);
	getInput(11, ora_fine, false);
	
rescan1:
	printf("Inserisci il numero del giorno: \n");
	ret = scanf("%d", &numero);
	flush(stdin);
	if(ret == 0){
		printf("input non valido\n");
		goto rescan1;
	}

	printf("Inserisci il giorno (es: lunedi, martedi..)\n");
	memset(giorno, 0, 15);
	getInput(16, giorno, false);

    
    printf("Inserisci il mese (es: gennaio, febbraio..)\n");
    memset(mese, 0, 15);
    getInput(16, mese, false);
    

 
rescan2:
	printf("Inserisci l'Anno: \n");
	ret = scanf("%d", &anno);
	flush(stdin);
	if(ret == 0){
		printf("input non valido\n");
		goto rescan2;
	}

	// Prepare prepared statement
	if(!setup_prepared_stmt(&prepared_stmt, "call add_richiesta_malattia(?,?,?,?,?,?,?,?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize richiesta_malattia insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id_utilizzatore_sistema;
	param[0].buffer_length = sizeof(id_utilizzatore_sistema);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = ora_inizio;
	param[1].buffer_length = strlen(ora_inizio);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = ora_fine;
	param[2].buffer_length = strlen(ora_fine);

	param[3].buffer_type = MYSQL_TYPE_LONG; 
	param[3].buffer = &numero;
	param[3].buffer_length = sizeof(numero);

	param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[4].buffer = giorno;
	param[4].buffer_length = strlen(giorno);

	param[5].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[5].buffer = mese;
	param[5].buffer_length = strlen(mese);

	param[6].buffer_type = MYSQL_TYPE_LONG;
	param[6].buffer = &anno;
	param[6].buffer_length = sizeof(anno);

	param[7].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[7].buffer = &ret_val;
	param[7].buffer_length = sizeof(ret_val);

	if(mysql_stmt_bind_param(prepared_stmt, param) != 0){
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for request inseriton\n", true);
	}

	// Run procedure
	if(mysql_stmt_execute(prepared_stmt) != 0){
		print_stmt_error(prepared_stmt, "An error occurred while adding the request");
		goto out;
	}

	// Get back the ID of the newly_added user
	memset(param, 0, sizeof(param));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &ret_val;
	param[0].buffer_length = sizeof(ret_val);

	if(mysql_stmt_bind_result(prepared_stmt, param)){
		finish_with_stmt_error(conn, prepared_stmt, "Could not retrieve output parameter", true);
	}

	// Retrieve output parameter
	if(mysql_stmt_fetch(prepared_stmt)){
		finish_with_stmt_error(conn, prepared_stmt, "Could not buffer the results", true);
	}

	
	if(ret_val == 0){
		system("clear");
		printf("Turno non trovato.. reinserire le info del turno\n");
		reopen_connection(conn);
		goto rescan_all1;
	}

	printf("**** Richiesta di malattia correttamente inviata ****\n");
	
	out:
	mysql_stmt_close(prepared_stmt);
	
}


static void add_new_user(MYSQL *conn){

	MYSQL_STMT *prepared_stmt1;
	MYSQL_BIND param1[13];

	// Input for the registration in the table Utilizzatori sistema
	char username[45];
	char password[30];
	char ruolo[7];

	// Input for the registration in the table Utenti
	char CF[17];
	char name[31];
	char surname[31];
	char date_birth[11];
	char gender[6];

	// Input for the registration in the table Recapiti
	char telefono[15];
    char cellulare[15];
    char email[45];
    int pref_recap;
    int ret;
	
	printf("\n**** PROCEDURA DI CREAZIONE DI UN NUOVO UTILIZZATORE ****\nInserire le informazioni dell'utente:\n");
	printf("premere invio per iniziare\n");
	flush(stdin);

	// Get the required information
	printf("\nUser username: ");
	getInput(46, username, false);
	printf("\nUser password: ");
	getInput(31, password, true);
	strcpy(ruolo, "utente");

	printf("\nUser CF: ");
	getInput(17, CF, false);
	printf("\nNome: ");
	getInput(31, name, false);
	printf("\nCognome: ");
	getInput(31, surname, false);
	printf("\nData di nascita (format YYYY-MM-DD): ");
	getInput(11, date_birth, false);
	printf("\nSesso (format M o F): ");
	getInput(6, gender, false);

	printf("\nInserire almeno un contatto tra telefono, cellulare ed email: (inserire null se un contatto tra questi non vuole essere specificato)\n");
	
rescan_contacts:
	printf("\nUser telefono: ");
	memset(telefono, 0, 15);
	getInput(16, telefono, false);
	printf("\nUser cellulare: ");
	memset(cellulare, 0, 15);
	getInput(16, cellulare, false);
	printf("\nUser email: ");
	memset(email, 0, 45);
	getInput(46, email, false);

	if((strcmp(telefono, "null") == 0) || (strcmp(telefono, "NULL") == 0) || (strcmp(telefono, "") == 0)){
		if((strcmp(cellulare, "null") == 0) || (strcmp(cellulare, "NULL") == 0) || (strcmp(cellulare, "") == 0)){
			if((strcmp(email, "null") == 0) || (strcmp(email, "NULL") == 0) || (strcmp(email, "") == 0)){
				printf("Specificare almeno un contatto..\n");
				goto rescan_contacts;
			}
		}
	}

	// Selezione del recapito preferito

rechoice:
  	printf("Specifica quale dei recapiti vuoi che sia il preferito: \n");
  	printf("Inserisci: \n 1) se vuoi che sia il telefono\n 2) se vuoi che sia il cellulare\n 3) se vuoi che sia l'email\n");

  	ret = scanf("%d", &pref_recap);
  	if(ret == 0){
  		printf("input non valido\n");
  		goto rechoice;
  	}

  	if(pref_recap != 1 && pref_recap != 2 && pref_recap != 3){
  		printf("input non valido\n");
  		goto rechoice;
  	}

  	if(pref_recap == 1){
  		if((strcmp(telefono, "null") == 0) || (strcmp(telefono, "NULL") == 0) || (strcmp(telefono, "") == 0)){
			printf("recapito scelto come preferito non valido\n");
			goto rechoice;
		}
  	}else{
  		if(pref_recap == 2){
  			if((strcmp(cellulare, "null") == 0) || (strcmp(cellulare, "NULL") == 0) || (strcmp(cellulare, "") == 0)){
				printf("recapito scelto come preferito non valido\n");
				goto rechoice;
			}
  		}else{
  			if(pref_recap == 3){
  				if((strcmp(email, "null") == 0) || (strcmp(email, "NULL") == 0) || (strcmp(email, "") == 0)){
					printf("recapito scelto come preferito non valido\n");
					goto rechoice;
				}
  			}
  		}
  	}

	// Prepare prepared statement
	if(!setup_prepared_stmt(&prepared_stmt1, "call add_user_proc_pppb(?,?,?,?,?,?,?,?,?,?,?,?,?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt1, "Unable to initialize utilizzatore_sistema insertion statement\n", false);
	}

	// Prepare parameters
	memset(param1, 0, sizeof(param1));

	param1[0].buffer_type = MYSQL_TYPE_VAR_STRING; // IN
	param1[0].buffer = username;
	param1[0].buffer_length = strlen(username);

	param1[1].buffer_type = MYSQL_TYPE_VAR_STRING; // IN
	param1[1].buffer = password;
	param1[1].buffer_length = strlen(password);

	param1[2].buffer_type = MYSQL_TYPE_VAR_STRING; // IN
	param1[2].buffer = ruolo;
	param1[2].buffer_length = strlen(ruolo);

	param1[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param1[3].buffer = CF;
	param1[3].buffer_length = strlen(CF);

	param1[4].buffer_type = MYSQL_TYPE_VAR_STRING; // IN
	param1[4].buffer = name;
	param1[4].buffer_length = strlen(name);

	param1[5].buffer_type = MYSQL_TYPE_VAR_STRING; // IN
	param1[5].buffer = surname;
	param1[5].buffer_length = strlen(surname);

	param1[6].buffer_type = MYSQL_TYPE_VAR_STRING; // IN
	param1[6].buffer = date_birth;
	param1[6].buffer_length = strlen(date_birth);

	param1[7].buffer_type = MYSQL_TYPE_VAR_STRING; // IN
	param1[7].buffer = gender;
	param1[7].buffer_length = strlen(gender);

	param1[8].buffer_type = MYSQL_TYPE_VAR_STRING; // IN
	param1[8].buffer = telefono;
	param1[8].buffer_length = strlen(telefono);

	param1[9].buffer_type = MYSQL_TYPE_VAR_STRING; // IN
	param1[9].buffer = cellulare;
	param1[9].buffer_length = strlen(cellulare);

	param1[10].buffer_type = MYSQL_TYPE_VAR_STRING; // IN
	param1[10].buffer = email;
	param1[10].buffer_length = strlen(email);

	param1[11].buffer_type = MYSQL_TYPE_LONG; // IN
	param1[11].buffer = &pref_recap;
	param1[11].buffer_length = sizeof(pref_recap);

	param1[12].buffer_type = MYSQL_TYPE_LONG; // IN
	param1[12].buffer = &id_utilizzatore_sistema;
	param1[12].buffer_length = sizeof(id_utilizzatore_sistema);


	if(mysql_stmt_bind_param(prepared_stmt1, param1) != 0){
		finish_with_stmt_error(conn, prepared_stmt1, "Could not bind parameters for user inseriton\n", true);
	}

	// Run procedure
	if(mysql_stmt_execute(prepared_stmt1) != 0){
		print_stmt_error(prepared_stmt1, "An error occurred while adding the user");
	}

	mysql_stmt_close(prepared_stmt1);

	printf("\n**** Utente iscritto e recapiti correttamente inseriti ****\n");

}

/*
static int add_new_user(MYSQL *conn){

	MYSQL_STMT *prepared_stmt1;
	MYSQL_BIND param1[4];

	// Input for the registration in the table Utilizzatori sistema
	char username[45];
	char password[30];
	char ruolo[7];
	int id_utente, ret;

	printf("**** PROCEDURA DI CREAZIONE DI UN NUOVO UTILIZZATORE ****\nInserire username e password dell'utente:\n");
	printf("premere invio per iniziare\n");
	flush(stdin);
	
	// Get the required information
	printf("\nUser username: ");
	getInput(46, username, false);
	printf("\nUser password: ");
	getInput(31, password, true);
	strcpy(ruolo, "utente");


	// Prepare prepared statement
	if(!setup_prepared_stmt(&prepared_stmt1, "call aggiungi_utilizzatore_sistema_proc(?,?,?,?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt1, "Unable to initialize utente_biblioteca insertion statement\n", false);
	}

	// Prepare parameters
	memset(param1, 0, sizeof(param1));

	param1[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param1[0].buffer = username;
	param1[0].buffer_length = strlen(username);

	param1[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param1[1].buffer = password;
	param1[1].buffer_length = strlen(password);

	param1[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param1[2].buffer = ruolo;
	param1[2].buffer_length = strlen(ruolo);

	param1[3].buffer_type = MYSQL_TYPE_LONG; // OUT
	param1[3].buffer = &id_utente;
	param1[3].buffer_length = sizeof(id_utente);

	if(mysql_stmt_bind_param(prepared_stmt1, param1) != 0){
		finish_with_stmt_error(conn, prepared_stmt1, "Could not bind parameters for user inseriton\n", true);
	}

	// Run procedure
	if(mysql_stmt_execute(prepared_stmt1) != 0){
		print_stmt_error(prepared_stmt1, "An error occurred while adding the user");
		ret = 1;
		goto out;
	}

	// Get back the ID of the newly_added user
	memset(param1, 0, sizeof(param1));
	param1[0].buffer_type = MYSQL_TYPE_LONG;
	param1[0].buffer = &id_utente;
	param1[0].buffer_length = sizeof(id_utente);

	if(mysql_stmt_bind_result(prepared_stmt1, param1)){
		finish_with_stmt_error(conn, prepared_stmt1, "Could not retrieve output parameter", true);
	}

	// Retrieve output parameter
	if(mysql_stmt_fetch(prepared_stmt1)){
		finish_with_stmt_error(conn, prepared_stmt1, "Could not buffer the results", true);
	}

	id_new_util = id_utente;
	printf(" **** Utente correttamente salvato tra gli Utilizzatori sistema con ID: %d ****\n", id_new_util);
	
	out:
	mysql_stmt_close(prepared_stmt1);
	return ret;

}



static void add_new_user_bis(MYSQL *conn){

	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[6];


	// Input for the registration routine
	char name[31];
	char surname[31];
	char date_birth[11];
	char sex[6];
	int id;


	printf("**** PROCEDURA DI REGISTRAZIONE DI UN NUOVO UTENTE DEL SISTEMA ****\nInserire generalità dell'utente da iscrivere\n");

	// Get the required information	
	printf("Premi invio per iniziare\n");

retry7:

	flush(stdin);
	printf("\nNome: ");
	getInput(31, name, false);
	printf("\nCognome: ");
	getInput(31, surname, false);
	printf("\nData di nascita (format YYYY-MM-DD): ");
	getInput(11, date_birth, false);
	printf("\nSesso (M o F): ");
	getInput(6, sex, false);
	

	// Prepare prepared statement
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_user_proc(?,?,?,?,?,?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = name;
	param[0].buffer_length = strlen(name);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = surname;
	param[1].buffer_length = strlen(surname);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = date_birth;
	param[2].buffer_length = strlen(date_birth);

	param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = sex;
	param[3].buffer_length = strlen(sex);

	param[4].buffer_type = MYSQL_TYPE_LONG;
	param[4].buffer = &id_new_util;
	param[4].buffer_length = sizeof(id_new_util);

	param[5].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[5].buffer = &id;
	param[5].buffer_length = sizeof(id);

	if(mysql_stmt_bind_param(prepared_stmt, param) != 0){
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user inseriton\n", true);
	}

	// Run procedure
	if(mysql_stmt_execute(prepared_stmt) != 0){
		print_stmt_error(prepared_stmt, "An error occurred while adding the user");
		goto retry7;
	}

	// Get back the ID of the newly_added user
	memset(param, 0, sizeof(param));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id;
	param[0].buffer_length = sizeof(id);

	if(mysql_stmt_bind_result(prepared_stmt, param)){
		finish_with_stmt_error(conn, prepared_stmt, "Could not retrieve output parameter", true);
	}

	// Retrieve output parameter
	if(mysql_stmt_fetch(prepared_stmt)){
		finish_with_stmt_error(conn, prepared_stmt, "Could not buffer the results", true);

	}

	id_new_user = id;
	printf("\n**** Utente correttamente inserito con ID utente ---> %d ****\n", id_new_user);

	mysql_stmt_close(prepared_stmt);
}


static void add_recapiti(MYSQL *conn){

	
	MYSQL_STMT *prepared_stmt, *prepared_stmt1;
	MYSQL_BIND param[4], param1[2];
	int ret, pref_recap;
	char telefono[15], cellulare[15], email[45];

	printf("Inserire almeno un contatto tra telefono, cellulare ed email: (inserire null se un contatto tra questi non vuole essere specificato)\n");
	

rescan5:
	printf("\nUser telefono: ");
	memset(telefono, 0, 15);
	getInput(16, telefono, false);
	printf("\nUser cellulare: ");
	memset(cellulare, 0, 15);
	getInput(16, cellulare, false);
	printf("\nUser email: ");
	memset(email, 0, 45);
	getInput(46, email, false);

	if((strcmp(telefono, "null") == 0) || (strcmp(telefono, "NULL") == 0) || (strcmp(telefono, "") == 0)){
		if((strcmp(cellulare, "null") == 0) || (strcmp(cellulare, "NULL") == 0) || (strcmp(cellulare, "") == 0)){
			if((strcmp(email, "null") == 0) || (strcmp(email, "NULL") == 0) || (strcmp(email, "") == 0)){
				printf("Specificare almeno un contatto..\n");
				goto rescan5;
			}
		}
	}

	// Prepare prepared statement
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_recapiti_proc(?,?,?,?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize aggiungi_recapiti statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = telefono;
	param[0].buffer_length = strlen(telefono);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = cellulare;
	param[1].buffer_length = strlen(cellulare);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = email;
	param[2].buffer_length = strlen(email);

	param[3].buffer_type = MYSQL_TYPE_LONG;
	param[3].buffer = &id_new_user;
	param[3].buffer_length = sizeof(id_new_user);

	

	if(mysql_stmt_bind_param(prepared_stmt, param) != 0){
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for aggiungi_recapiti\n", true);
	}

	// Run procedure
	if(mysql_stmt_execute(prepared_stmt) != 0){
		print_stmt_error(prepared_stmt, "An error occurred while aggiungi_recapiti");
		goto out;
	}

out:

	mysql_stmt_close(prepared_stmt);

	/////////////////////////////////////////////////////////////////////////////////////
	// Selezione del recapito preferito

rechoice:
  	printf("Specifica quale dei recapiti vuoi che sia il preferito: \n");
  	printf("Inserisci: \n 1) se vuoi che sia il telefono\n 2) se vuoi che sia il cellulare\n 3) se vuoi che sia l'email\n");

  	ret = scanf("%d", &pref_recap);
  	if(ret == 0){
  		printf("input non valido\n");
  		goto rechoice;
  	}

  	if(pref_recap != 1 && pref_recap != 2 && pref_recap != 3){
  		printf("input non valido\n");
  		goto rechoice;
  	}

  	if(pref_recap == 1){
  		if((strcmp(telefono, "null") == 0) || (strcmp(telefono, "NULL") == 0) || (strcmp(telefono, "") == 0)){
			printf("recapito scelto come preferito non valido\n");
			goto rechoice;
		}
  	}else{
  		if(pref_recap == 2){
  			if((strcmp(cellulare, "null") == 0) || (strcmp(cellulare, "NULL") == 0) || (strcmp(cellulare, "") == 0)){
				printf("recapito scelto come preferito non valido\n");
				goto rechoice;
			}
  		}else{
  			if(pref_recap == 3){
  				if((strcmp(email, "null") == 0) || (strcmp(email, "NULL") == 0) || (strcmp(email, "") == 0)){
					printf("recapito scelto come preferito non valido\n");
					goto rechoice;
				}
  			}
  		}
  	}

  	// Prepare prepared statement
	if(!setup_prepared_stmt(&prepared_stmt1, "call aggiungi_recapito_preferito_proc(?,?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt1, "Unable to initialize aggiungi_recapito_pref statement\n", false);
	}

	// Prepare parameters
	memset(param1, 0, sizeof(param1));

	param1[0].buffer_type = MYSQL_TYPE_LONG;
	param1[0].buffer = &pref_recap;
	param1[0].buffer_length = sizeof(pref_recap);

	param1[1].buffer_type = MYSQL_TYPE_LONG;
	param1[1].buffer = &id_new_user;
	param1[1].buffer_length = sizeof(id_new_user);

	
	if(mysql_stmt_bind_param(prepared_stmt1, param1) != 0){
		finish_with_stmt_error(conn, prepared_stmt1, "Could not bind parameters for aggiungi_recapito_pref\n", true);
	}

	// Run procedure
	if(mysql_stmt_execute(prepared_stmt1) != 0){
		print_stmt_error(prepared_stmt1, "An error occurred while adding recapito preferito");
		goto out1;
	}

	
out1:
	mysql_stmt_close(prepared_stmt1);


	printf("**** Recapiti e RecapitoPreferito correttamente inseriti ****\n");

}


static int subscribe_user_to_a_library(MYSQL *conn){

  	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[2];

	// Prepare prepared statement
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_iscritto_proc(?,?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id_utilizzatore_sistema;
	param[0].buffer_length = sizeof(id_utilizzatore_sistema);

	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &id_new_user;
	param[1].buffer_length = sizeof(id_new_user);

	
	if(mysql_stmt_bind_param(prepared_stmt, param) != 0){
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for iscritto inseriton\n", true);
	}

	// Run procedure
	if(mysql_stmt_execute(prepared_stmt) != 0){
		print_stmt_error(prepared_stmt, "An error occurred while adding the iscritto");
		goto out;
	}

	idUtente_gl = id_new_user;
	printf("**** Utente - %d - registrato correttamente nella biblioteca! **** \n", id_new_user);

out:
	mysql_stmt_close(prepared_stmt);
	return idUtente_gl;

}
*/

static void add_new_prestito(MYSQL *conn){

		
	MYSQL_STMT *prepared_stmt2;
	MYSQL_BIND param2[5];
	int idPrestito;

	printf("ID UTENTE: %d - ID COPIA: %d - Terminato: %d - PC: %d \n", idUtente_gl, idCopia_gl, Terminato_gl, Periodo_consultazione_gl);
	//char data_prestito[10];
/*		
reinsert2:
	printf("\nInserisci data del prestito: (formato YYYY:MM:DD)\n");
	getInput(11, data_prestito, false);
*/
	// Prepare prepared statement
	if(!setup_prepared_stmt(&prepared_stmt2, "call aggiungi_prestito_proc(?,?,?,?,?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt2, "Unable to initialize statement\n", false);
	}

	// Prepare parameters
	memset(param2, 0, sizeof(param2));

	param2[0].buffer_type = MYSQL_TYPE_LONG;
	param2[0].buffer = &idUtente_gl;
	param2[0].buffer_length = sizeof(idUtente_gl);

	param2[1].buffer_type = MYSQL_TYPE_LONG;
	param2[1].buffer = &idCopia_gl;
	param2[1].buffer_length = sizeof(idCopia_gl);

	param2[2].buffer_type = MYSQL_TYPE_LONG;
	param2[2].buffer = &Terminato_gl;
	param2[2].buffer_length = sizeof(Terminato_gl);

	param2[3].buffer_type = MYSQL_TYPE_LONG;
	param2[3].buffer = &Periodo_consultazione_gl;
	param2[3].buffer_length = sizeof(Periodo_consultazione_gl);

	param2[4].buffer_type = MYSQL_TYPE_LONG; // OUT
	param2[4].buffer = &idPrestito;
	param2[4].buffer_length = sizeof(idPrestito);


	if(mysql_stmt_bind_param(prepared_stmt2, param2) != 0){
		finish_with_stmt_error(conn, prepared_stmt2, "Could not bind parameters for loan inseriton\n", true);
	}

	// Run procedure
	if(mysql_stmt_execute(prepared_stmt2) != 0){
		print_stmt_error(prepared_stmt2, "An error occurred while adding the loan");
		goto out;
	}

	// Get back the ID of the newly_added loan
	memset(param2, 0, sizeof(param2));
	param2[0].buffer_type = MYSQL_TYPE_LONG;
	param2[0].buffer = &idPrestito;
	param2[0].buffer_length = sizeof(idPrestito);

	if(mysql_stmt_bind_result(prepared_stmt2, param2)){
		finish_with_stmt_error(conn, prepared_stmt2, "Could not retrieve output parameter", true);
	}

	// Retrieve output parameter
	if(mysql_stmt_fetch(prepared_stmt2)){
		finish_with_stmt_error(conn, prepared_stmt2, "Could not buffer the results", true);

	}

	printf("\n**** Prestito correttamente salvato nella tabella PRESTITO con ID: %d ****\n", idPrestito);

out:
	mysql_stmt_close(prepared_stmt2);

}





static void disponibilita_copie(MYSQL *conn){

	char q[1024];
	int ret, idCopia, idBibliotecaPartenza;
	int pConsultazione, idUtente, n_res, ripiano, scaffale;
	char risp;

	memset(q, 0, 1024);
	memset(buffer,0, 45);

	
	printf("\nInserisci nome del libro richiesto: ");
	

    while(true)
    {
        assert(fgets(buffer, 45, stdin) != NULL);
        // Verifies that the previous character to the last character in the
        // buffer array is '\n' (The last character is '\0') if the
        // character is '\n' leaves loop.
        if(buffer[strlen(buffer) - 1] == '\n')
        {
            // fgets reads and adds '\n' in the string, replace '\n' by '\0' to 
            // remove the line break .
            buffer[strlen(buffer) - 1] = '\0';
            printf("Libro : %s\n", buffer);
            break;
        }
        printf("Libro : %s\n", buffer);  
    }

  	///////////////////////////////////////////////////////////////////////////////////////////////////

    MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[3];

	int var_ret, idBiblioteca;


	// Input for the registration routine
	
	// Prepare prepared statement
	if(!setup_prepared_stmt(&prepared_stmt, "call recupera_disponibilita_copie_proc(?,?,?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize recupera_disponibilita_copie_proc statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id_utilizzatore_sistema;
	param[0].buffer_length = sizeof(id_utilizzatore_sistema);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = buffer;
	param[1].buffer_length = strlen(buffer);

	param[2].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[2].buffer = &var_ret;
	param[2].buffer_length = sizeof(var_ret);

	if(mysql_stmt_bind_param(prepared_stmt, param) != 0){
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for recupera_disponibilita_copie_proc\n", true);
	}

	// Run procedure
	if(mysql_stmt_execute(prepared_stmt) != 0){
		print_stmt_error(prepared_stmt, "An error occurred while recupera_disponibilita_copie_proc");
		goto out;
	}

	// Get back the ID of the newly_added user
	memset(param, 0, sizeof(param));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &var_ret;
	param[0].buffer_length = sizeof(var_ret);

	if(mysql_stmt_bind_result(prepared_stmt, param)){
		finish_with_stmt_error(conn, prepared_stmt, "Could not retrieve output parameter", true);
	}

	// Retrieve output parameter
	if(mysql_stmt_fetch(prepared_stmt)){
		finish_with_stmt_error(conn, prepared_stmt, "Could not buffer the results", true);

	}

out:
	mysql_stmt_close(prepared_stmt);

  	///////////////////////////////////////////////////////////////////////////////////////////////////

	printf("Disponibilità copie: %d\n", var_ret);
  	if(var_ret == 0){
  		
  		printf("Nessuna copia del libro richiesto è disponibile nella nostra biblioteca.. Verificare la presenza in un'altra biblioteca del circuito?");
  		
  		MYSQL_STMT *prepared_stmt3, *prepare_meta_result3;
		MYSQL_BIND param3[3];
		int column_count3;
		bool is_null3[3];
		bool error3[3];
		int status3;
		unsigned long length3[3];
		
		// Prepare stored procedure call
		if(!setup_prepared_stmt(&prepared_stmt3, "call copie_trasferibili_proc(?,?)", conn)){
			finish_with_stmt_error(conn, prepared_stmt3, "Unable to initializa copie_trasferibili_proc statement\n", false);
		}
	
		// Prepare parameters
		memset(param3, 0, sizeof(param3));

		param3[0].buffer_type = MYSQL_TYPE_VAR_STRING;
		param3[0].buffer = buffer;
		param3[0].buffer_length = strlen(buffer);

		param3[1].buffer_type = MYSQL_TYPE_LONG;
		param3[1].buffer = &idBiblioteca;
		param3[1].buffer_length = sizeof(idBiblioteca);


		if(mysql_stmt_bind_param(prepared_stmt3, param3) != 0){
			finish_with_stmt_error(conn, prepared_stmt3, "Could not bind parameters for copie_trasferibili_proc\n", true);
		}

		// Run procedure
		if(mysql_stmt_execute(prepared_stmt3) != 0){
			print_stmt_error(prepared_stmt3, "An error occurred while retrieving the copie_trasferibili_proc\n");
			return;
		}

		/* Fetch result set meta information */
		prepare_meta_result3 = mysql_stmt_result_metadata(prepared_stmt3);
		if (!prepare_meta_result3)
		{
  			fprintf(stderr," mysql_stmt_result_metadata(), returned no meta information\n");
  			fprintf(stderr, " %s\n", mysql_stmt_error(prepared_stmt3));
  			exit(0);
		}
		
		/* Get total columns in the query */
		column_count3= mysql_num_fields(prepare_meta_result3);
		fprintf(stdout, " total columns in SELECT statement: %d\n", column_count3);

		if (column_count3 != 2) /* validate column count */
		{
  			fprintf(stderr, " invalid column count returned by MySQL\n");
  			exit(0);
		}
	
		memset(param3, 0, sizeof(param3));
		
		param3[0].buffer_type = MYSQL_TYPE_LONG;
		param3[0].buffer = &idBiblioteca;
		param3[0].buffer_length = sizeof(idBiblioteca);
		param3[0].is_null = &is_null3[2];
		param3[0].length = &length3[2];
		param3[0].error = &error3[2];
		
		param3[1].buffer_type = MYSQL_TYPE_LONG;
		param3[1].buffer = &idCopia;
		param3[1].buffer_length = sizeof(idCopia);
		param3[1].is_null = &is_null3[0];
		param3[1].length = &length3[0];
		param3[1].error = &error3[0];


		dump_result_set(conn, prepared_stmt3, "**** Copie trasferibili ****");
	
		mysql_stmt_close(prepared_stmt3);
		mysql_close(conn);

		if(!parse_config("users/login.json", &conf)){
			fprintf(stderr, "Unable to load login configuration\n");
			exit(EXIT_FAILURE);
		}

		conn = mysql_init (NULL);
		if(conn == NULL){
			fprintf(stderr, "mysql_init() failed (probably out of memory)\n");
			exit(EXIT_FAILURE);
		}

		if(mysql_real_connect(conn, conf.host, conf.db_username, conf.db_password, conf.database, conf.port, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS) == NULL) {
			fprintf(stderr, "mysql_real_connect() failed\n");
			mysql_close(conn);
			exit(EXIT_FAILURE);
		}


  		  	
  		
rescan6:
		printf("Seleziona l'id della biblioteca da cui richiedere il libro: \n");
		ret = scanf("%d", &idBibliotecaPartenza);
		flush(stdin);
		if(ret == 0){
			printf("input non valido\n");
			goto rescan6;
		}

rescan7:
		printf("Seleziona l'id della copia da trasferire: \n");
		ret = scanf("%d", &idCopia);
		flush(stdin);
		if(ret == 0){
			printf("input non valido\n");
			goto rescan7;
		}

		
		MYSQL_STMT *prepared_stmt4;
		MYSQL_BIND param4[3];
		
		// Prepare prepared statement
		if(!setup_prepared_stmt(&prepared_stmt4, "call update_copie_proc(?,?,?)", conn)){
			finish_with_stmt_error(conn, prepared_stmt4, "Unable to initialize statement\n", false);
		}

		// Prepare parameters
		memset(param4, 0, sizeof(param4));

		param4[0].buffer_type = MYSQL_TYPE_LONG;
		param4[0].buffer = &idCopia;
		param4[0].buffer_length = sizeof(idCopia);

		param4[1].buffer_type = MYSQL_TYPE_LONG;
		param4[1].buffer = &idBibliotecaPartenza;
		param4[1].buffer_length = sizeof(idBibliotecaPartenza);

		param4[2].buffer_type = MYSQL_TYPE_LONG;
		param4[2].buffer = &id_utilizzatore_sistema;
		param4[2].buffer_length = sizeof(id_utilizzatore_sistema);


	
		if(mysql_stmt_bind_param(prepared_stmt4, param4) != 0){
			finish_with_stmt_error(conn, prepared_stmt4, "Could not bind parameters for transfer inseriton\n", true);
		}

		// Run procedure
		if(mysql_stmt_execute(prepared_stmt4) != 0){
			print_stmt_error(prepared_stmt4, "An error occurred while adding the transfer");
			goto out4;
		}


		printf("\n**** trasferimento correttamente concluso ****\n");

out4:
		mysql_stmt_close(prepared_stmt4);
		
  	

  	}else{

  		// copie disponibili trovate
		MYSQL_STMT *prepared_stmt1, *prepare_meta_result;
		MYSQL_BIND param1[3];
		int column_count;
		bool is_null[3];
		bool error[3];
		
		unsigned long length[3];

		
		// Prepare stored procedure call
		if(!setup_prepared_stmt(&prepared_stmt1, "call mostra_copie_disponibili_proc(?,?)", conn)){
			finish_with_stmt_error(conn, prepared_stmt1, "Unable to initializa mostra_copie_disponibili_proc statement\n", false);
		}

		// Prepare parameters
		memset(param1, 0, sizeof(param1));

		param1[0].buffer_type = MYSQL_TYPE_LONG;
		param1[0].buffer = &id_utilizzatore_sistema;
		param1[0].buffer_length = sizeof(id_utilizzatore_sistema);

		param1[1].buffer_type = MYSQL_TYPE_VAR_STRING;
		param1[1].buffer = buffer;
		param1[1].buffer_length = strlen(buffer);


		if(mysql_stmt_bind_param(prepared_stmt1, param1) != 0){
			finish_with_stmt_error(conn, prepared_stmt1, "Could not bind parameters for mostra_copie_disponibili_proc\n", true);
		}

		// Run procedure
		if(mysql_stmt_execute(prepared_stmt1) != 0){
			print_stmt_error(prepared_stmt1, "An error occurred while retrieving the mostra_copie_disponibili_proc\n");
			return;
		}

		/* Fetch result set meta information */
		prepare_meta_result = mysql_stmt_result_metadata(prepared_stmt1);
		if (!prepare_meta_result)
		{
  			fprintf(stderr," mysql_stmt_result_metadata(), returned no meta information\n");
  			fprintf(stderr, " %s\n", mysql_stmt_error(prepared_stmt1));
  			exit(0);
		}
		
		/* Get total columns in the query */
		column_count= mysql_num_fields(prepare_meta_result);
		fprintf(stdout, " total columns in SELECT statement: %d\n", column_count);

		if (column_count != 3) /* validate column count */
		{
  			fprintf(stderr, " invalid column count returned by MySQL\n");
  			exit(0);
		}
	
		memset(param1, 0, sizeof(param1));
	
		param1[0].buffer_type = MYSQL_TYPE_LONG;
		param1[0].buffer = &idCopia;
		param1[0].buffer_length = sizeof(idCopia);
		param1[0].is_null = &is_null[0];
		param1[0].length = &length[0];
		param1[0].error = &error[0];

		param1[1].buffer_type = MYSQL_TYPE_LONG;
		param1[1].buffer = &ripiano;
		param1[1].buffer_length = sizeof(ripiano);
		param1[1].is_null = &is_null[1];
		param1[1].length = &length[1];
		param1[1].error = &error[1];

		param1[2].buffer_type = MYSQL_TYPE_LONG;
		param1[2].buffer = &scaffale;
		param1[2].buffer_length = sizeof(scaffale);
		param1[2].is_null = &is_null[2];
		param1[2].length = &length[2];
		param1[2].error = &error[2];


		dump_result_set(conn, prepared_stmt1, "**** Copie disponibili ****");
	
		mysql_stmt_close(prepared_stmt1);
		mysql_close(conn);

		if(!parse_config("users/login.json", &conf)){
			fprintf(stderr, "Unable to load login configuration\n");
			exit(EXIT_FAILURE);
		}

		conn = mysql_init (NULL);
		if(conn == NULL){
			fprintf(stderr, "mysql_init() failed (probably out of memory)\n");
			exit(EXIT_FAILURE);
		}

		if(mysql_real_connect(conn, conf.host, conf.db_username, conf.db_password, conf.database, conf.port, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS) == NULL) {
			fprintf(stderr, "mysql_real_connect() failed\n");
			mysql_close(conn);
			exit(EXIT_FAILURE);
		}

rescan9:
		printf("Selezionare ID della copia da consegnare all'utente: \n");
		ret = scanf("%d", &idCopia);
		flush(stdin);
		if(ret == 0){
			printf("input non valido");
			goto rescan9;
		}

  		printf("Inserire dati dell'utente per la procedura di creazione del prestito\n");

rescan10:
  		printf("L'utente è già registrato nella nostra biblioteca? (y/n)\n");
  		ret = scanf("%c", &risp);
  		if(ret == 0){
  			printf("input non valido\n");
  			goto rescan10;
  		}

  		if(risp != 'y' && risp != 'n' && risp != 'Y' && risp != 'N'){
  			printf("input non valido\n");
  			goto rescan10;
  		}else{
  			if(risp == 'y' || risp == 'Y'){
  				
  				MYSQL_STMT *prepared_stmt2, *prepare_meta_result2;
				MYSQL_BIND param2[3];
				int column_count2;
				bool is_null2[3];
				bool error2[3];
				
				unsigned long length2[3];

				char nome[30], cognome[30];
				int idUtente;

				
				// Prepare stored procedure call
				if(!setup_prepared_stmt(&prepared_stmt2, "call mostra_utenti_biblioteca_proc(?)", conn)){
					finish_with_stmt_error(conn, prepared_stmt2, "Unable to initializa mostra_copie_disponibili_proc statement\n", false);
				}

				
				// Prepare parameters
				memset(param2, 0, sizeof(param2));

				param2[0].buffer_type = MYSQL_TYPE_LONG;
				param2[0].buffer = &id_utilizzatore_sistema;
				param2[0].buffer_length = sizeof(id_utilizzatore_sistema);

		
				if(mysql_stmt_bind_param(prepared_stmt2, param2) != 0){
					finish_with_stmt_error(conn, prepared_stmt2, "Could not bind parameters for mostra_copie_disponibili_proc\n", true);
				}

				
				// Run procedure
				if(mysql_stmt_execute(prepared_stmt2) != 0){
					print_stmt_error(prepared_stmt2, "An error occurred while retrieving the mostra_copie_disponibili_proc\n");
					return;
				}

				/* Fetch result set meta information */
				prepare_meta_result2 = mysql_stmt_result_metadata(prepared_stmt2);
				if (!prepare_meta_result2)
				{
  					fprintf(stderr," mysql_stmt_result_metadata(), returned no meta information\n");
  					fprintf(stderr, " %s\n", mysql_stmt_error(prepared_stmt2));
  					exit(0);
				}
			
				
				/* Get total columns in the query */
				column_count2 = mysql_num_fields(prepare_meta_result2);
				fprintf(stdout, " total columns in SELECT statement: %d\n", column_count2);

				if (column_count2 != 3) /* validate column count */
				{
  					fprintf(stderr, " invalid column count returned by MySQL\n");
  					exit(0);
				}
	
				memset(param2, 0, sizeof(param2));
	
				param2[0].buffer_type = MYSQL_TYPE_LONG;
				param2[0].buffer = &idUtente;
				param2[0].buffer_length = sizeof(idUtente);
				param2[0].is_null = &is_null2[0];
				param2[0].length = &length2[0];
				param2[0].error = &error2[0];

				param2[1].buffer_type = MYSQL_TYPE_VAR_STRING;
				param2[1].buffer = nome;
				param2[1].buffer_length = strlen(nome);
				param2[1].is_null = &is_null2[1];
				param2[1].length = &length2[1];
				param2[1].error = &error2[1];

				param2[2].buffer_type = MYSQL_TYPE_VAR_STRING;
				param2[2].buffer = cognome;
				param2[2].buffer_length = strlen(cognome);
				param2[2].is_null = &is_null2[2];
				param2[2].length = &length2[2];
				param2[2].error = &error2[2];

				
				dump_result_set(conn, prepared_stmt2, "**** Utenti biblioteca ****");
	
				mysql_stmt_close(prepared_stmt2);
				mysql_close(conn);

				if(!parse_config("users/login.json", &conf)){
					fprintf(stderr, "Unable to load login configuration\n");
					exit(EXIT_FAILURE);
				}

				conn = mysql_init (NULL);
				if(conn == NULL){
					fprintf(stderr, "mysql_init() failed (probably out of memory)\n");
					exit(EXIT_FAILURE);
				}

				if(mysql_real_connect(conn, conf.host, conf.db_username, conf.db_password, conf.database, conf.port, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS) == NULL) {
					fprintf(stderr, "mysql_real_connect() failed\n");
					mysql_close(conn);
					exit(EXIT_FAILURE);
				}
  				

  				

retry3:
				printf("Inserire l'ID del utente a cui dare la copia: \n");
				ret = scanf("%d", &idUtente);
				if(ret == 0){
					printf("input non valido\n");
					flush(stdin);
					goto retry3;
				}
		
				idUtente_gl = idUtente;
  				
  			}else{

  				// procedura di iscrizione dell'utente alla biblioteca
  				add_new_user(conn);

  				
  			}

  		}
		
  			


		printf("inizio procedura add_prestito\n");
		
		idCopia_gl = idCopia;
		Terminato_gl = 0;
		printf("---> idUtente: %d\t idCopia: %d\n", idUtente_gl, idCopia_gl);

  		
		printf("**** CONSEGNA COPIA ****\n L'utente per quanto tempo vuole consultare il libro? (1, 2 o 3 mesi max)\n");

rescan8:
		printf("Periodo di consultazione: \n");
		scanf("%d", &pConsultazione);
		flush(stdin);
		if(ret == 0){
			printf("input non valido\n");
			goto rescan8;
		}

		if(pConsultazione != 1 && pConsultazione != 2 && pConsultazione != 3){
			printf("input non valido\n");
			goto rescan8;
		}

		Periodo_consultazione_gl = pConsultazione;  		
  		add_new_prestito(conn);		
  	}
	
}




static void report_libri(MYSQL *conn){

	MYSQL_STMT *prepared_stmt, *prepare_meta_result;
	MYSQL_BIND param[6];
	int column_count;
	bool is_null[4];
	bool error[6];
	unsigned long length[6];

	int idLibro;
	char nome[45];
	int idCopia;
	int idUtente;
	char nomeUtente[30];
	char cognomeUtente[30];

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call report_prestiti_proc(?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initializa report_prestiti statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id_utilizzatore_sistema;
	param[0].buffer_length = sizeof(id_utilizzatore_sistema);


	if(mysql_stmt_bind_param(prepared_stmt, param) != 0){
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for report prestiti\n", true);
	}

	// Run procedure
	if(mysql_stmt_execute(prepared_stmt) != 0){
		print_stmt_error(prepared_stmt, "An error occurred while retrieving the report_prestiti\n");
		return;
	}

	/* Fetch result set meta information */
	prepare_meta_result = mysql_stmt_result_metadata(prepared_stmt);
	if (!prepare_meta_result)
	{
  		fprintf(stderr," mysql_stmt_result_metadata(), returned no meta information\n");
  		fprintf(stderr, " %s\n", mysql_stmt_error(prepared_stmt));
  		exit(0);
	}

	/* Get total columns in the query */
	column_count= mysql_num_fields(prepare_meta_result);
	fprintf(stdout, " total columns in SELECT statement: %d\n", column_count);

	if (column_count != 6) /* validate column count */
	{
  		fprintf(stderr, " invalid column count returned by MySQL\n");
  		exit(0);
	}
	
	memset(param, 0, sizeof(param));
	
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &idLibro;
	param[0].buffer_length = sizeof(idLibro);
	param[0].is_null = &is_null[0];
	param[0].length = &length[0];
	param[0].error = &error[0];

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = nome;
	param[1].buffer_length = strlen(nome);
	param[1].is_null = &is_null[1];
	param[1].length = &length[1];
	param[1].error = &error[1];

	param[2].buffer_type = MYSQL_TYPE_LONG;
	param[2].buffer = &idCopia;
	param[2].buffer_length = sizeof(idCopia);
	param[2].is_null = &is_null[2];
	param[2].length = &length[2];
	param[2].error = &error[2];

	param[3].buffer_type = MYSQL_TYPE_LONG;
	param[3].buffer = &idUtente;
	param[3].buffer_length = sizeof(idUtente);
	param[3].is_null = &is_null[3];
	param[3].length = &length[3];
	param[3].error = &error[3];

	param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[4].buffer = nomeUtente;
	param[4].buffer_length = strlen(nomeUtente);
	param[4].is_null = &is_null[4];
	param[4].length = &length[4];
	param[4].error = &error[4];

	param[5].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[5].buffer = cognomeUtente;
	param[5].buffer_length = strlen(cognomeUtente);
	param[5].is_null = &is_null[5];
	param[5].length = &length[5];
	param[5].error = &error[5];

	dump_result_set(conn, prepared_stmt, "**** Prestiti in corso ****");
	
	mysql_stmt_close(prepared_stmt);
	mysql_close(conn);

	if(!parse_config("users/login.json", &conf)){
		fprintf(stderr, "Unable to load login configuration\n");
		exit(EXIT_FAILURE);
	}

	conn = mysql_init (NULL);
	if(conn == NULL){
		fprintf(stderr, "mysql_init() failed (probably out of memory)\n");
		exit(EXIT_FAILURE);
	}

	if(mysql_real_connect(conn, conf.host, conf.db_username, conf.db_password, conf.database, conf.port, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS) == NULL) {
		fprintf(stderr, "mysql_real_connect() failed\n");
		mysql_close(conn);
		exit(EXIT_FAILURE);
	}



}


static void contatta_utente(MYSQL *conn){

	MYSQL_STMT *prepared_stmt, *prepare_meta_result;
	MYSQL_BIND param[4];
	int column_count;
	bool is_null[4];
	bool error[4];
	unsigned long length[4];

	int id_utente;
	char nome[30], cognome[30], titolo_libro[45], contatto_preferito[45];
	

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call recapito_utenti_proc(?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initializa report_prestiti statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id_utilizzatore_sistema;
	param[0].buffer_length = sizeof(id_utilizzatore_sistema);


	if(mysql_stmt_bind_param(prepared_stmt, param) != 0){
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for report prestiti\n", true);
	}

	// Run procedure
	if(mysql_stmt_execute(prepared_stmt) != 0){
		print_stmt_error(prepared_stmt, "An error occurred while retrieving the report_prestiti\n");
		return;
	}

	/* Fetch result set meta information */
	prepare_meta_result = mysql_stmt_result_metadata(prepared_stmt);
	if (!prepare_meta_result)
	{
  		fprintf(stderr," mysql_stmt_result_metadata(), returned no meta information\n");
  		fprintf(stderr, " %s\n", mysql_stmt_error(prepared_stmt));
  		exit(0);
	}

	/* Get total columns in the query */
	column_count= mysql_num_fields(prepare_meta_result);
	fprintf(stdout, " total columns in SELECT statement: %d\n", column_count);

	if (column_count != 5) /* validate column count */
	{
  		fprintf(stderr, " invalid column count returned by MySQL\n");
  		exit(0);
	}
	
	memset(param, 0, sizeof(param));

	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &id_utente;
	param[1].buffer_length = sizeof(id_utente);
	param[1].is_null = &is_null[1];
	param[1].length = &length[1];
	param[1].error = &error[1];
	
	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = nome;
	param[1].buffer_length = strlen(nome);
	param[1].is_null = &is_null[1];
	param[1].length = &length[1];
	param[1].error = &error[1];

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = cognome;
	param[2].buffer_length = strlen(cognome);
	param[2].is_null = &is_null[2];
	param[2].length = &length[2];
	param[2].error = &error[2];

	param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = titolo_libro;
	param[3].buffer_length = strlen(titolo_libro);
	param[3].is_null = &is_null[3];
	param[3].length = &length[3];
	param[3].error = &error[3];

	param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[4].buffer = contatto_preferito;
	param[4].buffer_length = strlen(contatto_preferito);
	param[4].is_null = &is_null[4];
	param[4].length = &length[4];
	param[4].error = &error[4];

	

	dump_result_set(conn, prepared_stmt, "**** Contatta utenti  ****");
	
	mysql_stmt_close(prepared_stmt);
	mysql_close(conn);

	if(!parse_config("users/login.json", &conf)){
		fprintf(stderr, "Unable to load login configuration\n");
		exit(EXIT_FAILURE);
	}

	conn = mysql_init (NULL);
	if(conn == NULL){
		fprintf(stderr, "mysql_init() failed (probably out of memory)\n");
		exit(EXIT_FAILURE);
	}

	if(mysql_real_connect(conn, conf.host, conf.db_username, conf.db_password, conf.database, conf.port, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS) == NULL) {
		fprintf(stderr, "mysql_real_connect() failed\n");
		mysql_close(conn);
		exit(EXIT_FAILURE);
	}





}

static void restituzione_prestito(MYSQL *conn, int idBibliotecario_rp){

	MYSQL_RES *query_result, *query_result1, *query_result2;
	MYSQL_ROW row, row1, row2;
	char q[1024];
	int n_res, index = 0;
	int i = 0,idCopia, ret, cop_ind, day_diff;
	int day_pen, scaffale, ripiano;
	float penale = 0;
	char username[45];

	printf("**** RESTITUZIONE PRESTITO ****\n");

reinsert3:
	printf("Inserisci username del utente: ");
	memset(username, 0, 45);
	while(true){
       	assert(fgets(username, 45, stdin) != NULL);
        if(username[strlen(username) - 1] == '\n'){
            username[strlen(username) - 1] = '\0';
            printf("Username: %s\n", username);
            break;
        }
        printf("Username: %s\n", username);  
    }

	memset(q, 0, 1024);
	snprintf(q, 1024, "SELECT U.ID FROM UTENTE as U inner join UTILIZZATORI_SISTEMA as US on (U.UTILIZZATORI_SISTEMA_ID = US.ID) WHERE US.username = '%s'", username);
	
	if(mysql_query(conn, q) != 0){
		  print_error(conn, "query SELECT statement (10) failed\n");
	}else{
  		printf ("query SELECT statement (10) succeeded\n");
	}

	query_result2 = mysql_store_result(conn);
	if((n_res = mysql_num_rows(query_result2)) == 0){
		printf("Utente non trovato..\n");
		goto reinsert3;
	}

	row2 = mysql_fetch_row(query_result2);
	idUtente_rp = atoi(row2[0]);
	printf("ID UTENTE: %d\n", idUtente_rp);

	memset(q, 0, 1024);
	snprintf(q, 1024, "SELECT C.idCopia, L.Nome, CP.Data_inizio, CP.Periodo_consultazione, P.idPRESTITO FROM UTENTE as U inner join PRESTITO as P on (U.ID = P.UTENTE_ID) inner join COPIA as C on (P.COPIA_idCOPIA = C.idCOPIA) inner join COPIA_IN_PRESTITO as CP on (C.idCOPIA = CP.COPIA_idCOPIA) inner join LIBRO as L on (C.LIBRO_idLIBRO = L.idLIBRO) WHERE U.ID = '%d' and P.Terminato = '%d'", idUtente_rp, 0);

	if(mysql_query(conn, q) != 0){
  		print_error(conn, "query SELECT statement (10) failed\n");
  	}else{
  		printf ("query SELECT statement (10) succeeded\n");
  		
  		query_result = mysql_store_result(conn);
		n_res = mysql_num_rows(query_result);

		if(n_res == 0){

			printf("Nessuna copia da restituire\n");
			return;
		}else{

			cpu = malloc(sizeof(struct copie_possedute_utente) * n_res);
  			if(cpu == NULL){
  				printf("malloc error\n");
  				exit(-1);
  			}

  			memset(cpu, 0, (size_t)sizeof(cpu));

    		while ((row = mysql_fetch_row (query_result)) != NULL ) {
    			cpu[index].idCopia = atoi(row[0]);
    			memset(cpu[index].nome, 0, 30);
    			memset(cpu[index].data_inizio, 0, 15);
				strncpy(cpu[index].nome, row[1], strlen(row[1]));
				strncpy(cpu[index].data_inizio, row[2], strlen(row[2]));
				cpu[index].periodo_cons = atoi(row[3]);
				cpu[index].idPrestito = atoi(row[4]);
				printf("ID copia: %d\t Titolo libro: %s\t Data inizio prestito: %s\t Periodo di consultazione: %d mesi\n", cpu[index].idCopia, cpu[index].nome, cpu[index].data_inizio, cpu[index].periodo_cons);
				index++;		
			}

retry:
			printf("Inserire l' ID della copia che si vuole restituire: \n");
			ret = scanf("%d", &idCopia);
			if(ret == 0){
				printf("input non valido\n");
				flush(stdin);
				goto retry;
			}

			for(int j = 0;j<n_res;j++){
				for(i = 0; i < n_res; i++){
					if(idCopia == cpu[i].idCopia){
						cop_ind = i;
						goto cont;
					}
				}
				printf("input non valido\n");
				goto retry;
			}

		
cont:
			printf("ID copia da restituire: %d ottenuta in data: %s\n", idCopia, cpu[cop_ind].data_inizio);

			// verifico penale da pagare
		
			memset(q, 0, 1024);
			snprintf(q, 1024, "SELECT DATEDIFF(current_date(), '%s')", cpu[cop_ind].data_inizio);

			if(mysql_query(conn, q) != 0){
  				print_error(conn, "query SELECT statement (11) failed\n");
  			}else{
  				printf ("query SELECT statement (11) succeeded\n");
  	    		query_result1 = mysql_store_result(conn);
  	    		row1 = mysql_fetch_row(query_result1);
  	    		day_diff = atoi(row1[0]);

  	    		
  	    	}


  	    	// assumo che un mese abbia mediamente 30 giorni
  	    	if((day_pen = ((day_diff - cpu[cop_ind].periodo_cons * 30))) <= 0){
  	    	
  	    		printf("copia restituita in tempo.. PENALE NON PREVISTA\n");

  	    		// query per popolare la tabella in cui sono salvate tutte le copie restituite nel sistema
  	    		memset(q, 0, 1024);
  	    		snprintf(q, 1024, "INSERT INTO RESTITUIRE (Penale, Data_restituzione, UTENTE_ID, COPIA_idCOPIA) VALUES ('%f', current_date(), '%d', '%d')", penale, idUtente_rp, cpu[cop_ind].idCopia);
  	    	
  	    		if(mysql_query(conn, q) != 0){
  					print_error(conn, "query INSERT statement (12) failed\n");
  				}else{
  					printf ("query INSERT statement (12) succeeded\n");
  				}

  				// query per eliminare la copia restituita dalle copie attualmente in prestito
  				memset(q, 0, 1024);
  	    		snprintf(q, 1024, "DELETE FROM COPIA_IN_PRESTITO WHERE COPIA_idCOPIA = '%d'", cpu[cop_ind].idCopia);
  	    	
  	    		if(mysql_query(conn, q) != 0){
  					print_error(conn, "query DELETE statement (12) failed\n");
  				}else{
  					printf ("query DELETE statement (12) succeeded\n");
  				}
rechoice:
  				printf("Inserire Ripiano in cui riporre la copia: \n");
  				ret = scanf("%d", &ripiano);
  				if(ret == 0){
  					printf("input non valido\n");
  					flush(stdin);
  					goto rechoice;
  				}

  				printf("---> Ripiano: %d\n", ripiano);

rechoice1:
  				printf("Inserire Scaffale in cui riporre la copia: \n");
  				ret = scanf("%d", &scaffale);
  				if(ret == 0){
  					printf("input non valido\n");
  					flush(stdin);
  					goto rechoice1;
  				}

  				printf("---> Scaffale: %d\n", scaffale);
  				memset(q, 0, 1024);
  	    		snprintf(q, 1024, "INSERT INTO COPIA_DISPONIBILE (Ripiano, Scaffale, COPIA_idCOPIA) VALUES ('%d', '%d', '%d')",ripiano, scaffale, cpu[cop_ind].idCopia);
  	    	
  	    		if(mysql_query(conn, q) != 0){
  					print_error(conn, "query INSERT statement (12) failed\n");
  				}else{
  					printf ("query INSERT statement (12) succeeded\n");
  				}

  				memset(q, 0, 1024);
  	    		snprintf(q, 1024, "UPDATE PRESTITO SET Terminato = 1 WHERE idPrestito = '%d'", cpu[cop_ind].idPrestito);
  	    	
  	    		if(mysql_query(conn, q) != 0){
  					print_error(conn, "query UPDATE statement (12) failed\n");
  				}else{
  					printf ("query UPDATE statement (12) succeeded\n");
  				}

  				printf("**** copia restituita correttamente ****\n");
  	    	}else{
  	    		printf("DIFFERENZA Di GIORNI: %d\n", day_pen);
  	    		printf("Copia restituita in ritardo\n");

  	    		// calcolo della penale

  	    		if(day_pen <= 10){
  	    			penale = 0.10*day_pen;
  	    		}else{
  	    			day_pen = day_pen - 10;
  	    			penale = penale + (0.10*10) + (0.50*day_pen);
  	    		}

  	    		printf("---> PENALE DA PAGARE: %.2f euro\n", penale);
  	    		flush(stdin);

  	    		// query per popolare la tabella in cui sono salvate tutte le copie restituite nel sistema
  	    		memset(q, 0, 1024);
  	    		snprintf(q, 1024, "INSERT INTO RESTITUIRE (Penale, Data_restituzione, UTENTE_ID, COPIA_idCOPIA) VALUES ('%.2f', current_date(), '%d', '%d')", penale, idUtente_rp, cpu[cop_ind].idCopia);
  	    	
  	    		if(mysql_query(conn, q) != 0){
  					print_error(conn, "query INSERT statement (12) failed\n");
  				}else{
  					printf ("query INSERT statement (12) succeeded\n");
  				}

  				// query per eliminare la copia restituita dalle copie attualmente in prestito
  				memset(q, 0, 1024);
  	    		snprintf(q, 1024, "DELETE FROM COPIA_IN_PRESTITO WHERE COPIA_idCOPIA = '%d'", cpu[cop_ind].idCopia);
  	    	
  	    		if(mysql_query(conn, q) != 0){
  					print_error(conn, "query DELETE statement (12) failed\n");
  				}else{
  					printf ("query DELETE statement (12) succeeded\n");
  				}
rechoice2:
  				printf("Inserire Ripiano in cui riporre la copia: \n");
  				ret = scanf("%d", &ripiano);
  				if(ret == 0){
  					printf("input non valido\n");
  					flush(stdin);
  					goto rechoice2;
  				}

  				printf("---> Ripiano: %d\n", ripiano);

rechoice3:
  				printf("Inserire Scaffale in cui riporre la copia: \n");
  				ret = scanf("%d", &scaffale);
  				if(ret == 0){
  					printf("input non valido\n");
  					flush(stdin);
  					goto rechoice3;
  				}

  				printf("---> Scaffale: %d\n", scaffale);
  				memset(q, 0, 1024);
  	    		snprintf(q, 1024, "INSERT INTO COPIA_DISPONIBILE (Ripiano, Scaffale, COPIA_idCOPIA) VALUES ('%d', '%d', '%d')",ripiano, scaffale, cpu[cop_ind].idCopia);
  	    	
  	    		if(mysql_query(conn, q) != 0){
  					print_error(conn, "query INSERT statement (12) failed\n");
  				}else{
  					printf ("query INSERT statement (12) succeeded\n");
  				}

  				memset(q, 0, 1024);
  	    		snprintf(q, 1024, "UPDATE PRESTITO SET Terminato = 1 WHERE idPrestito = '%d'", cpu[cop_ind].idPrestito);
  	    	
  	    		if(mysql_query(conn, q) != 0){
  					print_error(conn, "query UPDATE statement (12) failed\n");
  				}else{
  					printf ("query UPDATE statement (12) succeeded\n");
  				}

  				printf("**** copia restituita correttamente ****\n");

  	    	}

		}
		

  	}
}




void run_as_bibliotecario(MYSQL *conn, int idUtilizzatoreSistema){

	char options[7] = {'1', '2', '3', '4', '5', '6', '7'};
	char op, q[1024];
	MYSQL_RES *query_result;
	MYSQL_ROW row;
	int ret;

	printf("Switching to libriarian role...\n");

	if(!parse_config("users/bibliotecario.json", &conf)){
		fprintf(stderr, "Unable to load libriarian configuration\n");
		exit(EXIT_FAILURE);
	}

	if(mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)){
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

	id_utilizzatore_sistema = idUtilizzatoreSistema;

rechoice1:

	while(true){

		printf("\033[2J\033[H");
		printf("*** What should I do for you? ***\n\n");
		printf("1) Richiesta di malattia\n");
		printf("2) Aggiungi utente\n");
		printf("3) Disponibilità copie\n");
		printf("4) Report libri\n");
		printf("5) Contatta utente\n");
		printf("6) Restituzione prestito\n");
		printf("7) Quit\n");

		op = multiChoice("Select an option", options, 7);

		switch(op) {
			case '1':
				
				richiesta_di_malattia(conn);
				break;

			case '2':
				
				add_new_user(conn);
				/*
				if(ret == 1){
					goto rechoice1;
				}
				add_new_user_bis(conn);
				add_recapiti(conn);
				subscribe_user_to_a_library(conn);*/
				break;

			case '3':

				disponibilita_copie(conn);
				break;

			case '4':
				
				report_libri(conn);
				break;
			
			case '5':
				
				contatta_utente(conn);
				break;

			case '6':
				memset(q, 0, 1024);
				snprintf(q, 1024, "SELECT idBibliotecario FROM BIBLIOTECARIO WHERE UTILIZZATORI_SISTEMA_ID = '%d'", idUtilizzatoreSistema);
				if(mysql_query(conn, q)!= 0) {
					print_error (conn, "query SELECT (1) statement failed\n");
				} else {
					printf ("query SELECT statement (1) succeeded\n");
				}

				query_result = mysql_store_result(conn);

				while ((row = mysql_fetch_row (query_result)) != NULL) {
					for (int i = 0; i < (int)mysql_num_fields (query_result); i++) {
						idBibliotecario_rp = atoi(row[0]);
					}
					printf("\n");
				}
				restituzione_prestito(conn, idBibliotecario_rp);
				break;
			
			case '7':
				return;
				
			default:
				fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
				abort();
		}

		printf("\npremere invio per continuare..\n");
		getchar();

	}


}