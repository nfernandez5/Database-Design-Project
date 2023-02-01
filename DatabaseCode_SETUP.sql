DROP TABLE people CASCADE;
CREATE TABLE people (
	ID 			NUMERIC(6) PRIMARY KEY,
	username 	VARCHAR(30) NOT NULL UNIQUE,
	password 	VARCHAR(30) NOT NULL,
	email 		VARCHAR(40) NOT NULL,
	fname 		VARCHAR(20) NOT NULL,
	lname 		VARCHAR(20),
	address 	VARCHAR(100),
	phone_num 	NUMERIC(10)
);

DROP TABLE banned_people CASCADE;
CREATE TABLE banned_people (
	ID 			NUMERIC(6) PRIMARY KEY,
	username 	VARCHAR(30) NOT NULL UNIQUE,
	password 	VARCHAR(30) NOT NULL,
	email 		VARCHAR(40) NOT NULL,
	fname 		VARCHAR(20) NOT NULL,
	lname 		VARCHAR(20),
	address 	VARCHAR(100),
	phone_num 	NUMERIC(10)
);

DROP TABLE employees CASCADE;
CREATE TABLE employees (
	ID 				NUMERIC(6) PRIMARY KEY REFERENCES people(ID) ON DELETE CASCADE,
	SSN 			NUMERIC(9) NOT NULL UNIQUE,
	salary 			NUMERIC(6) NOT NULL DEFAULT 40000 CHECK (salary > 0),
	hire_date 		DATE NOT NULL DEFAULT CURRENT_DATE,
	is_full_time 	BOOLEAN NOT NULL DEFAULT FALSE,
	weekly_hours 	NUMERIC(2) NOT NULL DEFAULT 20 CHECK(weekly_hours >= 0),
	is_manager 		BOOLEAN NOT NULL DEFAULT FALSE
);

DROP TABLE customers CASCADE;
CREATE TABLE customers (
    ID              NUMERIC(6) PRIMARY KEY REFERENCES people(ID) ON DELETE CASCADE,
    library_size    NUMERIC(7) NOT NULL DEFAULT 0 CHECK(library_size >= 0),
    funds           NUMERIC(5) NOT NULL DEFAULT 0 CHECK(funds >= 0),
    customer_since  DATE NOT NULL DEFAULT CURRENT_DATE
);

DROP TABLE members CASCADE;
CREATE TABLE members (
	ID 				NUMERIC(6) PRIMARY KEY REFERENCES people(ID) ON DELETE CASCADE,
	points 			NUMERIC(5) NOT NULL DEFAULT 0 CHECK (points >= 0),
	reward_tier 	VARCHAR(8) NOT NULL DEFAULT 'Bronze' CHECK (reward_tier IN('Bronze', 'Silver', 'Gold', 'Platinum', 'Diamond')),
	member_since 	DATE NOT NULL DEFAULT CURRENT_DATE
);

DROP TABLE xbox_linked_accounts CASCADE;
CREATE TABLE xbox_linked_accounts (
	ID 					NUMERIC(6) 	PRIMARY KEY REFERENCES people(ID) ON DELETE CASCADE,
	xbox_account_name 	VARCHAR(30) NOT NULL UNIQUE
);

DROP TABLE steam_linked_accounts CASCADE;
CREATE TABLE steam_linked_accounts (
	ID 					NUMERIC(6) 	PRIMARY KEY REFERENCES people(ID) ON DELETE CASCADE,
	steam_account_name 	VARCHAR(30) NOT NULL UNIQUE
);

DROP TABLE pstation_linked_accounts CASCADE;
CREATE TABLE pstation_linked_accounts (
	ID 		        			NUMERIC(6) 	PRIMARY KEY REFERENCES people(ID) ON DELETE CASCADE,
	pstation_account_name   	VARCHAR(30) NOT NULL UNIQUE
);

DROP TABLE nswitch_linked_accounts CASCADE;
CREATE TABLE nswitch_linked_accounts (
	ID 	    				NUMERIC(6) 	PRIMARY KEY REFERENCES people(ID) ON DELETE CASCADE,
	nswitch_account_name 	VARCHAR(30) NOT NULL UNIQUE
);

DROP TABLE games CASCADE;
CREATE TABLE games (
    SKU                 NUMERIC(6) PRIMARY KEY,
    name                VARCHAR(30) NOT NULL,
    release_date        DATE NOT NULL DEFAULT CURRENT_DATE,
    added_to_store_date DATE NOT NULL DEFAULT CURRENT_DATE,
    rating_sum          INTEGER NOT NULL DEFAULT 0 CHECK(rating_sum >= 0),
    rating_count        INTEGER NOT NULL DEFAULT 0 CHECK(rating_count >= 0),
    genre               VARCHAR(20),
    retail_price        NUMERIC(6, 2) NOT NULL DEFAULT 60,
    discount            NUMERIC(3) NOT NULL DEFAULT 0 CHECK(discount >= 0 AND discount <= 100),
    numSales            INTEGER NOT NULL DEFAULT 0,
    on_Xbox             BOOLEAN NOT NULL DEFAULT FALSE,
    on_Playstation      BOOLEAN NOT NULL DEFAULT FALSE,
    on_Steam            BOOLEAN NOT NULL DEFAULT FALSE,
    on_NSwitch          BOOLEAN NOT NULL DEFAULT FALSE
);

DROP TABLE purchased_games CASCADE;
CREATE TABLE purchased_games (
    ID              NUMERIC(6) REFERENCES people(ID) ON DELETE CASCADE,
    SKU             NUMERIC(6) REFERENCES games(SKU) ON DELETE CASCADE,
    platform        VARCHAR(9) CHECK( platform IN('nswitch', 'xbox', 'pstation', 'steam')),
    rating          SMALLINT CHECK(rating >= 0 AND rating <= 5),
    purchase_date   DATE NOT NULL DEFAULT CURRENT_DATE,
    last_played     DATE,
    price_paid      NUMERIC(6, 2) NOT NULL CHECK(price_paid >= 0),
    CONSTRAINT      purchasedGames_pk PRIMARY KEY(ID, SKU, platform)
);

DROP VIEW EmployeeView CASCADE;
CREATE VIEW EmployeeView 
AS SELECT people.ID, username, email, fname, lname 
FROM people FULL OUTER JOIN employees ON people.ID = employees.ID
WHERE SSN IS NULL;

DROP VIEW ManagerView CASCADE;
CREATE VIEW ManagerView 
AS SELECT people.ID, username, email, fname, lname, salary, hire_date, weekly_hours
FROM people FULL OUTER JOIN employees ON people.ID = employees.ID
WHERE is_manager IS NULL OR is_manager = FALSE;

DROP FUNCTION banned_backup();
CREATE FUNCTION banned_backup()
	RETURNS trigger AS $banned$
	BEGIN
		INSERT INTO banned_people
		VALUES(OLD.ID, OLD.username, OLD.password, OLD.email, OLD.fname, OLD.lname, OLD.address, OLD.phone_num);
		RETURN OLD;
	END;
$banned$ LANGUAGE PLPGSQL;

CREATE TRIGGER banned_cust
BEFORE DELETE ON people
FOR EACH ROW
EXECUTE PROCEDURE banned_backup();