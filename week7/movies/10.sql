SELECT name FROM people WHERE id in (SELECT person_id FROM directors WHERE directors.movie_id in (SELECT movie_id FROM ratings WHERE rating >= 9.0));
