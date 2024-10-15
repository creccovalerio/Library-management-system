#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "defines.h"

#define flush(stdin) while(getchar() != '\n')

struct bibliotecari_disponibili *bd;
char buffer[64];
int idBibliotecario, idBiblioteca, idBibliotecario_rp, idUtente_rp, idBiblioteca_sb;
int idUtente_gl, idCopia_gl, idTurno_gl, Terminato_gl;
int id_new_util, id_new_user;
int id_utilizzatore_sistema;


struct copie_disponibili *cd;
struct utente *ub, *ub1;
struct configuration conf;


struct repo_turni_scoperti{
	char ora_inizio[10];
	char ora_fine[10];
	int numero;
	char giorno[10];
	char mese[15];
	int anno;
};


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

static void find_slots_sub(MYSQL *conn){

	MYSQL_STMT *prepared_stmt, *prepare_meta_result;
	MYSQL_STMT *prepared_stmt2, *prepare_meta_result2;
	MYSQL_STMT *prepared_stmt3, *prepare_meta_result3;
	MYSQL_STMT *prepared_stmt4, *prepare_meta_result4;
	MYSQL_STMT *prepared_stmt5;

	MYSQL_BIND param[8];
	MYSQL_BIND param1[1];
	MYSQL_BIND param2[9];
	MYSQL_BIND param3[3];
	MYSQL_BIND param4[2];
	MYSQL_BIND param5[3];


	int column_count, ret, rows_count;
	bool is_null[2];
	bool error[2];
	unsigned long length[2];

	int numero, anno, idTurno, idTurno_ck;
	char giorno[15];
	char mese[15];
	char ora_inizio[10];
	char ora_fine[10];
	char bibliotecario_cf[17];
	char motivo[50];
	char nome[31];
	char cognome[31];

	// chiamo find_slots_to sub() per mostrare i turni attualmente scoperti
	if(!setup_prepared_stmt(&prepared_stmt, "call find_slots_to_sub(?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize richiesta_malattia insertion statement\n", false);
	}

	// Prepare parameters
	memset(param1, 0, sizeof(param));

	param1[0].buffer_type = MYSQL_TYPE_LONG;
	param1[0].buffer = &id_utilizzatore_sistema;
	param1[0].buffer_length = sizeof(id_utilizzatore_sistema);

	if(mysql_stmt_bind_param(prepared_stmt, param1) != 0){
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for request inseriton\n", true);
	}

	// Run procedure
	if(mysql_stmt_execute(prepared_stmt) != 0){
		print_stmt_error(prepared_stmt, "An error occurred while adding the request");
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

	if (column_count != 8) /* validate column count */
	{
  		fprintf(stderr, " invalid column count returned by MySQL\n");
  		exit(0);
	}
	
	memset(param, 0, sizeof(param));
	
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &idTurno;
	param[0].buffer_length = sizeof(idTurno);
	param[0].is_null = &is_null[0];
	param[0].length = &length[0];
	param[0].error = &error[0];

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = ora_inizio;
	param[1].buffer_length = strlen(ora_inizio);
	param[1].is_null = &is_null[1];
	param[1].length = &length[1];
	param[1].error = &error[1];

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = ora_fine;
	param[2].buffer_length = strlen(ora_fine);
	param[2].is_null = &is_null[2];
	param[2].length = &length[2];
	param[2].error = &error[2];

	param[3].buffer_type = MYSQL_TYPE_LONG;
	param[3].buffer = &numero;
	param[3].buffer_length = sizeof(numero);
	param[3].is_null = &is_null[3];
	param[3].length = &length[3];
	param[3].error = &error[3];

	param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[4].buffer = giorno;
	param[4].buffer_length = strlen(giorno);
	param[4].is_null = &is_null[4];
	param[4].length = &length[4];
	param[4].error = &error[4];

	param[5].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[5].buffer = mese;
	param[5].buffer_length = strlen(mese);
	param[5].is_null = &is_null[5];
	param[5].length = &length[5];
	param[5].error = &error[5];

	param[6].buffer_type = MYSQL_TYPE_LONG;
	param[6].buffer = &anno;
	param[6].buffer_length = sizeof(anno);
	param[6].is_null = &is_null[6];
	param[6].length = &length[6];
	param[6].error = &error[6];

	param[7].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[7].buffer = bibliotecario_cf;
	param[7].buffer_length = strlen(bibliotecario_cf);
	param[7].is_null = &is_null[7];
	param[7].length = &length[7];
	param[7].error = &error[7];

	dump_result_set(conn, prepared_stmt, "**** Turni scoperti ****");
	
	mysql_stmt_close(prepared_stmt);
	
	reopen_connection(conn);


	// inserisco le informazioni del turno che vogliamo andare a coprire
rescan_all:

	printf("\nInserisci infomazioni del turno da ricoprire:\n");

	printf("Inserisci ora inizio del turno: (formato: HH:MM:SS)\n");
	memset(ora_inizio, 0, 10);
	while(true){
        assert(fgets(ora_inizio, 10, stdin) != NULL);
        if(ora_inizio[strlen(ora_inizio) - 1] == '\n'){
            ora_inizio[strlen(ora_inizio) - 1] = '\0';
            printf("Ora inizio del turno: %s\n", ora_inizio);
            break;
        }
        printf("Ora inizio del turno : %s\n", ora_inizio);  
    }

    printf("Inserisci ora fine del turno: (formato: HH:MM:SS)\n");
	memset(ora_fine, 0, 10);
	while(true){
        assert(fgets(ora_fine, 10, stdin) != NULL);
        if(ora_fine[strlen(ora_fine) - 1] == '\n'){
            ora_fine[strlen(ora_fine) - 1] = '\0';
            printf("Ora fine del turno: %s\n", ora_fine);
            break;
        }
        printf("Ora fine del turno : %s\n", ora_fine);  
    }

rescan2:
	printf("Inserisci il numero del giorno: \n");
	ret = scanf("%d", &numero);
	flush(stdin);
	if(ret == 0){
		printf("input non valido\n");
		goto rescan2;
	}

	printf("Inserisci il giorno (es: lunedi, martedi..)\n");
	memset(giorno, 0, 15);
	while(true){
        assert(fgets(giorno, 15, stdin) != NULL);
        if(giorno[strlen(giorno) - 1] == '\n'){
            giorno[strlen(giorno) - 1] = '\0';
            printf("Giorno : %s\n", giorno);
            break;
        }
        printf("Giorno (1): %s\n", giorno);  
    }


    printf("Inserisci il mese (es: gennaio, febbraio..)\n");
    memset(mese, 0, 15);
	while(true){
        assert(fgets(mese, 15, stdin) != NULL);
        if(mese[strlen(mese) - 1] == '\n'){
            mese[strlen(mese) - 1] = '\0';
            printf("Mese : %s\n", mese);
            break;
        }
        printf("Mese : %s\n", mese);  
    }


rescan3:
	printf("Inserisci l'anno: \n");
	ret = scanf("%d", &anno);
	flush(stdin);
	if(ret == 0){
		printf("input non valido\n");
		goto rescan3;
	}

	printf("Inserisci il CF del bibliotecario da sostituire: \n");
	getInput(17, bibliotecario_cf, false);
	
	
	// verifico che tale turno effettivamente esista nella base di dati
	// chiamando check_inserted_slot() 
	if(!setup_prepared_stmt(&prepared_stmt2, "call check_inserted_slot(?,?,?,?,?,?,?,?,?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt2, "Unable to initialize richiesta_malattia insertion statement\n", false);
	}

	// Prepare parameters
	memset(param2, 0, sizeof(param2));

	param2[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param2[0].buffer = ora_inizio;
	param2[0].buffer_length = strlen(ora_inizio);

	param2[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param2[1].buffer = ora_fine;
	param2[1].buffer_length = strlen(ora_fine);

	param2[2].buffer_type = MYSQL_TYPE_LONG;
	param2[2].buffer = &numero;
	param2[2].buffer_length = sizeof(numero);

	param2[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param2[3].buffer = giorno;
	param2[3].buffer_length = strlen(giorno);

	param2[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	param2[4].buffer = mese;
	param2[4].buffer_length = strlen(mese);

	param2[5].buffer_type = MYSQL_TYPE_LONG;
	param2[5].buffer = &anno;
	param2[5].buffer_length = sizeof(anno);

	param2[6].buffer_type = MYSQL_TYPE_VAR_STRING;
	param2[6].buffer = bibliotecario_cf;
	param2[6].buffer_length = strlen(bibliotecario_cf);

	param2[7].buffer_type = MYSQL_TYPE_LONG;
	param2[7].buffer = &id_utilizzatore_sistema;
	param2[7].buffer_length = sizeof(id_utilizzatore_sistema);

	param2[8].buffer_type = MYSQL_TYPE_LONG;
	param2[8].buffer = &idTurno_ck;
	param2[8].buffer_length = sizeof(idTurno_ck);
	
	if(mysql_stmt_bind_param(prepared_stmt2, param2) != 0){
		finish_with_stmt_error(conn, prepared_stmt2, "Could not bind parameters for request inseriton\n", true);
	}

	// Run procedure
	if(mysql_stmt_execute(prepared_stmt2) != 0){
		print_stmt_error(prepared_stmt2, "An error occurred while adding the request");
	}

	/* Fetch result set meta information */
	prepare_meta_result2 = mysql_stmt_result_metadata(prepared_stmt2);
	if (!prepare_meta_result2)
	{
  		fprintf(stderr," mysql_stmt_result_metadata(), returned no meta information\n");
  		fprintf(stderr, " %s\n", mysql_stmt_error(prepared_stmt2));
  		exit(0);
	}

	if (mysql_stmt_store_result(prepared_stmt2)) {
		fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(prepared_stmt2));
		exit(0);
	}

	/* Get total rows in the query */
	rows_count = mysql_stmt_num_rows(prepared_stmt2);
	fprintf(stdout, " total rows in SELECT statement: %d\n", rows_count);

	if (rows_count < 1) /* validate column count */
	{
  		fprintf(stderr, " invalid slot information inserted.. please retry...\n");
  		goto rescan_all;
	}
	else{

		// inserisco le informazioni per ricoprire il turno scelto
		printf("\n Inserted slot OK.. ");

		memset(param2, 0, sizeof(param));
	
		param2[0].buffer_type = MYSQL_TYPE_LONG;
		param2[0].buffer = &idTurno;
		param2[0].buffer_length = sizeof(idTurno);

		if(mysql_stmt_bind_result(prepared_stmt2, param2)){
			finish_with_stmt_error(conn, prepared_stmt2, "Could not retrieve output parameter", true);
		}

		// Retrieve output parameter
		if(mysql_stmt_fetch(prepared_stmt2)){
			finish_with_stmt_error(conn, prepared_stmt2, "Could not buffer the results", true);
		}

		printf("\n ID turno selezionato: %d", idTurno);

		mysql_stmt_close(prepared_stmt2);
		
		reopen_connection(conn);

		printf("\n Inserire motivo dell'assenza: ");
	  	memset(motivo, 0, 50);
		getInput(51, motivo, false);

		// mostriamo i bibliotecari disponibili a ricoprire tale turno


		if(!setup_prepared_stmt(&prepared_stmt3, "call show_free_librarians(?,?,?)", conn)){
			finish_with_stmt_error(conn, prepared_stmt3, "Unable to initialize richiesta_malattia insertion statement\n", false);
		}

		// Prepare parameters
		memset(param3, 0, sizeof(param3));

		param3[0].buffer_type = MYSQL_TYPE_LONG;
		param3[0].buffer = &id_utilizzatore_sistema;
		param3[0].buffer_length = sizeof(id_utilizzatore_sistema);

		param3[1].buffer_type = MYSQL_TYPE_VAR_STRING;
		param3[1].buffer = bibliotecario_cf;
		param3[1].buffer_length = strlen(bibliotecario_cf);

		param3[2].buffer_type = MYSQL_TYPE_LONG;
		param3[2].buffer = &idTurno;
		param3[2].buffer_length = sizeof(idTurno);

		
		if(mysql_stmt_bind_param(prepared_stmt3, param3) != 0){
			finish_with_stmt_error(conn, prepared_stmt3, "Could not bind parameters for request inseriton\n", true);
		}

		// Run procedure
		if(mysql_stmt_execute(prepared_stmt3) != 0){
			print_stmt_error(prepared_stmt3, "An error occurred while adding the request");
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
		column_count= mysql_num_fields(prepare_meta_result3);
		fprintf(stdout, " total columns in SELECT statement: %d\n", column_count);

		if (column_count != 3) /* validate column count */
		{
	  		fprintf(stderr, " invalid column count returned by MySQL\n");
	  		exit(0);
		}
		
		memset(param3, 0, sizeof(param3));
		
		param3[0].buffer_type = MYSQL_TYPE_VAR_STRING;
		param3[0].buffer = bibliotecario_cf;
		param3[0].buffer_length = strlen(bibliotecario_cf);
		param3[0].is_null = &is_null[0];
		param3[0].length = &length[0];
		param3[0].error = &error[0];

		param3[1].buffer_type = MYSQL_TYPE_VAR_STRING;
		param3[1].buffer = nome;
		param3[1].buffer_length = strlen(nome);
		param3[1].is_null = &is_null[1];
		param3[1].length = &length[1];
		param3[1].error = &error[1];

		param3[2].buffer_type = MYSQL_TYPE_VAR_STRING;
		param3[2].buffer = cognome;
		param3[2].buffer_length = strlen(cognome);
		param3[2].is_null = &is_null[2];
		param3[2].length = &length[2];
		param3[2].error = &error[2];

		
		dump_result_set(conn, prepared_stmt3, "**** Bibliotecari disponibili ****");

		mysql_stmt_close(prepared_stmt3);
		reopen_connection(conn);

rechoice:

		printf("\nSeleziona il CF del bibliotecario da scegliere come sostituto: \n");
		getInput(17, bibliotecario_cf, false);

		if(!setup_prepared_stmt(&prepared_stmt4, "call check_inserted_librarian_cf(?,?)", conn)){
			finish_with_stmt_error(conn, prepared_stmt4, "Unable to initialize richiesta_malattia insertion statement\n", false);
		}

		// Prepare parameters
		memset(param4, 0, sizeof(param4));

		param4[0].buffer_type = MYSQL_TYPE_VAR_STRING;
		param4[0].buffer = bibliotecario_cf;
		param4[0].buffer_length = strlen(bibliotecario_cf);

		param4[1].buffer_type = MYSQL_TYPE_LONG;
		param4[1].buffer = &id_utilizzatore_sistema;
		param4[1].buffer_length = sizeof(id_utilizzatore_sistema);

		
		if(mysql_stmt_bind_param(prepared_stmt4, param4) != 0){
			finish_with_stmt_error(conn, prepared_stmt4, "Could not bind parameters for request inseriton\n", true);
		}

		// Run procedure
		if(mysql_stmt_execute(prepared_stmt4) != 0){
			print_stmt_error(prepared_stmt4, "An error occurred while adding the request");
		}

		/* Fetch result set meta information */
		prepare_meta_result4 = mysql_stmt_result_metadata(prepared_stmt4);
		if (!prepare_meta_result4)
		{
	  		fprintf(stderr," mysql_stmt_result_metadata(), returned no meta information\n");
	  		fprintf(stderr, " %s\n", mysql_stmt_error(prepared_stmt4));
	  		exit(0);
		}

		if (mysql_stmt_store_result(prepared_stmt4)) {
			fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
			fprintf(stderr, " %s\n", mysql_stmt_error(prepared_stmt4));
			exit(0);
		}

		/* Get total rows in the query */
		rows_count = mysql_stmt_num_rows(prepared_stmt4);
		fprintf(stdout, " total rows in SELECT statement: %d\n", rows_count);

		if (rows_count < 1) /* validate column count */
		{
	  		fprintf(stderr, " invalid cf information inserted.. please retry...\n");
	  		goto rechoice;
		}
		else{

			printf("\n Inserted cf OK.. ");

			mysql_stmt_close(prepared_stmt4);
			reopen_connection(conn);

			if(!setup_prepared_stmt(&prepared_stmt5, "call add_librarian_sub(?,?,?)", conn)){
				finish_with_stmt_error(conn, prepared_stmt5, "Unable to initialize richiesta_malattia insertion statement\n", false);
			}

			printf("\nINFO: %s, %d, %s", motivo, idTurno, bibliotecario_cf);
			// Prepare parameters
			memset(param5, 0, sizeof(param5));

			param5[0].buffer_type = MYSQL_TYPE_VAR_STRING;
			param5[0].buffer = motivo;
			param5[0].buffer_length = strlen(motivo);

			param5[1].buffer_type = MYSQL_TYPE_LONG;
			param5[1].buffer = &idTurno;
			param5[1].buffer_length = sizeof(idTurno);

			param5[2].buffer_type = MYSQL_TYPE_VAR_STRING;
			param5[2].buffer = bibliotecario_cf;
			param5[2].buffer_length = strlen(bibliotecario_cf);

			if(mysql_stmt_bind_param(prepared_stmt5, param5) != 0){
				finish_with_stmt_error(conn, prepared_stmt5, "Could not bind parameters for user inseriton\n", true);
			}

			// Run procedure
			if(mysql_stmt_execute(prepared_stmt5) != 0){
				print_stmt_error(prepared_stmt5, "An error occurred while adding the user");
			}

			mysql_stmt_close(prepared_stmt5);

			printf("\nBibliotecario sostituto correttamente!");
		
		}

	
	}
	

}




static void dismiss_book(MYSQL *conn){

  	MYSQL_STMT *prepared_stmt, *prepare_meta_result;
	MYSQL_BIND param[2];
	int column_count;
	bool is_null[2];
	bool error[2];
	unsigned long length[2];

	int idLibro;
	char nome[45];
	
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call dismiss_book_pppb()", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initializa dismiss_book statement\n", false);
	}

	
	// Run procedure
	if(mysql_stmt_execute(prepared_stmt) != 0){
		print_stmt_error(prepared_stmt, "An error occurred while retrieving the dismiss_book\n");
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

	if (column_count != 2) /* validate column count */
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

	dump_result_set(conn, prepared_stmt, "**** Libri dismessi ****");
	
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


static void report_slots(MYSQL *conn){

	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[5];
	MYSQL_RES *prepare_meta_result;
	
	int column_count;
	bool is_null[5];
	bool error[5];
	unsigned long length[5];
	
	int idTurno;
	int numero;
	char giorno[10];
	char mese[15];
	int anno;
	
	
	// Prepare the stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call report_turni_scoperti_proc_pppb(?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize turni scoperti report statement\n", false);
	}



	// prepare parameters
	memset(param, 0, sizeof(param));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id_utilizzatore_sistema;
	param[0].buffer_length = sizeof(id_utilizzatore_sistema);

	if(mysql_stmt_bind_param(prepared_stmt, param) != 0){
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameter for turni scoperti report\n", true);
	}

	// run procedure
	if(mysql_stmt_execute(prepared_stmt) != 0){
		print_stmt_error(prepared_stmt, "An error occurred while retrieving the turni scoperti report");
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

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &idTurno;
	param[0].buffer_length = sizeof(idTurno);
	param[0].is_null = &is_null[0];
	param[0].length = &length[0];
	param[0].error = &error[0];
	
	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &numero;
	param[1].buffer_length = sizeof(numero);
	param[1].is_null = &is_null[1];
	param[1].length = &length[1];
	param[1].error = &error[1];

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = giorno;
	param[2].buffer_length = strlen(giorno);
	param[2].is_null = &is_null[2];
	param[2].length = &length[2];
	param[2].error = &error[2];

	param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = mese;
	param[3].buffer_length = strlen(mese);
	param[3].is_null = &is_null[3];
	param[3].length = &length[3];
	param[3].error = &error[3];

	param[4].buffer_type = MYSQL_TYPE_LONG;
	param[4].buffer = &anno;
	param[4].buffer_length = sizeof(anno);
	param[4].is_null = &is_null[4];
	param[4].length = &length[4];
	param[4].error = &error[4];


	dump_result_set(conn, prepared_stmt, "Turni scoperti");
	
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





static void create_work_slot(MYSQL *conn){

	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[9];
	char q[1024];
	MYSQL_RES *query_result;
	MYSQL_ROW row;
	int index = 0, n_res, idBib, ret;

	char ora_inizio[10], ora_fine[10], giorno[15], mese[15];
	int numero, anno, idTurno;

	printf("\n**** Procedure creazione nuovi turni mensili ****\n");

	printf("Selezione l'ID del bibliotecario a cui assegnare il turno: \n");
	memset(q, 0, 1024);
	snprintf(q, 1024, "SELECT idBibliotecario, Nome, Cognome FROM BIBLIOTECARIO WHERE BIBLIOTECA_idBIBLIOTECA = '%d'", idBiblioteca_sb);

	if(mysql_query(conn, q) != 0){
  		print_error(conn, "query SELECT statement (11) failed\n");
  	}else{
  		printf ("query SELECT statement (11) succeeded\n");
  		
  		query_result = mysql_store_result(conn);
		n_res = mysql_num_rows(query_result);
		if(n_res == 0){

			printf("Nessuna bibliotecario trovato\n");
			return;
		}else{

			bd = malloc(sizeof(struct bibliotecari_disponibili) * n_res);
  			if(bd == NULL){
  				printf("malloc error\n");
  				exit(-1);
  			}

  			memset(bd, 0, (size_t)sizeof(bd));

    		while ((row = mysql_fetch_row (query_result)) != NULL ) {
    			bd[index].idBibliotecario = atoi(row[0]);
    			memset(bd[index].Nome, 0, 30);
    			memset(bd[index].Cognome, 0, 30);
				strncpy(bd[index].Nome, row[1], strlen(row[1]));
				strncpy(bd[index].Cognome, row[2], strlen(row[2]));
				printf("idBibliotecario: %d\t Nome: %s\t Cognome: %s\n", bd[index].idBibliotecario, bd[index].Nome, bd[index].Cognome);
				index++;
			
			}
rechoice:
			printf("Seleziona ID del bibliotecario da scegliere come sostituto: \n");
			ret = scanf("%d", &idBib);
			if(ret == 0){
				printf("input non valido\n");
				goto rechoice;
			}

			for(int j = 0; j < n_res; j++){
				if(bd[j].idBibliotecario == idBib){
					printf("Selezione andata a buon fine\n");
					goto next;
				}
			}

			printf("Id selezionato non valido..\n");
			goto rechoice;

next:
			printf("\nID selezionato: %d", idBib);

		}	

	}

	printf("\nInserisci le informazioni generali del turno da creare:");
	flush(stdin);

	printf("\nOra inizio: ");
	getInput(10, ora_inizio, false);

	printf("\nOra fine: ");
	getInput(10, ora_fine, false);

rescan1:
	printf("\nNumero del giorno: ");
	ret = scanf("%d", &numero);
	flush(stdin);
	if(ret == 0){
		printf("input non valido\n");
		goto rescan1;
	}

	printf("\nGiorno (lunedi, martedi..): ");
	getInput(15, giorno, false);

	printf("\nMese (gennaio, febbraio..): ");
	getInput(6, mese, false);	

rescan2:
	printf("\nAnno: ");
	ret = scanf("%d", &anno);
	flush(stdin);
	if(ret == 0){
		printf("input non valido\n");
		goto rescan2;
	}


	// Prepare prepared statement
	if(!setup_prepared_stmt(&prepared_stmt, "call add_turno_proc(?,?,?,?,?,?,?,?,?)", conn)){
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize add_turno statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = ora_inizio;
	param[0].buffer_length = strlen(ora_inizio);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = ora_fine;
	param[1].buffer_length = strlen(ora_fine);

	param[2].buffer_type = MYSQL_TYPE_LONG;
	param[2].buffer = &numero;
	param[2].buffer_length = sizeof(numero);

	param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = giorno;
	param[3].buffer_length = strlen(giorno);

	param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[4].buffer = mese;
	param[4].buffer_length = strlen(mese);

	param[5].buffer_type = MYSQL_TYPE_LONG;
	param[5].buffer = &anno;
	param[5].buffer_length = sizeof(anno);

	param[6].buffer_type = MYSQL_TYPE_LONG;
	param[6].buffer = &idBib;
	param[6].buffer_length = sizeof(idBib);

	param[7].buffer_type = MYSQL_TYPE_LONG;
	param[7].buffer = &idBiblioteca_sb;
	param[7].buffer_length = sizeof(idBiblioteca_sb);

	param[8].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[8].buffer = &idTurno;
	param[8].buffer_length = sizeof(idTurno);

	if(mysql_stmt_bind_param(prepared_stmt, param) != 0){
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user inseriton\n", true);
	}

	// Run procedure
	if(mysql_stmt_execute(prepared_stmt) != 0){
		print_stmt_error(prepared_stmt, "An error occurred while adding the slot");
		goto out;
	}

	// Get back the ID of the newly_added user
	memset(param, 0, sizeof(param));
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &idTurno;
	param[0].buffer_length = sizeof(idTurno);

	if(mysql_stmt_bind_result(prepared_stmt, param)){
		finish_with_stmt_error(conn, prepared_stmt, "Could not retrieve output parameter", true);
	}

	// Retrieve output parameter
	if(mysql_stmt_fetch(prepared_stmt)){
		finish_with_stmt_error(conn, prepared_stmt, "Could not buffer the results", true);

	}

	printf("\n **** Turno correttamente creato con ID: %d ****\n", idTurno);
	
	out:
	mysql_stmt_close(prepared_stmt);


}



void run_as_amministratore(MYSQL *conn, int idUtilizzatoreSistema){

	char options[7] = {'1', '2', '3', '4', '5', '6', '7'};
	char op, q[1024];
	MYSQL_RES *query_result;
	MYSQL_ROW row;

	printf("Switching to administrative role...\n");

	if(!parse_config("users/amministratore.json", &conf)){
		fprintf(stderr, "Unable to load administrator configuration\n");
		exit(EXIT_FAILURE);

	}

	if(mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)){
		fprintf(stderr, "mysql_change_user failed\n");
		exit(EXIT_FAILURE);
	}

	id_utilizzatore_sistema = idUtilizzatoreSistema;

	while(true){

		printf("\033[2J\033[H");
		printf("*** What should I do for you? ***\n\n");
		printf("1) Sostituisci bibliotecario non disponibile\n");
		printf("2) Dismissione libro\n");
		printf("3) Report turni di lavoro\n");
		printf("4) Crea nuovi turni\n");
		printf("5) Quit\n");

		op = multiChoice("Select an option", options, 5);

		switch(op){

			case '1':
	
				find_slots_sub(conn);			
				break;

			case '2':

				dismiss_book(conn);
				break;
				
			case '3':
				
				report_slots(conn);
				break;

			case '4':

				memset(q, 0, 1024);
				snprintf(q, 1024, "SELECT BB.BIBLIOTECA_idBIBLIOTECA FROM AMMINISTRATORE as A inner join UTILIZZATORI_SISTEMA as U on (A.UTILIZZATORI_SISTEMA_ID = U.ID) inner join BIBLIOTECARIO as BB on (A.BIBLIOTECARIO_idBIBLIOTECARIO = BB.idBibliotecario) WHERE A.UTILIZZATORI_SISTEMA_ID = '%d'", idUtilizzatoreSistema);
				if(mysql_query(conn, q)!= 0) {
					print_error (conn, "query SELECT (2) statement failed\n");
				} else {
					printf ("query SELECT statement (2) succeeded\n");
					query_result = mysql_store_result(conn);

					while ((row = mysql_fetch_row (query_result)) != NULL) {
						for (int i = 0; i < (int)mysql_num_fields (query_result); i++) {
							printf ("idBiblioteca: %s\t", row[0] != NULL ? row[0] : "NULL");
							fflush(stdout);
							idBiblioteca_sb = atoi(row[0]);
						}
						printf("\n");
					}
					create_work_slot(conn);
				}
				
				break;
				
			case '5':
				return;
				
			
			default:
				fprintf(stderr, "Invalid condition at %s: %d\n", __FILE__, __LINE__);
				abort();
		}

		printf("\npremere invio per continuare..\n");
		getchar();
	}
}