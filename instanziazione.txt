-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema BD_PROJECT
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema BD_PROJECT
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `BD_PROJECT` DEFAULT CHARACTER SET utf8 ;
USE `BD_PROJECT` ;

-- -----------------------------------------------------
-- Table `BD_PROJECT`.`BIBLIOTECA`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`BIBLIOTECA` (
  `idBIBLIOTECA` INT(11) NOT NULL AUTO_INCREMENT,
  `Telefono` VARCHAR(10) NOT NULL,
  `Via` VARCHAR(30) NOT NULL,
  `Civico` INT(11) NOT NULL,
  PRIMARY KEY (`idBIBLIOTECA`))
ENGINE = InnoDB
AUTO_INCREMENT = 11
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`UTILIZZATORI_SISTEMA`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (
  `username` VARCHAR(45) NOT NULL,
  `password` VARCHAR(30) NOT NULL,
  `ruolo` ENUM('amministratore', 'bibliotecario', 'utente') NOT NULL,
  `ID` INT(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`ID`),
  UNIQUE INDEX `username_UNIQUE` (`username` ASC) VISIBLE)
ENGINE = InnoDB
AUTO_INCREMENT = 31
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`BIBLIOTECARIO`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`BIBLIOTECARIO` (
  `idBibliotecario` INT(11) NOT NULL,
  `Nome` VARCHAR(30) NOT NULL,
  `Cognome` VARCHAR(30) NOT NULL,
  `Data_nascita` DATE NOT NULL,
  `Titolo_di_studio` VARCHAR(45) NOT NULL,
  `BIBLIOTECA_idBIBLIOTECA` INT(11) NOT NULL AUTO_INCREMENT,
  `UTILIZZATORI_SISTEMA_ID` INT(11) NOT NULL,
  PRIMARY KEY (`idBibliotecario`),
  INDEX `fk_BIBLIOTECARIO_BIBLIOTECA1_idx` (`BIBLIOTECA_idBIBLIOTECA` ASC) VISIBLE,
  INDEX `fk_BIBLIOTECARIO_UTILIZZATORI_SISTEMA1_idx` (`UTILIZZATORI_SISTEMA_ID` ASC) VISIBLE,
  CONSTRAINT `fk_BIBLIOTECARIO_BIBLIOTECA1`
    FOREIGN KEY (`BIBLIOTECA_idBIBLIOTECA`)
    REFERENCES `BD_PROJECT`.`BIBLIOTECA` (`idBIBLIOTECA`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_BIBLIOTECARIO_UTILIZZATORI_SISTEMA1`
    FOREIGN KEY (`UTILIZZATORI_SISTEMA_ID`)
    REFERENCES `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 11
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`AMMINISTRATORE`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`AMMINISTRATORE` (
  `BIBLIOTECARIO_idBIBLIOTECARIO` INT(11) NOT NULL,
  `UTILIZZATORI_SISTEMA_ID` INT(11) NOT NULL,
  PRIMARY KEY (`BIBLIOTECARIO_idBIBLIOTECARIO`),
  INDEX `fk_AMMINISTRATORE_UTILIZZATORI_SISTEMA1_idx` (`UTILIZZATORI_SISTEMA_ID` ASC) VISIBLE,
  CONSTRAINT `fk_AMMINISTRATORE_BIBLIOTECARIO1`
    FOREIGN KEY (`BIBLIOTECARIO_idBIBLIOTECARIO`)
    REFERENCES `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_AMMINISTRATORE_UTILIZZATORI_SISTEMA1`
    FOREIGN KEY (`UTILIZZATORI_SISTEMA_ID`)
    REFERENCES `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`LIBRO`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`LIBRO` (
  `idLIBRO` INT(11) NOT NULL AUTO_INCREMENT,
  `Nome` VARCHAR(45) NOT NULL,
  `Autore` VARCHAR(45) NOT NULL,
  `Edizione` VARCHAR(10) NOT NULL,
  `Dismesso` TINYINT(1) NOT NULL,
  PRIMARY KEY (`idLIBRO`))
ENGINE = InnoDB
AUTO_INCREMENT = 51
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`COPIA`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`COPIA` (
  `idCOPIA` INT(11) NOT NULL AUTO_INCREMENT,
  `LIBRO_idLIBRO` INT(11) NOT NULL,
  PRIMARY KEY (`idCOPIA`),
  INDEX `fk_COPIA_LIBRO1_idx` (`LIBRO_idLIBRO` ASC) VISIBLE,
  CONSTRAINT `fk_COPIA_LIBRO1`
    FOREIGN KEY (`LIBRO_idLIBRO`)
    REFERENCES `BD_PROJECT`.`LIBRO` (`idLIBRO`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 101
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`COPIA_DISPONIBILE`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`COPIA_DISPONIBILE` (
  `Ripiano` INT(11) NOT NULL,
  `Scaffale` INT(11) NOT NULL,
  `COPIA_idCOPIA` INT(11) NOT NULL,
  PRIMARY KEY (`COPIA_idCOPIA`),
  CONSTRAINT `fk_COPIA DISPONIBILE_COPIA1`
    FOREIGN KEY (`COPIA_idCOPIA`)
    REFERENCES `BD_PROJECT`.`COPIA` (`idCOPIA`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`COPIA_IN_PRESTITO`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`COPIA_IN_PRESTITO` (
  `Data_inizio` DATE NOT NULL,
  `Periodo_consultazione` INT(11) NULL DEFAULT NULL,
  `COPIA_idCOPIA` INT(11) NOT NULL,
  PRIMARY KEY (`COPIA_idCOPIA`),
  CONSTRAINT `fk_COPIA IN PRESTITO_COPIA1`
    FOREIGN KEY (`COPIA_idCOPIA`)
    REFERENCES `BD_PROJECT`.`COPIA` (`idCOPIA`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`TURNO`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`TURNO` (
  `idTURNO` INT(11) NOT NULL AUTO_INCREMENT,
  `Ora_inizio` TIME NOT NULL,
  `Ora_fine` TIME NOT NULL,
  `Numero` INT(11) NOT NULL,
  `Giorno` VARCHAR(15) NOT NULL,
  `Mese` VARCHAR(15) NOT NULL,
  `Anno` INT(11) NOT NULL,
  `BIBLIOTECA_idBIBLIOTECA` INT(11) NOT NULL,
  `BIBLIOTECARIO_idBIBLIOTECARIO` INT(11) NOT NULL,
  `turno_Coperto` TINYINT(1) NOT NULL,
  PRIMARY KEY (`idTURNO`),
  INDEX `fk_TURNO_BIBLIOTECA1_idx` (`BIBLIOTECA_idBIBLIOTECA` ASC) VISIBLE,
  INDEX `fk_TURNO_BIBLIOTECARIO1_idx` (`BIBLIOTECARIO_idBIBLIOTECARIO` ASC) VISIBLE,
  CONSTRAINT `fk_TURNO_BIBLIOTECA1`
    FOREIGN KEY (`BIBLIOTECA_idBIBLIOTECA`)
    REFERENCES `BD_PROJECT`.`BIBLIOTECA` (`idBIBLIOTECA`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_TURNO_BIBLIOTECARIO1`
    FOREIGN KEY (`BIBLIOTECARIO_idBIBLIOTECARIO`)
    REFERENCES `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`COPIA_PRESTATA_AD_ALTRA_BIBLIOTECA`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`COPIA_PRESTATA_AD_ALTRA_BIBLIOTECA` (
  `COPIA_idCOPIA` INT(11) NOT NULL,
  `BIBLIOTECA_idBIBLIOTECA` INT(11) NOT NULL,
  `TURNO_idTURNO` INT(11) NOT NULL,
  PRIMARY KEY (`COPIA_idCOPIA`, `BIBLIOTECA_idBIBLIOTECA`, `TURNO_idTURNO`),
  INDEX `fk_COPIA PRESTATA AD ALTRA BIBLIOTECA_BIBLIOTECA1_idx` (`BIBLIOTECA_idBIBLIOTECA` ASC) VISIBLE,
  INDEX `fk_COPIA PRESTATA AD ALTRA BIBLIOTECA_TURNO1_idx` (`TURNO_idTURNO` ASC) VISIBLE,
  CONSTRAINT `fk_COPIA PRESTATA AD ALTRA BIBLIOTECA_BIBLIOTECA1`
    FOREIGN KEY (`BIBLIOTECA_idBIBLIOTECA`)
    REFERENCES `BD_PROJECT`.`BIBLIOTECA` (`idBIBLIOTECA`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_COPIA PRESTATA AD ALTRA BIBLIOTECA_COPIA1`
    FOREIGN KEY (`COPIA_idCOPIA`)
    REFERENCES `BD_PROJECT`.`COPIA` (`idCOPIA`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_COPIA PRESTATA AD ALTRA BIBLIOTECA_TURNO1`
    FOREIGN KEY (`TURNO_idTURNO`)
    REFERENCES `BD_PROJECT`.`TURNO` (`idTURNO`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`Dismettere`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`Dismettere` (
  `Data_dismissione` DATE NOT NULL,
  `AMMINISTRATORE_BIBLIOTECARIO_CF` INT(11) NOT NULL,
  `LIBRO_idLIBRO` INT(11) NOT NULL,
  PRIMARY KEY (`LIBRO_idLIBRO`),
  INDEX `fk_Dismettere_AMMINISTRATORE1_idx` (`AMMINISTRATORE_BIBLIOTECARIO_CF` ASC) VISIBLE,
  INDEX `fk_Dismettere_LIBRO1_idx` (`LIBRO_idLIBRO` ASC) VISIBLE,
  CONSTRAINT `fk_Dismettere_AMMINISTRATORE1`
    FOREIGN KEY (`AMMINISTRATORE_BIBLIOTECARIO_CF`)
    REFERENCES `BD_PROJECT`.`AMMINISTRATORE` (`BIBLIOTECARIO_idBIBLIOTECARIO`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Dismettere_LIBRO1`
    FOREIGN KEY (`LIBRO_idLIBRO`)
    REFERENCES `BD_PROJECT`.`LIBRO` (`idLIBRO`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`UTENTE`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`UTENTE` (
  `ID` INT(11) NOT NULL AUTO_INCREMENT,
  `Nome` VARCHAR(30) NOT NULL,
  `Cognome` VARCHAR(30) NOT NULL,
  `Data_nascita` DATE NOT NULL,
  `Sesso` VARCHAR(5) NOT NULL,
  `UTILIZZATORI_SISTEMA_ID` INT(11) NOT NULL,
  PRIMARY KEY (`ID`),
  INDEX `fk_UTENTE_UTILIZZATORI_SISTEMA1_idx` (`UTILIZZATORI_SISTEMA_ID` ASC) VISIBLE,
  CONSTRAINT `fk_UTENTE_UTILIZZATORI_SISTEMA1`
    FOREIGN KEY (`UTILIZZATORI_SISTEMA_ID`)
    REFERENCES `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`ISCRIVERSI`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`ISCRIVERSI` (
  `BIBLIOTECA_idBIBLIOTECA` INT(11) NOT NULL,
  `UTENTE_ID` INT(11) NOT NULL,
  `data_iscrizione` DATE NOT NULL,
  PRIMARY KEY (`BIBLIOTECA_idBIBLIOTECA`, `UTENTE_ID`),
  INDEX `fk_BIBLIOTECA_has_UTENTE_BIBLIOTECA1_idx` (`BIBLIOTECA_idBIBLIOTECA` ASC) VISIBLE,
  INDEX `fk_ISCRIVERSI_UTENTE1_idx` (`UTENTE_ID` ASC) VISIBLE,
  CONSTRAINT `fk_BIBLIOTECA_has_UTENTE_BIBLIOTECA1`
    FOREIGN KEY (`BIBLIOTECA_idBIBLIOTECA`)
    REFERENCES `BD_PROJECT`.`BIBLIOTECA` (`idBIBLIOTECA`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_ISCRIVERSI_UTENTE1`
    FOREIGN KEY (`UTENTE_ID`)
    REFERENCES `BD_PROJECT`.`UTENTE` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`ORARIO_SETTIMANALE`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`ORARIO_SETTIMANALE` (
  `idORARIO_SETTIMANALE` INT NOT NULL AUTO_INCREMENT,
  `Ora_inizio` TIME NOT NULL,
  `Ora_fine` TIME NOT NULL,
  `Giorno` VARCHAR(10) NOT NULL,
  `BIBLIOTECA_idBIBLIOTECA` INT(11) NOT NULL,
  PRIMARY KEY (`idORARIO_SETTIMANALE`),
  INDEX `fk_ORARIO_SETTIMANALE_BIBLIOTECA_idx` (`BIBLIOTECA_idBIBLIOTECA` ASC) VISIBLE,
  CONSTRAINT `fk_ORARIO_SETTIMANALE_BIBLIOTECA`
    FOREIGN KEY (`BIBLIOTECA_idBIBLIOTECA`)
    REFERENCES `BD_PROJECT`.`BIBLIOTECA` (`idBIBLIOTECA`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`POSSEDERE`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`POSSEDERE` (
  `COPIA_idCOPIA` INT(11) NOT NULL AUTO_INCREMENT,
  `BIBLIOTECA_idBIBLIOTECA` INT(11) NOT NULL,
  PRIMARY KEY (`COPIA_idCOPIA`, `BIBLIOTECA_idBIBLIOTECA`),
  INDEX `fk_COPIA_has_BIBLIOTECA_BIBLIOTECA1_idx` (`BIBLIOTECA_idBIBLIOTECA` ASC) VISIBLE,
  INDEX `fk_COPIA_has_BIBLIOTECA_COPIA1_idx` (`COPIA_idCOPIA` ASC) VISIBLE,
  CONSTRAINT `fk_COPIA_has_BIBLIOTECA_BIBLIOTECA1`
    FOREIGN KEY (`BIBLIOTECA_idBIBLIOTECA`)
    REFERENCES `BD_PROJECT`.`BIBLIOTECA` (`idBIBLIOTECA`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_COPIA_has_BIBLIOTECA_COPIA1`
    FOREIGN KEY (`COPIA_idCOPIA`)
    REFERENCES `BD_PROJECT`.`COPIA` (`idCOPIA`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
AUTO_INCREMENT = 101
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`PRESTITO`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`PRESTITO` (
  `idPRESTITO` INT(11) NOT NULL AUTO_INCREMENT,
  `UTENTE_ID` INT(11) NOT NULL,
  `COPIA_idCOPIA` INT(11) NOT NULL,
  `TURNO_idTURNO` INT(11) NULL,
  `Data_prestito` DATE NOT NULL,
  `Terminato` INT NOT NULL,
  PRIMARY KEY (`idPRESTITO`),
  INDEX `fk_PRESTITO_COPIA1_idx` (`COPIA_idCOPIA` ASC) VISIBLE,
  INDEX `fk_PRESTITO_TURNO1_idx` (`TURNO_idTURNO` ASC) VISIBLE,
  INDEX `fk_PRESTITO_UTENTE1_idx` (`UTENTE_ID` ASC) VISIBLE,
  CONSTRAINT `fk_PRESTITO_COPIA1`
    FOREIGN KEY (`COPIA_idCOPIA`)
    REFERENCES `BD_PROJECT`.`COPIA` (`idCOPIA`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_PRESTITO_TURNO1`
    FOREIGN KEY (`TURNO_idTURNO`)
    REFERENCES `BD_PROJECT`.`TURNO` (`idTURNO`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_PRESTITO_UTENTE1`
    FOREIGN KEY (`UTENTE_ID`)
    REFERENCES `BD_PROJECT`.`UTENTE` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`RESTITUIRE`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`RESTITUIRE` (
  `Penale` FLOAT NOT NULL,
  `Data_restituzione` DATE NOT NULL,
  `UTENTE_ID` INT(11) NOT NULL,
  `COPIA_idCOPIA` INT(11) NOT NULL,
  PRIMARY KEY (`Data_restituzione`, `UTENTE_ID`, `COPIA_idCOPIA`),
  INDEX `fk_RESTITUIRE_COPIA1_idx` (`COPIA_idCOPIA` ASC) VISIBLE,
  INDEX `fk_RESTITUIRE_UTENTE1_idx` (`UTENTE_ID` ASC) VISIBLE,
  CONSTRAINT `fk_RESTITUIRE_COPIA1`
    FOREIGN KEY (`COPIA_idCOPIA`)
    REFERENCES `BD_PROJECT`.`COPIA` (`idCOPIA`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_RESTITUIRE_UTENTE1`
    FOREIGN KEY (`UTENTE_ID`)
    REFERENCES `BD_PROJECT`.`UTENTE` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`Recapito`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`Recapito` (
  `Tipo` INT(11) NOT NULL,
  `Descrizione` VARCHAR(45) NOT NULL,
  `UTENTE_ID` INT(11) NOT NULL,
  `RecapitoPreferito` TINYINT(4) NOT NULL,
  PRIMARY KEY (`Tipo`, `UTENTE_ID`),
  INDEX `fk_Recapito_UTENTE1_idx` (`UTENTE_ID` ASC) VISIBLE,
  CONSTRAINT `fk_Recapito_UTENTE1`
    FOREIGN KEY (`UTENTE_ID`)
    REFERENCES `BD_PROJECT`.`UTENTE` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `BD_PROJECT`.`TURNO_RICOPERTO`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `BD_PROJECT`.`TURNO_RICOPERTO` (
  `Motivo` VARCHAR(50) NOT NULL,
  `TURNO_idTURNO` INT(11) NOT NULL,
  `BIBLIOTECARIO_idBIBLIOTECARIO` INT(11) NOT NULL,
  PRIMARY KEY (`TURNO_idTURNO`),
  INDEX `fk_TURNO SCOPERTO_BIBLIOTECARIO1_idx` (`BIBLIOTECARIO_idBIBLIOTECARIO` ASC) VISIBLE,
  CONSTRAINT `fk_TURNO SCOPERTO_BIBLIOTECARIO1`
    FOREIGN KEY (`BIBLIOTECARIO_idBIBLIOTECARIO`)
    REFERENCES `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_TURNO SCOPERTO_TURNO1`
    FOREIGN KEY (`TURNO_idTURNO`)
    REFERENCES `BD_PROJECT`.`TURNO` (`idTURNO`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

USE `BD_PROJECT` ;

-- -----------------------------------------------------
-- procedure aggiungi_user_proc
-- -----------------------------------------------------

DELIMITER $$
USE `BD_PROJECT`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `aggiungi_user_proc`(IN var_nome VARCHAR(30), IN var_cognome varchar(30), IN var_birth date, IN var_sex varchar(5), IN var_username int, out var_id int)
BEGIN
    insert into `UTENTE` (`Nome`, `Cognome`, `Data_nascita`, `Sesso`, `UTILIZZATORI_SISTEMA_ID`) values (var_nome, var_cognome, var_birth, var_sex, var_username);
    set var_id = last_insert_id();
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure default_population_proc
-- -----------------------------------------------------

DELIMITER $$
USE `BD_PROJECT`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `default_population_proc`()
BEGIN

		declare exit handler for sqlexception
	begin
		rollback; -- rollback any changes made in the transaction
        resignal; -- raise again the sql exception to the caller
	end;
    
		
        START TRANSACTION;
                  
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (1,'Il piccolo principe','Antoine de Saint-Exupéry','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (2,'Orgoglio e pregiudizio','Jane Austen','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (3,'Il signore degli anelli','J.R.R Tolkien','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (4,'Se questo è un uomo','Primo Levi','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (5,'1984','George Orwell','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (6,'I promessi sposi','Alessandro Manzoni','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (7,'La divina commedia','Dante Alighieri','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (8,'Il nome della rosa','Umberto Eco','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (9,'Le affinità elettive','Goethe','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (10,'Il sentiero dei nidi di ragno','Italo Calvino','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (11,'La metamorfosi','Franz Kafka','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (12,'Lo Hobbit','J.R.R Tolkien','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (13,'Il grande Gatsby','F. Scott Fitzgerald','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (14,'Guerra e pace','Leo Tolstoy','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (15,'Walden','Henry David Thoreau','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (16,'Delitto e Castigo','Fyodor Dostoyevsky','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (17,'Anna Karenina','Leo Tolstoy','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (18,'Odissea','Omero','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (19,'La coscienza di Zeno','Italo Svevo','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (20,'Il cacciatore di aquiloni','Khaled Hosseini','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (21,'Il codice Da Vinci','Dan Brown','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (22,'Il barone rampante','Italo Calvino','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (23,'I malavoglia','Giovanni Verga','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (24,'Il fu Mattia Pascal','Luigi Pirandello','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (25,'Il vecchio e il mare','Hernest Hemingway','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (26,'I dolori del giovane Werther','Goethe','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (27,'Pinocchio','Carlo Collodi','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (28,'Amore ai tempi del colera','Gabriel Garcia Marquez','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (29,'Il conte di Montecristo','Alexandre Dumas','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (30,'Oliver Twist','Charles Dickens','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (31,'Il ritratto di Dorian Gray','Oscar Wilde','prima',0);
			insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (32,'Madame Bouvary','Gustave Flaubert','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (33,'Un uomo','Oriana Fallaci','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (34,'Amleto','William Shakespeare','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (35,'I miserabili','Victor Hugo','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (36,'I fratelli Karamazov','Fyodor Dostoyevsky','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (37,'La storia infinita','Micheal Ende','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (38,'Il giovane Holden','J.D Salinger','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (39,'Macbeth','William Shakespeare','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (40,'Alla ricerca del tempo perduto','Marcel Proust','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (41,'Venti mila leghe sotto i mari','Jules Verne','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (42,'Il gattopardo','Tomasi di Lampedusa','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (43,'Via col vento','Margaret Mitchell','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (44,'I pilastri della terra','Ken Follet','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (45,'Il buio oltre la siepe','Harper Lee','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (46,'Hunger Games','Suzanne Collins','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (47,'Il trono di spade','George R.R. Martin','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (48,'Io non ho paura','Niccolo Ammaniti','prima',0);
            insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (49,'Decamerone','Giovanni Boccaccio','prima',0);
			insert into `BD_PROJECT`.`LIBRO` (`idLIBRO`,`Nome`,`Autore`,`Edizione`,`Dismesso`) values (50,'Cime tempestose','Emily Bronte','prima',0);
        COMMIT;
        
        
       START TRANSACTION;
			
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (1,1);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (2,1);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (3,2);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (4,2);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (5,3);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (6,3);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (7,4);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (8,4);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (9,5);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (10,5);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (11,6);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (12,6);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (13,7);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (14,7);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (15,8);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (16,8);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (17,9);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (18,9);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (19,10);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (20,10);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (21,11);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (22,11);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (23,12);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (24,12);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (25,13);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (26,13);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (27,14);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (28,14);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (29,15);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (30,15);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (31,16);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (32,16);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (33,17);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (34,17);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (35,18);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (36,18);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (37,19);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (38,19);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (39,20);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (40,20);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (41,21);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (42,21);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (43,22);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (44,22);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (45,23);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (46,23);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (47,24);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (48,24);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (49,25);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (50,25);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (51,26);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (52,26);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (53,26);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (54,27);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (55,27);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (56,28);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (57,28);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (58,29);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (59,29);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (60,30);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (61,30);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (62,31);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (63,31);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (64,32);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (65,32);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (66,33);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (67,33);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (68,34);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (69,34);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (70,35);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (71,35);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (72,36);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (73,36);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (74,37);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (75,37);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (76,38);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (77,38);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (78,39);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (79,39);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (80,40);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (81,40);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (82,41);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (83,41);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (84,42);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (85,42);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (86,43);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (87,43);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (88,44);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (89,44);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (90,45);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (91,45);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (92,46);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (93,46);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (94,47);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (95,47);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (96,48);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (97,48);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (98,49);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (99,49);
            insert into `BD_PROJECT`.`COPIA` (`idCOPIA`,`LIBRO_idLIBRO`) values (100,50);

            
        COMMIT;

        
        START TRANSACTION;
			
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,1);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,2);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,3);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,4);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,5);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,6);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,7);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,8);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,9);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,10);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,11);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,12);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,13);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,14);
			insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,15);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,16);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,17);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,18);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,19);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,20);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,21);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,22);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,23);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,24);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,25);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,26);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,27);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,28);
			insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,29);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,30);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,31);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,32);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,33);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,34);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,35);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,36);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,37);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,38);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,39);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,40);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,41);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,42);
			insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,43);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,44);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,45);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,46);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,47);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,48);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,49);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,50);
			insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,51);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,52);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,53);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,54);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,55);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,56);
			insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,57);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,58);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,59);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,60);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,61);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,62);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,63);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,64);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,65);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,66);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,67);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,68);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,69);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,70);
			insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,71);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,72);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,73);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,74);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,75);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,76);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,77);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,78);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,79);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,80);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,81);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,82);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,83);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,84);
			insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,85);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,86);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,87);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,88);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,89);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,90);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,91);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,92);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,93);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,94);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,95);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (1,1,96);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (2,1,97);
            insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (3,1,98);
			insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (4,1,99);
			insert into `BD_PROJECT`.`COPIA_DISPONIBILE` (`Ripiano`, `Scaffale`, `COPIA_idCOPIA`) values (5,1,100);

        COMMIT;
        
        START TRANSACTION;
        
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('Valerio23','valerio23','amministratore', 1);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('Alessio97','alessio97','amministratore', 2);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('Matteo97','matteo97','amministratore', 3);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('Samuele33','samuele33','amministratore', 4);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('Giada56','giada56','amministratore', 5);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('Ludovico66','ludovico66','bibliotecario', 6);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('Daniele24','daniele24','bibliotecario', 7);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('Martina34','martina34','bibliotecario', 8);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('Alessandro99','alessandro99','bibliotecario', 9);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('Giulia6','giulia6','bibliotecario', 10);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('Paolo3','paolo3','bibliotecario', 11);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('John4','john4','bibliotecario', 12);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('JimMorrison','jimmorrison','bibliotecario', 13);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('Alessandro13','alessandro13','bibliotecario', 14);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('Ricardo22','ricardo22','bibliotecario', 15);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('LordVoldy','voldy666','bibliotecario', 16);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('HarryP','harryP','bibliotecario', 17);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('Hermione123','hermione123','bibliotecario', 18);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('JonSnow','jonSnow','bibliotecario', 19);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('Tyrion11','tyrion11','bibliotecario', 20);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('PietroSav','pietroSav','bibliotecario', 21);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('SalvoConte','salvoConte','bibliotecario', 22);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('GennySav','gennySav','bibliotecario', 23);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('ElliotAlderson,','elliotAlderson','bibliotecario', 24);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('WalterWhite','walterWhite','bibliotecario', 25);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('GusFring','gusFring','bibliotecario', 26);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('JackSparrow','Jackie55','bibliotecario', 27);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('ForrestGump','forrestGump','bibliotecario', 28);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('JimiHendrix','jimiHen','bibliotecario', 29);
			insert into `BD_PROJECT`.`UTILIZZATORI_SISTEMA` (`username`, `password`, `ruolo`, `ID`) values ('PippoBaudo','pippoBaudo','bibliotecario', 30);
			
        COMMIT;
        
        START TRANSACTION;
        
			insert into `BD_PROJECT`.`BIBLIOTECA` (`idBIBLIOTECA`, `Telefono`, `Via`, `Civico`) values (1,'1234567891', 'Via bella', 1);
			insert into `BD_PROJECT`.`BIBLIOTECA` (`idBIBLIOTECA`, `Telefono`, `Via`, `Civico`) values (2,'2234567891', 'Via brutta', 1);
			insert into `BD_PROJECT`.`BIBLIOTECA` (`idBIBLIOTECA`, `Telefono`, `Via`, `Civico`) values (3,'3234567891', 'Via alta', 1);
			insert into `BD_PROJECT`.`BIBLIOTECA` (`idBIBLIOTECA`, `Telefono`, `Via`, `Civico`) values (4,'4234567891', 'Via bassa', 1);
			insert into `BD_PROJECT`.`BIBLIOTECA` (`idBIBLIOTECA`, `Telefono`, `Via`, `Civico`) values (5,'5234567891', 'Via gluck', 1);
			
        COMMIT;
        
        
         
        
        
        START TRANSACTION;
        
			insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (1,1);
			insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (2,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (3,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (4,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (5,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (6,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (7,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (8,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (9,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (10,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (11,2);
			insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (12,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (13,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (14,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (15,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (16,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (17,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (18,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (19,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (20,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (21,3);
			insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (22,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (23,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (24,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (25,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (26,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (27,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (28,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (29,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (30,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (31,4);
			insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (32,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (33,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (34,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (35,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (36,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (37,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (38,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (39,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (40,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (41,5);
			insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (42,5);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (43,5);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (44,5);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (45,5);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (46,5);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (47,5);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (48,5);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (49,5);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (50,5);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (51,1);
			insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (52,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (53,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (54,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (55,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (56,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (57,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (58,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (59,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (60,1);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (61,2);
			insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (62,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (63,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (64,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (65,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (66,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (67,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (68,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (69,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (70,2);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (71,3);
			insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (72,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (73,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (74,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (75,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (76,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (77,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (78,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (79,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (80,3);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (81,4);
			insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (82,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (83,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (84,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (85,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (86,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (87,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (88,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (89,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (90,4);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (91,5);
			insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (92,5);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (93,5);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (94,5);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (95,5);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (96,5);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (97,5);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (98,5);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (99,5);
            insert into `BD_PROJECT`.`POSSEDERE` (`COPIA_idCOPIA`,`BIBLIOTECA_idBIBLIOTECA`) values (100,5);

            
        COMMIT;
        
        START TRANSACTION;
                  
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (1,'Valerio', 'Crecco', '1997-05-23', 'ingegnere informatico', 1, 1);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (2,'Alessio', 'Rossi', '1997-06-27', 'ingegnere gestionale', 2, 2);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (3,'Matteo', 'Verdi', '1997-04-28', 'ingegnere meccanico', 3, 3);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (4,'Samuele', 'Bianchi', '1998-05-21', 'ingegnere elettronico', 4, 4);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (5,'Giada', 'Neri', '1997-05-27', 'ingegnere medico', 5, 5);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (6,'Ludovico', 'Gialli', '1999-04-29', 'ingegnere informatico', 1, 6);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (7,'Daniele', 'Arancioni', '1994-06-24', 'ingegnere gestionale', 2, 7);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (8,'Martina', 'Viola', '1997-02-12', 'ingegnere energetico', 3, 8);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (9,'Alessandro', 'Grigi', '1991-04-21', 'ingegnere edile', 4, 9);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (10,'Giulia', 'Blu', '1997-05-11', 'ingegnere informatico', 5, 10);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (11,'Paolo', 'Maldini', '1977-05-23', 'ingegnere informatico', 1, 11);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (12,'John', 'Lennon', '1956-05-23', 'ingegnere elettronico', 2, 12);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (13,'Jim', 'Morrison', '1950-05-23', 'ingegnere informatico', 3, 13);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (14,'Alessandro', 'Nesta', '1983-05-23', 'ingegnere meccanico', 4, 14);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (15,'Ricardo', 'Kaka', '1985-05-23', 'ingegnere informatico', 5, 15);
			insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (16,'Tom', 'Riddle', '1997-05-23', 'ingegnere informatico', 1, 16);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (17,'Harry', 'Potter', '2002-05-05', 'ingegnere informatico', 2, 17);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (18,'Hermione', 'Granger', '2003-05-23', 'ingegnere informatico', 3, 18);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (19,'Jon', 'Snow', '1994-05-23', 'ingegnere informatico', 4, 19);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (20,'Tyrion', 'Lannister', '1987-05-23', 'ingegnere informatico', 5, 20);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (21,'Pietro', 'Savastano', '1977-05-23', 'ingegnere informatico', 1, 21);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (22,'Salvatore', 'Conte', '1987-05-23', 'ingegnere informatico', 2, 22);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (23,'Gennaro', 'Savastano', '1995-05-23', 'ingegnere informatico', 3, 23);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (24,'Elliot', 'Alderson', '1997-05-23', 'ingegnere informatico', 4, 24);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (25,'Walter', 'White', '1966-05-23', 'ingegnere informatico', 5, 25);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (26,'Gus', 'Fring', '1970-05-23', 'ingegnere informatico', 1, 26);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (27,'Jack', 'Sparrow', '1982-05-23', 'ingegnere informatico', 2, 27);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (28,'Forrest', 'Gump', '1955-05-23', 'ingegnere informatico', 3, 28);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (29,'Jimi', 'Hendrix', '1945-05-23', 'ingegnere informatico', 4, 29);
            insert into `BD_PROJECT`.`BIBLIOTECARIO` (`idBibliotecario`,`Nome`,`Cognome`,`Data_nascita`,`Titolo_di_studio`,`BIBLIOTECA_idBIBLIOTECA`,`UTILIZZATORI_SISTEMA_ID`) values (30,'Pippo', 'Baudo', '1930-05-23', 'ingegnere informatico', 5, 30);
        
        COMMIT;
		

    
		START TRANSACTION;
                  
            insert into `BD_PROJECT`.`AMMINISTRATORE` (`BIBLIOTECARIO_idBIBLIOTECARIO`,`UTILIZZATORI_SISTEMA_ID`) values (1,1);
			insert into `BD_PROJECT`.`AMMINISTRATORE` (`BIBLIOTECARIO_idBIBLIOTECARIO`,`UTILIZZATORI_SISTEMA_ID`) values (2,2);
            insert into `BD_PROJECT`.`AMMINISTRATORE` (`BIBLIOTECARIO_idBIBLIOTECARIO`,`UTILIZZATORI_SISTEMA_ID`) values (3,3);
            insert into `BD_PROJECT`.`AMMINISTRATORE` (`BIBLIOTECARIO_idBIBLIOTECARIO`,`UTILIZZATORI_SISTEMA_ID`) values (4,4);
            insert into `BD_PROJECT`.`AMMINISTRATORE` (`BIBLIOTECARIO_idBIBLIOTECARIO`,`UTILIZZATORI_SISTEMA_ID`) values (5,5);
            

       COMMIT;
        
        
        
        
        
        
       
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure login_proc
-- -----------------------------------------------------

DELIMITER $$
USE `BD_PROJECT`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `login_proc`(in var_username varchar(45), in var_pass varchar(45), out var_role INT)
BEGIN
	declare var_user_role ENUM('amministratore', 'bibliotecario', 'utente');
    
	select `ruolo` from `UTILIZZATORI_SISTEMA`
		where `username` = var_username
        and `password` = var_pass
        into var_user_role;
        
        -- See the corresponding enum in the client
		if var_user_role = 'amministratore' then
			set var_role = 1;
		elseif var_user_role = 'bibliotecario' then
			set var_role = 2;
		elseif var_user_role = 'utente' then
			set var_role = 3;
		else
			set var_role = 4;
		end if;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure default_turni_proc
-- -----------------------------------------------------

DELIMITER $$
USE `BD_PROJECT`$$
CREATE PROCEDURE `default_turni_proc` ()
BEGIN
		
	declare exit handler for sqlexception
	begin
		rollback; -- rollback any changes made in the transaction
        resignal; -- raise again the sql exception to the caller
	end;
    
		
        START TRANSACTION;
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (1, '09:00:00', '13:00:00', 1, 'martedi', 'settembre', 2020, 1, 1, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (2, '13:00:00', '17:00:00', 1, 'martedi', 'settembre', 2020, 1, 6, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (3, '09:00:00', '13:00:00', 2, 'mercoledi', 'settembre', 2020, 1, 11, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (4, '13:00:00', '17:00:00', 2, 'mercoledi', 'settembre', 2020, 1, 16, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (5, '09:00:00', '13:00:00', 3, 'giovedi', 'settembre', 2020, 1, 21, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (6, '13:00:00', '17:00:00', 3, 'giovedi', 'settembre', 2020, 1, 26, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (7, '09:00:00', '13:00:00', 4, 'venerdi', 'settembre', 2020, 1, 1, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (8, '13:00:00', '17:00:00', 4, 'venerdi', 'settembre', 2020, 1, 6, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (9, '09:00:00', '13:00:00', 7, 'lunedi', 'settembre', 2020, 1, 11, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (10, '13:00:00', '17:00:00', 7, 'lunedi', 'settembre', 2020, 1, 16, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (11, '09:00:00', '13:00:00', 8, 'martedi', 'settembre', 2020, 1, 21, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (12, '13:00:00', '17:00:00', 8, 'martedi', 'settembre', 2020, 1, 16, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (13, '09:00:00', '13:00:00', 9, 'mercoledi', 'settembre', 2020, 1, 1, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (14, '13:00:00', '17:00:00', 9, 'mercoledi', 'settembre', 2020, 1, 6, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (15, '09:00:00', '13:00:00', 10, 'giovedi', 'settembre', 2020, 1, 11, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (16, '13:00:00', '17:00:00', 10, 'giovedi', 'settembre', 2020, 1, 16, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (17, '09:00:00', '13:00:00', 11, 'venerdi', 'settembre', 2020, 1, 21, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (18, '13:00:00', '17:00:00', 11, 'venerdi', 'settembre', 2020, 1, 26, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (19, '09:00:00', '13:00:00', 14, 'lunedi', 'settembre', 2020, 1, 1, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (20, '13:00:00', '17:00:00', 14, 'lunedi', 'settembre', 2020, 1, 6, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (21, '09:00:00', '13:00:00', 15, 'martedi', 'settembre', 2020, 1, 11, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (22, '13:00:00', '17:00:00', 15, 'martedi', 'settembre', 2020, 1, 16, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (23, '09:00:00', '13:00:00', 16, 'mercoledi', 'settembre', 2020, 1, 21, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (24, '13:00:00', '17:00:00', 16, 'mercoledi', 'settembre', 2020, 1, 26, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (25, '09:00:00', '13:00:00', 17, 'giovedi', 'settembre', 2020, 1, 1, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (26, '13:00:00', '17:00:00', 17, 'giovedi', 'settembre', 2020, 1, 6, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (27, '09:00:00', '13:00:00', 18, 'venerdi', 'settembre', 2020, 1, 11, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (28, '13:00:00', '17:00:00', 18, 'venerdi', 'settembre', 2020, 1, 16, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (29, '09:00:00', '13:00:00', 21, 'lunedi', 'settembre', 2020, 1, 21, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (30, '13:00:00', '17:00:00', 21, 'lunedi', 'settembre', 2020, 1, 16, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (31, '09:00:00', '13:00:00', 22, 'martedi', 'settembre', 2020, 1, 1, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (32, '13:00:00', '17:00:00', 22, 'martedi', 'settembre', 2020, 1, 6, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (33, '09:00:00', '13:00:00', 23, 'mercoledi', 'settembre', 2020, 1, 11, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (34, '13:00:00', '17:00:00', 23, 'mercoledi', 'settembre', 2020, 1, 16, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (35, '09:00:00', '13:00:00', 24, 'giovedi', 'settembre', 2020, 1, 21, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (36, '13:00:00', '17:00:00', 24, 'giovedi', 'settembre', 2020, 1, 26, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (37, '09:00:00', '13:00:00', 25, 'venerdi', 'settembre', 2020, 1, 1, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (38, '13:00:00', '17:00:00', 25, 'venerdi', 'settembre', 2020, 1, 6, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (39, '09:00:00', '13:00:00', 28, 'lunedi', 'settembre', 2020, 1, 11, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (40, '13:00:00', '17:00:00', 28, 'lunedi', 'settembre', 2020, 1, 16, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (41, '09:00:00', '13:00:00', 29, 'martedi', 'settembre', 2020, 1, 21, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (42, '13:00:00', '17:00:00', 29, 'martedi', 'settembre', 2020, 1, 26, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (43, '09:00:00', '13:00:00', 30, 'mercoledi', 'settembre', 2020, 1, 1, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (44, '13:00:00', '17:00:00', 30, 'mercoledi', 'settembre', 2020, 1, 6, 1);
				
                
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (45, '09:00:00', '17:00:00', 1, 'martedi', 'settembre', 2020, 2, 2, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (46, '13:00:00', '17:00:00', 1, 'martedi', 'settembre', 2020, 2, 7, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (47, '09:00:00', '13:00:00', 2, 'mercoledi', 'settembre', 2020, 2, 12, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (48, '13:00:00', '17:00:00', 2, 'mercoledi', 'settembre', 2020, 2, 17, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (49, '09:00:00', '13:00:00', 3, 'giovedi', 'settembre', 2020, 2, 22, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (50, '13:00:00', '17:00:00', 3, 'giovedi', 'settembre', 2020, 2, 27, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (51, '09:00:00', '13:00:00', 4, 'venerdi', 'settembre', 2020, 2, 2, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (52, '13:00:00', '17:00:00', 4, 'venerdi', 'settembre', 2020, 2, 7, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (53, '09:00:00', '13:00:00', 7, 'lunedi', 'settembre', 2020, 2, 12, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (54, '13:00:00', '17:00:00', 7, 'lunedi', 'settembre', 2020, 2, 17, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (55, '09:00:00', '13:00:00', 8, 'martedi', 'settembre', 2020, 2, 22, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (56, '13:00:00', '17:00:00', 8, 'martedi', 'settembre', 2020, 2, 27, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (57, '09:00:00', '13:00:00', 9, 'mercoledi', 'settembre', 2020, 2, 2, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (58, '13:00:00', '17:00:00', 9, 'mercoledi', 'settembre', 2020, 2, 7, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (59, '09:00:00', '13:00:00', 10, 'giovedi', 'settembre', 2020, 2, 12, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (60, '13:00:00', '17:00:00', 10, 'giovedi', 'settembre', 2020, 2, 17, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (61, '09:00:00', '13:00:00', 11, 'venerdi', 'settembre', 2020, 2, 22, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (62, '13:00:00', '17:00:00', 11, 'venerdi', 'settembre', 2020, 2, 27, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (63, '09:00:00', '13:00:00', 14, 'lunedi', 'settembre', 2020, 2, 2, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (64, '13:00:00', '17:00:00', 14, 'lunedi', 'settembre', 2020, 2, 7, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (65, '09:00:00', '13:00:00', 15, 'martedi', 'settembre', 2020, 2, 12, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (66, '13:00:00', '17:00:00', 15, 'martedi', 'settembre', 2020, 2, 17, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (67, '09:00:00', '13:00:00', 16, 'mercoledi', 'settembre', 2020, 2, 22, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (68, '13:00:00', '17:00:00', 16, 'mercoledi', 'settembre', 2020, 2, 27, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (69, '09:00:00', '13:00:00', 17, 'giovedi', 'settembre', 2020, 2, 2, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (70, '13:00:00', '17:00:00', 17, 'giovedi', 'settembre', 2020, 2, 7, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (71, '09:00:00', '13:00:00', 18, 'venerdi', 'settembre', 2020, 2, 12, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (72, '13:00:00', '17:00:00', 18, 'venerdi', 'settembre', 2020, 2, 17, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (73, '09:00:00', '13:00:00', 21, 'lunedi', 'settembre', 2020, 2, 22, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (74, '13:00:00', '17:00:00', 21, 'lunedi', 'settembre', 2020, 2, 27, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (75, '09:00:00', '13:00:00', 22, 'martedi', 'settembre', 2020, 2, 2, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (76, '13:00:00', '17:00:00', 22, 'martedi', 'settembre', 2020, 2, 7, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (77, '09:00:00', '13:00:00', 23, 'mercoledi', 'settembre', 2020, 2, 12, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (78, '13:00:00', '17:00:00', 23, 'mercoledi', 'settembre', 2020, 2, 17, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (79, '09:00:00', '13:00:00', 24, 'giovedi', 'settembre', 2020, 2, 22, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (80, '13:00:00', '17:00:00', 24, 'giovedi', 'settembre', 2020, 2, 27, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (81, '09:00:00', '13:00:00', 25, 'venerdi', 'settembre', 2020, 2, 2, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (82, '13:00:00', '17:00:00', 25, 'venerdi', 'settembre', 2020, 2, 7, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (83, '09:00:00', '13:00:00', 28, 'lunedi', 'settembre', 2020, 2, 12, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (84, '13:00:00', '17:00:00', 28, 'lunedi', 'settembre', 2020, 2, 17, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (85, '09:00:00', '13:00:00', 29, 'martedi', 'settembre', 2020, 2, 22, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (86, '13:00:00', '17:00:00', 29, 'martedi', 'settembre', 2020, 2, 27, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (87, '09:00:00', '13:00:00', 30, 'mercoledi', 'settembre', 2020, 2, 2, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (88, '13:00:00', '17:00:00', 30, 'mercoledi', 'settembre', 2020, 2, 7, 1);
				
                
                
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (89, '09:00:00', '17:00:00', 1, 'martedi', 'settembre', 2020, 3, 3, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (90, '13:00:00', '17:00:00', 1, 'martedi', 'settembre', 2020, 3, 8, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (91, '09:00:00', '13:00:00', 2, 'mercoledi', 'settembre', 2020, 3, 13, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (92, '13:00:00', '17:00:00', 2, 'mercoledi', 'settembre', 2020, 3, 18, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (93, '09:00:00', '13:00:00', 3, 'giovedi', 'settembre', 2020, 3, 23, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (94, '13:00:00', '17:00:00', 3, 'giovedi', 'settembre', 2020, 3, 28, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (95, '09:00:00', '13:00:00', 4, 'venerdi', 'settembre', 2020, 3, 3, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (96, '13:00:00', '17:00:00', 4, 'venerdi', 'settembre', 2020, 3, 8, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (97, '09:00:00', '13:00:00', 7, 'lunedi', 'settembre', 2020, 3, 13, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (98, '13:00:00', '17:00:00', 7, 'lunedi', 'settembre', 2020, 3, 18, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (99, '09:00:00', '13:00:00', 8, 'martedi', 'settembre', 2020, 3, 23, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (100, '13:00:00', '17:00:00', 8, 'martedi', 'settembre', 2020, 3, 28, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (101, '09:00:00', '13:00:00', 9, 'mercoledi', 'settembre', 2020, 3, 3, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (102, '13:00:00', '17:00:00', 9, 'mercoledi', 'settembre', 2020, 3, 8, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (103, '09:00:00', '13:00:00', 10, 'giovedi', 'settembre', 2020, 3, 13, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (104, '13:00:00', '17:00:00', 10, 'giovedi', 'settembre', 2020, 3, 18, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (105, '09:00:00', '13:00:00', 11, 'venerdi', 'settembre', 2020, 3, 23, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (106, '13:00:00', '17:00:00', 11, 'venerdi', 'settembre', 2020, 3, 28, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (107, '09:00:00', '13:00:00', 14, 'lunedi', 'settembre', 2020, 3, 3, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (108, '13:00:00', '17:00:00', 14, 'lunedi', 'settembre', 2020, 3, 8, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (109, '09:00:00', '13:00:00', 15, 'martedi', 'settembre', 2020, 3, 13, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (110, '13:00:00', '17:00:00', 15, 'martedi', 'settembre', 2020, 3, 18, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (111, '09:00:00', '13:00:00', 16, 'mercoledi', 'settembre', 2020, 3, 23, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (112, '13:00:00', '17:00:00', 16, 'mercoledi', 'settembre', 2020, 3, 28, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (113, '09:00:00', '13:00:00', 17, 'giovedi', 'settembre', 2020, 3, 3, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (114, '13:00:00', '17:00:00', 17, 'giovedi', 'settembre', 2020, 3, 8, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (115, '09:00:00', '13:00:00', 18, 'venerdi', 'settembre', 2020, 3, 13, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (116, '13:00:00', '17:00:00', 18, 'venerdi', 'settembre', 2020, 3, 18, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (117, '09:00:00', '13:00:00', 21, 'lunedi', 'settembre', 2020, 3, 23, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (118, '13:00:00', '17:00:00', 21, 'lunedi', 'settembre', 2020, 3, 28, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (119, '09:00:00', '13:00:00', 22, 'martedi', 'settembre', 2020, 3, 3, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (120, '13:00:00', '17:00:00', 22, 'martedi', 'settembre', 2020, 3, 8, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (121, '09:00:00', '13:00:00', 23, 'mercoledi', 'settembre', 2020, 3, 13, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (122, '13:00:00', '17:00:00', 23, 'mercoledi', 'settembre', 2020, 3, 18, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (123, '09:00:00', '13:00:00', 24, 'giovedi', 'settembre', 2020, 3, 23, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (124, '13:00:00', '17:00:00', 24, 'giovedi', 'settembre', 2020, 3, 28, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (125, '09:00:00', '13:00:00', 25, 'venerdi', 'settembre', 2020, 3, 3, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (126, '13:00:00', '17:00:00', 25, 'venerdi', 'settembre', 2020, 3, 8, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (127, '09:00:00', '13:00:00', 28, 'lunedi', 'settembre', 2020, 3, 13, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (128, '13:00:00', '17:00:00', 28, 'lunedi', 'settembre', 2020, 3, 18, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (129, '09:00:00', '13:00:00', 29, 'martedi', 'settembre', 2020, 3, 23, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (130, '13:00:00', '17:00:00', 29, 'martedi', 'settembre', 2020, 3, 28, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (131, '09:00:00', '13:00:00', 30, 'mercoledi', 'settembre', 2020, 3, 3, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (132, '13:00:00', '17:00:00', 30, 'mercoledi', 'settembre', 2020, 3, 8, 1);
				
                
                
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (133, '09:00:00', '17:00:00', 1, 'martedi', 'settembre', 2020, 4, 4, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (134, '13:00:00', '17:00:00', 1, 'martedi', 'settembre', 2020, 4, 9, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (135, '09:00:00', '13:00:00', 2, 'mercoledi', 'settembre', 2020, 4, 14, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (136, '13:00:00', '17:00:00', 2, 'mercoledi', 'settembre', 2020, 4, 19, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (137, '09:00:00', '13:00:00', 3, 'giovedi', 'settembre', 2020, 4, 24, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (138, '13:00:00', '17:00:00', 3, 'giovedi', 'settembre', 2020, 4, 29, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (139, '09:00:00', '13:00:00', 4, 'venerdi', 'settembre', 2020, 4, 4, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (140, '13:00:00', '17:00:00', 4, 'venerdi', 'settembre', 2020, 4, 9, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (141, '09:00:00', '13:00:00', 7, 'lunedi', 'settembre', 2020, 4, 14, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (142, '13:00:00', '17:00:00', 7, 'lunedi', 'settembre', 2020, 4, 19, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (143, '09:00:00', '13:00:00', 8, 'martedi', 'settembre', 2020, 4, 24, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (144, '13:00:00', '17:00:00', 8, 'martedi', 'settembre', 2020, 4, 29, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (145, '09:00:00', '13:00:00', 9, 'mercoledi', 'settembre', 2020, 4, 4, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (146, '13:00:00', '17:00:00', 9, 'mercoledi', 'settembre', 2020, 4, 9, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (147, '09:00:00', '13:00:00', 10, 'giovedi', 'settembre', 2020, 4, 14, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (148, '13:00:00', '17:00:00', 10, 'giovedi', 'settembre', 2020, 4, 19, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (149, '09:00:00', '13:00:00', 11, 'venerdi', 'settembre', 2020, 4, 24, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (150, '13:00:00', '17:00:00', 11, 'venerdi', 'settembre', 2020, 4, 29, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (151, '09:00:00', '13:00:00', 14, 'lunedi', 'settembre', 2020, 4, 4, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (152, '13:00:00', '17:00:00', 14, 'lunedi', 'settembre', 2020, 4, 9, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (153, '09:00:00', '13:00:00', 15, 'martedi', 'settembre', 2020, 4, 14, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (154, '13:00:00', '17:00:00', 15, 'martedi', 'settembre', 2020, 4, 19, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (155, '09:00:00', '13:00:00', 16, 'mercoledi', 'settembre', 2020, 4, 24, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (156, '13:00:00', '17:00:00', 16, 'mercoledi', 'settembre', 2020, 4, 29, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (157, '09:00:00', '13:00:00', 17, 'giovedi', 'settembre', 2020, 4, 4, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (158, '13:00:00', '17:00:00', 17, 'giovedi', 'settembre', 2020, 4, 9, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (159, '09:00:00', '13:00:00', 18, 'venerdi', 'settembre', 2020, 4, 14, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (160, '13:00:00', '17:00:00', 18, 'venerdi', 'settembre', 2020, 4, 19, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (161, '09:00:00', '13:00:00', 21, 'lunedi', 'settembre', 2020, 4, 24, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (162, '13:00:00', '17:00:00', 21, 'lunedi', 'settembre', 2020, 4, 29, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (163, '09:00:00', '13:00:00', 22, 'martedi', 'settembre', 2020, 4, 4, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (164, '13:00:00', '17:00:00', 22, 'martedi', 'settembre', 2020, 4, 9, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (165, '09:00:00', '13:00:00', 23, 'mercoledi', 'settembre', 2020, 4, 14, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (166, '13:00:00', '17:00:00', 23, 'mercoledi', 'settembre', 2020, 4, 19, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (167, '09:00:00', '13:00:00', 24, 'giovedi', 'settembre', 2020, 4, 24, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (168, '13:00:00', '17:00:00', 24, 'giovedi', 'settembre', 2020, 4, 29, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (169, '09:00:00', '13:00:00', 25, 'venerdi', 'settembre', 2020, 4, 4, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (170, '13:00:00', '17:00:00', 25, 'venerdi', 'settembre', 2020, 4, 9, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (171, '09:00:00', '13:00:00', 28, 'lunedi', 'settembre', 2020, 4, 14, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (172, '13:00:00', '17:00:00', 28, 'lunedi', 'settembre', 2020, 4, 19, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (173, '09:00:00', '13:00:00', 29, 'martedi', 'settembre', 2020, 4, 24, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (174, '13:00:00', '17:00:00', 29, 'martedi', 'settembre', 2020, 4, 29, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (175, '09:00:00', '13:00:00', 30, 'mercoledi', 'settembre', 2020, 4, 4, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (176, '13:00:00', '17:00:00', 30, 'mercoledi', 'settembre', 2020, 4, 9, 1);
				
                
                
                
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (177, '09:00:00', '17:00:00', 1, 'martedi', 'settembre', 2020, 4, 4, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (178, '13:00:00', '17:00:00', 1, 'martedi', 'settembre', 2020, 4, 9, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (179, '09:00:00', '13:00:00', 2, 'mercoledi', 'settembre', 2020, 4, 14, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (180, '13:00:00', '17:00:00', 2, 'mercoledi', 'settembre', 2020, 4, 19, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (181, '09:00:00', '13:00:00', 3, 'giovedi', 'settembre', 2020, 4, 24, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (182, '13:00:00', '17:00:00', 3, 'giovedi', 'settembre', 2020, 4, 29, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (183, '09:00:00', '13:00:00', 4, 'venerdi', 'settembre', 2020, 4, 4, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (184, '13:00:00', '17:00:00', 4, 'venerdi', 'settembre', 2020, 4, 9, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (185, '09:00:00', '13:00:00', 7, 'lunedi', 'settembre', 2020, 4, 14, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (186, '13:00:00', '17:00:00', 7, 'lunedi', 'settembre', 2020, 4, 19, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (187, '09:00:00', '13:00:00', 8, 'martedi', 'settembre', 2020, 4, 24, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (188, '13:00:00', '17:00:00', 8, 'martedi', 'settembre', 2020, 4, 29, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (189, '09:00:00', '13:00:00', 9, 'mercoledi', 'settembre', 2020, 4, 4, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (190, '13:00:00', '17:00:00', 9, 'mercoledi', 'settembre', 2020, 4, 9, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (191, '09:00:00', '13:00:00', 10, 'giovedi', 'settembre', 2020, 4, 14, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (192, '13:00:00', '17:00:00', 10, 'giovedi', 'settembre', 2020, 4, 19, 1);
                insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (193, '09:00:00', '13:00:00', 11, 'venerdi', 'settembre', 2020, 4, 24, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (194, '13:00:00', '17:00:00', 11, 'venerdi', 'settembre', 2020, 4, 29, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (195, '09:00:00', '13:00:00', 14, 'lunedi', 'settembre', 2020, 4, 4, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (196, '13:00:00', '17:00:00', 14, 'lunedi', 'settembre', 2020, 4, 9, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (197, '09:00:00', '13:00:00', 15, 'martedi', 'settembre', 2020, 4, 14, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (198, '13:00:00', '17:00:00', 15, 'martedi', 'settembre', 2020, 4, 19, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (199, '09:00:00', '13:00:00', 16, 'mercoledi', 'settembre', 2020, 4, 24, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (200, '13:00:00', '17:00:00', 16, 'mercoledi', 'settembre', 2020, 4, 29, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (201, '09:00:00', '13:00:00', 17, 'giovedi', 'settembre', 2020, 4, 4, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (202, '13:00:00', '17:00:00', 17, 'giovedi', 'settembre', 2020, 4, 9, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (203, '09:00:00', '13:00:00', 18, 'venerdi', 'settembre', 2020, 4, 14, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (204, '13:00:00', '17:00:00', 18, 'venerdi', 'settembre', 2020, 4, 19, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (205, '09:00:00', '13:00:00', 21, 'lunedi', 'settembre', 2020, 4, 24, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (206, '13:00:00', '17:00:00', 21, 'lunedi', 'settembre', 2020, 4, 29, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (207, '09:00:00', '13:00:00', 22, 'martedi', 'settembre', 2020, 4, 4, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (208, '13:00:00', '17:00:00', 22, 'martedi', 'settembre', 2020, 4, 9, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (209, '09:00:00', '13:00:00', 23, 'mercoledi', 'settembre', 2020, 4, 14, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (210, '13:00:00', '17:00:00', 23, 'mercoledi', 'settembre', 2020, 4, 19, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (211, '09:00:00', '13:00:00', 24, 'giovedi', 'settembre', 2020, 4, 24, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (212, '13:00:00', '17:00:00', 24, 'giovedi', 'settembre', 2020, 4, 29, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (213, '09:00:00', '13:00:00', 25, 'venerdi', 'settembre', 2020, 4, 4, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (214, '13:00:00', '17:00:00', 25, 'venerdi', 'settembre', 2020, 4, 9, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (215, '09:00:00', '13:00:00', 28, 'lunedi', 'settembre', 2020, 4, 14, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (216, '13:00:00', '17:00:00', 28, 'lunedi', 'settembre', 2020, 4, 19, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (217, '09:00:00', '13:00:00', 29, 'martedi', 'settembre', 2020, 4, 24, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (218, '13:00:00', '17:00:00', 29, 'martedi', 'settembre', 2020, 4, 29, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (219, '09:00:00', '13:00:00', 30, 'mercoledi', 'settembre', 2020, 4, 4, 1);
				insert into `BD_PROJECT`.`TURNO` (`idTURNO`,`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`,`turno_Coperto`) values (220, '13:00:00', '17:00:00', 30, 'mercoledi', 'settembre', 2020, 4, 9, 1);
				
        COMMIT;
        
        
        START TRANSACTION;
				
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (1,'09:00:00','17:00:00','lunedi',1); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (2,'09:00:00','17:00:00','martedi',1); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (3,'09:00:00','17:00:00','mercoledi',1); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (4,'09:00:00','17:00:00','giovedi',1); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (5,'09:00:00','17:00:00','venerdi',1); 


                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (6,'09:00:00','17:00:00','lunedi',2); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (7,'09:00:00','17:00:00','martedi',2); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (8,'09:00:00','17:00:00','mercoledi',2); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (9,'09:00:00','17:00:00','giovedi',2); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (10,'09:00:00','17:00:00','venerdi',2); 


                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (11,'09:00:00','17:00:00','lunedi',3); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (12,'09:00:00','17:00:00','martedi',3); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (13,'09:00:00','17:00:00','mercoledi',3); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (14,'09:00:00','17:00:00','giovedi',3); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (15,'09:00:00','17:00:00','venerdi',3); 


                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (16,'09:00:00','17:00:00','lunedi',4); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (17,'09:00:00','17:00:00','martedi',4); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (18,'09:00:00','17:00:00','mercoledi',4); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (19,'09:00:00','17:00:00','giovedi',4); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (20,'09:00:00','17:00:00','venerdi',4); 


                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (21,'09:00:00','17:00:00','lunedi',5); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (22,'09:00:00','17:00:00','martedi',5); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (23,'09:00:00','17:00:00','mercoledi',5); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (24,'09:00:00','17:00:00','giovedi',5); 
                insert into `BD_PROJECT`.`ORARIO_SETTIMANALE` (`idORARIO_SETTIMANALE`,`Ora_inizio`,`Ora_fine`,`Giorno`,`BIBLIOTECA_idBIBLIOTECA`) values (25,'09:00:00','17:00:00','venerdi',5); 

        COMMIT;
    
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure aggiungi_utilizzatore_sistema_proc
-- -----------------------------------------------------

DELIMITER $$
USE `BD_PROJECT`$$
CREATE PROCEDURE `aggiungi_utilizzatore_sistema_proc` (in var_username VARCHAR(45), in var_password VARCHAR(30), in var_ruolo VARCHAR(6), out var_id int)
BEGIN
	insert into `UTILIZZATORI_SISTEMA` (`username`,`password`,`ruolo`) values (var_username, var_password, var_ruolo);
	set var_id = last_insert_id();
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure aggiungi_prestito_proc
-- -----------------------------------------------------

DELIMITER $$
USE `BD_PROJECT`$$
CREATE PROCEDURE `aggiungi_prestito_proc` (in var_id_utente int, in var_id_copia int, in var_id_turno int, in var_terminato boolean, out var_id_prestito int)
BEGIN
		insert into `PRESTITO` (`UTENTE_ID`,`COPIA_idCOPIA`,`TURNO_idTURNO`, `Data_prestito`, `Terminato`) values (var_id_utente, var_id_copia, var_id_turno, current_date(), var_terminato);
        set var_id_prestito = last_insert_id();
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure report_turni_scoperti_proc
-- -----------------------------------------------------

DELIMITER $$
USE `BD_PROJECT`$$
CREATE PROCEDURE `report_turni_scoperti_proc` (in var_idBiblioteca int)
BEGIN
	set transaction isolation level read committed;
    start transaction;
    
    select `idTURNO`,`Numero`, `Giorno`, `Mese`, `Anno`
	from `TURNO`
	where `BIBLIOTECA_idBIBLIOTECA` = var_idBiblioteca and
		  `turno_Coperto` = 0;
    
    commit;

END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure report_prestiti_proc
-- -----------------------------------------------------

DELIMITER $$
USE `BD_PROJECT`$$
CREATE PROCEDURE `report_prestiti_proc` (in var_id_biblioteca int) 
BEGIN
	
    start transaction;
    select `L`.`idLIBRO`, `L`.`Nome`, `C`.`idCOPIA`, `U`.`ID`, `U`.`Nome`, `U`.`Cognome` 
    from `LIBRO` as `L`
    inner join `COPIA` as `C` on (`L`.`idLIBRO` = `C`.`LIBRO_idLIBRO`)
    inner join `PRESTITO` as `P` on (`C`.`idCOPIA` = `P`.`COPIA_idCOPIA`)
    inner join `POSSEDERE` as `PS` on (`C`.`idCOPIA` = `PS`.`COPIA_idCOPIA`)
    inner join `UTENTE` as `U` on (`P`.`UTENTE_ID` = `U`.`ID`)
    where `PS`.`BIBLIOTECA_idBIBLIOTECA` = var_id_biblioteca
    and `P`.`Terminato` = 0;
    
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure add_turno_proc
-- -----------------------------------------------------

DELIMITER $$
USE `BD_PROJECT`$$
CREATE PROCEDURE `add_turno_proc` (in var_ora_inizio time, in var_ora_fine time, in var_numero int, in var_giorno varchar(15), in var_mese varchar(15), in var_anno int, in var_idBibliotecario int, in var_idBiblioteca int, out var_idTurno int)
BEGIN
	
		insert into `TURNO` (`Ora_inizio`,`Ora_fine`,`Numero`,`Giorno`,`Mese`,`Anno`,`BIBLIOTECA_idBIBLIOTECA`,`BIBLIOTECARIO_idBIBLIOTECARIO`, `turno_Coperto`) values (var_ora_inizio, var_ora_fine, var_numero, var_giorno, var_mese, var_anno, var_idBiblioteca, var_idBibliotecario, 1);
        set var_idTurno = last_insert_id();
END$$

DELIMITER ;
CREATE USER 'amministratore_us' IDENTIFIED BY 'amministratore';

GRANT ALL ON `BD_PROJECT`.* TO 'amministratore_us';
CREATE USER 'bibliotecario_us' IDENTIFIED BY 'bibliotecario';

GRANT SELECT ON TABLE `BD_PROJECT`.* TO 'bibliotecario_us';
CREATE USER 'utente_us' IDENTIFIED BY 'utente';

GRANT SELECT, INSERT, TRIGGER ON TABLE `BD_PROJECT`.* TO 'utente_us';
CREATE USER 'login_us' IDENTIFIED BY 'login';

GRANT SELECT, INSERT, TRIGGER, UPDATE, DELETE ON TABLE `BD_PROJECT`.* TO 'login_us';

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
