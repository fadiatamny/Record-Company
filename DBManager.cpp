#include "includes.hpp"
#include "DBManager.hpp"

char username[256];
char password[256];

int initDB()
{
	mysqlx_error_t *err;
	mysqlx_stmt_t* query;
	mysqlx_result_t* result;
	mysqlx_session_t* session = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, username, password, "", &err);

	if (NULL == session) 
	{
	 	std::cout << mysqlx_error_message(err) << "\t" << session << std::endl;
		return -1;
	}
	
	//if database exists drop it and create it 
	if (mysqlx_get_schema(session, "RecordCompany", 1)) 
	{
		query = mysqlx_sql_new(session, "DROP database `RecordCompany`", MYSQLX_NULL_TERMINATED);

		if( (result = mysqlx_execute(query)) == NULL)
			return -1;
	}

	query = mysqlx_sql_new(session, "create database `RecordCompany`", MYSQLX_NULL_TERMINATED);

	if( (result = mysqlx_execute(query)) == NULL)
		return -1;

	mysqlx_session_close(session);

	session =  mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, username, password, "RecordCompany", &err);
	
	if (NULL == session) 
	{
	 	std::cout << mysqlx_error_message(err) << "\t" << session << std::endl;
		return -1;
	}

	//create the main tables 

	query = mysqlx_sql_new(session, "CREATE TABLE `musician` (\
  `id_musician` INT NOT NULL,\
  `Name` VARCHAR(256) NOT NULL,\
  `Address` VARCHAR(256) NOT NULL,\
  `Phone` VARCHAR(256) NOT NULL,\
  `Skill` VARCHAR(256) NOT NULL,\
  PRIMARY KEY (`id_musician`));", MYSQLX_NULL_TERMINATED);


	if( (result = mysqlx_execute(query)) == NULL)
		return -1;

	query = mysqlx_sql_new(session,"CREATE TABLE `instrument` (\
  `I_id` INT NOT NULL UNIQUE,\
  `Brand` VARCHAR(256) NOT NULL,\
  `Type` VARCHAR(256) NOT NULL,\
  PRIMARY KEY (`I_id`, `Brand`, `Type`));", MYSQLX_NULL_TERMINATED);


	if( (result = mysqlx_execute(query)) == NULL)
		return -1;

	query = mysqlx_sql_new(session,"CREATE TABLE `track` (\
  `T_id` INT NOT NULL,\
  `Name` VARCHAR(256) NOT NULL,\
  `Music_Compuser` VARCHAR(256) NULL,\
  `Length` INT NOT NULL,\
  `Lyrics_Composer` VARCHAR(256) NULL,\
  `Date` VARCHAR(256) NOT NULL,\
  `Genre` VARCHAR(256) NOT NULL,\
  `Technician` VARCHAR(256) NOT NULL,\
  PRIMARY KEY (`T_id`));", MYSQLX_NULL_TERMINATED);


	if( (result = mysqlx_execute(query)) == NULL)
		return -1;

	query = mysqlx_sql_new(session,"CREATE TABLE `album` (\
  `A_id` INT NOT NULL,\
  `Name` VARCHAR(256) NOT NULL,\
  `S_Date` VARCHAR(256) NOT NULL,\
  `E_Date` VARCHAR(256) NOT NULL,\
  `Tracks_Number` INT NOT NULL,\
  PRIMARY KEY (`A_id`));", MYSQLX_NULL_TERMINATED);


	if( (result = mysqlx_execute(query)) == NULL)
		return -1;

	query = mysqlx_sql_new(session,"CREATE TABLE `producer` (\
  `p_ID` INT NOT NULL,\
  `Name` VARCHAR(256) NOT NULL,\
  PRIMARY KEY (`p_ID`));", MYSQLX_NULL_TERMINATED);


	if( (result = mysqlx_execute(query)) == NULL)
		return -1;


	//Relations tables

	query = mysqlx_sql_new(session,"CREATE TABLE `musician_instrument` (\
  `m_ID` INT NOT NULL,\
  `i_ID` INT NOT NULL,\
  PRIMARY KEY (`i_ID`, `m_ID`),\
  INDEX `MI_mID_idx` (`m_ID` ASC) VISIBLE,\
  CONSTRAINT `MI_mID`\
    FOREIGN KEY (`m_ID`)\
    REFERENCES `recordcompany`.`musician` (`id_musician`)\
    ON DELETE NO ACTION\
    ON UPDATE NO ACTION,\
  CONSTRAINT `MI_iID`\
    FOREIGN KEY (`i_ID`)\
    REFERENCES `recordcompany`.`instrument` (`I_id`)\
    ON DELETE NO ACTION\
    ON UPDATE NO ACTION);", MYSQLX_NULL_TERMINATED);

	if( (result = mysqlx_execute(query)) == NULL)
		return -1;

	query = mysqlx_sql_new(session,"CREATE TABLE `musician_tracks` (\
  `m_ID` INT NOT NULL,\
  `t_ID` INT NOT NULL,\
  PRIMARY KEY (`m_ID`, `t_ID`),\
  INDEX `MT_tID_idx` (`t_ID` ASC) VISIBLE,\
  CONSTRAINT `MT_mID`\
    FOREIGN KEY (`m_ID`)\
    REFERENCES `recordcompany`.`musician` (`id_musician`)\
    ON DELETE NO ACTION\
    ON UPDATE NO ACTION,\
  CONSTRAINT `MT_tID`\
    FOREIGN KEY (`t_ID`)\
    REFERENCES `recordcompany`.`track` (`T_id`)\
    ON DELETE NO ACTION\
    ON UPDATE NO ACTION);", MYSQLX_NULL_TERMINATED);

	if( (result = mysqlx_execute(query)) == NULL)
		return -1;

	query = mysqlx_sql_new(session,"CREATE TABLE `album_track` (\
  `t_ID` INT NOT NULL,\
  `a_ID` INT NOT NULL,\
  PRIMARY KEY (`t_ID`, `a_ID`),\
  INDEX `AT_aID_idx` (`a_ID` ASC) VISIBLE,\
  CONSTRAINT `AT_tID`\
    FOREIGN KEY (`t_ID`)\
    REFERENCES `recordcompany`.`track` (`T_id`)\
    ON DELETE NO ACTION\
    ON UPDATE NO ACTION,\
  CONSTRAINT `AT_aID`\
    FOREIGN KEY (`a_ID`)\
    REFERENCES `recordcompany`.`album` (`A_id`)\
    ON DELETE NO ACTION\
    ON UPDATE NO ACTION);", MYSQLX_NULL_TERMINATED);

	if( (result = mysqlx_execute(query)) == NULL)
		return -1;

	query = mysqlx_sql_new(session,"CREATE TABLE `album_producer` (\
  `a_ID` INT NOT NULL,\
  `p_ID` INT NOT NULL,\
  PRIMARY KEY (`a_ID`, `p_ID`),\
  INDEX `AP_pID_idx` (`p_ID` ASC) VISIBLE,\
  CONSTRAINT `AP_aID`\
    FOREIGN KEY (`a_ID`)\
    REFERENCES `recordcompany`.`album` (`A_id`)\
    ON DELETE NO ACTION\
    ON UPDATE NO ACTION,\
  CONSTRAINT `AP_pID`\
    FOREIGN KEY (`p_ID`)\
    REFERENCES `recordcompany`.`producer` (`p_ID`)\
    ON DELETE NO ACTION\
    ON UPDATE NO ACTION);", MYSQLX_NULL_TERMINATED);

	if( (result = mysqlx_execute(query)) == NULL)
		return -1;

	mysqlx_session_close(session);

	return 1;
}

