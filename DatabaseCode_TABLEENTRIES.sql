INSERT INTO people(ID, username, password, fname, lname, address, phone_num, email)
VALUES(1, 'OrangeMenace', 'Or@ng3', 'Noel', 'is my cat', 'behind you', 8138675309, 'IHatePrecious@gmail.com');
INSERT INTO people(ID, username, password, fname, address, email)
VALUES(2, 'BabyBoots', 'B@b3y', 'BB', 'zachs desk', 'LayInTheSun@gmail.com');
INSERT INTO people(ID, username, password, fname, address, email)
VALUES(3, 'XX_FatCat_XX', 'B1gb@i', 'Osiris', 'kitchen table', 'IAmBigBoy@gmail.com');
INSERT INTO people(ID, username, password, fname, lname, email)
VALUES(4, 'PreciousIsABoysName', '1AmGr@y', 'Precious', 'Gray Cat', 'NoelScaresMe@gmail.com');

INSERT INTO people(ID, username, password, fname, lname, address, phone_num, email)
VALUES(5, 'DanTheMan', 'D@n1', 'Dan', 'the Man', '1234 winchester lane', 8138675309, 'DanTheMan@gmail.com');
INSERT INTO people(ID, username, password, fname, lname, address, phone_num, email)
VALUES(6, 'ZachWithACH', 'Z@c4', 'Zach', 'Z', '1234 oak lane', 8131111111, 'zach@gmail.com');
INSERT INTO people(ID, username, password, fname, lname, address, phone_num, email)
VALUES(7, 'morganflower', 'M0rg@n', 'Morgan', 'B', '1234 oak lane', 8131111111, 'morgan@gmail.com');
INSERT INTO people(ID, username, password, fname, lname, address, phone_num, email)
VALUES(8, 'MyNameIsntJoe', 'J@3spass', 'Not', 'Joe', '1234 beachfront lane', 8138675309, 'Moe@gmail.com');

INSERT INTO employees(ID, SSN, salary, is_full_time, weekly_hours, is_manager) 
VALUES(5, 111111111, 90000, TRUE, 60, TRUE);
INSERT INTO employees(ID, SSN, is_full_time, weekly_hours) 
VALUES(6, 111111112, TRUE, 40);
INSERT INTO employees(ID, SSN, salary, is_full_time, weekly_hours) 
VALUES(7, 111111113, 45000, FALSE, 25);
INSERT INTO employees(ID, SSN, weekly_hours) 
VALUES(8, 111111114, 10);

INSERT INTO customers(ID, library_size, funds, customer_since)
VALUES(1, 2, 300, '2022-01-12');
INSERT INTO customers(ID, library_size, funds, customer_since)
VALUES(2, 1, 0, '2022-02-25');
INSERT INTO customers(ID, library_size, funds, customer_since)
VALUES(3, 1, 23, '2022-02-14');
INSERT INTO customers(ID, library_size, funds, customer_since)
VALUES(4, 4, 5003, '2022-01-22');

INSERT INTO members(ID, points, reward_tier) 
VALUES(1, 12345, 'Platinum');
INSERT INTO members(ID, reward_tier) 
VALUES(2, 'Gold');
INSERT INTO members(ID, points) 
VALUES(4, 12345);

INSERT INTO xbox_linked_accounts(ID, xbox_account_name) 
VALUES(1, 'IWillEatYou');
INSERT INTO xbox_linked_accounts(ID, xbox_account_name) 
VALUES(3, 'AlwaysEating');
INSERT INTO xbox_linked_accounts(ID, xbox_account_name) 
VALUES(4, 'NeverEaten');

INSERT INTO steam_linked_accounts(ID, steam_account_name) 
VALUES(1, 'IWillEatYou');
INSERT INTO steam_linked_accounts(ID, steam_account_name) 
VALUES(3, 'AlwaysEating');
INSERT INTO steam_linked_accounts(ID, steam_account_name) 
VALUES(4, 'NeverEaten');

INSERT INTO pstation_linked_accounts(ID, pstation_account_name) 
VALUES(1, 'IWillEatYou');
INSERT INTO pstation_linked_accounts(ID, pstation_account_name) 
VALUES(3, 'AlwaysEating');
INSERT INTO pstation_linked_accounts(ID, pstation_account_name) 
VALUES(4, 'NeverEaten');

INSERT INTO nswitch_linked_accounts(ID, nswitch_account_name) 
VALUES(1, 'IWillEatYou');
INSERT INTO nswitch_linked_accounts(ID, nswitch_account_name) 
VALUES(3, 'AlwaysEating');
INSERT INTO nswitch_linked_accounts(ID, nswitch_account_name) 
VALUES(4, 'NeverEaten');

INSERT INTO games(SKU, name, release_date, added_to_store_date, rating_sum, rating_count, genre, retail_price, discount, numSales, on_Xbox, on_Playstation, on_Steam, on_NSwitch)
VALUES(1, 'Monster Hunter Rise', '2022-01-12', '2022-01-11', 80000, 17000, 'Action', 60, 50, 200000, FALSE, FALSE, TRUE, TRUE);
INSERT INTO games(SKU, name, release_date, added_to_store_date, rating_sum, rating_count, genre, retail_price, discount, numSales, on_Xbox, on_Playstation, on_Steam, on_NSwitch)
VALUES(2, 'Elden Ring', '2022-02-24', '2022-01-01', 1500000, 349000, 'Souls-like', 60, 0, 400000, TRUE, TRUE, TRUE, FALSE);
INSERT INTO games(SKU, name, release_date, added_to_store_date, rating_sum, rating_count, genre, retail_price, discount, numSales, on_Xbox, on_Playstation, on_Steam, on_NSwitch)
VALUES(3, 'Halo Infinite', '2021-11-15', '2020-11-11', 500000, 137000, 'Shooter', 0, 0, 400000, TRUE, FALSE, TRUE, FALSE);

INSERT INTO purchased_games(ID, SKU, platform, rating, purchase_date, last_played, price_paid)
VALUES( 1, 1, 'steam', 5, '2022-01-12', '2022-04-27', 60);
INSERT INTO purchased_games(ID, SKU, platform, rating, purchase_date, last_played, price_paid)
VALUES( 1, 1, 'nswitch', 4, '2022-02-12', '2022-03-27', 60);
INSERT INTO purchased_games(ID, SKU, platform, rating, purchase_date, last_played, price_paid)
VALUES( 2, 2, 'xbox', 4, '2022-02-25', '2022-04-27', 60);
INSERT INTO purchased_games(ID, SKU, platform, rating, purchase_date, last_played, price_paid)
VALUES( 3, 3, 'xbox', 3, '2022-02-14', '2022-03-27', 0);
INSERT INTO purchased_games(ID, SKU, platform, rating, purchase_date, last_played, price_paid)
VALUES( 4, 1, 'steam', 4, '2022-01-22', '2022-02-27', 60);
INSERT INTO purchased_games(ID, SKU, platform, rating, purchase_date, last_played, price_paid)
VALUES( 4, 1, 'nswitch', 5, '2022-02-27', '2022-02-27', 60);
INSERT INTO purchased_games(ID, SKU, platform, rating, purchase_date, last_played, price_paid)
VALUES( 4, 2, 'xbox', 3, '2022-02-26', '2022-02-27', 60);
INSERT INTO purchased_games(ID, SKU, platform, rating, purchase_date, last_played, price_paid)
VALUES( 4, 3, 'xbox', 5, '2022-02-15', '2022-02-27', 0);