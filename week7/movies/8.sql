SELECT name FROM people WHERE people.id in (SELECT person_id FROM stars WHERE movie_id = (SELECT id FROM movies WHERE title = "Toy Story"));
