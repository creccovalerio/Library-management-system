#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "defines.h"

#define flush(stdin) while(getchar() != '\n')


struct copie_possedute_utente *cpu;
int idUtente_rp;


static void restituzione_prestito(MYSQL *conn){

	MYSQL_RES *query_result, *query_result1;
	MYSQL_ROW row, row1;
	char q[1024];
	int n_res, index = 0;
	int i = 0,idCopia, ret, cop_ind, day_diff;
	int day_pen, scaffale, ripiano;
	float penale = 0;

	printf("**** RESTITUZIONE PRESTITO ****\n");

	// query per visualizzare tutti i prestiti di un utente
	memset(q, 0, 1024);
	snprintf(q, 1024, "SELECT C.idCopia, L.Nome, CP.Data_inizio, CP.Periodo_consultazione, P.idPRESTITO FROM UTENTE as U inner join PRESTITO as P on (U.ID = P.UTENTE_ID) inner join COPIA as C on (P.COPIA_idCOPIA = C.idCOPIA) inner join COPIA_IN_PRESTITO as CP on (C.idCOPIA = CP.COPIA_idCOPIA) inner join LIBRO as L on (C.LIBRO_idLIBRO = L.idLIBRO) WHERE U.ID = '%d' and P.Terminato = 0", idUtente_rp);

	if(mysql_query(conn, q) != 0){
  		print_error(conn, "query SELECT statement (2) failed\n");
  	}else{
  		printf ("query SELECT statement (2) succeeded\n");
  		
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
				printf("\nID copia: %d\tTitolo libro: %s\tData inizio prestito: %s\tPeriodo di consultazione: %d mesi\n", cpu[index].idCopia, cpu[index].nome, cpu[index].data_inizio, cpu[index].periodo_cons);
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

  				printf("**** copia restituita correttamente ****");


  	    	}

		}
			
  	}

}





void run_as_utente(MYSQL *conn, int idUtilizzatoreSistema){

	char options[2] = {'1', '2'};
	char op;
	char q[1024];
	MYSQL_RES *query_result;
	MYSQL_ROW row;

	printf("Switching to user role...\n");
	
	if(!parse_config("users/utente.json", &conf)){
		fprintf(stderr, "Unable to load user configuration\n");
		exit(EXIT_FAILURE);
	}

	if(mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)){
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

	while(true){

		printf("\033[2J\033[H");
		printf("*** What should I do for you? ***\n\n");
		printf("1) Restituzione prestito\n");
		printf("2) Quit\n");

		op = multiChoice("Select an option", options, 2);

		switch(op) {
		
				
			case '1':

				memset(q, 0, 1024);
				snprintf(q, 1024, "SELECT ID FROM UTENTE WHERE UTILIZZATORI_SISTEMA_ID = '%d'", idUtilizzatoreSistema);
				if(mysql_query(conn, q)!= 0) {
					print_error (conn, "query SELECT (1) statement failed\n");
				} else {
					printf ("query SELECT statement (1) succeeded\n");
					query_result = mysql_store_result(conn);

					while ((row = mysql_fetch_row (query_result)) != NULL) {
						for (int i = 0; i < (int)mysql_num_fields (query_result); i++) {
							idUtente_rp = atoi(row[0]);
						}
						printf("\n");
					}
					restituzione_prestito(conn);
				}

				
				break;

			case '2':
				return;
				
			default:
				fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
				abort();
		}

		printf("\npremere invio per continuare..\n");
		getchar();

	}
}