int initData()
{
	mysqlx_error_t *err;
	mysqlx_stmt_t* query;
	mysqlx_result_t* result;
	mysqlx_session_t* session = mysqlx_get_session("localhost", DEFAULT_MYSQLX_PORT, username, password, "recordcompany", &err);

	if (NULL == session) 
	{
	 	std::cout << mysqlx_error_message(err) << "\t" << session << std::endl;
		return -1;
	}

	query = mysqlx_sql_new(session, "INSERT INTO `musician` (`id_musician`, `Name`, `Address`, `Phone`, `Skill`) VALUES\
 (1, 'Beethover', 'Austria', '098-909985', 'Player') ,\
 (2, 'Michael Jackson' ,'Los Angeles', '098-373489', 'Singer & Player'),\
 (3, 'James Cameron' ,'Los Angeles', '067-758937', 'Player'),\
 (4, 'Ice-T' ,'New York', '068-374838', 'Singer'),\
 (5, 'P-Dog' ,'New York', '098-373489', 'Singer'),\
 (6, 'Roger Waters' ,'London', '096-388389', 'Singer & Player'),\
 (7, 'p!nk', 'Las Vegas' , '123-868729', 'Singer & Player'),\
 (8, 'Freddy Mercury', 'London', '028-959395', 'Singer & Player'),\
 (9, 'ABBA', 'Sweden', '634-850258', 'Singer & Player'),\
 (10, 'Danny Gonzales', 'Oragon', '078-123963', 'Singer'),\
 (11, 'TMG', 'Los Angeles', '063-847492', 'Singer');", MYSQLX_NULL_TERMINATED);

	if( (result = mysqlx_execute(query)) == NULL)
		return -1;

	query = mysqlx_sql_new(session, "INSERT INTO `track` (`T_id`, `Name`, `Music_Compuser`, `Length`, `Lyrics_Composer`, `Date`, `Genre`, `Technician`) VALUES\
 (1, 'Ode to Joy', 'Beethoven', 220, 'NULL', '1824-01-01', 'Classical', 'Beethoven'),\
 (2, 'We Will Rock You','Queen',250,'Freddy Mercury','1977-10-07','Rock','Bill McGree'),\
 (3, 'Toxic','Jack Hallow',164,'Kelsey Lowe','2004-01-13','Pop','Bill McGree'),\
 (4, 'Johnny Johnny','Danny Gonzales',203,'Danny Gonzales','2017-06-21','Comedy','AudioCloud'),\
 (5, 'Slime','Danny Gonzales',189,'Danny Gonzales','2018-01-17','Comedy','AudioCloud'),\
 (6, 'No Flex','Noel Miller',89,'Noel Miller','2016-12-29','Rap','Cody Ko'),\
 (7, 'Walkman','Noel Miller',197,'Cody Ko','2018-11-25','Rap','Cody Ko'),\
 (8, 'Short King Anthem','Noel Miller',192,'Noel Miller','2019-02-14','Rap','Cody Ko'),\
 (9, 'Disco Queen','Bjorn and Benny',184,'Bjorn and Benny','1975-08-04','Pop','Bill McGree'),\
 (10, 'Thriller','Michael Jackson',217,'Freddy Mercury','1984-01-23','Pop','AudioCloud');", MYSQLX_NULL_TERMINATED);

	if( (result = mysqlx_execute(query)) == NULL)
		return -1;

	query = mysqlx_sql_new(session, "INSERT INTO `album` (`A_id`, `Name`, `S_Date`, `E_Date`, `Tracks_Number`) VALUES\
 (1, 'Ode to Joy RErelease', '2015-04-15', '2015-04-29', 1),\
 (2, 'Johnny Johnny SINGLE', '2017-06-20', '2017-07-01', 1),\
 (3, 'Slime SINGLE', '2018-01-15', '2018-02-02', 1),\
 (4, 'Danny the Collection', '2019-01-01', '2019-01-15', 2),\
 (5, 'TMG', '2016-12-20', '2019-02-20', 3),\
 (6, 'Diso Queen SINGLE', '1975-08-04', '1975-08-05', 1),\
 (7, 'Thriller SINGLE', '1984-01-20', '1984-01-26', 1),\
 (8, 'Pop Supreme', '2016-05-28', '2016-06-10', 3),\
 (9, 'Record Star', '2019-07-07', '2019-07-30', 10),\
 (10, 'Its Brittney', '2004-01-10', '2004-01-20', 1);", MYSQLX_NULL_TERMINATED);

	if( (result = mysqlx_execute(query)) == NULL)
		return -1;

	query = mysqlx_sql_new(session, "INSERT INTO `recordcompany`.`instrument` (`I_id`, `Brand`, `Type`) VALUES\
 (1, 'Yamhaa', 'Piano'),\
 (2, 'Yamhaa', 'Synth'),\
 (3, 'Gibson', 'Bass'),\
 (4, 'Fender', 'Bass'),\
 (5, 'Gibson', 'Guitar'),\
 (6, 'Fender', 'Guitar'),\
 (7, 'Epiphone', 'Guitar'),\
 (8, 'Gretch', 'Drums'),\
 (9, 'Ludwig', 'Drums'),\
 (10, 'Remo', 'Tambourine');", MYSQLX_NULL_TERMINATED);

	if( (result = mysqlx_execute(query)) == NULL)
		return -1;

	query = mysqlx_sql_new(session, "INSERT INTO `recordcompany`.`producer` (`p_ID`, `Name`) VALUES\
 (1, 'Bill McGree'),\
 (2, 'Dr Dre'),\
 (3, 'Jay-Z'),\
 (4, 'Rick Rubin'),\
 (5, 'George Martin'),\
 (6, 'Joe Meek'),\
 (7, 'Quincy Jones'),\
 (8, 'Phil Spector'),\
 (9, 'Butch Vig'),\
 (10, 'Arif Mardin');", MYSQLX_NULL_TERMINATED);

	if( (result = mysqlx_execute(query)) == NULL)
		return -1;

	query = mysqlx_sql_new(session, "INSERT INTO `musician_instrument` (`m_ID`, `i_ID`) VALUES\
 (1,1),\
 (2,2),\
 (2,5),\
 (3,10),\
 (6,1),\
 (6,6),\
 (7,5),\
 (7,6),\
 (8,1),\
 (8,2),\
 (9,1),\
 (9,5),\
 (9,8);", MYSQLX_NULL_TERMINATED);

	if( (result = mysqlx_execute(query)) == NULL)
		return -1;
	
	query = mysqlx_sql_new(session, "INSERT INTO `musician_tracks` (`m_ID`, `t_ID`) VALUES\
 (1,1),\
 (2,10),\
 (3,10),\
 (8,2),\
 (9,9),\
 (7,3),\
 (10,4),\
 (10,5),\
 (11,6),\
 (11,7),\
 (11,8),\
 (4,6),\
 (6,2),\
 (8,3);", MYSQLX_NULL_TERMINATED);

	if( (result = mysqlx_execute(query)) == NULL)
		return -1;
	
	query = mysqlx_sql_new(session, "INSERT INTO `album_track`(`t_ID`, `a_ID`) VALUES\
 (1,1),\
 (4,2),\
 (5,3),\
 (4,4),\
 (5,4),\
 (6,5),\
 (7,5),\
 (8,5),\
 (9,6),\
 (10,7),\
 (3,8),\
 (9,8),\
 (10,8),\
 (3,10),\
 (1,9),\
 (2,9),\
 (3,9),\
 (4,9),\
 (5,9),\
 (6,9),\
 (7,9),\
 (8,9),\
 (9,9),\
 (10,9);", MYSQLX_NULL_TERMINATED);

	if( (result = mysqlx_execute(query)) == NULL)
		return -1;

	query = mysqlx_sql_new(session, "INSERT INTO `album_producer` (`a_ID`,`p_ID`) VALUES\
 (1,1),\
 (2,2),\
 (3,2),\
 (3,1),\
 (4,1),\
 (4,2),\
 (4,3),\
 (5,2),\
 (5,6),\
 (6,4),\
 (6,5),\
 (8,4),\
 (8,5),\
 (8,6),\
 (9,1),\
 (9,7),\
 (9,10),\
 (10,9);", MYSQLX_NULL_TERMINATED);

	if( (result = mysqlx_execute(query)) == NULL)
		return -1;
	
	mysqlx_session_close(session);

	return 1;
}

