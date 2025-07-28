-- Keep a log of any SQL queries you execute as you solve the mystery.

-- look at crime scene reports at day of crime, time = 10:15am at Humphrey Street bakery, 3 witnesses interviews
SELECT * FROM crime_scene_reports

-- look at interviews.
-- NOTES: ids 161 Ruth (10 min after check parking lot) 162 Eugene (earlier in the morning thief ATM withdraw on Leggett Street)
-- 163 Raymond (<1 min call, earliest flight out of Fiftyville on 7 29, ticket bought by accomplice)
SELECT * FROM interviews WHERE day = 28 AND month = 7;

-- check bakery security logs 10 min before theft
-- NOTES: possible plates
SELECT * FROM bakery_security_logs WHERE day = 28 AND month = 7;

-- funnel search to check possible license plates
SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND hour = 10 AND minute > 15 AND minute < 25;

-- check atm (account number important)
SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND transaction_type = "withdraw" AND atm_location = "Leggett Street";

-- check phone_calls
SELECT * FROM phone_calls WHERE day = 28 AND month = 7 AND duration < 60;

-- checks first flight of the next day destination city
SELECT city FROM airports WHERE id =
    (SELECT destination_airport_id FROM flights WHERE day = 29 AND month = 7 AND origin_airport_id =
        (SELECT id FROM airports WHERE city = "Fiftyville") ORDER BY hour, minute LIMIT 1);

-- (New York City was the destination)

-- check for the flight id to check for passengers
SELECT id FROM flights WHERE day = 29 AND month = 7 AND origin_airport_id =
        (SELECT id FROM airports WHERE city = "Fiftyville") ORDER BY hour, minute LIMIT 1;

-- get passengers of that flight
SELECT passport_number FROM passengers WHERE flight_id =
    (SELECT id FROM flights WHERE day = 29 AND month = 7 AND origin_airport_id =
        (SELECT id FROM airports WHERE city = "Fiftyville") ORDER BY hour, minute LIMIT 1);

-- cross some informations
SELECT name
    FROM people
    WHERE license_plate IN (SELECT license_plate
        FROM bakery_security_logs
        WHERE day = 28 AND month = 7 AND hour = 10 AND minute > 15 AND minute < 25)
    AND id IN (SELECT person_id
        FROM bank_accounts
        WHERE account_number IN (SELECT account_number
            FROM atm_transactions
            WHERE day = 28 AND month = 7 AND transaction_type = "withdraw" AND atm_location = "Leggett Street"))
    AND passport_number IN (SELECT passport_number
        FROM passengers
        WHERE flight_id = (SELECT id
            FROM flights
            WHERE day = 29 AND month = 7 AND origin_airport_id = (SELECT id
                FROM airports
                WHERE city = "Fiftyville") ORDER BY hour, minute LIMIT 1))
    AND phone_number IN (SELECT caller
        FROM phone_calls
        WHERE day = 28 AND month = 7 AND duration < 60);
-- Bruce was the thief

-- check for who received bruce's call
SELECT name
    FROM people
    WHERE phone_number IN (SELECT receiver
    FROM phone_calls WHERE day = 28 AND month = 7 and DURATION < 60 AND caller = (SELECT phone_number
        FROM people WHERE name = "Bruce"));
-- Robin was the accomplice