int userInterface()
{
	bool flag = true;
	std::string tmp1;
	std::string tmp1;

	std::cout << "Welcome to MusicCompany project" << std::endl << std::endl;

		int choice = 0;
	do
	{	
		std::cout << " 1.\tHow many albums were created between two given dates\n\
 2.\tHow many songs recored by a specific musician between two given dates\n\
 3.\tHow many diffrent albums recorded specific musician between two given dates\n\
 4.\tWhat is the most popular instrument\n\
 5.\tList of instruments used in a specific album\n\
 6.\tThe most popular album producer between two given dates\n\
 7.\tThe most puplat manufacturer of an instrument\n\
 8.\tHow many musicians recorded throughout the years\n\
 9.\tThe musician who collaborated the most\n\
 10.\tThe most popular music genre\n\
 11.\tThe technician who worked on the most number of tracks between two given dates\n\
 12.\tThe first album ever recorded\n\
 13.\tList of tracks that were in two or more albums\n\
 14.\tList of technicians that worked on complete albums\n\
 15.\tThe musician with the most diverse musical genres \n\
 16.\tExit Program.\n\n\
		"<< std::endl;
		std::cout << "Please choose function by number >\t";
		std::cin >> choice;

		switch (choice)
		{
		case 1:
				std::cout << "Please input the two dates ( Year-Month-Day )" << std::endl << std::endl;
				std::cin >> tmp1 >> tmp2;
				std::cout << std::endl;
				if(albumsBetween(firstD,lastD) == -1)
					std::cout << "Error occured while executing the sql query" << std::endl; 	
			break;

		case 2:
				std::cout << "Plese input musician name" << std::endl;
				std::cin >> tmp1 ;
				musicianSongBetween(tmp1);
			break;

		case 3:
			break;

		case 4:
			break;

		case 5:
			break;

		case 6:
			break;

		case 7:
			break;

		case 8:
			break;

		case 9:
			break;

		case 10:
			break;

		case 11:
			break;

		case 12:
			break;

		case 13:
			break;

		case 14:
			break;

		case 15:
			break;

		case 16: 
				flag = false;
				std::cout << std::endl << "Goodbye" << std::endl;
			break;
		
		default:
			std::cout << "Incorrect number ! this choice does not exist please re enter" << std::endl;
			break;
		}
		std::cout << " ---------------------------------------------------------------------- " << std::endl << std::endl;
	}while(flag);

	return 1;
}